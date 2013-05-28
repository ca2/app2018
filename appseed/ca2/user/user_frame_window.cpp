#include "framework.h"

#ifdef WINDOWS
#include <dde.h>        // for DDE execute shell requests
#endif


namespace user
{


   /////////////////////////////////////////////////////////////////////////////
   // frame_window

   // // BEGIN_MESSAGE_MAP(frame_window, user::frame_window_interface)
   //{{__MSG_MAP(frame_window)
   /* xxx   ON_WM_INITMENU()
   ON_WM_INITMENUPOPUP()
   ON_WM_MENUSELECT()
   ON_MESSAGE(WM_POPMESSAGESTRING, &frame_window::OnPopMessageString)
   ON_MESSAGE(WM_SETMESSAGESTRING, &frame_window::OnSetMessageString)
   ON_MESSAGE(WM_HELPPROMPTADDR, &frame_window::OnHelpPromptAddr)
   ON_MESSAGE_VOID(WM_IDLEUPDATECMDUI, frame_window::OnIdleUpdateCmdUI)
   ON_WM_ENTERIDLE()
   ON_WM_HSCROLL()
   ON_WM_VSCROLL()
   ON_WM_SETFOCUS()

   ON_WM_DESTROY()
   ON_WM_CLOSE()
   ON_WM_SIZE()
   ON_WM_ERASEBKGND()
   ON_WM_ACTIVATE()
   ON_WM_NCACTIVATE()
   ON_WM_DROPFILES()
   ON_WM_QUERYENDSESSION()
   ON_WM_ENDSESSION()
   ON_WM_SETCURSOR()
   ON_WM_ENABLE()
   // OLE palette support
   ON_WM_QUERYNEWPALETTE()
   ON_WM_PALETTECHANGED()
   ON_MESSAGE(WM_COMMANDHELP, &frame_window::OnCommandHelp)
   ON_MESSAGE(WM_HELPHITTEST, &frame_window::OnHelpHitTest)
   ON_MESSAGE(WM_ACTIVATETOPLEVEL, &frame_window::OnActivateTopLevel)
   // turning on and off standard frame gadgetry
   ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, &frame_window::OnUpdateControlBarMenu)
   ON_COMMAND_EX(ID_VIEW_STATUS_BAR, &frame_window::OnBarCheck)
   ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, &frame_window::OnUpdateControlBarMenu)
   ON_COMMAND_EX(ID_VIEW_TOOLBAR, &frame_window::OnBarCheck)
   ON_UPDATE_COMMAND_UI(ID_VIEW_REBAR, &frame_window::OnUpdateControlBarMenu)
   ON_COMMAND_EX(ID_VIEW_REBAR, &frame_window::OnBarCheck)
   // turning on and off standard mode indicators
   ON_UPDATE_COMMAND_UI(ID_INDICATOR_CAPS, &frame_window::OnUpdateKeyIndicator)
   ON_UPDATE_COMMAND_UI(ID_INDICATOR_NUM, &frame_window::OnUpdateKeyIndicator)
   ON_UPDATE_COMMAND_UI(ID_INDICATOR_SCRL, &frame_window::OnUpdateKeyIndicator)
   ON_UPDATE_COMMAND_UI(ID_INDICATOR_KANA, &frame_window::OnUpdateKeyIndicator)
   // standard help handling
   ON_UPDATE_COMMAND_UI(ID_CONTEXT_HELP, &frame_window::OnUpdateContextHelp)
   // toolbar "tooltip" notification
   ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, &frame_window::OnToolTipText)
   ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, &frame_window::OnToolTipText)
   ON_NOTIFY_EX_RANGE(RBN_CHEVRONPUSHED, 0, 0xFFFF, &frame_window::OnChevronPushed)
   //}}__MSG_MAP
   // message handling for standard DDE commands
   ON_MESSAGE(WM_DDE_INITIATE, &frame_window::OnDDEInitiate)
   ON_MESSAGE(WM_DDE_EXECUTE, &frame_window::OnDDEExecute)
   ON_MESSAGE(WM_DDE_TERMINATE, &frame_window::OnDDETerminate)
   ON_REGISTERED_MESSAGE(gen_MsgMouseWheel, &frame_window::OnRegisteredMouseWheel) */
   // // END_MESSAGE_MAP()

   /////////////////////////////////////////////////////////////////////////////
   // frame_window construction/destruction

   frame_window::frame_window()
   {

      m_nWindow = -1;                 // unknown ::ca::window ID
      m_bAutoMenuEnable = TRUE;       // auto enable on by default
      m_lpfnCloseProc = ::null();
      m_hMenuDefault = ::null();
      m_hAccelTable = ::null();
      //m_nIDHelp = 0;
      m_nIDTracking = 0;
      m_nIDLastMessage = 0;

      m_cModalStack = 0;              // initialize modality support
      m_hMenuAlt = ::null();
      m_nIdleFlags = 0;               // no idle work at start
      m_rectBorder.null();

      m_bHelpMode = 0;    // not in Shift+F1 help mode
      m_dwPromptContext = 0;

      m_pNextFrameWnd = ::null();         // not in list yet

      m_bInRecalcLayout = FALSE;
      m_nShowDelay = -1;              // no delay pending

      m_bFrameMoveEnable = true;

      AddFrameWnd();

   }


   frame_window::~frame_window()
   {

      RemoveFrameWnd();

   }


   void frame_window::AddFrameWnd()
   {

   }


   void frame_window::RemoveFrameWnd()
   {

   }


   /////////////////////////////////////////////////////////////////////////////
   // Special processing etc

   bool frame_window::LoadAccelTable(const char * lpszResourceName)
   {
      ASSERT(m_hAccelTable == ::null());  // only do once
      ASSERT(lpszResourceName != ::null());

      /*   HINSTANCE hInst = ::ca::FindResourceHandle(lpszResourceName, RT_ACCELERATOR);
      m_hAccelTable = ::LoadAccelerators(hInst, lpszResourceName);*/
      return (m_hAccelTable != ::null());
   }

   HACCEL frame_window::GetDefaultAccelerator()
   {
      // use ::user::document_interface specific accelerator table over m_hAccelTable
      HACCEL hAccelTable = m_hAccelTable;
      HACCEL hAccel;
      sp(::user::document_interface) pDoc = GetActiveDocument();
      if (pDoc != ::null() && (hAccel = pDoc->GetDefaultAccelerator()) != ::null())
         hAccelTable = hAccel;

      return hAccelTable;
   }

   void frame_window::pre_translate_message(::ca::signal_object * pobj)
   {
      ENSURE_ARG(pobj != ::null());
      // check for special cancel modes for combo boxes
      //if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN)
      //   __cancel_modes(pMsg->oswindow);    // filter clicks

      // allow tooltip messages to be filtered
      ::user::frame_window_interface::pre_translate_message(pobj);
      if(pobj->m_bRet)
         return;


      /* trans if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
      {
      // finally, translate the message
      HACCEL hAccel = GetDefaultAccelerator();
      return hAccel != ::null() &&  ::TranslateAccelerator(get_handle(), hAccel, pMsg);
      }*/
   }

   void frame_window::PostNcDestroy()
   {
      ::user::interaction::PostNcDestroy();
      // default for frame windows is to allocate them on the heap
      //  the default post-cleanup is to 'delete this'.
      // never explicitly call 'delete' on a frame_window, use DestroyWindow instead
      //release();
   }

   void frame_window::OnPaletteChanged(sp(::user::interaction) pFocusWnd)
   {
      UNREFERENCED_PARAMETER(pFocusWnd);
      // trans user::frame_window_interface::OnPaletteChanged(pFocusWnd);
   }

