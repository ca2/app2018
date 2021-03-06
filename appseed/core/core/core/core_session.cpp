#include "framework.h"
//#include "core/user/user/user.h"
//#include "core/filesystem/filemanager/filemanager.h"


#define MAGIC_PALACE_TAB_SPLT "->:<-"
#define MAGIC_PALACE_TAB_SIZE "-/-"
#define MAGIC_PALACE_TAB_TEXT "/"



namespace core
{

   session_docs * create_session_docs();

   void destroy_session_docs(session_docs * pdocs);


   session::session(::aura::application * papp):
      object(papp),
      ::aura::session(papp),
      ::axis::session(papp),
      ::base::session(papp),
      ::thread(papp)
   {

      m_strAppId                    = "core_session";
      m_strAppName                  = "core_session";
      m_strBaseSupportId            = "core_session";
      m_strInstallToken             = "core_session";

      m_puiLastUserInputPopup = NULL;

      m_pdocs = create_session_docs();

      m_bLicense				               = false;

      m_puserex                           = NULL;

      m_bLicense           = false;
      m_eexclusiveinstance = ExclusiveInstanceNone;

   }

   session::~platform_parent
   {

      destroy_session_docs(m_pdocs);


   }


//   void session::construct(const char * pszAppId)
//   {
//
//      ::core::application::construct("session");
//
//      m_strAppName         = "session";
//      m_strBaseSupportId   = "ca2_bergedge";
//      m_strInstallToken    = "session";
//      m_bLicense           = false;
//      m_eexclusiveinstance = ExclusiveInstanceNone;
//
//   }


   void session::install_message_routing(::message::sender * psender)
   {

      core::application::install_message_routing(psender);
      base::session::install_message_routing(psender);

   }


   bool session::process_init()
   {

      if(!::core::application::process_init())
         return false;

      if(!::base::session::process_init())
         return false;

      if (!process_initialize_userex())
      {

         return false;

      }

      //m_puserex->construct(this);

      return true;

   }


   bool session::init1()
   {

      if(!::core::application::init1())
         return false;

      if(!::base::session::init1())
         return false;

      if (!initialize1_userex())
      {

         return false;

      }


      return true;


   }


   bool session::init()
   {

      if (!::core::application::init())
      {

         return false;

      }

      if (!::base::session::init())
      {

         return false;

      }

      return true;

   }


   bool session::init2()
   {

      if (!::core::application::init2())
      {

         return false;

      }

      if (!::base::session::init2())
      {

         return false;

      }

      return true;

   }



   bool session::init_application()
   {

      if (!::core::application::init_application())
      {

         return false;

      }

      if (!::base::session::init_application())
      {

         return false;

      }

      initialize_bergedge_application_interface();

      SetRegistryKey("ca2core");

      if(!InitializeLocalDataCentral())
      {

         simple_message_box(NULL, "Could not initialize Local data central");

         return false;

      }

      initialize_bergedge_application_interface();

      if(is_remote_session())
      {

         //Session.savings().save(::aura::resource_display_bandwidth);
         //Session.savings().save(::aura::resource_blur_background);
         //Session.savings().save(::aura::resource_blurred_text_embossing);
         //Session.savings().save(::aura::resource_translucent_background);

      }

      return true;

   }


   void session::term()
   {

      m_pfilemanager.release();

      try
      {

         ::base::session::term();

      }
      catch(...)
      {

         m_error.set_if_not_set();

      }

      try
      {

         ::core::application::term();

      }
      catch(...)
      {

         m_error.set_if_not_set();

      }

   }


   bool session::bergedge_start()
   {

      return true;

   }


   void session::_001OnFileNew()
   {

   }


   void session::route_command_message(::user::command * pcommand)
   {

      return application::route_command_message(pcommand);

   }


   void session::load_string_table()
   {

      application::load_string_table();
      application::load_string_table("plane", "");
      application::load_string_table();
      application::load_string_table("session", "");

   }


   void session::launch_app(const char * psz)
   {

      UNREFERENCED_PARAMETER(psz);

   }


   void session::install_app(const char * psz)
   {

      UNREFERENCED_PARAMETER(psz);

   }


   void session::on_request(::create * pcreate)
   {

      ::base::session::on_request(pcreate);

   }


   bool session::open_by_file_extension(const char * pszPathName, application_bias * pbiasCreate)
   {

      sp(::create) cc(allocer());

      cc->m_spCommandLine->m_varFile = pszPathName;

      if(pbiasCreate != NULL)
      {

         cc->m_spApplicationBias->operator=(*pbiasCreate);

      }

      return open_by_file_extension(cc);

   }


