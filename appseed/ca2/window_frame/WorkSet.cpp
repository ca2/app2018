#include "StdAfx.h"
#include "ControlBoxButton.h"
#include "SysMenuManager.h"
#include "FrameSchema.h"

namespace window_frame
{


   WorkSet::WorkSet(::ca::application * papp) :
      ca(papp)
   {

      m_pframeschema       = NULL;

      m_bSizingCapture     = false;
      m_bEnableMouse       = true;

      m_bFullScreenEnable  = false;
      m_bNotifyIconEnable  = false;

      m_pappearance                = new appearance(this);
      m_pmovemanager               = new MoveManager(this);
      m_psizemanager               = new SizeManager(this);
      m_psystemmenumanager         = new SysMenuManager(this);
      m_pdockmanager               = new DockManager();

      m_psizemanager->SetSWPFlags(SWP_SHOWWINDOW);
      m_pmovemanager->SetSWPFlags(SWP_SHOWWINDOW);


      m_pwndRegion      = NULL;
      m_pwndDraw        = NULL;
      m_pwndEvent       = NULL;
      m_pwndCommand     = NULL;

      m_bHoverModeOn    = false;
      m_bHoverActive    = false;
       
      m_bMovingEnabled  = true;
      m_bSizingEnabled  = true;
      m_bSysMenuEnabled = true;

      m_bEnable         = true;

   }

   WorkSet::~WorkSet()
   {
       if(m_pappearance != NULL)
       {
           delete m_pappearance;
           m_pappearance = NULL;
       }

       if(m_pmovemanager != NULL)
       {
           delete m_pmovemanager;
           m_pmovemanager = NULL;
       }

       if(m_psizemanager != NULL)
       {
           delete m_psizemanager;
           m_psizemanager = NULL;
       }
       if(m_psystemmenumanager != NULL)
       {
           delete m_psystemmenumanager;
           m_psystemmenumanager = NULL;
       }
      if(m_pdockmanager != NULL)
       {
           delete m_pdockmanager;
           m_pdockmanager = NULL;
       }
   }

   void WorkSet::message_handler(gen::signal_object * pobj)
   {
      relay_event(pobj);
   }

   void WorkSet::relay_event(gen::signal_object * pobj)
   {
      hover_relay_event(pobj);
      if(pobj->m_bRet)
         return;

      m_pdockmanager->relay_event(pobj);
      if(pobj->m_bRet)
         return;

       if(!m_pappearance->IsFullScreen() || !m_pappearance->IsZoomed())
       {
   /*        if(IsSizingEnabled() &&
               m_psizemanager->relay_event(lpmsg))
               return TRUE;*/

           /*if(IsMovingEnabled() &&
               m_pmovemanager->relay_event(lpmsg))
               return TRUE;*/
           if(!m_pappearance->IsFullScreen())
           {
               if(IsSysMenuEnabled())
               {
                   m_psystemmenumanager->relay_event(pobj);
                   if(pobj->m_bRet)
                     return;
               }
           }
       }


   /*    if(IsAppearanceEnabled() &&
           m_pappearance->relay_event(lpmsg))
           return TRUE;*/

   /*    if(lpmsg->message == WM_SIZE)
       {
         if(IsAppearanceEnabled())
         {
            UINT fwSizeType = lpmsg->wParam;      // resizing flag 
            int nWidth = LOWORD(lpmsg->lParam);  // width of client area 
            int nHeight = HIWORD(lpmsg->lParam); 
            OnSizeRegion(fwSizeType, nWidth, nHeight);
         }
       }*/

      

   }

   void WorkSet::_001OnDraw(::ca::graphics *pdc)
   {
      if(IsAppearanceEnabled() && m_pframeschema != NULL)
      {
         try
         {
            m_pframeschema->_001OnDraw(pdc);
         }
         catch(...)
         {
         }
      }
   }

   void WorkSet::EnableMove(bool bEnable)
   {
       m_bMovingEnabled = bEnable;
   }

   void WorkSet::EnableSize(bool bEnable)
   {
       m_bSizingEnabled = bEnable;
   }

   void WorkSet::EnableSysMenu(bool bEnable)
   {
       m_bSysMenuEnabled = bEnable;
   }

   void WorkSet::EnableApperance(bool bEnable)
   {
       m_pappearance->Enable(bEnable);
   }

   void WorkSet::Enable(bool bEnable)
   {
      m_bEnable = bEnable;
      EnableApperance(bEnable);
      EnableMove(bEnable);
      EnableSize(bEnable);
      EnableSysMenu(bEnable);
   }

   bool WorkSet::IsEnabled()
   {
      return m_bEnable;
   }



   bool WorkSet::IsMovingEnabled()
   {
       return m_bMovingEnabled && !IsFullScreen();
   }

   bool WorkSet::IsSizingEnabled()
   {
       return m_bSizingEnabled && (m_pappearance == NULL ||
           m_pappearance->GetAppearanceMode() != AppearanceFixedSize);
   }