   bool frame_window::OnQueryNewPalette()
   {
      // trans    return user::frame_window_interface::OnQueryNewPalette();
      return FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // frame_window support for context sensitive help.

   void frame_window::ExitHelpMode()
   {
      /* trans
      // if not in help mode currently, this is a no-op
      if (!m_bHelpMode)
      return;

      // only post new WM_EXITHELPMODE message if one doesn't already exist
      //  in the queue.
      MESSAGE msg;
      if (!::PeekMessage(&msg, get_handle(), WM_EXITHELPMODE, WM_EXITHELPMODE,
      PM_REMOVE|PM_NOYIELD))
      {
      VERIFY(::PostMessage(get_handle(), WM_EXITHELPMODE, 0, 0));
      }

      // release capture if this ::ca::window has it
      if (System.get_capture_uie() == get_handle())
      System.release_capture_uie();

      sp(::user::frame_window) pFrameWnd = GetTopLevelFrame();
      ENSURE_VALID(pFrameWnd);
      pFrameWnd->m_bHelpMode = m_bHelpMode = HELP_INACTIVE;
      PostMessage(WM_KICKIDLE);   // trigger idle update
      */
   }

   bool frame_window::OnSetCursor(sp(::user::interaction) pWnd, UINT nHitTest, UINT message)
   {
      UNREFERENCED_PARAMETER(pWnd);
      UNREFERENCED_PARAMETER(nHitTest);
      UNREFERENCED_PARAMETER(message);
      sp(::user::frame_window) pFrameWnd = GetTopLevelFrame();
      ENSURE_VALID(pFrameWnd);
      if (pFrameWnd->m_bHelpMode)
      {
         //      SetCursor(afxData.hcurHelp);
         return TRUE;
      }
      // trans   return user::frame_window_interface::OnSetCursor(pWnd, nHitTest, message);
      return TRUE;
   }

   /*LRESULT frame_window::OnCommandHelp(WPARAM, LPARAM lParam)
   {
   if (lParam == 0)
   {
   if (IsTracking())
   lParam = HID_BASE_COMMAND+m_nIDTracking;
   else
   lParam = HID_BASE_RESOURCE+m_nIDHelp;
   }
   if (lParam != 0)
   {
   application* pApp = &System;
   if (pApp != ::null())
   pApp->WinHelpInternal(lParam);
   return TRUE;
   }
   return FALSE;
   }*/

   /*LRESULT frame_window::OnHelpHitTest(WPARAM, LPARAM)
   {
   if (m_nIDHelp != 0)
   return HID_BASE_RESOURCE+m_nIDHelp;
   else
   return 0;
   }*/

   bool frame_window::OnCommand(WPARAM wParam, LPARAM lParam)
      // return TRUE if command invocation was attempted
   {
      //   ::oswindow oswindow_Ctrl = (::oswindow) lParam;
      //   UINT nID = LOWORD(wParam);

      sp(::user::frame_window) pFrameWnd = GetTopLevelFrame();
      ENSURE_VALID(pFrameWnd);
      /*   if (pFrameWnd->m_bHelpMode && oswindow_Ctrl == ::null() &&
      nID != ID_HELP && nID != ID_DEFAULT_HELP && nID != ID_CONTEXT_HELP)
      {
      // route as help
      if (!SendMessage(WM_COMMANDHELP, 0, HID_BASE_COMMAND+nID))
      SendMessage(WM_COMMAND, ID_DEFAULT_HELP);
      return TRUE;
      }
      */
      // route as normal command
      // trans   return user::frame_window_interface::OnCommand(wParam, lParam);
      return FALSE;
   }




   void frame_window::BeginModalState()
   {
      //   ASSERT(get_handle() != ::null());
      ASSERT(IsWindow());

      // allow stacking, but don't do anything
      if (++m_cModalStack > 1)
         return;

      // determine top-level parent, since that is the true parent of any
      //  modeless windows anyway...
      sp(::user::interaction) pParent = EnsureTopLevelParent();

      m_uiptraDisable.remove_all();

      // disable all windows connected to this frame (and add them to the list)
      sp(::user::interaction) oswindow = System.get_desktop_window()->GetWindow(GW_CHILD);

      while (oswindow != ::null())
      {

         if (oswindow->IsWindowEnabled() &&    ::user::is_descendant(pParent, oswindow) && oswindow->send_message(WM_DISABLEMODAL) == 0)
         {

            oswindow->EnableWindow(FALSE);

            m_uiptraDisable.add(oswindow);

         }

         oswindow = oswindow->GetWindow( GW_HWNDNEXT);

      }

   }

   void frame_window::EndModalState()
   {
      // pop one off the stack (don't undo modalness unless stack is down to zero)
      if (m_cModalStack == 0 || --m_cModalStack > 0)
         return;

      for(index nIndex = 0; nIndex < m_uiptraDisable.get_count(); nIndex++)
      {

         ASSERT(m_uiptraDisable[nIndex] != ::null());

         if (m_uiptraDisable[nIndex]->IsWindow())
            m_uiptraDisable[nIndex]->EnableWindow(TRUE);

      }

      m_uiptraDisable.remove_all();


   }

   void frame_window::ShowOwnedWindows(bool bShow)
   {
      UNREFERENCED_PARAMETER(bShow);
      // walk through all top-level windows
      throw not_implemented(get_app());
      /*   oswindow oswindow = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
      while (oswindow != ::null())
      {
      sp(::ca::window) pWnd = ::ca::window::FromHandlePermanent(oswindow);
      if (pWnd != ::null() && get_handle() != oswindow && __is_descendant(this, pWnd))
      {
      uint32_t dwStyle = ::GetWindowLong(oswindow, GWL_STYLE);
      if (!bShow && (dwStyle & (WS_VISIBLE|WS_DISABLED)) == WS_VISIBLE)
      {
      ::ShowWindow(oswindow, SW_HIDE);
      pWnd->m_nFlags |= WF_TEMPHIDE;
      }
      // don't show temporarily hidden windows if we're in print preview mode
      else if (bShow && (dwStyle & (WS_VISIBLE|WS_DISABLED)) == 0 &&
      (pWnd->m_nFlags & WF_TEMPHIDE) && !m_lpfnCloseProc)
      {
      ::ShowWindow(oswindow, SW_SHOWNOACTIVATE);
      pWnd->m_nFlags &= ~WF_TEMPHIDE;
      }
      }
      oswindow = ::GetWindow(oswindow, GW_HWNDNEXT);
      }*/
   }

   void frame_window::OnEnable(bool bEnable)
   {

      if (bEnable && (m_nFlags & WF_STAYDISABLED))
      {

         // Work around for MAPI support. This makes sure the main ::ca::window
         // remains disabled even when the mail system is booting.

         EnableWindow(FALSE);

#ifdef WINDOWSEX

         ::SetFocus(::null());
#else

         throw todo(get_app());

#endif

         return;

      }


      // this causes modal dialogs to be "truly modal"
      if (!bEnable && !InModalState())
      {
         ASSERT((m_nFlags & WF_MODALDISABLE) == 0);
         m_nFlags |= WF_MODALDISABLE;
         BeginModalState();
      }
      else if (bEnable && (m_nFlags & WF_MODALDISABLE))
      {
         m_nFlags &= ~WF_MODALDISABLE;
         EndModalState();

         // cause normal focus logic to kick in
         if (System.get_active_guie() == this)
            send_message(WM_ACTIVATE, WA_ACTIVE);
      }

      // force WM_NCACTIVATE because Windows may think it is unecessary
      if (bEnable && (m_nFlags & WF_STAYACTIVE))
         send_message(WM_NCACTIVATE, TRUE);
      // force WM_NCACTIVATE for floating windows too
   }

   /////////////////////////////////////////////////////////////////////////////
   // frame_window second phase creation

   bool frame_window::pre_create_window(CREATESTRUCT& cs)
   {

      if (cs.style & FWS_ADDTOTITLE)
         cs.style |= FWS_PREFIXTITLE;

#ifdef WINDOWS

      cs.dwExStyle |= WS_EX_CLIENTEDGE;

#endif

      cs.style &= ~WS_VISIBLE;

      return true;

   }

   bool frame_window::create(const char * lpszClassName, const char * lpszWindowName, uint32_t dwStyle, const RECT& rect, sp(::user::interaction) pParentWnd, const char * lpszMenuName, uint32_t dwExStyle, sp(::ca::create_context) pContext)
   {

      UNREFERENCED_PARAMETER(lpszMenuName);

      m_strTitle = lpszWindowName;    // save title for later

      if(!::user::interaction::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, id(), pContext))
      {

         TRACE(::ca::trace::category_AppMsg, 0, "Warning: failed to create frame_window.\n");

         return FALSE;

      }

      return TRUE;

   }