   bool session::open_by_file_extension(::create * pcreate)
   {

      string strId;

      string strOriginalPathName(pcreate->m_spCommandLine->m_varFile);

      ::file::path strPathName(strOriginalPathName);

      strsize iFind = strPathName.find("?");

      if(iFind >= 0)
      {

         strPathName = strPathName.Left(iFind);

      }

      if(::str::ends_ci(strPathName, ".ca2"))
      {

      }

      string strProtocol = System.url().get_protocol(strPathName);

      if(strProtocol == "app")
      {

         strId = System.url().get_server(strPathName);

         string str = System.url().get_object(strPathName);

         ::str::begins_eat(str, "/");

         pcreate->m_spCommandLine->m_varFile = str;

      }
      else
      {

         string strExtension = strPathName.extension();

         stringa straApp;

         System.filehandler().get_extension_app(straApp, strExtension);

         if(straApp.get_count() == 1)
         {

            strId = straApp[0];

         }
         else
         {

            strId = "app/sphere/default_file_handler";

         }

      }

      ::aura::application * papp = application_get(strId, true, true, pcreate->m_spApplicationBias);

      if (papp == NULL)
      {

         return false;

      }

      papp->::object::handle(pcreate);

      return true;

   }


   session::run_application::run_application()
   {

      m_papp = NULL;

      m_puiParent = NULL;

   }

   //   void session::on_exclusive_instance_conflict(EExclusiveInstance eexclusive)
   //   {
   //
   //      if(eexclusive == ExclusiveInstanceLocalId)
   //      {
   //         /*
   //         ::memory_file file(get_app());
   //         file.from_string(handler()->m_varTopicFile);
   //         COPYDATASTRUCT data;
   //         data.dwData = 1984;
   //         data.cbData = (uint32_t) file.get_length();
   //         data.lpData = file.get_data();
   //         oswindow oswindow = ::FindWindowA(NULL, "::draw2d::fontopus::message_wnd::session::");
   //
   //         ::SendMessage(oswindow, WM_COPYDATA, NULL, (LPARAM) &data);*/
   //
   //#if defined(WINDOWSEX) || defined(LINUX) || defined(APPLEOS)
   //
   //         ::aura::ipc::tx channel;
   //
   //         if(channel.open("::draw2d::fontopus::message_wnd::session::"))
   //         {
   //            channel.send(handler()->m_varTopicFile, false);
   //            channel.close();
   //         }
   //
   //#else
   //
   //          _throw(todo(get_app()));
   //
   //#endif
   //
   //
   //      }
   //
   //   }
   //

   void session::request_create(::create * pcreate)
   {

      ::base::session::request_create(pcreate);

   }

   void session::request_topic_file(var & varQuery)
   {
      request_file_query(Session.m_varTopicFile, varQuery);
   }

   void session::request_topic_file()
   {
      request_file(Session.m_varTopicFile);
   }

   /*void session::request_application(const char * pszId, var varFile, var varQuery, application_bias * pbiasCreate)
   {

   ::core::application_request request;

   request.m_iEdge         = m_iEdge;
   request.m_strApp        = pszId;
   request.m_varFile       = varFile;
   request.m_varQuery      = varQuery;
   request.m_pbiasCreate   = pbiasCreate;

   request_application(&request);

   }*/
   /*
   sp(::aura::application) session::application_get(const char * pszType, const char * pszId, bool bCreate, bool bSynch, application_bias * pbiasCreate)
   {
   ::aura::application * papp = NULL;

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
   catch(::exit_exception & e)
   {

   _throw(e);

   }
   catch(::exception::exception &)
   {

   if(!Application.on_run_exception(e))
   _throw(exit_exception(get_app()));

   }
   catch(...)
   {

   papp = NULL;

   }

   if(papp == NULL)
   return NULL;

   m_mapApplication.set_at(string(pszType) + ":" + string(pszId), papp);
   return papp;
   }
   }
   */


   bool session::is_session()
   {
      return true;
   }

   sp(::aura::application) session::get_current_application()
   {
      return Session.m_pappCurrent;
   }




   bool session::on_install()
   {

      string strFormat;

      string strSentinelPath;

      strSentinelPath = System.dir().install() / "stage/x86/app.sentinel.exe";

      System.os().local_machine_set_run("core app.sentinel", "\"" + strSentinelPath + "\"", true);

      System.os().defer_register_ca2_plugin_for_mozilla();

      return ::core::application::on_install();

   }




   bool session::is_remote_session()
   {


      return System.os().is_remote_session();


   }


   bool session::is_mouse_button_pressed(::user::e_mouse emouse)
   {

      if(emouse == ::user::mouse_left_button)
      {
         return Session.is_key_pressed(::user::key_lbutton);
      }
      else if(emouse == ::user::mouse_right_button)
      {
         return Session.is_key_pressed(::user::key_rbutton);
      }
      else if(emouse == ::user::mouse_middle_button)
      {
         return Session.is_key_pressed(::user::key_mbutton);
      }
      else
      {
         _throw(simple_exception(get_app(), "not expected e_mouse value"));
      }


   }






