#include "framework.h"


namespace user
{



   namespace wndfrm
   {



      namespace frame
      {


         DockManager::DockManager(WorkSet * pworkset) :
            object(pworkset->get_app())
         {
            
            ASSERT(pworkset != NULL);
            m_pworkset = pworkset;
            m_bDocking = false;
            SetSWPFlags(0);
            m_eborderMask = BorderAll;

         }

         DockManager::~DockManager()
         {

         }


         bool DockManager::_000OnLButtonDown(::message::mouse * pmouse)
         {
            
            if(!m_pworkset->IsDockingEnabled() || m_pworkset->m_bSizingCapture)
               return false;

            class point ptCursor = pmouse->m_pt;

            sp(WorkSetClientInterface) pinterface = m_pworkset->GetEventWindow();

            if(pinterface == NULL)
               pinterface = m_pworkset->get_draw_window();

            if(pinterface->WfiOnBeginMoving(ptCursor))
               return true;

            m_ptCursorOrigin = ptCursor;

            rect rectWindow;

            if(GetDockWindow()->get_appearance() != ::user::appearance_normal)
            {

               GetDockWindow()->_001WindowRestore();

            }

            if(GetDockWindow().cast < ::user::box >() != NULL)
            {

               GetDockWindow().cast < ::user::box >()->WindowDataEnableSaveWindowRect(false);

            }

            m_pworkset->get_draw_window()->GetWindowRect(rectWindow);

            m_ptWindowOrigin = rectWindow.top_left();

            m_pworkset->get_draw_window()->GetWindowRect(rectWindow);

            m_sizeOrigin = rectWindow.size();

            GetEventWindow()->SetCapture();

            m_eappearanceOrigin = GetDockWindow()->get_appearance();

            m_bDocking = true;

            pmouse->m_bRet = true;

            return true;

         }


         bool DockManager::_000OnMouseMove(::message::mouse * pmouse)
         {

            if(!m_pworkset->IsDockingEnabled() || m_pworkset->m_bSizingCapture)
               return false;

            return Relay(pmouse);

         }


         bool DockManager::_000OnLButtonUp(::message::mouse * pmouse)
         {
            
            if(!m_pworkset->IsDockingEnabled() || m_pworkset->m_bSizingCapture)
               return false;

            return Relay(pmouse);

         }


