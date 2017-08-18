//#include "framework.h"


namespace console
{


   prompt_frame::prompt_frame(::aura::application * papp) :
      ::object(papp),
      simple_frame_window(papp)
   {

      m_pimagelist = NULL;
      
      
      m_bTimerHide = false;

      WfiEnableFullScreen();

      m_bMoveDrag = true;

   }


   prompt_frame::~prompt_frame()
   {

   }


   #ifdef DEBUG
   void prompt_frame::assert_valid() const
   {
	   simple_frame_window::assert_valid();
   }

   void prompt_frame::dump(dump_context & dumpcontext) const
   {
	   simple_frame_window::dump(dumpcontext);
   }


   #endif //DEBUG


   void prompt_frame::_001OnTimer(::timer * ptimer)
   {
      
      simple_frame_window::_001OnTimer(ptimer);;
      
//      UINT nIDEvent = ptimer->m_nIDEvent;
//      static float theta;
//      if(nIDEvent == 3)
//      {
//      }
//      else if(nIDEvent == 1000)
//      {
//         KillTimer(nIDEvent);
//         m_bTimerOn = false;
//
//#ifdef WINDOWSEX
//
//         bool bLControlKeyDown = (GetAsyncKeyState (VK_LCONTROL) >> ((sizeof(SHORT) * 8) - 1)) != 0;
//         bool bLAltKeyDown = (GetAsyncKeyState (VK_LMENU) >> ((sizeof(SHORT) * 8) - 1)) != 0;
//         if(bLControlKeyDown && !bLAltKeyDown)
//         {
//            OnHoverAction(true);
//         }
//         else
//         {
//            ShowWindow(SW_HIDE);
//         }
//
//#else
//
//         throw todo(get_app());
//
//#endif
//
//      }
//      else if(nIDEvent == 1001)
//      {
//         KillTimer(nIDEvent);
//         ShowWindow(SW_HIDE);
//      }
//      else if(nIDEvent == 8913)
//      {
//      }
//      // every 100 ms approximately
//      else if(nIDEvent == 4033)
//      {
//   /*      rect rectWindow;
//         GetWindowRect(rectWindow);
//         point pt;
//         Session.get_cursor_pos(&pt);
//         if(!rectWindow.contains(pt) && !m_bTimerHide)
//         {
//            m_bTimerHide = true;
//            SetTimer(1001, 800, NULL);
//         }
//         else
//         {
//            m_bTimerHide = false;
//            KillTimer(1001);
//         }*/
//         /*point pt;
//         if(m_bHoverMouse && get_tick_count() > m_dwLastHover + 300)
//         {
//            OnHoverAction();
//         }
//         if(Session.get_cursor_pos(&pt))
//         {
//            if(!m_bHoverMouse && pt.x == 0 && pt.y == 0)
//            {
//               m_dwLastHover = ::get_tick_count();
//               m_bHoverMouse = true;
//
//            }
//            else if(m_bHoverMouse && (pt.x > 10 || pt.y > 0))
//            {
//               m_bHoverMouse = false;
//            }
//
//         }*/
//   // OpenGL animation code goes here
//
//			   //glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
//			   //glClear( GL_COLOR_BUFFER_BIT );
//
//			   /*glPushMatrix();
//			   glRotatef( theta, 0.0f, 1.0f, 1.0f );
//			   glBegin( GL_TRIANGLES );
//			   glColor3f( 1.0f, 0.0f, 0.0f ); glVertgenf( 0.0f, 1.0f );
//			   glColor3f( 0.0f, 1.0f, 0.0f ); glVertgenf( 0.87f, -0.5f );
//			   glColor3f( 0.0f, 0.0f, 1.0f ); glVertgenf( -0.87f, -0.5f );
//			   glEnd();
//			   glPopMatrix();*/
//
//			   //SwapBuffers( m_hdcOpenGL );
//
//			   theta += 2.0f;
//
//      }
//   //	simple_frame_window::OnTimer(nIDEvent);
   }



