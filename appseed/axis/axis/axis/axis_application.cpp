#include "framework.h"
#include "fiasco_finder.h"
#include "aura/compress/zip/zip.h"


namespace axis
{


   application::application()
   {

      m_psimpledb = canew(::simpledb::simpledb(this));

      m_bInitializeDataCentral = true;

      if (&System != NULL)
      {
         m_bInitializeDataCentral = System.m_bInitializeDataCentral;

      }

      m_bAxisProcessInitialize         = false;
      m_bAxisProcessInitializeResult   = false;

      m_bAxisInitialize1               = false;
      m_bAxisInitialize1Result         = false;

      m_bAxisInitialize                = false;
      m_bAxisInitializeResult          = false;

      m_bAxisInitializeInstance        = false;
      m_bAxisInitializeInstanceResult  = false;


      m_pdataserver = NULL;

      m_iWaitCursorCount         = 0;

      m_pcommand = NULL;

   }

   application::~application()
   {
   }


   void application::assert_valid() const
   {

      thread::assert_valid();


   }

   void application::dump(dump_context & dumpcontext) const
   {

      aura::application::dump(dumpcontext);

   }


   string application::load_string(id id)
   {

      synch_lock sl(&m_mutexStr);

      string str;

      if(m_stringmap.Lookup(id, str))
      {

         return str;

      }

      sl.unlock();

      if(!load_string(str,id))
      {

         id.to_string(str);

         return str;

      }

      sl.lock();

      m_stringmap.set_at(id, str);

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

         return load_cached_string_by_id(str,id,bLoadStringTable);

      }

      sp(::xml::node) pnodeRoot = doc.get_root();

      if(pnodeRoot->get_name() == "string")
      {

         string strId = pnodeRoot->attr("id");

         if (!load_cached_string_by_id(str, strId, bLoadStringTable))
         {

            str = pnodeRoot->get_value();

         }

         return true;

      }

