﻿#include "framework.h"
#include "axis/user/user.h"
#include "metrowin.h"
#include <shlobj.h>

bool windows_load_dib_from_file(::draw2d::dib * pdib, Windows::Storage::Streams::IRandomAccessStream ^stream, ::aura::application * papp);

#ifdef METROWIN


CLASS_DECL_AURA bool windows_write_dib_to_file(Windows::Storage::Streams::InMemoryRandomAccessStream ^ randomAccessStream, ::draw2d::dib * pdib, ::visual::save_image * psaveimage);


#endif


namespace metrowin
{


   copydesk::copydesk(::aura::application * papp) :
      ::object(papp),
      ::user::copydesk(papp)
   {
   }

   copydesk::~copydesk()
   {
   }


   bool copydesk::_has_filea()
   {

      bool bHasFile = false;

      ::wait(
      Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(
      ::Windows::UI::Core::CoreDispatcherPriority::Normal,
      ref new Windows::UI::Core::DispatchedHandler([&bHasFile, this]()
      {

         ::Windows::ApplicationModel::DataTransfer::DataPackageView ^ view = ::Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();

         if(view == nullptr)
         {

            //iFileCount = 0;
            return;

         }

         if(view->Contains(::Windows::ApplicationModel::DataTransfer::StandardDataFormats::ApplicationLink))
         {

            bHasFile = true;

         }
         //else if(view->Contains("FileDrop"))
         //{

         //   HGLOBAL hglobal;

         //   ::Windows::Storage::Streams::IInputStream ^ stream = (::Windows::Storage::Streams::IInputStream ^):: wait(view->GetDataAsync("FileDrop"));

         //   ::Windows::Storage::Streams::IBuffer ^ buffer = ref new ::Windows::Storage::Streams::Buffer(sizeof(HGLOBAL));

         //   stream->ReadAsync(buffer, sizeof(HGLOBAL), ::Windows::Storage::Streams::InputStreamOptions::None);

         //   memory memory(get_app());

         //   memory.set_os_buffer(buffer);

         //   memcpy(&hglobal, memory.get_data(), sizeof(HGLOBAL));

         //   //iCount = ::DragQueryFile(hglobal , 0xFFFFFFFF, NULL, 0);

         //   //_throw(todo(get_app()));
         //   // assumes true

         //   bHasFile = true;

         //}
         else if(view->Contains(::Windows::ApplicationModel::DataTransfer::StandardDataFormats::StorageItems))
         {

            ::Windows::Foundation::Collections::IVectorView < ::Windows::Storage::IStorageItem ^ > ^ items = ::wait(view->GetStorageItemsAsync());

            bHasFile = true;

         }

      })));

      return bHasFile;


   }


   bool copydesk::_get_filea(::file::patha & patha, e_op & eop)
   {

      bool bHasFile = false;

      ::wait(
      Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(
      ::Windows::UI::Core::CoreDispatcherPriority::Normal,
      ref new Windows::UI::Core::DispatchedHandler([&bHasFile, &patha, this]()
      {

         ::Windows::ApplicationModel::DataTransfer::DataPackageView ^ view = ::Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();

         if (view == nullptr)
         {

            return;

         }

         if (view->Contains(::Windows::ApplicationModel::DataTransfer::StandardDataFormats::ApplicationLink))
         {

            bHasFile = true;

         }
         else if (view->Contains(::Windows::ApplicationModel::DataTransfer::StandardDataFormats::StorageItems))
         {

            ::Windows::Foundation::Collections::IVectorView < ::Windows::Storage::IStorageItem ^ > ^ items = ::wait(view->GetStorageItemsAsync());

            for (int i = 0; i < items->Size; i++)
            {

               patha.add(items->GetAt(i)->Path->Begin());

            }

            bHasFile = true;

         }

      })));

      return bHasFile;

   }


   bool copydesk::_set_filea(const ::file::patha & patha, e_op eop)
   {

#ifdef WINDOWSEX

      ASSERT(m_p->IsWindow());

      strsize iLen = 0;

      for(int i = 0; i < stra.get_size(); i++)
      {
         iLen += ::str::international::utf8_to_unicode_count(stra[i]) + 1;
      }


      HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(DROPFILES) + (iLen + 1) * sizeof(WCHAR));
      LPDROPFILES pDropFiles = (LPDROPFILES) ::GlobalLock(hglbCopy);
      pDropFiles->pFiles = sizeof(DROPFILES);
      pDropFiles->pt.x = pDropFiles->pt.y = 0;
      pDropFiles->fNC = TRUE;
      pDropFiles->fWide = TRUE; // ANSI charset

      ASSERT(m_p->IsWindow());
      LPTSTR lptstrCopy = (char *) pDropFiles;
      lptstrCopy += pDropFiles->pFiles;
      unichar * lpwstrCopy = (unichar *) lptstrCopy;
      for(int i = 0; i < stra.get_size(); i++)
      {
         ASSERT(m_p->IsWindow());
         ::str::international::utf8_to_unicode(lpwstrCopy, ::str::international::utf8_to_unicode_count(stra[i]) + 1, stra[i]);
         ASSERT(m_p->IsWindow());
         lpwstrCopy += (stra[i].get_length() + 1);
      }
      ASSERT(m_p->IsWindow());
      *lpwstrCopy = '\0';    // NULL character
      ASSERT(m_p->IsWindow());
      ::GlobalUnlock(hglbCopy);
      ASSERT(m_p->IsWindow());
      if(!m_p->OpenClipboard())
      {
         ::GlobalFree(hglbCopy);
         return;
      }
      EmptyClipboard();
      SetClipboardData(CF_HDROP, hglbCopy);
      VERIFY(::CloseClipboard());

#else

