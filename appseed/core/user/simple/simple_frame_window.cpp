//#include "framework.h"
#ifdef WINDOWSEX
   //#include <dde.h>
#endif

#define TEST 0

extern CLASS_DECL_CORE thread_int_ptr < DWORD_PTR > t_time1;



manual_reset_event * simple_frame_window::helper_task::g_pevent = NULL;

simple_frame_window::helper_task::helper_task(simple_frame_window * pframe) :
   ::thread(pframe->get_app()),
   m_pframe(pframe) //,
//m_ev(pframe->get_app())
{

   if(g_pevent == NULL)
   {

      g_pevent = new manual_reset_event(pframe->get_app());

   }

   begin();

}


int simple_frame_window::helper_task::run()
{

   while(m_bRun)
   {


      g_pevent->wait(millis(84));

      // the computer maybe blown here, where there is no code (when it is not running)... by falling into a curve in a road from a truck or by the multiverses bramas collapsing into a high energy dot.com... and bubble restarts when the spirtual world decides for restarting the virtual machine - with some pauses - as we does not detect change in time vector, as it is a non-readable, executable/paused/non existent only register in the parent processor... Imagine a overhaul upgrade with much more strings in the chords, why they mantain consitency between virtual machines versions... they like to hinder a lot!! strange, this is a hello 666... // and the time they have to overhaul is infinite, because they can pause our ticker... besides I hope no ones stops their tick counters...
      if(m_pframe->m_bSizeMove) // not here, any error here (or am i wrong, the OpSys may not have started the FULLStack DevOp).... because it is lInUx... its not ADVENTURE_Clean_NoERRORs_may_be_old_tommorrow_just_EX_OS...
      {

         if( // slim and clean people (no drugs) do not like fatty acids double check in the bag( unless they are slim and clean?!?!?!)
            ::get_tick_count() - m_pframe->m_dwLastSizeMove > 284)// the tester (without UPS) can loose a save specially here (where is the error, sixes or 666) // Halloween is coming
            // this a reason for using manual_reset_event for every simple_frame_window, accepts the candy, and the trick? you get both, this the whole trick!!!
         {

            //::MessageBox(NULL, "Going to save window rect", "SaveWindowRect", MB_ICONEXCLAMATION);

            m_pframe->m_bSizeMove = false;// the tester (without UPS) can loose a save here

            try
            {

               TRACE("m_pframe->does_display_match() the tester is about to get close to get sad... (yes, he is mad...)");
               if(m_pframe->does_display_match()) // the tester (without UPS) can loose a save even here
               {

                  TRACE("m_pframe->WindowDataSaveWindowRect() the tester is close to get sad... (yes, he is mad...)");
                  m_pframe->WindowDataSaveWindowRect(); // the tester (without UPS) can loose a save here

               }

            }
            catch(...)
            {

            }

         }

      }
      //Sleep(184); // the tester (without UPS) can loose a save here

   }

   return 0;

}


void simple_frame_window::helper_task::defer_save_window_rect()
{
}


simple_frame_window::simple_frame_window(::aura::application * papp) :
   object(papp),
   m_dibBk(allocer()),
   m_fastblur(allocer())
{

   m_bShowTask = true;

   m_bDefaultNotifyIcon = false;

   m_bTransparentFrame = false;

   m_bblur_Background = false;
   m_bCustomFrameBefore = true;
   m_bWindowFrame = true;
   m_bLayered = true;
   m_pframeschema = NULL;
   m_pdocumenttemplate = NULL;

   //m_phelpertask = new helper_task(this);

}

simple_frame_window::~simple_frame_window()
{
   //m_phelpertask->m_pframe = NULL;
   //m_phelpertask->m_bRun = false;
}

void simple_frame_window::assert_valid() const
{

   ::user::frame_window::assert_valid();

}

void simple_frame_window::dump(dump_context & dumpcontext) const
{

   ::user::frame_window::dump(dumpcontext);

}

// // BEGIN_MESSAGE_MAP(simple_frame_window, ::user::frame_window)
//{{__MSG_MAP(simple_frame_window)
/* xxx   ON_WM_DESTROY()
   ON_WM_SIZE()
   ON_WM_CLOSE()
   ON_WM_NCCALCSIZE()
   ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateControlBarMenu)
   ON_COMMAND_EX(ID_VIEW_STATUS_BAR, OnBarCheck)
   ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateControlBarMenu)
   ON_COMMAND_EX(ID_VIEW_TOOLBAR, OnBarCheck)
   ON_UPDATE_COMMAND_UI(ID_VIEW_REBAR, OnUpdateControlBarMenu)
   ON_COMMAND_EX(ID_VIEW_REBAR, OnBarCheck) */
//}}__MSG_MAP
// // END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// simple_frame_window message handlers

void simple_frame_window::install_message_handling(::message::dispatch * pinterface)
{
   ::user::frame_window::install_message_handling(pinterface);
   IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &simple_frame_window::_001OnCreate);
   IGUI_WIN_MSG_LINK(WM_NCACTIVATE, pinterface, this, &simple_frame_window::_001OnNcActivate);
#ifdef WINDOWSEX
   IGUI_WIN_MSG_LINK(WM_DDE_INITIATE   , pinterface, this, &simple_frame_window::_001OnDdeInitiate);
#endif
   IGUI_WIN_MSG_LINK(WM_DESTROY, pinterface, this, &simple_frame_window::_001OnDestroy);
   IGUI_WIN_MSG_LINK(WM_CLOSE, pinterface, this, &simple_frame_window::_001OnClose);
   IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &simple_frame_window::_001OnSize);
   IGUI_WIN_MSG_LINK(WM_SYSCOMMAND, pinterface, this, &simple_frame_window::_001OnSysCommand);
   IGUI_WIN_MSG_LINK(WM_GETMINMAXINFO, pinterface, this, &simple_frame_window::_001OnGetMinMaxInfo);
   IGUI_WIN_MSG_LINK(WM_USER + 184, pinterface, this, &simple_frame_window::_001OnUser184);
   IGUI_WIN_MSG_LINK(WM_MOUSEMOVE, pinterface, this, &simple_frame_window::_001OnMouseMove);
   IGUI_WIN_MSG_LINK(WM_DISPLAYCHANGE, pinterface, this, &simple_frame_window::_001OnDisplayChange);
   IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &simple_frame_window::_001OnShowWindow);

   connect_update_cmd_ui("view_full_screen", &simple_frame_window::_001OnUpdateViewFullScreen);
   connect_command("view_full_screen", &simple_frame_window::_001OnViewFullScreen);

   if(m_bDefaultNotifyIcon)
   {

      connect_command("app_exit", &simple_frame_window::_001OnAppExit);

   }
   else
   {

      connect_command("app_exit", &simple_frame_window::_001OnClose);

   }

   IGUI_WIN_MSG_LINK(WM_APPEXIT, pinterface, this, &simple_frame_window::_001OnAppExit);

}




bool simple_frame_window::IsFullScreen()
{

   return WfiIsFullScreen();

}


sp(::user::interaction) simple_frame_window::WindowDataGetWnd()
{

   return this;

}


void simple_frame_window::_001OnDestroy(signal_details * pobj)
{

   try
   {

      if (m_pnotifyicon != NULL)
      {

         m_pnotifyicon->Destroy();

      }

   }
   catch(...)
   {

   }


   try
   {
      if (m_pauraapp != NULL && &Application != NULL)
      {
         Application.remove_frame(this);
      }
   }
   catch (...)
   {
   }

   try
   {
      if (&Session != NULL)
      {
         Session.remove_frame(this);
      }
   }
   catch (...)
   {
   }

   try
   {
      if (m_pauraapp != NULL && m_pauraapp->m_pcoresystem != NULL && &System != NULL)
      {
         System.remove_frame(this);
      }
   }
   catch (...)
   {
   }

   pobj->previous();

}


