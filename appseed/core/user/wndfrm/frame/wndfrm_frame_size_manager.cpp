#include "framework.h"


extern CLASS_DECL_CORE thread_int_ptr < DWORD_PTR > t_time1;
namespace user
{

   namespace wndfrm
   {

      namespace frame
      {


         SizeManager::SizeManager(WorkSet * pworkset) :
            object(pworkset->get_app()),
            m_minBorder(33, 33)
         {

            ASSERT(pworkset != NULL);

            m_pworkset = pworkset;

            m_ehittestMode = HitTestNone;

            m_ehittestCursor = HitTestNone;

            SetSWPFlags(0);

            m_egripMask = GripAll;

         }


         SizeManager::~SizeManager()
         {

         }


         bool SizeManager::_000OnLButtonDown(::message::mouse * pmouse)
         {

            ASSERT(pmouse->m_id == WM_LBUTTONDOWN || pmouse->m_id == WM_NCLBUTTONDOWN);

            if(!m_pworkset->IsSizingEnabled())
            {

               return false;

            }

            class point ptCursor = pmouse->m_pt;

            m_ptCursorOrigin = ptCursor;

            rect rectWindow;

            GetSizingWindow()->GetWindowRect(rectWindow);

            m_rcWindowOrigin = rectWindow;

            rect rectEvent;

            GetEventWindow()->GetWindowRect(rectEvent);

            //EHitTest ehittest = hit_test(ptCursor);

            EHitTest ehittest = m_ehittestCursor;

            sp(WorkSetClientInterface) pinterface = m_pworkset->get_draw_window();

            if(ehittest != HitTestNone)
            {

               UINT nHitTest = HTCLIENT;

               switch(ehittest)
               {
               case HitTestSizingLeft:
                  nHitTest = HTLEFT;
                  break;
               case HitTestSizingTopLeft:
                  nHitTest = HTTOPLEFT;
                  break;
               case HitTestSizingTop:
                  nHitTest = HTTOP;
                  break;
               case HitTestSizingTopRight:
                  nHitTest = HTTOPRIGHT;
                  break;
               case HitTestSizingRight:
                  nHitTest = HTRIGHT;
                  break;
               case HitTestSizingBottomRight:
                  nHitTest = HTBOTTOMRIGHT;
                  break;
               case HitTestSizingBottom:
                  nHitTest = HTBOTTOM;
                  break;
               case HitTestSizingBottomLeft:
                  nHitTest = HTBOTTOMLEFT;
                  break;
               default:
                  break;
               }

               if(pinterface->WfiOnBeginSizing(nHitTest, ptCursor))
                  return true;
            }

            if(ehittest != HitTestNone)
            {
               // SizeManager captures the mouse and
               // now have priority with mouse messages
               m_pworkset->m_bSizingCapture = true;

               rect r;

               GetEventWindow()->GetClientRect(r);

               GetEventWindow()->ClientToScreen(r);

               point p;

               Session.get_cursor_pos(p);

               //point pNew(p);

               //if (pNew.x < r.left)
               //   pNew.x = r.left;
               //else if (pNew.x >= r.right)
               //   pNew.x = r.right - 1;
               //if (pNew.y < r.top)
               //   pNew.y = r.top;
               //else if (pNew.y >= r.bottom)
               //   pNew.y = r.bottom - 1;

               //SetCursorPos(pNew.x, pNew.y);

               GetEventWindow()->SetCapture();

//               SetCursorPos(p.x, p.y);

               pmouse->m_bRet = true;
            }
            else
            {
               // SizeManager release mouse capture and
               // currently, does not have priority with
               // mouse messages
               m_pworkset->m_bSizingCapture = false;
               Session.ReleaseCapture();
            }

            pmouse->m_ecursor = translate(ehittest);

            m_ehittestCursor = HitTestNone;

            m_ehittestMode = ehittest;

            if(m_ehittestMode == HitTestNone)
            {

               return false;

            }
            else
            {
               m_iPaintCount = 0;
               return true;
            }
         }


