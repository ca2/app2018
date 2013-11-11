#include "framework.h"


namespace user
{


   user::user(base_application * papp) :
      element(papp),
      ::base_departament(papp)
   {
      
      
      m_pkeyboardfocus  = NULL;
      m_pshellimageset  = NULL;
      m_pkeyboard       = NULL;
      m_pufeschema      = NULL;
      m_pufe            = NULL;


      //::core::user * papp = dynamic_cast <::core::user *>(System.GetThread()->m_pAppThread);
      //::core::connect(papp->m_pplaneapp->m_signalAppLanguageChange, this, &user::VmsGuiiOnAppLanguage);

      m_pkeyboard = NULL;
      //m_pwindowmap = NULL;

   }

   user::~user()
   {
   }


   bool user::initialize1()
   {

      if(m_pbaseapp->is_system())
      {
         m_pwindowmap = new class ::user::window_map(get_app());
      }
      else
      {
         m_pwindowmap = System.user()->m_pwindowmap;
      }

      m_pkeyboard = new ::user::keyboard(m_pbaseapp);

      if(m_pkeyboard == NULL)
         return false;







      if(m_pbaseapp->is_system())
      {
         System.factory().creatable_small < keyboard_layout > ();
      }

      m_pshellimageset = new filemanager::_shell::ImageSet(m_pbaseapp);

      if(!base_departament::initialize1())
         return false;

      return true;

   }

   bool user::initialize()
   {




      if(!base_departament::initialize())
         return false;


      Application.simpledb().get_data_server()->add_client(this);


      TRACE("::user::application::initialize");

      xml::document docUser(get_app());
      string strUser = Application.file().as_string(Application.dir().userappdata("langstyle_settings.xml"));
      string strLangUser;
      string strStyleUser;
      if(docUser.load(strUser))
      {
         if(docUser.get_child("lang") != NULL)
         {
            strLangUser = docUser.get_child("lang")->get_value();
         }
         if(docUser.get_child("style") != NULL)
         {
            strStyleUser = docUser.get_child("style")->get_value();
         }
      }

      if(strLangUser.has_char())
         Application.set_locale(strLangUser, false);
      if(strStyleUser.has_char())
         Application.set_schema(strStyleUser, false);

      string strLicense = Application.get_license_id();


      var & varTopicQuey = System.directrix()->m_varTopicQuery;

      bool bHasInstall = varTopicQuey.has_property("install");

      bool bHasUninstall = varTopicQuey.has_property("uninstall");

      if(!(bHasInstall || bHasUninstall)
         && Application.m_bLicense
         && strLicense.has_char())
      {

         if(&ApplicationUser == NULL)
         {
            return false;
         }

         // call application's is_licensed function
         // because if delay is needed for authentication -
         // or either asking for authentication -
         // current application startup won't be
         // exited by timeout.

         int32_t iRetry = 3;

retry_license:

         iRetry--;

         if(!Application.is_licensed(strLicense))
         {

            Application.license().m_mapInfo.remove_key(strLicense);

            if(iRetry > 0)
               goto retry_license;

            return false;

         }

      }

      set_data_server(Application.simpledb().get_data_server());


      if(!Application.is_system())
      {

         m_pufeschema      = new ::user::front_end_schema(get_app());
         m_pufe            = new ::user::front_end();

      }

      if(Application.is_system())
      {

         System.factory().cloneable_small < int_biunique > ();
         System.factory().creatable_small < ::user::edit_plain_text > ();
         System.factory().cloneable_small < XfplayerViewLine > ();
         System.factory().creatable_small < ::user::place_holder > ();
         System.factory().creatable_small < ::user::place_holder_container > ();


         System.factory().cloneable_small < ::user::split_layout > ();
         System.factory().cloneable_small < ::user::split_bar >();
         System.factory().cloneable_small < ::user::split_view::Pane >();
         System.factory().cloneable_small < simple_frame_window >();
         System.factory().creatable_small < MetaButton >();
         System.factory().creatable_small < BaseButtonControl >();
         System.factory().creatable_small < ::user::menu_list_view >();
         System.factory().cloneable_small < simple_child_frame >();
         System.factory().cloneable_small < simple_main_frame >();
         System.factory().creatable_small < simple_button >();
         System.factory().cloneable_small < ::user::document >();
         System.factory().cloneable_small < ::user::split_view >();
         System.factory().cloneable_small < ::user::edit_plain_text_view >();
         System.factory().creatable_small < simple_list_view >();
         System.factory().creatable_small < simple_document > ();
         System.factory().creatable_small < simple_printer_list_view > ();
         System.factory().creatable_small < simple_combo_list > ();

         System.factory().creatable_small < ::user::tree >();


         System.factory().creatable_small < SimplePushButton > ();


      // menu
      System.factory().creatable_small < menu_document > ();
      System.factory().creatable_small < menu_frame > ();
      System.factory().creatable_small < menu_view > ();

      // form
      System.factory().creatable_small < form_document > ();
      System.factory().creatable_small < ::userex::pane_tab_view > ();
      System.factory().creatable_small < form_frame > ();
      System.factory().creatable_small < form_child_frame > ();
      System.factory().creatable_small < form_view > ();

      System.factory().creatable_small < ::userex::hour_list_view > ();
      System.factory().creatable_small < ::userex::minute_list_view > ();
      System.factory().creatable_small < ::userex::second_list_view > ();




      }


      //if(Application.m_pdocmanager != NULL)
        // Application.m_pdocmanager->add_document_template(NULL);

      if(!BaseMenuCentralContainer::initialize_central_container(get_app()))
         return false;


      if(!base_departament::initialize())
         return false;

      return true;

   }




