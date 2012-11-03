#include "framework.h"


namespace userstack
{


   application::application()
   {


      m_bDrawCursor              = true;
      m_bShowPlatform            = false;
      m_pappCurrent              = NULL;
      m_bLicense				      = false;

      m_strAppName               = "userstack";


      ::fs::set * pset = new class ::fs::set(this);
      pset->m_spafsdata.add(new ::fs::native(this));
      m_spfsdata(pset);


   }

   application::~application()
   {


      string strId;
      ::ca::application * pcaapp;

      POSITION pos = m_mapApplication.get_start_position();

      while(pos != NULL)
      {

         strId.Empty();
         pcaapp = NULL;

         m_mapApplication.get_next_assoc(pos, strId, pcaapp);

         ::ca2::application * papp = dynamic_cast < ::ca2::application * > (pcaapp);

         papp->post_thread_message(WM_QUIT, 0, 0);
      }

   }

   void application::construct()
   {

      m_strAppName         = "userstack";
      m_strBaseSupportId   = "votagus_ca2_userstack";
      m_strInstallToken    = "userstack";
      m_bLicense           = false;
      m_eexclusiveinstance = ::radix::ExclusiveInstanceNone;

   }

   bool application::initialize_instance()
   {

      if(!cube2::application::initialize_instance())
         return false;

      initialize_bergedge_application_interface();


      GetStdFileManagerTemplate()->m_strLevelUp = "levelup";


      SetRegistryKey("ca2core");


      return true;


   }

   int application::exit_instance()
   {
      try
      {
         ::cubebase::application::exit_instance();
      }
      catch(...)
      {
      }
      return 0;
   }

   bool application::bergedge_start()
   {
      return true;
   }



   void application::_001OnFileNew()
   {
      userbase::application::m_pdocmanager->_001OnFileNew();
   }


   bool application::_001OnCmdMsg(BaseCmdMsg * pcmdmsg)

   {
      return gen::application::_001OnCmdMsg(pcmdmsg);
   }

   ::ca::application * application::get_app() const
   {
      return ::cubebase::application::get_app();
   }

   void application::OnFileManagerOpenFile(
         ::filemanager::data * pdata,
         ::fs::item_array & itema)
   {
      UNREFERENCED_PARAMETER(pdata);
   }

   void application::load_string_table()
   {
      cube1::application::load_string_table();
      cube1::application::load_string_table("platform", "");
   }

   bool application::file_manager_open_file(
            ::filemanager::data * pdata,
            ::fs::item_array & itema)
   {
      UNREFERENCED_PARAMETER(pdata);
      if(itema.get_size() > 0)
      {
         return true;
      }
      return false;
   }


   void application::initialize_bergedge_application_interface()
   {

      int iCount = 32; // todo: get from bergedge profile
      
      System.factory().creatable < ::userstack::document > (iCount);
      System.factory().creatable < ::userstack::view > (iCount);
      System.factory().creatable < ::userstack::pane_view > (iCount);
      System.factory().creatable < ::userstack::frame > (iCount);

      m_ptemplate_pane    = new ::userbase::single_document_template(
         this,
         "bergedge/frame",
         System.template type_info < document > (),
         System.template type_info < frame > (),
         System.template type_info < pane_view > ());

   }


   void application::launch_app(const char * psz)
   {
      UNREFERENCED_PARAMETER(psz);
   }

   void application::install_app(const char * psz)
   {
      UNREFERENCED_PARAMETER(psz);
   }

   void application::on_request(::ca::create_context * pcreatecontext)
   {


      m_ptemplate_pane->open_document_file(pcreatecontext);

   }


