#include "framework.h"
#ifdef WINDOWSEX
#include <dde.h>
#endif

#define TEST 0

uint32_t simple_frame_window_save_window_rect(void * pvoidParam);


simple_frame_window::simple_frame_window(sp(base_application) papp) :
   element(papp),
   m_dibBk(allocer()),
   m_fastblur(allocer())
{

   m_bblur_Background      = false;
   m_bCustomFrameBefore    = true;
   m_bWindowFrame          = true;
   m_bLayered              = true;
//   m_hdcOpenGL             = NULL;
//   m_hglrc                 = NULL;
   m_pframeschema          = NULL;

}

simple_frame_window::~simple_frame_window()
{
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

void simple_frame_window::install_message_handling(::ca2::message::dispatch * pinterface)
{
   ::user::frame_window::install_message_handling(pinterface);
   IGUI_WIN_MSG_LINK(WM_CREATE         , pinterface, this, &simple_frame_window::_001OnCreate);
   IGUI_WIN_MSG_LINK(WM_NCACTIVATE     , pinterface, this, &simple_frame_window::_001OnNcActivate);
#ifdef WINDOWSEX
   IGUI_WIN_MSG_LINK(WM_DDE_INITIATE   , pinterface, this, &simple_frame_window::_001OnDdeInitiate);
#endif
   IGUI_WIN_MSG_LINK(WM_DESTROY        , pinterface, this, &simple_frame_window::_001OnDestroy);
   IGUI_WIN_MSG_LINK(WM_CLOSE          , pinterface, this, &simple_frame_window::_001OnClose);
   IGUI_WIN_MSG_LINK(WM_SIZE           , pinterface, this, &simple_frame_window::_001OnSize);
   IGUI_WIN_MSG_LINK(WM_MOVE           , pinterface, this, &simple_frame_window::_001OnMove);
   IGUI_WIN_MSG_LINK(WM_SYSCOMMAND     , pinterface, this, &simple_frame_window::_001OnSysCommand);
   IGUI_WIN_MSG_LINK(WM_GETMINMAXINFO  , pinterface, this, &simple_frame_window::_001OnGetMinMaxInfo);
   IGUI_WIN_MSG_LINK(WM_USER + 184     , pinterface, this, &simple_frame_window::_001OnUser184);
   IGUI_WIN_MSG_LINK(WM_MOUSEMOVE      , pinterface, this, &simple_frame_window::_001OnMouseMove);
   IGUI_WIN_MSG_LINK(WM_DISPLAYCHANGE      , pinterface, this, &simple_frame_window::_001OnDisplayChange);

   connect_update_cmd_ui("view_full_screen", &simple_frame_window::_001OnUpdateViewFullScreen);
   connect_command("view_full_screen", &simple_frame_window::_001OnViewFullScreen);
   connect_command("app_exit", &simple_frame_window::_001OnClose);
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
      if(m_papp != NULL && &Application != NULL)
      {
         Application.remove_frame(this);
      }
   }
   catch(...)
   {
   }

   try
   {
      if(&Session != NULL)
      {
         Session.remove_frame(this);
      }
   }
   catch(...)
   {
   }

   try
   {
      if(m_papp != NULL && m_papp->m_psystem != NULL && &System != NULL)
      {
         System.remove_frame(this);
      }
   }
   catch(...)
   {
   }

   pobj->previous();

}


sp(::uinteraction::frame::frame) simple_frame_window::create_frame_schema()
{

   sp(::uinteraction::frame::frame) pschema = Application.uinteraction().get_frame_schema("app-core/uinteraction", "005");

   pschema->m_typeinfoControlBoxButton = System.type_info < MetaButton > ();

   return pschema;

}


