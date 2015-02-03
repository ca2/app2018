//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


namespace user
{


   user::user(::aura::application * papp):
      element(papp),
      ::aura::departament(papp)
   {


      m_pkeyboardfocus  = NULL;
      //      m_pshellimageset  = NULL;
      m_pkeyboard       = NULL;
      //    m_pufeschema      = NULL;
      //  m_pufe            = NULL;


      //::core::user * papp = dynamic_cast <::core::user *>(::get_thread()->m_pAppThread);
      //::core::connect(papp->m_pcoreapp->m_signalAppLanguageChange, this, &user::VmsGuiiOnAppLanguage);

      //m_pwindowmap = NULL;

   }

   user::~user()
   {
   }


   bool user::initialize1()
   {

      if(m_pauraapp->is_session())
      {
         m_pwindowmap = new class ::user::window_map(get_app());
      }
      else
      {
         m_pwindowmap = Session.user()->m_pwindowmap;
      }







      if(m_pauraapp->is_system())
      {
         System.factory().creatable_small < keyboard_layout >();
      }

      //      m_pshellimageset = new filemanager::_shell::ImageSet(m_pauraapp);

      if(!::aura::departament::initialize1())
         return false;

      return true;

   }

   bool user::initialize()
   {




      if(!::aura::departament::initialize())
         return false;


      TRACE("::user::application::initialize");

      xml::document docUser(get_app());
      string strUser = Application.file_as_string(Application.dir_userappdata("langstyle_settings.xml"));
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
         Session.set_locale(strLangUser,::action::source_database);
      if(strStyleUser.has_char())
         Session.set_schema(strStyleUser,::action::source_database);

      string strLicense = Application.get_license_id();


      var & varTopicQuey = System.directrix()->m_varTopicQuery;

      bool bHasInstall = varTopicQuey.has_property("install");

      bool bHasUninstall = varTopicQuey.has_property("uninstall");

      printf("user::initialize bHasInstall %c", bHasInstall);

      printf("user::initialize bHasUninstall %c", bHasUninstall);

      if(!::aura::departament::initialize())
         return false;

      return true;

   }




   bool user::initialize2()
   {


      if(!::aura::departament::initialize2())
         return false;

      
      return true;


   }


   bool user::finalize()
   {
      try
      {
         ::aura::departament::finalize();
      }
      catch(...)
      {
      }

      /* xxx      try
            {
            if(m_pufeschema != NULL)
            {
            delete m_pufeschema;
            m_pufeschema = NULL;
            }
            }
            catch(...)
            {
            } */

      /* xxx try
      {
      if(m_pufe != NULL)
      {
      delete m_pufe;
      m_pufe = NULL;
      }
      }
      catch(...)
      {
      } */

      /* xxx      try
            {
            if(!BaseMenuCentralContainer::finalize_central_container())
            {
            TRACE("Errors occurred while finalizing BaseMenuCentralContainer");
            }
            }
            catch(...)
            {
            } */

      return true;

   }


   /* xxx   filemanager::_shell::ImageSet & user::shellimageset()
      {
      return *m_pshellimageset;
      }
      */

   /* xxx

   string user::message_box(const char * pszMatter, property_set & propertyset)
   {
   class ::userex::message_box box(get_app());

   box.show(pszMatter, &propertyset);

   return box.m_strResponse;

   }

   int32_t user::simple_message_box(sp(::user::interaction) pwndOwner, const char * pszMessage, UINT fuStyle)
   {

   //if(m_pauraapp->m_pcoreapp->m_psession != NULL)
   //{
   // return m_pauraapp->m_pcoreapp->m_psession->m_pcoreplatform->m_pcoreplatform->simple_message_box(pwndOwner, pszMessage, fuStyle);
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
   return simple_message_box((oswindow) (pwndOwner.is_null() ? NULL : pwndOwner->get_wnd()->get_os_data()), strMessage, Application.m_strAppName, fuStyle);
   }
   }
   catch(...)
   {
   string strMessage = pszMessage;
   strMessage.replace("<br>", "\r\n");
   return simple_message_box(pwndOwner == NULL ? NULL : pwndOwner->get_handle(), strMessage, Application.m_strAppName, fuStyle);
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
   pchange->data_get(Session.savings().m_eresourceflagsShouldSave);
   }
   }
   }
   */