sp(::user::wndfrm::frame::frame) simple_frame_window::create_frame_schema()
{

   sp(::user::wndfrm::frame::frame) pschema = Application.wndfrm().get_frame_schema(NULL, "005");

   pschema->set_style("DarkWarmBlue");

//   pschema->m_typeinfoControlBoxButton = System.type_info < MetaButton >();

   return pschema;

}


void simple_frame_window::_001OnCreate(signal_details * pobj)
{

   SCAST_PTR(::message::create, pcreate, pobj);

   if (pobj->previous())
      return;

   m_puserschema = Session.m_puserschema;

   data_get("transparent_frame",m_bTransparentFrame);


   sp(::user::place_holder) pplaceholder = GetParent();

   if (pplaceholder != NULL)
   {
      sp(::user::place_holder_container) pcontainer = pplaceholder->GetParent();
      if (pcontainer != NULL)
      {
         pcontainer->on_hold(this, pplaceholder);
      }
   }

   if (m_bAutoWindowFrame)
   {
      if(Application.command()->m_varTopicQuery.has_property("client_only"))
      {
         m_bWindowFrame = false;
      }
      else
      {
#if defined(METROWIN) || defined(APPLE_IOS)
         m_bWindowFrame = GetParent() == NULL || GetParent()->m_pimpl != NULL;
#else
         m_bWindowFrame = GetParent() == NULL;
#endif
      }
   }

   if (m_bWindowFrame)
   {
      /*WNDCLASS wndclass;

      char szBuf [64];
      ::GetClassName(get_handle(), szBuf, _countof(szBuf));

      GetClassInfo(System.m_hInstance,
      szBuf,
      &wndclass);*/

      // trans      SetIcon(wndclass.hIcon, false);

      // trans      HICON hicon = GetIcon(false);

      sp(::user::wndfrm::frame::frame) pinteractionframe = NULL;

      try
      {


         pinteractionframe = create_frame_schema();

      }
      catch (not_installed & e)
      {

         System.remove_frame(this);

         throw e;

      }

      //frame::FrameSchema * pschema = dynamic_cast < ::frame::FrameSchema * > (pinteractionframe);

      if (pinteractionframe != NULL && (_ca_is_basis() || Application.command()->m_varTopicQuery["version"] == "basis"))
      {

         //pinteractionframe->set_style("BlueRedPurple");

      }

      /*{
         frame::FrameSchemaHardCoded001 * pschemaSpec = dynamic_cast < frame::FrameSchemaHardCoded001 * > (pschema);
         if(pschemaSpec != NULL && (_ca_is_basis() || Application.command()->m_varTopicQuery["version"] == "basis"))
         {
         pschemaSpec->SetStyle(frame::FrameSchemaHardCoded001::StyleBlueRedPurple);
         }
         }
         {
         frame::FrameSchemaHardCoded002 * pschemaSpec = dynamic_cast < frame::FrameSchemaHardCoded002 * > (pschema);
         if(pschemaSpec != NULL && (_ca_is_basis() || Application.command()->m_varTopicQuery["version"] == "basis"))
         {
         pschemaSpec->SetStyle(frame::FrameSchemaHardCoded002::StyleBlueRedPurple);
         }
         }
         {
         frame::FrameSchemaHardCoded005 * pschemaSpec = dynamic_cast < frame::FrameSchemaHardCoded005 * > (pschema);
         if(pschemaSpec != NULL && (_ca_is_basis() || Application.command()->m_varTopicQuery["version"] == "basis"))
         {
         pschemaSpec->SetStyle(frame::FrameSchemaHardCoded005::StyleBlueRedPurple);
         }
         }
         {
         frame::FrameSchemaHardCoded008 * pschemaSpec = dynamic_cast < frame::FrameSchemaHardCoded008 * > (pschema);
         if(pschemaSpec != NULL && (_ca_is_basis() || Application.command()->m_varTopicQuery["version"] == "basis"))
         {
         pschemaSpec->SetStyle(frame::FrameSchemaHardCoded008::StyleBlueRedPurple);
         }
         }*/

      m_pframeschema = pinteractionframe;
      m_workset.AttachFrameSchema(m_pframeschema);
      if (!m_workset.update(
               this,
               this,
               this,
               this))
      {
         pcreate->m_bRet = false;
         return;
      }

#if defined(LINUX) || defined(APPLEOS)

      m_workset.SetActiveFlag(TRUE);

#endif


   }

   //if (GetParent() == NULL)
   {

      defer_synch_layered();

   }


   create_bars();

#ifdef WINDOWSEX
   if (GetParent() == NULL)
   {
      ::file::path strMatter = get_window_default_matter();
      //http://www.cplusplus.com/forum/general/28470/
      //blackcoder41 (1426)  Sep 12, 2010 at 2:43pm
      //hIconSm = (HICON)LoadImage(NULL, "menu_two.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
      HICON hicon;
      ::file::path path = Application.dir().matter(strMatter / "icon.ico");
      hicon = (HICON)LoadImage(NULL, path, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
      if (hicon != NULL)
      {
         SendMessage(get_handle(), (UINT)WM_SETICON, ICON_SMALL, (LPARAM)hicon);
      }
      path = Application.dir().matter(strMatter / "icon.ico");
      hicon = (HICON)LoadImage(NULL, path, IMAGE_ICON, 48, 48, LR_LOADFROMFILE);
      if (hicon != NULL)
      {
         SendMessage(get_handle(), (UINT)WM_SETICON, ICON_BIG, (LPARAM)hicon);
      }
      if (m_bWindowFrame)
      {
         path = Application.dir().matter(strMatter / "icon.ico");
         hicon = (HICON)LoadImage(NULL, path, IMAGE_ICON, 24, 24, LR_LOADFROMFILE);

         if (hicon != NULL)
         {

            ::visual::icon * picon = new ::visual::icon(hicon);

            if (picon != NULL)
            {

               m_workset.m_pappearance->m_picon = picon;

            }

         }

      }
   }
#endif



   post_message(WM_USER + 184, 2);




   pcreate->m_bRet = false;



}


void simple_frame_window::_001OnShowWindow(signal_details * pobj)
{

   SCAST_PTR(::message::show_window, pshow, pobj);

   if(m_bDefaultNotifyIcon)
   {

      OnUpdateToolWindow(pshow->m_bShow);

   }

}


void simple_frame_window::_001OnDisplayChange(signal_details * pobj)
{

   SCAST_PTR(::message::base, pbase, pobj);

   post_simple_command(simple_command_load_window_rect, (LPARAM)FALSE);

   pobj->m_bRet = true;

   pbase->set_lresult(0);

   Default();

}


void simple_frame_window::_001OnSize(signal_details * pobj)
{

   UNREFERENCED_PARAMETER(pobj);

   //if (does_display_match())
   //{

   //   WindowDataSaveWindowRect();

   //}

   //m_phelpertask->m_bSizeMove = true;

   //m_phelpertask->defer_save_window_rect();



//   m_phelpertask->m_ev.set_event();

   //WindowDataSaveWindowRect();

}



bool simple_frame_window::on_create_client(::user::create_struct * lpcs, sp(::create) pContext)
{
   // trans   HICON hicon = GetIcon(false);
   return ::user::frame_window::on_create_client(lpcs, pContext);
}

bool simple_frame_window::pre_create_window(::user::create_struct& cs)
{

   if (!::user::frame_window::pre_create_window(cs))
      return FALSE;


   //cs.style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;
   cs.style = WS_POPUP;
   cs.style &= ~WS_VISIBLE;

   return TRUE;
}

void simple_frame_window::layout()
{

   //{

   //   DWORD dwTime2 = ::get_tick_count();

   //   //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
   //   TRACE("smpfrmwnd::lyot call timeF= %d ms",dwTime2 - t_time1.operator DWORD_PTR());

   //}

   if (m_bWindowFrame && m_workset.IsAppearanceEnabled())
   {
      m_workset.layout();
   }
   else
   {
      ::user::frame_window::layout();
   }
   //{

   //   DWORD dwTime2 = ::get_tick_count();

   //   //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
   //   TRACE("smpfrmwnd::lyot call timeG= %d ms",dwTime2 - t_time1.operator DWORD_PTR());

   //}


}

void simple_frame_window::ViewOnActivateFrame(sp(::user::impact) pview, UINT user, sp(::user::interaction) pframe)
{
   UNREFERENCED_PARAMETER(pview);
   UNREFERENCED_PARAMETER(user);
   UNREFERENCED_PARAMETER(pframe);
   //   if(pview != NULL)
   //      pview->OnActivateFrame(WA_INACTIVE, (sp(::user::simple_frame_window)) pframe);
}

void simple_frame_window::_001OnGetMinMaxInfo(signal_details * pobj)
{
#ifdef WINDOWSEX
   SCAST_PTR(::message::base, pbase, pobj);
   MINMAXINFO FAR * lpMMI = (MINMAXINFO FAR*) pbase->m_lparam.m_lparam;
   if (IsFullScreen())
   {
      lpMMI->ptMaxSize.y = m_FullScreenWindowRect.height();
      lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y;
      lpMMI->ptMaxSize.x = m_FullScreenWindowRect.width();
      lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x;
   }
#else
   throw todo(get_app());
#endif
}

void simple_frame_window::ShowControlBars(bool bShow, bool bLeaveFullScreenBarsOnHide)
{
   UINT nShow;
   if (bShow)
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


   POSITION pos = m_toolbarmap.get_start_position();

   id idKey;
   sp(::user::toolbar) pbar;
   while (pos != NULL)
   {
      m_toolbarmap.get_next_assoc(pos, idKey, pbar);
      try
      {
         if(pbar != NULL && (bShow || (!pbar->m_bFullScreenBar || !bLeaveFullScreenBarsOnHide)))
         {
            pbar->ShowWindow(nShow);
         }
      }
      catch (...)
      {
      }
   }

   layout();

}


void simple_frame_window::WfiOnFullScreen()
{

   ShowControlBars(false, true);

   _001WindowFullScreen();

}

bool simple_frame_window::WfiToggleTransparentFrame()
{

   m_bTransparentFrame = !m_bTransparentFrame;

   data_set("transparent_frame",m_bTransparentFrame);

   return true;

}


bool simple_frame_window::frame_is_transparent()
{

   return m_bTransparentFrame;

}


void simple_frame_window::WfiOnExitFullScreen()
{

   ShowControlBars(true);

   WorkSetClientInterface::WfiOnExitFullScreen();

}


void simple_frame_window::_001OnViewFullScreen(signal_details * pobj)
{

   UNREFERENCED_PARAMETER(pobj);

   ToggleFullScreen();

}


void simple_frame_window::_001OnMouseMove(signal_details * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
   //   SCAST_PTR(::message::mouse, pmouse, pobj);
}

void simple_frame_window::_001OnUpdateViewFullScreen(signal_details * pobj)
{
   SCAST_PTR(::aura::cmd_ui, pcmdui, pobj);
   pcmdui->m_pcmdui->Enable();
   pcmdui->m_pcmdui->_001SetCheck(WfiIsFullScreen());
   pcmdui->m_bRet = true;
}


void simple_frame_window::ToggleFullScreen()
{

   if (WfiIsFullScreen())
   {

      WfiRestore(false);

   }
   else
   {

      WfiFullScreen();

   }

}


bool simple_frame_window::_001CanEnterScreenSaver()
{
   return true;
}

void simple_frame_window::_001OnSysCommand(signal_details * pobj)
{
   SCAST_PTR(::message::base, pbase, pobj);

   if (pbase->m_wparam == SC_SCREENSAVE)
   {
      if (!_001CanEnterScreenSaver())
      {
         pbase->m_bRet = true;
         pbase->set_lresult(0);
         return;
      }
   }

   if (m_bWindowFrame)
   {
      if (pbase->m_wparam == SC_MAXIMIZE)
      {
         WfiMaximize();
         pbase->m_bRet = true;
         pbase->set_lresult(0);
      }
      else if (pbase->m_wparam == SC_RESTORE)
      {
         if (WfiIsFullScreen())
         {
            WfiFullScreen();
         }
         else
         {
            WfiRestore(false);
         }
         pbase->m_bRet = true;
         pbase->set_lresult(0);
      }
      else if (pbase->m_wparam == SC_MINIMIZE)
      {
         WfiMinimize();
         pbase->m_bRet = true;
         pbase->set_lresult(0);
      }
   }

}


void simple_frame_window::_001OnToggleCustomFrame(signal_details * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
   SetCustomFrame(!GetCustomFrame());
}

void simple_frame_window::_001OnUpdateToggleCustomFrame(signal_details * pobj)
{
   SCAST_PTR(::aura::cmd_ui, pcmdui, pobj);
   pcmdui->m_pcmdui->Enable();
   pcmdui->m_pcmdui->_001SetCheck(m_bWindowFrame);
}



void simple_frame_window::ActivateFrame(int32_t nCmdShow)
{

   ::user::frame_window::ActivateFrame(nCmdShow);
}

void simple_frame_window::GetBorderRect(LPRECT lprect)
{
   *lprect = m_rectBorder;
}

void simple_frame_window::SetBorderRect(const RECT & rect)
{
   m_rectBorder = rect;
}

void simple_frame_window::SetCustomFrame(bool bCustom)
{
   m_bWindowFrame = bCustom;
   m_workset.Enable(bCustom);
   layout();
   RedrawWindow();

}

bool simple_frame_window::GetCustomFrame()
{
   return m_bWindowFrame;
}


void simple_frame_window::_001OnAppExit(signal_details * pobj)
{

   if(pobj != NULL)
   {

      pobj->m_bRet = true;

   }

   System.request_exit();


}


void simple_frame_window::_001OnClose(signal_details * pobj)
{

   if(m_bDefaultNotifyIcon)
   {

      if(pobj != NULL)
      {

         SCAST_PTR(::message::base, pbase, pobj);

         pbase->m_bRet = true;

      }

      if(IsWindowVisible())
      {

         ShowWindow(SW_HIDE);

      }
      else
      {

         ShowWindow(SW_SHOW);

         InitialFramePosition(true);

      }

      return;

   }



   if (m_iModalCount > 0)
   {
      EndModalLoop(IDOK);
      pobj->m_bRet = true;
      return;
   }
   else if (GetTopLevelFrame() != NULL
            && (GetTopLevelFrame()->m_iModalCount > 0))
   {
      GetTopLevelFrame()->EndModalLoop(IDOK);
      pobj->m_bRet = true;
      return;
   }
   else if (m_iModalCount > 0)
   {
      m_pimpl->EndModalLoop(IDOK);
      pobj->m_bRet = true;
      ShowWindow(SW_HIDE);
      return;
   }
   else if (GetTopLevelFrame() != NULL && GetTopLevelFrame()->m_iModalCount > 0)
   {
      GetTopLevelFrame()->EndModalLoop(IDOK);
      pobj->m_bRet = true;
      return;
   }

   if (pobj)
   {
      pobj->m_bRet = true;
   }
   // Note: only queries the active document
   sp(::user::document) pdocument = GetActiveDocument();
   if (pdocument != NULL && !pdocument->can_close_frame(this))
   {
      // document can't close right now -- don't close it
      return;
   }

   ::aura::application * papp = &Application;

   if (papp->m_pcoreapp->is_system() || papp->m_pcoreapp->is_session())
   {

      // TODO: instead of closing all applications in process System.m_apptra, should close application that make part of
      // cube, bergedge, session or system.

      ::aura::application_ptra appptra = System.get_appptra();

      for (int32_t i = 0; i <  appptra.get_count(); i++)
      {

         sp(::aura::application) pappChild = appptra[i];

         if (!pappChild->m_pcoreapp->_001CloseApplicationByUser(this))
            return;

      }

   }
   else if(Application.GetVisibleTopLevelFrameCountExcept(this) <= 0)
   {

      if (!papp->m_pcoreapp->_001CloseApplicationByUser(this))
         return;

   }
   else
   {
      DestroyWindow();
   }

}

#ifdef WINDOWSEX

void simple_frame_window::OnNcCalcSize(bool bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
   UNREFERENCED_PARAMETER(bCalcValidRects);
   if(m_workset.IsEnabled() && m_workset.m_pframeschema != NULL)
   {
      m_workset.OnNcCalcSize(&lpncsp->rgrc[0]);
   }
   else
   {
      // trans      ::user::frame_window::OnNcCalcSize(bCalcValidRects, lpncsp);
   }

}

#endif

void simple_frame_window::_001OnNcActivate(signal_details * pobj)
{
   SCAST_PTR(::message::nc_activate, pncactivate, pobj);
   // stay active if WF_STAYACTIVE bit is on
   if (m_nFlags & WF_STAYACTIVE)
      pncactivate->m_bActive = TRUE;

   // but do not stay active if the window is disabled
   if (!is_window_enabled())
      pncactivate->m_bActive = FALSE;

   /*if(m_bWindowFrame)
   {
   m_workset.SetActiveFlag(pncactivate->m_bActive);
   pncactivate->set_lresult(TRUE); // bStop
   pncactivate->m_bRet = true;
   return;
   }
   else*/
   {
      // do not call the base class because it will call Default()
      //  and we may have changed bActive.
      pncactivate->set_lresult(DefWindowProc(WM_NCACTIVATE, pncactivate->m_bActive, 0)); // bStop
      pncactivate->m_bRet = pncactivate->get_lresult() != 0;
      return;
   }
}



bool simple_frame_window::LoadFrame(const char * pszMatter, uint32_t dwDefaultStyle, sp(::user::interaction) pParentWnd, sp(::create) pContext)
{

   UNREFERENCED_PARAMETER(pParentWnd);

   // only do this once
   //   ASSERT_VALID_IDR(nIDResource);
   //   ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

   m_strMatterHelp = pszMatter;    // ID for help context (+HID_BASE_RESOURCE)

   //   string strFullString;
   //   if (strFullString.load_string(nIDResource))
   //      __extract_sub_string(m_strTitle, strFullString, 0);    // first sub-string

   const char * lpszTitle = m_strTitle;

   if (pParentWnd == NULL)
      pParentWnd = Application.get_request_parent_ui(this, pContext);

   dwDefaultStyle &= ~WS_VISIBLE;

   ::rect rectFrame;

   sp(::user::place_holder) pholder;

   if(pParentWnd != NULL && (pholder = pParentWnd).is_set())
   {

      pholder->GetClientRect(rectFrame);

   }
   else
   {

      rectFrame = null_rect();

   }

   m_bLayoutEnable = false;

   if (!create_window_ex(0L, NULL, lpszTitle, dwDefaultStyle, rectFrame, pParentWnd, /*nIDResource*/ 0, pContext))
   {

      return FALSE;   // will self destruct on failure normally
   }


   /* trans   // save the default menu handle
      ASSERT(get_handle() != NULL);
      m_hMenuDefault = ::GetMenu(get_handle()); */

   // load accelerator resource
   //   LoadAccelTable(MAKEINTRESOURCE(nIDResource));

   if (pContext == NULL)   // send initial update
      SendMessageToDescendants(WM_INITIALUPDATE, 0, (LPARAM)0, TRUE, TRUE);

   return TRUE;
}




void simple_frame_window::_001OnDdeInitiate(signal_details * pobj)
{
   SCAST_PTR(::message::base, pbase, pobj);
   pbase->set_lresult(DefWindowProc((uint32_t)pbase->m_wparam, pbase->m_lparam, pbase->get_lresult()));
}


void simple_frame_window::pre_translate_message(signal_details * pobj)
{

   SCAST_PTR(::message::base, pbase, pobj);

   if(pbase->m_uiMessage == message_display_change)
   {

      Wfi();

   }
   else if (pbase->m_uiMessage == WM_MOUSEMOVE)
   {

   }
   else if(pbase->m_uiMessage == WM_KEYDOWN || pbase->m_uiMessage == WM_SYSKEYDOWN)
   {

      SCAST_PTR(::message::key, pkey, pobj);

      if(pkey->m_ekey == ::user::key_alt || pkey->m_ekey == ::user::key_lalt || pkey->m_ekey == ::user::key_ralt)
      {

         m_bFullScreenAlt = false;

      }
      else if(pkey->m_ekey == ::user::key_control || pkey->m_ekey == ::user::key_lcontrol || pkey->m_ekey == ::user::key_rcontrol)
      {

         m_bFullScreenCtrl = false;

      }
      else if(pkey->m_ekey == ::user::key_return)
      {

         if (Session.is_key_pressed(::user::key_control) && Session.is_key_pressed(::user::key_alt))
         {

            m_bFullScreenAlt = true;

            m_bFullScreenCtrl = true;

            if (!IsFullScreen())
            {

               if (DeferFullScreen(true, false))
               {

                  pbase->m_bRet = true;

                  return;

               }

            }

         }

      }

   }
   else if(pbase->m_uiMessage == WM_KEYUP || pbase->m_uiMessage == WM_SYSKEYUP)
   {

      SCAST_PTR(::message::key, pkey, pobj);

      if(pkey->m_ekey == ::user::key_alt || pkey->m_ekey == ::user::key_lalt || pkey->m_ekey == ::user::key_ralt)
      {

         if (IsFullScreen() && Session.is_key_pressed(::user::key_control) && !m_bFullScreenAlt && !m_bFullScreenCtrl)
         {

            if (WfiRestore(m_eappearanceBefore != ::user::AppearanceFullScreen))
            {

               pbase->m_bRet = true;

               m_bFullScreenAlt = false;

               return;

            }

         }

         m_bFullScreenAlt = false;

      }
      else if(pkey->m_ekey == ::user::key_control || pkey->m_ekey == ::user::key_lcontrol || pkey->m_ekey == ::user::key_rcontrol)
      {

         if(IsFullScreen() && Session.is_key_pressed(::user::key_alt) && !m_bFullScreenAlt && !m_bFullScreenCtrl)
         {

            if(WfiRestore(m_eappearanceBefore != ::user::AppearanceFullScreen))
            {

               pbase->m_bRet = true;

               m_bFullScreenAlt = false;

               return;

            }

         }

         m_bFullScreenAlt = false;

      }

   }

   return ::user::frame_window::pre_translate_message(pobj);

}


void simple_frame_window::InitialFramePosition(bool bForceRestore)
{

   if(m_bFrameMoveEnable)
   {

      if(Application.command()->m_varTopicQuery.has_property("wfi_maximize")
            && (GetParent() == NULL
#if defined(ANDROID) || defined(METROWIN) || defined(APPLE_IOS)
                || GetParent() == System.m_posdata->m_pui
#endif
               ))
      {

         WfiMaximize();

      }
      else if(Application.command()->m_varTopicQuery.has_property("client_only")
              || Application.command()->m_varTopicQuery.has_property("full_screen"))
      {

         if(m_workset.IsAppearanceEnabled())
         {

            WfiFullScreen();

         }
         else
         {

            best_monitor(NULL,::null_rect(),true);

         }

      }
      else
      {


         WindowDataLoadWindowRect(bForceRestore,true);

         WindowDataEnableSaveWindowRect(true);

      }

   }

   ActivateTopParent();

   BringToTop(-1);

   if (m_workset.IsAppearanceEnabled())
   {

      if (m_workset.get_appearance() != NULL && m_workset.GetAppearance() == ::user::AppearanceIconic)
      {

         WfiRestore(false);

      }

   }

   m_bLayoutEnable = true;

   layout();

   m_bInitialFramePosition = false;


}


void simple_frame_window::_001OnDeferPaintLayeredWindowBackground(::draw2d::dib * pdib)
{

   ::draw2d::graphics * pdc = pdib->get_graphics();

   if (Session.savings().is_trying_to_save(::aura::resource_processing)
         || Session.savings().is_trying_to_save(::aura::resource_translucent_background))
   {
      rect rectClient;
      GetClientRect(rectClient);
      pdc->FillSolidRect(rectClient, RGB(0, 0, 0));
   }
   else
   {

      ::user::frame_window::_001OnDeferPaintLayeredWindowBackground(pdib);
   }

}


void simple_frame_window::_000OnDraw(::draw2d::dib * pdibParam)
{

   if (!(IsWindowVisible() && (GetParent() == NULL || GetTopLevelFrame() == NULL || !GetTopLevelFrame()->WfiIsIconic())))
      return;

   ::draw2d::graphics * pdcParam = pdibParam->get_graphics();

   rect rectClient;

   GetWindowRect(rectClient);

   rectClient -= rectClient.top_left();

   bool bDib = false;

   double dAlpha = get_alpha();

   ::draw2d::graphics * pdc = pdcParam;

   ::draw2d::dib * pdib = pdibParam;

   if(rectClient.area() > 0 && dAlpha > 0.0 && dAlpha < 1.0 && GetExStyle() & WS_EX_LAYERED)
   {

      if(m_dibAlpha.is_null())
      {

         m_dibAlpha.alloc(allocer());

      }

      m_dibAlpha->create(rectClient.size());

      m_dibAlpha->Fill(0,0,0,0);

      pdc = m_dibAlpha->get_graphics();

      pdib = m_dibAlpha;

      pdc->SetViewportOrg(pdc->GetViewportOrg());

      bDib = true;

   }

   if(dAlpha > 0.0)
   {

      if(m_puserschema != NULL && m_puserschema->_001OnDrawMainFrameBackground(pdib,this))
      {
         _001DrawThis(pdib);
         _001DrawChildren(pdib);
         _008CallOnDraw(pdib);
      }
      else if (m_bblur_Background)
      {
         _001DrawThis(pdib);
         _001DrawChildren(pdib);
         _008CallOnDraw(pdib);
      }
      else if(!Session.savings().is_trying_to_save(::aura::resource_processing)
              && !Session.savings().is_trying_to_save(::aura::resource_display_bandwidth)
              && !Session.savings().is_trying_to_save(::aura::resource_memory))
         //&& (GetParent() != NULL || (this->GetExStyle() & WS_EX_LAYERED) != 0))
      {
#if TEST

         pdc->FillSolidRect(60, 10, 50, 50, ARGB(128, 184, 177, 84));
#endif

         _010OnDraw(pdib);
#if TEST

         pdc->FillSolidRect(10, 60, 50, 50, ARGB(128, 255, 248, 84));
#endif
      }
      else
      {
#if TEST

         pdc->FillSolidRect(60, 10, 50, 50, ARGB(128, 184, 177, 84));
#endif

         _001DrawThis(pdib);
         _001DrawChildren(pdib);
         _008CallOnDraw(pdib);
#if TEST

         pdc->FillSolidRect(10, 60, 50, 50, ARGB(128, 184, 177, 84));
#endif
      }

   }

   if(bDib)
   {

      pdcParam->set_alpha_mode(::draw2d::alpha_mode_blend);

      pdcParam->alpha_blend(null_point(),rectClient.size(),pdc,null_point(),dAlpha);

   }

}


void simple_frame_window::_001OnDraw(::draw2d::dib * pdib)
{
   single_lock sl(m_pmutex, true);


   if(m_bblur_Background)
   {
      class imaging & imaging = System.visual().imaging();
      rect rectClient;
      GetClientRect(rectClient);
      //rectClient.offset(rectClient.top_left());
      if(Session.savings().is_trying_to_save(::aura::resource_translucent_background))
      {
         //pdc->FillSolidRect(rectClient, RGB(150, 220, 140));
      }
      else if(Session.savings().is_trying_to_save(::aura::resource_processing)
              || Session.savings().is_trying_to_save(::aura::resource_blur_background))
      {
         imaging.color_blend(pdib->get_graphics(),rectClient,RGB(150,180,140),150);
      }
      else
      {
#ifndef LINUX
         if(rectClient.size() != m_dibBk->size())
         {
            m_dibBk->create(rectClient.size());
            m_dibBk->Fill(0,184,184,170);
            //HMODULE hmodule = ::LoadLibrary("ca2performance.dll");
            //::visual::fastblur *( *pfnNew )(sp(::aura::application)) = (::visual::fastblur *(*)(sp(::aura::application))) ::GetProcAddress(hmodule, "new_fastblur");
            m_fastblur.alloc(allocer());
            m_fastblur.initialize(rectClient.size(),2);
         }
         if(m_fastblur.is_set() && m_fastblur->area() > 0)
         {
            m_fastblur->get_graphics()->BitBlt(0,0,rectClient.width(),rectClient.height(),pdib->get_graphics(),0,0,SRCCOPY);
            m_fastblur.blur();
            imaging.bitmap_blend(
               m_fastblur->get_graphics(),
               null_point(),
               rectClient.size(),
               m_dibBk->get_graphics(),
               null_point(),
               49);
            pdib->get_graphics()->from(rectClient.size(),
                                       m_fastblur->get_graphics(),
                                       null_point(),
                                       SRCCOPY);
         }
#endif
      }
   }

   _011OnDraw(pdib);


}


bool simple_frame_window::on_before_set_parent(sp(::user::interaction) pinterface)
{

   WindowDataSaveWindowRect();

   if (!::user::box::on_before_set_parent(pinterface))
      return false;

   return true;

}


void simple_frame_window::on_set_parent(sp(::user::interaction) puiParent)
{

   ::user::box::on_set_parent(puiParent);

   UNREFERENCED_PARAMETER(puiParent);
   m_workset.m_pwndEvent = m_pimpl->m_pui;

   if (m_pupdowntarget != NULL && m_pupdowntarget->is_up_down_target())
   {
      if (puiParent == NULL)
      {
         m_bWindowFrame = true;
         m_workset.Enable(true);
      }
      else
      {
         m_bWindowFrame = false;
         m_workset.Enable(false);
      }
   }

   if (puiParent == NULL || !puiParent->is_place_holder())
   {

      WindowDataLoadWindowRect(false);

      WindowDataEnableSaveWindowRect(true);


   }

   if (m_bAutoWindowFrame)
   {
      // an updowntarget always show the frame for upping/downing
      if (!m_bWindowFrame)
      {
         m_bWindowFrame = m_bCustomFrameBefore;
      }
      if (!m_workset.IsEnabled())
      {
         m_workset.Enable(true);
         layout();
      }
   }
   else
   {
      if (puiParent != NULL)
      {
         m_bCustomFrameBefore = m_bWindowFrame;
         m_bWindowFrame = false;
         m_workset.Enable(false);
         layout();
      }
      else
      {
         m_bWindowFrame = m_bCustomFrameBefore;
         m_workset.Enable(m_bWindowFrame);
         layout();
      }
   }


}

bool simple_frame_window::GetClientRect(LPRECT lprect)
{

   if (m_bWindowFrame && m_pframeschema != NULL && !WfiIsFullScreen())
   {

      return m_pframeschema->get_window_client_rect(lprect);

   }
   else
   {

      return ::user::frame_window::GetClientRect(lprect);

   }


}


bool simple_frame_window::is_application_main_window()
{

   return Application.m_puiMain == this;

}


bool simple_frame_window::LoadToolBar(sp(::type) sptype, id idToolBar, const char * pszToolBar, uint32_t dwCtrlStyle, uint32_t dwStyle)
{

   sp(::user::toolbar) ptoolbar = m_toolbarmap[idToolBar];

   if(ptoolbar.is_null())
   {

      ptoolbar = Application.alloc(sptype);

      if(ptoolbar == NULL)
         return false;

      ptoolbar->create_window_ex(this,dwCtrlStyle,dwStyle);

   }

   if(ptoolbar == NULL)
      return false;

   string strMatter = Application.dir().matter(pszToolBar);

   string strXml = Application.file().as_string(strMatter);

   if(!ptoolbar->LoadXmlToolBar(strXml))
   {
      try
      {
         ptoolbar;

      }
      catch(...)
      {
      }
      return false;
   }

   m_toolbarmap.set_at(idToolBar,ptoolbar);

   AddControlBar(ptoolbar);

   layout();

   return true;

}


void simple_frame_window::_001OnUser184(signal_details * pobj)
{

   SCAST_PTR(::message::base, pbase, pobj);

   if (pbase->m_wparam == 0 && pbase->m_lparam == 0)
   {

      InitialFramePosition(true);

      pbase->m_bRet = true;

   }
   else if(pbase->m_wparam == 2)
   {

      if(m_bDefaultNotifyIcon)
      {

         m_piconNotify = canew(::visual::icon(get_app()));

         const char * pszAppName = Application.m_strAppName;

         m_piconNotify->load_app_tray_icon(pszAppName);

         m_pnotifyicon = canew(::user::notify_icon(get_app()));

         m_pnotifyicon->create(1, this, m_piconNotify);

         if(m_workset.m_pframeschema != NULL)
         {

            m_workset.m_pframeschema->m_spcontrolbox->hide_button(::user::wndfrm::frame::button_minimize);

         }

      }

      m_pimpl->show_task(IsWindowVisible() && m_bShowTask);

   }
   else if(pbase->m_wparam == 123)
   {
      if(does_display_match())
      {

         WindowDataSaveWindowRect();

      }


   }
}

// persistent frame implemenation using updowntarget
bool simple_frame_window::WndFrameworkDownUpGetUpEnable()
{
   return
      m_pupdowntarget != NULL
      && m_pupdowntarget->is_up_down_target()
      && m_pupdowntarget->up_down_target_is_down();
}

bool simple_frame_window::WndFrameworkDownUpGetDownEnable()
{
   return
      m_pupdowntarget != NULL
      && m_pupdowntarget->is_up_down_target()
      && m_pupdowntarget->up_down_target_is_up();
}


void simple_frame_window::WfiOnDown()
{

   frame_Attach();

}


void simple_frame_window::WfiOnUp()
{

   frame_Detach();

}

bool simple_frame_window::WfiIsMoving()
{

   return m_bWindowFrame && m_workset.GetMovingManager() != NULL && m_workset.GetMovingManager()->IsMoving();

}


bool simple_frame_window::WfiIsSizing()
{

   return m_bWindowFrame && m_workset.GetSizingManager() != NULL && m_workset.GetSizingManager()->IsSizing();

}



bool simple_frame_window::create_window(const char * lpszClassName,const char * lpszWindowName,uint32_t dwStyle,const RECT & rect,sp(::user::interaction) pParentWnd,const char * lpszMenuName,uint32_t dwExStyle,sp(::create) pContext)
{
   return ::user::frame_window::create_window(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);

}





bool simple_frame_window::_001OnCmdMsg(::aura::cmd_msg * pcmdmsg)
{

   if (m_workset._001OnCmdMsg(pcmdmsg))
      return true;

   // pump through current ::user::impact FIRST
   sp(::user::impact) pview = GetActiveView();
   if (pview != NULL && pview->_001OnCmdMsg(pcmdmsg))
      return TRUE;

   pview = get_child_by_id("pane_first");
   if (pview != NULL && pview->_001OnCmdMsg(pcmdmsg))
      return TRUE;

   // then pump through frame
   if (::user::frame_window::_001OnCmdMsg(pcmdmsg))
      return TRUE;

   // then pump through parent
   sp(::user::interaction) puiParent = GetParent();
   if (puiParent != NULL && puiParent->_001OnCmdMsg(pcmdmsg))
      return TRUE;

   // last but not least, pump through cast
   ::core::application* pApp = get_app()->m_pcoreapp;
   if (pApp != NULL && pApp->_001OnCmdMsg(pcmdmsg))
      return TRUE;

   sp(command_target_interface) pcommandtargetinterface = Session.get_keyboard_focus();

   if (pcommandtargetinterface != NULL)
   {
      if (pcommandtargetinterface->_001OnCmdMsg(pcmdmsg))
         return TRUE;
   }

   return FALSE;
}


#ifdef WINDOWSEX



void simple_frame_window::OnDropFiles(HDROP hDropInfo)
{

   SetActiveWindow();      // activate us first !
   UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);

   ::userex::userex* puser = Session.userex();
   ASSERT(puser != NULL);
   for (UINT iFile = 0; iFile < nFiles; iFile++)
   {
      char szFileName[_MAX_PATH];
      ::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);

      sp(::create) createcontext(allocer());
      createcontext->m_spCommandLine->m_varFile = szFileName;

      puser->open_document_file(createcontext);
   }
   ::DragFinish(hDropInfo);


}