   /*
   sp(::user::interaction) frame_window::CreateView(sp(::ca::create_context) pContext, UINT nID)
   {
   // trans   ASSERT(get_handle() != ::null());
   ASSERT(IsWindow());
   ENSURE_ARG(pContext != ::null());
   ENSURE_ARG(pContext->m_typeinfoNewView != ::null());

   // Note: can be a ::user::interaction with PostNcDestroy self cleanup
   sp(::user::interaction) pview =  (System.alloc(pContext->m_typeinfoNewView));
   if (pview == ::null())
   {
   TRACE(::ca::trace::category_AppMsg, 0, "Warning: Dynamic create of ::user::view type %hs failed.\n",
   pContext->m_typeinfoNewView.name());
   return ::null();
   }
   ASSERT_KINDOF(::user::interaction, pview);

   // views are always created with a border!
   if (!pview->create(::null(), ::null(), __WS_DEFAULT_VIEW,
   rect(0,0,0,0), this, nID, pContext))
   {
   TRACE(::ca::trace::category_AppMsg, 0, "Warning: could not create ::user::view for frame.\n");
   return ::null();        // can't continue without a ::user::view
   }

   if (pview->GetExStyle() & WS_EX_CLIENTEDGE)
   {
   // remove the 3d style from the frame, since the ::user::view is
   //  providing it.
   // make sure to recalc the non-client area
   ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
   }
   return pview;
   }
   */

   bool frame_window::OnCreateClient(LPCREATESTRUCT, sp(::ca::create_context) pContext)
   {
      // default create client will create a ::user::view if asked for it
      if (pContext != ::null() &&
         (pContext->m_user->m_typeinfoNewView ||
         pContext->m_user->m_puiNew != ::null()))
      {
         if(::user::view::s_create_view(pContext, this, "pane_first") == ::null())
            return FALSE;
      }
      return TRUE;
   }

   void frame_window::_001OnCreate(::ca::signal_object * pobj)
   {

      ::ca::thread * pappthread = System.GetThread()->get_app_thread();
      if(pappthread != ::null())
      {
         if(pappthread->GetMainWnd() == ::null())
         {
            pappthread->SetMainWnd(this);
         }
      }
      if(Application.GetMainWnd() == ::null())
      {
         Application.SetMainWnd(this);
      }

      if(pobj->previous())
         return;

      SCAST_PTR(::ca::message::create, pcreate, pobj)
         ENSURE_ARG(pcreate->m_lpcreatestruct != ::null());
      sp(::ca::create_context) pContext = pcreate->m_lpcreatestruct->lpCreateParams;
      pcreate->set_lresult(OnCreateHelper(pcreate->m_lpcreatestruct, pContext));
      pcreate->m_bRet = pcreate->get_lresult() == -1;
   }

   int32_t frame_window::OnCreateHelper(LPCREATESTRUCT lpcs, sp(::ca::create_context) pContext)
   {

      // create special children first
      if (!OnCreateClient(lpcs, pContext))
      {
         TRACE(::ca::trace::category_AppMsg, 0, "Failed to create client pane/::user::view for frame.\n");
         return -1;
      }

      // post message for initial message string
      // trans   PostMessage(WM_SETMESSAGESTRING, __IDS_IDLEMESSAGE);

      // make sure the child windows have been properly sized
      //   layout();

      return 0;   // create ok
   }


   bool frame_window::LoadFrame(const char * pszMatter, uint32_t dwDefaultStyle,
      sp(::user::interaction) pParentWnd, sp(::ca::create_context) pContext)
   {
      UNREFERENCED_PARAMETER(pszMatter);
      UNREFERENCED_PARAMETER(dwDefaultStyle);
      UNREFERENCED_PARAMETER(pParentWnd);
      UNREFERENCED_PARAMETER(pContext);
      return FALSE;
      // only do this once
      //   ASSERT_VALID_IDR(nIDResource);
      //   ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

      /*m_nIDHelp = pszMatter;    // ID for help context (+HID_BASE_RESOURCE)

      string strFullString;
      if (strFullString.load_string(nIDResource))
      __extract_sub_string(m_strTitle, strFullString, 0);    // first sub-string

      VERIFY(__defer_register_class(__WNDFRAMEORVIEW_REG));

      // attempt to create the ::ca::window
      const char * lpszClass = GetIconWndClass(dwDefaultStyle, nIDResource);
      string strTitle = m_strTitle;
      if (!CreateEx(0, lpszClass, strTitle, dwDefaultStyle, rectDefault,
      pParentWnd, nIDResource, (LPVOID) pContext))
      {
      return FALSE;   // will self destruct on failure normally
      }

      // save the default menu handle
      //ASSERT(get_handle() != ::null());
      // trans m_hMenuDefault = ::GetMenu(get_handle());
      m_hMenuDefault = ::null(); // trans

      // load accelerator resource
      LoadAccelTable(MAKEINTRESOURCE(nIDResource));

      if (pContext == ::null())   // send initial update
      SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

      return TRUE;*/
   }

   void frame_window::OnUpdateFrameMenu(HMENU hMenuAlt)
   {
      if (hMenuAlt == ::null())
      {
         // attempt to get default menu from ::user::document_interface
         sp(::user::document_interface) pDoc = GetActiveDocument();
         if (pDoc != ::null())
            hMenuAlt = pDoc->GetDefaultMenu();
         // use default menu stored in frame if none from ::user::document_interface
         if (hMenuAlt == ::null())
            hMenuAlt = m_hMenuDefault;
      }
      // finally, set the menu
      // trans ::SetMenu(get_handle(), hMenuAlt);
   }

   void frame_window::InitialUpdateFrame(sp(::user::document_interface) pDoc, bool bMakeVisible)
   {
      // if the frame does not have an active ::user::view, set to first pane
      sp(::user::view) pview = ::null();
      if (GetActiveView() == ::null())
      {
         sp(::user::interaction) pWnd = GetDescendantWindow("pane_first");
         if (pWnd != ::null() && base < ::user::view > ::bases(pWnd))
         {
            pview =  pWnd;   
            SetActiveView(pview, FALSE);
         }
      }

      //   oswindow oswindow = get_handle();
      //   uint32_t dwStyle = ::GetWindowLong(oswindow, GWL_STYLE);
      //   bool bChild =  dwStyle & WS_CHILD;

      if (bMakeVisible)
      {

         if(dynamic_cast < ::user::place_holder * > (get_parent().m_p) == ::null())
         {

            InitialFramePosition();

         }

         // send initial update to all views (and other controls) in the frame
         SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

         // give ::user::view a chance to save the focus (CFormView needs this)
         if (pview != ::null())
            pview->OnActivateFrame(WA_INACTIVE, this);

         // finally, activate the frame
         // (send the default show command unless the main desktop ::ca::window)
         int32_t nCmdShow = -1;      // default
         ::ca::application* pApp = &System;
         if (pApp != ::null() && pApp->GetMainWnd() == this)
         {
            nCmdShow = pApp->m_nCmdShow; // use the parameter from WinMain
            pApp->m_nCmdShow = -1; // set to default after first time
         }
         ActivateFrame(nCmdShow);
         if (pview != ::null())
            pview->OnActivateView(TRUE, pview, pview);
      }

      // update frame counts and frame title (may already have been visible)
      if (pDoc != ::null())
         pDoc->update_frame_counts();
      on_update_frame_title(TRUE);

      sp(::user::place_holder) pholder = get_parent();
      if(pholder != ::null())
      {
         if(!oprop("should_not_be_automatically_holded_on_initial_update_frame").is_set()
            || !oprop("should_not_be_automatically_holded_on_initial_update_frame"))
         {
            pholder->hold(this);
            pholder->layout();
         }
      }


   }

   void frame_window::InitialFramePosition(bool bForceRestore)
   {
      UNREFERENCED_PARAMETER(bForceRestore);
      if(m_bFrameMoveEnable)
      {
         rect rectWindow;
         GetWindowRect(rectWindow);
         rect rectDesktop;
         if(get_parent() != ::null())
         {
            get_parent()->GetClientRect(rectDesktop);
            get_parent()->ScreenToClient(rectWindow);
         }
         else
         {
            System.get_screen_rect(rectDesktop);
         }
         if(!rectDesktop.contains(rectWindow)
            ||  rectWindow.width() < 100
            ||  rectWindow.height() < 100)
         {
            SetWindowPos(
               -3,
               rectDesktop.left + rectDesktop.width() / 7,
               rectDesktop.top + rectDesktop.height() / 7,
               rectDesktop.width() * 2 / 5,
               rectDesktop.height() * 2 / 5,
               0);
         }
         else
         {
            SetWindowPos(
               -3,
               rectWindow.left,
               rectWindow.top,
               rectWindow.width(),
               rectWindow.height(),
               0);
         }
      }
      ActivateTopParent();
      ActivateFrame();
      //BringWindowToTop();
   }