         bool SizeManager::_000OnMouseMove(::message::mouse * pmouse)
         {

            if(!m_pworkset->IsSizingEnabled())
               return false;

            ASSERT(pmouse->m_id == WM_MOUSEMOVE || pmouse->m_id == WM_NCMOUSEMOVE);

            if(m_ehittestMode != HitTestNone)
            {

               SizeWindow(GetSizingWindow(), pmouse->m_pt, true);

               pmouse->m_ecursor = translate(m_ehittestMode);

               pmouse->set_lresult(1);

               pmouse->m_bRet = true;

               return true;

            }

            m_ehittestCursor = hit_test(pmouse->m_pt);

            if(m_ehittestCursor != HitTestNone)
            {

               pmouse->m_ecursor = translate(m_ehittestCursor);

               pmouse->set_lresult(1);

               pmouse->m_bRet = true;

               return true;

            }

            return false;

         }


         bool SizeManager::_000OnLButtonUp(::message::mouse * pmouse)
         {

            if(!m_pworkset->IsSizingEnabled())
            {

               return false;

            }

            Session.ReleaseCapture();

            ASSERT(pmouse->m_id == WM_LBUTTONUP || pmouse->m_id == WM_NCLBUTTONUP);

            if(m_ehittestMode != HitTestNone)
            {

               pmouse->m_bRet = true;

               SizeWindow(GetSizingWindow(), pmouse->m_pt, false);

               m_ehittestMode = HitTestNone;

               m_pworkset->m_bSizingCapture = false;

               Session.set_cursor(GetSizingWindow(),::visual::cursor_default);

               return true;

            }

            return false;

         }


         bool SizeManager::Relay(::message::mouse * pmouse)
         {
            UNREFERENCED_PARAMETER(pmouse);
            return false;
         }

         ::visual::e_cursor SizeManager::translate(EHitTest emode)
         {
            switch(emode)
            {
            case HitTestNone:
               return ::visual::cursor_default;
            case HitTestSizingTopLeft:
               return ::visual::cursor_size_top_left;
            case HitTestSizingTop:
               return ::visual::cursor_size_top;
            case HitTestSizingTopRight:
               return ::visual::cursor_size_top_right;
            case HitTestSizingRight:
               return ::visual::cursor_size_right;
            case HitTestSizingBottomRight:
               return ::visual::cursor_size_bottom_right;
            case HitTestSizingBottom:
               return ::visual::cursor_size_bottom;
            case HitTestSizingBottomLeft:
               return ::visual::cursor_size_bottom_left;
            case HitTestSizingLeft:
               return ::visual::cursor_size_left;
               break;
            default:
               return ::visual::cursor_default;
            }
         }


         ::size SizeManager::GetMinSize()
         {

            return m_pworkset->m_pframeschema->GetMinSize();

         }


         void SizeManager::SetSWPFlags(UINT uiFlags)
         {
            m_uiSWPFlags = uiFlags;
            m_uiSWPFlags &= ~SWP_NOSIZE & ~SWP_NOMOVE;

         }

