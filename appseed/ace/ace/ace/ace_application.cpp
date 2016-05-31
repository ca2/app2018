//#include "framework.h"


#if defined(LINUX)
#ifdef _GNU_SOURCE
#undef _GNU_SOURCE

#include "ace/node/ansios/ansios.h"
#include "ace/node/linux/linux.h"

//#include <X11/cursorfont.h>
#include <sys/time.h>
#include <link.h>
#include <pthread.h>

#endif
#define _GNU_SOURCE
//#include <link.h>
#elif defined(APPLEOS)
//#include <dlfcn.h>
#elif defined(ANDROID)

#include "ace/node/ansios/ansios.h"
#include "ace/node/android/android.h"

#endif


extern void * g_pf1;


namespace ace
{


   application_signal_details::application_signal_details(class ::signal * psignal,e_application_signal esignal):
      ::signal_details(psignal)
   {

         m_esignal = esignal;
         m_bOk = true;

      }

   application::application():
      m_allocer(this),
      m_mutexMatterLocator(this),
      m_mutexStr(this)
   {

      m_eexclusiveinstance = ExclusiveInstanceNone;

      m_peventReady = NULL;

      m_bAgreeExit = true;
      m_bAgreeExitOk = true;
      m_bFranceExit = true;

      //m_pmutex = new mutex(this);

      m_pacesystem     = NULL;
      m_pacesession    = NULL;
      m_paxisapp        = NULL;
      m_paxissystem     = NULL;
      m_paxissession    = NULL;
      m_pbaseapp        = NULL;
      m_pbasesystem     = NULL;
      m_pbasesession    = NULL;
      m_pcoreapp        = NULL;
      m_pcoresystem     = NULL;
      m_pcoresession   = NULL;
      m_peventReady     = NULL;

#ifdef WINDOWS

      m_hinstance       = NULL;

#endif

      if(m_paceapp == NULL)
      {

         m_paceapp              = this;

      }

      if(m_paceapp != NULL)
      {

         m_pacesystem           = m_paceapp->m_pacesystem;

         m_paxissystem           = m_paceapp->m_paxissystem;

         m_paxissession          = m_paceapp->m_paxissession;

         m_paxisapp              = m_paceapp->m_paxisapp;

         m_pbasesystem           = m_paceapp->m_pbasesystem;

         m_pbasesession          = m_paceapp->m_pbasesession;

         m_pbaseapp              = m_paceapp->m_pbaseapp;

         m_pcoresystem           = m_paceapp->m_pcoresystem;

         m_pcoresession         = m_paceapp->m_pcoresession;

         m_pcoreapp              = m_paceapp->m_pcoreapp;

         if(m_paceapp->m_pacesession == NULL && m_pacesystem != NULL)
         {

            m_pacesession       = m_pacesystem->m_pacesession;

         }
         else
         {

            m_pacesession       = m_paceapp->m_pacesession;

         }

#ifdef WINDOWS

         m_hinstance             = m_paceapp->m_hinstance;

#endif

      }
      else
      {

         m_pacesystem           = NULL;

      }

//      m_pcoreapp                 = NULL;





      m_psignal                  = canew(class signal());

      m_pcommandthread           = canew(::command_thread(this));

      m_bLicense                 = false;

      m_bAuraProcessInitialize         = false;
      m_bAuraProcessInitializeResult   = false;

      m_bAuraInitialize1               = false;
      m_bAuraInitialize1Result         = false;

      m_bAuraInitialize                = false;
      m_bAuraInitializeResult          = false;

      m_bAuraInitializeInstance        = false;
      m_bAuraInitializeInstanceResult  = false;

      m_pinitmaindata                  = NULL;

#if defined(LINUX)

      ::linux::shell::theLinuxShell.Initialize();

#elif defined(ANDROID)

      ::android::shell::theLinuxShell.Initialize();

#endif // LINUX

   }


   application::~application()
   {

      if (m_peventReady != NULL)
      {

         delete m_peventReady;

      }

      if (m_pinitmaindata != NULL)
      {

         delete m_pinitmaindata;

      }

   }


   void application::assert_valid() const
   {

      thread::assert_valid();

   }


   void application::dump(dump_context & dumpcontext) const
   {

      thread::dump(dumpcontext);

#ifdef WINDOWS

      dumpcontext << "m_hinstance = " << (void *)m_hinstance;

#endif

      //dumpcontext << "\nm_strCmdLine = " << m_strCmdLine;
      //dumpcontext << "\nm_nCmdShow = " << m_nCmdShow;
      dumpcontext << "\nm_bHelpMode = " << m_strAppName;

      dumpcontext << "\n";

   }


   void application::install_message_handling(::message::dispatch * pdispatch)
   {

      ::message::dispatch::install_message_handling(pdispatch);
      ::signalizable::install_message_handling(pdispatch);
      ::thread::install_message_handling(pdispatch);

   }


   void application::on_command(::primitive::command * pcommand)
   {

      if(pcommand->m_ecommand == ::primitive::command_on_agree_exit)
      {

         m_bAgreeExit = _001OnAgreeExit();

         m_bAgreeExitOk = true;

      }
      else if (pcommand->m_ecommand == ::primitive::command_france_exit)
      {

         _001OnFranceExit();

         m_bFranceExit = true;

      }
      else
      {

         ::thread::on_command(pcommand);

      }

   }

   bool application::check_install()
   {

      return true;

   }

   void application::throw_not_installed()
   {

      string strBuildNumber = System.command()->m_varTopicQuery["build_number"];

      if(strBuildNumber.is_empty())
      {

         strBuildNumber = "installed";

      }

       throw not_installed(get_app(),System.install_get_version(),strBuildNumber,"application",m_strAppName,Session.m_strLocale,Session.m_strSchema);
   }



   void application::on_create(::create * pcreate)
   {

      pcreate->set_app(this);


      try
      {

         if (pcreate->m_spCommandLine.is_set()
            && (pcreate->m_spCommandLine->m_varQuery.has_property("install")
               || pcreate->m_spCommandLine->m_varQuery.has_property("uninstall")))
         {

            if (!is_system() && !is_session())
            {

               check_install();

               System.post_quit();

            }
            else
            {

               on_request(pcreate);

            }

         }
         else
         {

            on_request(pcreate);

         }

      }
      catch (not_installed & e)
      {

         System.on_run_exception(e);

         throw exit_exception(e.get_app());

      }
      catch (::exit_exception & e)
      {

         throw e;

      }
      catch (::exception::exception & e)
      {

         if (!Application.on_run_exception(e))
            throw exit_exception(get_app());

      }
      catch (...)
      {

      }


      // Verry Sory for the per request overhead here for the needed information of only first request
      if (System.m_dwAfterApplicationFirstRequest == 0)
      {

         System.m_dwAfterApplicationFirstRequest = ::get_tick_count(); // cross your fingers that the first recorded is not 0, it will be cleaned up by other requests.

      }

      try
      {
         pcreate->m_spCommandLine->m_varQuery["document"].unset();
      }
      catch (...)
      {
      }
      //sp(::core::session) pbergedge = pcreatecontext->m_spCommandLine->m_varQuery["bergedge_callback"].cast < ::core::session >();
      // todobergedge
      /*if(pbergedge != NULL)
      {
      pbergedge->on_app_request_bergedge_callback(this);
      }*/
      pcreate->m_spCommandLine->m_eventReady.SetEvent();

   }

   //void application::tellme_destroyed(::user::primitive * pui, bool * pDestroyed, bool bTell)
   //{

   //   UNREFERENCED_PARAMETER(pui);
   //   UNREFERENCED_PARAMETER(pDestroyed);
   //   UNREFERENCED_PARAMETER(bTell);

   //}


   int32_t application::simple_message_box(::user::primitive * puiOwner,const char * pszMessage,UINT fuStyle)
   {

      return ::simple_message_box(get_safe_handle(puiOwner), pszMessage, m_strAppName, fuStyle);

   }


   int32_t application::simple_message_box(const char * pszMessage,UINT fuStyle)
   {

      return ::simple_message_box(NULL,pszMessage,m_strAppName,fuStyle);

   }

   string application::message_box(const string & pszMatter,property_set & propertyset)
   {

      simple_message_box(pszMatter,0);

      return "";

   }

   // lang string
   // load string
   string application::lstr(id id, const string & strDefault)
   {

      string str;

      if(!load_string(str,id))
      {

         if(strDefault.has_char())
            return strDefault;

         return "";

      }

      return str;

   }


   string application::load_string(id id)
   {
      string str;
      if(!load_string(str,id))
      {
         return (const string &)id;
      }
      return str;
   }

   bool application::load_string(string & str,id id)
   {
      if(!load_cached_string(str,id,true))
      {
         return false;
      }
      return true;
   }

   bool application::load_cached_string(string & str,id id,bool bLoadStringTable)
   {

      ::xml::document doc(this);

      if(!doc.load(id))
      {

         if(load_cached_string_by_id(str,id,bLoadStringTable))
         {

            return true;

         }

      }

      sp(::xml::node) pnodeRoot = doc.get_root();

      if(pnodeRoot->get_name() == "string")
      {

         string strId = pnodeRoot->attr("id");

         if (load_cached_string_by_id(str, strId, bLoadStringTable))
         {

            return true;
         }

         str = pnodeRoot->get_value();

         return true;

      }

      return false;

   }


   bool application::load_cached_string_by_id(string & str,id id,bool bLoadStringTable)
   {

      synch_lock sl(&m_mutexStr);

      string strId(id.str());

      string strTable;

      string strString;

      string_to_string * pmap = NULL;

      index iFind = 0;

      if((iFind = strId.find(':')) <= 0)
      {

         strTable = "";

         strString = strId;

      }
      else
      {

         strTable = strId.Mid(0,iFind);

         strString = strId.Mid(iFind + 1);

      }

      if(m_stringtableStd.Lookup(strTable,pmap))
      {

         if(pmap->Lookup(strString,str))
         {

            return true;

         }

      }
      else if(m_stringtable.Lookup(strTable,pmap))
      {

         if(pmap->Lookup(strString,str))
         {

            return true;

         }

      }
      else if(bLoadStringTable)
      {

         load_string_table(strTable,"");

         return load_cached_string_by_id(str,id,false);

      }

      return false;

   }


   void application::load_string_table(const string & pszApp,const string & pszId)
   {

   }




   void application::load_string_table()
   {
      load_string_table("","");
   }