// query end session for main frame will attempt to close it all down
bool simple_frame_window::OnQueryEndSession()
{
   ::base::application* pApp = &System;
   if(pApp != NULL && pApp->m_puiMain == this)
      return pApp->m_pcoreapp->save_all_modified();

   return TRUE;
}


void simple_frame_window::OnEndSession(bool bEnding)
{

   if(!bEnding)
      return;

   Application.close(::core::end_system);

}


#else

//void simple_frame_window::OnDropFiles(HDROP hDropInfo)
//{
//
//   UNREFERENCED_PARAMETER(hDropInfo);
//
//}
//


#endif






LRESULT simple_frame_window::OnDDEInitiate(WPARAM wParam, LPARAM lParam)
{


#ifdef WINDOWSEX

   ::core::application* pApp = &System;
   if (pApp != NULL &&
         LOWORD(lParam) != 0 && HIWORD(lParam) != 0 &&
         (ATOM)LOWORD(lParam) == pApp->m_atomApp &&
         (ATOM)HIWORD(lParam) == pApp->m_atomSystemTopic)
   {
      // make duplicates of the incoming atoms (really adding a reference)
      char szAtomName[_MAX_PATH];
      VERIFY(GlobalGetAtomName(pApp->m_atomApp,
                               szAtomName, _MAX_PATH - 1) != 0);
      VERIFY(GlobalAddAtom(szAtomName) == pApp->m_atomApp);
      VERIFY(GlobalGetAtomName(pApp->m_atomSystemTopic,
                               szAtomName, _MAX_PATH - 1) != 0);
      VERIFY(GlobalAddAtom(szAtomName) == pApp->m_atomSystemTopic);

      // send the WM_DDE_ACK (caller will delete duplicate atoms)
      ::SendMessage((oswindow)wParam, WM_DDE_ACK, (WPARAM)get_handle(),
                    MAKELPARAM(pApp->m_atomApp, pApp->m_atomSystemTopic));
   }

#else

   ::exception::throw_not_implemented(get_app());

#endif

   return 0L;
}