         // process only WM_MOUSEMOVE and WM_LBUTTONUP messages
         bool DockManager::Relay(::message::mouse * pmouse)
         {

            ASSERT(pmouse->m_id == WM_MOUSEMOVE || pmouse->m_id == WM_LBUTTONUP || pmouse->m_id == WM_NCMOUSEMOVE || pmouse->m_id == WM_NCLBUTTONUP);
            
            if(!m_bDocking)
               return false;

            pmouse->m_bRet = true;

            sp(::user::interaction) puieCapture = Session.GetCapture();

            if(puieCapture == NULL)
            {

#ifdef LINUX

               // for safety in Linux
               ::ReleaseCapture();

#endif

               m_bDocking = false;

               return false;

            }

            sp(::user::interaction) puieEventWindow = GetEventWindow();

            if(puieCapture != puieEventWindow)
            {

               if(puieCapture != NULL && puieCapture == GetEventWindow())
               {

                  TRACE("DockManager::message_handler oswindow ReleaseCapture %x\n",Session.GetCapture().m_p);

                  Session.ReleaseCapture();

               }

               return false;

            }

            class point ptCursor = pmouse->m_pt;

            if(pmouse->m_id == WM_MOUSEMOVE || pmouse->m_id == WM_LBUTTONUP)
            {

               //GetEventWindow()->ClientToScreen(&ptCursor);

            }

            if(pmouse->m_id == WM_LBUTTONUP)
            {
               if(GetDockWindow().cast < ::user::box >() != NULL)
               {

                  GetDockWindow().cast < ::user::box >()->WindowDataEnableSaveWindowRect(true);

               }
            }


            class point pt;

            pt = m_ptWindowOrigin + ptCursor - m_ptCursorOrigin;

            //TRACE("m_ptWindowOrigin.x = %d m_ptWindowOrigin = %d\n", m_ptWindowOrigin.x, m_ptWindowOrigin.y);
            //TRACE("ptCursor.x = %d ptCursor = %d\n", ptCursor.x, ptCursor.y);
            //TRACE("m_ptCursorOrigin.x = %d m_ptCursorOrigin = %d\n", m_ptCursorOrigin.x, m_ptCursorOrigin.y);

            rect rectWindow;

            m_pworkset->GetWndDraw()->GetWindowRect(rectWindow);

            bool bMove = true;

            //      sp(::user::interaction) pWndParent = m_pworkset->GetWndDraw()->GetParent();
            //::ReleaseCapture();
            //::ShowWindow(m_pworkset->GetWndDraw()->GetWindow()->get_os_data(), SW_HIDE);

            rect rectEvent = rectWindow;

            rectEvent.move_to(pt);

            rect rectCursor(ptCursor.x - 1,ptCursor.y - 1,ptCursor.x + 1,ptCursor.y + 1);

            rect screen;

            rect rectWork;

            int iMonitor = (int) Session.get_best_monitor(screen,rectCursor);

            System.get_wkspace_rect(iMonitor, rectWork);

            int cx2 =  screen.width() / 3;
            int cy2 =  screen.height() / 3;

//            int cx =  rectWork.width() / 3;
  //          int cy =  rectWork.height() / 3;

            if((ptCursor.x >= screen.left && ptCursor.x - screen.left <= cx2) || (ptCursor.x >= screen.center().x - cx2 && ptCursor.x <= screen.center().x + cx2) || (ptCursor.x >= screen.right - cx2 && ptCursor.x <= screen.right))
            {
               if((ptCursor.y >= screen.top && ptCursor.y - screen.top <= cx2) || (ptCursor.y >= screen.center().y - cy2 && ptCursor.y <= screen.center().y + cy2) || (ptCursor.y >= screen.bottom - cx2 && ptCursor.y <= screen.bottom))
               {
                  if((ptCursor.x >= screen.center().x - cx2 && ptCursor.x <= screen.center().x + cx2))
                  {
                     if((ptCursor.y >= screen.center().y - cy2 && ptCursor.y <= screen.center().y + cy2))
                     {
                        // ignore
                        if(bMove && rectWindow.top_left() != pt)
                        {
                           m_eappearanceOrigin = ::user::appearance_normal;
                           GetDockWindow()->set_appearance(::user::appearance_normal);
                        }
                     }
                     else if(ptCursor.y >= screen.top && ptCursor.y - screen.top <= cx2)
                     {
//                        if(m_eappearanceOrigin != ::user::appearance_top)
                        {
                           GetDockWindow()->set_appearance(::user::appearance_top);
                           ::rect rectDock = rect_dim(rectWork.left,rectWork.top,rectWork.width(),rectWork.height() / 2);
                           GetDockWindow()->SetWindowPos(ZORDER_TOP,rectDock,SWP_SHOWWINDOW);
                        }
                     }
                     else
                     {
  //                      if(m_eappearanceOrigin != ::user::appearance_bottom)
                        {
                           GetDockWindow()->set_appearance(::user::appearance_bottom);
                           ::rect rectDock = rect_dim(rectWork.left,rectWork.top + rectWork.height() / 2,rectWork.width(),rectWork.height() / 2);
                           GetDockWindow()->SetWindowPos(ZORDER_TOP,rectDock,SWP_SHOWWINDOW);
                        }
                     }
                  }
                  else if((ptCursor.y >= screen.center().y - cy2 && ptCursor.y <= screen.center().y + cy2))
                  {
                     if(ptCursor.x >= screen.left && ptCursor.x - screen.left <= cx2)
                     {
                        //if(m_eappearanceOrigin != ::user::appearance_left)
                        {
                           GetDockWindow()->set_appearance(::user::appearance_left);
                           ::rect rectDock = rect_dim(rectWork.left,rectWork.top,rectWork.width() / 2,rectWork.height());
                           GetDockWindow()->SetWindowPos(ZORDER_TOP,rectDock,SWP_SHOWWINDOW);
                        }
                     }
                     else
                     {
                        //if(m_eappearanceOrigin != ::user::appearance_right)
                        {
                           GetDockWindow()->set_appearance(::user::appearance_right);
                           ::rect rectDock = rect_dim(rectWork.left + rectWork.width() / 2,rectWork.top,rectWork.width() / 2,rectWork.height());
                           GetDockWindow()->SetWindowPos(ZORDER_TOP,rectDock,SWP_SHOWWINDOW);
                        }
                     }
                  }
                  else if(ptCursor.x >= screen.left && ptCursor.x - screen.left <= cx2)
                  {
                     if(ptCursor.y >= screen.top && ptCursor.y - screen.top <= cx2)
                     {
                        //if(m_eappearanceOrigin != ::user::appearance_top_left)
                        {
                           GetDockWindow()->set_appearance(::user::appearance_top_left);
                           ::rect rectDock = rect_dim(rectWork.left,rectWork.top,rectWork.width() / 2,rectWork.height() / 2);
                           GetDockWindow()->SetWindowPos(ZORDER_TOP,rectDock,SWP_SHOWWINDOW);
                        }
                     }
                     else
                     {
                        //if(m_eappearanceOrigin != ::user::appearance_bottom_left)
                        {
                           GetDockWindow()->set_appearance(::user::appearance_bottom_left);
                           ::rect rectDock = rect_dim(rectWork.left,rectWork.top + rectWork.height() / 2,rectWork.width() / 2,rectWork.height() / 2);
                           GetDockWindow()->SetWindowPos(ZORDER_TOP,rectDock,SWP_SHOWWINDOW);
                        }
                     }
                  }
                  else
                  {
                     if(ptCursor.y >= screen.top && ptCursor.y - screen.top <= cx2)
                     {
                        //if(m_eappearanceOrigin != ::user::appearance_top_right)
                        {
                           GetDockWindow()->set_appearance(::user::appearance_top_right);
                           ::rect rectDock = rect_dim(rectWork.left + rectWork.width() / 2,rectWork.top,rectWork.width() / 2,rectWork.height() / 2);
                           GetDockWindow()->SetWindowPos(ZORDER_TOP,rectDock,SWP_SHOWWINDOW);
                        }
                     }
                     else
                     {
                        //if(m_eappearanceOrigin != ::user::appearance_bottom_right)
                        {
                           GetDockWindow()->set_appearance(::user::appearance_bottom_right);
                           ::rect rectDock = rect_dim(rectWork.left + rectWork.width() / 2,rectWork.top + rectWork.height() / 2,rectWork.width() / 2,rectWork.height() / 2);
                           GetDockWindow()->SetWindowPos(ZORDER_TOP,rectDock,SWP_SHOWWINDOW);
                        }
                     }
                  }
               }
               else
               {
                  if(bMove && rectWindow.top_left() != pt)
                  {
                     m_eappearanceOrigin = ::user::appearance_normal;
                     GetDockWindow()->set_appearance(::user::appearance_normal);
                  }
               }
            }
            else
            {
               if(bMove && rectWindow.top_left() != pt)
               {
                  m_eappearanceOrigin = ::user::appearance_normal;
                  GetDockWindow()->set_appearance(::user::appearance_normal);
               }
            }


            if(bMove && rectWindow.top_left() != pt && GetDockWindow()->get_appearance() == ::user::appearance_normal)
            {
               class point ptMove = pt;
               if(GetDockWindow()->GetParent() != NULL)
               {
                  GetDockWindow()->GetParent()->ScreenToClient(&ptMove);
               }
               GetDockWindow()->SetWindowPos(ZORDER_TOP,ptMove.x,ptMove.y,m_sizeOrigin.cx,m_sizeOrigin.cy,SWP_SHOWWINDOW);
            }



            sp(WorkSetClientInterface) pinterface = m_pworkset->GetEventWindow();

            if(pinterface == NULL)
               pinterface = m_pworkset->get_draw_window();

            pinterface->WfiOnMove(pmouse->m_id == WM_MOUSEMOVE || pmouse->m_id == WM_NCMOUSEMOVE);

            if(pmouse->m_id == WM_LBUTTONUP || pmouse->m_id == WM_NCLBUTTONUP)
            {
               TRACE("DockManager::message_handler oswindow ReleaseCapture 2 %x\n",Session.GetCapture().m_p);
               index iMatchingMonitor = m_pworkset->GetWndDraw()->good_move(rectEvent,null_rect(),true);

               if(iMatchingMonitor >= 0)
               {

                  bMove = false;

                  m_pworkset->GetWndDraw()->GetWindowRect(rectEvent);

                  ptCursor = -m_ptWindowOrigin + rectEvent.top_left() + m_ptCursorOrigin;

                  if(Session.m_bSystemSynchronizedCursor)
                  {
#ifdef WINDOWSEX
                     ::SetCursorPos(ptCursor.x,ptCursor.y);
#else
                     throw todo(get_app());
#endif
                  }

                  Session.m_ptCursor = ptCursor;

               }
               Session.ReleaseCapture();
               m_bDocking = false;
            }
            return true;
         }