   bool user::initialize2()
   {


      if(!base_departament::initialize2())
         return false;

      m_ptemplateForm = new ::user::multiple_document_template(
         get_app(),
         "system/form",
         System.type_info < form_document > (),
         System.type_info < form_frame > (),
         System.type_info < form_view > ());

      m_ptemplateChildForm = new ::user::multiple_document_template(
         get_app(),
         "system/form",
         System.type_info < form_document > (),
         System.type_info < form_child_frame > (),
         System.type_info < form_view > ());

      m_ptemplatePlaceHolder = new ::user::multiple_document_template(
         get_app(),
         "system/form",
         System.type_info < simple_document > (),
         System.type_info < simple_frame_window > (),
         System.type_info < ::user::place_holder > ());


      return true;


   }


   bool user::finalize()
   {
      try
      {
         base_departament::finalize();
      }
      catch(...)
      {
      }

      try
      {
         if(m_pufeschema != NULL)
         {
            delete m_pufeschema;
            m_pufeschema = NULL;
         }
      }
      catch(...)
      {
      }

      try
      {
         if(m_pufe != NULL)
         {
            delete m_pufe;
            m_pufe = NULL;
         }
      }
      catch(...)
      {
      }

      try
      {
         if(!BaseMenuCentralContainer::finalize_central_container())
         {
            TRACE("Errors occurred while finalizing BaseMenuCentralContainer");
         }
      }
      catch(...)
      {
      }

      return true;

   }


   filemanager::_shell::ImageSet & user::shellimageset()
   {
      return *m_pshellimageset;
   }

   string user::message_box(const char * pszMatter, property_set & propertyset)
   { 
         class ::userex::message_box box(get_app());

         box.show(pszMatter, &propertyset);

         return box.m_strResponse;

   }

   int32_t user::simple_message_box(sp(::user::interaction) pwndOwner, const char * pszMessage, UINT fuStyle)
   {

      //if(m_pbaseapp->m_pplaneapp->m_psession != NULL)
      //{
        // return m_pbaseapp->m_pplaneapp->m_psession->m_pplanesession->m_pplanesession->simple_message_box(pwndOwner, pszMessage, fuStyle);
      //}

      class ::userex::message_box box(get_app());

      property_set propertyset;
      propertyset["message"] = pszMessage;

      string strMatter;
      if(fuStyle & MB_YESNOCANCEL)
      {
         strMatter = "system\\user\\simple_message_box\\yesnocancel.xhtml";
      }
      else
      {
         strMatter = "system\\user\\simple_message_box\\ok.xhtml";
      }
      try
      {
         if(!box.show(strMatter, &propertyset))
         {
            string strMessage = pszMessage;
            strMessage.replace("<br>", "\r\n");
            return MessageBox((oswindow) (pwndOwner.is_null() ? NULL : pwndOwner->get_wnd()->get_os_data()), strMessage, Application.m_strAppName, fuStyle);
         }
      }
      catch(...)
      {
         string strMessage = pszMessage;
         strMessage.replace("<br>", "\r\n");
         return MessageBox(pwndOwner == NULL ? NULL : pwndOwner->get_handle(), strMessage, Application.m_strAppName, fuStyle);
      }
      if(box.m_strResponse == "ok")
      {
         return IDOK;
      }
      else if(box.m_strResponse == "yes")
      {
         return IDYES;
      }
      else if(box.m_strResponse == "no")
      {
         return IDNO;
      }
      else if(box.m_strResponse == "cancel")
      {
         return IDCANCEL;
      }
      if(fuStyle & MB_YESNOCANCEL)
      {
         return IDCANCEL;
      }
      else
      {
         return 0;
      }
   }