      return false;

   }


   bool application::load_cached_string_by_id(string & str,id id,bool bLoadStringTable)
   {

      string strId(id.str());

      string strTable;

      string strString;

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

      synch_lock sl(&m_mutexStr);

      string_to_string * pmap = NULL;

      if(m_stringtableStd.Lookup(strTable,pmap))
      {

         if(pmap != NULL && pmap->Lookup(strString,str))
         {

            return true;

         }

      }
      else if(m_stringtable.Lookup(strTable,pmap))
      {

         if(pmap != NULL && pmap->Lookup(strString,str))
         {

            return true;

         }

      }
      else if(bLoadStringTable)
      {

         sl.unlock();

         load_string_table(strTable,"");

         return load_cached_string_by_id(str,id,false);

      }

      return false;

   }


   void application::load_string_table(const string & pszApp,const string & pszId)
   {

      string strApp(pszApp);

      string strMatter;

      string strLocator;

      if(strApp.is_empty())
      {

         strLocator = matter_locator(this);

      }
      else
      {

         strLocator = matter_locator(strApp);

      }

      if(pszId.is_empty())
      {
         strMatter = "stringtable.xml";
      }
      else
      {
         strMatter = pszId + ".xml";
      }

      string strTableId = strApp;

      if(pszId.has_char() && *pszId != '\0')
      {

         strTableId += "\\";

         strTableId += pszId;

      }

      string_to_string * pmapOld = NULL;

      {

         synch_lock sl(&m_mutexStr);

         pmapOld = m_stringtable[strTableId];

         m_stringtable[strTableId] = NULL;

      }

      ::aura::del(pmapOld);

      fork([this, strTableId, strLocator, strMatter]()
      {

         ::xml::document doc(get_app());

         string strFilePath = System.dir().matter_from_locator(this, strLocator, { strLocator }, strMatter);

         if (!System.file().exists(strFilePath, this))
         {

            return;

         }

         string strFile = Application.file().as_string(strFilePath);

         if (!doc.load(strFile))
         {

            return;

         }

         string_to_string * pmapNew = new string_to_string;

         for (int32_t i = 0; i < doc.get_root()->children().get_count(); i++)
         {

            string strId = doc.get_root()->child_at(i)->attr("id");

            string strValue = doc.get_root()->child_at(i)->get_value();

            pmapNew->set_at(strId, strValue);

         }

         string_to_string * pmapOld = NULL;

         {

            synch_lock sl(&m_mutexStr);

            pmapOld = m_stringtable[strTableId];

            m_stringtable[strTableId] = pmapNew;

         }

         ::aura::del(pmapOld);

      });

   }




   void application::load_string_table()
   {
      load_string_table("","");
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


   bool application::verb()
   {
      //axiom()->run();

      return true;

   }


   void application::SetCurrentHandles()
   {

      ::aura::application::SetCurrentHandles();

//      if (is_installing() || is_unstalling())
//      {
//
//         if (is_system())
//         {
//
//            System.install().trace().initialize();
//
//         }
//
//      }

   }




   //void application::set_env_var(const string & var,const string & value)
   //{

   //   ::exception::throw_interface_only(this);

   //}




   //bool application::_001OnDDECommand(const char * lpcsz)
   //{

   //   ::exception::throw_interface_only(this);

   //   return false;

   //}


   //void application::_001EnableShellOpen()
   //{

   //   ::exception::throw_interface_only(this);

   //}


   //::user::document * application::_001OpenDocumentFile(var varFile)
   //{

   //   ::exception::throw_interface_only(this);

   //   return NULL;

   //}


   //void application::_001OnFileNew(::message::message * pobj)
   //{

   //   ::exception::throw_interface_only(this);

   //}


   //sp(::aura::printer) application::get_printer(const char * pszDeviceName)
   //{

   //   ::exception::throw_interface_only(this);

   //}


   //bool application::update_module_paths()
   //{


   //   if(is_system())
   //   {

   //      if(!m_pimpl->update_module_paths())
   //         return false;


   //   }

   //   return true;


   //}











   /*
      void application::process_machine_event_data(machine_event_data * pdata)
      {
         if(pdata->m_fixed.m_bRequestCloseApplication)
         {
            _001CloseApplication();
         }
      }
      */

   void application::_001CloseApplication()
   {

      Sleep(500);
      
      if(m_puiMain != NULL)
      {

         m_puiMain->post_message(WM_CLOSE);
         
      }


   }





//   sp(::user::interaction) application::release_capture_uie()
//   {
//
//#if defined(LINUX)
//
//      oswindow oswindowCapture = ::GetCapture();
//      if (oswindowCapture == NULL)
//         return NULL;
//      return oswindowCapture->get_user_interaction()->ReleaseCapture();
//
//#elif defined(WINDOWS)
//
//      oswindow oswindowCapture = ::GetCapture();
//      if(oswindowCapture == NULL)
//         return NULL;
//      return System.ui_from_handle(oswindowCapture)->ReleaseCapture();
//
//#elif defined(APPLEOS)
//
//      oswindow oswindowCapture = ::GetCapture();
//      if (oswindowCapture == NULL)
//         return NULL;
//      return oswindowCapture->get_user_interaction()->ReleaseCapture();
//
//#else
//
//      ::exception::throw_not_implemented(get_app());
//
//#endif
//
//   }
//
//
//   sp(::user::interaction) application::get_capture_uie()
//   {
//
//#if defined(METROWIN)
//
//      oswindow oswindowCapture = ::GetCapture();
//
//      if (oswindowCapture == NULL)
//         return NULL;
//
//      ::user::interaction * pui = oswindowCapture->window();
//
//      if (pui == NULL)
//         return NULL;
//
//      return pui->GetCapture();
//
//#elif defined(WINDOWS) || defined(APPLE_IOS)
//
//      oswindow oswindowCapture = ::GetCapture();
//
//      if(oswindowCapture == NULL)
//         return NULL;
//
//      sp(::user::interaction) pui = System.ui_from_handle(oswindowCapture);
//
//      if(pui == NULL)
//         return NULL;
//
//      return pui->GetCapture();
//
//#else
//
//      //      ::exception::throw_not_implemented(get_app());
//
//      oswindow oswindowCapture = ::GetCapture();
//
//      if (oswindowCapture == NULL)
//         return NULL;
//
//      return ::GetCapture()->get_user_interaction()->m_pimpl->GetCapture();
//
//#endif
//
//   }
//
//
//
//
//
//
//
//   uint32_t application::get_thread_id()
//   {
//      return m_pimpl->get_thread_id();
//   }
//
//
//

//
//#ifndef METROWIN
//
//   void application::get_time(timeval *p)
//   {
//      m_pimpl->get_time(p);
//   }
//
//#endif
//

   //bool application::do_prompt_file_name(var & varFile,UINT nIDSTitle,uint32_t lFlags,bool bOpenFileDialog,sp(::user::impact_system) ptemplate, ::user::document * pdocument)
   //{

   //   UNREFERENCED_PARAMETER(varFile);
   //   UNREFERENCED_PARAMETER(nIDSTitle);

   //   return false;

   //}

   //string CLASS_DECL_AXIS application::get_cred(const string & strRequestUrl,const RECT & rect,string & strUsername,string & strPassword,string strToken,string strTitle,bool bInteractive)
   //{

   //   return ::fontopus::get_cred(this,strRequestUrl, rect,strUsername,strPassword,strToken,strTitle,bInteractive);

   //}













   /*

      ::visual::icon * application::set_icon(object * pobject,::visual::icon * picon,bool bBigIcon)
      {

         ::visual::icon * piconOld = get_icon(pobject,bBigIcon);

         if(bBigIcon)
         {

            pobject->oprop("big_icon").operator =((sp(object)) picon);

         }
         else
         {

            pobject->oprop("small_icon").operator =((sp(object)) picon);

         }

         return piconOld;

      }


      ::visual::icon * application::get_icon(object * pobject,bool bBigIcon) const
      {

         if(bBigIcon)
         {

            return const_cast <object *> (pobject)->oprop("big_icon").cast < ::visual::icon >();

         }
         else
         {

            return const_cast <object *> (pobject)->oprop("small_icon").cast < ::visual::icon >();

         }

      }
   */


   bool application::final_handle_exception(::exception::exception * pexception)
   {

      UNREFERENCED_PARAMETER(pexception);
      //linux      exit(-1);

      if(!is_system())
      {

         // get_app() may be it self, it is ok...
         if(System.final_handle_exception(pexception))
            return true;


      }

      return false;
   }


   void application::main()
   {

      ::aura::application::main();

   }


   //bool application::pre_run()
   //{

   //   TRACE(string(typeid(*this).name()) + " main_start");;
   //   try
   //   {

   //      m_dwAlive = ::get_tick_count();
   //      TRACE(string(typeid(*this).name()) + "application_pre_run");;
   //      int32_t m_iErrorCode = application_pre_run();
   //      if(m_iErrorCode != 0)
   //      {
   //         dappy(string(typeid(*this).name()) + " : applicationpre_run failure : " + ::str::from(m_iErrorCode));
   //         m_bReady = true;
   //         TRACE("application::main application_pre_run failure");
   //         return false;
   //      }

   //      xxdebug_box("pre_runnned","pre_runnned",MB_ICONINFORMATION);
   //      dappy(string(typeid(*this).name()) + " : pre_runned : " + ::str::from(m_iErrorCode));
   //      TRACE(string(typeid(*this).name()) + " initial_check_directrix");;
   //      if(!initial_check_directrix())
   //      {
   //         dappy(string(typeid(*this).name()) + " : initial_check_directrix failure");
   //         m_iErrorCode = -1;
   //         exit_thread();
   //         m_bReady = true;
   //         ::output_debug_string("exiting on check handler\n");
   //         return false;
   //      }


   //      TRACE(string(typeid(*this).name()) + " os_native_bergedge_start");;
   //      m_dwAlive = ::get_tick_count();
   //      if(!os_native_bergedge_start())
   //      {
   //         dappy(string(typeid(*this).name()) + " : os_native_bergedge_start failure");
   //         exit_thread();
   //         m_iErrorCode = -1;
   //         m_bReady = true;
   //         ::output_debug_string("application::main os_native_bergedge_start failure");
   //         return false;
   //      }

   //      return true;
   //   }
   //   catch(::exit_exception &)
   //   {

   //      dappy(string(typeid(*this).name()) + " : main_start exit_exception");

   //      ::multithreading::post_quit(&System);

   //   }

   //   return false;

   //}



   void application::on_run()
   {


      try
      {

         ::aura::application_message signal(::aura::application_message_beg);

         route_message(&signal);

      }
      catch(...)
      {

      }

      //dappy(string(typeid(*this).name()) + " : starting on_run : " + ::str::from(m_iErrorCode));

      thread * pthread = ::get_thread();

      install_message_routing(pthread);

      //dappy(string(typeid(*this).name()) + " : starting on_run 2 : " + ::str::from(m_iErrorCode));

      try
      {

         try
         {

            m_bReady = true;

            if (m_pevAppBeg != NULL)
            {

               m_pevAppBeg->SetEvent();

            }

         }
         catch(...)
         {

         }

run:
         try
         {

            run();

         }
         catch (exit_exception * pexception)
         {

            _rethrow(pexception);

         }
         catch (::exception::exception * pexception)
         {

            esp671 esp(pexception);

            if (on_run_exception(esp))
            {

               goto run;

            }

            if (final_handle_exception(esp))
            {

               goto run;

            }

            try
            {

               term_thread();

            }
            catch (exit_exception * pexception)
            {

               _rethrow(pexception);

            }
            catch (::exception::exception * pexception)
            {

               esp671 esp(pexception);

               m_error.set(-1);

               goto InitFailure;

            }

         }

      }
      catch (exit_exception * pexception)
      {

         _rethrow(pexception);

      }
      catch (::exception::exception * pexception)
      {

         esp671 esp(pexception);

      }
      catch (...)
      {

      }

InitFailure:

      try
      {

         if (m_pevAppEnd != NULL)
         {

            m_pevAppEnd->SetEvent();

         }

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

         m_error.set_if_not_set();

      }

//      try
//      {
//
//         term_thread();
//
//      }
//      catch (...)
//      {
//
//
//      }
//
   }


   bool application::init_application()
   {

      return ::aura::application::init_application();

   }


   bool application::application_pre_run()
   {

      return ::aura::application::application_pre_run();

   }



   bool application::InitApplication()
   {

      return true;

   }



   bool application::initial_check_directrix()
   {

      if (!::aura::application::initial_check_directrix())
      {

         return false;

      }

      //string strLicense = get_license_id();

      //var & varTopicQuey = System.handler()->m_varTopicQuery;

      //bool bHasInstall = varTopicQuey.has_property("install");

      //bool bHasUninstall = varTopicQuey.has_property("uninstall");

      //if(!(bHasInstall || bHasUninstall)
      //   && m_bLicense
      //   && strLicense.has_char())
      //{

      //   if(!Session.assert_user_logged_in())
      //   {
      //      return false;
      //   }

      //   // call application's is_licensed function
      //   // because if delay is needed for authentication -
      //   // or either asking for authentication -
      //   // current application startup won't be
      //   // exited by timeout.

      //   int32_t iRetry = 1;

      //retry_license:

      //   iRetry--;

      //   if(!Session.is_licensed(strLicense))
      //   {

      //      if(iRetry > 0)
      //         goto retry_license;

      //      return false;

      //   }

      //}


      //::output_debug_string("initial_check_directrix : ok ("+string(typeid(*this).name())+")" + m_strAppId + "\n\n");


      return true;

   }


   bool application::do_install()
   {


      if (!::aura::application::do_install())
      {

         return false;

      }

      /*
       if (handler()->m_varTopicQuery.has_property("install"))
       {

       if (!on_install())
       {

       ::output_debug_string("Failed at on_install : " + m_strAppId + "\n\n");

       System.m_iErrorCode = -1;

       return false;

       }

       string strBuild;

       string strAppId = m_strAppId;

       xxdebug_box("on_install1", strAppId, 0);

       if (strAppId.is_empty())
       {

       strAppId = m_strAppName;

       }

       if (strAppId.has_char() && handler()->m_varTopicQuery.has_property("app") && strAppId == handler()->m_varTopicQuery["app"])
       {

       system_add_app_install(strAppId, "installed");

       if (strBuild.has_char())
       {

       system_add_app_install(strAppId, strBuild);

       }

       }
       else if (strAppId.has_char() && handler()->m_varTopicQuery.has_property("session_start") && strAppId == handler()->m_varTopicQuery["session_start"])
       {

       system_add_app_install(strAppId, "installed");

       if (strBuild.has_char())
       {

       system_add_app_install(strAppId, strBuild);

       }

       }
       else if (m_strInstallToken.has_char())
       {

       system_add_app_install(m_strInstallToken, "installed");

       if (strBuild.has_char())
       {

       system_add_app_install(m_strInstallToken, strBuild);

       }

       }

       }
       else if (handler()->m_varTopicQuery.has_property("uninstall"))
       {

       if (!on_uninstall())
       {

       return false;

       }

       #ifdef

       System.install().remove_spa_start(m_strAppId);

       #endif

       }*/

      return true;

   }




   bool application::do_uninstall()
   {

      bool bOk = aura::application::do_uninstall();

      return bOk;

   }


   bool application::on_install()
   {

      if(!aura::application::on_install())
         return false;

      return true;

   }



   bool application::on_uninstall()
   {

      if(!aura::application::on_uninstall())
      {

         return false;

      }

      return true;

   }





   bool application::os_native_bergedge_start()
   {

      if (!::aura::application::os_native_bergedge_start())
      {

         return false;

      }

      return true;

   }











   void application::on_request(::create * pcreate)
   {

      ::object::on_request(pcreate);

      handler()->merge(pcreate);

   }


   void application::run()
   {

      ::aura::application::run();

   }


   sp(::aura::application) application::assert_running(const char * pszAppId)
   {

      sp(::aura::application) papp;

      papp = Session.m_appptra.find_running_defer_try_quit_damaged(pszAppId);

      if(papp.is_null())
      {

         sp(::create) spcreate(allocer());

         papp = Session.start_application(pszAppId,spcreate);

      }

      return papp;

   }




   /*
      typedef  void(*PFN_ca2_factory_exchange)(sp(application) papp);

   */


   /*::file::file_sp application::friendly_get_file(var varFile, UINT nOpenFlags)
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




   //bool application::is_installing()
   //{

   //   return handler()->has_property("install");

   //}


   //bool application::is_unstalling()
   //{

   //   return handler()->has_property("uninstall");

   //}


   //bool application::create_new_service()
   //{

   //   if(m_pservice != NULL)
   //      return false;

   //   m_pservice = allocate_new_service();

   //   if(m_pservice == NULL)
   //      return false;

   //   return true;

   //}



   //bool application::create_service()
   //{

   //   return System.os().create_service(this);

   //}

   //bool application::remove_service()
   //{

   //   return System.os().remove_service(this);

   //}

   //bool application::start_service()
   //{

   //   return System.os().start_service(this);

   //}

   //bool application::stop_service()
   //{

   //   return System.os().stop_service(this);

   //}


   //void application::on_service_request(::create * pcreate)
   //{

   //   if(!is_serviceable())
   //      return;

   //   if(pcreate->m_spCommandLine->m_varQuery.has_property("create_service"))
   //   {
   //      create_service();
   //   }
   //   else if(pcreate->m_spCommandLine->m_varQuery.has_property("start_service"))
   //   {
   //      start_service();
   //   }
   //   else if(pcreate->m_spCommandLine->m_varQuery.has_property("stop_service"))
   //   {
   //      stop_service();
   //   }
   //   else if(pcreate->m_spCommandLine->m_varQuery.has_property("remove_service"))
   //   {
   //      remove_service();
   //   }


   //}







   bool application::process_init()
   {

      if (m_bAxisProcessInitialize)
      {

         return m_bAxisProcessInitializeResult;

      }

      thisstart;

      m_bAxisProcessInitialize = true;

      m_bAxisProcessInitializeResult = false;

      if (!::aura::application::process_init())
      {

         thisfail << 1;

         return false;

      }

      if (m_psystem->m_phtml == NULL)
      {

         m_psystem->m_phtml = create_html();

         if (m_psystem->m_phtml == NULL)
         {

            thisfail << 2;

            return false;

         }

         m_psystem->m_phtml->construct(m_papp);

      }

      m_bAxisProcessInitializeResult = true;

      thisend;

      return true;

   }


   bool application::init_instance()
   {

      if (m_bAxisInitializeInstance)
      {

         return m_bAxisInitializeInstanceResult;

      }

      thisstart;

      m_bAxisInitializeInstance = true;

      m_bAxisInitializeInstanceResult = false;

      if(!::aura::application::init_application())
      {

         thisfail << 1;

         return false;

      }

      thisok << 1;

      m_bAxisInitializeInstanceResult = true;

      thisend;

      return true;

   }


   ::database::key application::calc_data_key()
   {

      return ::database::key("app://" + m_strAppName, is_local_data());

   }


   bool application::init1()
   {

      if (m_bAxisInitialize1)
      {

         return m_bAxisInitialize1Result;

      }

      if (m_bInitializeDataCentral)
      {

         m_psimpledb->construct(this);

         if (!m_psimpledb->initialize())
            return false;

         if (!m_psimpledb->init2())
            return false;

         set_data_server(&Application.dataserver());

      }

      m_bAxisInitialize1 = true;

      m_bAxisInitialize1Result = false;

      m_dwAlive = ::get_tick_count();

      if (!::aura::application::init1())
      {

         return false;

      }

      if (!m_psystem->m_phtml->initialize())
      {

         return false;

      }



      m_bAxisInitialize1Result = true;

      return true;

   }


   bool application::init2()
   {

      if(!::aura::application::init2())
         return false;

      return true;

   }


   bool application::init3()
   {

      if(!::aura::application::init3())
      {

         return false;

      }

      return true;

   }


   bool application::init()
   {

      if(m_bAxisInitialize)
         return m_bAxisInitializeResult;

      m_bAxisInitialize = true;
      m_bAxisInitializeResult = false;

      ::aura::application_message signal(::aura::application_message_init);

      route_message(&signal);

      if(!signal.m_bOk)
         return false;

      m_dwAlive = ::get_tick_count();

      if(is_system())
      {

         if(handler()->m_varTopicQuery.propset().has_property("save_processing"))
         {

            Session.savings().save(::aura::resource_processing);

         }

         if(handler()->m_varTopicQuery.propset().has_property("save_blur_back"))
         {

            Session.savings().save(::aura::resource_blur_background);

         }

         if(handler()->m_varTopicQuery.propset().has_property("save_transparent_back"))
         {

            Session.savings().save(::aura::resource_translucent_background);

         }

      }

      if(handler()->m_varTopicQuery.propset().has_property("install"))
      {
         // core level app install
         if(!Ex2OnAppInstall())
            return false;
      }
      else if(handler()->m_varTopicQuery.propset().has_property("uninstall"))
      {
         // core level app uninstall
         if(!Ex2OnAppUninstall())
            return false;
      }
      else
      {
#ifdef WINDOWSEX
         // when this process is started in the context of a system account,
         // for example, this code ensure that the process will
         // impersonate a loggen on ::fontopus::user
         HANDLE hprocess;
         HANDLE htoken;

         hprocess = ::GetCurrentProcess();

         if(!OpenProcessToken(
               hprocess,
               TOKEN_ALL_ACCESS,
               &htoken))
            return false;

         if(!ImpersonateLoggedOnUser(htoken))
         {
            TRACELASTERROR();
            return false;
         }
#endif
      }

      m_dwAlive = ::get_tick_count();

      if(is_system()
            && !handler()->m_varTopicQuery["app"].get_string().begins_ci("app-core/netnode")
            && handler()->m_varTopicQuery["app"] != "app-core/netnode_dynamic_web_server"
            && handler()->m_varTopicQuery["app"] != "app-gtech/alarm"
            && handler()->m_varTopicQuery["app"] != "app-gtech/sensible_service")
      {
         System.http().defer_auto_initialize_proxy_configuration();
      }

      m_dwAlive = ::get_tick_count();

      m_bAxisInitializeResult = true;

//      dappy(string(typeid(*this).name()) + " : initialize ok : " + ::str::from(m_iErrorCode));

      return true;

   }


   void application::term_application()
   {


      try
      {


         //destroy_message_queue();

      }
      catch(...)
      {

         //m_iErrorCode = -1;

         m_error.set_if_not_set();


      }




      //try
      //{


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


      //m_pcommandthread.release();

      release_exclusive();

//         if(m_spuiMessage.is_set())
      {

         //if(!destroy_message_queue())
         {

            // TRACE("Could not finalize message interaction_impl");

         }

      }

      ::aura::application_message signal(::aura::application_message_term_instance);

      try
      {

         route_message(&signal);

      }
      catch(...)
      {

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


      try
      {

         try
         {

            aura::application::term_application();

         }
         catch(...)
         {

         }

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

      if (m_psimpledb.is_set())
      {

         try
         {

            m_psimpledb->finalize();

         }
         catch (...)
         {

            m_error.set_if_not_set();

         }

      }

      m_psimpledb.release();

//      return m_iErrorCode;

   }


   void application::term()
   {

      try
      {

         ::aura::application::term();

      }
      catch(...)
      {

         m_error.set_if_not_set();

      }

   }






   bool application::is_running()
   {
      return is_alive();
   }



   bool application::Ex2OnAppInstall()
   {
      return true;
   }


   bool application::Ex2OnAppUninstall()
   {

      return true;

   }


   bool application::update_appmatter(::sockets::socket_handler & handler, sp(::sockets::http_session) & psession,const ::file::path & pszRoot,const string & pszRelative)
   {

      ::str::international::locale_schema localeschema(this);

      Session.fill_locale_schema(localeschema);

      bool bIgnoreStdStd = string(pszRoot) == "app" && (string(pszRelative) == "main" || string(pszRelative) == "bergedge");

      //update_appmatter(h, psession, pszRoot, pszRelative, localeschema.m_idLocale, localeschema.m_idSchema);

      ::count iCount = localeschema.m_idaLocale.get_count();

      for(index i = 0; i < iCount; i++)
      {

         if(localeschema.m_idaLocale[i] == __id(std) && localeschema.m_idaSchema[i] == __id(std) && bIgnoreStdStd)
            continue;

         string strLocale;

         localeschema.m_idaLocale[i].to_string(strLocale);

         string strSchema;

         localeschema.m_idaSchema[i].to_string(strSchema);

         update_appmatter(handler, psession,pszRoot,pszRelative,strLocale,strSchema);

         System.install_progress_add_up();

      }


      return true;

   }

   bool application::update_appmatter(::sockets::socket_handler & handler, sp(::sockets::http_session) & psession,const ::file::path & pszRoot,const string & pszRelative,const string & pszLocale,const string & pszStyle)
   {

      string strLocale;
      string strSchema;
      TRACE("update_appmatter(root=%s, relative=%s, locale=%s, style=%s)",pszRoot,pszRelative,pszLocale,pszStyle);
      ::file::path strRelative = ::file::path(pszRoot) / "appmatter" / pszRelative  / Sess(this).get_locale_schema_dir(pszLocale,pszStyle) + ".zip";
      ::file::path strFile = System.dir().install() / strRelative;
      ::file::path strUrl(::file::path_url);
      if(_ca_is_basis())
      {
         strUrl = "http://basis-server.ca2.cc/api/spaignition/download?authnone&configuration=basis&stage=";
      }
      else
      {
         strUrl = "http://stage-server.ca2.cc/api/spaignition/download?authnone&configuration=stage&stage=";
      }

      strUrl += System.url().url_encode(strRelative);


      if(psession == NULL)
      {

         while(true)
         {

            property_set setEmpty(get_app());

            if (System.http().open(handler, psession, System.url().get_server(strUrl), System.url().get_protocol(strUrl), setEmpty, NULL, NULL))
            {

               break;

            }

            Sleep(200);

         }

      }

      property_set set;

      set["get_memory"] = "";

      if (!System.http().request(handler, psession, strUrl, set))
      {

         return false;

      }

      ::memory_file file(get_app(),set["get_memory"].cast < memory >());

      if(set["get_memory"].cast < memory >() != NULL && set["get_memory"].cast < memory >()->get_size() > 0)
      {

         zip::util util;

         string strDir = strFile;

         ::str::ends_eat_ci(strDir,".zip");

         try
         {

            util.extract_all(this, strDir,&file);

         }
         catch(...)
         {

            // spa app_app_admin.exe would recover by retrying or someone would fix the resource packaging problem and then zip extraction at least should work.

            return false;

         }

         //System.compress().extract_all(strFile, this);

      }

      return true;

   }





   bool application::assert_user_logged_in()
   {

      const char * pszRequestUrl = NULL;

      string strRequestUrl;

      if(file_as_string_dup(::dir::system() / "config\\system\\ignition_server.txt").has_char())
      {

         strRequestUrl = "https://" + file_as_string_dup(::dir::system() / "config\\system\\ignition_server.txt") + "/api/spaignition";

         pszRequestUrl = strRequestUrl;

      }

      if (m_psession == NULL)
      {

         return false;

      }

      if (m_psession->fontopus() == NULL)
      {

         return false;

      }

      if (m_psession->fontopus()->get_user(true, pszRequestUrl) == NULL)
      {
         //   _throw(exit_exception(get_app(),"You have not logged in!! db_str_set::load"));
         return false;

      }

      return true;

   }




   string application::matter_as_string(const char * pszMatter,const char * pszMatter2)
   {

      var varQuery;

      varQuery["disable_ca2_sessid"] = true;

      string strMatter = dir().matter(::file::path(pszMatter) / pszMatter2);

      return file().as_string(strMatter,varQuery);

   }

   //string application::dir().matter(const char * pszMatter,const char * pszMatter2)
   //{

   //   return dir().matter(pszMatter,pszMatter2);

   //}

   //bool application::is_inside_time_dir(const char * pszPath)
   //{
   //   return dir().is_inside_time(pszPath);
   //}


   bool application::file_is_read_only(const char * pszPath)
   {

      return false;
      //return file().is_read_only(pszPath);

   }


//   string application::file_as_string(var varFile)
//   {
//
//      //if(::str::begins_ci(varFile.get_string(),"http://")
//      //   || ::str::begins_ci(varFile.get_string(),"https://"))
//      //{
//
//      //   ::property_set set(get_app());
//
//      //   return http().get(varFile.get_string(),set);
//
//      //}
//      //else if(varFile.has_property("url") &&
//      //   (::str::begins_ci(varFile["url"].get_string(),"http://")
//      //   || ::str::begins_ci(varFile["url"].get_string(),"https://")))
//      //{
//
//      //   ::property_set set(get_app());
//
//      //   return http().get(varFile["url"].get_string(),set);
//
//      //}
//      //else
//      //{
//         return file().as_string(varFile.get_string());
////      }
//
//   }
//
//   //string application::dir_path(const char * psz1,const char * psz2,const char * psz3)
//   //{
//   //   return psz1,psz2,psz3);
//   //}
//
//   string application::dir_name(const char * psz)
//   {
//      return System.dir().name(psz);
//   }
//
//   bool application::dir_mk(const char * psz)
//   {
//      return dir().mk(psz);
//   }
//
//   string application::file_title(const char * psz)
//   {
//      return System.file().title_(psz);
//   }
//   string application::file_name(const char * psz)
//   {
//      return System.file().name_(psz);
//   }
//
//
//   ::file::file_sp application::file_get_file(var varFile,uint32_t uiFlags)
//   {
//
//      return file().get_file(varFile,uiFlags);
//
//   }

   //void application::dir_matter_ls_file(const string & str,stringa & stra)
   //{

   //   return dir().matter_ls_file(str,stra);

   //}

//#ifdef APPLEOS
//
//   string application::dir_pathfind(const char * pszEnv, const char * pszTopic, const char * pszMode)
//   {
//
//      return dir().pathfind(pszEnv, pszTopic, pszMode);
//
//   }
//
//#endif
//
//   string application::file_as_string(var varFile,var & varQuery)
//   {
//
//      return file().as_string(varFile);
//
//   }
//
//
//
//   bool application::file().exists(const char * pszPath)
//   {
//
//      return file().exists(pszPath) != FALSE;
//
//   }
//
//
//   bool application::file_is_equal_path(const char * pszPath1,const char * pszPath2)
//   {
//
//      return System.file().path().is_equal(pszPath1,pszPath2) != FALSE;
//
//   }
//
//
//   bool application::dir_is(const char * psz)
//   {
//
//      return dir().is(psz);
//
//   }


   //bool application::file_del(const char * psz)
   //{

   //   try
   //   {

   //      System.file().del(psz);

   //   }
   //   catch(...)
   //   {

   //      return false;

   //   }

   //   return true;

   //}

   //string application::file_extension(const char * pszPath)
   //{

   //   return file_extension_dup(pszPath);

   //}


   //string application::dir_userappdata(const char * lpcsz,const char * lpcsz2)
   //{

   //   return dir().userappdata(lpcsz,lpcsz2);

   //}

   //string application::dir_appdata(const char * lpcsz,const char * lpcsz2)
   //{

   //   return System.dir().appdata(lpcsz,lpcsz2);

   //}


   //string application::dir_simple_path(const string & str1,const string & str2)
   //{

   //   return System.dir().simple_path(str1,str2);

   //}


   //string application::dir_element(const char * psz)
   //{

   //   return System.dir().install(psz);

   //}


   //string application::dir_ca2module(const char * psz)
   //{

   //   return System.dir().ca2module(psz);

   //}


   //void application::dir_ls_dir(const char * lpcsz,::file::patha & patha)
   //{

   //   dir().ls_dir(lpcsz,ppatha,ppathaName);

   //}


   //void application::dir_rls(const char * lpcsz,::file::patha & patha)
   //{

   //   dir().rls(lpcsz,ppatha,ppathaName,ppathaRelative);

   //}




   //string application::file_time_square()
   //{

   //   return file().time_square();

   //}


   //string application::http_get_locale_schema(const char * pszUrl,const char * pszLocale,const char * pszSchema)
   //{

   //   return http().get_locale_schema(pszUrl,pszLocale,pszSchema);

   //}





} // namespace axis







#include "framework.h" // from "axis/user/user.h"

namespace axis
{


   //UINT application::APPM_LANGUAGE = WM_APP + 117;
   //WPARAM application::WPARAM_LANGUAGE_UPDATE = 1;






   void application::process_message_filter(int32_t code,::message::message * pobj)
   {

      //SCAST_PTR(::message::base,pbase,pobj);

      UNREFERENCED_PARAMETER(code);

      UNREFERENCED_PARAMETER(pobj);


   }







   void application::DoWaitCursor(int32_t nCode)
   {

      if(nCode < 0)
      {

         m_iWaitCursorCount = 0;
         ShowWaitCursor(false);

      }
      else if(nCode == 0)
      {

         if(m_iWaitCursorCount > 0)
         {
            m_iWaitCursorCount--;
         }

         if(m_iWaitCursorCount > 0)
         {

            ShowWaitCursor(true);

         }

         m_iWaitCursorCount = 0;
         ShowWaitCursor(false);

      }
      else
      {

         if(m_iWaitCursorCount < 0)
         {
            m_iWaitCursorCount = 0;
         }

         m_iWaitCursorCount++;

         ShowWaitCursor(true);

      }



   }

   //void application::ShowWaitCursor(bool bShow)
   //{

   //}



   void application::TermThread(HINSTANCE hInstTerm)
   {

      ::exception::throw_interface_only(this);

   }




   //string application::get_version()
   //{

   //   ::exception::throw_interface_only(this);

   //   return "";

   //}




   void application::set_env_var(const string & var,const string & value)
   {

      ::exception::throw_interface_only(this);

   }



   sp(::aura::printer) application::get_printer(const char * pszDeviceName)
   {

      ::exception::throw_interface_only(this);

      return NULL;

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










   //IDTHREAD application::get_thread_id()
   //{
   //   return  == NULL->get_thread_id();
   //}





//#ifndef METROWIN
//
//   void application::get_time(timeval *p)
//   {
//      m_pimpl->get_time(p);
//   }
//
//#endif


   bool application::do_prompt_file_name(var & varFile,UINT nIDSTitle,uint32_t lFlags,bool bOpenFileDialog,::user::impact_system * ptemplate,::user::document * pdocument)
   {

      UNREFERENCED_PARAMETER(varFile);
      UNREFERENCED_PARAMETER(nIDSTitle);

      return false;

   }






   //bool application::get_temp_file_name_template(string & strRet,const char * pszName,const char * pszExtension,const char * pszTemplate)
   //{

   //   _throw(not_implemented(this));

   //   return false;

   //}


   //bool application::get_temp_file_name(string & strRet,const char * pszName,const char * pszExtension)
   //{

   //   return get_temp_file_name_template(strRet,pszName,pszExtension,NULL);

   //}








   ::visual::icon * application::set_icon(object * pobject,::visual::icon * picon,bool bBigIcon)
   {

      return NULL;

   }


   ::visual::icon * application::get_icon(object * pobject,bool bBigIcon) const
   {

      return NULL;

   }


   void application::on_service_request(::create * pcreate)
   {

      if(!is_serviceable())
      {
         
         return;
         
      }

      if(pcreate->m_spCommandLine->m_varQuery.has_property("create_service"))
      {
         
         create_service();
         
      }
      else if(pcreate->m_spCommandLine->m_varQuery.has_property("start_service"))
      {
         
         start_service();
         
      }
      else if(pcreate->m_spCommandLine->m_varQuery.has_property("stop_service"))
      {
         
         stop_service();
         
      }
      else if(pcreate->m_spCommandLine->m_varQuery.has_property("remove_service"))
      {
         
         remove_service();
         
      }

   }


   string application::get_mutex_name_gen()
   {
      return m_strAppName;
   }



   //bool application::Ex2OnAppInstall()
   //{
   //   return true;
   //}

   //bool application::Ex2OnAppUninstall()
   //{
   //   return true;
   //}







   int32_t application::simple_message_box_timeout(::user::primitive * pwndOwner,const char * pszMessage,::duration durationTimeOut,UINT fuStyle)
   {
      UNREFERENCED_PARAMETER(durationTimeOut);
      return simple_message_box(pwndOwner,pszMessage,fuStyle);
   }


   //   void application::tellme_destroyed(::user::interaction * pui, bool * pDestroyed, bool bTell)
   //   {
   //
   //      if(pui == NULL)
   //         return;
   //
   //      try
   //      {
   //
   //#ifdef LINUX
   //
   //         synch_lock sl(get_ui_destroyed_mutex());
   //
   //         if(bTell)
   //         {
   //
   //            if(pui->m_bDestroying)
   //            {
   //
   //               *pDestroyed = true;
   //
   //            }
   //            else
   //            {
   //
   //               pui->m_bptraTellMeDestroyed.add(pDestroyed);
   //
   //            }
   //
   //         }
   //         else
   //         {
   //
   //            pui->m_bptraTellMeDestroyed.remove(pDestroyed);
   //
   //         }
   //
   //#endif
   //
   //      }
   //      catch(...)
   //      {
   //
   //      }
   //
   //   }


   string application::get_license_id()
   {

      if(m_strLicense.has_char())
         return m_strLicense;

      if(m_strInstallToken.has_char())
         return m_strInstallToken;

      return m_strAppId;

   }


   void application::on_create_keyboard()
   {

      Session.set_keyboard_layout(NULL,::action::source::database());

   }




   //bool application::assert_user_logged_in()
   //{

   //   return axis::application::assert_user_logged_in();

   //}








   ::user::primitive * application::window_from_os_data(void * pdata)
   {

      return NULL;

   }


   //sp(::message::base) application::get_message_base(LPMESSAGE lpmsg)
   //{

   //   return get_message_base(lpmsg->message,lpmsg->wParam,lpmsg->lParam);

   //}



   int32_t application::simple_message_box(::user::primitive * puiOwner,const char * pszMessage,UINT fuStyle)
   {

      return ::simple_message_box(puiOwner->get_safe_handle(),pszMessage,m_strAppName,fuStyle);

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


   bool application::set_keyboard_layout(const char * pszPath,::action::context actioncontext)
   {

      if(get_app()->m_psession == NULL)
      {
         
         return false;
         
      }

      if(pszPath == NULL)
      {

         if(is_set_ref(Session.keyboard().on_layout()))
         {

            //            if(Session.fontopus()->m_puser != NULL
            //             && Session.fontopus()->m_puser->m_strFontopusServerSessId.has_char())
            //        {

            // xxx data_set("keyboard_layout", keyboard().on_layout().m_strPath);

            //      }

            return true;
         }

         string strCurrentSystemLayout = Session.keyboard().get_current_system_layout();

         if(strCurrentSystemLayout.is_empty())
            return false;

         if(!set_keyboard_layout(strCurrentSystemLayout,::action::source_database))
            return false;

         //         if(Session.fontopus()->m_puser != NULL
         //          && Session.fontopus()->m_puser->m_strFontopusServerSessId.has_char())
         //     {

         // xxx            data_set("keyboard_layout", keyboard().on_layout().m_strPath);

         //   }

         return true;
      }

      if(!Session.keyboard().load_layout(pszPath,actioncontext))
         return false;

      // xxx Application.simpledb().on_set_keyboard_layout(pszPath, actioncontext);

      return true;
   }


   int32_t application::hotplugin_host_starter_start_sync(const char * pszCommandLine,::aura::application * papp,hotplugin::host * phost,hotplugin::plugin * pplugin)
   {

      {

         ::install::mutex mutex(process_platform_dir_name2());

         if (mutex.already_exists())
         {

//            simple_message_box("Could not launch spa installer. It is already running.", MB_OK);

            return -35;

         }

      }

      string strValue;

      if(get_command_line_param(strValue,pszCommandLine,"enable_desktop_launch"))
      {

#ifdef METROWIN

         return -1;

#else

         return ::call_sync(::path::app_app(process_platform_dir_name2(), process_configuration_dir_name()),pszCommandLine,::path::app_app(process_platform_dir_name2(), process_configuration_dir_name()),SW_SHOWNORMAL,840,84,NULL,0);

#endif

      }
      else
      {

         return hotplugin_host_host_starter_start_sync(pszCommandLine,get_app(),NULL);

      }

   }

   int32_t application::hotplugin_host_host_starter_start_sync(const char * pszCommandLine, ::aura::application * papp, hotplugin::host * phost, hotplugin::plugin * pplugin)
   {

      return -1;

   }



   void application::on_control_event(::user::control_event * pevent)
   {

   }


   bool application::on_open_document(::user::document * pdocument,var varFile)
   {

      UNREFERENCED_PARAMETER(pdocument);
      UNREFERENCED_PARAMETER(varFile);
      return true;

   }


   bool application::on_save_document(::user::document * pdocument,var varFile)
   {

      UNREFERENCED_PARAMETER(pdocument);
      UNREFERENCED_PARAMETER(varFile);
      return true;

   }


   void application::on_update_view(::user::impact * pview,::user::impact * pviewSender,LPARAM lHint,object* pHint)
   {


   }




   string application::fontopus_get_cred(::aura::application * papp,const string & strRequestUrl, const RECT & rect, string & strUsername, string & strPassword, string strToken, string strTitle, bool bInteractive, ::user::interactive * pinteractive)
   {

      return ::fontopus::get_cred(papp, strRequestUrl, rect, strUsername, strPassword, strToken, strTitle, bInteractive, pinteractive);

   }

   string application::fontopus_get_cred(string & strUsername, string & strPassword, string strToken)
   {

      return ::fontopus::get_cred(this, strUsername, strPassword, strToken);

   }

   void application::set_cred(string strToken, const char * pszUsername, const char * pszPassword)
   {

      ::fontopus::set_cred(this,strToken, pszUsername, pszPassword);

   }

   void application::set_cred_ok(string strToken, bool bOk)
   {

      ::fontopus::set_cred_ok(this, strToken, bOk);

   }

   ::html::html * application::create_html()
   {

      return new ::html::html(m_papp);

   }


   string application::http_get(const string & strUrl, ::property_set & set)
   {

      return http().get(strUrl, set);

   }

   bool application::app_data_set(::id id, serialize & serialize)
   {

      return data_set(id, serialize);

   }


   bool application::app_data_get(::id id, serialize & serialize)
   {

      return data_get(id, serialize);

   }


   bool application::app_data_set(::id id, ::object & obj)
   {

      return data_set(id, obj);

   }


   bool application::app_data_get(::id id, ::object & obj)
   {

      return data_get(id, obj);

   }


   bool application::compress_ungz(stream & ostreamUncompressed, const ::file::path & lpcszGzFileCompressed)
   {

      return System.compress().ungz(this, ostreamUncompressed, lpcszGzFileCompressed);

   }


   bool application::compress_ungz(::primitive::memory_base & mem)
   {

      return System.compress().ungz(this, mem);

   }


   bool application::compress_gz(::file::file * pfileOut, const ::file::path & lpcszUncompressed, int iLevel)
   {

      return System.compress().gz(this, pfileOut, lpcszUncompressed, iLevel);

   }


   bool application::compress_gz(::file::file * pfileOut, ::file::file * pfileIn, int iLevel)
   {

      return System.compress().gz(this, pfileOut, pfileIn, iLevel);

   }


   bool application::is_local_data()
   {

      return m_datakey.m_bLocalData;

   }


} // namespace axis