   bool WorkSet::IsSysMenuEnabled()
   {
       return m_bSysMenuEnabled;
   }

   bool WorkSet::IsAppearanceEnabled()
   {
      if(m_pappearance == NULL)
         return false;
      else
         return m_pappearance->IsEnabled();
   }


   void WorkSet::GetRegionClientRect(LPRECT lprect)
   {
      ASSERT(m_pframeschema != NULL);
       //m_pappearance->GetWndClientRect(lprect);
      m_pframeschema->GetWndClientRect(lprect);
   }

   void WorkSet::GetDrawClientRect(LPRECT lprect)
   {
      ASSERT(m_pframeschema != NULL);
       //m_pappearance->GetWndClientRect(lprect);
      m_pframeschema->GetWndClientRect(lprect);
   }

   void WorkSet::SetAppearanceMode(EAppearanceMode nMode)
   {
       if(m_pappearance != NULL)
           m_pappearance->SetAppearanceMode(nMode);
   }

   EAppearanceMode WorkSet::GetAppearanceMode()
   {
      ASSERT(m_pappearance != NULL);
      return m_pappearance->GetAppearanceMode();
   }

   void WorkSet::UpdateApperanceMode(bool bFullScreen)
   {
       if(m_pappearance != NULL)
           m_pappearance->UpdateAppearanceMode(bFullScreen);

   }


   void WorkSet::SetAppearanceMode()
   {
       if(m_pappearance != NULL)
           m_pappearance->SetAppearanceMode();
   }

   bool WorkSet::IsFullScreen()
   {
       if(m_pappearance != NULL)
           return m_pappearance->IsFullScreen();
       else
           return false;
   }

   bool WorkSet::ViewFullScreen(bool bFullScreen)
   {
   //    m_bFullScreen = bFullScreen;
       if(bFullScreen)
       {
           m_pappearance->SetAppearanceMode(AppearanceModeFullScreen);
       }
       else
       {
           m_pappearance->SetAppearanceMode();
       }

       return VMSR_SUCCESS;
   };

   bool WorkSet::update(
      ::user::interaction *pwndDraw,
      ::user::interaction *pwndRegion,
      ::user::interaction *pwndEvent,
      ::user::interaction *pwndCommand)
   {

      if(m_pwndRegion !=  NULL)
      {
         m_pwndRegion->KillTimer(16319);
      }

      m_pwndDraw = pwndDraw;
      m_pwndRegion = pwndRegion;
      m_pwndEvent = pwndEvent;
      m_pwndCommand = pwndCommand;

      if(!m_pmovemanager->update(this))
         return false;

      if(!m_pdockmanager->update(this))
         return false;

      if(!m_pappearance->update())
         return false;

      if(!m_psizemanager->update(this))
         return false;
      
      if(!m_psystemmenumanager->update(this))
         return false;

      m_psizemanager->SetMinSize(m_pframeschema->GetMinSize());

      m_pdockmanager->UpdateDocking();

      pwndRegion->SetTimer(16319, 100, NULL);

      _001InstallEventHandling(dynamic_cast<::gen::message::dispatch*>(pwndEvent->m_pimpl));

      return true;
   }

   /*void WorkSet::OnSizeRegion( UINT nType, int cx, int cy )
   {
       if(m_pappearance != NULL)
       {
           if(IsFullScreen())
           {
               m_pappearance->SetAppearanceMode(WorkSet::AppearanceModeFullScreen);
           }
           else
           {
               m_pappearance->SetAppearanceMode();
           }
           m_pappearance->OnSizeRegion(nType, cx, cy);
       }
      if(m_pdockmanager != NULL)
      {
         m_pdockmanager->OnSize();
      }
   }*/

   bool WorkSet::BaseOnControlEvent(::user::control_event * pevent)
   {
      if(pevent->m_eevent == ::user::event_button_clicked)
      {
          WorkSetClientInterface * pinterface = dynamic_cast<WorkSetClientInterface *>(m_pwndCommand);
          if(pinterface == NULL)
             return false;
          ASSERT(pinterface != NULL);
          FrameSchema::EButton ebutton = m_pframeschema->GetButtonId(dynamic_cast < ::user::interaction * > (pevent->m_puie)->GetDlgCtrlId());
          switch(ebutton)
          {
          case FrameSchema::ButtonClose:
             pinterface->WfiClose();
             return TRUE;
          case FrameSchema::ButtonMinimize:
             pinterface->WfiMinimize();
             return TRUE;
          case FrameSchema::ButtonMaximize:
             pinterface->WfiMaximize();
             return TRUE;
          case FrameSchema::ButtonRestore:
             pinterface->WfiRestore();
             return TRUE;
          case FrameSchema::ButtonUp:
             pinterface->WfiUp();
             return TRUE;
          case FrameSchema::ButtonDown:
             pinterface->WfiDown();
             return TRUE;
          case FrameSchema::ButtonNotifyIcon:
             pinterface->WfiNotifyIcon();
             return TRUE;

          }
      }
      return false;
   }