// always ACK the execute command - even if we do nothing
LRESULT simple_frame_window::OnDDEExecute(WPARAM wParam, LPARAM lParam)
{


#ifdef WINDOWSEX

   // unpack the DDE message
   uint_ptr unused;
   HGLOBAL hData;
   //IA64: Assume DDE LPARAMs are still 32-bit
   VERIFY(UnpackDDElParam(WM_DDE_EXECUTE, lParam, &unused, (uint_ptr*)&hData));

   // get the command string
   const char * lpsz = (const char *)GlobalLock(hData);
   string strCommand;
   try
   {
      strCommand = lpsz;
      GlobalUnlock(hData);
   }
   catch (memory_exception * pe)
   {
      GlobalUnlock(hData);
      pe->Delete();
   }


   // acknowledge now - before attempting to execute
   ::PostMessage((oswindow)wParam, WM_DDE_ACK, (WPARAM)get_handle(),
                 //IA64: Assume DDE LPARAMs are still 32-bit
                 ReuseDDElParam(lParam, WM_DDE_EXECUTE, WM_DDE_ACK,
                                (UINT)0x8000, (uint_ptr)hData));

   // don't execute the command when the window is disabled
   if (!is_window_enabled())
   {
      TRACE(::aura::trace::category_AppMsg, 0, "Warning: DDE command '%s' ignored because window is disabled.\n",
            strCommand.GetString());
      return 0;
   }

   // execute the command
   LPTSTR lpszCommand = strCommand.GetBuffer();
   if (!System.OnDDECommand(lpszCommand))
      TRACE(::aura::trace::category_AppMsg, 0, "Error: failed to execute DDE command '%s'.\n", lpszCommand);
   strCommand.ReleaseBuffer();

#else

   ::exception::throw_not_implemented(get_app());

#endif

   return 0L;

}