   object * application::alloc(sp(type) info)
   {
      return System.alloc(this,info);
   }

   object * application::alloc(const  id & idType)
   {
      return System.alloc(this,idType);
   }

   bool application::is_system()
   {

      return false;

   }

   bool application::is_session()
   {

      return false;

   }


   bool application::is_serviceable()
   {

      return false;

   }


   bool application::is_user_service()
   {

      bool bIsUserService = true;

      bool bIsService = is_serviceable();

      return bIsUserService && bIsService;

   }

   bool application::app_map_lookup(const char * psz,void * & p)
   {
      return m_appmap.Lookup(psz,p) != FALSE;
   }

   void application::app_map_set(const char * psz,void * p)
   {
      m_appmap.set_at(psz,p);
   }




   bool application::open_link(const string & strLink,const string & pszTarget)
   {

      if(is_system())
      {
         return open_url::start(this, strLink, pszTarget);
      }
      else
      {
         return System.open_link(strLink,pszTarget);
      }

      return false;

   }


   void application::construct(const char * pszAppId)
   {

   }


   void application::TermThread(HINSTANCE hInstTerm)
   {

      ::exception::throw_interface_only(this);

   }


   //string application::get_version()
   //{

   //   if (m_pappimpl.is_null())
   //   {

   //      return "";

   //   }

   //   return m_pappimpl->get_version();

   //}


   void application::SetCurrentHandles()
   {

      dappy(string(typeid(*this).name()) + " : SetCurrentHandles 1 : " + ::str::from(m_iReturnCode));

      set_os_data((void *) ::get_current_thread());

      dappy(string(typeid(*this).name()) + " : SetCurrentHandles 2 : " + ::str::from(m_iReturnCode));

      set_os_int(::get_current_thread_id());

      dappy(string(typeid(*this).name()) + " : SetCurrentHandles impled : " + ::str::from(m_iReturnCode));



   }



   bool application::_001OnDDECommand(const char * lpcsz)
   {

      ::exception::throw_interface_only(this);

      return false;

   }



   /*::user::document *  application::_001OpenDocumentFile(var varFile)
   {

      ::exception::throw_interface_only(this);

      return NULL;

   }*/


   void application::_001OnFileNew(signal_details * pobj)
   {

      ::exception::throw_interface_only(this);

   }


   //bool application::update_module_paths()
   //{


   //   if(is_system())
   //   {

   //      if(!m_pimpl->update_module_paths())
   //         return false;


   //   }

   //   return true;


   //}











   void application::process_machine_event_data(machine_event_data * pdata)
   {

      if(pdata->m_fixed.m_bRequestCloseApplication)
      {

         _001CloseApplication();

      }

   }


   void application::_001CloseApplication()
   {

      throw todo(get_app());

   }












   void application::DoWaitCursor(int32_t nCode) // 0 => restore, 1=> begin, -1=> end
   {

      UNREFERENCED_PARAMETER(nCode);

   }


//   void application::ShowWaitCursor(bool bShow)
//   {
//
//      if (m_pappimpl.is_null())
//         return;
//
//      m_pappimpl->ShowWaitCursor(bShow);
//
//
//   }
//
//
//#ifndef METROWIN
//
//
//   void application::get_time(timeval *p)
//   {
//
//      m_pappimpl->get_time(p);
//
//   }
//
//
//#endif


   string CLASS_DECL_ACE application::get_cred(const string & strRequestUrl,const RECT & rect,string & strUsername,string & strPassword,string strToken,string strTitle,bool bInteractive)
   {

      throw not_implemented(this);

   }




   bool application::get_temp_file_name_template(string & strRet,const char * pszName,const char * pszExtension,const char * pszTemplate)
   {

      throw not_implemented(this);

      return false;

   }


   bool application::get_temp_file_name(string & strRet,const char * pszName,const char * pszExtension)
   {

      return get_temp_file_name_template(strRet,pszName,pszExtension,NULL);

   }











   bool application::final_handle_exception(::exception::exception & e)
   {
      UNREFERENCED_PARAMETER(e);
      //linux      exit(-1);

      if(!is_system())
      {

         // get_app() may be it self, it is ok...
         if(Sys(get_app()).final_handle_exception((::exception::exception &) e))
            return true;


      }

      return false;
   }