   bool WorkSet::_001OnCmdMsg(BaseCmdMsg * pcmdmsg) 
   {
      if(pcmdmsg->m_etype == BaseCmdMsg::type_command
      && m_pwndCommand != NULL)
      {
          WorkSetClientInterface * pinterface = dynamic_cast<WorkSetClientInterface *>(m_pwndCommand);
          ASSERT(pinterface != NULL);
          FrameSchema::EButton ebutton = m_pframeschema->GetButtonId(pcmdmsg->m_id);
          switch(ebutton)
          {
          case FrameSchema::ButtonClose:
             pinterface->WfiClose();
             return TRUE;
          case FrameSchema::ButtonMinimize:
             pinterface->WfiMinimize();
             return TRUE;
          case FrameSchema::ButtonMaximize:
             pinterface->WfiMaximize();
             return TRUE;
          case FrameSchema::ButtonRestore:
             pinterface->WfiRestore();
             return TRUE;
          case FrameSchema::ButtonUp:
             pinterface->WfiUp();
             return TRUE;
          case FrameSchema::ButtonDown:
             pinterface->WfiDown();
             return TRUE;

          }
      }
      return FALSE;
   }

   void WorkSet::_001OnCommand(gen::signal_object * pobj)
   {
      SCAST_PTR(::gen::message::base, pbase, pobj)
      if(m_pframeschema == NULL)
         pbase->m_bRet = false;
      else
         pbase->m_bRet = m_pframeschema->_000OnCommand(pbase->m_wparam, pbase->m_lparam, pbase->get_lresult());

   }


   //  define System flags que ser�o usados para posicionar ou
   //  dimensionar pelo uso da fun��o SetWindowPos

   void WorkSet::SetSWPFlags(UINT uiFlags)
   {
       m_uiSWPFlags = uiFlags;

       m_pmovemanager->SetSWPFlags(m_uiSWPFlags);
       m_psizemanager->SetSWPFlags(m_uiSWPFlags);
       
   }


   void WorkSet::SetActiveFlag(BOOL fActive)
   {
      if(fActive != m_pappearance->m_fActive)
      {
         TRACE("WorkSet::SetActiveFlag %d\n", fActive);
         m_pappearance->m_fActive = fActive;
         //m_pappearance->GetDrawWindow()->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME);
         if(m_pframeschema != NULL)
         {
            m_pframeschema->OnActivate();
         }
         if(GetDrawWindow() != NULL)
         {
            GetDrawWindow()->_001RedrawWindow();
         }
      }
   }

   void WorkSet::SetAppearanceTransparency(EAppearanceTransparency nTransparency)
   {
       if(m_pappearance != NULL)
           m_pappearance->SetTransparency(nTransparency);

   }
   /*void WorkSet::SetAppearanceStyle(EAppearanceStyle nStyle)
   {
       if(m_pappearance != NULL)
           m_pappearance->SetStyle(nStyle);

   }*/


   ::user::interaction * WorkSet::GetWndDraw()
   {
       return m_pwndDraw;
   }

   ::user::interaction * WorkSet::GetWndRegion()
   {
       return m_pwndRegion;
   }

   void WorkSet::AppearanceUpdateControlBox()
   {
   }

   //void WorkSet::OnUp()
   //{
   //   FrameWnd(m_pwndRegion);
   //   
   //   m_pwndRegion->SendMessage(WM_USER + 101, 1, ID_VMSGUI_UP);
   //}

   //void WorkSet::OnDown()
   //{
   //   ChildWnd(m_pwndRegion, m_pwndParent);
   //   CDBCentral * pdb = db();
   //   CDBLongSet * plongset = pdb->get_db_long_set();
   //   plongset->MoveWindow_(m_strRegSection, "UpRect", m_pwndRegion);
   //   plongset->Save(m_strRegSection, "Level", 50);
   //   m_pappearance->m_pbuttonUp->ShowWindow(SW_SHOW);
   //   m_pappearance->m_pbuttonDown->ShowWindow(SW_HIDE);
   //   m_pwndRegion->SendMessage(WM_USER + 101, 1, ID_VMSGUI_DOWN);
   //}

   bool WorkSet::Start()
   {
      return true;
   }

   void WorkSet::FrameWnd(::user::interaction *pwnd)
   {
      UNREFERENCED_PARAMETER(pwnd);
   }

   void WorkSet::ChildWnd(::user::interaction *pwnd, ::user::interaction * pwndParent)
   {
      UNREFERENCED_PARAMETER(pwnd);
      UNREFERENCED_PARAMETER(pwndParent);

   }