         void SizeManager::SizeWindow(sp(::user::interaction)pwnd, point pt, bool bTracking)
         {

            UNREFERENCED_PARAMETER(pwnd);

            bool bSize = true;

            rect rectWindow;

            class rect rectMonitor;

            pwnd->best_monitor(rectMonitor);

            int xPad = -4;
            int yPad = -4;

            ::size sizeMin = GetMinSize();

            if(m_ehittestMode == HitTestSizingTopLeft)
            {
               rectWindow.top = pt.y + xPad;
               rectWindow.left = pt.x + yPad;
               rectWindow.bottom = m_rcWindowOrigin.bottom;
               rectWindow.right = m_rcWindowOrigin.right;
               bSize = rectWindow.width() > 0 &&
                       rectWindow.height() > 0;
               if(rectWindow.width() < sizeMin.cx)
               {
                  rectWindow.left = m_rcWindowOrigin.right - sizeMin.cx;
               }
               if(rectWindow.height() < sizeMin.cy)
               {
                  rectWindow.top = m_rcWindowOrigin.bottom - sizeMin.cy;
               }
               rectWindow.left = MIN(rectWindow.left, rectMonitor.right - m_minBorder.cx);
               rectWindow.top = MIN(rectWindow.top, rectMonitor.bottom - m_minBorder.cy);
            }
            else if(m_ehittestMode == HitTestSizingTop)
            {
               rectWindow.top = pt.y + yPad;
               rectWindow.left = m_rcWindowOrigin.left;
               rectWindow.bottom = m_rcWindowOrigin.bottom;
               rectWindow.right = m_rcWindowOrigin.right;
               if(rectWindow.height() < sizeMin.cy)
               {
                  rectWindow.top = m_rcWindowOrigin.bottom - sizeMin.cy;
               }
               rectWindow.top = MIN(rectWindow.top, rectMonitor.bottom - m_minBorder.cy);
               sp(WorkSetClientInterface) pinterface = m_pworkset->get_draw_window();
               if (pinterface->oprop("ysnap").i32() > 1)
               {
                  rectWindow.top -= rectWindow.top % pinterface->oprop("ysnap").i32();
               }
            }
            else if(m_ehittestMode == HitTestSizingTopRight)
            {
               rectWindow.top = pt.y + yPad;
               rectWindow.left = m_rcWindowOrigin.left;
               rectWindow.bottom = m_rcWindowOrigin.bottom;
               rectWindow.right = pt.x - xPad;
               if(rectWindow.width() < sizeMin.cx)
               {
                  rectWindow.right = m_rcWindowOrigin.left + sizeMin.cx;

               }
               if(rectWindow.height() < sizeMin.cy)
               {
                  rectWindow.top = m_rcWindowOrigin.bottom - sizeMin.cy;
               }
               rectWindow.right = MAX(rectWindow.right, rectMonitor.left + m_minBorder.cx);
               rectWindow.top = MIN(rectWindow.top, rectMonitor.bottom - m_minBorder.cy);
            }
            else if(m_ehittestMode == HitTestSizingRight)
            {
               rectWindow.top = m_rcWindowOrigin.top;
               rectWindow.left = m_rcWindowOrigin.left;
               rectWindow.bottom = m_rcWindowOrigin.bottom;
               rectWindow.right = pt.x - xPad;
               if(rectWindow.width() < sizeMin.cx)
               {
                  rectWindow.right = m_rcWindowOrigin.left + sizeMin.cx;

               }
               rectWindow.right = MAX(rectWindow.right, rectMonitor.left + m_minBorder.cx);
            }
            else if(m_ehittestMode == HitTestSizingBottomRight)
            {
               rectWindow.top = m_rcWindowOrigin.top;
               rectWindow.left = m_rcWindowOrigin.left;
               rectWindow.bottom = pt.y - yPad;
               rectWindow.right = pt.x - xPad;
               if(rectWindow.width() < sizeMin.cx)
               {
                  rectWindow.right = m_rcWindowOrigin.left + sizeMin.cx;

               }
               if(rectWindow.height() < sizeMin.cy)
               {
                  rectWindow.bottom = m_rcWindowOrigin.top + sizeMin.cy;
               }
               rectWindow.right = MAX(rectWindow.right, rectMonitor.left + m_minBorder.cx);
               rectWindow.bottom = MAX(rectWindow.bottom, rectMonitor.top + m_minBorder.cy);
            }
            else if(m_ehittestMode == HitTestSizingBottom)
            {
               rectWindow.top = m_rcWindowOrigin.top;
               rectWindow.left = m_rcWindowOrigin.left;
               rectWindow.bottom = pt.y - yPad;
               rectWindow.right = m_rcWindowOrigin.right;
               if(rectWindow.height() < sizeMin.cy)
               {
                  rectWindow.bottom = m_rcWindowOrigin.top + sizeMin.cy;
               }
               rectWindow.bottom = MAX(rectWindow.bottom, rectMonitor.top + m_minBorder.cy);
               sp(WorkSetClientInterface) pinterface = m_pworkset->get_draw_window();

               if (pinterface->oprop("ysnap").i32() > 1)
               {
                  int h = rectWindow.height();
                  h -= rectWindow.height() % pinterface->oprop("ysnap").i32();
                  rectWindow.bottom = rectWindow.top + h;
               }
            }
            else if(m_ehittestMode == HitTestSizingBottomLeft)
            {
               rectWindow.top = m_rcWindowOrigin.top;
               rectWindow.left = pt.x + xPad;
               rectWindow.bottom = pt.y - yPad;
               rectWindow.right = m_rcWindowOrigin.right;
               if(rectWindow.width() < sizeMin.cx)
               {
                  rectWindow.left = m_rcWindowOrigin.right - sizeMin.cx;

               }
               if(rectWindow.height() < sizeMin.cy)
               {
                  rectWindow.bottom = m_rcWindowOrigin.top + sizeMin.cy;
               }
               rectWindow.left = MIN(rectWindow.left, rectMonitor.right - m_minBorder.cx);
               rectWindow.bottom = MAX(rectWindow.bottom, rectMonitor.top + m_minBorder.cy);
            }
            else if(m_ehittestMode == HitTestSizingLeft)
            {
               rectWindow.top = m_rcWindowOrigin.top;
               rectWindow.left = pt.x + xPad;
               rectWindow.bottom = m_rcWindowOrigin.bottom;
               rectWindow.right = m_rcWindowOrigin.right;
               if(rectWindow.width() < sizeMin.cx)
               {
                  rectWindow.left = m_rcWindowOrigin.right - sizeMin.cx;

               }
               rectWindow.left = MIN(rectWindow.left, rectMonitor.right - m_minBorder.cx);
            }
            else
            {
               bSize = false;
            }
            {

//               DWORD dwTime2 = ::get_tick_count();

               //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
               //TRACE("SizeManager::Mv call timeA= %d ms",dwTime2 - t_time1.operator DWORD_PTR());

            }






            MoveWindow(GetSizingWindow(), rectWindow);

            {

//               DWORD dwTime2 = ::get_tick_count();

               //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
               //TRACE("SizeManager::Mv call time1= %d ms",dwTime2 - t_time1.operator DWORD_PTR());

            }






            sp(WorkSetClientInterface) pinterface = m_pworkset->get_draw_window();
            pinterface->WfiOnSize(bTracking);
            NotifyFramework((EHitTest)m_ehittestMode);
            {

//               DWORD dwTime2 = ::get_tick_count();

               //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
               //TRACE("SizeManager::Mv call time3= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
            }
         }