   int32_t user::simple_message_box_timeout(sp(::user::interaction) puiOwner, const char * pszMessage, ::duration durationTimeout, UINT fuStyle)
   {

      UNREFERENCED_PARAMETER(puiOwner);

      class ::userex::message_box box(get_app());

      property_set propertyset;
      propertyset["message"] = pszMessage;
      propertyset["simple_message_box_timeout_ms"] = durationTimeout.get_total_milliseconds();

      string strMatter;
      if(fuStyle & MB_YESNOCANCEL)
      {
         strMatter = "system\\user\\simple_message_box\\yesnocancel.xhtml";
      }
      else
      {
         strMatter = "system\\user\\simple_message_box\\ok.xhtml";
      }
      
      box.show(strMatter, &propertyset);

      if(box.m_strResponse == "ok")
      {
         return IDOK;
      }
      else if(box.m_strResponse == "yes")
      {
         return IDYES;
      }
      else if(box.m_strResponse == "no")
      {
         return IDNO;
      }
      else if(box.m_strResponse == "cancel")
      {
         return IDCANCEL;
      }
      if(fuStyle & MB_YESNOCANCEL)
      {
         return IDCANCEL;
      }
      else
      {
         return 0;
      }
   }

   int32_t user::track_popup_menu(const char * pszMatter, point pt, sp(::user::interaction) puie)
   {
      UNREFERENCED_PARAMETER(pszMatter);
      UNREFERENCED_PARAMETER(pt);
      UNREFERENCED_PARAMETER(puie);
      return 1;
   }


   bool user::get_fs_size(string & strSize, const char * pszPath, bool & bPending)
   {
      int64_t i64Size;
      if(!get_fs_size(i64Size, pszPath, bPending))
      {
         strSize.Empty();
         return false;
      }
      if(i64Size > 1024 * 1024 * 1024)
      {
         double d = (double) i64Size / (1024.0 * 1024.0 * 1024.0);
         strSize.Format("%0.2f GB", d);
      }
      else if(i64Size > 1024 * 1024)
      {
         double d = (double) i64Size / (1024.0 * 1024.0);
         strSize.Format("%0.1f MB", d);
      }
      else if(i64Size > 1024)
      {
         double d = (double) i64Size / (1024.0);
         strSize.Format("%0.0f KB", d);
      }
      else if(i64Size > 0)
      {
         strSize.Format("1 KB");
      }
      else
      {
         strSize.Format("0 KB");
      }
      if(bPending)
      {
         strSize = "~" + strSize;
      }
      return true;
   }

   bool user::get_fs_size(int64_t & i64Size, const char * pszPath, bool & bPending)
   {
      db_server * pcentral = dynamic_cast < db_server * > (&System.m_simpledb.db());
      if(pcentral == NULL)
         return false;
      return pcentral->m_pfilesystemsizeset->get_cache_fs_size(i64Size, pszPath, bPending);
   }

   void user::data_on_after_change(signal_details * pobj)
   {
      SCAST_PTR(::database::change_event, pchange, pobj);
      if(pchange->m_key.m_idKey == "ca2")
      {
         if(pchange->m_key.m_idIndex  == "savings")
         {
            pchange->data_get(Application.savings().m_eresourceflagsShouldSave);
         }
      }
   }