   void WorkSet::hover_relay_event(gen::signal_object * pobj)
   {
      SCAST_PTR(gen::message::base, pbase, pobj);
      if(m_bHoverModeOn)
      {
   /*      if(m_bHoverActive &&
            (lpmsg->message == WM_MOUSEMOVE
            || lpmsg->message == WM_LBUTTONDOWN
            || lpmsg->message == WM_LBUTTONUP 
            || lpmsg->message == WM_RBUTTONDOWN 
            || lpmsg->message == WM_RBUTTONUP
            || lpmsg->message == WM_CONTEXTMENU) )
         {
            point ptCursor = lpmsg->pt;
            ::ca::region rgn;
            rect rectWindow;
            rect rect;
            rgn.CreateRectRgn(0, 0, 0, 0);
            int i;
            for(::user::interaction * pwnd = GetWndRegion()->GetWindow(GW_CHILD) ;
               pwnd ; pwnd = pwnd->GetNextWindow())
            {
               pwnd->GetWindowRect(rectWindow);
               i = pwnd->GetWindowRgn(rgn);
               if(i == NULLREGION || i == ERROR)
               {
                  if(rectWindow.contains(ptCursor))
                  {
                     pwnd->SendMessage(
                        APPM_FORWARDSYNCMESSAGE,
                        lpmsg->message,
                        (LPARAM) lpmsg);
                     break;
                  }
               }
               else
               {
                  rgn.GetRgnBox(rect);
                  rgn.OffsetRgn(rectWindow.top_left());
                  if(rgn.PtInRegion(ptCursor))
                  {
                     pwnd->SendMessage(
                        APPM_FORWARDSYNCMESSAGE,
                        lpmsg->message,
                        (LPARAM) lpmsg);
                     break;
                  }
               }

            }*/
         
         if(pbase->m_uiMessage == WM_TIMER 
            && pbase->m_wparam == 16319
            && IsHoverModeOn())
         {
            rect rectWindow;
            ::user::interaction * pwnd = GetWndRegion();
            pwnd->GetWindowRect(rectWindow);
            point ptCursor;
            System.get_cursor_pos(&ptCursor);
            if(rectWindow.contains(ptCursor))
            {
               if(!IsHoverActive())
                  Hover(true);
            }
            else if(!m_pmovemanager->IsMoving() &&
               !m_psizemanager->IsSizing())
            {
               if(IsHoverActive())
                  Hover(false);
            }
            return;
         }
         else
         {
            return;
         }
         /*if(lpmsg->message == WM_MOUSEMOVE)
         {
            rect rectWindow;
            ::user::interaction * pwnd = GetWndRegion();
            pwnd->GetWindowRect(rectWindow);
            point ptCursor = lpmsg->pt;
            if(rectWindow.contains(ptCursor))
            {
               if(uieApplication.get_capture_uie() != pwnd)
               {
                  pwnd->SetCapture();
                  Hover(true);
               }
            }
            else if(!m_pmovemanager->IsMoving() &&
               !m_psizemanager->IsSizing())
            {
               if(m_bHoverActive)
               {
                  System.release_capture_uie();
                  Hover(false);
               }
            }
            return FALSE;
         }
         else
         {
            return FALSE;
         }*/
      }
      else
      {
         return;
      }

   }

   bool WorkSet::Hover(bool bHoverActive)
   {
      m_bHoverActive = bHoverActive;
      if(m_bHoverModeOn)
      {
         if(bHoverActive)
         {
            m_pappearance->Enable(true);
            ::user::interaction * pwnd = GetWndRegion();
            pwnd->RedrawWindow();
         }
         else
         {
            m_pappearance->Enable(false);
            ::user::interaction * pwnd = GetWndRegion();
            pwnd->RedrawWindow();
         }
      }
      return true;
   }

   void WorkSet::layout()
   {
      
      if(m_pframeschema != NULL)
      {
         m_pframeschema->layout();
      }

      if(m_pappearance != NULL)
      {
         m_pdockmanager->layout();
      }

   }


   void WorkSet::EnableDock(bool bEnable)
   {
      m_bDockEnable = bEnable;
   }

