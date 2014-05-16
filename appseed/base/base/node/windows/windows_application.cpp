#include "framework.h"


namespace windows
{


   application::application(sp(::base::application) papp) :
      element(papp)
   {

      ::thread::m_p.create(allocer());

      ::thread::m_p->m_p = this;

      NODE_THREAD(::thread::m_p.m_p)->m_pAppThread = this;

      m_pbasesystem                    =  papp->m_pbaseapp->m_pbasesystem;

      //m_pfilemanager                   = NULL;
      //m_pszHelpFilePath                = NULL;
      //m_pszProfileName                 = NULL;
      //m_pszRegistryKey                 = NULL;
      //m_pdocmanager                    = NULL;
      m_atomApp = m_atomSystemTopic    = NULL;
      m_hcurWaitCursorRestore          = NULL;

      //m_hDevMode                       = NULL;
      //m_hDevNames                      = NULL;
      //m_nNumPreviewPages               = 0;     // not specified (defaults to 1)

      //m_bHelpMode                      = FALSE;
      //m_nSafetyPoolSize                = 512;        // default size

      shell::theWindowsShell.Initialize();

   }


   application::~application()
   {

   }


   void application::_001OnFileNew()
   {

      //smart_pointer < ::base::application >::m_p->_001OnFileNew(NULL);

   }


   sp(::user::object) application::_001OpenDocumentFile(var varFile)
   {

      //return smart_pointer < ::base::application >::m_p->_001OpenDocumentFile(varFile);

      return NULL;

   }


   void application::_001EnableShellOpen()
   {

      ASSERT(m_atomApp == NULL && m_atomSystemTopic == NULL); // do once

      m_atomApp            = ::GlobalAddAtomW(::str::international::utf8_to_unicode(m_strAppName));

      m_atomSystemTopic    = ::GlobalAddAtomW(L"system");

   }


   bool application::_001OnDDECommand(const char * lpcsz)
   {

      UNREFERENCED_PARAMETER(lpcsz);

      return FALSE;

   }


   HINSTANCE application::GetHinstance()
   {

      return NULL;

   }

