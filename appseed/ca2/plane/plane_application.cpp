#include "framework.h"



#if defined(LINUX) || defined(MACOS)
#include <dlfcn.h>
#endif



namespace plane
{
   void trace_progress(index iStep);
   void progress();


   application::application()
   {

      m_papp      = this;
      m_pappThis  = this;

      m_dir.set_app(this);
      m_file.set_app(this);
      m_http.set_app(this);

      m_bIfs            = true;



   }

/*   application::application(const char * pszId)
   {


      m_papp      = this;
      m_pappThis  = this;

      construct(pszId);

   }*/

   void application::construct(const char *pszId)
   {
      //if(m_strAppName.has_char())
      //   return;
      //m_strAppName.Empty();
      //m_strId.Empty();
      if(pszId == ::null())
      {
#ifdef WINDOWSEX
         wstring wstr = ::GetCommandLineW();
         string str = ::ca::international::unicode_to_utf8(wstr);
         strsize iFind = str.find(" : ");
         if(iFind >= 0)
         {
            iFind = str.find("app=", iFind);
            if(iFind >= 0)
            {
               strsize iEnd = str.find(" ", iFind);
               if(iEnd < 0)
               {
                  m_strId = str.Mid(iFind + 4);
               }
               else
               {
                  m_strId = str.Mid(iFind + 4, iEnd - iFind - 4);
               }
               ::ca::str::begins_eat(m_strId, "\"");
               ::ca::str::ends_eat(m_strId, "\"");
            }
         }
#endif
      }
      else
      {
         m_strId = pszId;
      }
      if(m_strId.is_empty())
         m_strId = "mplite";
      construct();
      if(m_strAppName.is_empty())
      {
         if(m_strAppId.has_char())
            m_strAppName = m_strAppId;
         else if(m_strInstallToken.has_char())
            m_strAppName = m_strInstallToken;
      }
   }

   application::~application()
   {
   }

   sp(::ca::application) application::get_system()
   {
      return new application();
   }

   void application::construct()
   {
      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
   }




   void application::_001OnFileNew()
   {
      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      ::ca::application::_001OnFileNew(::null());
   }


   bool application::bergedge_start()
   {
      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      return ::ca::application::bergedge_start();
   }


   bool application::os_native_bergedge_start()
   {
      return true;
   }


   bool application::on_install()
   {
      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      return ::ca::application::on_install();
   }

   bool application::on_uninstall()
   {
      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      return ::ca::application::on_uninstall();
   }


   void application::on_request(sp(::ca::create_context) pcreatecontext)
   {
      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      return ::ca::application::on_request(pcreatecontext);


   }

   bool application::is_serviceable()
   {


      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      return ::ca::application::is_serviceable();
   }

   service_base * application::allocate_new_service()
   {
      
      return ::null();

   }


   sp(::user::document_interface) application::_001OpenDocumentFile(var varFile)
   {
      string strId = m_strId;
      char chFirst = '\0';
      if(strId.get_length() > 0)
      {
         chFirst = strId[0];
      }
      return ::ca::application::_001OpenDocumentFile(varFile);

   }


   int32_t application::run()
   {

      if((command()->m_varTopicQuery.has_property("install")
      || command()->m_varTopicQuery.has_property("uninstall"))
      &&
      ((is_session() && command()->m_varTopicQuery["session_start"] == "session")))
      {
      }
      else if(!is_system() && !is_session())
      {
         if(command()->m_varTopicQuery.has_property("install")
         || command()->m_varTopicQuery.has_property("uninstall"))
         {
         }
         else if(command()->m_varTopicQuery.has_property("service"))
         {
            create_new_service();
            ::service_base::serve(*m_pservice);
         }
         else if(command()->m_varTopicQuery.has_property("run") || is_serviceable())
         {
            create_new_service();
            m_pservice->Start(0);
            return ::ca::application::run();
         }
         else
         {
            return ::ca::application::run();
         }
      }
      else
      {
         return ::ca::application::run();
      }

      return 0;

   }




   sp(::plane::application) application::assert_running(const char * pszAppId)
   {


      sp(::plane::application) papp = ::null();


      try
      {

         bool bFound = false;

         for(int32_t i  = 0; i < System.m_appptra.get_count(); i++)
         {
            try
            {

               papp = System.m_appptra(i);

               if(papp->m_strAppName == pszAppId)
               {
                  bFound = true;
                  break;
               }

            }
            catch(...)
            {
            }

         }

         bool bCreate = !bFound;

         if(bFound)
         {

            bool bRunning = false;

            try
            {
               if(papp->is_running())
               {
                  bRunning = true;
               }
            }
            catch(...)
            {
            }

            if(!bRunning)
            {

               try
               {
                  papp->post_thread_message(WM_QUIT, 0, 0);
               }
               catch(...)
               {
               }
               try
               {
                  papp.release();
               }
               catch(...)
               {
               }

               bCreate = true;

            }


         }

         if(bCreate)
         {

            sp(::ca::create_context) spcreatecontext(allocer());

            papp = Session.start_application("application", pszAppId, spcreatecontext);

         }

      }
      catch(::exit_exception & e)
      {

         throw e;

      }
      catch(::ca::exception & e)
      {

         if(!Application.on_run_exception(e))
            throw exit_exception(get_app());

      }
      catch(...)
      {

         papp = ::null();

      }


      return papp;

   }





typedef  void (* PFN_ca2_factory_exchange)(sp(::ca::application) papp);