   void WorkSet::OnDock()
   {
      ASSERT(m_pdockmanager != NULL);
      EDock edock = m_pdockmanager->GetDockState();
      EGrip egripRemove = GripNone;
      MoveManager::EBorder eborderRemove = MoveManager::BorderNone;
      if(edock == DockNone)
      {
         m_psizemanager->SetGripMask(GripAll);
         m_pmovemanager->SetBorderMask(MoveManager::BorderAll);
      }
      if(edock & DockTop)
      {
         egripRemove = 
            (EGrip)
            (
               egripRemove
               | GripTop
            );
         eborderRemove =
            (MoveManager::EBorder)
            (
               eborderRemove
               | MoveManager::BorderTop
            );
      }
      if(edock & DockBottom)
      {
         egripRemove = 
            (EGrip)
            (
               egripRemove
               | GripBottom
            );
         eborderRemove =
            (MoveManager::EBorder)
            (
               eborderRemove
               | MoveManager::BorderBottom
            );
      }
      if(edock & DockLeft)
      {
         egripRemove = 
            (EGrip)
            (
               egripRemove
               | GripLeft
            );
         eborderRemove =
            (MoveManager::EBorder)
            (
               eborderRemove
               | MoveManager::BorderLeft
            );
      }
      if(edock & DockRight)
      {
         egripRemove = 
            (EGrip)
            (
               egripRemove
               | GripRight
            );
         eborderRemove =
            (MoveManager::EBorder)
            (
               eborderRemove
               | MoveManager::BorderRight
            );
      }
      if(edock != DockNone)
      {
         m_psizemanager->SetGripMask(
            (EGrip)
            (
               GripAll
               & ~egripRemove
            ));
         m_pmovemanager->SetBorderMask(
            (MoveManager::EBorder)
            (
               MoveManager::BorderAll
               & ~eborderRemove
            ));
      }

      m_wfla.WFLOnDock(
         this,
         m_pwndEvent);

   }

   SizeManager * WorkSet::GetSizingManager()
   {
      return m_psizemanager;
   }

   void WorkSet::SetDockMask(EDock emask)
   {
      ASSERT(m_pdockmanager != NULL);
      m_pdockmanager->SetDockMask(emask);
   }

   void WorkSet::OnMove()
   {
      if(m_pdockmanager != NULL)
      {
         m_pdockmanager->OnMove();
      }
   }

   void WorkSet::OnMoving()
   {
      if(m_pdockmanager != NULL)
      {
         m_pdockmanager->OnMoving();
      }
   }

   appearance * WorkSet::GetAppearance()
   {
      return m_pappearance;
   }

   DockManager * WorkSet::GetDockManager()
   {
      return m_pdockmanager;
   }

   bool WorkSet::IsHoverActive()
   {
      return m_bHoverActive;
   }

   bool WorkSet::IsHoverModeOn()
   {
      return m_bHoverModeOn;
   }

   MoveManager * WorkSet::GetMovingManager()
   {
      return m_pmovemanager;
   }

   void WorkSet::OnSizingGripMove(
      EGrip egrip)
   {
      m_wfla.WFLOnSizingGripMove(
         this,
         m_pwndEvent,
         egrip);
   }

   void WorkSet::AddListener(WorkSetListener *plistener)
   {
      m_wfla.Attach(plistener);
   }

   void WorkSet::RemoveListener(WorkSetListener *plistener)
   {
      m_wfla.remove(plistener);
   }