   string application::get_version()
   {

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
            struct LANGANDCODEPAGE {
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
            for(int32_t i=0; i < 1; i++ )
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

   }


   bool application::Ex2OnAppInstall()
   {
#ifdef IMPLEMENT_VISTA_TOOLS
      if(VistaTools::IsVista())
      {
         if(VistaTools::IsElevated() != S_OK)
         {
            TRACE0("Error! Installing application ( : (un)install run parameter ) without Elevation (required since Vista Windows version)");
            return false;
         }
      }
#endif 
      return true;
   }


   bool application::Ex2OnAppUninstall()
   {
      if(VistaTools::IsVista())
      {
         if(VistaTools::IsElevated() != S_OK)
         {
            TRACE0("Error! Installing application ( : (un)install run parameter ) without Elevation (required since Vista Windows version)");
            return false;
         }
      }
      return true;
   }

   void application::LockTempMaps()
   {
      NODE_THREAD(thread::m_p.m_p)->LockTempMaps();
   }

   bool application::UnlockTempMaps(bool bDeleteTemp)
   {
      return NODE_THREAD(thread::m_p.m_p)->UnlockTempMaps(bDeleteTemp);
   }


   void application::TermThread(HINSTANCE hInstTerm)
   {
/*      try
      {
   #ifdef DEBUG
         // check for missing ::core::LockTempMap calls
         if (__get_module_thread_state()->m_pCurrentWinThread->m_nTempMapLock != 0)
         {
            TRACE(::core::trace::category_AppMsg, 0, "Warning: Temp map lock ::count non-zero (%ld).\n",
               __get_module_thread_state()->m_pCurrentWinThread->m_nTempMapLock);
         }
   #endif
         ::core::LockTempMaps(smart_pointer < ::base::application >::m_p);
         ::core::UnlockTempMaps(smart_pointer < ::base::application >::m_p, -1);
      }
      catch( ::exception::base* e )
      {
         e->Delete();
      }*/


   }




   // application
   HCURSOR application::LoadCursor(const char * lpszResourceName) const
   { 
      return NULL;
   }

   HCURSOR application::LoadCursor(UINT nIDResource) const
   { 
      return NULL;
   }

   HCURSOR application::LoadStandardCursor(const char * lpszCursorName) const
   { 
      return ::LoadCursor(NULL, lpszCursorName);
   }

   HCURSOR application::LoadOEMCursor(UINT nIDCursor) const
   { 
   
      return ::LoadCursor(NULL, MAKEINTRESOURCE(nIDCursor));
   
   }

   HICON application::LoadIcon(const char * lpszResourceName) const
   { 
      return NULL;
   }

   HICON application::LoadIcon(UINT nIDResource) const
   { 
      return NULL;
   }

   HICON application::LoadStandardIcon(const char * lpszIconName) const
   { 
      return ::LoadIcon(NULL, lpszIconName);
   }
   
   HICON application::LoadOEMIcon(UINT nIDIcon) const
   { 
      return ::LoadIcon(NULL, MAKEINTRESOURCE(nIDIcon));
   }





   /*void application::construct(__THREADPROC pfnThreadProc, LPVOID pParam)
   {
      ::windows::thread::construct(pfnThreadProc, pParam);
   }

   void * application::get_os_data()
   {
      return ::windows::thread::get_os_data();
   }

   int_ptr application::get_os_int()
   {
      return ::windows::thread::get_os_int();
   }


   bool application::begin(int32_t nPriority, UINT nStackSize,
                           DWORD dwCreateFlags, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
   {
      return ::windows::thread::begin(nPriority, nStackSize, dwCreateFlags, lpSecurityAttrs);
   }

   bool application::create_thread(DWORD dwCreateFlags, UINT nStackSize,
                                  LPSECURITY_ATTRIBUTES lpSecurityAttrs)
   {
      return ::windows::thread::create_thread(dwCreateFlags, nStackSize, lpSecurityAttrs);
   }



   int32_t application::GetThreadPriority()
   {
      return ::windows::thread::GetThreadPriority();
   }
   bool application::SetThreadPriority(int32_t nPriority)
   {
      return ::windows::thread::SetThreadPriority(nPriority);
   }

   // Operations
   DWORD application::SuspendThread()
   {
      return ::windows::thread::SuspendThread();
   }
   DWORD application::ResumeThread()
   {
      return ::windows::thread::ResumeThread();
   }
   bool application::post_thread_message(UINT message, WPARAM wParam, LPARAM lParam)
   {
      return ::windows::thread::post_thread_message(message, wParam, lParam);
   }
   bool application::post_message(sp(::user::interaction) pui, UINT message, WPARAM wParam, LPARAM lParam)
   {
      return ::windows::thread::post_message(pui, message, wParam, lParam);
   }

   bool application::PreInitInstance()
   {
      return ::windows::thread::PreInitInstance();
   }

   // called when occurs an standard_exception exception in run
   // return true to call run again
   bool application::on_run_exception(::exception::exception &)
   {
      return ::windows::thread::on_run_exception(e);
   }

   // Overridables
   // thread initialization
   bool application::initialize_instance()
   {
      return ::windows::thread::initialize_instance();
   }

   message::e_prototype application::GetMessagePrototype(UINT uiMessage, UINT uiCode)
   {
      return ::windows::thread::GetMessagePrototype(uiMessage, uiCode);
   }

   // running and idle processing
   int32_t application::run()
   {
      return ::windows::thread::run();
   }
   bool application::pre_translate_message(signal_details * pobj)
   {
      return ::windows::thread::pre_translate_message(pMsg);
   }
   bool application::pump_message()    // low level message pump
   {
      return ::windows::thread::pump_message();
   }
   bool application::on_idle(LONG lCount) // return TRUE if more idle processing
   {
      return ::windows::thread::on_idle(lCount);
   }
   bool application::is_idle_message(MSG* pMsg)  // checks for special messages
   {
      return ::windows::thread::is_idle_message(pMsg);
   }
*/
   bool application::process_initialize()
   {
      if(m_pimpl->is_system())
      {
         //if(__get_module_state()->m_pmapHWND == NULL)
         //{
         //   __get_module_state()->m_pmapHWND = new oswindow_map(this);
         //   __get_module_state()->m_pmutexoswindow_ = new mutex(this);
         //}
/*         if(__get_module_state()->m_pmapHDC == NULL)
         {
            __get_module_state()->m_pmapHDC = new hdc_map;
         }
         if(__get_module_state()->m_pmapHGDIOBJ == NULL)
         {
            __get_module_state()->m_pmapHGDIOBJ = new hgdiobj_map;
         }*/
/*         if(__get_module_state()->m_pmapHMENU == NULL)
         {
            __get_module_state()->m_pmapHMENU = new hmenu_map;
         }*/
      }


      return true;
   }

   bool application::initialize1()
   {

      NODE_THREAD(thread::m_p.m_p)->set_run();

      return true;

   }

   bool application::initialize2()
   {
      return true;
   }

   bool application::initialize3()
   {
      return true;
   }

   // thread termination
   int32_t application::exit_instance() // default will 'delete this'
   {

      // avoid calling CloseHandle() on our own thread handle
      // during the thread destructor
      thread::m_p->set_os_data(NULL);

      NODE_THREAD(thread::m_p.m_p)->m_bRun = false;

      int32_t iRet = ::base::application::exit_instance();

      //smart_pointer < ::base::application >::destroy();



      return iRet;
   }
/*
   // Advanced: exception handling
   LRESULT application::ProcessWndProcException(::exception::base* e, const MSG* pMsg)
   {
      return   ::windows::thread::ProcessWndProcException(e, pMsg);
   }


   // Advanced: handling messages sent to message filter hook
   bool application::ProcessMessageFilter(int32_t code, LPMSG lpMsg)
   {
      return  ::windows::thread::ProcessMessageFilter(code, lpMsg);
   }


   // Advanced: access to GetMainWnd()
   sp(::user::interaction) application::GetMainWnd()
   {
      return ::windows::thread::GetMainWnd();
   }


   
   void application::assert_valid() const
   {
      ::windows::thread::assert_valid();
   }

   void application::dump(dump_context & dumpcontext) const
   {
      ::windows::thread::dump(dumpcontext);
   }

   
   void application::CommonConstruct()
   {
      ::windows::thread::CommonConstruct();
   }
   void application::Delete()
   {
      ::windows::thread::Delete();
   }
   // 'delete this' only if m_bAutoDelete == TRUE

   bool application::DispatchThreadMessageEx(MSG* msg)  // helper
   {
      return ::windows::thread::DispatchThreadMessageEx(msg);
   }*/

/*   ::draw2d::graphics * application::graphics_from_os_data(void * pdata)
   {
      return ::windows::graphics::from_handle((HDC) pdata);
   }*/

   ::window_sp application::window_from_os_data(void * pdata)
   {
      return ::windows::window::from_handle((oswindow) pdata);
   }

   ::window_sp application::window_from_os_data_permanent(void * pdata)
   {
      ::window_sp pwnd = ::windows::window::FromHandlePermanent((oswindow) pdata);
      if(pwnd != NULL)
         return pwnd;
      user::interaction_ptr_array wndptra = System.frames();
      for(int32_t i = 0; i < wndptra.get_count(); i++)
      {
         if(wndptra[i].get_safe_handle() == pdata)
         {
            return wndptra[i].get_wnd();
         }
      }
      return NULL;
   }

   ::thread * application::GetThread()
   {
      if(__get_thread() == NULL)
         return NULL;
      else
         return __get_thread()->m_p;
   }

   void application::set_thread(thread * pthread)
   {
      __set_thread(pthread);
   }

   ///////////////////////////////////////////////////////////////////////////
   // application Initialization

   void application::SetCurrentHandles()
   {
      //ASSERT(this == afxCurrentWinApp);
      //if(afxCurrentAppName != NULL)
        // return;
      //ASSERT(afxCurrentAppName == NULL);


      // Note: there are a number of _tcsdup (aka _strdup) calls that are
      // made here for the exe path, help file path, etc.  In previous
      // versions of core API, this primitive::memory was never freed.  In this and future
      // versions this primitive::memory is automatically freed during application's
      // destructor.  If you are freeing the primitive::memory yourself, you should
      // either remove the code or set the pointers to NULL after freeing
      // the primitive::memory.

      // get path of executable
   /*   char szBuff[_MAX_PATH];
      DWORD dwRet = ::GetModuleFileName(m_hInstance, szBuff, _MAX_PATH);
      ASSERT( dwRet != 0 && dwRet != _MAX_PATH );
      if( dwRet == 0 || dwRet == _MAX_PATH )
         throw user_exception();*/

      /*
      LPTSTR lpszExt = ::PathFindExtension(szBuff);
      ASSERT(lpszExt != NULL);
      if( lpszExt == NULL )
         throw user_exception();

      ASSERT(*lpszExt == '.');
      *lpszExt = 0;       // no suffix
      */

      string strExeName;
      //string strTitle = System.load_string("System.title");
      // get the exe title from the full path name [no extension]
      strExeName = System.get_module_title();

//      __get_module_state()->m_lpszCurrentAppName = _tcsdup(m_strAppName);



//      dynamic_cast < ::windows::thread * > ((smart_pointer < ::base::application >::m_p->thread::m_p))->m_hThread = __get_thread()->m_hThread;
  //    dynamic_cast < ::windows::thread * > ((smart_pointer < ::base::application >::m_p->thread::m_p))->m_nThreadID = __get_thread()->m_nThreadID;
      dynamic_cast < class ::windows::thread * > (thread::m_p.m_p)->m_hThread      =  ::GetCurrentThread();
      dynamic_cast < class ::windows::thread * > (thread::m_p.m_p)->m_nThreadID    =  ::GetCurrentThreadId();
      
      // initialize thread specific data (for main thread)
//      if (!afxContextIsDLL)
      __init_thread();

   }

   ::window_sp application::FindWindow(const char * lpszClassName, const char * lpszWindowName)
   {
      return window::FindWindow(lpszClassName, lpszWindowName);
   }

   ::window_sp application::FindWindowEx(oswindow oswindowParent, oswindow oswindowChildAfter, const char * lpszClass, const char * lpszWindow)
   {
      return window::FindWindowEx(oswindowParent, oswindowChildAfter, lpszClass, lpszWindow);
   }


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

   void application::set_env_var(const string & var,const string & value)
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
         putenv( vmap[var] );
      }
   #elif defined _WIN32
      {
         string slask = var + "=" + value;
         _putenv( (const char *)slask);
      }
   #else
      setenv(var, value, 1);
   #endif
   }

   uint32_t application::get_thread_id()
   {
      return ::GetCurrentThreadId();
   }



   bool application::set_main_init_data(::core::main_init_data * pdata)
   {

      m_pmaininitdata = (::windows::main_init_data *) pdata;

      if(m_pmaininitdata != NULL && m_pimpl->is_system())
      {
         if(!win_init(m_pmaininitdata))
            return false;
      }

      return true;

   }

   bool application::win_init(main_init_data * pdata)
   {
         ASSERT(pdata->m_hPrevInstance == NULL);

         HINSTANCE hInstance        = pdata->m_hInstance;
//         HINSTANCE hPrevInstance    = pdata->m_hPrevInstance;
         string strCmdLine          = pdata->m_vssCommandLine;
         UINT nCmdShow              = pdata->m_nCmdShow;

         // handle critical errors and avoid Windows message boxes
         SetErrorMode(SetErrorMode(0) | SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);

         m_hinstance = hInstance;
         m_pbaseapp->m_hinstance = hInstance;
         m_pimpl->m_hinstance = hInstance;
         //hPrevInstance; // Obsolete.
         m_strCmdLine = strCmdLine;
         m_nCmdShow = nCmdShow;
         //pApp->SetCurrentHandles();
         m_pimpl->SetCurrentHandles();

         string strCgcl = read_resource_as_string_dup(NULL, 1984, "TBS");

         if (strCgcl.has_char())
         {
            directrix()->m_varTopicQuery["cgcl_app"] = strCgcl;
            m_pimpl->directrix()->m_varTopicQuery["cgcl_app"] = strCgcl;
         }

         // Initialize window::m_pfnNotifyWinEvent
      /*   HMODULE hModule = ::GetModuleHandle("user32.dll");
         if (hModule != NULL)
         {
            window::m_pfnNotifyWinEvent = (window::PFNNOTIFYWINEVENT)::GetProcAddress(hModule, "NotifyWinEvent");
         }*/

      return true;

   }

   bool application::update_module_paths()
   {

      {

         char lpszModuleFilePath[MAX_PATH + 1];
         GetModuleFileName(NULL, lpszModuleFilePath, MAX_PATH + 1);
         m_strModulePath = lpszModuleFilePath;
         char lpszModuleFolder[MAX_PATH + 1];
         LPTSTR lpszModuleFileName;
         GetFullPathName(lpszModuleFilePath, MAX_PATH + 1, lpszModuleFolder, &lpszModuleFileName);
         string strModuleFolder(lpszModuleFolder, lpszModuleFileName - lpszModuleFolder);
         m_strModuleFolder = strModuleFolder;

      }

      {

         char lpszModuleFilePath[MAX_PATH + 1];
         GetModuleFileName(::GetModuleHandleA("core.dll"), lpszModuleFilePath, MAX_PATH + 1);
         m_strCa2ModulePath = lpszModuleFilePath;

         char lpszModuleFolder[MAX_PATH + 1];
         LPTSTR lpszModuleFileName;
         GetFullPathName(lpszModuleFilePath, MAX_PATH + 1, lpszModuleFolder, &lpszModuleFileName);
         string strModuleFolder(lpszModuleFolder, lpszModuleFileName - lpszModuleFolder);
         m_strCa2ModuleFolder = strModuleFolder;

      }


      return true;

   }

   void application::ShowWaitCursor(bool bShow)
   {

      if(bShow)
      {

         HCURSOR hcursorWait =  ::LoadCursor(NULL, IDC_WAIT);

         HCURSOR hcursorPrevious = ::SetCursor(hcursorWait);
         
         if(hcursorPrevious != hcursorWait)
           m_hcurWaitCursorRestore = hcursorPrevious;

      }
      else
      {

         ::SetCursor(m_hcurWaitCursorRestore);
      }

   }
   
   sp(::user::printer) application::get_printer(const char * pszDeviceName)
   {

      //::windows::printer * pprinter = new ::windows::printer(get_app());

      sp(::user::printer) pprinter(allocer());

      if(!pprinter->open(pszDeviceName))
      {

         delete pprinter;

         return NULL;

      }

      return pprinter;

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
      return "audio_mmsystem";
   }

   string application::draw2d_get_default_library_name()
   {

      string str;

      if (file_exists_dup("C:\\ca2\\config\\draw2d.txt"))
      {

         str = file_as_string_dup("C:\\ca2\\config\\draw2d.txt");

      }
      else
      {

         string strPath;

         strPath = ::dir::userappdata("draw2d.txt");

         str = file_as_string_dup(strPath);

      }

      if(str.has_char())
         return "draw2d_" + str;
      else
         return "draw2d_gdiplus";
   }


} // namespace windows