   ::ca::filesp application::friendly_get_file(var varFile, UINT nOpenFlags)
   {

      try
      {

         return m_file.get_file(varFile, nOpenFlags);

      }
      catch(::ca::file_exception & e)
      {

         string strMessage = e.get_message();

         App(this).simple_message_box(::null(), strMessage, MB_OK);

         return ::ca::null();

      }

   }


   ::fontopus::fontopus * application::create_fontopus()
   {

      return new ::fontopus::fontopus();

   }


   ::user::user * application::create_user()
   {

      return new ::user::user();

   }


   ::fs::fs * application::create_fs()
   {

      return new ::ca::fs::fs();

   }


   ::html::html * application::create_html()
   {

      return new ::html::html();

   }



   bool application::is_installing()
   {

      return directrix()->has_property("install");

   }


   bool application::is_uninstalling()
   {

      return directrix()->has_property("uninstall");

   }


   bool application::create_new_service()
   {

      if(m_pservice != ::null())
         return false;

      m_pservice = allocate_new_service();

      if(m_pservice == ::null())
         return false;

      return true;

   }



   bool application::create_service()
   {

      return System.os().create_service(this);

   }

   bool application::remove_service()
   {

      return System.os().remove_service(this);

   }

    bool application::start_service()
   {

      return System.os().start_service(this);

   }

   bool application::stop_service()
   {

      return System.os().stop_service(this);

   }


