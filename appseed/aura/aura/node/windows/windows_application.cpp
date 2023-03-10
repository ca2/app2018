#include "framework.h"


namespace aura
{

   bool application::impl_process_init()
   {

      return true;

   }

   bool application::impl_init1()
   {

      //set_run();

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

      set_os_data(NULL);

      //int32_t iRet = ::aura::application::term_instance();

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

   //
   //
   //   application::application(::aura::application * papp) :
   //      object(papp)
   //   {
   //
   //      //m_pthreadimpl.alloc(allocer());
   //
   //      //m_pthreadimpl->m_pthread = this;
   //
   //      m_psystem                    =  papp->m_papp->m_psystem;
   //
   //      m_atomApp = m_atomSystemTopic    = NULL;
   //
   //   }


   //application::~application()
   //{

   //}


   //void application::_001OnFileNew()
   //{

   //}


   //::user::document *  application::_001OpenDocumentFile(var varFile)
   //{

   //   return NULL;

   //}


   void application::_001EnableShellOpen()
   {

      //ASSERT(m_atomApp == NULL && m_atomSystemTopic == NULL); // do once

      //m_atomApp            = ::GlobalAddAtomW(::str::international::utf8_to_unicode(m_strAppName));

      //m_atomSystemTopic    = ::GlobalAddAtomW(L"system");

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

      unichar lpszModuleFilePath[MAX_PATH + 1];
      GetModuleFileNameW(NULL, lpszModuleFilePath, MAX_PATH + 1);

      DWORD dw;

      DWORD dwResSize = GetFileVersionInfoSizeW(
                        lpszModuleFilePath,
                        &dw);


      if (dwResSize > 0)
      {
         LPVOID lpdata = new BYTE[dwResSize];
         if (GetFileVersionInfoW(
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
            for (int32_t i = 0; i < 1; i++)
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
                             (LPTSTR)(const char *)strKey,
                             (LPVOID *)&lpsz,
                             &uiSize);


               string strVersion(lpsz, uiSize);

               return strVersion;
            }
         }
         delete[](LPBYTE) lpdata;
      }


      return "";

   }


   //   bool application::Ex2OnAppInstall()
   //   {
   //#ifdef IMPLEMENT_VISTA_TOOLS
   //      if(VistaTools::IsVista())
   //      {
   //         if(VistaTools::IsElevated() != S_OK)
   //         {
   //            TRACE0("Error! Installing application ( : (un)install run parameter ) without Elevation (required since Vista Windows version)");
   //            return false;
   //         }
   //      }
   //#endif
   //      return true;
   //   }
   //
   //
   //   bool application::Ex2OnAppUninstall()
   //   {
   //      if(VistaTools::IsVista())
   //      {
   //         if(VistaTools::IsElevated() != S_OK)
   //         {
   //            TRACE0("Error! Installing application ( : (un)install run parameter ) without Elevation (required since Vista Windows version)");
   //            return false;
   //         }
   //      }
   //      return true;
   //   }


   //void application::TermThread(HINSTANCE hInstTerm)
   //{

   //}






   //void application::BaseSetCurrentHandles()
   //{

   //   //string strExeName;
   //   //strExeName = System.get_module_title();
   //   dappy(string(typeid(*this).name()) + " : got module title windows application : " + ::str::from(m_iErrorCode));

   //   __init_thread();

   //}


   //::user::primitive * application::FindWindow(const char * lpszClassName,const char * lpszWindowName)
   //{

   //   return NULL;

   //}


   //::user::primitive * application::FindWindowEx(oswindow oswindowParent,oswindow oswindowChildAfter,const char * lpszClass,const char * lpszWindow)
   //{

   //   return NULL;

   //}


   void application::get_time(struct timeval *p)
   {
#ifdef _WIN32
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

   void application::set_env_var(const string & var, const string & value)
   {
#if (defined(SOLARIS8) || defined(SOLARIS))
      {
         static stdmap<string, char *> vmap;
         if (vmap.find(var) != vmap.end())
         {
            delete[] vmap[var];
         }
         vmap[var] = new char[var.get_length() + 1 + value.get_length() + 1];
         sprintf(vmap[var], "%s=%s", var, value);
         putenv(vmap[var]);
      }
#elif defined _WIN32
      {
         string slask = var + "=" + value;
         _putenv((const char *)slask);
      }
#else
      setenv(var, value, 1);
#endif
   }

   uint32_t application::get_thread_id()
   {
      return ::GetCurrentThreadId();
   }



   bool application::process_command(::command::command * pcommand)
   {

      // m_pmaininitdata = pauradata;

      if (m_pcommand != NULL && m_papp->is_system())
      {

         ::command::command * pdata = m_pcommand.cast < ::command::command >();
         if (!m_papp->is_system())
            return false;

         ASSERT(System.m_pappcore->m_pmaindata->m_hPrevInstance == NULL);

         HINSTANCE hinstance = System.m_pappcore->m_pmaindata->m_hinstance;

         UINT nCmdShow = System.m_pappcore->m_pmaindata->m_nCmdShow;

         // handle critical errors and avoid Windows message boxes
         SetErrorMode(SetErrorMode(0) | SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);

         m_hinstance = hinstance;
         m_papp->m_hinstance = hinstance;
         System.m_nCmdShow = nCmdShow;
         m_papp->SetCurrentHandles();

      }

      return true;

   }



   void application::ShowWaitCursor(bool bShow)
   {

      if (bShow)
      {

         HCURSOR hcursorWait = ::LoadCursor(NULL, IDC_WAIT);

         HCURSOR hcursorPrevious = ::SetCursor(hcursorWait);

         //     if(hcursorPrevious != hcursorWait)
         //         m_hcurWaitCursorRestore = hcursorPrevious;

      }
      else
      {

         //         ::SetCursor(m_hcurWaitCursorRestore);
      }

   }


   string application::veriwell_multimedia_music_midi_get_default_library_name()
   {
      return "music_midi_mmsystem";
   }

   string application::multimedia_audio_mixer_get_default_library_name()
   {
      return "audio_mixer_mmsystem";
   }

   string application::multimedia_audio_get_default_library_name()
   {

      string str;

      if (file_exists_dup(::dir::system() / "config\\system\\audio.txt"))
      {

         str = file_as_string_dup(::dir::system() / "config\\system\\audio.txt");

      }
      else
      {

         ::file::path strPath;

         strPath = ::dir::appdata() / "audio.txt";

         str = file_as_string_dup(strPath);

      }

      if (str.has_char())
         return "audio_" + str;
      else
         return "audio_mmsystem";

   }

   string application::draw2d_get_default_library_name()
   {

      string str;

      if (file_exists_dup(::dir::system() / "config\\system\\draw2d.txt"))
      {

         str = file_as_string_dup(::dir::system() / "config\\system\\draw2d.txt");

      }
      else
      {

         ::file::path strPath;

         strPath = ::dir::appdata() / "draw2d.txt";

         str = file_as_string_dup(strPath);

      }

      if (str.has_char())
         return "draw2d_" + str;
      else
         return "draw2d_gdiplus";
   }


} // namespace windows





//
//void __node_init_main_data(::aura::application * papp, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
//{
//
//
//   ::comm::command * pmaininitdata = new ::windows::command;
//
//
//   pmaininitdata->m_hInstance = hInstance;
//   pmaininitdata->m_hPrevInstance = hPrevInstance;
//   pmaininitdata->m_strCommandLine = ::str::international::unicode_to_utf8(::GetCommandLineW());
//   pmaininitdata->m_nCmdShow = nCmdShow;
//
//   papp->startup_command(pmaininitdata);
//
//}
//