         bool DockManager::update(WorkSet * pwf)
         {
            m_pworkset = pwf;
            return true;
         }

         bool DockManager::relay_event(MESSAGE *lpMsg)
         {
            UNREFERENCED_PARAMETER(lpMsg);
            ASSERT(FALSE);
            return false;
         }

         void DockManager::SetSWPFlags(UINT uiFlags)
         {
            m_uiSWPFlags = uiFlags;
            m_uiSWPFlags |= SWP_NOSIZE;
            m_uiSWPFlags |= SWP_FRAMECHANGED;
            m_uiSWPFlags &= ~SWP_NOMOVE;

         }





         void DockManager::MoveWindow(void * oswindow,point pt)
         {

            sp(::user::interaction) pwnd = System.ui_from_handle(oswindow);

            rect rectWindow;
            pwnd->GetWindowRect(rectWindow);


            pwnd->SetWindowPos(ZORDER_TOP,pt.x,pt.y,rectWindow.width(),rectWindow.height(),0);


            m_pworkset->m_pframeschema->OnMove(GetDockWindow());

         }


         bool DockManager::IsMoving()
         {

            return m_bDocking;

         }

         
         EBorder DockManager::GetBorderMask()
         {

            return m_eborderMask;

         }


         void DockManager::SetBorderMask(EBorder emask)
         {
            m_eborderMask = emask;
         }