   void application::on_exclusive_instance_conflict(::radix::EExclusiveInstance eexclusive)
   {
      if(eexclusive == ::radix::ExclusiveInstanceLocalId)
      {
         gen::memory_file file(get_app());
         file.from_string(command().m_varTopicFile);
         COPYDATASTRUCT data;
         data.dwData = 1984;
         data.cbData = (DWORD) file.get_length();
         data.lpData = file.get_data();
         ::oswindow oswindow = ::FindWindowA(NULL, "ca2::fontopus::message_wnd::application::");

         ::SendMessage(oswindow, WM_COPYDATA, NULL, (LPARAM) &data);
      }
   }


/*   void application::request(::ca::create_context * pcreatecontext)
   {
      
      if(m_pappCurrent != NULL && m_pappCurrent != this 
         && (pcreatecontext->m_spCommandLine->m_strApp.is_empty() 
         ||App(m_pappCurrent).m_strAppName == pcreatecontext->m_spCommandLine->m_strApp))
      {
         if(get_document() != NULL && get_document()->get_typed_view < pane_view >() != NULL)
         {
            get_document()->get_typed_view < pane_view >()->set_cur_tab_by_id("app:" + App(m_pappCurrent).m_strAppName);
         }
         App(m_pappCurrent).request(pcreatecontext);
         if(pcreatecontext->m_spCommandLine->m_varQuery["document"].ca2 < ::user::document_interface > () == NULL)
         {
            goto alt1;
         }

      }
      else
      {
         alt1:
         if(pcreatecontext->m_spCommandLine->m_varFile.get_type() == var::type_string)
         {
            if(gen::str::ends_ci(pcreatecontext->m_spCommandLine->m_varFile, ".ca2"))
            {
               string strCommand = Application.file().as_string(pcreatecontext->m_spCommandLine->m_varFile);
               if(gen::str::begins_eat(strCommand, "ca2prompt\r")
               || gen::str::begins_eat(strCommand, "ca2prompt\n"))
               {
                  strCommand.trim();
                  command().add_fork_uri(strCommand);
               }
               return;
            }
            else
            {
               on_request(pcreatecontext);
            }
         }
         else if(pcreatecontext->m_spCommandLine->m_strApp.has_char() &&
            get_document() != NULL && get_document()->get_typed_view < pane_view >() != NULL
            && (!pcreatecontext->m_spApplicationBias.is_set() || pcreatecontext->m_spApplicationBias->m_puiParent == NULL))
         {
            //MessageBox(NULL, "request3", "request3", MB_ICONEXCLAMATION);
            get_document()->get_typed_view < pane_view >()->set_cur_tab_by_id("app:" + pcreatecontext->m_spCommandLine->m_strApp);
            App(m_pappCurrent).request(pcreatecontext);
         }
         else
         {
            //MessageBox(NULL, "request4", "request4", MB_ICONEXCLAMATION);
            on_request(pcreatecontext);
         }
      }
   }*/

   void application::request_topic_file(var & varQuery)
   {
   }

   void application::request_topic_file()
   {
   }


   ::ca::application * application::application_get(const char * pszType, const char * pszId, bool bCreate, bool bSynch, ::ca::application_bias * pbiasCreate)
   {
      ::ca::application * papp = NULL;

      if(m_mapApplication.Lookup(string(pszType) + ":" + string(pszId), papp))
         return papp;
      else
      {
         if(!bCreate)
            return NULL;
         papp = NULL;
         try
         {
            papp = create_application(pszType, pszId, bSynch, pbiasCreate);
         }
         catch(...)
         {
            papp = NULL;
         }
         if(papp == NULL)
            return NULL;
         m_mapApplication.set_at(string(pszType) + ":" + string(pszId), papp);
         Session.m_mapApplication.set_at(string(pszType) + ":" + string(pszId), papp);
         return papp;
      }
   }

   ::ca::application * application::get_current_application()
   {
      return m_pappCurrent;
   }


   void application::check_topic_file_change()
   {
      if(m_varCurrentViewFile != m_varTopicFile && !m_varTopicFile.is_empty())
      {
         m_varCurrentViewFile = m_varTopicFile;
         request_topic_file();
      }
   }

/*   ::user::interaction * application::get_request_parent_ui(::userbase::main_frame * pmainframe, ::ca::create_context * pcreatecontext)
   {

      return get_request_parent_ui((::user::interaction * ) pmainframe, pcreatecontext);

   }

   ::user::interaction * application::get_request_parent_ui(::user::interaction * pinteraction, ::ca::create_context * pcreatecontext)
   {


      ::user::interaction * puiParent = NULL;

      if(pcreatecontext->m_spCommandLine->m_varQuery["uicontainer"].ca2 < ::user::interaction >() != NULL)
         puiParent = pcreatecontext->m_spCommandLine->m_varQuery["uicontainer"].ca2 < ::user::interaction >();

      if(puiParent == NULL && pcreatecontext->m_puiParent != NULL)
      {
         puiParent = pcreatecontext->m_puiParent;
      }

      if(puiParent == NULL && pcreatecontext->m_spCommandLine->m_pbiasCreate != NULL)
      {
         puiParent = pcreatecontext->m_spCommandLine->m_pbiasCreate->m_puiParent;
      }

      if(puiParent == NULL && pcreatecontext->m_spApplicationBias.is_set())
      {
         puiParent = pcreatecontext->m_spApplicationBias->m_puiParent;
      }


/*      if(pui == NULL && m_puiInitialPlaceHolderContainer != NULL)
      {
         pui = m_puiInitialPlaceHolderContainer;
      }*/

/*      if(pui == NULL && m_bShowPlatform && m_pbergedge->get_document() != NULL)
      {
         pui = Bergedge.get_document()->get_bergedge_view();
      }

      return pui;

   }*/