   bool session::open_file(::filemanager::data * pdata, ::fs::item_array & itema)
   {

      UNREFERENCED_PARAMETER(pdata);

      if(itema.get_size() > 0)
      {

         return true;

      }

      return false;

   }








   bool session::InitializeLocalDataCentral()
   {

      //m_pdatabase = new nature::database(this);

      //if(m_pdatabase == NULL)
      //{
      //   TRACE("VmpLightApp::init_instance failed to instatiate LightDB\n");
      //   return false;
      //}

      //if(!m_pdatabase->Initialize())
      //{
      //   TRACE("VmpLightApp::init_instance failed to initialize LightDB\n");
      //   return false;
      //}

      return true;

   }


   bool session::on_exclusive_instance_conflict(EExclusiveInstance eexclusive)
   {
      if(eexclusive == ExclusiveInstanceLocalId)
      {
#ifdef WINDOWSEX
         ::memory_file file(get_app());
         file.from_string(handler()->m_varTopicFile);
         COPYDATASTRUCT data;
         data.dwData = 1984;
         data.cbData = (uint32_t) file.get_length();
         data.lpData = file.get_data();
         ::oswindow oswindow = ::FindWindowA(NULL, "::draw2d::fontopus::message_wnd::bergedge::");


         ::SendMessage(oswindow, WM_COPYDATA, 0, (LPARAM) &data);

         return true;

#else

         _throw(todo(get_app()));

#endif

      }

      return false;

   }


   /*void session::request_application(const char * pszId, var varFile, var varQuery, application_bias * pbiasCreate)
   {

   ::core::application_request request;

   request.m_iEdge         = m_iEdge;
   request.m_strApp        = pszId;
   request.m_varFile       = varFile;
   request.m_varQuery      = varQuery;
   request.m_pbiasCreate   = pbiasCreate;

   request_application(&request);

   }*/

   ::aura::application * session::application_get(const char * pszAppId, bool bCreate, bool bSynch, application_bias * pbiasCreate)
   {

      sp(::aura::application) papp;

      if(m_psession->appptra().lookup(pszAppId,papp))
      {

         return papp;

      }

         if(!bCreate)
            return NULL;

         papp = NULL;

         try
         {

            papp = create_application(pszAppId, bSynch, pbiasCreate);

         }
         catch (exit_exception * pexception)
         {

            _rethrow(pexception);

         }
         catch (::exception::exception * pexception)
         {

            esp671 esp(pexception);

            if (!App(this).on_run_exception(esp))
            {

               papp = NULL;

            }

         }
         catch(...)
         {

            papp = NULL;

         }

         if (papp == NULL)
         {

            return NULL;

         }

         if(&App(papp.m_p) == NULL)
         {

            return NULL;

         }

         m_psession->appptra_add(papp);

         return papp;

   }


   void session::check_topic_file_change()
   {

      if(Session.m_varCurrentViewFile != Session.m_varTopicFile && !Session.m_varTopicFile.is_empty())
      {

         Session.m_varCurrentViewFile = Session.m_varTopicFile;

         request_topic_file();

      }

   }


   //::user::place_holder_ptra session::get_place_holder(sp(::user::frame_window) pmainframe, ::create * pcreate)
   //{

   //   UNREFERENCED_PARAMETER(pcreate);

   //   ::user::place_holder_ptra holderptra;


   //   ::core::application & app = App(pmainframe->get_app());

   //   string strAppName = app.m_strAppName;

   //   //if(get_document()->get_typed_view < ::bergedge::pane_view >() != NULL)
   //   //{

   //   //   get_document()->get_typed_view < ::bergedge::pane_view >()->set_cur_tab_by_id("app:" + strAppName);

   //   //   holderptra.add(get_document()->get_typed_view < ::bergedge::pane_view >()->get_tab_holder(get_document()->get_typed_view < ::bergedge::pane_view >()->get_tab_by_id("app:" + strAppName)));

   //   //}
   //   //else
   //   //{

   //   //   holderptra.add(get_document()->get_typed_view < ::bergedge::view >());

   //   //}

   //   return holderptra;

   //}

   bool session::place(::user::main_frame * pmainframe, ::create * pcreate)
   {

      //get_place_holder(pmainframe, pcreate).hold(pmainframe);

      return true;

   }