   ::user::elemental * user::get_keyboard_focus()
   {
      if(Application.is_session())
      {
         ::user::interaction * puieFocus = Application.get_focus_guie();
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
      else if(Application.m_pbasesession != NULL)
      {
         return Sess(get_app()).user()->get_keyboard_focus();
      }
      else if(Application.m_pbasesystem != NULL)
      {
         return Sess(get_app()).user()->get_keyboard_focus();
      }
      else
      {
         return NULL;
      }
   }

   void user::set_keyboard_focus(sp(::user::elemental) pkeyboardfocus)
   {
      if(pkeyboardfocus == NULL || pkeyboardfocus->keyboard_focus_OnSetFocus())
      {
         
         if(m_pkeyboardfocus != NULL && m_pkeyboardfocus != pkeyboardfocus)
         {

            ::user::elemental * pkeyboardfocusOld = m_pkeyboardfocus;

            try
            {
               
               m_pkeyboardfocus = NULL;

               pkeyboardfocusOld->keyboard_focus_OnKillFocus();

            }
            catch(...)
            {

            }

         }


         m_pkeyboardfocus = pkeyboardfocus;

         if(m_pkeyboardfocus != NULL)
         {

            if(m_pkeyboardfocus->get_wnd() != NULL)
            {

               m_pkeyboardfocus->get_wnd()->on_keyboard_focus(m_pkeyboardfocus);

            }

         }

      }

   }

   ::user::elemental * user::get_mouse_focus_LButtonDown()
   {
      return m_pmousefocusLButtonDown;
   }

   void user::set_mouse_focus_LButtonDown(::user::elemental * pmousefocus)
   {
      m_pmousefocusLButtonDown = pmousefocus;
   }

   ::user::elemental * user::get_mouse_focus_RButtonDown()
   {
      return m_pmousefocusRButtonDown;
   }

   void user::set_mouse_focus_RButtonDown(::user::elemental * pmousefocus)
   {
      m_pmousefocusRButtonDown = pmousefocus;
   }


   bool user::set_keyboard_layout(const char * pszPath,::action::context actioncontext)
   {

      if(get_app()->m_pbasesession == NULL)
         return false;

      if(pszPath == NULL)
      {

         if(&keyboard().layout() != NULL)
         {

            //            if(Session.fontopus()->m_puser != NULL
            //             && Session.fontopus()->m_puser->m_strFontopusServerSessId.has_char())
            //        {

            // xxx data_set("keyboard_layout", keyboard().layout().m_strPath);

            //      }

            return true;
         }

         if(keyboard().get_current_system_layout().is_empty())
            return false;

         if(!set_keyboard_layout(keyboard().get_current_system_layout(),::action::source_database))
            return false;

         //         if(Session.fontopus()->m_puser != NULL
         //          && Session.fontopus()->m_puser->m_strFontopusServerSessId.has_char())
         //     {

         // xxx            data_set("keyboard_layout", keyboard().layout().m_strPath);

         //   }

         return true;
      }

      if(!Session.user()->keyboard().load_layout(pszPath,actioncontext))
         return false;

      // xxx Application.simpledb().on_set_keyboard_layout(pszPath, actioncontext);

      return true;
   }


   ::user::keyboard & user::keyboard()
   {

      if(m_pkeyboard == NULL)
      {
         
         m_pkeyboard = new ::user::keyboard(m_pauraapp);

         if(m_pkeyboard == NULL)
            throw simple_exception(get_app(), "Could not create keyboard");

         if(!m_pkeyboard->initialize())
            throw simple_exception(get_app(),"Could not initialize keyboard");

         Application.on_create_keyboard();

      }

      return *m_pkeyboard;

   }




   class window_map & user::window_map()
   {

      return *m_pwindowmap;

   }


   /* xxx    sp(type) user::controltype_to_typeinfo(::user::e_control_type e_type)
      {

      return sp(type)();

      } */


   void user::SendMessageToWindows(UINT message,WPARAM wparam,LPARAM lparam)
   {

      synch_lock sl(&Application.m_mutexFrame);

      ::user::interaction * pui = NULL;

      while(Application.get_frame(pui))
      {

         if(pui != NULL && pui->IsWindow())
         {

            pui->send_message(message,wparam,lparam);

            pui->SendMessageToDescendants(message,wparam,lparam);

         }

      }

   }


#ifdef LINUX

   sp(::message::base) user::get_base(XEvent * pevent,::user::interaction * pwnd)
   {

      throw todo(get_app());

      return NULL;

   }

#endif


} //namespace user