         EDock DockManager::GetDockState()
         {

            return m_edockState;

         }


         void DockManager::SetDockMask(EDock edock)
         {
            m_edockState= edock;
         }


         void DockManager::message_handler(::user::interaction * pui, ::message::base * pbase)
         {

            if(pbase->m_id == WM_LBUTTONDOWN)
            {
               
               point ptCursor((int16_t)LOWORD(pbase->m_lparam),(int16_t)HIWORD(pbase->m_lparam));
               
               pui->ClientToScreen(&ptCursor);

               m_ptCursorOrigin = ptCursor;

               rect rectWindow;

               GetDockWindow()->GetWindowRect(rectWindow);

               sp(::user::interaction) pWndParent = GetDockWindow()->GetParent();

               if(pWndParent != NULL)
               {
                  pWndParent->ScreenToClient(rectWindow);
               }
               m_ptWindowOrigin = rectWindow.top_left();
               GetEventWindow()->SetCapture();
               m_bDocking = true;
               pbase->m_bRet = true;
               return;
            }
            else if(pbase->m_id == WM_MOUSEMOVE ||
               pbase->m_id == WM_LBUTTONUP)
            {
               sp(::user::interaction) pWndCapture = Session.GetCapture();
               TRACE("DockManager::message_handler oswindow Capture %x\n",Session.GetCapture().m_p);
               if(!m_bDocking ||
                  pWndCapture == NULL ||
                  pWndCapture->get_handle() != GetEventWindow()->get_handle())
               {
                  if(pWndCapture != NULL
                     && pWndCapture->get_handle() == GetEventWindow()->get_handle())
                  {
                     Session.ReleaseCapture();
                  }
                  return;
               }
               //           uint32_t fwKeys = pbase->m_wparam;        // key flags
               
               point ptCursor((int16_t)LOWORD(pbase->m_lparam),(int16_t)HIWORD(pbase->m_lparam));
               
               pui->ClientToScreen(&ptCursor);

               point pt;
               pt = m_ptWindowOrigin + ptCursor - m_ptCursorOrigin;
               //TRACE("m_ptWindowOrigin.x = %d m_ptWindowOrigin = %d\n", m_ptWindowOrigin.x, m_ptWindowOrigin.y);
               //TRACE("ptCursor.x = %d ptCursor = %d\n", ptCursor.x, ptCursor.y);
               //TRACE("m_ptCursorOrigin.x = %d m_ptCursorOrigin = %d\n", m_ptCursorOrigin.x, m_ptCursorOrigin.y);
               rect rectWindow;
               GetEventWindow()->GetWindowRect(rectWindow);
               bool bMove = true;
               sp(::user::interaction) pWndParent = GetDockWindow()->GetParent();
               if(pWndParent == NULL)
                  pWndParent = System.get_desktop_window();
               if(pWndParent != NULL)
               {
                  pWndParent->ScreenToClient(rectWindow);
                  rect rectParentClient;
                  pWndParent->GetClientRect(rectParentClient);
                  rect rectEvent;
                  GetEventWindow()->GetWindowRect(rectEvent);
                  pWndParent->ScreenToClient(rectEvent);
                  rectEvent += pt - rectWindow.top_left();
                  rect rectIntersect;
                  rectIntersect.intersect(rectParentClient,rectEvent);
                  if(rectIntersect.width() <= 30 ||
                     rectIntersect.height() <= 30)
                     bMove = false;
               }

               if(bMove && rectWindow.top_left() != pt)
               {

                  MoveWindow(GetDockWindow()->get_handle(),pt);

               }
               if(pbase->m_id == WM_LBUTTONUP)
               {
                  Session.ReleaseCapture();
                  m_bDocking = false;
               }
               pbase->m_bRet = true;
               return;
            }
         }

         bool DockManager::_000OnTimer(UINT nIDEvent)
         {
            return false;
         }

         sp(::user::interaction) DockManager::GetEventWindow()
         {
            return m_pworkset->GetEventWindow();
         }

         sp(::user::interaction) DockManager::GetDockWindow()
         {
            return m_pworkset->get_draw_window();
         }


      } // namespace frame


   } // namespace wndfrm






} // namespace user