   sp(::user::keyboard_focus) user::get_keyboard_focus()
   {
      if(Application.is_session())
      {
         sp(::user::interaction) puieFocus = Application.get_focus_guie();
         if(m_pkeyboardfocus != NULL && puieFocus != NULL)
         {
            if((bool)oprop("NativeWindowFocus") && puieFocus != m_pkeyboardfocus->get_wnd())
               return NULL;
            return m_pkeyboardfocus;
         }
         else
         {
            return NULL;
         }
      }
      else if(Application.is_system())
      {
         return m_pkeyboardfocus;
      }
      else if(Application.m_psession != NULL)
      {
         return Sess(get_app()).user()->get_keyboard_focus();
      }
      else if(Application.m_psystem != NULL)
      {
         return Sys(get_app()).user()->get_keyboard_focus();
      }
      else
      {
         return NULL;
      }
   }

   void user::set_keyboard_focus(sp(::user::keyboard_focus) pkeyboardfocus)
   {
      if(Application.is_session())
      {
         if(pkeyboardfocus == NULL || pkeyboardfocus->keyboard_focus_OnSetFocus())
         {
            m_pkeyboardfocus = pkeyboardfocus;
         }
         if(Application.m_psystem != NULL)
         {
            return Sys(get_app()).user()->set_keyboard_focus(pkeyboardfocus);
         }
      }
      else if(Application.is_system())
      {
         if(pkeyboardfocus == NULL || pkeyboardfocus->keyboard_focus_OnSetFocus())
         {
            m_pkeyboardfocus = pkeyboardfocus;
         }
      }
      else if(Application.m_psession != NULL)
      {
         return Sess(get_app()).user()->set_keyboard_focus(pkeyboardfocus);
      }
      else if(Application.m_psystem != NULL)
      {
         return Sys(get_app()).user()->set_keyboard_focus(pkeyboardfocus);
      }
   }

   ::user::mouse_focus * user::get_mouse_focus_LButtonDown()
   {
      return m_pmousefocusLButtonDown;
   }

   void user::set_mouse_focus_LButtonDown(::user::mouse_focus * pmousefocus)
   {
      m_pmousefocusLButtonDown = pmousefocus;
   }

   ::user::mouse_focus * user::get_mouse_focus_RButtonDown()
   {
      return m_pmousefocusRButtonDown;
   }

   void user::set_mouse_focus_RButtonDown(::user::mouse_focus * pmousefocus)
   {
      m_pmousefocusRButtonDown = pmousefocus;
   }


   bool user::set_keyboard_layout(const char * pszPath, bool bUser)
   {

      if(pszPath == NULL)
      {

         if(&keyboard().layout() != NULL)
         {

            if(Application.fontopus()->m_puser != NULL
               && Application.fontopus()->m_puser->m_strFontopusServerSessId.has_char())
            {

               data_set("keyboard_layout", keyboard().layout().m_strPath);

            }

            return true;
         }

         if(keyboard().get_current_system_layout().is_empty())
            return false;

         if(!set_keyboard_layout(keyboard().get_current_system_layout(), false))
            return false;

         if(Application.fontopus()->m_puser != NULL
            && Application.fontopus()->m_puser->m_strFontopusServerSessId.has_char())
         {

            data_set("keyboard_layout", keyboard().layout().m_strPath);

         }

         return true;
      }

      if(!System.user()->keyboard().load_layout(pszPath, bUser))
         return false;

      Application.simpledb().on_set_keyboard_layout(pszPath, bUser);

      return true;
   }


   ::user::keyboard & user::keyboard()
   {
      if(!Application.is_system())
      {
         return System.user()->keyboard();
      }
      return *m_pkeyboard;
   }


   class window_map & user::window_map()
   {
      return *m_pwindowmap;
   }


   sp(type) user::controltype_to_typeinfo(::user::control::e_type e_type)
   {

      return sp(type)();

   }

   void user::SendMessageToWindows(UINT message, WPARAM wparam, LPARAM lparam)
   {
      ::user::interaction_ptr_array wnda = Application.frames();
      for(int32_t i = 0; i < wnda.get_size(); i++)
      {
         sp(::user::interaction) pwnd = wnda.element_at(i);
         if(pwnd != NULL && pwnd->IsWindow())
         {
            pwnd->send_message(message, wparam, lparam);
            pwnd->SendMessageToDescendants(message, wparam, lparam);
         }
      }
   }






