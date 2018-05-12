#include "framework.h"

extern CLASS_DECL_CORE thread_int_ptr < DWORD_PTR > t_time1;

namespace user
{


   const int32_t split_layout::m_iMarging = 5;

   split_layout::split_layout():
      split_layout(get_app())
   {

      m_bSplitBar = true;
      m_econtroltype = control_type_split;

      m_flagNonClient.unsignalize(non_client_background);
      m_flagNonClient.unsignalize(non_client_focus_rect);

   }


   split_layout::split_layout(::aura::application * papp) :
      object(papp),
      m_splitbara(papp),
      m_panea(papp),
      place_holder_container(papp)
   {

      m_flagNonClient.unsignalize(non_client_background);
      m_flagNonClient.unsignalize(non_client_focus_rect);

      m_bInitialized    = true;
      m_iState          = stateInitial;
      m_cxBorder        = 1;
      m_cyBorder        = 1;
      m_iPaneCount      = 0;

   }


   split_layout::~split_layout()
   {

   }


   void split_layout::install_message_routing(::message::sender * pinterface)
   {

      place_holder_container::install_message_routing(pinterface);

      IGUI_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &split_layout::_001OnShowWindow);

   }


   void split_layout::_001OnShowWindow(::message::message * pobj)
   {

      SCAST_PTR(::message::show_window, pshowwindow, pobj);

      bool bIsWindowVisible = pshowwindow->m_bShow;

      ::count iSplitBarCount = get_split_count();

      split_layout::Pane * pcomponent;

      sp(::user::interaction) pwnd;

      int32_t i;

      for (i = 0; i < iSplitBarCount; i++)
      {

         pwnd = m_splitbara.element_at(i);

         if (!bIsWindowVisible)
         {

            pwnd->ShowWindow(SW_HIDE);

         }
         else
         {

            pwnd->ShowWindow(SW_SHOW);

         }

      }

      rect rectClient;

      for (i = 0; i < get_pane_count(); i++)
      {

         rect & rectPane = m_panea[i]->m_rect;

         pcomponent = m_panea.element_at(i);

         pwnd = pcomponent->m_pholder;

         if (pwnd == NULL)
         {

            continue;

         }

         rectClient = rectPane;

         rectClient.deflate(m_cxBorder, m_cyBorder);

         if (rectPane.area() <= 0 || !bIsWindowVisible)
         {

            pwnd->ShowWindow(SW_HIDE);

         }
         else
         {

            pwnd->ShowWindow(SW_SHOW);

         }

      }

   }


   bool split_layout::SetPaneCount(::count iPaneCount)
   {

      m_iPaneCount = iPaneCount;

      m_panea.set_size(iPaneCount);

      m_splitbara.set_size(iPaneCount - 1);

      for(::index i = 0; i < m_splitbara.get_count(); i++)
      {
         if(m_splitbara[i].is_null())
         {
            m_splitbara[i] = canew(::user::split_bar(get_app()));
            ::user::split_bar & splitbar = m_splitbara(i);
            splitbar.m_iIndex = (int)i;
            if(!splitbar.create_window(null_rect(), this))
               return false;
            splitbar.m_dwPosition = 0;
            splitbar.m_dwPosition = 0;
         }
      }

      for(::index i = 0; i < m_panea.get_count(); i++)
      {
         if(m_panea[i].is_null())
         {
            m_panea[i] = canew(::user::split_layout::Pane(get_app()));
            m_panea(i).m_bFixedSize =  false;
         }
      }

      return true;

   }


   bool split_layout::initialize_split_layout()
   {

      ::count iPaneCount = m_iPaneCount;

      m_bInitialized = false;

      ASSERT(iPaneCount > 0);

      on_layout();

      rect rectPane;

      for(::index i = 0; i < m_panea.get_count(); i++)
      {

         if(m_panea[i]->m_pholder.is_null())
         {

            m_panea[i]->m_pholder = get_new_place_holder(m_panea[i]->m_rectClient);

         }

      }

      m_bInitialized = true;

      return true;

   }


   void split_layout::SetSplitOrientation(e_orientation eorientationSplit)
   {

      m_eorientationSplit = eorientationSplit;

   }


   void split_layout::RelayChildEvent(index iIndex, const MESSAGE * lpMsg)
   {

      if(!m_bInitialized)
         return;

      rect splitRect;
      CalcSplitBarRect(iIndex, &splitRect);
      point ptCursor = lpMsg->pt;
      if(lpMsg->message == WM_LBUTTONDOWN)
      {

//         int32_t   fwKeys = (int32_t) lpMsg->wParam;        // key flags
         if(Session.is_mouse_button_pressed(::user::mouse_left_button))
         {
            ::user::split_bar & splitbar = *m_splitbara.element_at(iIndex);
            splitbar.SetCapture();
            m_iIndex = iIndex;
            m_iState = stateDragging;
         }
      }
      else if(lpMsg->message == WM_LBUTTONUP)
      {

         if(m_iState != stateInitial)
         {

            Session.ReleaseCapture();

            m_iState = stateInitial;

         }

      }
      else if(lpMsg->message == WM_CAPTURECHANGED)
      {

      }
      else if(lpMsg->message == WM_MOUSEMOVE)
      {

         int32_t   fwKeys = (int32_t) lpMsg->wParam;        // key flags

         point ptClient = ptCursor;

         ScreenToClient(&ptClient);

         if((fwKeys & MK_LBUTTON) > 0 && (m_iState == stateDragging) && (iIndex == m_iIndex))
         {

            cslock sl(&m_mutex);
            {
               //      TRACE("split_layout::RelayChildEvent LOWORD(lpMsg->lParam) %d\n", LOWORD(lpMsg->lParam));
               //      TRACE("split_layout::RelayChildEvent HIWORD(lpMsg->lParam) %d\n", HIWORD(lpMsg->lParam));



               int32_t nPos;
               bool bMove;
               nPos = GetPos(ptClient.x, ptClient.y);
               if(m_iIndex <= 0)
               {
                  bMove = nPos > GetMinPos();
               }
               else
               {
                  bMove = nPos > (int32_t) m_splitbara[m_iIndex - 1]->m_dwPosition;
               }
               if(get_pane_count() >= m_iIndex )
               {
                  bMove = bMove && nPos < GetMaxPos();
               }
               else
               {
                  bMove = bMove && nPos < (int32_t) m_splitbara[m_iIndex]->m_dwPosition;
               }
               if(bMove)
               {
                  bMove = nPos != (int32_t) m_splitbara[m_iIndex]->m_dwPosition;
               }
               TRACE("split_layout::RelayChildEvent nPos %d\nOldPos", m_splitbara[m_iIndex]->m_dwPosition);
               TRACE("split_layout::RelayChildEvent nPos %d\n", nPos);
               if(bMove)
               {

                  m_splitbara[m_iIndex]->m_dwPosition = nPos;
                  m_splitbara[m_iIndex]->m_dRate = 0.0;
                  on_layout();
               }
               m_mutex.unlock();
            }
         }
         else
         {
            m_iState = stateInitial;
         }
      }

   }


   int32_t split_layout::GetPos(int32_t xPos, int32_t yPos)
   {

      if (m_eorientationSplit == orientation_horizontal)
      {

         return yPos;

      }
      else
      {

         return xPos;

      }

   }


   int32_t split_layout::GetMinPos()
   {

      rect rectClient;

      GetClientRect(rectClient);

      if (m_eorientationSplit == orientation_horizontal)
      {

         return rectClient.top;

      }
      else
      {

         return rectClient.left;

      }

   }


   int32_t split_layout::GetMaxPos()
   {

      rect rectClient;

      GetClientRect(rectClient);

      if (m_eorientationSplit == orientation_horizontal)
      {

         return rectClient.bottom;

      }
      else
      {

         return rectClient.right;

      }

   }


   void split_layout::on_layout()
   {

      bool bIsWindowVisible = IsWindowVisible();

      rect rectClient;

      GetClientRect(rectClient);

      if (rectClient.area() <= 0)
      {

         return;

      }

      int32_t iDimension = get_normal_dimension();

      uint32_t dwPosition;

      double dRate;

      if(iDimension > 0)
      {

         for(int32_t i = 0 ; i < m_splitbara.get_count(); i++)
         {

            if (m_splitbara[i]->m_dRate >= 0.0)
            {

               if ((m_splitbara[i]->m_dRate < m_splitbara[i]->m_dMinimumRate
                     || m_splitbara[i]->m_dRate > m_splitbara[i]->m_dMaximumRate)
                     && m_splitbara[i]->m_dwPosition > 0)
               {

                  dwPosition = m_splitbara[i]->m_dwPosition;

                  dRate = (double)dwPosition / (double)iDimension;

                  m_splitbara[i]->m_dRate = dRate;

               }

               if (m_splitbara[i]->m_dRate < m_splitbara[i]->m_dMinimumRate)
               {

                  m_splitbara[i]->m_dRate = m_splitbara[i]->m_dMinimumRate;

               }
               else if (m_splitbara[i]->m_dRate > m_splitbara[i]->m_dMaximumRate)
               {

                  m_splitbara[i]->m_dRate = m_splitbara[i]->m_dMaximumRate;

               }

               m_splitbara[i]->m_dwPosition = MIN(m_splitbara[i]->m_dwMaxPosition, (uint32_t)(m_splitbara[i]->m_dRate * iDimension));

            }
            else
            {

               m_splitbara[i]->m_dwPosition = MIN(m_splitbara[i]->m_dwMaxPosition, (uint32_t)(m_splitbara[i]->m_dwPosition));

            }

         }

      }

      rect rectBar;

      int32_t i;

      ::count iSplitBarCount = get_split_count();

      split_layout::Pane * pcomponent;

      sp(::user::interaction) pwnd;

      UINT uiBaseFlags = SWP_NOZORDER;

      UINT uiFlags = uiBaseFlags;

      for(i = 0; i < iSplitBarCount; i++)
      {

         pwnd = m_splitbara.element_at(i);

         if (!m_bSplitBar || !is_pane_visible(i) || !bIsWindowVisible || m_panea[i]->m_bFixedSize)
         {

            pwnd->ShowWindow(SW_HIDE);

            continue;

         }

         CalcSplitBarRect(i, &rectBar);

         uiFlags = uiBaseFlags;

         pwnd->SetWindowPos(
         0,
         rectBar.left,
         rectBar.top,
         rectBar.width(),
         rectBar.height(),
         uiFlags & ~(SWP_NOZORDER));

         if (bIsWindowVisible)
         {

            pwnd->ShowWindow(SW_SHOW);

         }

      }

      for(i = 0; i < get_pane_count(); i++)
      {

         if (!is_pane_visible(i))
         {

            continue;

         }

         rect & rectPane = m_panea[i]->m_rect;

         rect & rectClient = m_panea[i]->m_rectClient;

         CalcPaneRect(i,&rectPane);

         pcomponent = m_panea.element_at(i);

         pwnd = pcomponent->m_pholder;

         if (pwnd == NULL)
         {

            continue;

         }

         rectClient = rectPane;

         rectClient.deflate(m_cxBorder,m_cyBorder);

         if (rectPane.area() <= 0 || !bIsWindowVisible)
         {

            pwnd->ShowWindow(SW_HIDE);

         }

         pwnd->SetWindowPos(0, rectClient, uiFlags & ~(SWP_NOZORDER));

         if (rectPane.area() > 0 && bIsWindowVisible)
         {

            pwnd->ShowWindow(SW_SHOW);

         }

      }

   }


   void split_layout::set_position(index iIndex, int32_t nPos)
   {

      ASSERT(iIndex >= 0);

      ASSERT(iIndex < get_split_count());

      if (iIndex < 0 && iIndex >= get_split_count())
      {

         return;

      }

      m_splitbara[iIndex]->m_dwPosition    = nPos;

      m_splitbara[iIndex]->m_dRate         = -1.0; // disable rate evaluation at first

   }


   void split_layout::set_position_rate(index iIndex, double dRate, double dMinimumRate, double dMaximumRate)
   {

      ASSERT(iIndex >= 0);

      ASSERT(iIndex < get_split_count());

      if (iIndex < 0 && iIndex >= get_split_count())
      {

         return;

      }

      m_splitbara[iIndex]->m_dRate         = dRate;
      m_splitbara[iIndex]->m_dMinimumRate  = dMinimumRate;
      m_splitbara[iIndex]->m_dMaximumRate  = dMaximumRate;
      m_splitbara[iIndex]->m_dwPosition    = (uint32_t) -1; // disable position evaluation at first on on_layout

   }


   int32_t split_layout::get_position(index iIndex)
   {

      ASSERT(iIndex >= 0);

      ASSERT(iIndex < get_split_count());

      return m_splitbara[iIndex]->m_dwPosition;

   }


   ::count split_layout::get_split_count()
   {

      return get_pane_count() - 1;

   }


   ::count split_layout::get_pane_count()
   {

      return (int32_t) m_panea.get_count();

   }

   ::count split_layout::get_visible_pane_count()
   {

      if (!m_bInitialized)
      {

         return get_pane_count();

      }

      ::count c = 0;

      for (index i = 0; i < get_pane_count(); i++)
      {

         if (is_pane_visible((int) (i)))
         {

            c++;

         }

      }

      return c;

   }


   bool split_layout::is_pane_visible(int iPane)
   {

      if (iPane < 0)
      {

         return false;

      }

      if (iPane >= m_panea.get_size())
      {

         return false;

      }

      split_layout::Pane * ppane = m_panea[iPane];

      sp(::user::place_holder) pholder = ppane->m_pholder;

      if (pholder == NULL)
      {

         return true; // assume future place_holder by default is visible

      }

      if (pholder->m_uiptraChild.is_empty())
      {

         return true; // assume future child by default is visible

      }

      if (!pholder->m_uiptraChild[0]->is_this_visible())
      {

         return false;

      }

      return true;

   }


   void split_layout::CalcPaneRect(index iPane, LPRECT lpRect)
   {

      int32_t nMinPos = GetMinPos(iPane);

      int32_t nMaxPos = GetMaxPos(iPane);

      CalcPaneRect(nMinPos, nMaxPos, lpRect);

   }

   void split_layout::CalcPaneRect(int32_t nMinPos, int32_t nMaxPos, LPRECT lpRect)
   {

      GetClientRect(lpRect);

      if(m_eorientationSplit == orientation_horizontal)
      {

         lpRect->top      = nMinPos;

         lpRect->bottom   = nMaxPos;

      }
      else
      {

         lpRect->left   = nMinPos;

         lpRect->right   = nMaxPos;

      }

   }


   int32_t split_layout::get_normal_dimension()
   {

      rect rectClient;

      GetClientRect(rectClient);

      if(m_eorientationSplit == orientation_horizontal)
      {

         return rectClient.height();

      }
      else
      {

         return rectClient.width();

      }

   }


   int32_t split_layout::get_ortogonal_dimension()
   {

      rect rectClient;

      GetClientRect(rectClient);

      if(m_eorientationSplit == orientation_horizontal)
      {

         return rectClient.width();

      }
      else
      {

         return rectClient.height();

      }

   }


   void split_layout::CalcSplitBarRect(index iIndex, LPRECT lpRect)
   {

      ASSERT(iIndex >= 0);

      ASSERT(iIndex < get_split_count());

      if (iIndex >= m_splitbara.get_count())
      {

         return;

      }

      int32_t nPos = 0;

      index i = 0;

      while (i <= iIndex)
      {

         if (is_pane_visible((int)(i)))
         {

            nPos = m_splitbara[iIndex]->m_dwPosition;

         }

         i++;

      }


      GetClientRect(lpRect);

      if(m_eorientationSplit == orientation_horizontal)
      {

         nPos = MAX(nPos, lpRect->top + m_iMarging / 2);

         nPos = MIN(nPos, lpRect->bottom - m_iMarging / 2);

         lpRect->top      = nPos - m_iMarging / 2;

         lpRect->bottom   = nPos + m_iMarging / 2;

      }
      else
      {

         nPos = MAX(nPos, lpRect->left + m_iMarging / 2);

         nPos = MIN(nPos, lpRect->right - m_iMarging / 2);

         lpRect->left   = nPos - m_iMarging / 2;

         lpRect->right   = nPos + m_iMarging / 2;

      }

   }


   bool split_layout::InsertPaneAt(index iIndex, sp(::user::interaction)pwnd, bool bFixedSize, class id id)
   {

      ::count iSplitBarCount = get_pane_count();

      m_splitbara.remove_all();

      index i;

      for(i = 0; i < iSplitBarCount; i++)
      {

         sp(split_bar) pbar(allocer());

         m_splitbara.insert_at(iIndex, pbar);

         ::user::split_bar & splitbar = *m_splitbara.element_at(i);

         splitbar.m_iIndex = i;

         if (!splitbar.create_window(null_rect(), this))
         {

            return false;

         }

      }

      {

         m_panea.insert_at(iIndex, new Pane(get_app()));

      }

      ASSERT(iIndex >= 0);
      ASSERT(iIndex < get_pane_count());
      split_layout::Pane * pcomponent = m_panea.element_at(iIndex);
      if(pcomponent->m_pholder != NULL)
      {
         if(!pcomponent->m_pholder->hold(pwnd))
            return false;
      }
      else
      {
         //rect rectPane;
         //pcomponent->m_pholder->GetClientRect(rectPane);
         pcomponent->m_pholder = place(pwnd,pcomponent->m_rectClient);
         if(pcomponent->m_pholder == NULL)
            return false;
      }

      pcomponent->m_id = id.is_empty() ? (::id) iIndex : id;

      //pwnd->SetParent(this);

      //pwnd->ModifyStyle(WS_BORDER, 0, 0);
      //pwnd->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, 0);
      m_panea[iIndex]->m_bFixedSize = bFixedSize;

      return true;

   }


   bool split_layout::RemovePaneAt(index iIndex)
   {

      ASSERT(iIndex >= 0);

      ASSERT(iIndex < get_pane_count());

      m_panea.remove_at(iIndex);

      ::count iSplitBarCount = get_pane_count();

      m_splitbara.remove_all();

      index i;

      for(i = 0; i < iSplitBarCount; i++)
      {

         m_splitbara.add_new();

         ::user::split_bar & splitbar = *m_splitbara.element_at(i);

         splitbar.m_iIndex = i;

         if (!splitbar.create_window(null_rect(), this))
         {

            return false;

         }

      }

      return true;

   }


   bool split_layout::SetPane(index iIndex, sp(::user::interaction)pwnd, bool bFixedSize, id id)
   {

      ASSERT(iIndex >= 0);

      ASSERT(iIndex < get_pane_count());

      split_layout::Pane * pcomponent = m_panea.element_at(iIndex);

      if(pcomponent->m_pholder != NULL)
      {

         if (!pcomponent->m_pholder->hold(pwnd))
         {

            return false;

         }

      }
      else
      {

         rect rectPane;

         pcomponent->m_pholder->GetClientRect(rectPane);

         pcomponent->m_pholder = place(pwnd, rectPane);

         if (pcomponent->m_pholder == NULL)
         {

            return false;

         }

      }

      pcomponent->m_id = id.is_empty() ? (::id) iIndex : id;

      m_panea[iIndex]->m_bFixedSize = bFixedSize;

      return true;

   }


   void split_layout::SetPaneFixedSize(index iIndex, SIZE * pSize)
   {

      UNREFERENCED_PARAMETER(iIndex);

      UNREFERENCED_PARAMETER(pSize);

      ASSERT(iIndex >= 0);

      ASSERT(iIndex < get_pane_count());
      //    m_aFixedSize.element_at(iIndex) = *pSize;

   }


   int32_t split_layout::GetMinPos(index iPane)
   {

      if (get_split_count() <= 0 || iPane <= 0)
      {

         return GetMinPos();

      }
      else
      {

         rect rect;

         CalcSplitBarRect(iPane - 1, rect);

         if(m_eorientationSplit == orientation_horizontal)
         {

            return rect.bottom;

         }
         else
         {

            return rect.right;

         }

      }

   }

   int32_t split_layout::GetMaxPos(index iPane)
   {

      if (get_split_count() <= 0 || iPane >= get_split_count())
      {

         return GetMaxPos();

      }
      else
      {

         rect rect;

         CalcSplitBarRect(iPane, rect);

         if(m_eorientationSplit == orientation_horizontal)
         {

            return rect.top;

         }
         else
         {

            return rect.left;

         }

      }

   }

   e_orientation split_layout::GetSplitOrientation()
   {

      return m_eorientationSplit;

   }


   void split_layout::RelayEventSplitBar(index iSplitBar, UINT uiMessage, WPARAM wParam, LPARAM lParam)
   {

      ASSERT(FALSE);

      if(!m_bInitialized)
         return;

      rect splitRect;

      CalcSplitBarRect(iSplitBar, &splitRect);

      if(uiMessage == WM_LBUTTONDOWN)
      {

         int32_t   fwKeys = (int32_t) wParam;        // key flags
//         int32_t xPos = splitRect.left + (int16_t) LOWORD(lParam);  // horizontal position of cursor
//         int32_t yPos = splitRect.top + (int16_t) HIWORD(lParam);  // vertical position of cursor
         if((fwKeys & MK_LBUTTON) > 0)
         {
            ::user::split_bar * pSplitBar = m_splitbara.element_at(iSplitBar);
            pSplitBar->SetCapture();
            m_iIndex = iSplitBar;
            m_iState = stateDragging;
         }
      }
      else if(uiMessage == WM_LBUTTONUP)
      {
//         int32_t   fwKeys = wParam;        // key flags
//         int32_t xPos = splitRect.left + (int16_t) LOWORD(lParam);  // horizontal position of cursor
//         int32_t yPos = splitRect.top + (int16_t) HIWORD(lParam);  // vertical position of cursor
         if(m_iState != stateInitial)
         {
            Session.ReleaseCapture();
            m_iState = stateInitial;
         }
      }
      else if(uiMessage == WM_CAPTURECHANGED)
      {
      }
      else if(uiMessage == WM_MOUSEMOVE)
      {
         int32_t   fwKeys = (int32_t) wParam;        // key flags
         int32_t xPos = splitRect.left + (int16_t) LOWORD(lParam);  // horizontal position of cursor
         int32_t yPos = splitRect.top + (int16_t) HIWORD(lParam);  // vertical position of cursor
         if((fwKeys & MK_LBUTTON) > 0 && (m_iState == stateDragging) && (iSplitBar == m_iIndex))
         {
            cslock lock(&m_mutex);
            {
               TRACE("split_layout::RelayChildEvent LOWORD(lParam) %d\n", LOWORD(lParam));
               TRACE("split_layout::RelayChildEvent HIWORD(lParam) %d\n", HIWORD(lParam));


               int32_t nPos;
               bool bMove;
               nPos = GetPos(xPos, yPos);
               if(m_iIndex <= 0)
               {
                  bMove = nPos > GetMinPos();
               }
               else
               {
                  bMove = nPos > (int32_t) m_splitbara[m_iIndex - 1]->m_dwPosition;
               }
               if(get_pane_count() >= m_iIndex )
               {
                  bMove = bMove && nPos < GetMaxPos();
               }
               else
               {
                  bMove = bMove && nPos < (int32_t) m_splitbara[m_iIndex]->m_dwPosition;
               }
               if(bMove)
               {
                  bMove = nPos != (int32_t) m_splitbara[m_iIndex]->m_dwPosition;
               }
               if(bMove)
               {

                  m_splitbara[m_iIndex]->m_dwPosition = nPos;
                  m_splitbara[m_iIndex]->m_dRate = 0.0;
                  on_layout();
               }
               m_mutex.unlock();
            }
         }
         else
         {
            m_iState = stateInitial;
         }
      }
   }

   rect & split_layout::get_pane_rect(index iPane)
   {

      ASSERT(iPane >= 0);

      ASSERT(iPane < get_pane_count());

      if (iPane < 0 || iPane >= get_pane_count())
      {

         _throw(invalid_argument_exception(get_app()));

      }

      return m_panea[iPane]->m_rectClient;

   }

   sp(::user::place_holder) split_layout::get_pane_holder(index iPane)
   {

      ASSERT(iPane >= 0);

      ASSERT(iPane < get_pane_count());

      if (iPane < 0 || iPane >= get_pane_count())
      {

         return NULL;

      }

      return m_panea[iPane]->m_pholder;

   }


   sp(::user::interaction) split_layout::get_pane_window(index iPane)
   {

      sp(::user::place_holder) pholder = get_pane_holder(iPane);

      if (pholder == NULL)
      {

         return NULL;

      }

      return pholder->get_hold();

   }


   id split_layout::get_pane_id(index iPane)
   {

      ASSERT(iPane >= 0);

      ASSERT(iPane < get_pane_count());

      if (iPane < 0 || iPane >= get_pane_count())
      {

         return id();

      }

      Pane & pane = m_panea(iPane);

      return pane.m_id;

   }


   index split_layout::get_pane_by_id(::id id)
   {

      for(index iPane = 0; iPane < m_panea.get_count(); iPane++)
      {

         if (m_panea[iPane]->m_id == id)
         {

            return iPane;

         }

      }

      return -1;

   }


   split_layout::Pane::Pane(::aura::application * papp) :
      ::object(papp)
   {

      m_pholder = NULL;

      m_rect.null();

      m_rectClient.null();

      m_sizeFixed = ::size(0, 0);

   }


   void split_layout::draw_control_background(::draw2d::graphics * pgraphics)
   {

      COLORREF crBackground = _001GetColor(::user::color_split_layout_background);

      if (argb_get_a_value(crBackground) > 0)
      {

         rect rectClient;

         GetClientRect(rectClient);

         pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

         pgraphics->fill_solid_rect(rectClient, crBackground);

      }


   }


   void split_layout::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      if (m_puserstyle == NULL)
      {

         return;

      }


      if (!m_puserstyle->_001OnDrawSplitLayout(pgraphics, this))
      {

         _001OnDrawSplitLayout(pgraphics);

      }

   }


   void split_layout::_001OnDrawSplitLayout(::draw2d::graphics * pgraphics)
   {


   }


} // namespace user