LRESULT simple_frame_window::OnDDETerminate(WPARAM wParam, LPARAM lParam)
{

#ifdef WINDOWSEX

   ::PostMessage((oswindow)wParam, WM_DDE_TERMINATE, (WPARAM)get_handle(), lParam);

#else

   ::exception::throw_not_implemented(get_app());

#endif

   return 0L;

}


void simple_frame_window::NotifyFloatingWindows(uint32_t dwFlags)
{
   ASSERT_VALID(this);
   // trans   ASSERT(get_handle() != NULL);

   // get top level parent frame window first unless this is a child window
   sp(::user::frame_window) pParent = (GetStyle() & WS_CHILD) ? this : GetTopLevelFrame();
   ASSERT(pParent != NULL);
   if (dwFlags & (FS_DEACTIVATE | FS_ACTIVATE))
   {
      // update parent window activation state
      bool bActivate = !(dwFlags & FS_DEACTIVATE);
      bool bEnabled = pParent->is_window_enabled();

      if (bActivate && bEnabled && pParent != this)
      {
         // Excel will try to Activate itself when it receives a
         // WM_NCACTIVATE so we need to keep it from doing that here.
         m_nFlags |= WF_KEEPMINIACTIVE;
         pParent->send_message(WM_NCACTIVATE, TRUE);
         m_nFlags &= ~WF_KEEPMINIACTIVE;
      }
      else
      {
         pParent->send_message(WM_NCACTIVATE, FALSE);
      }
   }

   // then update the state of all floating windows owned by the parent
#ifdef WINDOWSEX

   sp(::user::interaction) oswindowDesktop = System.get_desktop_window();

   if (oswindowDesktop.is_null())
      return;

   sp(::user::interaction) oswindow = oswindowDesktop->get_wnd(GW_CHILD);

   while (oswindow != NULL)
   {

      if (::user::is_descendant(pParent, oswindow))
         oswindow->send_message(WM_FLOATSTATUS, dwFlags);

      oswindow = oswindow->get_wnd(GW_HWNDNEXT);

   }

#else
   throw todo(get_app());
#endif
}



