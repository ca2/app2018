//#include "framework.h"


namespace userex
{


   userex::userex(::aura::application * papp):
      element(papp),
      ::aura::departament(papp),
      ::user::core(papp)
   {

      m_pshellimageset  = NULL;
      m_pufeschema      = NULL;
      m_pufe            = NULL;

   }


   userex::~userex()
   {

   }


   bool userex::initialize1()
   {

      System.factory().creatable_small < keyboard_layout >();

      m_pshellimageset = new filemanager::_shell::ImageSet(m_pauraapp);

      if(!::aura::departament::initialize1())
         return false;

      return true;

   }


   bool userex::initialize()
   {


      if(!::user::core::initialize())
         return false;


      Application.simpledb().get_data_server()->add_client(this);


      TRACE("::userex::application::initialize");

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
         Session.set_locale(strLangUser,::action::source::database());
      if(strStyleUser.has_char())
         Session.set_schema(strStyleUser,::action::source::database());

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

         int32_t iRetry = 1;

      retry_license:

         iRetry--;

         if(!Session.is_licensed(strLicense))
         {

            Session.licensing().m_mapInfo.remove_key(strLicense);

            if(iRetry > 0)
               goto retry_license;

            return false;

         }

      }

      set_data_server(Application.simpledb().get_data_server());


      m_pufeschema      = new ::user::front_end_schema(get_app());
      m_pufe            = new ::user::front_end();


      // form
      System.factory().creatable_small < ::aura::document >();
      System.factory().creatable_small < pane_tab_view >();
      System.factory().creatable_small < form_frame >();
      System.factory().creatable_small < form_child_frame >();
      System.factory().creatable_small < ::user::form >();


      // hour (alarm configuration with ease ...

      //System.factory().creatable_small < weekday_list_view >();
      //System.factory().creatable_small < hour_list_view >();
      //System.factory().creatable_small < minute_list_view >();
      //System.factory().creatable_small < second_list_view >();
      System.factory().creatable_small < simple_toolbar >();
      System.factory().creatable_small < OrtoButton >();


      //if(Application.m_pdocmanager != NULL)
      // Application.m_pdocmanager->add_document_template(NULL);

      if(!BaseMenuCentralContainer::initialize_central_container(get_app()))
         return false;


      if(!::aura::departament::initialize())
         return false;