void simple_frame_window::_001OnCreate(signal_details * pobj)
{

   SCAST_PTR(::ca2::message::create, pcreate, pobj)

   if(pobj->previous())
      return;

   sp(::user::place_holder) pplaceholder = get_parent();

   if(pplaceholder != NULL)
   {
      sp(::user::place_holder_container) pcontainer = pplaceholder->get_parent();
      if(pcontainer != NULL)
      {
         pcontainer->on_hold(this, pplaceholder);
      }
   }

   if(m_bAutoWindowFrame)
   {
#ifdef METROWIN
      m_bWindowFrame = get_parent() == NULL || dynamic_cast < ::ca2::window * > (get_parent()->m_pimpl.m_p) != NULL;
#else
      m_bWindowFrame = get_parent() == NULL;
#endif
   }


   if(m_bWindowFrame)
   {
      /*WNDCLASS wndclass;

      char szBuf [64];
      ::GetClassName(get_handle(), szBuf, _countof(szBuf));

      GetClassInfo(System.m_hInstance,
         szBuf,
         &wndclass);*/

// trans      SetIcon(wndclass.hIcon, false);

// trans      HICON hicon = GetIcon(false);

        sp(::uinteraction::frame::frame) pinteractionframe = NULL;

        try
        {


            pinteractionframe = create_frame_schema();

        }
        catch(const not_installed & e)
        {

            System.remove_frame(this);

            throw e;

        }


      //frame::FrameSchema * pschema = dynamic_cast < ::frame::FrameSchema * > (pinteractionframe);

      if(pinteractionframe != NULL && (_ca_is_basis() || Application.command()->m_varTopicQuery["version"] == "basis"))
      {

         pinteractionframe->set_style("BlueRedPurple");

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
      if(!m_workset.update(
         this,
         this,
         this,
         this))
      {
         pcreate->m_bRet = false;
         return;
      }

#if defined(LINUX) || defined(MACOS)

   m_workset.SetActiveFlag(TRUE);

#endif


   }

   defer_synch_layered();



   pcreate->m_bRet = false;

}

uint32_t simple_frame_window_save_window_rect(void * pvoidParam)
{

   sp(simple_frame_window) pframe = (sp(::simple_frame_window)) pvoidParam;

   pframe->WindowDataSaveWindowRect();

   return 0;

}


void simple_frame_window::_001OnDisplayChange(signal_details * pobj)
{

   SCAST_PTR(::ca2::message::base, pbase, pobj);

   post_simple_command(simple_command_load_window_rect, (LPARAM) FALSE);

   pobj->m_bRet = true;

   pbase->set_lresult(0);

   Default();

}

void simple_frame_window::_001OnSize(signal_details * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
   if((m_workset.GetMovingManager() == NULL ||
      m_workset.GetSizingManager() == NULL) || (!m_workset.GetMovingManager()->IsMoving()
   && !m_workset.GetSizingManager()->IsSizing()))
   {
      _001RedrawWindow();
   }

   if(does_display_match() && dynamic_cast < simple_child_frame * > (this) == NULL)
   {
      __begin_thread(get_app(), ::simple_frame_window_save_window_rect, this);
   }

}


void simple_frame_window::_001OnMove(signal_details * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
   if((m_workset.GetMovingManager() == NULL ||
      m_workset.GetSizingManager() == NULL) || (!m_workset.GetMovingManager()->IsMoving()
   && !m_workset.GetSizingManager()->IsSizing()))
   {
      _001RedrawWindow();
   }



   if(does_display_match() && dynamic_cast < simple_child_frame * > (this) == NULL)
   {
      __begin_thread(get_app(), ::simple_frame_window_save_window_rect, this);

   }

}

bool simple_frame_window::OnCreateClient(LPCREATESTRUCT lpcs, sp(create_context) pContext)
{
// trans   HICON hicon = GetIcon(false);
   return ::user::frame_window::OnCreateClient(lpcs, pContext);
}

bool simple_frame_window::pre_create_window(CREATESTRUCT& cs)
{

   if(!::user::frame_window::pre_create_window(cs))
      return FALSE;


   //cs.style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;
   cs.style = WS_POPUP;
   cs.style &= ~WS_VISIBLE;

   return TRUE;
}

void simple_frame_window::layout()
{

   if(m_bWindowFrame && m_workset.IsAppearanceEnabled() && !WfiIsFullScreen())
   {
      m_workset.layout();
   }
   else
   {
      ::user::frame_window::layout();
   }

}

void simple_frame_window::ViewOnActivateFrame(sp(::user::view) pview, UINT user, sp(::user::interaction) pframe)
{
   UNREFERENCED_PARAMETER(pview);
   UNREFERENCED_PARAMETER(user);
   UNREFERENCED_PARAMETER(pframe);
//   if(pview != NULL)
//      pview->OnActivateFrame(WA_INACTIVE, (sp(::user::frame_window)) pframe);
}

void simple_frame_window::_001OnGetMinMaxInfo(signal_details * pobj)
{
#ifdef WINDOWSEX
   SCAST_PTR(::ca2::message::base, pbase, pobj);
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

   void simple_frame_window::ShowControlBars(bool bShow)
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


      POSITION pos = m_toolbarmap.get_start_position();

      id idKey;
      sp(::user::interaction) pbar;
      while(pos != NULL)
      {
          m_toolbarmap.get_next_assoc(pos, idKey, pbar);
          try
          {
             pbar->ShowWindow(nShow);
          }
          catch(...)
          {
          }
      }

      layout();

   }

void simple_frame_window::WfiOnFullScreen(bool bFullScreen)
{
   /*oswindow oswindowTrayWindow = NULL;
   if(get_parent() == NULL)
   {
      oswindowTrayWindow = FindWindowA("Shell_traywnd", "");
   }*/
   if(bFullScreen)
   {

      ShowControlBars(false);

  //    uint32_t dwStyle = GetStyle();

//      uint32_t dwStyleEx = GetExStyle();

      rect rectDesktop;

      if(get_parent() != NULL)
      {
         get_parent()->GetClientRect(rectDesktop);
      }
      else
      {
         System.get_screen_rect(rectDesktop);
         /*::AdjustWindowRectEx(
            &rectDesktop,
            dwStyle,
            FALSE,
            dwStyleEx);*/
      }

      m_FullScreenWindowRect = rectDesktop;

      //if(get_parent() == NULL)
      {
         SetWindowPos(
            ZORDER_TOP,
            rectDesktop.left, rectDesktop.top,
            rectDesktop.width(), rectDesktop.height(),
            SWP_FRAMECHANGED |
            SWP_SHOWWINDOW);

         //::SetWindowPos(oswindowTrayWindow, 0, 0, 0, 0, 0, SWP_HIDEWINDOW);
      }
   }
   else
   {
      ShowControlBars(true);
   }
}


void simple_frame_window::_001OnViewFullScreen(signal_details * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
   ToggleFullScreen();
}

void simple_frame_window::_001OnMouseMove(signal_details * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
//   SCAST_PTR(::ca2::message::mouse, pmouse, pobj)
}

void simple_frame_window::_001OnUpdateViewFullScreen(signal_details * pobj)
{
   SCAST_PTR(base_cmd_ui, pcmdui, pobj)
   pcmdui->m_pcmdui->Enable();
   pcmdui->m_pcmdui->_001SetCheck(WfiIsFullScreen());
   pcmdui->m_bRet = true;
}

void simple_frame_window::ToggleFullScreen()
{
   WfiFullScreen(!WfiIsFullScreen(), true);
}

bool simple_frame_window::_001CanEnterScreenSaver()
{
   return true;
}

void simple_frame_window::_001OnSysCommand(signal_details * pobj)
{
   SCAST_PTR(::ca2::message::base, pbase, pobj);

   if(pbase->m_wparam == SC_SCREENSAVE)
   {
      if(!_001CanEnterScreenSaver())
      {
         pbase->m_bRet = true;
         pbase->set_lresult(0);
         return;
      }
   }

   if(m_bWindowFrame)
   {
      if(pbase->m_wparam == SC_MAXIMIZE)
      {
         WfiMaximize();
         pbase->m_bRet = true;
         pbase->set_lresult(0);
      }
      else if(pbase->m_wparam == SC_RESTORE)
      {
         WfiRestore();
         pbase->m_bRet = true;
         pbase->set_lresult(0);
      }
      else if(pbase->m_wparam == SC_MINIMIZE)
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
   SCAST_PTR(base_cmd_ui, pcmdui, pobj)
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

void simple_frame_window::SetBorderRect(LPCRECT lpcrect)
{
   m_rectBorder = *lpcrect;
}

void simple_frame_window::SetCustomFrame(bool bCustom)
{
   m_bWindowFrame = bCustom;
   m_workset.Enable(bCustom);
   layout();
   _001RedrawWindow();

}

bool simple_frame_window::GetCustomFrame()
{
   return m_bWindowFrame;
}

void simple_frame_window::_001OnClose(signal_details * pobj)
{
   if(m_iModalCount > 0)
   {
      EndModalLoop(IDOK);
      pobj->m_bRet = true;
      return;
   }
   else if(GetTopLevelFrame() != NULL
   && (GetTopLevelFrame()->m_iModalCount > 0))
   {
      GetTopLevelFrame()->EndModalLoop(IDOK);
      pobj->m_bRet = true;
      return;
   }
   else if(m_pimpl->m_iModalCount > 0)
   {
      m_pimpl->EndModalLoop(IDOK);
      pobj->m_bRet = true;
      ShowWindow(SW_HIDE);
      return;
   }
   else if(GetTopLevelFrame() != NULL
   && GetTopLevelFrame()->m_pimpl != NULL
   && (GetTopLevelFrame()->m_pimpl->m_iModalCount > 0))
   {
      GetTopLevelFrame()->m_pimpl->EndModalLoop(IDOK);
      pobj->m_bRet = true;
      return;
   }

   if(get_wnd() != NULL
   && get_wnd()->m_iModalCount > 0)
   {
      get_wnd()->EndModalLoop(IDOK);
      pobj->m_bRet = true;
      return;
   }

   pobj->m_bRet = true;
   // Note: only queries the active document
   sp(::user::document_interface) pdocument = GetActiveDocument();
   if (pdocument != NULL && !pdocument->can_close_frame(this))
   {
      // document can't close right now -- don't close it
      return;
   }

   sp(::plane::application) papp = &Application;

   if(papp->is_system() || papp->is_session())
   {

      // TODO: instead of closing all applications in process System.m_apptra, should close application that make part of
      // cube, bergedge, session or system.

      for(int32_t i = 0; Sys(papp).m_appptra.get_count(); i++)
      {

         sp(::plane::application) pappChild = &App(Sys(papp).m_appptra(i).m_p);

         if(!pappChild->_001CloseApplicationByUser(this))
            return;

      }

   }
   else if(papp->user()->GetVisibleTopLevelFrameCountExcept(this) <= 0)
   {

      if(!papp->_001CloseApplicationByUser(this))
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
   SCAST_PTR(::ca2::message::nc_activate, pncactivate, pobj)
   // stay active if WF_STAYACTIVE bit is on
   if (m_nFlags & WF_STAYACTIVE)
      pncactivate->m_bActive = TRUE;

   // but do not stay active if the ::ca2::window is disabled
   if (!IsWindowEnabled())
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
      pncactivate->set_lresult(DefWindowProc(WM_NCACTIVATE, pncactivate->m_bActive, 0L)); // bStop
      pncactivate->m_bRet = pncactivate->get_lresult() != 0;
      return;
   }
}



bool simple_frame_window::LoadFrame(const char * pszMatter, uint32_t dwDefaultStyle, sp(::user::interaction) pParentWnd, sp(create_context) pContext)
{

   UNREFERENCED_PARAMETER(pParentWnd);

   Application.defer_initialize_twf();

   // only do this once
//   ASSERT_VALID_IDR(nIDResource);
//   ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

   m_strMatterHelp = pszMatter;    // ID for help context (+HID_BASE_RESOURCE)

//   string strFullString;
//   if (strFullString.load_string(nIDResource))
//      __extract_sub_string(m_strTitle, strFullString, 0);    // first sub-string

   const char * lpszTitle = m_strTitle;

   if(pParentWnd == NULL)
      pParentWnd = Application.get_request_parent_ui(this, pContext);

   dwDefaultStyle &= ~WS_VISIBLE;

   if (!CreateEx(0L, NULL, lpszTitle, dwDefaultStyle, rect(0, 0, 0, 0), pParentWnd, /*nIDResource*/ 0, pContext))
   {
      return FALSE;   // will self destruct on failure normally
   }

/* trans   // save the default menu handle
   ASSERT(get_handle() != NULL);
   m_hMenuDefault = ::GetMenu(get_handle()); */

   // load accelerator resource
//   LoadAccelTable(MAKEINTRESOURCE(nIDResource));

   if (pContext == NULL)   // send initial update
      SendMessageToDescendants(WM_INITIALUPDATE, 0, (LPARAM) 0, TRUE, TRUE);

   return TRUE;
}




void simple_frame_window::_001OnDdeInitiate(signal_details * pobj)
{
   SCAST_PTR(::ca2::message::base, pbase, pobj)
   pbase->set_lresult(DefWindowProc((uint32_t) pbase->m_wparam, pbase->m_lparam, pbase->get_lresult()));
}


void simple_frame_window::pre_translate_message(signal_details * pobj)
{
#ifdef WINDOWSEX
   SCAST_PTR(::ca2::message::base, pbase, pobj);
   if(pbase->m_uiMessage == WM_KEYDOWN)
   {

      SCAST_PTR(::ca2::message::key, pkey, pobj);

      if(pkey->m_ekey == ::user::key_alt)
      {
         m_bFullScreenAlt = false;
      }
      else if(pkey->m_ekey == ::user::key_return)
      {
         if(Application.is_key_pressed(::user::key_control)
         && Application.is_key_pressed(::user::key_alt))
         {
            m_bFullScreenAlt = true;
            if(!IsFullScreen())
            {
               if(DeferFullScreen(true, false))
               {
                  pbase->m_bRet = true;
                  return;
               }
            }
         }
      }
   }
   else if(pbase->m_uiMessage == WM_KEYUP)
   {

      SCAST_PTR(::ca2::message::key, pkey, pobj);

      if(pkey->m_ekey == ::user::key_alt)
      {
         if(IsFullScreen()
         && Application.is_key_pressed(::user::key_control)
         && !m_bFullScreenAlt)
         {
            if(DeferFullScreen(false, true))
            {
               pbase->m_bRet = true;
               return;
            }
         }
      }
   }
#else
   //throw todo(get_app());
#endif
   return ::user::frame_window::pre_translate_message(pobj);
}







void simple_frame_window::InitialFramePosition(bool bForceRestore)
{

   if(m_bFrameMoveEnable)
   {

      WindowDataLoadWindowRect(bForceRestore);

      WindowDataEnableSaveWindowRect(true);

   }

   ::user::frame_window::InitialFramePosition(bForceRestore);

   if(m_workset.GetAppearance() != NULL && m_workset.GetAppearanceMode() == ::uinteraction::frame::AppearanceModeIconic)
   {

      WfiRestore();

   }

}


void simple_frame_window::_001OnDeferPaintLayeredWindowBackground(::draw2d::graphics * pdc)
{
   if(System.savings().is_trying_to_save(::ca2::resource_processing)
   || System.savings().is_trying_to_save(::ca2::resource_translucent_background))
   {
      rect rectClient;
      GetClientRect(rectClient);
      pdc->FillSolidRect(rectClient, RGB(0, 0, 0));
   }
   else
   {

      ::user::frame_window::_001OnDeferPaintLayeredWindowBackground(pdc);
   }
}

void simple_frame_window::_000OnDraw(::draw2d::graphics * pdc)
{
   if(!m_bVisible)
      return;
#if !defined(LINUX) && !defined(MACOS)
   if(m_bblur_Background)
   {
      _001DrawThis(pdc);
      _001DrawChildren(pdc);
   }
   else
#endif
   if(!Session.savings().is_trying_to_save(::ca2::resource_processing)
   && !Session.savings().is_trying_to_save(::ca2::resource_display_bandwidth)
   && !Session.savings().is_trying_to_save(::ca2::resource_memory))
   //&& (get_parent() != NULL || (this->GetExStyle() & WS_EX_LAYERED) != 0))
   {
      #if TEST

      pdc->FillSolidRect(60, 10, 50, 50, ARGB(128, 184, 177, 84));
      #endif

      ::user::frame_window::_000OnDraw(pdc);
      #if TEST

      pdc->FillSolidRect(10, 60, 50, 50, ARGB(128, 255, 248, 84));
      #endif
   }
   else
   {
      #if TEST

      pdc->FillSolidRect(60, 10, 50, 50, ARGB(128, 184, 177, 84));
      #endif

      _001DrawThis(pdc);
      _001DrawChildren(pdc);
      #if TEST

      pdc->FillSolidRect(10, 60, 50, 50, ARGB(128, 184, 177, 84));
      #endif
   }
}


void simple_frame_window::_001OnDraw(::draw2d::graphics * pdc)
{

   single_lock sl(m_pmutex, true);

#if !defined(LINUX) && !defined(MACOS)

   if(m_bblur_Background)
   {
      class imaging & imaging = System.visual().imaging();
      rect rectClient;
      GetClientRect(rectClient);
      //rectClient.offset(rectClient.top_left());
      if(System.savings().is_trying_to_save(::ca2::resource_translucent_background))
      {
         //pdc->FillSolidRect(rectClient, RGB(150, 220, 140));
      }
      else if(System.savings().is_trying_to_save(::ca2::resource_processing)
      || System.savings().is_trying_to_save(::ca2::resource_blur_background))
      {
         imaging.color_blend(pdc, rectClient, RGB(150, 180, 140), 150);
      }
      else
      {
         if(rectClient.size() != m_dibBk->size())
         {
            m_dibBk->create(rectClient.size());
            m_dibBk->Fill(0, 184, 184, 170);
            //HMODULE hmodule = ::LoadLibrary("ca2performance.dll");
            //::visual::fastblur *( *pfnNew )(sp(base_application)) = (::visual::fastblur *(*)(sp(base_application))) ::GetProcAddress(hmodule, "new_fastblur");
            m_fastblur.create(allocer());
            m_fastblur.initialize(rectClient.size(), 2);
         }
         if(m_fastblur.is_set() && m_fastblur->area() > 0)
         {
            m_fastblur->get_graphics()->BitBlt(0, 0, rectClient.width(), rectClient.height(), pdc, 0, 0, SRCCOPY);
            m_fastblur.blur();
            imaging.bitmap_blend(
               m_fastblur->get_graphics(),
               null_point(),
               rectClient.size(),
               m_dibBk->get_graphics(),
               null_point(),
               49);
            pdc->from(rectClient.size(),
               m_fastblur->get_graphics(),
               null_point(),
               SRCCOPY);
         }
      }
      pdc->SelectClipRgn(NULL);
   }

#endif

   ::user::frame_window::_001OnDraw(pdc);

}


void simple_frame_window::on_set_parent(sp(::user::interaction) pguieParent)
{
   ::user::frame_window::on_set_parent(pguieParent);
   if(m_pupdowntarget != NULL && m_pupdowntarget->is_up_down_target())
   {
      // an updowntarget always show the frame for upping/downing
      if(!m_bWindowFrame)
      {
         m_bWindowFrame = m_bCustomFrameBefore;
      }
      if(!m_workset.IsEnabled())
      {
         m_workset.Enable(true);
         layout();
      }
   }
   else
   {
      if(pguieParent != NULL)
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

void simple_frame_window::GetClientRect(LPRECT lprect)
{
   if(m_bWindowFrame && m_pframeschema != NULL && !WfiIsFullScreen())
   {
      m_pframeschema->GetWndClientRect(lprect);
   }
   else
   {
      ::user::frame_window::GetClientRect(lprect);
   }
}


bool simple_frame_window::is_application_main_window()
{
   return Application.GetMainWnd() == this;
}


void simple_frame_window::LoadToolBar(id idToolBar, const char * pszToolBar, uint32_t dwCtrlStyle, uint32_t dwStyle)
{
   sp(::user::interaction) pui = m_toolbarmap[idToolBar];
   simple_toolbar * ptoolbar;
   if(pui != NULL)
   {
      ptoolbar = dynamic_cast < simple_toolbar * > (pui.m_p);
   }
   else
   {
      ptoolbar = new simple_toolbar(get_app());
      ptoolbar->CreateEx(this, dwCtrlStyle, dwStyle);
      pui = ptoolbar;
      m_toolbarmap.set_at(idToolBar, pui);
   }
   if(ptoolbar != NULL)
   {
      ptoolbar->LoadXmlToolBar(Application.file().as_string(Application.dir().matter(pszToolBar)));
      layout();
   }
}


void simple_frame_window::_001OnUser184(signal_details * pobj)
{
   SCAST_PTR(::ca2::message::base, pbase, pobj);
   if(pbase->m_wparam == 0 &&
      pbase->m_lparam == 0)
   {
      InitialFramePosition(true);
      pbase->m_bRet = true;
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


bool simple_frame_window::create(const char * lpszClassName,
         const char * lpszWindowName,
         uint32_t dwStyle,
         const RECT& rect,
         sp(::user::interaction) pParentWnd,        // != NULL for popups
         const char * lpszMenuName,
         uint32_t dwExStyle,
         sp(create_context) pContext)
{
   return ::user::frame_window::create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
}