   void WorkSet::WindowProcHover(::user::interaction * pwnd, gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pwnd);
      SCAST_PTR(gen::message::base, pbase, pobj);
      if(m_bHoverModeOn)
      {
         if(pbase->m_uiMessage == WM_TIMER 
            && pbase->m_wparam == 16319
            && IsHoverModeOn())
         {
            rect rectWindow;
            ::user::interaction * pwnd = GetWndRegion();
            pwnd->GetWindowRect(rectWindow);
            point ptCursor;
            System.get_cursor_pos(&ptCursor);
            if(rectWindow.contains(ptCursor))
            {
               if(!IsHoverActive())
                  Hover(true);
            }
            else if(!m_pmovemanager->IsMoving() &&
               !m_psizemanager->IsSizing())
            {
               if(IsHoverActive())
                  Hover(false);
            }
         }
      }
   }


   void WorkSet::WindowProcBefore(::user::interaction * pwnd, gen::signal_object * pobj)
   {

      WindowProcHover(pwnd, pobj);
      if(pobj->m_bRet)
         return;

      if(m_pappearance != NULL && 
         (!m_pappearance->IsFullScreen()
       || !m_pappearance->IsZoomed()
        ))
       {
           if(IsSizingEnabled() &&
            m_psizemanager != NULL)
           {
               m_psizemanager->message_handler(pwnd, pobj);
               if(pobj->m_bRet)
                  return;
           }

           if(IsMovingEnabled() &&
            m_pmovemanager != NULL)
           {
               m_pmovemanager->message_handler(pwnd, pobj);
               if(pobj->m_bRet)
                  return;
           }

           if(!m_pappearance->IsFullScreen())
           {
               if(IsSysMenuEnabled() &&
                  m_psystemmenumanager != NULL)
               {
                   m_psystemmenumanager->message_handler(pwnd, pobj);
                   if(pobj->m_bRet)
                     return;
               }
           }
       }


   /*    if(m_pappearance != NULL &&
         IsAppearanceEnabled() &&
           m_pappearance->_000WndProc(message, wparam, lparam, lresult))
           return true;*/
      SCAST_PTR(gen::message::base, pbase, pobj);
      pbase->set_lresult(0);

      if(pbase->m_uiMessage == WM_COMMAND)
      {
         SCAST_PTR(gen::message::command, pcommand, pobj);
         _001OnCommand(pcommand);
         if(pcommand->m_bRet)
         {
            pcommand->set_lresult(1);
            return;
         }
      }
      else if(pbase->m_uiMessage == WM_MOVE)
      {
         OnMove();
      }
      else if(pbase->m_uiMessage == WM_SIZE)
      {
   //      OnSizeRegion(wparam, LOWORD(lparam), HIWORD(lparam));
      }
      else if(pbase->m_uiMessage == WM_ACTIVATE)
      {
         ASSERT(FALSE);
   //      _001OnActivate();
      }

   }


   void WorkSet::_001OnActivate(gen::signal_object * pobj)
   {
      SCAST_PTR(::gen::message::activate, pactivate, pobj);
      pactivate->m_bRet = false;
      SetActiveFlag(pactivate->m_nState == WA_ACTIVE || pactivate->m_nState == WA_CLICKACTIVE);
      if(IsFullScreen())
      {
         GetDrawWindow()->SetWindowPos(ZORDER_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
      }
   }

   void WorkSet::_001OnNcActivate(gen::signal_object * pobj)
   {
      SCAST_PTR(::gen::message::nc_activate, pncactivate, pobj);
      //SetActiveFlag(pncactivate->m_bActive);
      pncactivate->set_lresult(TRUE);
      pncactivate->m_bRet = true;
   }

   void WorkSet::SetDownUpInterface(CWorkSetDownUpInterface *pinterface)
   {
      m_pappearance->SetDownUpInterface(pinterface);
   }

   /*void WorkSet::UpdateControlBox()
   {
      if(m_pappearance != NULL)
      {
         m_pappearance->UpdateControlBox();
      }
      else
      {
         ASSERT(FALSE);
      }
   }*/


   void WorkSet::WindowClose()
   {
      WorkSetClientInterface * pinterface = dynamic_cast<WorkSetClientInterface *>(m_pwndCommand);
      ASSERT(pinterface != NULL);
      if(pinterface->WfiClose())
      {
         m_wfla.WFLOnClose(this, m_pwndRegion);
      }
   }

   bool WorkSet::IsFullScreenEnabled()
   {
      return m_bFullScreenEnable;
   }

   void WorkSet::EnableFullScreen(bool bEnable)
   {
      m_bFullScreenEnable = bEnable;
   }

   void WorkSet::EnableNotifyIcon(bool bEnable)
   {
      m_bNotifyIconEnable = bEnable;
   }

   bool WorkSet::IsNotifyIconEnabled()
   {

      return m_bNotifyIconEnable;
   }

   void WorkSet::OnSize(UINT nType, int cx, int cy)
   {
      UNREFERENCED_PARAMETER(nType);
      UNREFERENCED_PARAMETER(cx);
      UNREFERENCED_PARAMETER(cy);
      if(m_pappearance != NULL)
      {
   //      m_pappearance->_001OnSize(nType, cx, cy);
      }
   }
   void WorkSet::_001InstallEventHandling(::gen::message::dispatch * pdispatch)
   {
      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN    , pdispatch, this, &WorkSet::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP      , pdispatch, this, &WorkSet::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_MOUSEMOVE      , pdispatch, this, &WorkSet::_001OnMouseMove);
      IGUI_WIN_MSG_LINK(WM_NCLBUTTONDOWN  , pdispatch, this, &WorkSet::_001OnNcLButtonDown);
      IGUI_WIN_MSG_LINK(WM_NCLBUTTONUP    , pdispatch, this, &WorkSet::_001OnNcLButtonUp);
      IGUI_WIN_MSG_LINK(WM_NCMOUSEMOVE    , pdispatch, this, &WorkSet::_001OnNcMouseMove);
      IGUI_WIN_MSG_LINK(WM_NCHITTEST      , pdispatch, this, &WorkSet::_001OnNcHitTest);
      IGUI_WIN_MSG_LINK(WM_ACTIVATE       , pdispatch, this, &WorkSet::_001OnActivate);
      IGUI_WIN_MSG_LINK(WM_NCACTIVATE     , pdispatch, this, &WorkSet::_001OnNcActivate);
      IGUI_WIN_MSG_LINK(WM_TIMER          , pdispatch, this, &WorkSet::_001OnTimer);
      IGUI_WIN_MSG_LINK(WM_SIZE           , pdispatch, this, &WorkSet::_001OnSize);
      IGUI_WIN_MSG_LINK(WM_COMMAND        , pdispatch, this, &WorkSet::_001OnCommand);
      IGUI_WIN_MSG_LINK(WM_MOVE           , pdispatch, this, &WorkSet::_001OnMove);

   /*   if(m_pappearance != NULL)
      {
         m_pappearance->InitializeMessageHandling(pinterface);
      }*/

   }

   void WorkSet::_001OnLButtonDown(gen::signal_object * pobj) 
   {
      SCAST_PTR(::gen::message::mouse, pmouse, pobj)
      if(!m_bEnable)
      {
         pmouse->m_bRet = false;
         return;
      }
      ASSERT(m_pframeschema != NULL);
      m_pframeschema->_000OnLButtonDown(pmouse);
      if(pmouse->m_bRet)
      {
         pmouse->set_lresult(1);
      }
   }

   void WorkSet::_001OnMouseMove(gen::signal_object * pobj) 
   {
      SCAST_PTR(::gen::message::mouse, pmouse, pobj)
      if(!m_bEnable)
      {
         pmouse->m_bRet = false;
         return;
      }
      ASSERT(m_pframeschema != NULL);
      point ptCursor = pmouse->m_pt;
      m_pframeschema->_000OnMouseMove(pmouse);
      if(pmouse->m_bRet)
      {
         pmouse->set_lresult(1);
      }
   }

   void WorkSet::_001OnLButtonUp(gen::signal_object * pobj) 
   {
      SCAST_PTR(::gen::message::mouse, pmouse, pobj)
      if(!m_bEnable)
      {
         pmouse->m_bRet = false;
         return;
      }
      ASSERT(m_pframeschema != NULL);
      m_pframeschema->_000OnLButtonUp(pmouse);
      if(pmouse->m_bRet)
      {
         pmouse->set_lresult(1);
      }
   }

   void WorkSet::_001OnNcLButtonDown(gen::signal_object * pobj) 
   {
      SCAST_PTR(::gen::message::mouse, pmouse, pobj)
      if(!m_bEnable)
      {
         pmouse->m_bRet = false;
         return;
      }
      ASSERT(m_pframeschema != NULL);
      m_pframeschema->_000OnNcLButtonDown(pmouse);
      if(pmouse->m_bRet)
      {
         pmouse->set_lresult(1);
      }
   }

   void WorkSet::_001OnNcMouseMove(gen::signal_object * pobj) 
   {
      SCAST_PTR(::gen::message::mouse, pmouse, pobj)
      if(!m_bEnable)
      {
         pmouse->m_bRet = false;
         return;
      }
      ASSERT(m_pframeschema != NULL);
      point pt(pmouse->m_pt);

      m_pframeschema->_000OnNcMouseMove(pmouse);
      if(pmouse->m_bRet)
      {
         pmouse->set_lresult(1);
      }

   }

   void WorkSet::_001OnNcLButtonUp(gen::signal_object * pobj) 
   {
      SCAST_PTR(::gen::message::mouse, pmouse, pobj)
      if(!m_bEnable)
      {
         pmouse->m_bRet = false;
         return;
      }
      ASSERT(m_pframeschema != NULL);
      m_pframeschema->_000OnNcLButtonUp(pmouse);
      if(pmouse->m_bRet)
      {
         pmouse->set_lresult(1);
      }

   }

   void WorkSet::_001OnNcHitTest(gen::signal_object * pobj) 
   {
      SCAST_PTR(::gen::message::nchittest, pnchittest, pobj)
      if(!m_bEnable)
      {
         pnchittest->m_bRet = false;
         return;
      }
      ASSERT(m_pframeschema != NULL);
      pnchittest->m_bRet = m_pframeschema->_000OnNcHitTest(pnchittest->m_pt, pnchittest->get_lresult());
   }

   void WorkSet::_001OnTimer(gen::signal_object * pobj)
   {
      SCAST_PTR(::gen::message::timer, ptimer, pobj)
      if(!m_bEnable)
      {
         ptimer->m_bRet = false;
         return;
      }
      ASSERT(m_pframeschema != NULL);
      ptimer->m_bRet = m_pframeschema->_000OnTimer(ptimer->m_nIDEvent);
   }

   void WorkSet::_001OnSize(gen::signal_object * pobj)
   {
      SCAST_PTR(::gen::message::size, psize, pobj)
      if(!m_bEnable)
      {
         psize->m_bRet = false;
         return;
      }
      if(m_pframeschema != NULL)
      {
         psize->m_bRet = m_pframeschema->_000OnSize(psize->m_nType, psize->m_size.cx, psize->m_size.cy);
      }
   }

   void WorkSet::_001OnMove(gen::signal_object * pobj)
   {
      SCAST_PTR(::gen::message::move, pmove, pobj)
      if(!m_bEnable)
      {
         pmove->m_bRet = false;
         return;
      }
      if(m_pframeschema != NULL)
      {
         pmove->m_bRet = m_pframeschema->_000OnMove(pmove->m_pt.x, pmove->m_pt.y);
      }
   }


   void WorkSet::_000OnBeforeSize(LPCRECT lpcrect)
   {
      ASSERT(m_pframeschema != NULL);
      m_pframeschema->_000OnBeforeSize(lpcrect);
   }

   void WorkSet::OnNcCalcSize(LPRECT lprect)
   {
      ASSERT(m_pframeschema != NULL);
      m_pframeschema->OnNcCalcSize(lprect);
   }


   // point should be in screen coordinates
   EHitTest WorkSet::hit_test(point ptCursor)
   {
      ASSERT(m_pframeschema != NULL);
      return m_pframeschema->_000HitTest(ptCursor);
   }

   // point should be in screen coordinates
   /*EHitTest appearance::hit_test(point ptCursor)
   {
      EHitTest etest = HitTestClient;
      {
         //point pt = ptCursor;
         //m_pwndEvent->ScreenToClient(pt);
         rect rectEvent;
         m_pwnd->GetWindowRect(rectEvent);
         rect rect;
         point ptCenter = rectEvent.center();
         try
         {
            EGrip egrip = GetGripMask();
            
            if(egrip & GripNW)
            {
               rect = rectEvent;
               rect.right = rect.left + 16;
               rect.bottom = rect.top + 5;
               if(rect.contains(ptCursor))
               {
                  etest = HitTestSizingTopLeft;
                  goto SizingSuccess;
               }
               rect = rectEvent;
               rect.right = rect.left + 5;
               rect.bottom = rect.top + 16;
               if(rect.contains(ptCursor))
               {
                  etest = HitTestSizingTopLeft;
                  goto SizingSuccess;
               }
            }
            if(egrip & GripNE)
            {
               rect = rectEvent;
               rect.left = rect.right - 16;
               rect.bottom = rect.top + 5;
               if(rect.contains(ptCursor))
               {
                  throw ModeNE;
               }
               rect = rectEvent;
               rect.left = rect.right - 5;
               rect.bottom = rect.top + 16;
               if(rect.contains(ptCursor))
               {
                  throw ModeNE;
               }
            }
            if(egrip & GripSE)
            {
               rect = rectEvent;
               rect.left = rect.right - 16;
               rect.top = rect.bottom - 5;
               if(rect.contains(ptCursor))
               {
                  throw ModeSE;
               }
               rect = rectEvent;
               rect.left = rect.right - 5;
               rect.top = rect.bottom - 16;
               if(rect.contains(ptCursor))
               {
                  throw ModeSE;
               }
            }
            if(egrip & GripSW)
            {
               rect = rectEvent;
               rect.right = rect.left + 16;
               rect.top = rect.bottom - 5;
               if(rect.contains(ptCursor))
               {
                  throw ModeSW;
               }
               rect = rectEvent;
               rect.right = rect.left + 5;
               rect.top = rect.bottom - 16;
               if(rect.contains(ptCursor))
               {
                  throw ModeSW;
               }
            }
            if(egrip & GripN)
            {
               rect.top = rectEvent.top;
               rect.left = ptCenter.x - 8;
               rect.right = ptCenter.x + 8;
               rect.bottom = rectEvent.top + 5;
               if(rect.contains(ptCursor))
               {
                  throw ModeN;
               }
            }
            if(egrip & GripS)
            {
               rect.top = rectEvent.bottom - 5;
               rect.left = ptCenter.x - 8;
               rect.right = ptCenter.x + 8;
               rect.bottom = rectEvent.bottom;
               if(rect.contains(ptCursor))
               {
                  throw ModeS;
               }
            }
            if(egrip & GripW)
            {
               rect.top = ptCenter.y - 8;
               rect.left = rectEvent.left;
               rect.right = rectEvent.left + 5;
               rect.bottom = ptCenter.y + 8;
               if(rect.contains(ptCursor))
               {
                  throw ModeW;
               }
            }
            if(egrip & GripE)
            {
               rect.top = ptCenter.y - 8;
               rect.left = rectEvent.right - 5;
               rect.right = rectEvent.right;
               rect.bottom = ptCenter.y + 8;
               if(rect.contains(ptCursor))
               {
                  throw ModeE;
               }
            }
            
   SizingSuccess;
         return etest;
       }
       catch(enum EMode e)
       {
          return e;
       }
       ASSERT(FALSE);
       return ModeNone;
   }*/

   void WorkSet::AttachFrameSchema(FrameSchema * pframeschema)
   {

      m_pframeschema = pframeschema;

      pframeschema->m_pworkset = this;

      m_pframeschema->OnAttach();

   }

   ::user::interaction * WorkSet::GetDrawWindow()
   {
      return dynamic_cast<::user::interaction*>(m_pwndDraw);
   }

   ::user::interaction * WorkSet::GetEventWindow()
   {
      return dynamic_cast<::user::interaction*>(m_pwndEvent);
   }

   ::user::interaction * WorkSet::GetRegionWindow()
   {
      return dynamic_cast<::user::interaction*>(m_pwndRegion);
   }

   ::user::interaction * WorkSet::GetCommandWindow()
   {
      return dynamic_cast<::user::interaction*>(m_pwndCommand);
   }

} // namespace window_frame