   void application::on_service_request(sp(::ca::create_context) pcreatecontext)
   {

      if(!is_serviceable())
         return;

      if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("create_service"))
      {
         create_service();
      }
      else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("start_service"))
      {
         start_service();
      }
      else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("stop_service"))
      {
         stop_service();
      }
      else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("remove_service"))
      {
         remove_service();
      }

   }



   void application::defer_initialize_twf()
   {
      if(System.m_ptwf == ::null() && (System.m_bShouldInitializeGTwf && m_bShouldInitializeGTwf && m_bInitializeProDevianMode))
      {
         System.create_twf();
      }
   }


      int32_t application::pre_run()
   {

//      m_dir.m_psystem      = m_psystem;
  //    m_file.m_psystem     = m_psystem;

#ifdef WINDOWSEX

      MESSAGE msg;

      // Create Windows Message Queue
      ::PeekMessageA(&msg, ::null(), 0, 0xffff, 0);

      if(!is_system() && (bool)oprop("SessionSynchronizedInput"))
      {
         ::AttachThreadInput(GetCurrentThreadId(), (uint32_t) System.::ca::thread_sp::m_p->get_os_int(), TRUE);
      }

#endif

      if(is_system())
      {
         translator::attach();
      }

      m_iReturnCode            = 0;

      m_dwAlive = ::get_tick_count();

      // App global initializations (rare)
      if (!InitApplication())
         goto InitFailure;

      m_dwAlive = ::get_tick_count();
      // Perform specific initializations
#if !defined(DEBUG) || defined(WINDOWS)
      try
      {
         try
         {
#endif
            if(!process_initialize())
            {
               if (GetMainWnd() != ::null())
               {
                  TRACE(::ca::trace::category_AppMsg, 0, "Warning: Destroying non-::null() GetMainWnd()\n");
                  GetMainWnd()->DestroyWindow();
               }
               goto InitFailure;
            }
#if !defined(DEBUG) || defined(WINDOWS)
         }
         catch(::exit_exception & e)
         {

            throw e;

         }
         catch(const ::ca::exception &)
         {
            if (GetMainWnd() != ::null())
            {
               GetMainWnd()->DestroyWindow();
               SetMainWnd(::null());
            }
            goto InitFailure;
         }
#endif
         m_dwAlive = ::get_tick_count();
#if !defined(DEBUG) || defined(WINDOWS)
         try
         {
#endif
            if(!initialize_instance())
            {
               if (GetMainWnd() != ::null())
               {
                  TRACE(::ca::trace::category_AppMsg, 0, "Warning: Destroying non-::null() GetMainWnd()\n");
                  GetMainWnd()->DestroyWindow();
               }

#if !defined(DEBUG) || defined(WINDOWS)
               try
               {
#endif
                  exit();
#if !defined(DEBUG) || defined(WINDOWS)
               }
               catch(...)
               {
               }
#endif
               m_iReturnCode = -1;
               goto InitFailure;
            }
#if !defined(DEBUG) || defined(WINDOWS)
         }
         catch(::exit_exception & e)
         {

            throw e;

         }
         catch(const ::ca::exception & e)
         {
            if(on_run_exception((::ca::exception &) e))
               goto run;
            if(GetMainWnd() != ::null())
            {
               TRACE(::ca::trace::category_AppMsg, 0, "Warning: Destroying non-::null() GetMainWnd()\n");
               try
               {
                  GetMainWnd()->DestroyWindow();
               }
               catch(::exit_exception & e)
               {

                  throw e;

               }
               catch(::ca::exception &)
               {
               }
               SetMainWnd(::null());
            }
            if(final_handle_exception((::ca::exception &) e))
               goto run;
            if(GetMainWnd() != ::null())
            {
               TRACE(::ca::trace::category_AppMsg, 0, "Warning: Destroying non-::null() GetMainWnd()\n");
               try
               {
                  GetMainWnd()->DestroyWindow();
               }
               catch(::exit_exception & e)
               {

                  throw e;

               }
               catch(::ca::exception &)
               {
               }
               SetMainWnd(::null());
            }
            try
            {
               m_iReturnCode = exit();
            }
            catch(...)
            {
               m_iReturnCode = -1;
            }
            if(m_iReturnCode == 0)
               m_iReturnCode = -1;
            goto InitFailure;
         }
      }
      catch(::exit_exception & e)
      {

         throw e;

      }
      catch(...)
      {
         // linux-like exit style on crash, differently from general windows error message approach
         // to prevent or correct from crash, should:
         // - look at dumps - to do;
         // - look at trace and log - always doing;
         // - look at debugger with the visual or other tool atashed - to doing;
         // - fill other possibilities;
         // - restart and send information in the holy back, and stateful or self-heal as feedback from below;
         // - ...
         // - ..
         // - .
         // - .
         // - .
         // - .
         // -  .
         // - ...
         // - ...
         // - ...
         // to pro-activia and overall benefits workaround:
         // - stateful applications:
         //      - browser urls, tabs, entire history, in the ca2computing cloud;
         //      - uint16_t - html document to simplify here - with all history of undo and redos per document optimized by cvs, svn, syllomatter;
         //           - not directly related but use date and title to name document;
         //      - save forms after every key press in .undo.redo.form file parallel to appmatter / form/undo.redo.file parity;
         //      - last ::ikaraoke::karaoke song and scoring, a little less motivated at time of writing;
         //
         // - ex-new-revolut-dynamic-news-self-healing
         //      - pre-history, antecendentes
         //            - sometimes we can't recover from the last state
         //            - to start from the beggining can be too heavy, waity, worky, bory(ing)
         //      - try to creativetily under-auto-domain with constrained-learning, or heuristcally recover from restart, shutdown, login, logoff;
         //           - reification :
         //           - if the document is corrupted, try to open the most of it
         //           - if can only detect that the document cannot be opened or damaged, should creatively workarounds as it comes, as could it be
         //              done, for example search in the web for a proper solution?
         //           - ::ikaraoke::karaoke file does not open? can open next? do it... may animate with a temporary icon...
         //           - import a little as pepper for the meal, prodevian technology into estamira, so gaming experience relativity can open ligh
         //               speed into cartesian dimensions of
         //               ca, estamira and prodevian. Take care not to flood prodevian brand black ink over the floor of the estamira office...
         //               black letters, or colorful and pink are accepted and sometimes desired, for example, hello kity prodevian, pirarucu games,
         //               I think no one likes to be boring, but a entire background in black... I don't know... only for your personal office, may be...
         //           - could an online colaborator investigate crashes promptly in a funny way, and make news and jokes? Like terra and UOL for the real world?
         //               - new crash, two documents lost, weathers fault, too hot, can't think, my mother was angry with me, lead to buggy code;
         //               - new version with bug fixes;
         //      - new versions
         //      - automatic updates
         //      - upgrades
         //      - rearrangemntes
         //      - downgrade in the form of retro
         // - ...
         // - ..
         // - .
         // - .
         // - .
         // - .
         // -  .
         // - ...
         // - ...
         // - ...

      }
#endif
InitFailure:
#if !defined(DEBUG) || defined(WINDOWS)
run:
#endif
      return m_iReturnCode;
   }




      int32_t application::on_run()
   {
      int32_t m_iReturnCode = 0;

      try
      {
         ::ca::application_signal_object signal(this, m_psignal, ::ca::application_signal_start);
         m_psignal->emit(&signal);
      }
      catch(...)
      {
      }

      install_message_handling(System.GetThread()->::ca::smart_pointer < ::ca::thread > ::m_p);
#if !defined(DEBUG) || defined(WINDOWS)
      try
      {
         try
         {
#endif
            m_bReady = true;
            if(m_peventReady != ::null())
               m_peventReady->SetEvent();
#if !defined(DEBUG) || defined(WINDOWS)
         }
         catch(...)
         {
         }
#endif
#if !defined(DEBUG) || defined(WINDOWS)
run:
#endif
#if !defined(DEBUG) || defined(WINDOWS)
         try
         {
#endif
            m_iReturnCode = run();
#if !defined(DEBUG) || defined(WINDOWS)
         }
         catch(::exit_exception & e)
         {

            throw e;

         }
         catch(const ::ca::exception & e)
         {
            if(on_run_exception((::ca::exception &) e))
               goto run;
            if (GetMainWnd() != ::null())
            {
               TRACE(::ca::trace::category_AppMsg, 0, "Warning: Destroying non-::null() GetMainWnd()\n");
               try
               {
                  GetMainWnd()->DestroyWindow();
               }
               catch(::ca::exception &)
               {
               }
               SetMainWnd(::null());
            }
            if(final_handle_exception((::ca::exception &) e))
               goto run;
            if (GetMainWnd() != ::null())
            {
               TRACE(::ca::trace::category_AppMsg, 0, "Warning: Destroying non-::null() GetMainWnd()\n");
               try
               {
                  GetMainWnd()->DestroyWindow();
               }
               catch(::exit_exception & e)
               {

                  throw e;

               }
               catch(::ca::exception &)
               {
               }
               SetMainWnd(::null());
            }
            try
            {
               m_iReturnCode = exit();
            }
            catch(::exit_exception & e)
            {

               throw e;

            }
            catch(...)
            {
               m_iReturnCode = -1;
            }
            goto InitFailure;
         }
      }
      catch(::exit_exception & e)
      {

         throw e;

      }
      catch(...)
      {
         // linux-like exit style on crash, differently from general windows error message approach
         // to prevent or correct from crash, should:
         // - look at dumps - to do;
         // - look at trace and log - always doing;
         // - look at debugger with the visual or other tool atashed - to doing;
         // - fill other possibilities;
         // - restart and send information in the holy back, and stateful or self-heal as feedback from below;
         // - ...
         // - ..
         // - .
         // - .
         // - .
         // - .
         // -  .
         // - ...
         // - ...
         // - ...
         // to pro-activia and overall benefits workaround:
         // - stateful applications:
         //      - browser urls, tabs, entire history, in the ca2computing cloud;
         //      - uint16_t - html document to simplify here - with all history of undo and redos per document optimized by cvs, svn, syllomatter;
         //           - not directly related but use date and title to name document;
         //      - save forms after every key press in .undo.redo.form file parallel to appmatter / form/undo.redo.file parity;
         //      - last ::ikaraoke::karaoke song and scoring, a little less motivated at time of writing;
         //
         // - ex-new-revolut-dynamic-news-self-healing
         //      - pre-history, antecendentes
         //            - sometimes we can't recover from the last state
         //            - to start from the beggining can be too heavy, waity, worky, bory(ing)
         //      - try to creativetily under-auto-domain with constrained-learning, or heuristcally recover from restart, shutdown, login, logoff;
         //           - reification :
         //           - if the document is corrupted, try to open the most of it
         //           - if can only detect that the document cannot be opened or damaged, should creatively workarounds as it comes, as could it be
         //              done, for example search in the web for a proper solution?
         //           - ::ikaraoke::karaoke file does not open? can open next? do it... may animate with a temporary icon...
         //           - import a little as pepper for the meal, prodevian technology into estamira, so gaming experience relativity can open ligh
         //               speed into cartesian dimensions of
         //               ca, estamira and prodevian. Take care not to flood prodevian brand black ink over the floor of the estamira office...
         //               black letters, or colorful and pink are accepted and sometimes desired, for example, hello kity prodevian, pirarucu games,
         //               I think no one likes to be boring, but a entire background in black... I don't know... only for your personal office, may be...
         //           - could an online colaborator investigate crashes promptly in a funny way, and make news and jokes? Like terra and UOL for the real world?
         //               - new crash, two documents lost, weathers fault, too hot, can't think, my mother was angry with me, lead to buggy code;
         //               - new version with bug fixes;
         //      - new versions
         //      - automatic updates
         //      - upgrades
         //      - rearrangemntes
         //      - downgrade in the form of retro
         // - ...
         // - ..
         // - .
         // - .
         // - .
         // - .
         // -  .
         // - ...
         // - ...
         // - ...

      }
#endif
#if !defined(DEBUG) || defined(WINDOWS)
InitFailure:
#endif
      try
      {
         if(m_peventReady != ::null())
            m_peventReady->SetEvent();
      }
      catch(...)
      {
      }
      try
      {
         ::ca::thread * pthread = dynamic_cast < ::ca::thread * > (::ca::smart_pointer < ::ca::thread >::m_p);
         if(pthread != ::null() && pthread->m_pbReady != ::null())
         {
            *pthread->m_pbReady = true;
         }
      }
      catch(...)
      {
      }
      /*try
      {
         ::ca::thread * pthread = dynamic_cast < ::ca::thread * > (this);
         ::SetEvent((HANDLE) pthread->m_peventReady);
      }
      catch(...)
      {
      }*/

      // let translator run undefinetely
      /*if(is_system())
      {
         translator::detach();
      }*/

      return m_iReturnCode;
   }

   bool application::main_start()
   {

      TRACE(string(typeid(*this).name()) + " main_start");;
      try
      {

         m_dwAlive = ::get_tick_count();
         TRACE(string(typeid(*this).name()) + "pre_run");;
         int32_t m_iReturnCode = pre_run();
         if(m_iReturnCode != 0)
         {
            m_bReady = true;
            TRACE("application::main pre_run failure");
            return false;
         }

         TRACE(string(typeid(*this).name()) + " initial_check_directrix");;
         if(!initial_check_directrix())
         {
            exit();
            m_iReturnCode = -1;
            m_bReady = true;
            ::OutputDebugStringW(L"exiting on check directrix");
            return false;
         }


         TRACE(string(typeid(*this).name()) + " os_native_bergedge_start");;
         m_dwAlive = ::get_tick_count();
         if(!os_native_bergedge_start())
         {
            exit();
            m_iReturnCode = -1;
            m_bReady = true;
            ::OutputDebugStringW(L"application::main os_native_bergedge_start failure");
            return false;
         }

         return true;
      }
      catch(::exit_exception &)
      {

         System.os().post_to_all_threads(WM_QUIT, 0, 0);

      }

      return false;

   }

   int32_t application::main()
   {

      TRACE(string(typeid(*this).name()) + " main");;

      try
      {

         if(!main_start())
         {
            m_bReady = true;
            TRACE("application::start failure");
            return m_iReturnCode;
         }

      }
      catch(::exit_exception &)
      {

         System.os().post_to_all_threads(WM_QUIT, 0, 0);

         goto exit_application;

      }
      catch(...)
      {

         goto exit_application;

      }


      try
      {

         TRACE(string(typeid(*this).name()) + " on_run");;
         m_iReturnCode = 0;
         m_bReady = true;
         m_iReturnCode = on_run();
         if(m_iReturnCode != 0)
         {
            ::OutputDebugStringW(L"application::main on_run termination failure");
         }

      }
      catch(::exit_exception &)
      {

         System.os().post_to_all_threads(WM_QUIT, 0, 0);

         goto exit_application;

      }
      catch(...)
      {

         goto exit_application;

      }

      try
      {

         if(is_system())
         {

            System.os().post_to_all_threads(WM_QUIT, 0, 0);

         }

      }
      catch(...)
      {

      }

exit_application:

      try
      {

         m_iReturnCode = exit();

      }
      catch(::exit_exception &)
      {

         System.os().post_to_all_threads(WM_QUIT, 0, 0);

         m_iReturnCode = -1;

      }
      catch(...)
      {

         m_iReturnCode = -1;

      }

      return m_iReturnCode;

   }


   sp(::ca::application) application::instantiate_application(const char * pszType, const char * pszId, ::ca::application_bias * pbias)
   {

      sp(::ca::application) pcaapp = ::null();
      sp(::ca::application) papp = ::null();

      string strId(pszId);

      if(strId.CompareNoCase("session") == 0)
      {
         ::plane::session * psession = new ::plane::session();
         pcaapp = psession;
         psession->construct();
         if(m_psystem != ::null() && m_psystem->m_psession == ::null())
         {
            m_psystem->m_psession = psession;
         }
         psession->m_strAppId = "session";
      }
      else
      {

         string strNewId;

         if(strId == "bergedge")
         {

            strNewId = "app/ca2/bergedge";

         }
         else if(strId == "cube")
         {

            strNewId = "app/ca2/cube";

         }
         else
         {

            strNewId = strId;

         }

         pcaapp = System.get_new_app(this, pszType, strNewId);

         if(pcaapp == ::null())
            return ::null();

         pcaapp->m_psession                          = m_psession;

         /*if(pcaapp->m_bService)
         {
            App(pcaapp).m_puiInitialPlaceHolderContainer  = ::null();
         }*/
         if(m_psystem != ::null() && m_psystem->m_psession == ::null())
         {
            m_psystem->m_psession = m_psession;
         }

         sp(::ca::application) pgenapp =  (pcaapp);

         if(pgenapp != ::null())
         {

            if(strId == "bergedge"
            || strId == "cube")
            {

               pgenapp->m_strAppId = strId;

            }

            if(pgenapp->m_strInstallToken.is_empty())
            {

               pgenapp->m_strInstallToken = pgenapp->m_strAppId;

            }

         }



      }

      //pcaapp->m_papp                               = this;
      pcaapp->m_psystem                            = m_psystem;

      papp =  (pcaapp);

      papp->command_central()->consolidate(command_central());

      papp->m_bSessionSynchronizedCursor = m_bSessionSynchronizedCursor;

      if(pbias != ::null())
      {
         papp->propset().merge(pbias->m_set);
      }
      else
      {
         papp->oprop("SessionSynchronizedInput")   = true;
         papp->oprop("NativeWindowFocus")          = true;
      }




      if((papp == ::null() || papp->m_strAppId != strId)
         &&
         (!Application.command()->m_varTopicQuery.has_property("install")
         && !Application.command()->m_varTopicQuery.has_property("uninstall")))
      {

         TRACE("Failed to instantiate %s, going to try installation through ca2_cube_install", strId);

         string strCommandLine;

         strCommandLine    =" : app=" + strId;
         strCommandLine    += " locale=" + string(Application.str_context()->m_plocaleschema->m_idLocale);
         strCommandLine    += " style=" + string(Application.str_context()->m_plocaleschema->m_idSchema);
         strCommandLine    += " install";

         System.install().start(strCommandLine);

         throw installing_exception(get_app());

         return ::null();

      }


      return pcaapp;
   }


   sp(::ca::application) application::create_application(const char * pszType, const char * pszId, bool bSynch, ::ca::application_bias * pbias)
   {

      sp(::ca::application) pcaapp = instantiate_application(pszType, pszId, pbias);

      if(pcaapp == ::null())
         return ::null();

      sp(::ca::application) papp = (pcaapp);

      if(!papp->start_application(bSynch, pbias))
      {
         try
         {
            delete pcaapp;
         }
         catch(...)
         {
         }
         return ::null();
      }


      return pcaapp;

   }

   count application::get_monitor_count()
   {

      return 0;

   }

   bool application::get_monitor_rect(index i, LPRECT lprect)
   {

      return false;

   }

   count application::get_desk_monitor_count()
   {

      return 0;

   }

   bool application::get_desk_monitor_rect(index i, LPRECT lprect)
   {

      return false;

   }

      bool application::is_licensed(const char * pszId, bool bInteractive)
   {

      if(directrix()->m_varTopicQuery.has_property("install"))
         return true;

      if(directrix()->m_varTopicQuery.has_property("uninstall"))
         return true;

      return license().has(pszId, bInteractive);

   }


   bool application::initial_check_directrix()
   {

      if(directrix()->m_varTopicQuery.has_property("install"))
      {

         if(!on_install())
            return false;

         string strId = m_strAppId;

         xxdebug_box("on_install1", strId, 0);

         if(strId.is_empty())
            strId = m_strAppName;

         if(strId.has_char() && command()->m_varTopicQuery.has_property("app") && strId == command()->m_varTopicQuery["app"])
         {

            system_add_app_install(strId);

         }
         else if(strId.has_char() && command()->m_varTopicQuery.has_property("session_start") && strId == command()->m_varTopicQuery["session_start"])
         {

            system_add_app_install(strId);

         }
         else if(m_strInstallToken.has_char())
         {

            system_add_app_install(m_strInstallToken);

         }

      }
      else if(directrix()->m_varTopicQuery.has_property("uninstall"))
      {

         if(!on_uninstall())
            return false;
         
         System.install().remove_spa_start(m_strInstallType, m_strInstallToken);

      }

      return true;

   }

      void application::set_title(const char * pszTitle)
   {

      Session.set_app_title(m_strInstallType, m_strAppName, pszTitle);

   }


