﻿#include "framework.h"
//#include "metrowin.h"


//extern thread_local_storage * gen_ThreadData;


//namespace metrowin
namespace aura
{


   //application::application(::aura::application * papp) :
   //   ::object(papp)
   //{

   //   m_pthreadimpl.alloc(allocer());
   //   m_pthreadimpl->m_pthread = this;

   //   m_psystem = papp->m_psystem;

   //   m_atomApp = m_atomSystemTopic = NULL;

   //}

   //application::~application()
   //{

   //}


   //void application::_001OnFileNew()
   //{
   //   m_pimpl->_001OnFileNew(NULL);
   //}

   //::user::document * application::_001OpenDocumentFile(var varFile)
   //{
   //   return m_pimpl->_001OpenDocumentFile(varFile);
   //}

   void application::_001EnableShellOpen()
   {
      //ASSERT(m_atomApp == NULL && m_atomSystemTopic == NULL); // do once
      /*
            m_atomApp            = ::GlobalAddAtomW(::str::international::utf8_to_unicode(m_strAppName));
            m_atomSystemTopic    = ::GlobalAddAtomW(L"system");*/
   }

   //bool application::_001OnDDECommand(const char * lpcsz)
   //{
   //   UNREFERENCED_PARAMETER(lpcsz);
   //   return FALSE;
   //}

   //HINSTANCE application::GetHinstance()
   //{
   //   return NULL;
   //}

   string application::get_version()
   {
#ifdef WINDOWSEX
      char lpszModuleFilePath[MAX_PATH + 1];
      GetModuleFileName(NULL, lpszModuleFilePath, MAX_PATH + 1);

      DWORD dw;

      DWORD dwResSize = GetFileVersionInfoSize(
                        lpszModuleFilePath,
                        &dw);


      if(dwResSize > 0)
      {
         LPVOID lpdata = new BYTE[dwResSize];
         if(GetFileVersionInfo(
               lpszModuleFilePath,
               0,
               dwResSize,
               lpdata))
         {
            UINT cbTranslate;
            struct LANGANDCODEPAGE
            {
               WORD wLanguage;
               WORD wCodePage;
            } *lpTranslate;

            // read the list of languages and code pages.

            VerQueryValue(lpdata,
                          TEXT("\\VarFileInfo\\Translation"),
                          (LPVOID*)&lpTranslate,
                          &cbTranslate);

            string strKey;
            //for( i=0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ )
            for(int i=0; i < 1; i++ )
            {
               LPTSTR lpsz;
               UINT uiSize;

               strKey.Format(
               TEXT("\\StringFileInfo\\%04x%04x\\FileDescription"),
               lpTranslate[i].wLanguage,
               lpTranslate[i].wCodePage);




               strKey.Format(
               TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"),
               lpTranslate[i].wLanguage,
               lpTranslate[i].wCodePage);

               // Retrieve file description for language and code page "i".
               VerQueryValue(lpdata,
                             (LPTSTR) (const char *) strKey,
                             (LPVOID *)&lpsz,
                             &uiSize);


               string strVersion(lpsz, uiSize);

               return strVersion;
            }
         }
         delete [] (LPBYTE) lpdata;
      }


      return "";
#else
      TRACE("returning hard coded default 1.0 version for metrowin::application::get_version");
      return "1.0";
#endif

   }



   bool application::impl_process_init()
   {

      return true;
   }

   bool application::impl_init1()
   {

      return true;

   }

   bool application::impl_init2()
   {
      return true;
   }

   bool application::impl_init3()
   {
      return true;
   }

   // thread termination
   void application::impl_process_term() // default will 'delete this'
   {

      // avoid calling CloseHandle() on our own thread handle
      // during the thread destructor
      set_os_data(NULL);
      //int32_t iRet = ::aura::application::term_instance();

      //smart_pointer < application_base >::destroy();

      //return iRet;
      //return 0;

   }

   void application::impl_term3()
   {

   }

   void application::impl_term2()
   {

   }


   void application::impl_term1()
   {

   }

   /*
      // Advanced: exception handling
      LRESULT application::ProcessWndProcException(::exception::aura* e, const MSG* pMsg)
      {
         return   ::metrowin::thread::ProcessWndProcException(e, pMsg);
      }


      // Advanced: handling messages sent to message filter hook
      bool application::ProcessMessageFilter(int code, LPMSG lpMsg)
      {
         return  ::metrowin::thread::ProcessMessageFilter(code, lpMsg);
      }


      // Advanced: access to GetMainWnd()
      ::user::interaction* application::GetMainWnd()
      {
         return ::metrowin::thread::GetMainWnd();
      }



      void application::assert_valid() const
      {
         ::metrowin::thread::assert_valid();
      }

      void application::dump(dump_context & dumpcontext) const
      {
         ::metrowin::thread::dump(dumpcontext);
      }


      void application::CommonConstruct()
      {
         ::metrowin::thread::CommonConstruct();
      }
      void application::Delete()
      {
         ::metrowin::thread::Delete();
      }
      // 'delete this' only if m_bAutoDelete == TRUE

      bool application::DispatchThreadMessageEx(MSG* msg)  // helper
      {
         return ::metrowin::thread::DispatchThreadMessageEx(msg);
      }*/

   /*   ::draw2d::graphics * application::graphics_from_os_data(void * pdata)
      {
         return ::metrowin::graphics::from_handle((HDC) pdata);
      }*/


   /*void application::BaseSetCurrentHandles()
   {

      set_os_data(::get_current_thread());
      set_os_int(::GetCurrentThreadId());


   }
   */


   void application::get_time(struct timeval *p)
   {
#ifdef WINDOWS
      FILETIME ft; // Contains a 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC).
      GetSystemTimeAsFileTime(&ft);
      uint64_t tt;
      memcpy(&tt, &ft, sizeof(tt));
      tt /= 10; // make it usecs
      p->tv_sec = (long)tt / 1000000;
      p->tv_usec = (long)tt % 1000000;
#else
      gettimeofday(p, NULL);
#endif
   }

   void application::set_env_var(const string & var,const string & value)
   {
#if (defined(SOLARIS8) || defined(SOLARIS))
      {
         static std::map<string, char *> vmap;
         if (vmap.find(var) != vmap.end())
         {
            delete[] vmap[var];
         }
         vmap[var] = new char[var.get_length() + 1 + value.get_length() + 1];
         sprintf(vmap[var], "%s=%s", var, value);
         putenv( vmap[var] );
      }
#elif defined WINDOWSEX
      {
         string slask = var + "=" + value;
         _putenv( (const char *)slask);
      }
#elif defined METROWIN


#else
      setenv(var, value, 1);
#endif
   }


   uint32_t application::get_thread_id()
   {

      return ::GetCurrentThreadId();

   }


   bool application::process_command(::command::command * pdata)
   {

      m_pcommand = pdata;

      SetCurrentHandles();

      __init_thread();

      return true;

   }





   void application::ShowWaitCursor(bool bShow)
   {
   }


   string application::draw2d_get_default_library_name()
   {
      return "draw2d_direct2d";
   }

   string application::veriwell_multimedia_music_midi_get_default_library_name()
   {

      return "music_midi_winrt";
   }



   string application::multimedia_audio_mixer_get_default_library_name()
   {

      return "";

   }



   string application::multimedia_audio_get_default_library_name()
   {

      return "audio_xaudio";

   }


   bool application::os_on_start_application()
   {

      return true;

   }

} // namespace metrowin



















