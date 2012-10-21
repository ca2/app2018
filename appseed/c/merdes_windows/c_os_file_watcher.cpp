/**
	Copyright (c) 2009 James Wynn (james@jameswynn.com)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/
#include "framework.h"


namespace file_watcher
{
	/// Internal watch data
   class watch_struct
	{
   public:
      
		OVERLAPPED mOverlapped;
		HANDLE mDirHandle;
		BYTE mBuffer[32 * 1024];
		LPARAM lParam;
		DWORD mNotifyFilter;
		bool mStopNow;
		
		id mWatchid;


      ::Windows::Storage::StorageFolder ^    m_folder;
      ::Windows::Storage::Search::StorageItemQueryResult ^ m_queryresult;
      ::Windows::Foundation::EventRegistrationToken m_evtoken;

      file_watcher_impl *                 m_pwatcher;   // should be exactly file_watcher_impl *
      file_watch_listener *               m_plistener;  // should be exactly file_watch_listener *
      vsstring                            m_strDirName;
      id                                  m_id;

      static watch_struct * create_watch(Platform::String ^ strDirectory);

      void ContentsChanged(::Windows::Storage::Search::IStorageQueryResultBase ^ r, ::Platform::Object ^ o)
      {
         ((file_watcher_impl *) m_pwatcher)->handle_action(this, r);
      }

	};


#pragma region Internal Functions

	// forward decl
	bool RefreshWatch(watch_struct * pWatch, bool _clear = false);

	/// Unpacks events and passes them to a user defined callback.
	/*void CALLBACK WatchCallback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
	{
		TCHAR szFile[MAX_PATH];
		PFILE_NOTIFY_INFORMATION pNotify;
		watch_struct* pWatch = (watch_struct*) lpOverlapped;
		size_t offset = 0;

		if(dwNumberOfBytesTransfered == 0)
			return;

		if (dwErrorCode == ERROR_SUCCESS)
		{
			do
			{
				pNotify = (PFILE_NOTIFY_INFORMATION) &pWatch->m_buffer[offset];
				offset += pNotify->NextEntryOffset;

#			if defined(UNICODE)
				{
					lstrcpynW(szFile, pNotify->FileName,
						min(MAX_PATH, pNotify->FileNameLength / sizeof(WCHAR) + 1));
				}
#			else
				{
					int count = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName,
						pNotify->FileNameLength / sizeof(WCHAR),
						szFile, MAX_PATH - 1, NULL, NULL);
					szFile[count] = TEXT('\0');
				}
#			endif

				pWatch->m_pwatcher->handle_action(pWatch, szFile, pNotify->Action);

			} while (pNotify->NextEntryOffset != 0);
		}

		if (!pWatch->m_bStop)
		{
			RefreshWatch(pWatch);
		}
	}

	/// Refreshes the directory monitoring.
	bool RefreshWatch(watch_struct* pWatch, bool _clear)
	{
		return ReadDirectoryChangesW(
			pWatch->m_hDirectory, pWatch->m_buffer, sizeof(pWatch->m_buffer), FALSE,
			pWatch->m_dwNotify, NULL, &pWatch->m_overlapped, _clear ? 0 : WatchCallback) != 0;
	}*/

	/// Stops monitoring a directory.
	void DestroyWatch(watch_struct ^ pwatch)
	{
		if(pwatch != nullptr)
		{
         //delete pWatch;
		}
	}
	/// Starts monitoring a directory.
	watch_struct ^ watch_struct::create_watch(Platform::String ^ strDirectory)
	{
		watch_struct ^ pwatch = ref new watch_struct;


      Windows::Storage::Search::QueryOptions ^ options = ref new Windows::Storage::Search::QueryOptions();

      options->FolderDepth = ::Windows::Storage::Search::FolderDepth::Shallow;
      options->IndexerOption = ::Windows::Storage::Search::IndexerOption::DoNotUseIndexer;

      pwatch->m_folder = m_wait(::Windows::Storage::StorageFolder::GetFolderFromPathAsync(strDirectory));

      pwatch->m_queryresult = pwatch->m_folder->CreateItemQuery();

		if(pwatch->m_queryresult == nullptr)
         return nullptr;

      pwatch->m_evtoken = pwatch->m_queryresult->ContentsChanged += ref new ::Windows::Foundation::TypedEventHandler < ::Windows::Storage::Search::IStorageQueryResultBase ^, 
         ::Platform::Object ^ >(pwatch, &watch_struct::ContentsChanged);

		return pwatch;

	}

#pragma endregion


	os_file_watcher::os_file_watcher()
		: m_idLast(0)
	{
	}

	os_file_watcher::~os_file_watcher()
	{
      watch_map::pair * ppair = m_watchmap.PGetFirstAssoc();
		for(; ppair != NULL; m_watchmap.PGetNextAssoc(ppair))
		{
			DestroyWatch(ppair->m_value);
		}
		m_watchmap.remove_all();
	}

	id os_file_watcher::add_watch(const char * directory, file_watch_listener* watcher)
	{
		id watchid = ++m_idLast;

		//watch_struct ^ pwatch = watch_struct::create_watch(m_str(directory), FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_FILE_NAME);
      watch_struct ^ pwatch = watch_struct::create_watch(m_str(directory));

		if(pwatch == nullptr)
			throw file_not_found_exception(directory);

		pwatch->mWatchid = watchid;
		pwatch->mFileWatcher = this;
		pwatch->mFileWatchListener = watcher;
		pwatch->m_strDirName = directory;
      
		m_watchmap.set_at(watchid, watch);

		return watchid;
	}

	void os_file_watcher::remove_watch(const char * directory)
	{
      watch_map::pair * ppair = m_watchmap.PGetFirstAssoc();
      for(; ppair != NULL; m_watchmap.PGetNextAssoc(ppair))
		{
			if(directory == ppair->m_value->m_strDirName)
			{
				remove_watch(ppair->m_key);
				return;
			}
		}
	}

	void os_file_watcher::remove_watch(id watchid)
	{
      watch_map::pair * ppair = m_watchmap.PLookup(watchid);

		if(ppair == NULL)
			return;

      watch_struct* pwatch = ppair->m_value;
      m_watchmap.remove_key(ppair->m_key);

		DestroyWatch(pwatch);
	}

   vsstring os_file_watcher::watch_path(id watchid)
   {
      return m_watchmap[watchid]->m_strDirName;
   }

	void os_file_watcher::update()
	{
		MsgWaitForMultipleObjectsEx(0, NULL, 0, QS_ALLINPUT, MWMO_ALERTABLE);
	}

	void os_file_watcher::handleAction(watch_struct* watch, const String& filename, unsigned long action)
	{
		Action fwAction;

		switch(action)
		{
		case FILE_ACTION_RENAMED_NEW_NAME:
		case FILE_ACTION_ADDED:
			fwAction = Actions::Add;
			break;
		case FILE_ACTION_RENAMED_OLD_NAME:
		case FILE_ACTION_REMOVED:
			fwAction = Actions::Delete;
			break;
		case FILE_ACTION_MODIFIED:
			fwAction = Actions::Modified;
			break;
		};

		watch->mFileWatchListener->handleFileAction(watch->mWatchid, watch->m_strDirName, filename, fwAction);
	}

} // namespace file_watcher