// query end session for main frame will attempt to close it all down
void simple_frame_window::_001OnQueryEndSession(signal_details * pobj)
{
   SCAST_PTR(::message::base, pbase, pobj);
   if (&System != NULL && System.m_puiMain == this)
   {
      pbase->set_lresult(System.save_all_modified());
      pbase->m_bRet = true;
      return;
   }
   pbase->set_lresult(TRUE);
   return;
}

bool simple_frame_window::BaseOnControlEvent(::user::control_event * pevent)
{
   if (m_workset.BaseOnControlEvent(pevent))
      return true;
   return ::user::frame_window::BaseOnControlEvent(pevent);
}


string simple_frame_window::get_window_default_matter()
{

   if (m_pdocumenttemplate == NULL)
   {

      return frame_window::get_window_default_matter();

   }

   return m_pdocumenttemplate->m_strMatter;

}


void simple_frame_window::guserbaseOnInitialUpdate(signal_details * pobj)
{
   SCAST_PTR(::message::base, pbase, pobj);
   ::user::FrameInitialUpdate * pfiu = (::user::FrameInitialUpdate *)pbase->m_lparam.m_lparam;
   if (pfiu != NULL)
   {
      sp(::user::frame_window) pframe = (this);
      // if the frame does not have an active ::user::impact, set to first pane
      sp(::user::impact) pview = NULL;
      if (pframe->GetActiveView() == NULL)
      {
         sp(::user::interaction) pwindow = pframe->GetDescendantWindow("pane_first");
         if (pwindow != NULL && base_class < ::user::impact >::bases(pwindow))
         {
            pview = (pwindow.m_p);
            pframe->SetActiveView(pview, FALSE);
         }
      }

      if (pfiu->m_bMakeVisible)
      {
         // send initial update to all views (and other controls) in the frame
         pframe->SendMessageToDescendants(WM_INITIALUPDATE, 0, (LPARAM)0, TRUE, TRUE);

         // give ::user::impact a chance to save the focus (CFormView needs this)
         if (pview != NULL)
            pview->OnActivateFrame(WA_INACTIVE, pframe);

         // finally, activate the frame
         // (send the default show command unless the main desktop window)
         int32_t nCmdShow = -1;      // default
         ::core::application* pApp = &System;
         if (pApp != NULL && pApp->m_puiMain == pframe)
         {
            nCmdShow = System.m_nCmdShow; // use the parameter from WinMain
            System.m_nCmdShow = -1; // set to default after first time
         }
         bool bFullScreen;
         data_get("FullScreen", bFullScreen);
         if (bFullScreen)
         {
            WfiFullScreen();
         }
         else
         {
            pframe->ActivateFrame(nCmdShow);
         }
         if (pview != NULL)
            pview->OnActivateView(TRUE, pview, pview);

      }

      sp(::user::document) pdoc = pfiu->m_pdoc;
      // update frame counts and frame title (may already have been visible)
      if (pdoc != NULL)
         pdoc->update_frame_counts();
      pframe->on_update_frame_title(TRUE);

      RedrawWindow();
   }
   pbase->set_lresult(0);
}


