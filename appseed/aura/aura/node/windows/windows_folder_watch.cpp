#include "framework.h"


string get_volume_path(const char * psz)
{
   WCHAR wsz[4096];
   if (!GetVolumePathNameW(::str::international::utf8_to_unicode(psz), wsz, sizeof(wsz) / sizeof(wsz[0])))
   {
      return "";
   }
   return wsz;
}

//namespace windows
//{
//
//
//   folder_watch::folder_watch(::aura::application * papp) :
//      object(papp),
//      thread(papp)
//   {
//
//   }
//
//
//   bool folder_watch::watch(const char * pszPath)
//   {
//
//      if(!::file::folder_watch::watch(pszPath))
//         return false;
//
//      if(!begin())
//         return false;
//
//      return true;
//
//   }
//
//
//   void folder_watch::run() // thread procedure
//   {
//      HANDLE hDirectory = ::CreateFileW(::str::international::utf8_to_unicode(get_volume_path(m_strPath)),
//                                        FILE_LIST_DIRECTORY,
//                                        FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
//                                        NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
//
//      if(INVALID_HANDLE_VALUE == hDirectory)
//      {
//         m_error.set_last_error();
//         return;
//      }
//
//      DWORD dwBytesReturned = 0;
//      const DWORD dwBuffLength = 4096;
//      BYTE buffer[dwBuffLength];
//      WCHAR wchFileName[dwBuffLength];
//
//
//      while(::ReadDirectoryChangesW(hDirectory, buffer, dwBuffLength, TRUE,
//                                    FILE_NOTIFY_CHANGE_FILE_NAME |
//                                    FILE_NOTIFY_CHANGE_LAST_WRITE |
//                                    FILE_NOTIFY_CHANGE_CREATION, &dwBytesReturned,
//                                    NULL, NULL))
//      {
//         DWORD dwNextEntryOffset = 0;
//         PFILE_NOTIFY_INFORMATION pfni = NULL;
//         do
//         {
//            pfni = (PFILE_NOTIFY_INFORMATION)(buffer + dwNextEntryOffset);
//            memcpy(wchFileName, pfni->FileName, pfni->FileNameLength);
//            wchFileName[pfni->FileNameLength / sizeof(WCHAR)] = L'\0';
//            e_action eaction = translate_os_action(pfni->Action);
//            //if(eaction != action_unexpected)
//            {
//               on_file_action(eaction, ::str::international::unicode_to_utf8(wchFileName));
//            }
//            dwNextEntryOffset += pfni->NextEntryOffset; // next please!
//         }
//         while(pfni->NextEntryOffset != 0);
//      }
//      ::CloseHandle(hDirectory);
//      //return 0;
//   }
//
//   folder_watch::e_action folder_watch::translate_os_action(int32_t iAction)
//   {
//      switch(iAction)
//      {
//      case FILE_ACTION_ADDED:
//         // The file was added to the directory.
//         return action_added;
//      case FILE_ACTION_MODIFIED:
//         // The file was changed at the directory.
//         return action_modified;
//      case FILE_ACTION_REMOVED:
//         // The file was removed from the directory.
//         return action_removed;
//      case FILE_ACTION_RENAMED_OLD_NAME:
//         // The file was renamed and this is the old name.
//         return action_renamed_old_name;
//      case FILE_ACTION_RENAMED_NEW_NAME:
//         // The file was renamed and this is the new name.
//         return action_renamed_new_name;
//      // ...
//      default:
//         return action_unexpected;
//      }
//   }
//
//
//} // namespace windows