   int32_t application::main()
   {

      TRACE(string(typeid(*this).name()) + " main");;

      dappy(string(typeid(*this).name()) + " : application::main 1");

      try
      {

         TRACE(string(typeid(*this).name()) + " on_run");;
         dappy(string(typeid(*this).name()) + " : going to on_run : " + ::str::from(m_iReturnCode));
         m_iReturnCode = 0;
         m_bReady = true;
         m_bRun = true;
         m_iReturnCode = on_run();
         if(m_iReturnCode != 0)
         {
            dappy(string(typeid(*this).name()) + " : on_run failure : " + ::str::from(m_iReturnCode));
            ::output_debug_string("application::main on_run termination failure");
         }

      }
      catch(::exit_exception &)
      {

         dappy(string(typeid(*this).name()) + " : on_run exit_exception");

         System.post_thread_message(WM_QUIT);

         goto exit_application;

      }
      catch(...)
      {

         dappy(string(typeid(*this).name()) + " : on_run general exception");

         goto exit_application;

      }

      try
      {

         if(is_system())
         {

            dappy(string(typeid(*this).name()) + " : quiting main");

            //post_to_all_threads(WM_QUIT,0,0);

            //Sleep(5000);

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

         post_to_all_threads(WM_QUIT,0,0);

         m_iReturnCode = -1;

      }
      catch(...)
      {

         m_iReturnCode = -1;

      }

      return m_iReturnCode;

   }




   bool application::pre_run()
   {

      if(!::thread::pre_run())
         return false;

      TRACE(string(typeid(*this).name()) + " main_start");;
      try
      {

         m_dwAlive = ::get_tick_count();
         TRACE(string(typeid(*this).name()) + "application_pre_run");;
         int32_t m_iReturnCode = application_pre_run();
         if(m_iReturnCode != 0)
         {
            dappy(string(typeid(*this).name()) + " : applicationpre_run failure : " + ::str::from(m_iReturnCode));
            m_bReady = true;
            TRACE("application::main application_pre_run failure");
            return false;
         }

         xxdebug_box("pre_runnned","pre_runnned",MB_ICONINFORMATION);
         dappy(string(typeid(*this).name()) + " : pre_runned : " + ::str::from(m_iReturnCode));
         TRACE(string(typeid(*this).name()) + " initial_check_directrix");;
         if(!initial_check_directrix())
         {
            dappy(string(typeid(*this).name()) + " : initial_check_directrix failure");
            m_iReturnCode = -1;
            exit();
            m_bReady = true;
            ::output_debug_string("exiting on check directrix");
            return false;
         }


         TRACE(string(typeid(*this).name()) + " os_native_bergedge_start");;
         m_dwAlive = ::get_tick_count();
         if(!os_native_bergedge_start())
         {
            dappy(string(typeid(*this).name()) + " : os_native_bergedge_start failure");
            exit();
            m_iReturnCode = -1;
            m_bReady = true;
            ::output_debug_string("application::main os_native_bergedge_start failure");
            return false;
         }

         return true;
      }
      catch(::exit_exception &)
      {

         dappy(string(typeid(*this).name()) + " : main_start exit_exception");

         post_to_all_threads(WM_QUIT,0,0);

      }

      return false;

   }




   int32_t application::on_run()
   {
      int32_t m_iReturnCode = 0;

      try
      {
         application_signal_details signal(m_psignal,application_signal_start);
         m_psignal->emit(&signal);
      }
      catch(...)
      {
      }

      dappy(string(typeid(*this).name()) + " : starting on_run : " + ::str::from(m_iReturnCode));

      ::output_debug_string(string(typeid(*this).name()) + " : starting on_run : " + ::str::from(m_iReturnCode) + "\n\n");

      thread * pthread = ::get_thread();

      install_message_handling(pthread);

      dappy(string(typeid(*this).name()) + " : starting on_run 2 : " + ::str::from(m_iReturnCode));

      ::output_debug_string(string(typeid(*this).name()) + " : starting on_run 2 : " + ::str::from(m_iReturnCode) + "\n\n");

      try
      {
         try
         {
            m_bReady = true;
            if(m_peventReady != NULL)
               m_peventReady->SetEvent();
         }
         catch(...)
         {
         }
      run:
         try
         {
            m_iReturnCode = run();

         }
         catch(::exit_exception & e)
         {

            throw e;

         }
         catch(const ::exception::exception & e)
         {
            if(on_run_exception((::exception::exception &) e))
               goto run;
            if(final_handle_exception((::exception::exception &) e))
               goto run;
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
         //               core, estamira and prodevian. Take care not to flood prodevian brand black ink over the floor of the estamira office...
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
   InitFailure:
      try
      {
         if(m_peventReady != NULL)
            m_peventReady->SetEvent();
      }
      catch(...)
      {
      }
      try
      {
         thread * pthread = this;
         if(pthread != NULL && pthread->m_pevReady != NULL)
         {
            pthread->m_pevReady->SetEvent();
         }
      }
      catch(...)
      {
      }
      /*try
      {
      thread * pthread = dynamic_cast < thread * > (this);
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




   int32_t application::application_pre_run()
   {


#ifdef WINDOWSEX

      MESSAGE msg;

      // Create Windows Message Queue
      ::PeekMessageA(&msg,NULL,0,0xffff,0);

      if(!is_system() && (bool)oprop("SessionSynchronizedInput"))
      {
         ::AttachThreadInput(GetCurrentThreadId(),(uint32_t)System.get_os_int(),TRUE);
      }

#endif

      m_iReturnCode = 0;

      m_dwAlive = ::get_tick_count();

      if(!InitApplication())
      {
         dappy(string(typeid(*this).name()) + " : InitApplication failure : " + ::str::from(m_iReturnCode));
         goto InitFailure;
      }


      //::simple_message_box(NULL,"e1","e1",MB_OK);

      m_dwAlive = ::get_tick_count();

      try
      {
         try
         {
            if(!process_initialize())
            {
               dappy(string(typeid(*this).name()) + " : process_initialize failure : " + ::str::from(m_iReturnCode));
               goto InitFailure;
            }
         }
         catch(::exit_exception & e)
         {

            throw e;

         }
         catch(const ::exception::exception &)
         {
            goto InitFailure;
         }


         dappy(string(typeid(*this).name()) + " : e2 : " + ::str::from(m_iReturnCode));
         //::simple_message_box(NULL,"e2","e2",MB_OK);

         System.install_progress_add_up();
         m_dwAlive = ::get_tick_count();
         try
         {

            if(!initialize_instance())
            {
               dappy(string(typeid(*this).name()) + " : initialize_instance failure : " + ::str::from(m_iReturnCode));
               if(System.directrix()->m_varTopicQuery["app"] == m_strAppName)
               {
                  System.post_quit();
               }
               try
               {
                  exit();
               }
               catch(...)
               {
               }
               goto InitFailure;
            }
         }
         catch(::exit_exception & e)
         {

            throw e;

         }
         catch(const ::exception::exception & e)
         {
            if(on_run_exception((::exception::exception &) e))
               goto run;
            if(final_handle_exception((::exception::exception &) e))
               goto run;
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
      catch(const char * psz)
      {
         TRACE("String Exception: This is the Error Message : \"%s\"", psz);
         m_iReturnCode = -666;
      }
      catch(...)
      {
         m_iReturnCode = -66;
      }
      goto run;
   InitFailure:
      if(m_iReturnCode == 0)
         m_iReturnCode = -1;
   run:
      return m_iReturnCode;

   }



   bool application::InitApplication()
   {

      return true;

   }



   bool application::initial_check_directrix()
   {

      return true;

   }


   bool application::on_install()
   {

      if(is_serviceable())
      {

         if(!create_service())
            return false;

         start_service();

      }

      return true;

   }


   bool application::on_uninstall()
   {

      //bool bOk = axis::application::on_uninstall();

      bool bOk = true;


      if(is_serviceable())
      {

         if(!remove_service())
            bOk = false;

      }

      return bOk;

   }


   bool application::system_add_app_install(const char * pszId)
   {

      ::exception::throw_not_implemented(get_app());

      return true;

   }


   bool application::os_native_bergedge_start()
   {

      return true;

   }











   //   void application::construct(const char *pszId)
   //   {
   //      //if(m_strAppName.has_char())
   //      //   return;
   //      //m_strAppName.Empty();
   //      //m_strId.Empty();
   //      if (pszId == NULL)
   //      {
   //#ifdef WINDOWSEX
   //         wstring wstr = ::GetCommandLineW();
   //         string str = ::str::international::unicode_to_utf8(wstr);
   //         strsize iFind = str.find(" : ");
   //         if (iFind >= 0)
   //         {
   //            iFind = str.find("app=", iFind);
   //            if (iFind >= 0)
   //            {
   //               strsize iEnd = str.find(" ", iFind);
   //               if (iEnd < 0)
   //               {
   //                  m_strId = str.Mid(iFind + 4);
   //               }
   //               else
   //               {
   //                  m_strId = str.Mid(iFind + 4, iEnd - iFind - 4);
   //               }
   //               ::str::begins_eat(m_strId, "\"");
   //               ::str::ends_eat(m_strId, "\"");
   //            }
   //         }
   //#endif
   //      }
   //      else
   //      {
   //         m_strId = pszId;
   //      }
   //      if (m_strId.is_empty())
   //         m_strId = "mplite";
   //      construct();
   //      if (m_strAppName.is_empty())
   //      {
   //         if (m_strAppId.has_char())
   //            m_strAppName = m_strAppId;
   //         else if (m_strInstallToken.has_char())
   //            m_strAppName = m_strInstallToken;
   //      }
   //   }


   //void application::construct()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }

   //   ::core::application::construct();

   //}




   //void application::_001OnFileNew()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   ::core::application::_001OnFileNew(NULL);
   //}


   //bool application::bergedge_start()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::bergedge_start();
   //}



   //bool application::on_install()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::on_install();
   //}

   //bool application::on_uninstall()
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::on_uninstall();
   //}


   void application::on_request(sp(::create) pcreatecontext)
   {

      ::object::on_request(pcreatecontext);

      command()->consolidate(pcreatecontext);

   }

   //bool application::is_serviceable()
   //{


   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::is_serviceable();
   //}

   //service_base * application::allocate_new_service()
   //{

   //   return NULL;

   //}


   //::user::document *  application::_001OpenDocumentFile(var varFile)
   //{
   //   string strId = m_strId;
   //   char chFirst = '\0';
   //   if (strId.get_length() > 0)
   //   {
   //      chFirst = strId[0];
   //   }
   //   return ::core::application::_001OpenDocumentFile(varFile);

   //}


   int32_t application::run()
   {

      if (!is_system() && !is_session())
      {

         if (command()->m_varTopicQuery.has_property("service"))
         {

            create_new_service();

            ::service_base::serve(*m_pservice);

         }
         else if (command()->m_varTopicQuery.has_property("run") || is_serviceable())
         {

            create_new_service();

            m_pservice->Start(0);

            return ::thread::run();

         }

      }

      return ::thread::run();

   }


   bool application::safe_is_running()
   {

      bool bRunning = false;

      try
      {

         if(is_running())
         {

            bRunning = true;

         }

      }
      catch(...)
      {

         bRunning = false;

      }


      return bRunning;

   }


   sp(application) application::assert_running(const char * pszAppId)
   {

      sp(application) papp;

      papp = Session.m_appptra.find_running_defer_try_quit_damaged(pszAppId);

      if(papp.is_null())
      {

         sp(::create) spcreatecontext(allocer());

         papp = Session.start_application("application",pszAppId,spcreatecontext);

      }

      return papp;

   }





   typedef  void(*PFN_ca2_factory_exchange)(sp(application) papp);




   /*::file::buffer_sp application::friendly_get_file(var varFile, UINT nOpenFlags)
   {

   try
   {

   return m_file.get_file(varFile, nOpenFlags);

   }
   catch (::file::exception & e)
   {

   string strMessage = e.get_message();

   App(this).simple_message_box(NULL, strMessage, MB_OK);

   return NULL;

   }

   }
   */




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

      if(m_pservice != NULL)
         return false;

      m_pservice = allocate_new_service();

      if(m_pservice == NULL)
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


   void application::on_service_request(sp(::create) pcreatecontext)
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







   bool application::process_initialize()
   {

      if(m_bAuraProcessInitialize)
         return m_bAuraProcessInitializeResult;

      m_bAuraProcessInitialize = true;
      m_bAuraProcessInitializeResult = false;

      m_spdir.alloc(allocer());
      m_spfile.alloc(allocer());



//      if(m_pthreadimpl == NULL)
//      {
//
//         m_pthreadimpl.alloc(allocer());
//
//         m_pthreadimpl->m_pthread = this;
//
//      }

      //m_pappimpl.alloc(allocer());

      //      impl_construct(NULL);

      if(::get_thread() == NULL)
      {
         ::set_thread(dynamic_cast <thread *> (this));
      }

      //if(is_system())
      //{
      //
      //
      //   TRACE("\n\n** %s **\n\n", "This command should work.");

      //   if(!update_module_paths())
      //      return false;

      //}


      if(!ca_process_initialize())
         return false;

      if(!impl_process_initialize())
         return false;

      m_bAuraProcessInitializeResult = true;

      return true;

   }


   bool application::initialize_instance()
   {

      if(m_bAuraInitializeInstance)
         return m_bAuraInitializeInstanceResult;

      m_bAuraInitializeInstance = true;
      m_bAuraInitializeInstanceResult = false;

      xxdebug_box("check_exclusive","check_exclusive",MB_ICONINFORMATION);

      if(!is_system() && !is_session())
      {

         try
         {
          
            m_pipi = create_ipi();

         }
         catch(...)
         {

         }

         if(!check_exclusive())
         {

            return false;

         }

         if(m_pipi != NULL)
         {

            try
            {

               m_pipi->ecall(m_pipi->m_strApp,{System.os().get_pid()},"application","on_new_instance",System.file().module(),System.os().get_pid());

            }
            catch(...)
            {

            }

         }

      }

      xxdebug_box("check_exclusive ok","check_exclusive ok",MB_ICONINFORMATION);

      m_dwAlive = ::get_tick_count();

      //::simple_message_box(NULL,"e2.b","e2.b",MB_OK);

      if(!initialize1())
      {
         dappy(string(typeid(*this).name()) + " : initialize1 failure : " + ::str::from(m_iReturnCode));
         return false;
      }



      //::simple_message_box(NULL,"e3","e3",MB_OK);


      System.install_progress_add_up(); // 2

      xxdebug_box("initialize1 ok","initialize1 ok",MB_ICONINFORMATION);

      /*
      string strWindow;

      if(m_strAppName.has_char())
      strWindow = m_strAppName;
      else
      strWindow = typeid(*this).name();

      #ifndef METROWIN

      if(!create_message_queue(this,strWindow))
      {
      dappy(string(typeid(*this).name()) + " : create_message_queue failure : " + ::str::from(m_iReturnCode));
      TRACE("Fatal error: could not initialize application message interaction_impl (name=\"%s\").",strWindow.c_str());
      return false;
      }

      #endif
      */

      m_dwAlive = ::get_tick_count();

      if(!initialize2())
      {
         dappy(string(typeid(*this).name()) + " : initialize2 failure : " + ::str::from(m_iReturnCode));
         return false;
      }

      System.install_progress_add_up(); // 3

      xxdebug_box("initialize2 ok","initialize2 ok",MB_ICONINFORMATION);

      m_dwAlive = ::get_tick_count();

      if(!initialize3())
      {
         dappy(string(typeid(*this).name()) + " : initialize3 failure : " + ::str::from(m_iReturnCode));
         return false;
      }

      System.install_progress_add_up(); // 4

      xxdebug_box("initialize3 ok","initialize3 ok",MB_ICONINFORMATION);

      m_dwAlive = ::get_tick_count();


      dappy(string(typeid(*this).name()) + " : initialize3 ok : " + ::str::from(m_iReturnCode));
      try
      {

         if(!initialize())
         {
            dappy(string(typeid(*this).name()) + " : initialize failure : " + ::str::from(m_iReturnCode));
            return false;
         }
      }
      catch(const char * psz)
      {
         if(!strcmp(psz,"You have not logged in! Exiting!"))
         {
            return false;
         }
         return false;
      }


      System.install_progress_add_up(); // 5

      m_bAuraInitializeInstanceResult = true;

//#ifndef METROWIN

//#endif

      return true;

   }

//#ifndef METROWIN

   ::ace::ipi * application::create_ipi()
   {

      return canew(::ace::ipi(this, m_strAppName));

   }

//#endif

   bool application::initialize1()
   {

      g_pf1 = (void *) (uint_ptr) ::str::to_uint64(file().as_string(::dir::system() / "config\\system\\pf1.txt"));

      if(m_bAuraInitialize1)
         return m_bAuraInitialize1Result;

      m_bAuraInitialize1 = true;

      m_bAuraInitialize1Result = false;

      m_dwAlive = ::get_tick_count();

      m_straMatterLocator.add_unique(System.dir_appmatter_locator(this));

      if(!ca_initialize1())
         return false;

      if(!impl_initialize1())
         return false;

      string strLocaleSystem;

      string strSchemaSystem;

      string strPath = System.dir().appdata() / "langstyle_settings.xml";

      if(file().exists(strPath))
      {

         string strSystem = file().as_string(strPath);

         ::xml::document docSystem(get_app());

         if(docSystem.load(strSystem))
         {

            if(docSystem.get_child("lang") != NULL)
            {

               strLocaleSystem = docSystem.get_child("lang")->get_value();

            }

            if(docSystem.get_child("style") != NULL)
            {

               strSchemaSystem = docSystem.get_child("style")->get_value();

            }

         }

      }

      string strLocale;

      string strSchema;

#ifdef METROWIN

      stringa stra;

      try
      {

         stra.explode("-",::Windows::Globalization::ApplicationLanguages::PrimaryLanguageOverride);

      }
      catch(long)
      {


      }

      strLocale = stra[0];

      strSchema = stra[0];

#elif defined(WINDOWS)

      LANGID langid = ::GetUserDefaultLangID();

#define SPR_DEUTSCH LANG_GERMAN

      if(langid == LANG_SWEDISH)
      {
         strLocale = "se";
         strSchema = "se";
      }
      else if(langid == MAKELANGID(LANG_PORTUGUESE,SUBLANG_PORTUGUESE_BRAZILIAN))
      {
         strLocale = "pt-br";
         strSchema = "pt-br";
      }
      else if(PRIMARYLANGID(langid) == SPR_DEUTSCH)
      {
         strLocale = "de";
         strSchema = "de";
      }
      else if(PRIMARYLANGID(langid) == LANG_ENGLISH)
      {
         strLocale = "en";
         strSchema = "en";
      }
      else if(PRIMARYLANGID(langid) == LANG_JAPANESE)
      {
         strLocale = "jp";
         strSchema = "jp";
      }
      else if(PRIMARYLANGID(langid) == LANG_POLISH)
      {
         strLocale = "pl";
         strSchema = "pl";
      }

#endif

      if(strLocale.is_empty())
         strLocale = "se";

      if(strSchema.is_empty())
         strSchema = "se";

      if(strLocaleSystem.has_char())
         strLocale = strLocaleSystem;

      if(strSchemaSystem.has_char())
         strSchema = strSchemaSystem;

      if(Sys(this).directrix()->m_varTopicQuery["locale"].get_count() > 0)
         strLocale = Sys(this).directrix()->m_varTopicQuery["locale"].stra()[0];

      if(Sys(this).directrix()->m_varTopicQuery["schema"].get_count() > 0)
         strSchema = Sys(this).directrix()->m_varTopicQuery["schema"].stra()[0];

      if(App(this).directrix()->m_varTopicQuery["locale"].get_count() > 0)
         strLocale = App(this).directrix()->m_varTopicQuery["locale"].stra()[0];

      if(App(this).directrix()->m_varTopicQuery["schema"].get_count() > 0)
         strSchema = App(this).directrix()->m_varTopicQuery["schema"].stra()[0];


      set_locale(strLocale,::action::source::database());
      set_schema(strSchema,::action::source::database());


      m_bAuraInitialize1Result = true;

      return true;

   }


   bool application::initialize2()
   {

      if(!impl_initialize2())
         return false;

      if(!ca_initialize2())
         return false;

      return true;

   }


   bool application::initialize3()
   {

      string strFolder = m_strAppName;

      strFolder.replace(".","_");
      strFolder.replace("::","-");
      strFolder.replace(":","_");

      m_strRelativeFolder = strFolder;

      if(!impl_initialize3())
         return false;

      if(!ca_initialize3())
         return false;

      return true;

   }


   bool application::initialize()
   {

      return true;


   }


   int32_t application::exit_instance()
   {


      try
      {


         //destroy_message_queue();

      }
      catch(...)
      {

         m_iReturnCode = -1;

      }




      try
      {


         /*      try
         {
         if(m_plemonarray != NULL)
         {
         delete m_plemonarray;
         }
         }
         catch(...)
         {
         }
         m_plemonarray = NULL;
         */

         release_exclusive();

         m_pcommandthread.release();


//         if(m_spuiMessage.is_set())
         {

            //if(!destroy_message_queue())
            {

               // TRACE("Could not finalize message interaction_impl");

            }

         }


         if(m_psignal != NULL)
         {

            application_signal_details signal(m_psignal,application_signal_exit_instance);

            try
            {

               m_psignal->emit(&signal);

            }
            catch(...)
            {

            }

            m_psignal.release();

         }

         //try
         //{
         //   if (!is_system())
         //   {
         //      System.unregister_bergedge_application(this);
         //   }
         //}
         //catch (...)
         //{
         //}

         /*try
         {
         ::release(smart_pointer <thread>::m_p);
         }
         catch(...)
         {
         }*/


         if(is_system())
         {

            //         try
            //       {
            //        if(m_spfilesystem.m_p != NULL)
            //      {
            //       ::core::del(m_spfilesystem.m_p);
            //  }
            //         }
            //       catch(...)
            //     {
            //   }
         }

         if (!is_session() && !is_system())
         {

            if (m_pacesystem != NULL && m_pacesystem->m_pcommandthread != NULL)
            {

               m_pacesystem->m_pcommandthread->command(::primitive::command_check_exit);

            }

         }

         for (auto & p : m_stringtable)
         {

            delete p.m_element2;

         }

         m_stringtable.remove_all();

         for (auto & p : m_stringtableStd)
         {

            delete p.m_element2;

         }

         m_stringtableStd.remove_all();

         try
         {

//            sp(thread_impl) pthread = m_pthreadimpl;
//
//            if(pthread != NULL)
//            {
//
//               try
//               {
                  // avoid calling CloseHandle() on our own thread handle
                  // during the thread destructor
                  // avoid thread object data auto deletion on thread termination,
                  // letting thread function terminate
                  //m_bAutoDelete = false;

                  set_run(false);

                  ::thread::exit_instance();

//               }
//               catch(...)
//               {
//
//               }
//
//            }

         }
         catch(...)
         {

         }

         /*try
         {

            m_pthreadimpl.release();

         }
         catch(...)
         {

         }
         */

         m_spfile.release();

         m_spdir.release();

         try
         {

            //application   * papp = m_pappimpl.detach();

            //if(papp != NULL && papp != this && !papp->is_system())
            //{

            //   try
            //   {

                  impl_exit_instance();

            //   }
            //   catch(...)
            //   {

            //   }

            //}

         }
         catch(...)
         {

         }

         return 0;


      }
      catch(...)
      {

         m_iReturnCode = -1;

      }

      try
      {

         if(Session.appptra().get_count() <= 1)
         {

            if(System.thread::get_os_data() != NULL)
            {
               System.post_thread_message(WM_QUIT);

            }

         }

      }
      catch(...)
      {

         m_iReturnCode = -1;

      }

      return m_iReturnCode;

   }


   bool application::finalize()
   {

      bool bOk = false;

      try
      {

         bOk = thread::finalize();

      }
      catch(...)
      {

         bOk = false;

      }

      return bOk;

   }






   bool application::is_running()
   {
      return is_alive();
   }

   service_base * application::allocate_new_service()
   {

      return NULL;

   }







   bool application::ca_initialize2()
   {

      application_signal_details signal(m_psignal,application_signal_initialize2);
      m_psignal->emit(&signal);
      return signal.m_bOk;

   }


   bool application::ca_initialize3()
   {

      application_signal_details signal(m_psignal,application_signal_initialize3);
      m_psignal->emit(&signal);
      if(!signal.m_bOk)
         return false;

      return true;

   }










   bool application::check_exclusive()
   {

#ifdef METROWIN

      return true;

#endif



      bool bSetOk;

      LPSECURITY_ATTRIBUTES lpsa = NULL;

      bool bResourceException = false;

#ifdef WINDOWSEX

      bSetOk = false;

      SECURITY_ATTRIBUTES MutexAttributes;
      ZeroMemory(&MutexAttributes,sizeof(MutexAttributes));
      MutexAttributes.nLength = sizeof(MutexAttributes);
      MutexAttributes.bInheritHandle = FALSE; // object uninheritable
      // declare and initialize a security descriptor
      SECURITY_DESCRIPTOR SD;
      bool bInitOk = InitializeSecurityDescriptor(&SD,SECURITY_DESCRIPTOR_REVISION) != FALSE;
      if(bInitOk)
      {
         // give the security descriptor a Null Dacl
         // done using the  "TRUE, (PACL)NULL" here
         bSetOk = SetSecurityDescriptorDacl(&SD,
            TRUE,
            (PACL)NULL,
            FALSE) != FALSE;
      }

      if(bSetOk)
      {

         MutexAttributes.lpSecurityDescriptor = &SD;

         lpsa = &MutexAttributes;

      }

#else

      bSetOk = true;

#endif

      if(bSetOk)
      {
         // Make the security attributes point
         // to the security descriptor
         bResourceException = false;
         try
         {
            m_pmutexGlobal = canew(mutex(this,FALSE,get_global_mutex_name(),lpsa));
         }
         catch(resource_exception &)
         {
            try
            {
               m_pmutexGlobal = canew(mutex(this,FALSE,get_global_mutex_name()));
            }
            catch(resource_exception &)
            {
               bResourceException = true;
            }
         }

         if(m_eexclusiveinstance == ExclusiveInstanceGlobal
            && (::GetLastError() == ERROR_ALREADY_EXISTS || bResourceException))
         {
            // Should in some way activate the other instance, but this is global, what to do? do not know yet.
            //System.simple_message_box("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same machine<br>Only one instance of this application can run globally: at the same machine.<br><br>Exiting this new instance.");
            TRACE("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same machine<br>Only one instance of this application can run globally: at the same machine.<br><br>Exiting this new instance.");
            on_exclusive_instance_conflict(ExclusiveInstanceGlobal);
            return false;
         }
         if(m_eexclusiveinstance == ExclusiveInstanceGlobalId)
         {
            bResourceException = false;
            try
            {
               m_pmutexGlobalId = canew(mutex(this,FALSE,get_global_id_mutex_name(),lpsa));
            }
            catch(resource_exception &)
            {
               try
               {
                  m_pmutexGlobalId = canew(mutex(this,FALSE,get_global_id_mutex_name()));
               }
               catch(resource_exception &)
               {
                  bResourceException = true;
               }
            }
            if(::GetLastError() == ERROR_ALREADY_EXISTS || bResourceException)
            {
               // Should in some way activate the other instance
               //System.simple_message_box("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same account.<br><br>Exiting this new instance.");
               TRACE("A instance of the application:<br><br>           - " + string(m_strAppName) + "with the id \"" + get_local_mutex_id() + "\" <br><br>seems to be already running at the same machine<br>Only one instance of this application can run globally: at the same machine with the same id.<br><br>Exiting this new instance.");
               on_exclusive_instance_conflict(ExclusiveInstanceGlobalId);
               return false;
            }
         }
         bResourceException = false;

#if defined(MACOS) || defined(LINUX)



         ::file::path p;

         p = "/var/tmp";

         p /= get_local_mutex_name();

         ::dir::mk(p.folder());

         int i = open(p,O_WRONLY | O_CREAT, 0777);

         int err =errno;

         i = lockf(i, F_TLOCK, 0);

         if(m_eexclusiveinstance == ExclusiveInstanceLocal && i<0)
#else



         try
         {
            m_pmutexLocal = canew(mutex(this,FALSE,get_local_mutex_name(),lpsa));
         }
         catch(resource_exception &)
         {
            try
            {
               m_pmutexLocal = canew(mutex(this,FALSE,get_local_mutex_name()));
            }
            catch(resource_exception &)
            {
               bResourceException = true;
            }
         }
         if(m_eexclusiveinstance == ExclusiveInstanceLocal && (::GetLastError() == ERROR_ALREADY_EXISTS || bResourceException))
#endif
         {
            try
            {
            // Should in some way activate the other instance
            //System.simple_message_box("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same account.<br><br>Exiting this new instance.");
            TRACE("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same account.<br><br>Exiting this new instance.");
            on_exclusive_instance_conflict(ExclusiveInstanceLocal);
            }
            catch(...)
            {
            }
            //System.post_quit();
            return false;
         }
         if(m_eexclusiveinstance == ExclusiveInstanceLocalId)
         {
            bResourceException = false;
            try
            {
               m_pmutexLocalId = canew(mutex(this,FALSE,get_local_id_mutex_name(),lpsa));
            }
            catch(resource_exception &)
            {
               try
               {
                  m_pmutexLocalId = canew(mutex(this,FALSE,get_local_id_mutex_name()));
               }
               catch(resource_exception &)
               {
                  bResourceException = true;
               }
            }
            if(::GetLastError() == ERROR_ALREADY_EXISTS || bResourceException)
            {
               try
               {
               // Should in some way activate the other instance
               //System.simple_message_box("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same account.<br><br>Exiting this new instance.");
               TRACE("A instance of the application:<br><br>           - " + string(m_strAppName) + "with the id \"" + get_local_mutex_id() + "\" <br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same ac::count with the same id.<br><br>Exiting this new instance.");
                  on_exclusive_instance_conflict(ExclusiveInstanceLocalId);
               }
               catch(...)
               {
               }
               release_exclusive();
               return false;
            }
         }
      }
      else
      {
         return false;
      }

      return true;

   }

   bool application::release_exclusive()
   {
      if(m_pmutexGlobal.is_set())
      {
         m_pmutexGlobal.release();
      }
      if(m_pmutexGlobalId.is_set())
      {
         m_pmutexGlobalId.release();
      }
      if(m_pmutexLocal.is_set())
      {
         m_pmutexLocal.release();
      }
      if(m_pmutexLocalId.is_set())
      {
         m_pmutexLocalId.release();
      }
      return true;
   }



   bool application::ca_process_initialize()
   {
      application_signal_details signal(m_psignal,application_signal_process_initialize);
      m_psignal->emit(&signal);
      return true;
   }

   bool application::ca_initialize1()
   {
      application_signal_details signal(m_psignal,application_signal_initialize1);
      m_psignal->emit(&signal);
      return signal.m_bOk;
   }



   bool application::ca_finalize()
   {
      application_signal_details signal(m_psignal,application_signal_finalize);
      try
      {
         m_psignal->emit(&signal);
      }
      catch(...)
      {
      }




      return signal.m_bOk;
   }



   string application::get_local_mutex_name(const char * pszAppName)
   {
      string strMutex;
      strMutex.Format("Local\\ca2_application_local_mutex:%s",pszAppName);
      return strMutex;
   }

   string application::get_local_id_mutex_name(const char * pszAppName,const char * pszId)
   {
      string strId(pszId);
      string strMutex;
      strMutex.Format("Local\\ca2_application_local_mutex:%s, id:%s",pszAppName,strId.c_str());
      return strMutex;
   }

   string application::get_global_mutex_name(const char * pszAppName)
   {
      string strMutex;
      strMutex.Format("Global\\ca2_application_global_mutex:%s",pszAppName);
      return strMutex;
   }

   string application::get_global_id_mutex_name(const char * pszAppName,const char * pszId)
   {
      string strId(pszId);
      string strMutex;
      strMutex.Format("Global\\ca2_application_global_mutex:%s, id:%s",pszAppName,strId.c_str());
      return strMutex;
   }

   string application::get_local_mutex_name()
   {
      return get_local_mutex_name(get_mutex_name_gen());
   }

   string application::get_local_id_mutex_name()
   {
      return get_local_id_mutex_name(get_mutex_name_gen(),get_local_mutex_id());
   }

   string application::get_global_mutex_name()
   {
      return get_global_mutex_name(get_mutex_name_gen());
   }

   string application::get_global_id_mutex_name()
   {
      return get_global_id_mutex_name(get_mutex_name_gen(),get_global_mutex_id());
   }




   void application::on_exclusive_instance_conflict(EExclusiveInstance eexclusive)
   {
      if(eexclusive == ExclusiveInstanceLocal)
      {
         on_exclusive_instance_local_conflict();
      }
   }

   void application::on_exclusive_instance_local_conflict()
   {

      try
      {

         if(m_pipi != NULL)
         {

            m_pipi->ecall(m_pipi->m_strApp,{System.os().get_pid()},"application","on_exclusive_instance_local_conflict",System.file().module(),System.os().get_pid(), string(System.directrix()->m_spcommandline->m_strCommandLine));

         }

      }
      catch(...)
      {


      }


   }


   void application::on_exclusive_instance_local_conflict(string strModule,int iPid, string strCommandLine)
   {


   }


   void application::on_new_instance(string strModule,int iPid)
   {

   }


   string application::get_mutex_name_gen()
   {
      return m_strAppName;
   }

   string application::get_local_mutex_id()
   {
      return command()->m_varTopicQuery["local_mutex_id"];
   }

   string application::get_global_mutex_id()
   {
      return command()->m_varTopicQuery["global_mutex_id"];
   }

   ::mutex * application::get_local_mutex()
   {
      return m_pmutexLocal;
   }

   ::mutex * application::get_global_mutex()
   {
      return m_pmutexGlobal;
   }



   bool application::Ex2OnAppInstall()
   {

      return true;

   }


   bool application::Ex2OnAppUninstall()
   {

      return true;

   }


   void application::on_set_scalar(e_scalar escalar,int64_t iValue,int iFlags)
   {

      //if (escalar == scalar_app_install_progress)
      //{

      //   m_iProgressInstallStep = iValue;

      //}
      //else if (escalar == scalar_app_install_progress_min)
      //{

      //   m_iProgressInstallStart = iValue;

      //}
      //else if (escalar == scalar_app_install_progress_max)
      //{

      //   m_iProgressInstallEnd = iValue;

      //}
      //else
      {

         return ::int_scalar_source::on_set_scalar(escalar,iValue, iFlags);

      }

   }


   void application::get_scalar_minimum(e_scalar escalar,int64_t & i)
   {

      //if (escalar == scalar_app_install_progress)
      //{

      //   i = m_iProgressInstallStart;

      //}
      //else if (escalar == scalar_app_install_progress_min)
      //{

      //   i = 0;

      //}
      //else if (escalar == scalar_app_install_progress_max)
      //{

      //   i = 0;

      //}
      //else
      {

         ::int_scalar_source::get_scalar_minimum(escalar,i);

      }

   }

   void application::get_scalar(e_scalar escalar,int64_t & i)
   {

      //if (escalar == scalar_app_install_progress)
      //{

      //   i = m_iProgressInstallStep;

      //}
      //else if (escalar == scalar_app_install_progress_min)
      //{

      //   i = m_iProgressInstallStart;

      //}
      //else if (escalar == scalar_app_install_progress_max)
      //{

      //   i = m_iProgressInstallEnd;

      //}
      //else
      {

         ::int_scalar_source::get_scalar(escalar,i);

      }

   }

   void application::get_scalar_maximum(e_scalar escalar,int64_t & i)
   {

      //if (escalar == scalar_download_size)
      //{

      //   i = m_iProgressInstallEnd;

      //}
      //else if (escalar == scalar_app_install_progress_min)
      //{

      //   i = 0x7fffffff;

      //}
      //else if (escalar == scalar_app_install_progress_max)
      //{

      //   i = 0x7fffffff;

      //}
      //else
      {

         ::int_scalar_source::get_scalar_minimum(escalar,i);

      }

   }


   int32_t application::simple_message_box_timeout(::user::primitive * pwndOwner,const char * pszMessage,::duration durationTimeOut,UINT fuStyle)
   {
      UNREFERENCED_PARAMETER(durationTimeOut);
      return simple_message_box(pwndOwner,pszMessage,fuStyle);
   }



   service_base * application::get_service()
   {

      return m_pservice;

   }








   application * application_ptra::find_by_app_name(const string & strAppName)
   {

      application * papp = NULL;

      for(int32_t i = 0; i < get_count(); i++)
      {
         try
         {

            papp = element_at(i).m_p;

            if(papp == NULL)
               continue;

            if(papp->m_strAppName == strAppName)
            {

               return papp;

            }

         }
         catch(...)
         {

         }

      }

      return NULL;


   }


   application * application_ptra::find_running_defer_try_quit_damaged(const string & strAppName)
   {

      sp(application) papp = find_by_app_name(strAppName);

      if(papp.is_null())
         return NULL;

      if(papp->safe_is_running())
         return papp;

      try
      {

         papp->post_thread_message(WM_QUIT);

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

      return NULL;

   }




   string application::get_license_id()
   {

      return m_strAppId;

   }





   bool application::gudo_get(const string & strKey,::file::serializable & obj)
   {

      ::file::path strPath(strKey);

      strPath.replace("::","/");

      synch_lock sl(System.m_spmutexUserAppData);

      {

         ::file::buffer_sp file = this->file().get_file(Application.dir().userappdata() / strPath,::file::mode_read);

         if(file.is_null())
         {

            return false;

         }

         ::file::buffered_buffer buffer(this,file);

         ::file::byte_istream is(&buffer);

         try
         {

            obj.read(is);

         }
         catch(...)
         {

         }

      }

      return true;

   }

   bool application::gudo_set(const string & strKey,::file::serializable & obj)
   {

      string strPath(strKey);

      strPath.replace("::","/");

      synch_lock sl(System.m_spmutexUserAppData);

      {

         ::file::buffer_sp file = this->file().get_file(Application.dir().userappdata() / strPath,::file::mode_write | ::file::mode_create | ::file::defer_create_directory);

         if(file.is_null())
         {

            return false;

         }

         ::file::buffered_buffer buffer(this, file);

         ::file::byte_ostream os(&buffer);

         try
         {

            obj.write(os);

         }
         catch(...)
         {

         }

      }

      return true;

   }


   bool application::assert_user_logged_in()
   {

      return false;

   }

   bool application::init_main_data(main_init_data * pdata)
   {

      m_pinitmaindata = pdata;

      return true;

   }

   //bool application::set_main_init_data(main_init_data * pdata)
   //{

   //   return m_pappimpl->set_main_init_data(pdata);

   //}






   //void application::dir_matter_ls_file(const string & str,stringa & stra)
   //{

   //   ::exception::throw_not_implemented(get_app());

   //}


   //string application::file().as_string(var varFile)
   //{

   //   return ::file_as_string_dup(varFile.get_string());

   //}


   //string application::file().as_string(var varFile,var & varQuery)
   //{

   //   return file().as_string(varFile);

   //}


   //string application::matter_as_string(const char * pszMatter,const char * pszMatter2)
   //{

   //   var varQuery;

   //   varQuery["disable_ca2_sessid"] = true;

   //   return file().as_string(dir().matter(pszMatter,pszMatter2),varQuery);

   //}

   //string application::dir().matter(const char * pszMatter,const char * pszMatter2)
   //{

   //   return dir().matter(pszMatter,pszMatter2);

   //}

   //bool application::is_inside_time_dir(const char * pszPath)
   //{
   //   throw not_implemented(this);
   //   return false;
   //}

   //bool application::file_is_read_only(const char * pszPath)
   //{
   //   throw not_implemented(this);
   //   return false;
   //}


   //bool application::file().exists(const char * pszPath)
   //{

   //   return ::file_exists_dup(pszPath) != FALSE;

   //}


   //bool application::file_is_equal_path(const char * pszPath1,const char * pszPath2)
   //{

   //   return ::file_is_equal_path_dup(pszPath1,pszPath2) != FALSE;

   //}


   //bool application::dir().id(const char * psz)
   //{

   //   return ::dir::is(psz);

   //}


   //bool application::file_del(const char * psz)
   //{

   //   return file_delete_dup(psz) != FALSE;

   //}

   //string application::file_extension(const char * pszPath)
   //{

   //   return ::file_extension_dup(pszPath);

   //}


   //string application::dir_userappdata(const char * lpcsz,const char * lpcsz2)
   //{

   //   throw not_implemented(this);

   //}

   //string application::dir_appdata(const char * lpcsz,const char * lpcsz2)
   //{

   //   throw not_implemented(this);

   //}


   //string application::dir_simple_path(const string & str1,const string & str2)
   //{

   //   return dir_path(str1,str2);

   //}


   //string application::dir_path(const char * psz1,const char * psz2,const char * psz3)
   //{

   //   return ::dir::path(psz1,psz2,psz3);

   //}


   //string application::dir_element(const char * psz)
   //{

   //   return ::dir::path(::dir::element(),psz);

   //}

   //string application::dir_ca2module(const char * psz)
   //{

   //   return ::dir::path(::dir::ca2_module(),psz);

   //}

   //string application::dir_name(const char * psz)
   //{

   //   return ::dir::name(psz);

   //}


   //void application::dir_ls_dir(const char * lpcsz,::file::patha & patha)
   //{

   //   ::exception::throw_not_implemented(get_app());

   //}


   //void application::dir_rls(const char * lpcsz,::file::patha & patha)
   //{

   //   ::exception::throw_not_implemented(get_app());

   //}


   //bool application::dir_mk(const char * psz)
   //{

   //   return ::dir::mk(psz);

   //}


   //string application::file_title(const char * psz)
   //{

   //   return ::file_title_dup(psz);

   //}


   //string application::file().name_(const char * psz)
   //{

   //   return ::file_name_dup(psz);

   //}

//#ifdef APPLEOS
//
//   string application::dir_pathfind(const char * pszEnv, const char * pszTopic, const char * pszMode)
//   {
//
//      ::exception::throw_interface_only(get_app());
//
//      return "";
//
//   }
//
//#endif
//

   //string application::file_time_square()
   //{

   //   //return get_temp_file_name_template(
   //   ::exception::throw_interface_only(get_app());

   //   return "";

   //}


   string application::http_get_locale_schema(const char * pszUrl,const char * pszLocale,const char * pszSchema)
   {

      ::exception::throw_interface_only(get_app());

      return "";

   }
/*

   ::file::buffer_sp application::file_get_file(var varFile,uint32_t uiFlags)
   {

      ::file::buffer_sp buffer(allocer());

      if(!buffer->open(varFile,uiFlags))
         return NULL;

      return buffer;

   }

*/

   void application::process_message_filter(int32_t code,signal_details * pobj)
   {

      //if(pobj == NULL)
      //   return;   // not handled

      //SCAST_PTR(::message::base,pbase,pobj);

      //sp(::user::frame_window) pTopFrameWnd;
      //::user::primitive * pMainWnd;
      //::user::primitive * pMsgWnd;
      //switch(code)
      //{
      ////case MSGF_DDEMGR:
      //   // Unlike other WH_MSGFILTER codes, MSGF_DDEMGR should
      //   //  never call the next hook.
      //   // By returning FALSE, the message will be dispatched
      //   //  instead (the default behavior).
      //   //return;

      //case MSGF_MENU:
      //   pMsgWnd = pbase->m_pwnd;
      //   if(pMsgWnd != NULL)
      //   {
      //      pTopFrameWnd = pMsgWnd->GetTopLevelFrame();
      //      if(pTopFrameWnd != NULL && pTopFrameWnd->IsTracking() &&
      //         pTopFrameWnd->m_bHelpMode)
      //      {
      //         //pMainWnd = __get_main_window();
      //         //if((m_puiMain != NULL) && (IsEnterKey(pbase) || IsButtonUp(pbase)))
      //         //{
      //         //   //                  pMainWnd->SendMessage(WM_COMMAND, ID_HELP);
      //         //   pbase->m_bRet = true;
      //         //   return;
      //         //}
      //      }
      //   }
      //   // fall through...

      //case MSGF_DIALOGBOX:    // handles message boxes as well.
      //   //pMainWnd = __get_main_window();
      //   if(code == MSGF_DIALOGBOX && m_pthread->m_puiActive != NULL &&
      //      pbase->m_uiMessage >= WM_KEYFIRST && pbase->m_uiMessage <= WM_KEYLAST)
      //   {
      //      //// need to translate messages for the in-place container
      //      //___THREAD_STATE* pThreadState = __get_thread_state();
      //      //ENSURE(pThreadState);

      //      //if (pThreadState->m_bInMsgFilter)
      //      //   return;
      //      //pThreadState->m_bInMsgFilter = TRUE;    // avoid reentering this code
      //      //if (m_puiActive->is_window_enabled())
      //      //{
      //      //   pre_translate_message(pobj);
      //      //   if(pobj->m_bRet)
      //      //   {
      //      //      pThreadState->m_bInMsgFilter = FALSE;
      //      //      return;
      //      //   }
      //      //}
      //      //pThreadState->m_bInMsgFilter = FALSE;    // ok again
      //   }
      //   break;
      //}
      //// default to not handled

   }

   bool application::on_thread_on_idle(::thread * pthread,LONG lCount)
   {

      UNREFERENCED_PARAMETER(pthread);

      return lCount < 0;

   }


   bool application::post_user_message(::thread * pthread,::user::primitive * pui,UINT message,WPARAM wparam,lparam lparam)
   {

      UNREFERENCED_PARAMETER(pthread);
      UNREFERENCED_PARAMETER(pui);
      UNREFERENCED_PARAMETER(message);
      UNREFERENCED_PARAMETER(wparam);
      UNREFERENCED_PARAMETER(lparam);

      return false;

   }


   void application::message_handler(signal_details * pobj)
   {

      ::thread::message_handler(pobj);

   }


   bool application::is_window(::user::primitive * pui)
   {

      UNREFERENCED_PARAMETER(pui);

      return false;

   }

   LRESULT application::send_message(::user::primitive * pui,UINT message,WPARAM wparam,lparam lparam)
   {

      UNREFERENCED_PARAMETER(pui);
      UNREFERENCED_PARAMETER(message);
      UNREFERENCED_PARAMETER(wparam);
      UNREFERENCED_PARAMETER(lparam);

      return 0;

   }

   oswindow application::get_safe_handle(::user::primitive * pui)
   {

      UNREFERENCED_PARAMETER(pui);

      return NULL;

   }


   void application::dispatch_user_message(::signal_details * pbase)
   {

      UNREFERENCED_PARAMETER(pbase);

   }


   ::user::primitive * application::get_parent(::user::primitive * pui)
   {

      UNREFERENCED_PARAMETER(pui);

      return NULL;

   }

   bool application::enable_window(::user::primitive * pui,bool bEnable)
   {

      UNREFERENCED_PARAMETER(pui);
      UNREFERENCED_PARAMETER(bEnable);

      return false;

   }


   bool application::set_window_text(::user::primitive * pui,const string & strText)
   {

      UNREFERENCED_PARAMETER(pui);
      UNREFERENCED_PARAMETER(strText);

      return false;

   }



   //void application::window_graphics_update_window(window_graphics ** ppdata,oswindow interaction_impl,COLORREF * pOsBitmapData,const RECT & rect,int cxParam,int cyParam,int iStride,bool bTransferBuffer)
   //{

   //   // derived classes should implement this function in order to update window

   //}

   sp(::message::base) application::get_message_base(LPMESSAGE lpmsg)
   {

      return NULL;

   }


   void application::process_message(signal_details * pobj)
   {

      message_handler(pobj);

   }


   void application::set_locale(const string & lpcsz,::action::context actioncontext)
   {
      string strLocale(lpcsz);
      strLocale.trim();
      m_strLocale = strLocale;
      on_set_locale(m_strLocale,actioncontext);
   }

   void application::set_schema(const string & lpcsz,::action::context actioncontext)
   {
      string strSchema(lpcsz);
      strSchema.trim();
      m_strSchema = strSchema;
      on_set_schema(m_strSchema,actioncontext);
   }

   void application::on_set_locale(const string & lpcsz,::action::context actioncontext)
   {
      UNREFERENCED_PARAMETER(actioncontext);
      UNREFERENCED_PARAMETER(lpcsz);
      //System.appa_load_string_table();
   }

   void application::on_set_schema(const string & lpcsz,::action::context actioncontext)
   {
      UNREFERENCED_PARAMETER(actioncontext);
      UNREFERENCED_PARAMETER(lpcsz);
      //System.appa_load_string_table();
   }


   string application::get_locale()
   {
      return m_strLocale;
   }

   string application::get_schema()
   {
      return m_strSchema;
   }


   ::file::path application::get_locale_schema_dir()
   {

      return ::file::path(get_locale())/get_schema();

   }


   ::file::path application::get_locale_schema_dir(const string & strLocale)
   {

      if(strLocale.is_empty())
      {

         return ::file::path(get_locale())/get_schema();

      }
      else
      {

         return ::file::path(strLocale) /get_schema();

      }

   }


   ::file::path application::get_locale_schema_dir(const string & strLocale,const string & strSchema)
   {

      if(strLocale.is_empty())
      {

         if(strSchema.is_empty())
         {

            return ::file::path(get_locale()) / get_schema();

         }
         else
         {

            return ::file::path(get_locale()) /strSchema;

         }

      }
      else
      {

         if(strSchema.is_empty())
         {

            return ::file::path(strLocale) /get_schema();

         }
         else
         {

            return ::file::path(strLocale) / strSchema;

         }

      }

   }


   void application::fill_locale_schema(::str::international::locale_schema & localeschema,const string & pszLocale,const string & pszSchema)
   {


      localeschema.m_idaLocale.remove_all();
      localeschema.m_idaSchema.remove_all();


      string strLocale(pszLocale);
      string strSchema(pszSchema);


      localeschema.m_idLocale = pszLocale;
      localeschema.m_idSchema = pszSchema;


      localeschema.add_locale_variant(strLocale,strSchema);
      localeschema.add_locale_variant(get_locale(),strSchema);
      localeschema.add_locale_variant(__id(std),strSchema);
      localeschema.add_locale_variant(__id(en),strSchema);


      localeschema.finalize();


   }


   void application::fill_locale_schema(::str::international::locale_schema & localeschema)
   {


      localeschema.m_idaLocale.remove_all();
      localeschema.m_idaSchema.remove_all();


      //localeschema.m_bAddAlternateStyle = true;


      stringa straLocale;
      stringa straSchema;

      straLocale.add(get_locale());
      straSchema.add(get_schema());


      stringa stra;

      stra = Application.directrix()->m_varTopicQuery["locale"].stra();

      stra.remove_ci("_std");

      straLocale.add_unique(Application.directrix()->m_varTopicQuery["locale"].stra());

      stra = Application.directrix()->m_varTopicQuery["schema"].stra();

      stra.remove_ci("_std");

      straSchema.add_unique(Application.directrix()->m_varTopicQuery["schema"].stra());


      localeschema.m_idLocale = straLocale[0];
      localeschema.m_idSchema = straSchema[0];

      for(index iLocale = 0; iLocale < straLocale.get_count(); iLocale++)
      {

         for(index iSchema = 0; iSchema < straSchema.get_count(); iSchema++)
         {

            localeschema.add_locale_variant(straLocale[iLocale],straSchema[iSchema]);

         }

      }

      for(index iSchema = 0; iSchema < straSchema.get_count(); iSchema++)
      {

         localeschema.add_locale_variant(get_locale(),straSchema[iSchema]);

      }

      for(index iSchema = 0; iSchema < straSchema.get_count(); iSchema++)
      {

         localeschema.add_locale_variant(__id(std),straSchema[iSchema]);

      }


      for(index iSchema = 0; iSchema < straSchema.get_count(); iSchema++)
      {

         localeschema.add_locale_variant(__id(en),straSchema[iSchema]);

      }

      localeschema.finalize();


   }


//   void application::defer_add_thread_run_wait(sync_object_ptra & soa)
//   {
//
////      soa.add(&axiom()->m_ev);
//
//   }
//

   bool application::platform_open_by_file_extension(index iEdge,const char * pszPathName,application_bias * pbiasCreate)
   {

      return false;

   }


   bool application::platform_open_by_file_extension(index iEdge,::create * pcc)
   {

      return false;

   }


   ::ace::application * application::instantiate_application(const char * pszType,const char * pszId,application_bias * pbias)
   {

      ::ace::application * papp;

      string strId(pszId);

      if(strId.CompareNoCase("session") == 0)
      {

         papp = create_platform(m_paceapp->m_pacesession);

         papp->construct("session");

         papp->m_strAppId = "session";

      }
      else
      {

         string strNewId;

         if(strId == "bergedge")
         {

            strNewId = "app/core/bergedge";

         }
         else if(strId == "cube")
         {

            strNewId = "app/core/cube";

         }
         else
         {

            strNewId = strId;

         }

         papp = Session.get_new_app(pbias == NULL ? this : pbias->get_app(),pszType,strNewId);

         if(papp == NULL)
            return NULL;

         papp->m_pacesession = m_pacesession;

         papp->m_paxissession = m_paxissession;

         papp->m_pbasesession = m_pbasesession;

         if(papp != NULL)
         {

            if(strId == "bergedge" || strId == "cube")
            {

               papp->m_strAppId = strId;

            }

            if(papp->m_strInstallToken.is_empty())
            {

               papp->m_strInstallToken = papp->m_strAppId;

            }

         }

      }

      papp->m_pcoresystem = m_pcoresystem;

      papp->m_pbasesystem = m_pbasesystem;

      papp->command_central()->consolidate(System.command_central());

      papp->command_central()->consolidate(command_central());

      //   papp->m_bSystemSynchronizedCursor = m_bSystemSynchronizedCursor;

      if(pbias != NULL)
      {

         papp->oprop_set().merge(pbias->m_set);

      }
      else
      {

         papp->oprop("SessionSynchronizedInput") = true;
         papp->oprop("NativeWindowFocus") = true;

      }

      if((papp == NULL || papp->m_strAppId != strId)
         &&
         (!Application.command()->m_varTopicQuery.has_property("install")
         && !Application.command()->m_varTopicQuery.has_property("uninstall")))
      {

         TRACE("Failed to instantiate %s, going to try installation through ca2_cube_install",strId);

         string strCommandLine;

         strCommandLine = " : app=" + strId;
         strCommandLine += " locale=" + string(Session.str_context()->m_plocaleschema->m_idLocale);
         strCommandLine += " style=" + string(Session.str_context()->m_plocaleschema->m_idSchema);
         strCommandLine += " install";

         System.install_start(strCommandLine,Application.command()->m_varTopicQuery["build_number"]);

         throw installing_exception(get_app());

         return NULL;

      }

      return papp;

   }


   ::ace::application * application::create_application(const char * pszType,const char * pszId,bool bSynch,application_bias * pbias)
   {

      ::ace::application * pbaseapp = instantiate_application(pszType,pszId,pbias);

      if(pbaseapp == NULL)
         return NULL;

      ::ace::application * papp = pbaseapp;

      if(!papp->start_application(bSynch,pbias))
      {

         /*
         MessageBox(NULL, "application::start_application failed at create_application(\""+string(pszType)+"\",\""+string(pszId)+"\")", "ace::application::create_application", MB_ICONEXCLAMATION | MB_OK);
         */

         return NULL;

      }

      return pbaseapp;

   }

   ::ace::application * application::create_platform(::ace::session * psession)
   {

      return NULL;

   }


   bool application::start_application(bool bSynch,application_bias * pbias)
   {
      /*      try
      {
      if(pbias != NULL)
      {
      papp->m_pcoreapp->m_puiInitialPlaceHolderContainer = pbias->m_puiParent;
      }
      }
      catch(...)
      {
      }*/
      try
      {
         if(pbias != NULL)
         {
            if(pbias->m_pcallback != NULL)
            {
               pbias->m_pcallback->connect_to(this);
            }
         }
      }
      catch(...)
      {
      }

      manual_reset_event * peventReady = NULL;

      if(bSynch)
      {
         peventReady = new manual_reset_event(get_app());
         m_peventReady = peventReady;
         peventReady->ResetEvent();
      }

//      m_pthreadimpl.alloc(allocer());
//
//      m_pthreadimpl->m_pthread = this;

      if(pbias != NULL)
      {

         m_biasCalling = *pbias;

      }

      if(bSynch)
      {

         if(!begin_synch(&m_iReturnCode))
            return false;

      }
      else
      {

         begin();

      }


      return true;

   }



   bool application::on_run_exception(::exception::exception & e)
   {

      ::output_debug_string("ace::application::on_run_exception An unexpected error has occurred and no special exception handling is available.");

      if(e.m_bHandled)
      {

         return !e.m_bContinue;

      }

      if(typeid(e) == typeid(not_installed))
      {

         not_installed & notinstalled = dynamic_cast <not_installed &> (e);

         return handle_not_installed(notinstalled);

      }

      //simple_message_box_timeout("An unexpected error has occurred and no special exception handling is available.<br>Timeout: $simple_message_box_timeout", 5000);

      return true; // continue or exit application? by default: continue by returning true

   }


   bool application::handle_not_installed(::not_installed & notinstalled)
   {

      notinstalled.m_bHandled = true;

      notinstalled.m_bContinue = true;

      bool bDebuggerCheck = true;

#ifdef APPLEOS

      bDebuggerCheck = false;

#endif

      if(((!bDebuggerCheck || ::is_debugger_attached()) && !file_exists_dup(::dir::system() / "config\\plugin\\disable_manual_install_warning.txt")
         && !file_exists_dup(::dir::system() / "config\\system\\skip_debug_install.txt")) || file_exists_dup(::dir::system() / "config\\system\\enable_debug_install.txt"))
         //|| (App(notinstalled.get_app()).is_serviceable() && !App(notinstalled.get_app()).is_user_service()))
      {

         try
         {

            string strModuleFilePath;

            DWORD dwExitCode = 0;

            bool bTimedOut = false;

            string strParam;

            string strPath = notinstalled.m_strId;

            strPath = System.dir().ca2module() / "app";

            string strBuildNumber = "latest";

            if(strcmp(g_pszCooperativeLevel,"core") != 0)
               strPath += "." + string(g_pszCooperativeLevel);

#ifdef WINDOWS

            strPath += ".exe";

#elif defined(APPLEOS)


            //                     strPath += ".app/Contents/MacOS/app";
            // please be extremely careful with code below.
            // Package app.app project with the <your-app> project!
            // The code below will seem to be guilty and blameable if you skipp app.app packaging with your app.
//            ::file::path path = get_exe_path();
//
//            index i = 5;
//
//            while(i >= 0)
//            {
//
//               path = path.folder();
//
//               strPath = path / "app";
//
//               output_debug_string("\n xyzxyzx " + strPath + "\n");
//               output_debug_string("\n xyzxyzx " + strPath + "\n");
//               output_debug_string("\n xyzxyzx " + strPath + "\n");
//               output_debug_string("\n xyzxyzx " + strPath + "\n");
//               output_debug_string("\n xyzxyzx " + strPath + "\n");
//
//               if(Application.file().exists(strPath))
//               {
//
//                  output_debug_string("\n OK OK K " + strPath + "\n");
//
//                  break;
//
//               }
//
//               i--;
//
//            }
//
//            //                     setenv("DYLD_FALLBACK_LIBRARY_PATH",System.dir().ca2module(), 1 );
//            //                     setenv("DYLD_FALLBACK_LIBRARY_PATH",strPath, 1 );

            strPath = get_exe_path();

            ::output_debug_string("\n\n xyzxyzx OK OK K " + strPath + "\n\n");

            strPath = "\"" + strPath + "\"";

#endif

            strModuleFilePath = strPath;

            //#if defined(APPLEOS)
            //                   strPath = "/usr/bin/open -n " + strPath + " --args : app=" + notinstalled.m_strId + " install build_number=" + strBuildNumber + " locale=" + notinstalled.m_strLocale + " schema=" + //notinstalled.m_strSchema;
            //#else
            strParam = " : app=" + notinstalled.m_strId + " install build_number=" + notinstalled.m_strBuild + " version=" + notinstalled.m_strVersion + " locale=" + notinstalled.m_strLocale + " schema=" + notinstalled.m_strSchema;
            //#endif

            //               if(App(notinstalled.get_app()).is_serviceable() && !App(notinstalled.get_app()).is_user_service())
            //               {
            //
            //
            //                  HANDLE hToken = NULL;
            //
            //                  //if(LogonUserW(L"LocalServer",L"NT AUTHORITY",NULL,LOGON32_LOGON_SERVICE,LOGON32_PROVIDER_DEFAULT,&hToken))
            //                  //{
            //                  //
            //                  //   ::simple_message_box(NULL,"Failed to Login at Local System Account","Debug only message, please install.",MB_ICONINFORMATION | MB_OK);
            //                  //}
            //
            //                  // os << "Impersonation OK!!<br>";
            //
            ////                  strPath = "\"" + System.file().name_(strModuleFilePath) + "\" : install";
            //                  string strCmdLine = strPath + strParam;
            //                  wstring wstrCmdLine = strCmdLine;
            //                  string strModuleFolder = System.get_module_folder();
            //                  wstring wstrModuleFolder = strModuleFolder;
            //                  LPSTR lpsz = (char *)(const char *)(strCmdLine);
            //                  LPWSTR lpwsz = (unichar *)(const unichar *)(wstrCmdLine);
            //                  STARTUPINFO m_si;
            //                  PROCESS_INFORMATION m_pi;
            //                  memset(&m_si,0,sizeof(m_si));
            //                  memset(&m_pi,0,sizeof(m_pi));
            //                  m_si.cb = sizeof(m_si);
            //                  m_si.wShowWindow = SW_HIDE;
            //
            //                  if(LaunchAppIntoSystemAcc(strModuleFilePath,lpsz,strModuleFolder,&m_si,&m_pi))
            //                  //if(LaunchAppIntoDifferentSession(strModuleFilePath,lpsz,strModuleFolder,&m_si,&m_pi, 0))
            //                  //if(::CreateProcessAsUserW(hToken,wstring(strModuleFilePath),lpsz,NULL,NULL,TRUE,CREATE_NEW_CONSOLE,NULL,wstrModuleFolder,&m_si,&m_pi))
            //                  {
            //                     TRACE("Launched");
            //
            //
            //                  }
            //                  else
            //                  {
            //                     uint32_t dwLastError = ::GetLastError();
            //                     TRACE("Not Launched");
            //
            //                     WCHAR wsz[1024];
            //
            //                     DWORD dwSize = sizeof(wsz) / sizeof(WCHAR);
            //
            //                     GetUserNameW(wsz,&dwSize);
            //
            //                     string strUserName = wsz;
            //
            //                     if(strUserName != "NetworkService")
            //                     {
            //
            //                        goto Launch;
            //
            //                     }
            //
            //                  }
            //
            //                  dwExitCode = 0;
            //
            //                  while(::GetExitCodeProcess(m_pi.hProcess,&dwExitCode))
            //                  {
            //
            //                     if(dwExitCode == STILL_ACTIVE)
            //                     {
            //
            //                        Sleep(84);
            //
            //                     }
            //                     else
            //                     {
            //                        break;
            //                     }
            //
            //                  }
            //
            //               }
            //else
            int iRet = IDNO;
            output_debug_string("\n not_installed Params " + strParam + "\n");

            {

               if(!(bool)System.oprop("not_installed_message_already_shown"))
               {
                  if((App(notinstalled.get_app()).is_serviceable() && !App(notinstalled.get_app()).is_user_service())
                     || (IDYES == (iRet = ::simple_message_box(NULL,"Debug only message, please install:\n\n\n\t" + notinstalled.m_strId + "\n\ttype = " + notinstalled.m_strType + "\n\tlocale = " + notinstalled.m_strLocale + "\n\tschema = " + notinstalled.m_strSchema + "\n\tbuild number = " + notinstalled.m_strBuild + "\n\n\nThere are helper scripts under <solution directory>/nodeapp/stage/install/","Debug only message, please install.",MB_ICONINFORMATION | MB_YESNO))))
                  {




                     ::duration durationWait = seconds((1.9841115 + 1.9770402 + 1.9510422) * 8.0);

                     //#ifdef MACOS

                     //                   TRACE0(strPath);

                     //                 DWORD dwExitCode = System.process().synch(strPath,SW_HIDE,durationWait,&bTimedOut);

                     //#else

                     #ifdef LINUX

                     dwExitCode = System.process().synch(strPath + strParam,SW_HIDE,durationWait,&bTimedOut);

                     #else

                     dwExitCode = System.process().elevated_synch(strPath + strParam,SW_HIDE,durationWait,&bTimedOut);

                     #endif

                     //#endif

                  }


               }

            }
            if(iRet == IDNO)
            {

               notinstalled.m_bContinue = false;

            }
            else if(bTimedOut)
            {

               ::simple_message_box(NULL," - " + notinstalled.m_strId + "\nhas timed out while trying to install.\n\nFor developers it is recommended to\nfix this installation timeout problem.\n\nIt is recommended to kill manually :\n - \"" + strPath + strParam + "\"\nif it has not been terminated yet.","Debug only message, please install.",MB_ICONINFORMATION | MB_OK);

               notinstalled.m_bContinue = false;

            }
            else if(dwExitCode == 0)
            {

               ::simple_message_box(NULL,"Successfully run : " + strPath + strParam,"Debug only message, please install.",MB_ICONINFORMATION | MB_OK);

               notinstalled.m_bContinue = false;

            }
            else
            {

               ::simple_message_box(NULL,strPath + strParam + "\n\nFailed return code : " + ::str::from((uint32_t)dwExitCode),"Debug only message, please install.",MB_ICONINFORMATION | MB_OK);

               notinstalled.m_bContinue = false;

            }


         }
         catch(...)
         {

         }

      }
      else
      {

         string strAddUp;

         if(System.directrix()->m_varTopicQuery.has_property("enable_desktop_launch"))
         {

            if(System.directrix()->m_varTopicQuery["enable_desktop_launch"].has_char())
            {

               strAddUp = " enable_desktop_launch=" + System.directrix()->m_varTopicQuery["enable_desktop_launch"];

            }
            else
            {

               strAddUp = " enable_desktop_launch=" + System.directrix()->m_varTopicQuery["app"];

            }

         }

         hotplugin_host_starter_start_sync(": app=" + notinstalled.m_strId + " app_type=" + notinstalled.m_strType + " install locale=" + notinstalled.m_strLocale + " schema=" + notinstalled.m_strSchema + " version=" + notinstalled.m_strVersion + strAddUp,get_app(),NULL);

      }

      return false;

   }


   int32_t application::hotplugin_host_starter_start_sync(const char * pszCommandLine,::ace::application * papp,hotplugin::host * phost,hotplugin::plugin * pplugin)
   {

      return -1;

   }


   bool application::is_application()
   {

      return !is_session() && !is_system();

   }


   ::file::listing & application::perform_file_listing(::file::listing & listing)
   {

      return dir().ls(listing);

   }

   bool application::_001OnAgreeExit()
   {

      return true;

   }

   void application::_001OnFranceExit()
   {

   }

   void application::dispatch_user_message_object(::object * pobject)
   {

      throw interface_only_exception(this);

   }


} // namespace ace