   /////////////////////////////////////////////////////////////////////////////
   // frame_window closing down

   void frame_window::OnClose()
   {
      throw not_implemented(get_app());
      /*if (m_lpfnCloseProc != ::null())
      (*m_lpfnCloseProc)(this);

      // Note: only queries the active ::user::document_interface
      sp(::user::document_interface) pdocument = GetActiveDocument();
      if (pdocument != ::null() && !pdocument->can_close_frame(this))
      {
      // ::user::document_interface can't close right now -- don't close it
      return;
      }
      application* pApp = &System;
      if (pApp != ::null() && pApp->GetMainWnd() == this)
      {
      // attempt to save all documents
      if (pdocument == ::null() && !pApp->save_all_modified())
      return;     // don't close it

      // hide the application's windows before closing all the documents
      pApp->HideApplication();

      // close all documents first
      pApp->close_all_documents(FALSE);


      // there are cases where destroying the documents may destroy the
      //  main ::ca::window of the application.
      if (!afxContextIsDLL && pApp->GetMainWnd() == ::null())
      {
      __post_quit_message(0);
      return;
      }
      }

      // detect the case that this is the last frame on the ::user::document_interface and
      // shut down with on_close_document instead.
      if (pdocument != ::null() && pdocument->m_bAutoDelete)
      {
      bool bOtherFrame = FALSE;
      POSITION pos = pdocument->get_view_count();
      while (pos != ::null())
      {
      sp(::user::view) pview = pdocument->get_view(pos);
      ENSURE_VALID(pview);
      if (pview->GetParentFrame() != this)
      {
      bOtherFrame = TRUE;
      break;
      }
      }
      if (!bOtherFrame)
      {
      pdocument->on_close_document();
      return;
      }

      // allow the ::user::document_interface to cleanup before the ::ca::window is destroyed
      pdocument->pre_close_frame(this);
      }

      // then destroy the ::ca::window
      DestroyWindow();*/
   }

   void frame_window::_001OnDestroy(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      // reset menu to default before final shutdown
      /* trans   if (m_hMenuDefault != ::null() && ::GetMenu(get_handle()) != m_hMenuDefault)
      {
      ::SetMenu(get_handle(), m_hMenuDefault);
      ASSERT(::GetMenu(get_handle()) == m_hMenuDefault);
      } */

      // Automatically quit when the main ::ca::window is destroyed.
      /* trans application* pApp = &System;
      if (pApp != ::null() && pApp->GetMainWnd() == this && pApp->m_eHelpType == afxWinHelp)
      {
      // closing the main application ::ca::window
      ::WinHelp(get_handle(), ::null(), HELP_QUIT, 0L);

      // will call PostQuitMessage in user::frame_window_interface::OnNcDestroy
      }*/
      //   user::frame_window_interface::OnDestroy();
   }


   /////////////////////////////////////////////////////////////////////////////
   // frame_window command/message routing
   void frame_window::install_message_handling(::ca::message::dispatch * pinterface)
   {
      ::user::interaction::install_message_handling(pinterface);
      frame_window_interface::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_DESTROY, pinterface, this, &frame_window::_001OnDestroy);
      IGUI_WIN_MSG_LINK(WM_SYSCOMMAND, pinterface, this, &frame_window::_001OnSysCommand);
      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &frame_window::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &frame_window::_001OnSize);
      IGUI_WIN_MSG_LINK(WM_IDLEUPDATECMDUI, pinterface, this, &frame_window::_001OnIdleUpdateCmdUI);
      IGUI_WIN_MSG_LINK(WM_SETFOCUS, pinterface, this, &frame_window::_001OnSetFocus);
      IGUI_WIN_MSG_LINK(WM_ACTIVATE, pinterface, this, &frame_window::_001OnActivate);
      IGUI_WIN_MSG_LINK(WM_NCACTIVATE, pinterface, this, &frame_window::_001OnNcActivate);
      IGUI_WIN_MSG_LINK(WM_QUERYENDSESSION, pinterface, this, &frame_window::_001OnQueryEndSession);

   }


   bool frame_window::_001OnCmdMsg(BaseCmdMsg * pcmdmsg)
   {

      if(m_workset._001OnCmdMsg(pcmdmsg))
         return true;

      // pump through current ::user::view FIRST
      sp(::user::view) pview = GetActiveView();
      if (pview != ::null() && pview->_001OnCmdMsg(pcmdmsg))
         return TRUE;

      pview =  get_child_by_id("pane_first");
      if (pview != ::null() && pview->_001OnCmdMsg(pcmdmsg))
         return TRUE;

      // then pump through frame
      if (::user::frame_window_interface::_001OnCmdMsg(pcmdmsg))
         return TRUE;

      // then pump through parent
      sp(::user::interaction) puiParent = get_parent();
      if(puiParent != ::null() && puiParent->_001OnCmdMsg(pcmdmsg))
         return TRUE;

      // last but not least, pump through cast
      ::ca::application* pApp = get_app();
      if (pApp != ::null() && pApp->_001OnCmdMsg(pcmdmsg))
         return TRUE;

      sp(command_target_interface) pcommandtargetinterface = Application.user()->get_keyboard_focus();

      if(pcommandtargetinterface != ::null())
      {
         if(pcommandtargetinterface->_001OnCmdMsg(pcmdmsg))
            return TRUE;
      }

      return FALSE;
   }




   // Delegate scroll messages to active ::user::view as well
   void frame_window::OnHScroll(UINT, UINT, CScrollBar*)
   {
      sp(::user::interaction) pActiveView = GetActiveView();
      if (pActiveView != ::null())
      {
         // trans const MESSAGE* pMsg = GetCurrentMessage();
         // trans pActiveView->SendMessage(WM_HSCROLL, pMsg->wParam, pMsg->lParam);
      }
   }

   void frame_window::OnVScroll(UINT, UINT, CScrollBar*)
   {
      sp(::user::interaction) pActiveView = GetActiveView();
      if (pActiveView != ::null())
      {
         // trans      const MESSAGE* pMsg = GetCurrentMessage();
         // trans      pActiveView->SendMessage(WM_VSCROLL, pMsg->wParam, pMsg->lParam);
      }
   }

   LRESULT frame_window::OnActivateTopLevel(WPARAM wParam, LPARAM lParam)
   {
      UNREFERENCED_PARAMETER(wParam);
      UNREFERENCED_PARAMETER(lParam);
      // trans   user::frame_window_interface::OnActivateTopLevel(wParam, lParam);

      // exit Shift+F1 help mode on activation changes
      ExitHelpMode();


      // deactivate current active ::user::view
      ::ca::thread *pThread = System.GetThread();
      ASSERT(pThread);
      if (pThread->GetMainWnd() == this)
      {
         sp(::user::view) pActiveView = GetActiveView();
         if (pActiveView == ::null())
            pActiveView = GetActiveFrame()->GetActiveView();
         if (pActiveView != ::null())
            pActiveView->OnActivateView(FALSE, pActiveView, pActiveView);
      }

      // force idle processing to update any key state indicators
      // trans   PostMessage(WM_KICKIDLE);

      return 0;
   }

   void frame_window::_001OnActivate(::ca::signal_object * pobj)
   {
      SCAST_PTR(::ca::message::activate, pactivate, pobj);

      sp(::user::interaction) pActive = (pactivate->m_nState == WA_INACTIVE ? pactivate->m_pWndOther.m_p : this);

      pobj->previous();

      // get top level frame unless this is a child ::ca::window
      // determine if ::ca::window should be active or not
      sp(::user::frame_window) pTopLevel = (GetStyle() & WS_CHILD) ? this : GetTopLevelFrame().m_p;

      if(pTopLevel == ::null())
         pTopLevel = this;



      bool bStayActive =
         (pTopLevel == pActive ||
         (pActive && pTopLevel == pActive->GetTopLevelFrame() &&
         (pActive == pTopLevel ||
         (pActive && pActive->send_message(WM_FLOATSTATUS, FS_SYNCACTIVE) != 0))));
      pTopLevel->m_nFlags &= ~WF_STAYACTIVE;
      if (bStayActive)
         pTopLevel->m_nFlags |= WF_STAYACTIVE;

      // sync floating windows to the new state
      NotifyFloatingWindows(bStayActive ? FS_ACTIVATE : FS_DEACTIVATE);

      // get active ::user::view (use active frame if no active ::user::view)
      sp(::user::view) pActiveView = GetActiveView();
      if (pActiveView == ::null())
         pActiveView =  (GetActiveFrame()->GetActiveView());

      // when frame gets activated, re-activate current ::user::view
      if (pActiveView != ::null())
      {
         if (pactivate->m_nState != WA_INACTIVE && !pactivate->m_bMinimized)
            pActiveView->OnActivateView(TRUE, pActiveView, pActiveView);

         // always notify the ::user::view of frame activations
         pActiveView->OnActivateFrame(pactivate->m_nState, this);
      }
      if(pactivate->m_nState == WA_ACTIVE)
      {
         _001RedrawWindow();
      }
   }

   void frame_window::_001OnNcActivate(::ca::signal_object * pobj)
   {
      SCAST_PTR(::ca::message::nc_activate, pncactivate, pobj)
         // stay active if WF_STAYACTIVE bit is on
         if (m_nFlags & WF_STAYACTIVE)
            pncactivate->m_bActive = TRUE;

      // but do not stay active if the ::ca::window is disabled
      if (!IsWindowEnabled())
         pncactivate->m_bActive = FALSE;

   }

   void frame_window::OnSysCommand(UINT nID, LPARAM lParam)
   {
      UNREFERENCED_PARAMETER(lParam);
      sp(::user::frame_window) pFrameWnd = GetTopLevelFrame();
      ENSURE_VALID(pFrameWnd);

      // set status bar as appropriate
      //   UINT nItemID = (nID & 0xFFF0);

      // don't interfere with system commands if not in help mode
      if (pFrameWnd->m_bHelpMode)
      {
         /*switch (nItemID)
         {
         case SC_SIZE:
         case SC_MOVE:
         case SC_MINIMIZE:
         case SC_MAXIMIZE:
         case SC_NEXTWINDOW:
         case SC_PREVWINDOW:
         case SC_CLOSE:
         case SC_RESTORE:
         case SC_TASKLIST:
         if (!SendMessage(WM_COMMANDHELP, 0,
         HID_BASE_COMMAND+ID_COMMAND_FROM_SC(nItemID)))
         SendMessage(WM_COMMAND, ID_DEFAULT_HELP);
         return;
         }*/
      }

      // call default functionality
      // trans   user::frame_window_interface::OnSysCommand(nID, lParam);
   }