      _throw(todo(get_app()));

#endif

      return false;

   }


   bool copydesk::initialize()
   {

      if(!::user::copydesk::initialize())
         return false;

      return true;

   }


   bool copydesk::finalize()
   {

      bool bOk;

      bOk = ::user::copydesk::finalize();

      return bOk;

   }


   bool copydesk::_set_plain_text(const string & str)
   {

      auto package = ref new ::Windows::ApplicationModel::DataTransfer::DataPackage;

      if (package == nullptr)
      {

         return false;

      }

      package->RequestedOperation = ::Windows::ApplicationModel::DataTransfer::DataPackageOperation::Copy;

      package->SetText(str);

      ::wait(
      Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(
      ::Windows::UI::Core::CoreDispatcherPriority::Normal,
      ref new Windows::UI::Core::DispatchedHandler([&package, this]()
      {

         ::Windows::ApplicationModel::DataTransfer::Clipboard::SetContent(package);

      })));

      return true;

   }


   bool copydesk::_get_plain_text(string & str)
   {

      bool bOk = false;

      ::wait(
      Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(
      ::Windows::UI::Core::CoreDispatcherPriority::Normal,
      ref new Windows::UI::Core::DispatchedHandler([&str, &bOk, this]()
      {

         auto dataPackage = ::Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();

         if (dataPackage == nullptr)
         {

            return;

         }

         if (!dataPackage->Contains(::Windows::ApplicationModel::DataTransfer::StandardDataFormats::Text))
         {

            return;

         }

         str = ::wait(dataPackage->GetTextAsync())->Begin();

         bOk = true;

      })));

      return bOk;

   }


   bool copydesk::_has_plain_text()
   {

      bool bOk = false;

      ::wait(
      Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(
      ::Windows::UI::Core::CoreDispatcherPriority::Normal,
      ref new Windows::UI::Core::DispatchedHandler([&bOk, this]()
      {

         auto dataPackage = ::Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();

         if (dataPackage == nullptr)
         {

            return;

         }

         if (!dataPackage->Contains(::Windows::ApplicationModel::DataTransfer::StandardDataFormats::Text))
         {

            return;

         }

         bOk = true;

      })));

      return bOk;

   }


   bool copydesk::_has_dib()
   {

      bool bOk = false;

      ::wait(Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(
             Windows::UI::Core::CoreDispatcherPriority::Normal,
             ref new Windows::UI::Core::DispatchedHandler([&bOk]()
      {

         auto dataPackage = ::Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();

         if (dataPackage == nullptr)
         {

            return;

         }

         if (dataPackage->Contains(::Windows::ApplicationModel::DataTransfer::StandardDataFormats::Bitmap))
         {

            bOk = true;

            return;

         }

      })));

      return bOk;

   }

   bool copydesk::_desk_to_dib(::draw2d::dib * pdib)
   {

      bool bOk = true;

      ::wait(Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(
             Windows::UI::Core::CoreDispatcherPriority::Normal,
             ref new Windows::UI::Core::DispatchedHandler([&]()
      {

         auto dataPackage = ::Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();

         if (dataPackage == nullptr)
         {

            bOk = false;

            return;

         }

         if (!dataPackage->Contains(::Windows::ApplicationModel::DataTransfer::StandardDataFormats::Bitmap))
         {

            bOk = false;

            return;

         }

         ::Windows::Storage::Streams::RandomAccessStreamReference ^ ref = ::wait(dataPackage->GetBitmapAsync());

         if (ref == nullptr)
         {

            bOk = false;

            return;

         }

         ::Windows::Storage::Streams::IRandomAccessStreamWithContentType ^ stream = ::wait(ref->OpenReadAsync());

         if (stream == nullptr)
         {


            bOk = false;

            return;

         }

         if (!windows_load_dib_from_file(pdib, stream, get_app()))
         {


            bOk = false;

            return;

         }

      })));

      return bOk;

   }


   bool copydesk::_dib_to_desk(::draw2d::dib * pdib)
   {

      bool bOk = true;

      auto package = ref new ::Windows::ApplicationModel::DataTransfer::DataPackage;

      if (package == nullptr)
      {

         return false;

      }

      Windows::Storage::Streams::InMemoryRandomAccessStream ^ randomAccessStream = ref new Windows::Storage::Streams::InMemoryRandomAccessStream();

      ::visual::save_image saveimage;

      saveimage.m_eformat = ::visual::image::format_png;

      saveimage.m_iQuality = 100;

      if (!windows_write_dib_to_file(randomAccessStream, pdib, &saveimage))
      {

         return false;

      }

      package->RequestedOperation = ::Windows::ApplicationModel::DataTransfer::DataPackageOperation::Copy;

      auto reference = ::Windows::Storage::Streams::RandomAccessStreamReference::CreateFromStream(randomAccessStream);

      package->SetBitmap(reference);

      ::wait(
      Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(
      ::Windows::UI::Core::CoreDispatcherPriority::Normal,
      ref new Windows::UI::Core::DispatchedHandler([&package, this]()
      {

         ::Windows::ApplicationModel::DataTransfer::Clipboard::SetContent(package);

      })));

      return true;

   }


} // namespace metrowin