   void user::add_document_template(sp(::user::document_template) ptemplate)
   {
      if(Application.m_pdocmanager == NULL)
         Application.m_pdocmanager = new ::user::document_manager(get_app());
      Application.m_pdocmanager->add_document_template(ptemplate);
   }


   void user::defer_add_document_template(sp(::user::document_template) ptemplate)
   {
      sp(::user::document_template) puserbasetemplate =  (ptemplate);

      if(puserbasetemplate == NULL)
         throw "should add user base document template";

      add_document_template(puserbasetemplate);

   }

   sp(::user::document) user::_vmsguserbaseOpenDocumentFile(const char * lpszFileName)
   {
      ASSERT(Application.m_pdocmanager != NULL);
      sp(::create_context) cc(allocer());
      cc->m_spCommandLine->m_varFile = lpszFileName;
      return (Application.m_pdocmanager->open_document_file(cc));
   }


   bool user::_001OnCmdMsg(BaseCmdMsg * pcmdmsg)  
   {
      UNREFERENCED_PARAMETER(pcmdmsg);
      return false;
   }





   void  user::AddToRecentFileList(const char * lpszPathName)
   {
      UNREFERENCED_PARAMETER(lpszPathName);
   }

   void  user::_001CloseAllDocuments(bool bEndSession)
   {
      if(Application.m_pdocmanager != NULL)
      {
         Application.m_pdocmanager->close_all_documents(bEndSession);
      }
   }


   ::user::front_end_schema * GetUfeSchema(sp(base_application) papp)
   {
      return App(papp).user()->GetUfeSchema();
   }

   ::user::front_end * GetUfe(sp(base_application) papp)
   {
      return App(papp).user()->GetUfe();
   }


   void user::_001OnFileNew()
   {
      Application.m_pdocmanager->_001OnFileNew();
   }

   ::user::front_end_schema * user::GetUfeSchema()
   {
      return m_pufeschema;
   }

   ::user::front_end * user::GetUfe()
   {
      return m_pufe;
   }


      int32_t user::exit_instance()
      {
/*         if(Application.is_system())
         {
            if(m_pwindowmap != NULL)
            {
               delete m_pwindowmap;
               m_pwindowmap = NULL;
            }
         }*/
         return 0;
      }




      int32_t user::GetVisibleFrameCountExcept(sp(::user::interaction) pwndExcept)
      {
         ::user::interaction_ptr_array wnda = Application.frames();
         int32_t iCount = 0;
         for(int32_t i = 0; i < wnda.get_size(); i++)
         {
              sp(::user::interaction) pwnd = wnda.element_at(i);
              if(pwnd != NULL &&
                  pwnd != pwndExcept &&
                  pwnd->IsWindowVisible())
              {
                  iCount++;
              }
          }
          return iCount;
      }

      int32_t user::GetVisibleTopLevelFrameCountExcept(sp(::user::interaction) pwndExcept)
      {
         ::user::interaction_ptr_array wnda = Application.frames();
         int32_t iCount = 0;
         for(int32_t i = 0; i < wnda.get_size(); i++)
         {
            sp(::user::interaction) pwnd = wnda.element_at(i);
            if(pwnd != NULL &&
               pwnd != pwndExcept &&
               pwnd->IsWindow() &&
               pwnd->IsWindowVisible() &&
               !(pwnd->GetStyle() & WS_CHILD))
            {
               iCount++;
            }
         }
         return iCount;
      }

      int32_t user::GetVisibleFrameCount()
      {
         ::user::interaction_ptr_array wnda = Application.frames();
         int32_t iCount = 0;
         for(int32_t i = 0; i < wnda.get_size(); i++)
         {
            sp(::user::interaction) pwnd = wnda.element_at(i);
            if(pwnd != NULL
               && pwnd->IsWindow()
               && pwnd->IsWindowVisible())
            {
               iCount++;
            }
         }
         return iCount;
      }

      void user::VmsGuiiOnAppLanguage(signal_details * pobject)
      {
         SendMessageToWindows(application::APPM_LANGUAGE, 0, (LPARAM) pobject);
      }