         void SizeManager::MoveWindow(sp(::user::interaction)pwnd, const RECT & rect)
         {

            ::rect rectWindow = rect;

            ::size sizeMin = GetMinSize();

            if(!(rectWindow.width() >= sizeMin.cx  &&
                  rectWindow.height() >= sizeMin.cy))
               return;


            ::rect rectWindowNow;
            pwnd->GetWindowRect(rectWindowNow);
            if(rectWindowNow == rectWindow)
               return;

            //      ::user::interaction_base * ptwi = dynamic_cast<::user::interaction_base *>(pwnd);
            //      if(ptwi != NULL)
            if(true)
            {
               //if(pwnd->GetParent() == NULL)
               if(true)
               {

                  m_pworkset->_000OnBeforeSize(rectWindow);

                  if(m_pworkset->GetWndDraw()->WfiIsZoomed())
                  {

                     m_pworkset->GetWndDraw()->WfiRestore(true);

                  }

                  ::rect rectBefore;
                  pwnd->GetWindowRect(rectBefore);
                  ::rect rectAfter = rectWindow;
                  ::rect rectParentClient = rectAfter;
                  if(m_pworkset->GetWndDraw()->GetParent() != NULL)
                  {
                     m_pworkset->GetWndDraw()->GetParent()->ScreenToClient(rectParentClient);
                  }

                  ::rect rectTotal;
                  rectTotal.unite(rectBefore, rectAfter);
                  m_pworkset->m_rectPending.unite(rectBefore, rectAfter);
                  {

//                     DWORD dwTime2 = ::get_tick_count();

                     //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
                     //TRACE("SizeManager::Mv call timex= %d ms",dwTime2 - t_time1.operator DWORD_PTR());

                  }


                  pwnd->SetWindowPos(
                  ZORDER_TOP,
                  rectParentClient.left,
                  rectParentClient.top,
                  rectParentClient.width(),
                  rectParentClient.height(),
                  (m_uiSWPFlags | SWP_NOZORDER) & ~SWP_SHOWWINDOW);

                  {

//                     DWORD dwTime2 = ::get_tick_count();

                     //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
                     //TRACE("SizeManager::Mv call timey= %d ms",dwTime2 - t_time1.operator DWORD_PTR());

                  }


               }
//               else
//               {
//
//                  ::rect rectParentBefore;
//                  pwnd->GetWindowRect(rectParentBefore);
//                  pwnd->SetWindowPos(ZORDER_TOP, rectWindow.left, rectWindow.top, rectWindow.width(), rectWindow.height(), m_uiSWPFlags | SWP_NOREDRAW);
//
//                  ::rect rectParentAfter;
//                  pwnd->GetWindowRect(rectParentAfter);
//
//                  ::rect rectParent;
//                  rectParent.unite(rectParentBefore, rectParentAfter);
//                  pwnd->ScreenToClient(rectParent);
//
//                  TRACE("CWndMovingManager::relay_event\n");
//                  TRACE("Before.left %d\n", rectParentBefore.left);
//                  TRACE("Before.right %d\n", rectParentBefore.right);
//                  TRACE("Before.top %d\n", rectParentBefore.top);
//                  TRACE("Before.bottom %d\n", rectParentBefore.bottom);
//
//                  TRACE("CWndMovingManager::relay_event\n");
//                  TRACE("After.left %d\n", rectParentAfter.left);
//                  TRACE("After.right %d\n", rectParentAfter.right);
//                  TRACE("After.top %d\n", rectParentAfter.top);
//                  TRACE("After.bottom %d\n", rectParentAfter.bottom);
//
//                  TRACE("CWndMovingManager::relay_event\n");
//                  TRACE("union.left %d\n", rectParent.left);
//                  TRACE("union.right %d\n", rectParent.right);
//                  TRACE("union.top %d\n", rectParent.top);
//                  TRACE("union.bottom %d\n", rectParent.bottom);
//
//                  pwnd->set_need_redraw();
//               }
            }
//            else
//            {
//               pwnd->SetWindowPos(
//                  ZORDER_TOP,
//                  rectWindow.
//                  left, rectWindow.top,
//                  rectWindow.width(),
//                  rectWindow.height(),
//                  m_uiSWPFlags);
//            }

         }