//////////////////////////////////////////////////////////////////////////////////////////////////
   // Session/Bergedge
   //
   sp(::bergedge::view) application::get_view()
   {

      return ::null();

   }

   sp(::bergedge::document) application::get_document()
   {

      return ::null();

   }


   void application::fill_locale_schema(::ca::international::locale_schema & localeschema, const char * pszLocale, const char * pszSchema)
   {


      localeschema.m_idaLocale.remove_all();
      localeschema.m_idaSchema.remove_all();


      string strLocale(pszLocale);
      string strSchema(pszSchema);


      localeschema.m_idLocale     = pszLocale;
      localeschema.m_idSchema     = pszSchema;


      localeschema.add_locale_variant(strLocale    , strSchema);
      localeschema.add_locale_variant(get_locale() , strSchema);
      localeschema.add_locale_variant(__id(std)    , strSchema);
      localeschema.add_locale_variant(__id(en)     , strSchema);


      localeschema.finalize();


   }


   void application::fill_locale_schema(::ca::international::locale_schema & localeschema)
   {


      localeschema.m_idaLocale.remove_all();
      localeschema.m_idaSchema.remove_all();


      //localeschema.m_bAddAlternateStyle = true;


      string strLocale;
      string strSchema;

      strLocale  = get_locale();
      strSchema  = get_schema();

      if(Application.directrix()->m_varTopicQuery["locale"].has_char() && Application.directrix()->m_varTopicQuery["locale"].get_string().CompareNoCase("_std") != 0)
      {
         strLocale = Application.directrix()->m_varTopicQuery["locale"];
      }

      if(Application.directrix()->m_varTopicQuery["schema"].has_char() && Application.directrix()->m_varTopicQuery["schema"].get_string().CompareNoCase("_std") != 0)
      {
         strSchema = Application.directrix()->m_varTopicQuery["schema"];
      }


      localeschema.m_idLocale     = strLocale;
      localeschema.m_idSchema     = strSchema;


      localeschema.add_locale_variant(strLocale    , strSchema);
      localeschema.add_locale_variant(get_locale() , strSchema);
      localeschema.add_locale_variant(__id(std)    , strSchema);
      localeschema.add_locale_variant(__id(en)     , strSchema);


      localeschema.finalize();


   }

   bool application::update_appmatter(::sockets::socket_handler & h, ::sockets::http_session * & psession,const char * pszRoot, const char * pszRelative)
   {

      ::ca::international::locale_schema localeschema(this);

      fill_locale_schema(localeschema);

      bool bIgnoreStdStd = string(pszRoot) == "app" && (string(pszRelative) == "main" || string(pszRelative) == "bergedge");

      //update_appmatter(h, psession, pszRoot, pszRelative, localeschema.m_idLocale, localeschema.m_idSchema);

      for(int32_t i = 0; i < localeschema.m_idaLocale.get_count(); i++)
      {
         if(localeschema.m_idaLocale[i] == __id(std) && localeschema.m_idaSchema[i] == __id(std) && bIgnoreStdStd)
            continue;
         update_appmatter(h, psession, pszRoot, pszRelative, localeschema.m_idaLocale[i], localeschema.m_idaSchema[i]);
         progress();
      }


      return true;

   }

   bool application::update_appmatter(::sockets::socket_handler & h, ::sockets::http_session * & psession,const char * pszRoot, const char * pszRelative, const char * pszLocale, const char * pszStyle)
   {

      string strLocale;
      string strSchema;
      TRACE("update_appmatter(root=%s, relative=%s, locale=%s, style=%s)", pszRoot, pszRelative, pszLocale, pszStyle);
      string strRelative = System.dir().path(System.dir().path(pszRoot, "appmatter", pszRelative), App(this).get_locale_schema_dir(pszLocale, pszStyle)) + ".zip";
      string strFile = System.dir().ca2(strRelative);
      string strUrl;
      if(_ca_is_basis())
      {
         strUrl = "http://basis.spaignition.api.server.ca2.cc/download?authnone&version=basis&stage=";
      }
      else
      {
         strUrl = "http://stage.spaignition.api.server.ca2.cc/download?authnone&version=stage&stage=";
      }

      strUrl += System.url().url_encode(strRelative);

      primitive::memory mem(get_app());

      if(psession == ::null())
      {

         while(true)
         {

            ::ca::property_set setEmpty(get_app());

            psession = System.http().open(h, System.url().get_server(strUrl), System.url().get_protocol(strUrl), setEmpty, ::null(), ::null());

            if(psession != ::null())
               break;

            Sleep(184);

         }

      }

      psession = System.http().get(h, psession, strUrl, mem);

      primitive::memory_file file(get_app(), &mem);

      if(mem.get_size() > 0)
      {

         zip::Util util;

         string strDir = strFile;

         ::ca::str::ends_eat_ci(strDir, ".zip");

         try
         {

            util.extract_all(strDir, &file);

         }
         catch(...)
         {

            // spa app-install.exe would recover by retrying or someone would fix the resource packaging problem and then zip extraction at least should work.

            return false;

         }

         //System.compress().extract_all(strFile, this);

      }

      return true;

   }


   bool application::add_library(::ca::library * plibrary)
   {

      plibrary->set_app(this);

      System.add_library(plibrary);

      return true;

   }


   ::fontopus::user * application::get_user()
   {

      return m_pfontopus->get_user();

   }


   count g_iCountProgress;
   index g_iStepProgress;

   void trace_progress(index iStep)
   {
      double dProgressStart   = 0.8;
      double dProgressEnd     = 1.0;
      double dProgress = dProgressStart + (dProgressEnd - dProgressStart) * ((double) iStep / (double) g_iCountProgress);
      ::trace_progress(dProgress);
   }

   void progress()
   {
      trace_progress(++g_iStepProgress);
   }


   bool application::system_add_app_install(const char * pszId)
   {

      string strId(pszId);
      string strLocale = command()->m_varTopicQuery["locale"];
      string strSchema = command()->m_varTopicQuery["schema"];

      System.install().remove_spa_start(m_strInstallType, strId);
      System.install().add_app_install(System.command()->m_varTopicQuery["build_number"], m_strInstallType, strId, m_strLocale, m_strSchema);
      System.install().add_app_install(System.command()->m_varTopicQuery["build_number"], m_strInstallType, strId, m_strLocale, strSchema);
      System.install().add_app_install(System.command()->m_varTopicQuery["build_number"], m_strInstallType, strId, strLocale, m_strSchema);
      System.install().add_app_install(System.command()->m_varTopicQuery["build_number"], m_strInstallType, strId, strLocale, strSchema);

      return true;

   }

   bool application::initialize()
   {




      if(m_bIfs)
      {
         if(&Session != ::null())
         {
            if(m_spfsdata.is_null())
               m_spfsdata = new ::fs::set(this);
            ::fs::set * pset = dynamic_cast < ::fs::set * > ((class ::fs::data *) m_spfsdata);
            pset->m_spafsdata.add(Session.m_pifs);
            pset->m_spafsdata.add(Session.m_prfs);
            stringa stra;
            pset->root_ones(stra);
         }
      }

      m_dwAlive = ::get_tick_count();

      m_splicense = new class ::fontopus::license(this);


      if(!is_system())
      {
         System.register_bergedge_application(this);
      }


      if(!::ca::application::initialize())
         return false;


      if(!m_pfs->initialize())
         return false;


      if(!m_simpledb.initialize())
         return false;

      if(!m_spuser->initialize())
         return false;

      if(!m_phtml->initialize())
         return false;

      if(!is_system() && !is_session() &&  !is_installing() && !is_uninstalling())
      {

         string str;
         // if system locale has changed (compared to last recorded one by ca)
         // use the system locale
         if(data_get("system_locale", str))
         {
            if(str.has_char())
            {
               if(str != get_locale())
               {
                  try
                  {
                     data_set("system_locale", get_locale());
                     data_set("locale", get_locale());
                  }
                  catch(...)
                  {
                  }
               }
            }
         }
         else
         {
            data_set("system_locale", get_locale());
         }

         if(command()->m_varTopicQuery["locale"].get_string().has_char())
         {
            str = command()->m_varTopicQuery["locale"];
            data_set("system_locale", str);
            data_set("locale", str);
            set_locale(str, false);
         }
         else if(command()->m_varTopicQuery["lang"].get_string().has_char())
         {
            str = command()->m_varTopicQuery["lang"];
            data_set("system_locale", str);
            data_set("locale", str);
            set_locale(str, false);
         }
         else if(data_get("locale", str))
         {
            if(str.has_char())
            {
               set_locale(str, false);
            }
         }
         // if system schema has changed (compared to last recorded one by ca)
         // use the system schema
         if(data_get("system_schema", str))
         {
            if(str.has_char())
            {
               if(str != get_schema())
               {
                  try
                  {
                     data_set("system_schema", get_schema());
                     data_set("schema", get_schema());
                  }
                  catch(...)
                  {
                  }
               }
            }
         }
         else
         {
            data_set("system_schema", get_schema());
         }

         if(command()->m_varTopicQuery["schema"].get_string().has_char())
         {
            str = command()->m_varTopicQuery["schema"];
            data_set("system_schema", str);
            data_set("schema", str);
            set_schema(str, false);
         }
         else if(data_get("schema", str))
         {
            if(str.has_char())
            {
               set_schema(str, false);
            }
         }

         // keyboard layout
         if(data_get("keyboard_layout", str) && str.has_char())
         {
            user()->set_keyboard_layout(str, false);
         }
         else
         {
            user()->set_keyboard_layout(::null(), false);
         }

         data_pulse_change("ca2", "savings", ::null());


         App(this).fill_locale_schema(*str_context()->m_plocaleschema);


         Sys(this).appa_load_string_table();

   }


      return true;

   }

   bool application::process_initialize()
   {

      if(!::ca::application::process_initialize())
         return false;

      m_pfontopus = create_fontopus();

      if(m_pfontopus == ::null())
         return false;

      m_pfontopus->construct(this);

      m_spuser = create_user();

      if(m_spuser == ::null())
         return false;

      m_spuser->construct(this);

      m_pfs = create_fs();

      if(m_pfs == ::null())
         return false;

      m_pfs->construct(this);

      m_phtml = create_html();

      if(m_phtml == ::null())
         return false;

      m_phtml->construct(this);

      return true;

   }

   bool application::initialize1()
   {

      


      if(!is_system())
      {
         if(m_spfsdata.is_null())
            m_spfsdata = new ::fs::set(this);
         ::fs::set * pset = dynamic_cast < ::fs::set * > ((class ::fs::data *) m_spfsdata);
         pset->m_spafsdata.add(new ::fs::native(this));
         stringa stra;
         pset->root_ones(stra);
      }


      if(fontopus()->m_puser == ::null() &&
        (Application.directrix()->m_varTopicQuery.has_property("install")
      || Application.directrix()->m_varTopicQuery.has_property("uninstall")))
      {
         
         if(fontopus()->create_system_user("system") == ::null())
            return false;
         
      }

      if(!::ca::application::initialize1())
         return false;

      if(!m_spuser->initialize1())
         return false;

      return true;

   }

   bool application::initialize2()
   {

      if(!::ca::application::initialize2())
         return false;

      m_simpledb.construct(this);

      if(!m_simpledb.initialize2())
         return false;


      fill_locale_schema(*str_context()->m_plocaleschema);


      if(!is_installing() && !is_uninstalling())
      {

         if(!user()->keyboard().initialize())
            return false;

         simpledb().set_keyboard_layout(::null(), false);

      }


      if(!m_spuser->initialize2())
         return false;


      return true;

   }


   bool application::initialize_instance()
   {

      if(!::ca::application::initialize_instance())
         return false;

      if(!m_pfontopus->initialize_instance())
         return false;
      

      return true;


   }


   int32_t session::main()
   {


      return ::platform::application::main();


   }

  int32_t application::exit_instance()
   {

      try
      {

         m_iReturnCode = ::ca::application::exit_instance();

      }
      catch(...)
      {

         m_iReturnCode = -1;

      }

      try
      {

         System.unregister_bergedge_application(this);

      }
      catch(...)
      {

         m_iReturnCode = -1;

      }

      try
      {

         if(System.appptra().get_count() <= 1)
         {

            System.post_thread_message(WM_QUIT, 0, 0);

         }

      }
      catch(...)
      {

         m_iReturnCode = -1;

      }

      return m_iReturnCode;

   }



   service_base * application::get_service()
   {
      return m_pservice;
   }


} //namespace plane





