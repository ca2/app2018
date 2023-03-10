#include "framework.h"


namespace userstack
{

   pane_view::pane_view(::aura::application * papp) :
      ::object(papp),
      ::user::tab_view(papp),
      ::userex::pane_tab_view(papp),
      m_dibBk(allocer()),
      place_holder_container(papp)
   {

      m_iNewArea        = 0;
      m_iArea           = -1;
      m_pviewdataOld    = NULL;

      m_pviewdata       = NULL;
      m_pviewdataOld    = NULL;

      m_iDisplay        = -1;
      m_ppropform       = NULL;
      //m_dataid          = "ca2::bergedge::pane_view";

      get_data()->m_matchanyRestore.add(new ::core::match::prefix("app:"));

   }

   pane_view::~pane_view()
   {
   }


   void pane_view::assert_valid() const
   {
      ::user::impact::assert_valid();
   }

   void pane_view::dump(dump_context & dumpcontext) const
   {
      ::user::impact::dump(dumpcontext);
   }


   void pane_view::_001OnCreate(::message::message * pobj)
   {

      if(pobj->previous())
         return;

      sp(frame) pframe =  (GetParentFrame());
      pframe->m_ppaneview = this;


      Application.::userstack::application::m_ppaneview = this;



      add_tab("menu", ::userstack::pane_view_context_menu);

      for(int32_t i = 0; i < Application.handler()->m_varTopicQuery["app"].stra().get_count(); i++)
      {

         string strId = Application.handler()->m_varTopicQuery["app"].stra()[i];

         if(i == 0 && strId == "app/sphere/userstack")
            continue;

         add_tab(strId, "app:" + strId);

         set_cur_tab_by_id("app:" + Application.handler()->m_varTopicQuery["app"].stra()[i]);

      }

      for (int32_t i = 0; i < Application.handler()->m_varTopicQuery["app/sphere/userstack"]["tab"].stra().get_count(); i++)
      {

         set_cur_tab_by_id("app:" + Application.handler()->m_varTopicQuery["app/sphere/userstack"]["tab"].stra()[i]);

      }

   }