   void prompt_frame::ShowControlBars(bool bShow)
   {
      UINT nShow;
      if(bShow)
      {
         nShow = SW_SHOW;
      }
      else
      {
         nShow = SW_HIDE;
      }

   /*   m_toolbar.ShowWindow(nShow);
      m_toolbarView.ShowWindow(nShow);
   //   m_statusbar.ShowWindow(nShow);
      m_menubar.ShowWindow(nShow);
      m_dialogbar.ShowWindow(nShow);*/

   }







   void prompt_frame::OnHoverAction(bool bShowWinActionArea)
   {
      if(!IsWindowVisible())
      {
         if(bShowWinActionArea)
         {
            if(GetActiveDocument() != NULL)
            {
               GetActiveDocument()->update_all_views(NULL, 543218);
            }
         }
         ToFront();
      }
   }


   void prompt_frame::_001OnClose(signal_details * pobj)
   {
      
      pobj->m_bRet = true;
      
      ShowWindow(SW_HIDE);
      
      sp(prompt_impact) pview = m_pviewMain;

      if(pview.is_set())
      {

         pview->clear();

      }

   }


   void prompt_frame::install_message_handling(::message::dispatch * pinterface)
   {
      simple_frame_window::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &prompt_frame::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_CLOSE, pinterface, this, &prompt_frame::_001OnClose);
//      //IGUI_WIN_MSG_LINK(WM_TIMER, pinterface, this, &prompt_frame::_001OnTimer);
      IGUI_WIN_MSG_LINK(WM_MOVE, pinterface, this, &prompt_frame::_001OnMove);
      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &prompt_frame::_001OnShowWindow);
      IGUI_WIN_MSG_LINK(WM_APP + 2000  , pinterface, this, &prompt_frame::_001OnApp2000);
   }

   void prompt_frame::_001OnCreate(signal_details * pobj)
   {

      SCAST_PTR(::message::create, pcreate, pobj);

      pobj->previous();

      if(pobj->m_bRet)
         return;

      if(!data_get(".local://DockPosition", (int32_t &) m_eposition))
      {

         m_eposition = position_left;

      }


      m_bTimerOn = false;

      //SetTimer(8913, 5000, 0);
      //SetTimer(4033, 100, 0);
      //ModifyStyleEx(WS_EX_OVERLAPPEDWINDOW, 0);



      m_pimagelist = new image_list(get_app());
      m_pimagelist->create(16, 16, 0, 10, 10);
      m_pimagelist->add_matter_icon("system/language_change.ico");



	   /*if (!LoadToolBar(0,"command\\toolbar.xml") )
	   {
		   pcreate->failed("Failed to create toolbar\n");
         pcreate->set_lresult(-1);
         pcreate->m_bRet = true;
         return;
	   }*/

      //if(!m_spqueue->create_message_queue("::ca2::fontopus::message_wnd::command", this))
      //{
      //   pcreate->set_lresult(-1);
      //   pcreate->m_bRet = true;
      //   return;
      //}

   }

   void prompt_frame::_001OnMove(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      /*if(m_workset.GetMovingManager()->IsMoving())
      {
         oswindow oswindowDesktop = ::GetDesktopWindow();
         rect rectDesktop;
         ::GetWindowRect(oswindowDesktop, rectDesktop);
         rect rectWindow;
         GetWindowRect(rectWindow);
         double a = (double) rectDesktop.height() / (double) rectDesktop.width();
         if(rectWindow.left < (rectDesktop.width() / 2))
         {
            // to the left
            if(rectWindow.top < rectWindow.left * a)
            {
               // top
               m_eposition = position_top;
            }
            else if(rectWindow.top > (rectDesktop.height() - rectWindow.left * a))
            {
               // bottom
               m_eposition = position_bottom;
            }
            else
            {
               // left
               m_eposition = position_left;
            }
         }
         else
         {
            // to the right
            // to the left
            if(rectWindow.top > rectWindow.left * a)
            {
               // bottom
               m_eposition = position_bottom;
            }
            else if(rectWindow.top < (rectDesktop.height() - rectWindow.left * a))
            {
               // top
               m_eposition = position_top;
            }
            else
            {
               // right
               m_eposition = position_right;
            }
         }
         data_set("DockPosition", (int32_t) m_eposition);
      }*/
   }

   void prompt_frame::_001OnShowWindow(signal_details * pobj)
   {
      
      //SCAST_PTR(::message::show_window, pshowwindow, pobj);

      //if(!pshowwindow->m_bShow)
      //{
      //   if(GetActiveDocument() != NULL)
      //      GetActiveDocument()->update_all_views(NULL, 5432108);
      //}

   }


   void prompt_frame::ToFront()
   {
      rect rectWindow;
      GetWindowRect(rectWindow);
      ShowWindow(SW_RESTORE);
      bool bChanged = false;
      if(rectWindow.width() < 400)
      {
         rectWindow.right = rectWindow.left + 400;
         bChanged = true;
      }
      if(rectWindow.height() < 200)
      {
         rectWindow.bottom = rectWindow.top + 200;
         bChanged = true;
      }
      SetWindowPos(
         ZORDER_TOPMOST,
         rectWindow.left,
         rectWindow.top,
         rectWindow.width(),
         rectWindow.height(),
         SWP_SHOWWINDOW);
   }

   void prompt_frame::message_queue_message_handler(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      if(pbase->m_uiMessage == (WM_APP + 2000))
      {
         _001OnApp2000(pbase);
         pbase->m_bRet = true;
      }
   }

   void prompt_frame::_001OnApp2000(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);


      if(pbase->m_wparam == 0)
      {
         if(pbase->m_lparam == 1)
         {
            m_bTimerOn = true;
            SetTimer(1000, 23, NULL);
         }
         else if(pbase->m_lparam == 0)
         {
            KillTimer(1000);
            m_bTimerOn = false;
         }

      }
      else if(pbase->m_wparam == 1)
      {
         pbase->set_lresult(2);
      }
      else if(pbase->m_wparam == 2)
      {
         pbase->set_lresult(4);
      }
      else if(pbase->m_wparam == 3)
      {
         if(pbase->m_lparam == 6)
         {
            GetTopLevelFrame()->ShowWindow(SW_HIDE);
            __post_quit_message(36);
         }
      }
      else if(pbase->m_wparam == 4)
      {
         pbase->set_lresult(5);
      }
      else if(pbase->m_wparam == 5)
      {
         pbase->set_lresult(8);
      }
      else if(pbase->m_wparam == 8)
      {
         pbase->set_lresult(11);
      }
      else if(pbase->m_wparam == 11)
      {
         pbase->set_lresult(23);
      }
      else if(pbase->m_wparam == 23)
      {
         pbase->set_lresult(33);
      }
      else if(pbase->m_wparam == 33)
      {
         
         pbase->set_lresult(1);
         
      }
      
      pbase->m_bRet = true;
      
   }


   bool prompt_frame::on_simple_action(id id)
   {
      
      if(id == "app_exit")
      {
         
         simple_frame_window::OnClose();
         
         return true;
         
      }
      
      return simple_frame_window::on_simple_action(id);
      
   }

   
   bool prompt_frame::on_simple_update(command_ui * pcommandui)
   {
      
      if(pcommandui->m_id == "app_exit")
      {
         
         pcommandui->Enable();
         
         return true;
         
      }
      
      return false;
      
   }

   
   sp(::user::wndfrm::frame::frame) prompt_frame::create_frame_schema()
   {
      
      sp(::user::wndfrm::frame::frame) pschema = Application.wndfrm()->get_frame_schema("wndfrm_core","002");

      return pschema;

   }


   bool prompt_frame::show_mini()
   {
      
      ::rect r;

      System.get_monitor_rect(0,&r);

      int iHeight = m_workset.m_pframeschema->calc_caption_height(::user::appearance_normal) + m_workset.m_pframeschema->m_rectMarginNormal.top;

      r.left += 100;

      r.top = r.bottom - 100 - iHeight;

      r.bottom -= 100;

      r.right -= 400;

      m_workset.SetAppearance(::user::appearance_minimal);

      set_appearance(::user::appearance_minimal);

      SetWindowPos(ZORDER_TOP,r,SWP_SHOWWINDOW);

      ShowWindow(SW_SHOW);

      SetActiveWindow();

      m_workset.m_pframeschema->title_bar_layout(true);

      return true;

   }
   
   bool prompt_frame::get_translucency(::user::e_translucency & etranslucency)
   {

      etranslucency = ::user::translucency_present;

      return true;

   }



} // namespace prompt