         bool SizeManager::IsSizing()
         {
            return m_ehittestMode != HitTestNone;
         }

         void SizeManager::SetGripMask(EGrip egrip)
         {
            m_egripMask = egrip;
         }

         EGrip SizeManager::GetGripMask()
         {
            return m_egripMask;
         }

         EGrip SizeManager::EModeToEGrip(EHitTest emode)
         {
            EGrip egrip;
            if(emode == HitTestSizingTop)
               egrip = GripCenterTop;
            else if(emode == HitTestSizingTopLeft)
               egrip = GripTopLeft;
            else if(emode == HitTestSizingLeft)
               egrip = GripCenterLeft;
            else if(emode == HitTestSizingBottomLeft)
               egrip = GripBottomLeft;
            else if(emode == HitTestSizingBottom)
               egrip = GripCenterBottom;
            else if(emode == HitTestSizingBottomRight)
               egrip = GripBottomRight;
            else if(emode == HitTestSizingRight)
               egrip = GripCenterRight;
            else if(emode == HitTestSizingTopRight)
               egrip = GripTopRight;
            else
               egrip = GripNone;
            return egrip;
         }

         void SizeManager::NotifyFramework(EHitTest emode)
         {
            UNREFERENCED_PARAMETER(emode);
         }



         EHitTest SizeManager::hit_test(point ptCursor)
         {
            EHitTest ehittest = m_pworkset->hit_test(ptCursor);
            switch(ehittest)
            {
            case HitTestSizingLeft:
            case HitTestSizingTopLeft:
            case HitTestSizingTop:
            case HitTestSizingTopRight:
            case HitTestSizingRight:
            case HitTestSizingBottomRight:
            case HitTestSizingBottom:
            case HitTestSizingBottomLeft:
               return ehittest;
            default:
               return HitTestNone;
            }
         }

         sp(::user::interaction) SizeManager::GetSizingWindow()
         {
            return m_pworkset->GetRegionWindow();
         }

         sp(::user::interaction) SizeManager::GetEventWindow()
         {
            return m_pworkset->GetEventWindow();
         }

         bool SizeManager::update(WorkSet * pset)
         {
            m_pworkset = pset;
            return true;
         }


      } // namespace frame


   } // namespace wndfrm


} // namespace user