#ifdef WINDOWSEX


   /////////////////////////////////////////////////////////////////////////////
   // default frame processing

   // default drop processing will attempt to open the file
   void frame_window::OnDropFiles(HDROP hDropInfo)
   {

      SetActiveWindow();      // activate us first !
      UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, ::null(), 0);

      ::user::user* puser = System.user();
      ASSERT(puser != ::null());
      for (UINT iFile = 0; iFile < nFiles; iFile++)
      {
         char szFileName[_MAX_PATH];
         ::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);

         sp(::ca::create_context) createcontext(allocer());
         createcontext->m_spCommandLine->m_varFile = szFileName;

         puser->open_document_file(createcontext);
      }
      ::DragFinish(hDropInfo);


   }

#endif

   // query end session for main frame will attempt to close it all down
   bool frame_window::OnQueryEndSession()
   {
      ::ca::application* pApp = &System;
      if (pApp != ::null() && pApp->GetMainWnd() == this)
         return pApp->save_all_modified();

      return TRUE;
   }

   // when Windows session ends, close all documents
   void frame_window::OnEndSession(bool bEnding)
   {
      if (!bEnding)
         return;

      ::ca::application* pApp = &System;
      if (pApp != ::null() && pApp->GetMainWnd() == this)
      {
         pApp->close_all_documents(TRUE);

         // allow application to save settings, etc.
         pApp->exit_instance();
      }
   }

   /////////////////////////////////////////////////////////////////////////////
   // Support for Shell DDE Execute messages

   LRESULT frame_window::OnDDEInitiate(WPARAM wParam, LPARAM lParam)
   {


#ifdef WINDOWSEX

      ::ca::application* pApp = &System;
      if (pApp != ::null() &&
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
         ::SendMessage((oswindow) wParam, WM_DDE_ACK, (WPARAM)get_handle(),
            MAKELPARAM(pApp->m_atomApp, pApp->m_atomSystemTopic));
      }

#else

      throw not_implemented(get_app());

#endif

      return 0L;
   }


   // always ACK the execute command - even if we do nothing
   LRESULT frame_window::OnDDEExecute(WPARAM wParam, LPARAM lParam)
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
      catch(memory_exception * pe)
      {
         GlobalUnlock(hData);
         pe->Delete();
      }


      // acknowledge now - before attempting to execute
      ::PostMessage((oswindow) wParam, WM_DDE_ACK, (WPARAM)get_handle(),
         //IA64: Assume DDE LPARAMs are still 32-bit
         ReuseDDElParam(lParam, WM_DDE_EXECUTE, WM_DDE_ACK,
         (UINT)0x8000, (uint_ptr)hData));

      // don't execute the command when the ::ca::window is disabled
      if (!IsWindowEnabled())
      {
         TRACE(::ca::trace::category_AppMsg, 0, "Warning: DDE command '%s' ignored because ::ca::window is disabled.\n",
            strCommand.GetString());
         return 0;
      }

      // execute the command
      LPTSTR lpszCommand = strCommand.GetBuffer();
      if (!System.OnDDECommand(lpszCommand))
         TRACE(::ca::trace::category_AppMsg, 0, "Error: failed to execute DDE command '%s'.\n", lpszCommand);
      strCommand.ReleaseBuffer();

#else

      throw not_implemented(get_app());

