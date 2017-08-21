#include "framework.h"
#include "axis/user/user.h"
#include "metrowin.h"
#include <shlobj.h>

bool windows_load_dib_from_file(::draw2d::dib * pdib, Windows::Storage::Streams::IRandomAccessStream ^stream, ::aura::application * papp);


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

   int copydesk::get_file_count()
   {

      int iFileCount = 0;

      ::wait(
         Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(
         ::Windows::UI::Core::CoreDispatcherPriority::Normal,
         ref new Windows::UI::Core::DispatchedHandler([&iFileCount, this]()
      {
         ::Windows::ApplicationModel::DataTransfer::DataPackageView ^ view = ::Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();

         if(view == nullptr)
         {
            iFileCount = 0;
            return;
         }

         if(view->Contains(::Windows::ApplicationModel::DataTransfer::StandardDataFormats::ApplicationLink))
         {
            iFileCount = 1;
         }
         else if(view->Contains("FileDrop"))
         {

            HGLOBAL hglobal;

            ::Windows::Storage::Streams::IInputStream ^ stream = (::Windows::Storage::Streams::IInputStream ^):: wait(view->GetDataAsync("FileDrop"));

            ::Windows::Storage::Streams::IBuffer ^ buffer = ref new ::Windows::Storage::Streams::Buffer(sizeof(HGLOBAL));

            stream->ReadAsync(buffer, sizeof(HGLOBAL), ::Windows::Storage::Streams::InputStreamOptions::None);

            memory memory(get_app());

            memory.set_os_buffer(buffer);

            memcpy(&hglobal, memory.get_data(), sizeof(HGLOBAL));

            //iCount = ::DragQueryFile(hglobal , 0xFFFFFFFF, NULL, 0);

            throw todo(get_app());

         }
         else if(view->Contains(::Windows::ApplicationModel::DataTransfer::StandardDataFormats::StorageItems))
         {

            ::Windows::Foundation::Collections::IVectorView < ::Windows::Storage::IStorageItem ^ > ^ items = ::wait(view->GetStorageItemsAsync());

            iFileCount = items->Size;

         }
         else
         {

            iFileCount = 0;

         }

      })));

      return iFileCount;


   }


   void copydesk::get_filea(::file::patha & patha)
   {
      int iCount = get_file_count();
      if(iCount <= 0)
         return;
//      if(!m_p->OpenClipboard())
  //       return;
#ifdef WINDOWSEX

      HDROP hdrop = (HDROP) ::GetClipboardData(CF_HDROP);
      string str;
      for(int i = 0; i < iCount; i++)
      {
         UINT uiLen = ::DragQueryFileW(hdrop, i, NULL, 0);
         unichar * lpwsz = (unichar *) malloc(sizeof(unichar) * (uiLen + 1));
         ::DragQueryFileW(hdrop, i, lpwsz, uiLen + 1);
         stra.add(::str::international::unicode_to_utf8(lpwsz));
         free(lpwsz);
      }
      ::CloseClipboard();
#else

      throw todo(get_app());

#endif

   }

   void copydesk::set_filea(const ::file::patha & patha)
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
      throw todo(get_app());
#endif
   }



   bool copydesk::initialize()
   {

      if(!::user::copydesk::initialize())
         return false;

      /*if(!m_p->CreateEx(0, System.RegisterWndClass(0), NULL, 0, rect(0, 0, 0, 0), NULL, id()))
         return false;*/

      return true;

   }


   bool copydesk::finalize()
   {

      bool bOk;

      bOk = ::user::copydesk::finalize();

/*      if(::user::window_sp::is_set() && ::user::window_sp::m_p->IsWindow())
      {
         bOk = ::user::window_sp::m_p->DestroyWindow() != FALSE;
      }
      else
      {
         bOk = false;
      }*/

      return bOk;

   }

   void copydesk::set_plain_text(const char * psz)
   {
#ifdef WINDOWSEX
      ASSERT(m_p->IsWindow());
   //   int iLen = 0;

      string str;
      str = ::str::international::utf8_to_unicode(psz);



      ASSERT(m_p->IsWindow());
      if(!m_p->OpenClipboard())
      {
         return;
      }
      EmptyClipboard();


      count iCount = ::str::international::utf8_to_unicode_count(str) + 1;
      HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, iCount * sizeof(WCHAR));
      unichar * lpwstrCopy  = (unichar *) ::GlobalLock(hglbCopy);
      ::str::international::utf8_to_unicode(lpwstrCopy, iCount, str);
      ::GlobalUnlock(hglbCopy);

      HGLOBAL hglbCopy2 = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(CHAR) * (strlen(psz) + 1));
      char * lpstrCopy  = (char *) ::GlobalLock(hglbCopy2);
      strcpy(lpstrCopy, psz);
      ::GlobalUnlock(hglbCopy2);


      SetClipboardData(CF_UNICODETEXT, hglbCopy);
      SetClipboardData(CF_TEXT, hglbCopy2);
      VERIFY(::CloseClipboard());
#else
      throw todo(get_app());
#endif

   }


   string copydesk::get_plain_text()
   {
      
      auto dataPackage = ::Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();

      if(dataPackage == nullptr)
      {

         return "";

      }

      if(!dataPackage->Contains(::Windows::ApplicationModel::DataTransfer::StandardDataFormats::Text))
      {

         return "";

      }

      return ::wait(dataPackage->GetTextAsync());

   }


   bool copydesk::desk_to_dib(::draw2d::dib * pdib)
   {

      auto dataPackage = ::Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();

      if (dataPackage == nullptr)
      {

         return false;

      }

      if (!dataPackage->Contains(::Windows::ApplicationModel::DataTransfer::StandardDataFormats::Bitmap))
      {

         return false;

      }

      ::Windows::Storage::Streams::RandomAccessStreamReference ^ ref = ::wait(dataPackage->GetBitmapAsync());

      if (ref == nullptr)
      {

         return false;

      }

      ::Windows::Storage::Streams::IRandomAccessStreamWithContentType ^ stream = ::wait(ref->OpenReadAsync());


      if (stream == nullptr)
      {

         return false;

      }

      if (!windows_load_dib_from_file(pdib, stream, get_app()))
      {

         return false;

      }

      return true;

#ifdef WINDOWSEX
      if(!m_p->OpenClipboard())
         return false;
      bool bOk = false;
      HBITMAP hbitmap = (HBITMAP) ::GetClipboardData(CF_BITMAP);
      try
      {
         ::draw2d::bitmap_sp bitmap(get_app());
         bitmap->Attach(hbitmap);
         //HDC hdc = ::CreateCompatibleDC(NULL);
         //::draw2d::graphics_sp g(get_app());
         //g->Attach(hdc);
         //::draw2d::graphics * pgraphics = Application.graphics_from_os_data(hdc);
         //g->SelectObject(hbitmap);
       //  BITMAP bm;
         //::GetObjectA(hbitmap, sizeof(bm), &bm);
         //if(!pdib->create(bm.bmWidth, bm.bmHeight))
           // return false;
         ::draw2d::graphics_sp g(get_app());
         g->SelectObject(bitmap);
         size sz = bitmap->GetBitmapDimension();
         if(pdib->create(sz))
         {
            bOk = pdib->get_graphics()->BitBlt(0, 0, sz.cx, sz.cy, g, 0, 0, SRCCOPY) != FALSE;
         }
      }
      catch(...)
      {
      }
      ::DeleteObject((HGDIOBJ) hbitmap);
      //::DeleteDC(hdc);
      ::CloseClipboard();
      return bOk;
#else
      throw todo(get_app());
#endif
   }


} // namespace metrowin