   void pane_view::on_update(::user::impact * pSender, LPARAM lHint, ::object* pHint)
   {
      ::user::tab_view::on_update(pSender, lHint, pHint);
      if(lHint == 543218)
      {
//         set_cur_tab_by_id(::bergedge::pane_view_winactionarea);
      }
      if(pHint != NULL)
      {
         if(base_class <pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint *) pHint;
            if(puh->is_type_of(pane_view_update_hint::type_get_view))
            {
            }
            else if(puh->is_type_of(pane_view_update_hint::type_set_view))
            {
               set_cur_tab_by_id(puh->m_eview);
            }
         }
      }
      if(pHint != NULL)
      {
         if(base_class <pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint * ) pHint;
            if(puh->is_type_of(pane_view_update_hint::TypeOnShowKaraoke))
            {
               ( (GetTopLevelFrame()))->SetActiveView(this);
            }
            else if(puh->is_type_of(pane_view_update_hint::TypeOnShowView))
            {
            }
         }
      }
   }

   void pane_view::on_show_view()
   {
      ::userex::pane_tab_view::on_show_view();
//      sp(frame) pframe =  (GetParentFrame());
      string strId = get_view_id();
      if(::str::begins_eat(strId, "app:"))
      {

         sp(::aura::application) pappTab;

         if(Session.appptra().lookup("application:" + strId, pappTab))
         {
            Session.m_pappCurrent = pappTab;
            //Session.m_pappCurrent = pappTab;
         }
         /*            sp(::simple_frame_window) pframeApp = dynamic_cast < sp(::simple_frame_window) > (m_pviewdata->m_pwnd);
                  if(pframeApp != NULL)
                  {
                     pframeApp->WfiFullScreen();
                  }*/
      }
      //   else if(get_view_id() == ::bergedge::pane_view_context_menu)
      //   {
      //      /*bergedge::menu_view * pview = dynamic_cast < bergedge::menu_view *  > (get_view());
      //      sp(::filemanager::manager) pdoc = (pview->get_document());
      //      pdoc->FileManagerBrowse(System.dir().appdata() / "bergedge\\menu"), ::action::source::system_default());*/
      //   }
      //   else if(get_view_id() == ::bergedge::pane_view_configuration)
      //   {
      ///*      sp(::user::interaction) pui = m_pformOptions->ve_display_bandwidth");
      //      elemental * pcheck = dynamic_cast < elemental * > (puie);
      //      if(Session.savings().save().is_signalized(::ca2::save_display_bandwidth))
      //      {
      //         pcheck->_001SetCheck(check::checked, false);
      //      }
      //      else
      //      {
      //         pcheck->_001SetCheck(check::unchecked, false);
      //      }*/
      //   }
      //   else if(get_view_id() == ::bergedge::pane_view_winactionarea)
      //   {
      //      string strDirName;
      //      strDirName.Format("desktop-%d", m_iDisplay);
      //      string strDir = Application.dir().userfolder(strDirName);
      //      if(m_iDisplay <= 0)
      //      {
      //         check_desktop_dir(strDir);
      //      }
      //      sp(::filemanager::manager) pdoc =  (m_pviewdata->m_pdoc);
      //      pdoc->FileManagerBrowse(strDir, ::action::source::system_default());
      //   }
      else
      {
//         pframe->m_bAutoHideOnOutClick = true;
      }
      /*if(m_pviewdataOld != NULL)
      {
         if(m_pviewdataOld->m_pwnd != NULL)
         {
            m_pviewdataOld->m_pwnd->ShowWindow(SW_HIDE);
         }
      }*/
   }

   bool pane_view::pre_create_window(::user::create_struct& cs)
   {
      cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

      return ::user::impact::pre_create_window(cs);
   }


   void pane_view::on_create_view(::user::view_creator_data * pcreatordata)
   {

      class sp(application) papp =  (get_app());

      string strId = pcreatordata->m_id;

      if(::str::begins_eat(strId, "app:"))
      {

         sp(::aura::application) pappTab;

         if(!Session.appptra().lookup("application:" + strId, pappTab))
         {

            application_bias * pbiasCreate = new application_bias;
            pbiasCreate->m_puiParent = pcreatordata->m_pholder;

            sp(::create) createcontext(allocer());
            createcontext->m_spApplicationBias = pbiasCreate;
            createcontext->m_spCommandLine->_001ParseCommandFork(strId);


            string str;

            if(papp->handler()->m_varTopicQuery.has_property(strId))
            {

               createcontext->m_spCommandLine->m_varQuery.propset().merge(papp->handler()->m_varTopicQuery[(const char *) strId].propset());

               if(papp->handler()->m_varTopicQuery[(const char *) strId].has_property("file"))
               {

                  createcontext->m_spCommandLine->m_varFile = papp->handler()->m_varTopicQuery[(const char *) strId]["file"];

               }

            }

            str = ::str::from((int_ptr) createcontext->m_spApplicationBias->m_puiParent);

            createcontext->m_spCommandLine->m_eventReady.ResetEvent();

            Session.on_request(createcontext);

            createcontext->m_spCommandLine->m_eventReady.wait();

         }

         sp(::aura::application) pappCurrent = Session.m_pappCurrent;

         Application.appptra_add(pappCurrent);

         string strTypeId = typeid(*pappCurrent.m_p).name();

         string strIcon = Sess(pappCurrent).dir().matter("mainframe/icon48.png");

         ::user::tab_pane * ppane = (::user::tab_pane *) get_pane_by_id(pcreatordata->m_id);

         pappCurrent = Session.m_pappCurrent;

         if(Sess(pappCurrent).file().exists(strIcon))
         {

            ppane->m_dib.alloc(allocer());

            ppane->m_dib.load_from_file(strIcon);

         }
         else
         {

            ppane->set_title(pcreatordata->m_id);

         }

         on_layout();

      }

      ::userex::pane_tab_view::on_create_view(pcreatordata);

   }



   void pane_view::_001OnMenuMessage(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      set_cur_tab_by_id(m_pviewdataOld->m_id);
   }

   void pane_view::install_message_routing(::message::sender * pinterface)
   {
      ::userex::pane_tab_view::install_message_routing(pinterface);
      IGUI_MSG_LINK(WM_CREATE, pinterface, this, &pane_view::_001OnCreate);
      IGUI_MSG_LINK(WM_USER + 1122, this, this, &pane_view::_001OnMenuMessage);
      IGUI_MSG_LINK(WM_RBUTTONUP, pinterface, this, &pane_view::_001OnRButtonUp);
      connect_command("properties", &pane_view::_001OnProperties);
   }

   void pane_view::rotate()
   {
   }

   /*void pane_view::OnFileManagerOpenFile(
         ::filemanager::data * pdata,
         ::fs::item_array & itema)
   {
      if(itema.get_size() > 0)
      {
         int32_t i = (int32_t) ::ShellExecuteW(
            GetTopLevel()->get_handle(),
            NULL,
            L"\"" + ::str::international::utf8_to_unicode(itema[0].m_strPath) + L"\"",
            NULL,
            L"\"" + ::str::international::utf8_to_unicode(System.dir().name(itema[0].m_strPath)) + L"\"",
            SW_SHOWNORMAL);
         string str;
         str.Format("%d", i);
         //System.simple_message_box(str);
         if(i == ERROR_FILE_NOT_FOUND)
         {
         }
      }
      //GetParentFrame()->ShowWindow(SW_HIDE);
   }*/

   void pane_view::check_menu_dir(const ::file::path & psz)
   {

      ::file::listing straPath(get_app());

      straPath.rls(System.dir().commonprograms());

      for(int32_t i = 0; i < straPath.get_size(); i++)
      {

         ::file::path str = psz/straPath[i].relative();

         Application.dir().mk(str.folder());

         Application.file().copy(str, straPath[i], true);

      }

      straPath.clear_results();

      straPath.rls(System.dir().userprograms(NULL));

      for(int32_t i = 0; i < straPath.get_size(); i++)

      {
         ::file::path str = psz / straPath[i].relative();

         Application.dir().mk(str.folder());

         Application.file().copy(str, straPath[i], true);

      }

   }


   void pane_view::check_3click_dir(const ::file::path & psz)
   {

      if(Application.dir().is(psz))
      {

         return;

      }

      Application.dir().mk(psz);

      string strDir(psz);

      POSITION pos = System.m_mapAppLibrary.get_start_position();

      string strApp;

      string strLibrary;

      while(pos != NULL)
      {

         System.m_mapAppLibrary.get_next_assoc(pos, strApp, strLibrary);

         if(::str::begins_eat(strApp, "application:"))
         {

            Application.file().put_contents(strDir / strApp + ".ca2", "ca2prompt\r\n"+ strApp);

         }

      }

      //Application.file().put_contents(System.dir().path(strDir, "veriwell Musical Player.ca2"), "ca2prompt\r\nmplite");
      /*      stringa straPath;
            stringa straRelative;
            straPath.remove_all();
            System.dir().rls(Application.dir().userquicklaunch(), &straPath, NULL, &straRelative);
            for(int32_t i = 0; i < straPath.get_size(); i++)
            {
               string str = System.dir().path(psz, straRelative[i]);
               Application.dir().mk(System.dir().name(str));
               ::CopyFile(straPath[i], str, TRUE);
            }*/
   }

   void pane_view::check_desktop_dir(const ::file::path & psz)
   {
#ifdef WINDOWSEX
      ::file::listing listing(get_app());
      char buf[4096];
      memset(buf, 0, sizeof(buf));
      SHGetSpecialFolderPath(
      NULL,
      buf,
      CSIDL_COMMON_DESKTOPDIRECTORY,
      FALSE);
      if(strlen(buf) > 0)
      {
         if(strlen(buf) > 0)
         {
            listing.rls(buf);
            for(int32_t i = 0; i < listing.get_size(); i++)
            {

               ::file::path str = psz / listing[i].relative();

               Application.dir().mk(str.folder());

               ::CopyFile(listing[i], str, TRUE);

            }
         }
      }
      memset(buf, 0, sizeof(buf));
      SHGetSpecialFolderPath(
      NULL,
      buf,
      CSIDL_DESKTOPDIRECTORY,
      FALSE);
      if(strlen(buf) > 0)
      {
         if(strlen(buf) > 0)
         {
            listing.clear_results();
            listing.rls(buf);
            for(int32_t i = 0; i < listing.get_size(); i++)
            {
               ::file::path str = psz / listing[i].relative();
               Application.dir().mk(listing[i].folder());
               ::CopyFile(listing[i], str, TRUE);
            }
         }
      }
#else
      _throw(todo(get_app()));
#endif
   }


   void pane_view::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      if(m_iNewArea != m_iArea)
      {
         m_iArea = m_iNewArea;
         //string strWallpaper;
         /*
               switch(m_iArea)
               {
               case 0:
                  strWallpaper = System.dir().standard_square_matter("windeskframe1_23.bmp");
                  break;
               case 1:
                  strWallpaper = System.dir().standard_square_matter("windeskframe1_24.bmp");
                  break;
               case 2:
                  strWallpaper = System.dir().standard_square_matter("windeskframe1_25.bmp");
                  break;
               case 3:on_create_on_crea
                  strWallpaper = System.dir().standard_square_matter("windeskframe1_26.bmp");
                  break;
               default:
                  strWallpaper = System.dir().standard_square_matter("windeskframe1_23.bmp");
                  break;
               }
               FIBITMAP * pfi;

               ::draw2d::memory_graphics pgraphics(this);

               pfi = Application.imaging().LoadImageFile(strWallpaper);

               m_dibBk.From((HDC)pgraphics->get_os_data(), pfi, true);

               */
         //set_need_redraw();
      }

      //m_dibBk.to(pgraphics, 0, 0, rectClient.right, rectClient.bottom);

      ::userex::pane_tab_view::_001OnDraw(pgraphics);

   }



   void pane_view::set_display(int32_t iDisplay)
   {
      m_iDisplay = iDisplay;
   }

   void pane_view::_001OnRButtonUp(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
//      SCAST_PTR(::message::mouse, pmouse, pobj);
      /*if(get_view_id() == ::bergedge::pane_view_winactionarea)
      {
         ::user::menu menu(get_app());
         menu.LoadXmlMenu("bergedge\\popup_winactionarea.xml");
         ::user::menu menuPopup(get_app(), menu.GetSubMenu(0));
         GetParentFrame()->SetActiveView(this);
         menuPopup.track_popup_menu(0, pmouse->m_pt.x, pmouse->m_pt.y, GetParentFrame());
      }*/
   }

   void pane_view::OnFileManagerOpenContextMenu(::filemanager::data * pdata)
   {

      UNREFERENCED_PARAMETER(pdata);

      //if(get_view_id() == ::bergedge::pane_view_winactionarea)
      //{

      //   track_popup_xml_matter_menu("bergedge/popup_winactionarea.xml", 0);

      //}





   }

   /*void pane_view::OnFileManagerOpenContextMenuFolder(::filemanager::data * pdata, ::fs::item & item)
   {
   }
   */
   void pane_view::OnFileManagerOpenContextMenuFile(::filemanager::data * pdata, const ::fs::item_array & itema)
   {
      UNREFERENCED_PARAMETER(pdata);
      m_itema = itema;
      //set_cur_tab_by_id(::bergedge::pane_view_properties);
   }



   void pane_view::_001OnProperties(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
//      if(get_view_id() == ::bergedge::pane_view_winactionarea)
//      {
//
//#ifndef METROWIN
//         ::aura::shell_launcher launcher(NULL, NULL, "control.exe", "desk.cpl", NULL, SW_SHOWNORMAL);
//
//         launcher.execute();
//#else
//
//         _throw(todo(get_app()));
//
//#endif
//
//      }
   }


   void pane_view::_001InitializeFormPreData(::user::form * pform)
   {

      class user::control_descriptor control;

      control.m_bTransparent = true;
      control.set_control_type(user::control_type_check_box);
      control.m_id = "save_display_bandwidth";
      control.set_ddx_dbflags("ca2.savings", ::aura::resource_display_bandwidth);
      pform->_001AddControl(control);

      control.m_bTransparent = true;
      control.set_control_type(user::control_type_check_box);
      control.m_id = "save_processing";
      control.set_ddx_dbflags("ca2.savings", ::aura::resource_processing);
      pform->_001AddControl(control);

      control.m_bTransparent = true;
      control.set_control_type(user::control_type_check_box);
      control.m_id = "save_memory";
      control.set_ddx_dbflags("ca2.savings", ::aura::resource_memory);
      pform->_001AddControl(control);

      control.m_bTransparent = true;
      control.set_control_type(user::control_type_check_box);
      control.m_id = "enable_wallpaper";
      control.set_ddx_dbflags(
      "ca2.bergedge",
      0);
      //   control.add_function(user::control_function_static);
      pform->_001AddControl(control);

   }


   void pane_view::on_control_event(::user::control_event * pevent)
   {

      UNREFERENCED_PARAMETER(pevent);

   }


} // namespace bergedge