#endif

      return 0L;

   }

   LRESULT frame_window::OnDDETerminate(WPARAM wParam, LPARAM lParam)
   {

#ifdef WINDOWSEX

      ::PostMessage((oswindow) wParam, WM_DDE_TERMINATE, (WPARAM)get_handle(), lParam);

#else

      throw not_implemented(get_app());

#endif

      return 0L;

   }


   /////////////////////////////////////////////////////////////////////////////
   // frame_window attributes

   sp(::user::view) frame_window::GetActiveView() const
   {
      ASSERT(m_pViewActive == ::null() ||
         base < ::user::view >::bases(m_pViewActive));
      return m_pViewActive;
   }

   void frame_window::SetActiveView(sp(::user::view) pViewNew, bool bNotify)
   {
#ifdef DEBUG
      if (pViewNew != ::null())
      {
         //trans      ASSERT(IsChild(pViewNew));
         //trans      ASSERT_KINDOF(::user::view, pViewNew);
      }
#endif //DEBUG

      sp(::user::view) pViewOld = m_pViewActive;
      if (pViewNew == pViewOld)
         return;     // do not re-activate if SetActiveView called more than once

      m_pViewActive.release();   // no active for the following processing

      // deactivate the old one
      if (pViewOld != ::null())
         pViewOld->OnActivateView(FALSE, pViewNew, pViewOld);

      // if the OnActivateView moves the active ::ca::window,
      //    that will veto this change
      if (m_pViewActive != ::null())
         return;     // already set
      m_pViewActive = pViewNew;
      // activate
      if (pViewNew != ::null())
      {
         if(bNotify)
         {
            pViewNew->OnActivateView(TRUE, pViewNew, pViewOld);
         }
      }
   }



   /////////////////////////////////////////////////////////////////////////////
   // Special ::user::view swapping/activation

   void frame_window::OnSetFocus(sp(::user::interaction) pOldWnd)
   {
      UNREFERENCED_PARAMETER(pOldWnd);
      if (m_pViewActive != ::null())
         m_pViewActive->SetFocus();
      /*trans else
      user::frame_window_interface::OnSetFocus(pOldWnd);
      */
   }

   sp(::user::document_interface) frame_window::GetActiveDocument()
   {
      ASSERT_VALID(this);
      sp(::user::view) pview = GetActiveView();
      if (pview != ::null())
         return pview->get_document();
      return ::null();
   }

   /////////////////////////////////////////////////////////////////////////////
   // Command prompts


   void frame_window::GetMessageString(UINT nID, string & rMessage) const
   {
      UNREFERENCED_PARAMETER(nID);
      UNREFERENCED_PARAMETER(rMessage);
      // load appropriate string
      throw not_implemented(get_app());
      /*   LPTSTR lpsz = rMessage.GetBuffer(255);
      if (::ca::LoadString(nID, lpsz) != 0)
      {
      // first newline terminates actual string
      lpsz = _tcschr(lpsz, '\n');
      if (lpsz != ::null())
      *lpsz = '\0';
      }
      else
      {
      // not found
      TRACE(::ca::trace::category_AppMsg, 0, "Warning: no message line prompt for ID 0x%04X.\n", nID);
      }
      rMessage.ReleaseBuffer();*/
   }

   LRESULT frame_window::OnPopMessageString(WPARAM wParam, LPARAM lParam)
   {
      if (m_nFlags & WF_NOPOPMSG)
         return 0;

      return send_message(WM_SETMESSAGESTRING, wParam, lParam);
   }

   LRESULT frame_window::OnSetMessageString(WPARAM wParam, LPARAM lParam)
   {
      UINT nIDLast = m_nIDLastMessage;
      m_nFlags &= ~WF_NOPOPMSG;

      sp(::user::interaction) pMessageBar = GetMessageBar();
      if (pMessageBar != ::null())
      {
         const char * lpsz = ::null();
         string strMessage;

         // set the message bar text
         if (lParam != 0)
         {
            ASSERT(wParam == 0);    // can't have both an ID and a string
            lpsz = (const char *)lParam; // set an explicit string
         }
         else if (wParam != 0)
         {
            // map SC_CLOSE to PREVIEW_CLOSE when in print preview mode
            /*         if (wParam == __IDS_SCCLOSE && m_lpfnCloseProc != ::null())
            wParam = __IDS_PREVIEW_CLOSE;*/

            // get message associated with the ID indicated by wParam
            //NT64: Assume IDs are still 32-bit
            GetMessageString((UINT)wParam, strMessage);
            lpsz = strMessage;
         }
         pMessageBar->SetWindowText(lpsz);

         // update owner of the bar in terms of last message selected
         sp(::user::frame_window) pFrameWnd = pMessageBar->GetParentFrame();
         if (pFrameWnd != ::null())
         {
            pFrameWnd->m_nIDLastMessage = (UINT)wParam;
            pFrameWnd->m_nIDTracking = (UINT)wParam;
         }
      }

      m_nIDLastMessage = (UINT)wParam;    // new ID (or 0)
      m_nIDTracking = (UINT)wParam;       // so F1 on toolbar buttons work
      return nIDLast;
   }

   LRESULT frame_window::OnHelpPromptAddr(WPARAM, LPARAM)
   {
      return (LRESULT)&m_dwPromptContext;
   }

   sp(::user::interaction) frame_window::GetMessageBar()
   {
      return GetDescendantWindow("status_bar");
   }

   void frame_window::OnEnterIdle(UINT nWhy, sp(::user::interaction) pWho)
   {
      UNREFERENCED_PARAMETER(pWho);
      // trans user::frame_window_interface::OnEnterIdle(nWhy, pWho);
#ifdef WINDOWS
      if(nWhy != MSGF_MENU || m_nIDTracking == m_nIDLastMessage)
         return;
#else
      if(m_nIDTracking == m_nIDLastMessage)
         return;
#endif

      SetMessageText(m_nIDTracking);
      ASSERT(m_nIDTracking == m_nIDLastMessage);
   }

   void frame_window::SetMessageText(const char * lpszText)
   {
      send_message(WM_SETMESSAGESTRING, 0, (LPARAM)lpszText);
   }

   void frame_window::SetMessageText(UINT nID)
   {
      send_message(WM_SETMESSAGESTRING, (WPARAM)nID);
   }



   /////////////////////////////////////////////////////////////////////////////
   // Setting title of frame ::ca::window - UISG standard

   void frame_window::on_update_frame_title(bool bAddToTitle)
   {
      if ((GetStyle() & FWS_ADDTOTITLE) == 0)
         return;     // leave it alone!


      sp(::user::document_interface) pdocument = GetActiveDocument();
      if (bAddToTitle && pdocument != ::null())
         UpdateFrameTitleForDocument(pdocument->get_title());
      else
         UpdateFrameTitleForDocument(::null());
   }

   void frame_window::UpdateFrameTitleForDocument(const char * lpszDocName)
   {
      string WindowText;

      if (GetStyle() & FWS_PREFIXTITLE)
      {
         // get name of currently active ::user::view
         if (lpszDocName != ::null())
         {
            WindowText += lpszDocName;

            // add current ::ca::window # if needed
            if (m_nWindow > 0)
            {

               string strText;

               // :%d will produce a maximum of 11 TCHARs
               strText.Format(":%d", m_nWindow);

               WindowText += strText;

            }

            WindowText += " - ";

         }
         WindowText += m_strTitle;
      }
      else
      {
         // get name of currently active ::user::view
         WindowText += m_strTitle;
         if (lpszDocName != ::null())
         {
            WindowText += " - ";
            WindowText += lpszDocName;

            // add current ::ca::window # if needed
            if (m_nWindow > 0)
            {

               string strText;

               // :%d will produce a maximum of 11 TCHARs
               strText.Format(":%d", m_nWindow);

               WindowText += strText;

            }
         }
      }

      // set title if changed, but don't remove completely
      // Note: will be excessive for MDI Frame with maximized child
      SetWindowText(WindowText);
   }

   /////////////////////////////////////////////////////////////////////////////

   void frame_window::OnSetPreviewMode(bool bPreview, CPrintPreviewState* pState)
   {
      ENSURE_ARG(pState != ::null());
      // default implementation changes control bars, menu and main pane ::ca::window


      // set visibility of standard ControlBars (only the first 32)
      //   uint32_t dwOldStates = 0;

      if (bPreview)
      {
         // Entering Print Preview


         // show any modeless dialogs, popup windows, float tools, etc
         ShowOwnedWindows(FALSE);

         // Hide the main pane
      }
      else
      {
         // Leaving Preview
         m_lpfnCloseProc = ::null();

         // shift original "pane_first" back to its rightful ID
         /*      sp(::user::interaction) oswindow = get_child_by_id(__IDW_PANE_SAVE);
         if (oswindow != ::null())
         {
         sp(::user::interaction) oswindow_Temp = get_child_by_id("pane_first");
         if (oswindow_Temp != ::null())
         __set_dialog_control_id_(oswindow_Temp, __IDW_PANE_SAVE);
         __set_dialog_control_id_(oswindow, "pane_first");
         }*/

         layout();


         // show any modeless dialogs, popup windows, float tools, etc
         ShowOwnedWindows(TRUE);
      }
   }

   void frame_window::DelayUpdateFrameMenu(HMENU hMenuAlt)
   {
      m_hMenuAlt = hMenuAlt;
      m_nIdleFlags |= idleMenu;
   }

   void frame_window::OnIdleUpdateCmdUI()
   {
      // update menu if necessary
      if (m_nIdleFlags & idleMenu)
      {
         m_nIdleFlags &= ~idleMenu;
         OnUpdateFrameMenu(m_hMenuAlt);
      }

      // update title if necessary
      if (m_nIdleFlags & idleTitle)
         on_update_frame_title(TRUE);

      // recalc layout if necessary
      if (m_nIdleFlags & idleLayout)
      {
         layout();
         UpdateWindow();
      }

      // set the current message string if necessary
      if (m_nIDTracking != m_nIDLastMessage)
      {
         SetMessageText(m_nIDTracking);
         ASSERT(m_nIDTracking == m_nIDLastMessage);
      }
      m_nIdleFlags = 0;
   }

   sp(::user::frame_window) frame_window::GetActiveFrame()
   {
      // by default, the active frame is the frame itself (MDI is different)
      return this;
   }

   void frame_window::layout()
   {
      if (m_bInRecalcLayout)
         return;

      keeper < bool > keepInRecalcLayout(&m_bInRecalcLayout, true, false, true);
      // clear idle flags for recalc layout if called elsewhere
      //   if (m_nIdleFlags & idleNotify)
      //   bNotify = TRUE;
      m_nIdleFlags &= ~(idleLayout|idleNotify);


      // reposition all the child windows (regardless of ID)
      if (GetStyle() & FWS_SNAPTOBARS)
      {

         rect rect(0, 0, 32767, 32767);

         RepositionBars(0, 0xffff, "pane_first", reposQuery, &rect, &rect, FALSE);

         RepositionBars(0, 0xffff, "pane_first", reposExtra, &m_rectBorder, &rect, TRUE);

         CalcWindowRect(&rect);

         SetWindowPos(0, 0, 0, rect.width(), rect.height(), SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);

      }
      else
      {

         RepositionBars(0, 0xffff, "pane_first", reposExtra, &m_rectBorder);

      }


   }


   // frame_window implementation of OLE border space negotiation
   bool frame_window::NegotiateBorderSpace(UINT nBorderCmd, LPRECT lpRectBorder)
   {
      rect border, request;

      switch (nBorderCmd)
      {
      case borderGet:
         ASSERT(lpRectBorder != ::null());
         RepositionBars(0, 0xffff, "pane_first", reposQuery,
            lpRectBorder);
         break;

      case borderRequest:
         return TRUE;

      case borderSet:
         if (lpRectBorder == ::null())
         {
            if (!m_rectBorder.is_null())
            {
               // releasing all border space -- recalc needed
               m_rectBorder.null();
               return TRUE;
            }
            // original rect is is_empty & lpRectBorder is ::null(), no recalc needed
            return FALSE;
         }
         if (!::EqualRect(m_rectBorder, lpRectBorder))
         {
            // the rects are different -- recalc needed
            m_rectBorder.copy(lpRectBorder);
            return TRUE;
         }
         return FALSE;   // no recalc needed

      default:
         ASSERT(FALSE);  // invalid frame_window::BorderCmd
      }

      return TRUE;
   }

   void frame_window::OnSize(UINT nType, int32_t cx, int32_t cy)
   {
      UNREFERENCED_PARAMETER(cx);
      UNREFERENCED_PARAMETER(cy);
      // trans   user::frame_window_interface::OnSize(nType, cx, cy);    // important for MDI Children
      if (nType != SIZE_MINIMIZED)
         layout();
   }

   bool frame_window::OnEraseBkgnd(::ca::graphics * pgraphics)
   {
      UNREFERENCED_PARAMETER(pgraphics);
      if (m_pViewActive != ::null())
         return TRUE;        // active ::user::view will erase/paint itself
      // for ::user::view-less frame just use the default background fill
      return TRUE;
      //   return user::frame_window_interface::OnEraseBkgnd(pgraphics);
   }

   LRESULT frame_window::OnRegisteredMouseWheel(WPARAM wParam, LPARAM lParam)
   {

      LRESULT lResult = 0;

      // convert from MSH_MOUSEWHEEL to WM_MOUSEWHEEL

#ifdef WINDOWSEX

      WORD keyState = 0;
      keyState |= Application.is_key_pressed(::user::key_control) ? MK_CONTROL : 0;
      keyState |= Application.is_key_pressed(::user::key_shift) ? MK_SHIFT : 0;

      oswindow hwFocus = ::GetFocus();
      const oswindow hwDesktop = ::GetDesktopWindow();

      if (hwFocus == ::null())
         lResult = send_message(WM_MOUSEWHEEL, (wParam << 16) | keyState, lParam);
      else
      {
         do {
            lResult = ::SendMessage(hwFocus, WM_MOUSEWHEEL,
               (wParam << 16) | keyState, lParam);
            hwFocus = ::GetParent(hwFocus);
         }
         while (lResult == 0 && hwFocus != ::null() && hwFocus != hwDesktop);
      }

#else

      throw not_implemented(get_app());

#endif

      return lResult;

   }


   void frame_window::ActivateFrame(int32_t nCmdShow)
      // nCmdShow is the normal show mode this frame should be in
   {
      // translate default nCmdShow (-1)
      if (nCmdShow == -1)
      {
         if (!IsWindowVisible())
            nCmdShow = SW_SHOWNORMAL;
         else if (IsIconic())
            nCmdShow = SW_RESTORE;
      }

      // bring to top before showing
      BringToTop(nCmdShow);

      if (nCmdShow != -1)
      {
         // show the ::ca::window as specified
         ShowWindow(nCmdShow);

         // and finally, bring to top after showing
         BringToTop(nCmdShow);
      }



   }

   void frame_window::BringToTop(int32_t nCmdShow)
   {

#ifdef WINDOWSEX

      if(get_parent() == ::null())
      {

         // place the ::ca::window on top except for certain nCmdShow

         if(
            nCmdShow != SW_HIDE
         && nCmdShow != SW_MINIMIZE
         && nCmdShow != SW_SHOWMINNOACTIVE
         && nCmdShow != SW_SHOWNA
         && nCmdShow != SW_SHOWNOACTIVATE
            )
         {

            oswindow oswindow = get_handle();

            oswindow = ::GetLastActivePopup(oswindow);

            ::BringWindowToTop(oswindow);

         }

      }

#else

      //throw not_implemented(get_app());

#endif

   }


   /////////////////////////////////////////////////////////////////////////////
   // frame_window Diagnostics



   //void frame_window::dump(dump_context & dumpcontext) const
   //{
   //   ::user::frame_window_interface::dump(dumpcontext);

   //   dumpcontext << "m_hAccelTable = " << (void *)m_hAccelTable;
   //   dumpcontext << "\nm_nWindow = " << m_nWindow;
   //   dumpcontext << "\nm_nIDHelp = " << m_strMatterHelp;
   //   dumpcontext << "\nm_nIDTracking = " << m_nIDTracking;
   //   dumpcontext << "\nm_nIDLastMessage = " << m_nIDLastMessage;
   //   if (m_pViewActive != ::null())
   //      dumpcontext << "\nwith active ::user::view: " << m_pViewActive.m_p;
   //   else
   //      dumpcontext << "\nno active ::user::view";

   //   dumpcontext << "\n";
   //}




   bool frame_window::is_frame_window()
   {
      return true;
   }

   bool frame_window::IsTracking() const
   {
      /*   return m_nIDTracking != 0 &&
      m_nIDTracking != __IDS_HELPMODEMESSAGE &&
      m_nIDTracking != __IDS_IDLEMESSAGE;*/
      return FALSE;
   }


   bool frame_window::ShowWindow(int32_t nCmdShow)
   {
      bool bResult = interaction::ShowWindow(nCmdShow);
      if(get_parent() != ::null()
         && nCmdShow == SW_RESTORE)
      {
         InitialFramePosition(true);
      }
      return bResult;
   }



   void frame_window::_001OnSysCommand(::ca::signal_object * pobj)
   {

#ifdef WINDOWS

      SCAST_PTR(::ca::message::base, pbase, pobj);
      if(get_parent() == ::null())
      {
         if(pbase->m_wparam == SC_RESTORE)
         {
            m_eappearance = appearance_normal;
            InitialFramePosition(true);
            pbase->m_bRet = true;
            pbase->set_lresult(0);
         }
      }

#else

      throw not_implemented(get_app());

#endif

   }


   // frame_window
   void frame_window::DelayUpdateFrameTitle()
   { m_nIdleFlags |= idleTitle; }
   void frame_window::DelayRecalcLayout(bool bNotify)
   { m_nIdleFlags |= (idleLayout | (bNotify ? idleNotify : 0)); };
   bool frame_window::InModalState() const
   { return m_cModalStack != 0; }
   void frame_window::set_title(const char * lpszTitle)
   { m_strTitle = lpszTitle; }
   string frame_window::get_title() const
   { return m_strTitle; }



   void frame_window::LoadToolBar(id idToolBar, const char * pszToolBar, uint32_t dwCtrlStyle, uint32_t dwStyle)
   {

      throw interface_only_exception(get_app());

   }

   bool frame_window::on_simple_command(e_simple_command ecommand, LPARAM lparam, LRESULT & lresult)
   {

      if(::user::frame_window_interface::on_simple_command(ecommand, lparam, lresult))
         return true;

      switch(ecommand)
      {
      case simple_command_update_frame_title:
         on_update_frame_title(lparam != FALSE);
         break;
      default:
         break;
      }


      return false;

   }


   void frame_window::CommonConstruct()
   {
      // trans ASSERT(get_handle() == ::null());

      m_nWindow = -1;                 // unknown ::ca::window ID
      m_bAutoMenuEnable = TRUE;       // auto enable on by default
      m_lpfnCloseProc = ::null();
      m_hMenuDefault = ::null();
      m_hAccelTable = ::null();
      //m_nIDHelp = 0;
      m_nIDTracking = 0;
      m_nIDLastMessage = 0;

      m_cModalStack = 0;              // initialize modality support
      m_hMenuAlt = ::null();
      m_nIdleFlags = 0;               // no idle work at start
      m_rectBorder.null();
#ifdef WINDOWSEX
      m_bHelpMode = HELP_INACTIVE;    // not in Shift+F1 help mode
#endif
      m_dwPromptContext = 0;

      m_pNextFrameWnd = ::null();         // not in list yet

      m_bInRecalcLayout = FALSE;
      m_nShowDelay = -1;              // no delay pending

      AddFrameWnd();
   }





   void frame_window::NotifyFloatingWindows(uint32_t dwFlags)
   {
      ASSERT_VALID(this);
      // trans   ASSERT(get_handle() != ::null());

      // get top level parent frame ::ca::window first unless this is a child ::ca::window
      sp(::user::frame_window) pParent = (GetStyle() & WS_CHILD) ? this : (GetTopLevelFrame().m_p);
      ASSERT(pParent != ::null());
      if (dwFlags & (FS_DEACTIVATE|FS_ACTIVATE))
      {
         // update parent ::ca::window activation state
         bool bActivate = !(dwFlags & FS_DEACTIVATE);
         bool bEnabled = pParent->IsWindowEnabled();

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
      sp(::user::interaction) oswindow = System.get_desktop_window()->GetWindow(GW_CHILD);
      while (oswindow != ::null())
      {
         if(::user::is_descendant(pParent, oswindow))
            oswindow->send_message(WM_FLOATSTATUS, dwFlags, 0);
         oswindow = oswindow->GetWindow(GW_HWNDNEXT);
      }
#else
      throw todo(get_app());
#endif
   }

   extern const CHAR _vfxWndFrameOrView[];
   //const CHAR _vfxWndFrameOrView[] = __WNDFRAMEORVIEW;

   /////////////////////////////////////////////////////////////////////////////
   // frame_window second phase creation

   //bool frame_window::pre_create_window(CREATESTRUCT& cs)
   //{

   //   if ((cs.style & FWS_ADDTOTITLE))
   //      cs.style |= FWS_PREFIXTITLE;

   //   //if (afxData.bWin4)
   //   cs.dwExStyle |= WS_EX_CLIENTEDGE;

   //   return TRUE;
   //}



   //void frame_window::_001OnCreate(::ca::signal_object * pobj)
   //{
   //   UNREFERENCED_PARAMETER(pobj);

   //   if(pobj->previous())
   //      return;

   //}




   void frame_window::RemoveControlBar(::user::control_bar *pBar)
   {
      POSITION pos = m_listControlBars.find(pBar);
      if (pos != ::null())
         m_listControlBars.remove_at(pos);
   }


   /////////////////////////////////////////////////////////////////////////////
   // frame_window command/message routing







   /////////////////////////////////////////////////////////////////////////////
   // default frame processing


   // query end session for main frame will attempt to close it all down
   void frame_window::_001OnQueryEndSession(::ca::signal_object * pobj)
   {
      SCAST_PTR(::ca::message::base, pbase, pobj);
      if (&System != ::null() && System.GetMainWnd() == this)
      {
         pbase->set_lresult(System.save_all_modified());
         pbase->m_bRet = true;
         return;
      }
      pbase->set_lresult(TRUE);
      return;
   }


   /////////////////////////////////////////////////////////////////////////////
   // Special ::user::view swapping/activation

   void frame_window::_001OnSetFocus(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      if (m_pViewActive != ::null())
         m_pViewActive->SetFocus();
   }



   void frame_window::OnUpdateControlBarMenu(cmd_ui * pcmdui)
   {
      /*      ASSERT(ID_VIEW_STATUS_BAR == "status_bar");
      ASSERT(ID_VIEW_TOOLBAR == __IDW_TOOLBAR);
      ASSERT(ID_VIEW_REBAR == __IDW_REBAR);*/

      pcmdui->ContinueRouting();
   }

   bool frame_window::OnBarCheck(UINT nID)
   {
      UNREFERENCED_PARAMETER(nID);
      /*ASSERT(ID_VIEW_STATUS_BAR == "status_bar");
      ASSERT(ID_VIEW_TOOLBAR == __IDW_TOOLBAR);
      ASSERT(ID_VIEW_REBAR == __IDW_REBAR);*/

      return FALSE;
   }


   /////////////////////////////////////////////////////////////////////////////
   // Setting title of frame ::ca::window - UISG standard



   //void frame_window::DelayUpdateFrameMenu(HMENU hMenuAlt)
   //{
   //   m_hMenuAlt = hMenuAlt;
   //   m_nIdleFlags |= idleMenu;
   //}

   void frame_window::_001OnIdleUpdateCmdUI(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      // update menu if necessary
      if (m_nIdleFlags & idleMenu)
         OnUpdateFrameMenu(m_hMenuAlt);

      // update title if necessary
      if (m_nIdleFlags & idleTitle)
         on_update_frame_title(TRUE);

      // recalc layout if necessary
      if (m_nIdleFlags & idleLayout)
      {
         layout();
         UpdateWindow();
      }

      // set the current message string if necessary
      if (m_nIDTracking != m_nIDLastMessage)
      {
         SetMessageText(m_nIDTracking);
         ASSERT(m_nIDTracking == m_nIDLastMessage);
      }
      m_nIdleFlags = 0;
   }



   void frame_window::_001OnSize(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //      SCAST_PTR(::ca::message::size, psize, pobj)
   }




   /////////////////////////////////////////////////////////////////////////////
   // frame_window Diagnostics

   //#ifdef DEBUG
   void frame_window::assert_valid() const
   {
      try
      {
         ::user::interaction::assert_valid();
      }
      catch(...)
      {
      }
      try
      {
         if (m_pViewActive != ::null())
            ASSERT_VALID(m_pViewActive);
      }
      catch(...)
      {
      }
   }


   void frame_window::dump(dump_context & dumpcontext) const
   {
      ::user::interaction::dump(dumpcontext);

      dumpcontext << "m_hAccelTable = " << (uint_ptr)m_hAccelTable;
      dumpcontext << "\nm_nWindow = " << m_nWindow;
      dumpcontext << "\nm_nIDHelp = " << m_strMatterHelp;
      dumpcontext << "\nm_nIDTracking = " << m_nIDTracking;
      dumpcontext << "\nm_nIDLastMessage = " << m_nIDLastMessage;
      if (m_pViewActive != ::null())
         dumpcontext << "\nwith active ::user::view: " << m_pViewActive.m_p;
      else
         dumpcontext << "\nno active ::user::view";

      dumpcontext << "\n";
   }
   //#endif //DEBUG



   // in this file for is_kind_of library granularity (is_kind_of references these)
   //// IMPLEMENT_DYNCREATE(frame_window, ::user::interaction)
   //// IMPLEMENT_DYNAMIC(::user::view, ::user::interaction)
   //// IMPLEMENT_DYNAMIC(::::user::control_bar, ::user::interaction)

   /////////////////////////////////////////////////////////////////////////////



   /////////////////////////////////////////////////////////////////////////////
   // frame_window (here for library granularity)



   sp(::user::interaction) frame_window::WindowDataGetWnd()
   {
      return this;
   }


   // dwDockBarMap
   /*const uint32_t frame_window::dwDockBarMap[4][2] =
   {
   { __IDW_DOCKBAR_TOP,      CBRS_TOP    },
   { __IDW_DOCKBAR_BOTTOM,   CBRS_BOTTOM },
   { __IDW_DOCKBAR_LEFT,     CBRS_LEFT   },
   { __IDW_DOCKBAR_RIGHT,    CBRS_RIGHT  },
   };*/



   bool frame_window::BaseOnControlEvent(::user::control_event * pevent)
   {
      if(m_workset.BaseOnControlEvent(pevent))
         return true;
      return ::user::frame_window_interface::BaseOnControlEvent(pevent);
   }



   void frame_window::_000OnDraw(::ca::graphics * pdc)
   {
      frame_window_interface::_000OnDraw(pdc);
   }


   void frame_window::AddControlBar(::user::control_bar *pBar)
   {

      m_listControlBars.add_tail(pBar);

   }


} // namespace user