      return true;

   }




   bool userex::initialize2()
   {


      if(!::aura::departament::initialize2())
         return false;

      m_ptemplateForm = new ::user::multiple_document_template(
         get_app(),
         "system/form",
         System.type_info < ::aura::document >(),
         System.type_info < form_frame >(),
         System.type_info < ::user::form >());

      m_ptemplateChildForm = new ::user::multiple_document_template(
         get_app(),
         "system/form",
         System.type_info < ::aura::document >(),
         System.type_info < form_child_frame >(),
         System.type_info < ::user::form >());

      m_ptemplatePlaceHolder = new ::user::multiple_document_template(
         get_app(),
         "system/form",
         System.type_info < ::aura::document >(),
         System.type_info < simple_frame_window >(),
         System.type_info < ::user::place_holder >());


      return true;


   }


   bool userex::finalize()
   {
      try
      {
         ::aura::departament::finalize();
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

      //try
      //{

      //   Application.m_pdocmanager.release();

      //}
      //catch(...)
      //{

      //}

      return true;

   }


   filemanager::_shell::ImageSet & userex::shellimageset()
   {

      return *m_pshellimageset;

   }


   string userex::message_box(const char * pszMatter,property_set & propertyset)
   {

      class ::userex::message_box box(get_app());

      box.show(pszMatter,&propertyset);

      return box.m_strResponse;

   }


   int32_t userex::simple_message_box(::aura::interaction * pwndOwner,const char * pszMessage,UINT fuStyle)
   {

      int iRet = ::simple_message_box(pwndOwner->get_safe_handle(), pszMessage, "ca2", fuStyle);

      return iRet;

      class ::userex::message_box box(get_app());

      property_set propertyset;
      propertyset["message"] = pszMessage;
      propertyset["application_name"] = pwndOwner == NULL ? Application.m_strAppName : App(pwndOwner->get_app()).m_strAppName;

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
         if(!box.show(strMatter,&propertyset))
         {
            string strMessage = pszMessage;
            strMessage.replace("<br>","\r\n");
            return ::simple_message_box(pwndOwner->get_safe_handle(),strMessage,Application.m_strAppName,fuStyle);
         }
      }
      catch(...)
      {
         string strMessage = pszMessage;
         strMessage.replace("<br>","\r\n");
         return ::simple_message_box(pwndOwner == NULL ? NULL : pwndOwner->get_handle(),strMessage,Application.m_strAppName,fuStyle);
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

   int32_t userex::simple_message_box_timeout(::aura::interaction * puiOwner,const char * pszMessage,::duration durationTimeout,UINT fuStyle)
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

      box.show(strMatter,&propertyset);

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

   int32_t userex::track_popup_menu(const char * pszMatter,point pt,sp(::user::interaction) puie)
   {
      UNREFERENCED_PARAMETER(pszMatter);
      UNREFERENCED_PARAMETER(pt);
      UNREFERENCED_PARAMETER(puie);
      return 1;
   }


   bool userex::get_fs_size(string & strSize,const char * pszPath,bool & bPending)
   {
      int64_t i64Size;
      if(!get_fs_size(i64Size,pszPath,bPending))
      {
         strSize.Empty();
         return false;
      }
      if(i64Size > 1024 * 1024 * 1024)
      {
         double d = (double)i64Size / (1024.0 * 1024.0 * 1024.0);
         strSize.Format("%0.2f GB",d);
      }
      else if(i64Size > 1024 * 1024)
      {
         double d = (double)i64Size / (1024.0 * 1024.0);
         strSize.Format("%0.1f MB",d);
      }
      else if(i64Size > 1024)
      {
         double d = (double)i64Size / (1024.0);
         strSize.Format("%0.0f KB",d);
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

   bool userex::get_fs_size(int64_t & i64Size,const char * pszPath,bool & bPending)
   {
      db_server * pcentral = dynamic_cast <db_server *> (&System.m_simpledb.db());
      if(pcentral == NULL)
         return false;
      return pcentral->m_pfilesystemsizeset->get_cache_fs_size(i64Size,pszPath,bPending);
   }

   void userex::data_on_after_change(signal_details * pobj)
   {
      SCAST_PTR(::database::change_event,pchange,pobj);
      if(pchange->m_key.m_id == "ca2.savings")
      {
         pchange->data_get(Session.savings().m_eresourceflagsShouldSave);
      }
   }


   sp(type) userex::controltype_to_typeinfo(::user::e_control_type e_type)
   {

      return sp(type)();

   }

   void userex::SendMessageToWindows(UINT message,WPARAM wparam,LPARAM lparam)
   {
      
      ::user::interaction * pwnd = NULL;

      while(Application.get_frame(pwnd))
      {
         if(pwnd != NULL && pwnd->IsWindow())
         {
            pwnd->send_message(message,wparam,lparam);
            pwnd->SendMessageToDescendants(message,wparam,lparam);
         }

      }

   }








   void  userex::AddToRecentFileList(const char * lpszPathName)
   {

      UNREFERENCED_PARAMETER(lpszPathName);

   }



   ::user::front_end_schema * GetUfeSchema(::aura::application * papp)
   {

      return Plat(papp).userex()->GetUfeSchema();

   }

   ::user::front_end * GetUfe(::aura::application * papp)
   {
      return Plat(papp).userex()->GetUfe();
   }


   void userex::_001OnFileNew()
   {
      Application.document_manager()._001OnFileNew();
   }

   ::user::front_end_schema * userex::GetUfeSchema()
   {
      return m_pufeschema;
   }

   ::user::front_end * userex::GetUfe()
   {
      return m_pufe;
   }



   int32_t userex::exit_instance()
   {

      return 0;

   }





   void userex::VmsGuiiOnAppLanguage(signal_details * pobject)
   {
      SendMessageToWindows(::base::application::APPM_LANGUAGE,0,(LPARAM)pobject);
   }


   sp(::aura::document) userex::create_form(sp(::user::form) pview,::user::form_callback * pcallback,sp(::user::interaction) pwndParent,var var)
   {
      sp(::aura::document) pdoc;
      if(m_ptemplateForm == NULL)
         return NULL;
      sp(::create) createcontext(allocer());
      createcontext->m_bMakeVisible                   = false;
      createcontext->m_puiParent                      = pwndParent;
      createcontext->m_puiAlloc                       = pview;

      if(var.get_type() == var::type_propset && var.has_property("hold") && !(bool)var["hold"])
      {
         createcontext->m_bHold                       = false;
      }

      pdoc = (m_ptemplateForm->open_document_file(createcontext));
      pview = pdoc->get_typed_view < ::user::form >();
      pdoc->form_document_set_view(pview);
      pdoc->form_document_set_callback(pcallback);
      return pdoc;
   }

   sp(::aura::document) userex::create_form(::user::form_callback * pcallback,sp(::user::interaction) pwndParent,var var)
   {
      //if(pwndParent != NULL && pwndParent->m_pauraapp != get_app())
      //{
      //   return Plat(pwndParent->m_pauraapp).userex()->create_form(pcallback,pwndParent,var);
      //}
      sp(::aura::document) pdoc;
      if(m_ptemplateForm == NULL)
         return NULL;

      ::aura::application * papp;

      if(pwndParent.is_set())
      {

         papp = pwndParent->get_app();

      }
      else if(pcallback != NULL)
      {

         papp = pcallback->get_app();

      }
      else
      {

         papp = get_app();

      }

      sp(::create) createcontext(papp->allocer());

      createcontext->m_bMakeVisible                   = false;
      createcontext->m_puiParent                      = pwndParent;
      if(var.get_type() == var::type_propset && var.has_property("hold") && !(bool)var["hold"])
      {
         createcontext->m_bHold                       = false;
      }

      pdoc = (m_ptemplateForm->open_document_file(createcontext));
      sp(::user::form) pview = pdoc->get_view(0);
      pdoc->form_document_set_view(pview);
      pdoc->form_document_set_callback(pcallback);
      return pdoc;
   }


   sp(::aura::document) userex::create_child_form(sp(::user::form) pview,::user::form_callback * pcallback,sp(::user::interaction) pwndParent,var var)
   {
      sp(::aura::document) pdoc;
      if(m_ptemplateChildForm == NULL)
         return NULL;
      sp(::create) createcontext(allocer());
      createcontext->m_bMakeVisible                   = false;
      createcontext->m_puiParent                      = pwndParent;
      createcontext->m_puiAlloc                       = pview;

      if(var.get_type() == var::type_propset && var.has_property("hold") && !(bool)var["hold"])
      {
         createcontext->m_bHold                       = false;
      }

      pdoc = (m_ptemplateChildForm->open_document_file(createcontext));
      pview = pdoc->get_typed_view < ::user::form >();
      pdoc->form_document_set_view(pview);
      pdoc->form_document_set_callback(pcallback);
      return pdoc;
   }

   sp(::aura::document) userex::create_child_form(::user::form_callback * pcallback,sp(::user::interaction) pwndParent,var var)
   {
      //if(pwndParent != NULL && pwndParent->m_pauraapp != get_app())
      //{
        // return Plat(pwndParent->m_pauraapp).userex()->create_child_form(pcallback,pwndParent,var);
      //}
      if(m_ptemplateChildForm == NULL)
         return NULL;
      sp(::aura::document) pdoc;
      sp(::create) createcontext(pwndParent->allocer());
      createcontext->m_bMakeVisible                   = false;
      createcontext->m_puiParent                      = pwndParent;
      pdoc = (m_ptemplateChildForm->open_document_file(createcontext));
      sp(::user::form) pview = pdoc->get_view(0);
      pdoc->form_document_set_view(pview);
      pdoc->form_document_set_callback(pcallback);
      return pdoc;
   }


   ::aura::document * userex::hold(sp(::user::interaction) pui)
   {

      sp(::create) createcontext(pui->allocer());

      createcontext->m_bMakeVisible    = false;
      createcontext->m_bHold           = false;

      sp(::aura::document) pdoc = m_ptemplatePlaceHolder->open_document_file(createcontext);

      sp(::user::place_holder) pholder = pdoc->get_typed_view < ::user::place_holder  >();

      pholder->hold(pui);

      return pdoc;

   }


   bool userex::_001OnCmdMsg(::aura::cmd_msg * pcmdmsg)
   {

      UNREFERENCED_PARAMETER(pcmdmsg);

      return false;

   }


} //namespace userex