  /*    if(pinteraction->get_app()->is_bergedge() || pcreatecontext->m_bClientOnly ||
         Application.directrix().m_varTopicQuery["client_only"] != 0 ||
         pcreatecontext->m_bOuterPopupAlertLike)
      {
         return puiParent;
      }

      if(!create_bergedge(pcreatecontext))
      {
         return NULL;
      }




      cube8::application & app = App(pinteraction->get_app());

      string strAppName = app.m_strAppName;

      if(strAppName != "bergedge")
      {

         if(get_document() != NULL)
         {

            if(get_document()->get_typed_view < ::application::pane_view >() != NULL)
            {

               get_document()->get_typed_view < ::application::pane_view >()->set_cur_tab_by_id("app:" + strAppName);

               puiParent = get_document()->get_typed_view < ::application::pane_view >()->get_tab_holder(get_document()->get_typed_view < ::application::pane_view >()->get_tab_by_id("app:" + strAppName));

            }
            else
            {

               puiParent = get_document()->get_typed_view < ::application::view >();

            }

         }

      }

      return puiParent;

   }

   ::user::place_holder_ptra application::get_place_holder(::userbase::main_frame * pmainframe, ::ca::create_context * pcreatecontext)
   {

      UNREFERENCED_PARAMETER(pcreatecontext);

      ::user::place_holder_ptra holderptra;


      cube8::application & app = App(pmainframe->get_app());

      string strAppName = app.m_strAppName;

      if(get_document()->get_typed_view < ::application::pane_view >() != NULL)
      {

         get_document()->get_typed_view < ::application::pane_view >()->set_cur_tab_by_id("app:" + strAppName);

         holderptra.add(get_document()->get_typed_view < ::application::pane_view >()->get_tab_holder(get_document()->get_typed_view < ::application::pane_view >()->get_tab_by_id("app:" + strAppName)));

      }
      else
      {

         holderptra.add(get_document()->get_typed_view < ::application::view >());

      }

      return holderptra;

   }

   bool application::place(::userbase::main_frame * pmainframe, ::ca::create_context * pcreatecontext)
   {

      get_place_holder(pmainframe, pcreatecontext).hold(pmainframe);

      return true;

   }

   */

   void application::get_screen_rect(LPRECT lprect)
   {
      if(get_document() != NULL && get_view() != NULL)
      {
         get_view()->GetWindowRect(lprect);
      }
      else 
      {
         System.get_screen_rect(lprect);
      }
   }

   bool application::on_install()
   {

      string strFormat;

      string strSentinelPath;

      strSentinelPath = System.dir().ca2("stage/x86/app-sentinel.exe");

      System.os().local_machine_set_run("ca2 app-sentinel", "\"" + strSentinelPath + "\"");

      return ::cubebase::application::on_install();
   }


   void application::set_app_title(const char * pszType, const char * pszAppId, const char * pszTitle)
   {

      ::ca::application * papp = NULL;

      if(m_mapApplication.Lookup(string(pszType) + ":" + string(pszAppId), papp) && papp != NULL)
      {

         pane_view * ppaneview = get_document()->get_typed_view < pane_view >();

         if(ppaneview != NULL)
         {
         
            string strAppName(pszAppId);

            ::user::tab::pane * ppane = ppaneview->get_pane_by_id("app:" + strAppName);

            if(ppane != NULL)
            {

               ppane->m_istrTitleEx = pszTitle;

               ppaneview->layout();

            }

         }

      }

   }


   bool application::initialize1()
   {

      
      if(!::cubebase::application::initialize1())
         return false;


      return true;


   }

   bool application::initialize()
   {

      
      if(!::cubebase::application::initialize())
         return false;


      return true;


   }

   bool application::os_native_bergedge_start()
   {

      return ::cubebase::application::os_native_bergedge_start();

   }

   int application::main()
   {
      
      
      return ::cubebase::application::main();


   }

   bool application::on_uninstall()
   {

      bool bOk1 = false;


      try
      {
         bOk1 = ::cubebase::application::on_uninstall();
      }
      catch(...)
      {
         bOk1 = false;
      }


      return bOk1;


   }

   bool application::is_serviceable()
   {


      return false;


   }

   service_base * application::allocate_new_service()
   {


      return NULL;


   }


} // namespace userstack