void simple_frame_window::_010OnDraw(::draw2d::dib * pdib)
{

   if (!m_bVisible)
      return;

   if(GetExStyle() & WS_EX_LAYERED)
   {

      sp(::user::interaction) pui = bottom_child();

      while (pui != NULL)
      {

         if (pui->IsWindowVisible() && !base_class < ::user::wndfrm::frame::control_box > ::bases(pui))
         {

            pui->_000OnDraw(pdib);

         }

         pui = pui->above_sibling();

      }

      _001DrawThis(pdib);

      pui = bottom_child();

      while (pui != NULL)
      {
         if (base_class < ::user::wndfrm::frame::control_box > ::bases(pui))
         {
            string str;
            pui->GetWindowText(str);
            if (str == "r")
            {
               //TRACE0("x button");
            }
            if (pui->IsWindowVisible())
            {
               if (str == "r")
               {
                  //TRACE0("x button visible");
               }
               pui->_000OnDraw(pdib);
            }

         }
         pui = pui->above_sibling();
      }

      _008CallOnDraw(pdib);

   }
   else
   {

      _001DrawThis(pdib);

      _001DrawChildren(pdib);

      _008CallOnDraw(pdib);

   }

}


void simple_frame_window::_011OnDraw(::draw2d::dib * pdib)
{

   if ((m_bWindowFrame || _001IsTranslucent()) && !Session.savings().is_trying_to_save(::aura::resource_display_bandwidth))
   {

      ::user::wndfrm::frame::WorkSetClientInterface::_001OnDraw(pdib);

   }
   else
   {

      rect rect;

      GetClientRect(rect);

      pdib->get_graphics()->FillSolidRect(rect, _001GetColor(::user::color_background));

   }

}