   sp(::form_document) user::create_form(sp(form_view) pview, ::user::form_callback * pcallback, sp(::user::interaction) pwndParent, var var)
   {
      sp(::form_document) pdoc;
      if(m_ptemplateForm == NULL)
         return NULL;
      sp(::create_context) createcontext(allocer());
      createcontext->m_bMakeVisible                   = false;
      createcontext->m_puiParent                      = pwndParent;
      createcontext->m_puiAlloc                       = pview;

      if(var.get_type() == var::type_propset && var.has_property("hold") && !(bool) var["hold"])
      {
         createcontext->m_bHold                       = false;
      }

      pdoc = (m_ptemplateForm->open_document_file(createcontext));
      pview = pdoc->get_typed_view < form_view >();
      pdoc->get_html_data()->m_pform = pview;
      pdoc->get_html_data()->m_pform->m_pcallback = pcallback;
      return pdoc;
   }

   sp(::form_document) user::create_form(::user::form_callback * pcallback, sp(::user::interaction) pwndParent, var var)
   {
      if(pwndParent != NULL && pwndParent->m_pbaseapp != get_app())
      {
         return App(pwndParent->m_pbaseapp).user()->create_form(pcallback, pwndParent, var);
      }
      sp(::form_document) pdoc;
      if(m_ptemplateForm == NULL)
         return NULL;
      sp(::create_context) createcontext(allocer());
      createcontext->m_bMakeVisible                   = false;
      createcontext->m_puiParent                      = pwndParent;
      if(var.get_type() == var::type_propset && var.has_property("hold") && !(bool) var["hold"])
      {
         createcontext->m_bHold                       = false;
      }

      pdoc = (m_ptemplateForm->open_document_file(createcontext));
      sp(::user::view) pview = pdoc->get_view(0);
      pdoc->get_html_data()->m_pform =  (pview);
      pdoc->get_html_data()->m_pform->m_pcallback = pcallback;
      return pdoc;
   }


   sp(::form_document) user::create_child_form(sp(form_view) pview, ::user::form_callback * pcallback, sp(::user::interaction) pwndParent, var var)
   {
      sp(::form_document) pdoc;
      if(m_ptemplateChildForm == NULL)
         return NULL;
      sp(::create_context) createcontext(allocer());
      createcontext->m_bMakeVisible                   = false;
      createcontext->m_puiParent                      = pwndParent;
      createcontext->m_puiAlloc                       = pview;

      if(var.get_type() == var::type_propset && var.has_property("hold") && !(bool) var["hold"])
      {
         createcontext->m_bHold                       = false;
      }

      pdoc = (m_ptemplateChildForm->open_document_file(createcontext));
      pview = pdoc->get_typed_view < form_view >();
      pdoc->get_html_data()->m_pform = pview;
      pdoc->get_html_data()->m_pform->m_pcallback = pcallback;
      return pdoc;
   }

   sp(::form_document) user::create_child_form(::user::form_callback * pcallback, sp(::user::interaction) pwndParent, var var)
   {
      if(pwndParent != NULL && pwndParent->m_pbaseapp != get_app())
      {
         return App(pwndParent->m_pbaseapp).user()->create_child_form(pcallback, pwndParent, var);
      }
      if(m_ptemplateChildForm == NULL)
         return NULL;
      sp(::form_document) pdoc;
      sp(::create_context) createcontext(allocer());
      createcontext->m_bMakeVisible                   = false;
      createcontext->m_puiParent                      = pwndParent;
      pdoc = (m_ptemplateChildForm->open_document_file(createcontext));
      sp(::user::view) pview = pdoc->get_view(0);
      pdoc->get_html_data()->m_pform =  (pview);
      pdoc->get_html_data()->m_pform->m_pcallback = pcallback;
      return pdoc;
   }


   sp(::user::document) user::hold(sp(::user::interaction) pui)
   {

      sp(::create_context) createcontext(allocer());

      createcontext->m_bMakeVisible    = false;
      createcontext->m_bHold           = false;

      sp(simple_document) pdoc =  (m_ptemplatePlaceHolder->open_document_file(createcontext));

      sp(::user::place_holder) pholder = pdoc->get_typed_view < ::user::place_holder  >();

      pholder->hold(pui);

      return pdoc;
   }


} //namespace user