   void session::set_app_title(const char * pszAppId, const char * pszTitle)
   {

      sp(::aura::application) papp;

      if(m_psession->appptra().lookup(pszAppId, papp) && papp.is_set())
      {

         //sp(::bergedge::pane_view) ppaneview = get_document()->get_typed_view < ::bergedge::pane_view >();

         //if(ppaneview != NULL)
         //{

         //   string strAppName(pszAppId);

         //   ::user::tab::pane * ppane = ppaneview->get_pane_by_id("app:" + strAppName);

         //   if(ppane != NULL)
         //   {

         //      ppane->m_istrTitleEx = pszTitle;

         //      ppaneview->on_layout();

         //   }

         //}

      }

   }



   sp(::core::session) session::get_session()
   {

      return this;

   }



   bool session::is_serviceable()
   {


//      return ::session::application::is_serviceable();
      return false;


   }


   bool session::on_uninstall()
   {


      bool bOk1 = true;
      bool bOk2 = true;

      try
      {

         bOk1 = ::core::application::on_uninstall();

      }
      catch(...)
      {
         bOk1 = false;
      }




      return bOk1 && bOk2;


   }


   bool session::os_native_bergedge_start()
   {

      if (!::core::application::os_native_bergedge_start())
      {

         return false;

      }

      return true;

   }


   service_base * session::allocate_new_service()
   {


      return ::core::application::allocate_new_service();


   }





   ::visual::cursor * session::get_default_cursor()
   {

      return System.visual().get_cursor(m_psession->m_ecursorDefault);

   }


   void session::main()
   {

      ::core::application::main();

   }


   ::core::session * session::query_bergedge()
   {

      ::core::session * psession = NULL;

      if(System.m_pbergedgemap == NULL)
         return NULL;

      if(!System.m_pbergedgemap->Lookup(0,psession))
      {
         return NULL;
      }

      return psession;

   }

   void session::on_user_login(::fontopus::user * puser)
   {

      ::base::session::on_user_login(puser);


   }


   void session::_001OnDefaultTabPaneDrawTitle(::user::tab_pane & pane, ::user::tab * ptab, ::draw2d::graphics * pgraphics, LPCRECT lpcrect, ::draw2d::brush_sp & brushText)
   {

      stringa & straTitle = pane.m_straTitle;

      pgraphics->SelectObject(brushText);

      if (straTitle.get_count() <= 1)
      {

         pgraphics->_DrawText(pane.get_title(), *lpcrect, DT_LEFT | DT_BOTTOM | DT_NOPREFIX);

      }
      else
      {

         ::rect rectText(lpcrect);

         ::draw2d::font_sp font;
         font = pgraphics->get_current_font();
         size sSep = ptab->get_data()->m_sizeSep;
         ::rect rectEmp;
         for (index i = 0; i < straTitle.get_size(); i++)
         {
            string str = straTitle[i];
            size s = pane.m_sizeaText[i];
            rectText.right = rectText.left + s.cx;
            pgraphics->_DrawText(str, rectText, DT_LEFT | DT_BOTTOM | DT_NOPREFIX);
            rectText.left += s.cx;
            if (i < straTitle.get_upper_bound())
            {
               rectText.right = rectText.left + sSep.cx;
               rectEmp = rectText;
               rectEmp.deflate(1, 1);
               ::draw2d::e_alpha_mode emode = pgraphics->m_ealphamode;
               pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
               if (ptab->m_eelementHover == (int)::user::element_split + i)
               {
                  pgraphics->fill_solid_rect(rectEmp, ARGB(128, 149, 184, 255));
                  pgraphics->SelectObject(ptab->get_data()->m_brushTextHover);
               }
               else
               {
                  //pgraphics->fill_solid_rect(rectEmp,ARGB(128,208,223,233));
                  pgraphics->SelectObject(ptab->get_data()->m_brushText);
               }
               pgraphics->set_font(ptab->_001GetFont(::user::font_tab_big_bold));
               pgraphics->set_alpha_mode(emode);
               pgraphics->_DrawText(MAGIC_PALACE_TAB_TEXT, rectText, DT_CENTER | DT_VCENTER | DT_NOPREFIX);
               rectText.left += sSep.cx;
               pgraphics->selectFont(font);
               pgraphics->SelectObject(brushText);
            }
         }

      }

   }


   stringa session::get_user_wallpaper()
   {

      stringa stra;

      stringa straSource = Session.userex()->get_wallpaper();

      for(string str : straSource)
      {

         if(::str::begins_eat_ci(str, "file://"))
         {

            str = System.url().url_decode(str);

         }

         stra.add(str.c_str());

      }

      return stra;

   }

   ::user::menu_interaction * session::create_menu_button(::user::menu_item * pitem)
   {

      ::user::menu_button * pui = canew(::user::menu_button(pitem));

      if (pitem->m_dib.is_set() && pitem->m_dib->area() > 0)
      {

         pui->set_button_style(::user::button::style_image_and_text);

         pui->LoadBitmaps(pitem->m_dib);

      }

      return pui;

   }


} // namespace plane