bool simple_frame_window::WfiOnMove(bool bTracking)
{
   if (!bTracking)
   {
      WindowDataSaveWindowRect();
   }
   return true;
}


bool simple_frame_window::WfiOnSize(bool bTracking)
{

   if (!bTracking)
   {

      WindowDataSaveWindowRect();

   }

   return true;

}


void simple_frame_window::WfiOnClose()
{

   post_message(WM_CLOSE);

}


void simple_frame_window::WfiOnMaximize()
{

   _001WindowMaximize();

}


void simple_frame_window::WfiOnMinimize()
{

   _001WindowMinimize();

}


void simple_frame_window::WfiOnRestore()
{

   _001WindowRestore();

}


void simple_frame_window::WfiOnDock(::user::EAppearance eappearance)
{

   _001WindowDock(eappearance);

}


bool simple_frame_window::DeferFullScreen(bool bFullScreen, bool bRestore)
{
   if (bFullScreen)
   {
      sp(::user::interaction) pwndParentFrame = GetParentFrame();
      if (pwndParentFrame == NULL)
      {
         if (!WfiIsFullScreen())
         {
            WfiFullScreen();
            return true;
         }
         else
         {
            return false;
         }
      }
      simple_frame_window * pframe = dynamic_cast <simple_frame_window *> (pwndParentFrame.m_p);
      if (pframe == NULL)
      {
         if (!WfiIsFullScreen())
         {
            WfiFullScreen();
            return true;
         }
         else
         {
            return false;
         }
      }

      if (pframe->DeferFullScreen(bFullScreen, bRestore))
         return true;

      if (!WfiIsFullScreen())
      {
         WfiFullScreen();
         return true;
      }
      else
      {
         return false;
      }
   }
   else
   {
      if (WfiIsFullScreen())
      {
         WfiRestore(false);
         return true;
      }
      sp(::user::interaction) pwndParentFrame = GetParentFrame();
      if (pwndParentFrame == NULL)
      {
         return false;
      }
      simple_frame_window * pframe = dynamic_cast <simple_frame_window *> (pwndParentFrame.m_p);
      if (pframe == NULL)
      {
         return false;
      }
      return pframe->DeferFullScreen(bFullScreen, bRestore);
   }
}


bool simple_frame_window::calc_layered()
{

   if (m_bLayered && _001GetTranslucency() != ::user::TranslucencyNone)
   {
      return !Session.savings().is_trying_to_save(::aura::resource_processing)
             && !Session.savings().is_trying_to_save(::aura::resource_display_bandwidth);
   }
   else
   {

      return false;

   }

}


void simple_frame_window::data_on_after_change(signal_details * pobj)
{
   user::box::data_on_after_change(pobj);
   SCAST_PTR(database::change_event, phint, pobj);
   if (phint->m_key.m_id == "ca2.savings")
   {
      defer_synch_layered();
   }
}




bool simple_frame_window::on_simple_command(e_simple_command ecommand, lparam lparam, LRESULT & lresult)
{

   return ::user::frame_window::on_simple_command(ecommand, lparam, lresult);

}

class mini_dock_frame_window* simple_frame_window::CreateFloatingFrame(uint32_t dwStyle)
{
   UNREFERENCED_PARAMETER(dwStyle);
   return NULL;
}


















bool simple_frame_window::set_appearance(::user::EAppearance eappearance)
{

   bool bOk1 = ::user::frame_window::set_appearance(eappearance);

   m_workset.SetAppearance(eappearance);

   return bOk1;

}


bool simple_frame_window::create_bars()
{

   if(!on_create_bars())
      return false;

   if (!m_bInitialFramePosition)
   {

      set_need_layout();

   }

   return true;

}


bool simple_frame_window::on_create_bars()
{

   return true;

}


void simple_frame_window::InitialUpdateFrame(::user::document * pDoc,bool bMakeVisible)
{

   ::user::frame_window::InitialUpdateFrame(pDoc,bMakeVisible);

}


void simple_frame_window::_001OnTimer(::timer * ptimer)
{

   ::user::frame_window::_001OnTimer(ptimer);
   ::user::wndfrm::frame::WorkSetClientInterface::_001OnTimer(ptimer);

}
void simple_frame_window::OnNotifyIconContextMenu(UINT uiNotifyIcon)
{
   point pt;
   ::GetCursorPos(&pt);
   track_popup_xml_matter_menu("popup_notification.xml", 0, pt);
}

void simple_frame_window::OnNotifyIconLButtonDblClk(UINT uiNotifyIcon)
{
   UNREFERENCED_PARAMETER(uiNotifyIcon);
}

void simple_frame_window::OnNotifyIconLButtonDown(UINT uiNotifyIcon)
{
   InitialFramePosition(true);
   UNREFERENCED_PARAMETER(uiNotifyIcon);
}


void simple_frame_window::OnNotifyIconClose(UINT uiNotifyIcon)
{

   if(m_bDefaultNotifyIcon)
   {

      post_message(WM_CLOSE);

   }

}


void simple_frame_window::OnNotifyIconQuit(UINT uiNotifyIcon)
{

   if(m_bDefaultNotifyIcon)
   {

      post_message(WM_APPEXIT);

   }

}


bool simple_frame_window::__close_is_closed()
{

   return !IsWindowVisible();

}


bool simple_frame_window::notify_icon_frame_is_opened()
{

   return !__close_is_closed();

}


void simple_frame_window::OnInitialFrameUpdate(bool bMakeVisible)
{

   if(!m_bDefaultNotifyIcon)
   {

      ::user::frame_window::OnInitialFrameUpdate(bMakeVisible);

      return;

   }

   OnUpdateToolWindow(bMakeVisible);

   if(bMakeVisible)
   {

      InitialFramePosition();

   }

}


void simple_frame_window::OnUpdateToolWindow(bool bVisible)
{

   if (!m_bDefaultNotifyIcon)
   {

      return;

   }

   if(!bVisible)
   {

      defer_dock_application(false);

   }

   if (m_pimpl == NULL)
   {

      return;

   }

   m_pimpl->show_task(bVisible && m_bShowTask);

}


void simple_frame_window::show_task(bool bShow)
{

   if (m_pimpl == NULL)
   {

      return;

   }

   m_pimpl->show_task(bShow);

}
