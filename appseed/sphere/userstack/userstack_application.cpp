//#include "framework.h"


namespace userstack
{


   application::application()
   {


      m_bDrawCursor              = true;
      m_bShowPlatform            = false;
      m_pappCurrent              = NULL;
      m_bLicense				      = false;

      m_ppaneview = NULL;

      m_strAppName               = "userstack";

   }


   application::~application()
   {

   }


   void application::construct()
   {

      m_strAppName         = "userstack";
      m_strBaseSupportId   = "ca2_userstack";
      m_strInstallToken    = "userstack";
      m_bLicense           = false;
      m_eexclusiveinstance = ::ExclusiveInstanceNone;

   }

   bool application::initialize_application()
   {

      if(!::asphere::application::initialize_application())
         return false;


      m_dataid += ".local://";

      initialize_bergedge_application_interface();


      Session.filemanager().std().m_strLevelUp = "levelup";


      SetRegistryKey("ca2core");


      return true;


   }

   int32_t application::exit_application()
   {
      try
      {
         ::asphere::application::exit_application();
      }
      catch(...)
      {
      }

      string strId;
      sp(::aura::application) pcaapp;

      POSITION pos = m_mapApplication.get_start_position();

      while(pos != NULL)
      {

         strId.Empty();
         pcaapp = NULL;

         m_mapApplication.get_next_assoc(pos, strId, pcaapp);

         sp(::aura::application) papp =  (pcaapp);

         papp->m_pcoreapp->post_quit();
      }

      return 0;

   }

   bool application::bergedge_start()
   {
      return true;
   }




   void application::OnFileManagerOpenFile(::filemanager::data * pdata, ::fs::item_array & itema)
   {
      UNREFERENCED_PARAMETER(pdata);
   }

   void application::load_string_table()
   {
      ::asphere::application::load_string_table();
      ::asphere::application::load_string_table("platform", "");
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

      int32_t iCount = 32; // todo: get from bergedge profile

      System.factory().creatable < ::userstack::document > (iCount);
      System.factory().creatable < ::userstack::view > (iCount);
      System.factory().creatable < ::userstack::pane_view > (iCount);
      System.factory().creatable < ::userstack::frame > (iCount);

      m_ptemplate_pane    = new ::user::single_document_template(
         this,
         "bergedge/frame",
         System.type_info < document > (),
         System.type_info < frame > (),
         System.type_info < pane_view > ());

   }


   void application::launch_app(const char * psz)
   {
      UNREFERENCED_PARAMETER(psz);
   }

   void application::install_app(const char * psz)
   {
      UNREFERENCED_PARAMETER(psz);
   }

   void application::on_request(sp(::create) pcreatecontext)
   {

      ::asphere::application::on_request(pcreatecontext);

      m_ptemplate_pane->open_document_file(pcreatecontext);

   }


   bool application::on_exclusive_instance_conflict(::EExclusiveInstance eexclusive)
   {

      if(eexclusive == ::ExclusiveInstanceLocalId)
      {
#ifdef WINDOWSEX
         ::memory_file file(get_app());
         file.from_string(command()->m_varTopicFile);
         COPYDATASTRUCT data;
         data.dwData = 1984;
         data.cbData = (uint32_t) file.get_length();
         data.lpData = file.get_data();
         ::oswindow oswindow = ::FindWindowA(NULL, "::ca2::fontopus::message_wnd::application::");

         ::SendMessage(oswindow, WM_COPYDATA, (WPARAM) 0, (LPARAM) &data);

         return true;
#else
         throw todo(get_app());
#endif
      }

      return false;

   }


/*   void application::request(sp(::create) pcreatecontext)
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
         if(pcreatecontext->m_spCommandLine->m_varQuery["document"].cast < ::user::document > () == NULL)
         {
            goto alt1;
         }

      }
      else
      {
         alt1:
         if(pcreatecontext->m_spCommandLine->m_varFile.get_type() == var::type_string)
         {
            if(::str::ends_ci(pcreatecontext->m_spCommandLine->m_varFile, ".ca2"))
            {
               string strCommand = Application.file().as_string(pcreatecontext->m_spCommandLine->m_varFile);
               if(::str::begins_eat(strCommand, "ca2prompt\r")
               || ::str::begins_eat(strCommand, "ca2prompt\n"))
               {
                  strCommand.trim();
                  command()->add_fork_uri(strCommand);
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
            //simple_message_box(NULL, "request3", "request3", MB_ICONEXCLAMATION);
            get_document()->get_typed_view < pane_view >()->set_cur_tab_by_id("app:" + pcreatecontext->m_spCommandLine->m_strApp);
            App(m_pappCurrent).request(pcreatecontext);
         }
         else
         {
            //simple_message_box(NULL, "request4", "request4", MB_ICONEXCLAMATION);
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


   sp(::aura::application) application::application_get(const char * pszId, bool bCreate, bool bSynch, application_bias * pbiasCreate)
   {
      sp(::aura::application) papp = NULL;

      if(m_mapApplication.Lookup(string(pszId), papp))
         return papp;
      else
      {
         if(!bCreate)
            return NULL;
         papp = NULL;
         try
         {
            papp = create_application(pszId, bSynch, pbiasCreate);
         }
         catch(...)
         {
            papp = NULL;
         }
         if(papp == NULL)
            return NULL;
         if(&App(papp) == NULL)
         {
            return NULL;
         }
         m_mapApplication.set_at(string(pszId), papp);
         Session.m_mapApplication.set_at(string(pszId), papp);
         return papp;
      }
   }

   sp(::aura::application) application::get_current_application()
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


   bool application::on_install()
   {

      string strFormat;

      string strSentinelPath;

      strSentinelPath = System.dir().element() / "stage/x86/app.sentinel.exe";

      System.os().local_machine_set_run("ca2 app.sentinel", "\"" + strSentinelPath + "\"", true);

      return ::asphere::application::on_install();
   }


   void application::set_app_title(const char * pszType, const char * pszAppId, const char * pszTitle)
   {

      sp(::aura::application) papp = NULL;

      if(m_mapApplication.Lookup(string(pszType) + ":" + string(pszAppId), papp) && papp != NULL)
      {

         sp(pane_view) ppaneview = m_ppaneview;

         if(ppaneview != NULL)
         {

            string strAppName(pszAppId);

            ::user::tab_pane * ppane = ppaneview->get_pane_by_id("app:" + strAppName);

            if(ppane != NULL)
            {

               ppane->m_istrTitleEx = pszTitle;

               ppaneview->on_layout();

            }

         }

      }

   }


   bool application::initialize1()
   {


      if(!::asphere::application::initialize1())
         return false;


      return true;


   }

   bool application::initialize()
   {


      if(!::asphere::application::initialize())
         return false;


      return true;


   }

   bool application::os_native_bergedge_start()
   {

      return ::asphere::application::os_native_bergedge_start();

   }

   int32_t application::main()
   {


      return ::asphere::application::main();


   }

   bool application::on_uninstall()
   {

      bool bOk1 = false;


      try
      {
         bOk1 = ::asphere::application::on_uninstall();
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



