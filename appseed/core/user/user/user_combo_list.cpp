//#include "framework.h"


namespace user
{


   combo_list::combo_list(::aura::application * papp):
      object(papp)
   {

      m_bComboList                  = true;

      m_puiDeactivateTogether       = NULL;

      m_puiDeactivateTogetherSet    = NULL;

      m_pcombo                      = NULL;

      m_iHover                      = -1;

      m_iBorder                     = 6;

   }


   combo_list::~combo_list()
   {


   }


   void combo_list::install_message_handling(::message::dispatch * pdispatch)
   {

      ::user::control::install_message_handling(pdispatch);

      IGUI_WIN_MSG_LINK(WM_SETFOCUS,pdispatch,this,&combo_list::_001OnSetFocus);
      IGUI_WIN_MSG_LINK(WM_KILLFOCUS, pdispatch, this, &combo_list::_001OnKillFocus);
      IGUI_WIN_MSG_LINK(WM_CLOSE, pdispatch, this, &combo_list::_001OnClose);
      IGUI_WIN_MSG_LINK(WM_ACTIVATE, pdispatch, this, &combo_list::_001OnActivate);
      IGUI_WIN_MSG_LINK(WM_MOUSEACTIVATE, pdispatch, this, &combo_list::_001OnMouseActivate);
      IGUI_WIN_MSG_LINK(WM_KEYDOWN,pdispatch,this,&combo_list::_001OnKeyDown);
      IGUI_WIN_MSG_LINK(WM_KEYUP,pdispatch,this,&combo_list::_001OnKeyUp);
      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN,pdispatch,this,&combo_list::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pdispatch, this, &combo_list::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_MBUTTONDOWN, pdispatch, this, &combo_list::_001OnMButtonDown);
      IGUI_WIN_MSG_LINK(WM_RBUTTONDOWN, pdispatch, this, &combo_list::_001OnRButtonDown);
      IGUI_WIN_MSG_LINK(WM_MOUSEMOVE, pdispatch, this, &combo_list::_001OnMouseMove);

   }


   void combo_list::_001OnDraw(::draw2d::dib * pdib)
   {

      if(m_pcombo == NULL)
         return;

      if(m_pcombo->m_estyle == ::user::combo_box::style_simply)
      {

         _002OnDrawSimply(pdc);

      }
      else
      {

         _002OnDrawVerisimple(pdc);

      }

   }

   void combo_list::_001OnDrawVerisimple(::draw2d::dib * pdib)
   {

      ::count ca = m_pcombo->_001GetListCount();

      string strItem;

      rect rectClient;

      GetClientRect(rectClient);

      ::draw2d::brush_sp br(allocer());

      br->create_solid(ARGB(230, 255, 255, 255));

      pdc->SelectObject(br);

      pdc->FillRectangle(rectClient);

      {

         color ca;

         ca.set_rgb(RGB(227, 227, 210));

         ca.hls_rate(0.0, -0.33, -0.23);

         COLORREF crBorder = ca.get_rgb() | (0xff << 24);

         pdc->Draw3dRect(rectClient, crBorder, crBorder);

      }

      rect rectItem;

      rectItem = rectClient;

      rectItem.bottom = rectClient.top + _001GetItemHeight();

      point ptCursor;

      Session.get_cursor_pos(&ptCursor);

      ScreenToClient(&ptCursor);

      br->create_solid(ARGB(255, 84, 84, 77));

//      int32_t dSize = (int32_t) ( _001GetItemHeight() * 0.7);

  //    GetFont()->m_dFontSize = dSize;
    //  GetFont()->m_eunitFontSize = ::draw2d::unit_pixel;
//      GetFont()->m_bUpdated = false;

      select_font(pdc);

      pdc->SelectObject(br);

      for(index i = 0; i < ca; i++)
      {
         rectItem.top = rectItem.bottom;
         rectItem.bottom = rectItem.top + _001GetItemHeight();
         if(i != m_pcombo->m_iSel)
         {
            if(rectItem.contains(ptCursor))
            {
               ::draw2d::pen_sp pen(allocer());
               pen->create_solid(m_iItemHeight / 8, ARGB(230, 77, 184, 63));
               pdc->SelectObject(pen);
               pdc->DrawRectangle(rectItem);
            }
            m_pcombo->_001GetListText(i, strItem);
            pdc->draw_text(strItem, rectItem, 0);
         }
      }

      if(m_pcombo->m_iSel >= 0)
      {
         rectItem.top = (LONG) (rectClient.top + (_001GetItemHeight() * (1 + m_pcombo->m_iSel)));
         rectItem.bottom = rectItem.top + _001GetItemHeight();
         if(rectItem.contains(ptCursor))
         {
            br->create_solid(ARGB(123, 123, 149, 108));
         }
         else
         {
            br->create_solid(ARGB(184, 77, 184, 63));
         }
         pdc->SelectObject(br);
         pdc->FillRectangle(rectItem);
         br->create_solid(ARGB(255, 255, 255, 240));
         m_pcombo->_001GetListText(m_pcombo->m_iSel, strItem);
         pdc->SelectObject(br);
         pdc->draw_text(strItem, rectItem, 0);
      }


   }


   void combo_list::_001OnDrawSimply(::draw2d::dib * pdib)
   {

      ::count ca = m_pcombo->_001GetListCount();

      string strItem;

      rect rectClient;

      GetClientRect(rectClient);

      ::draw2d::brush_sp br(allocer());

      br->create_solid(ARGB(230, 255, 255, 255));

      pdc->SelectObject(br);

      pdc->FillRectangle(rectClient);

      {

         color ca;

         ca.set_rgb(RGB(227, 227, 210));

         ca.hls_rate(0.0, -0.33, -0.23);

         COLORREF crBorder = ca.get_rgb() | (0xff << 24);

         pdc->Draw3dRect(rectClient, crBorder, crBorder);

      }

      rect rectItem;

      rectItem = rectClient;

      rectItem.bottom = rectClient.top + _001GetItemHeight();

      point ptCursor;

      Session.get_cursor_pos(&ptCursor);

      ScreenToClient(&ptCursor);

      br->create_solid(ARGB(255, 84, 84, 77));

      //int32_t dSize = (int32_t) (_001GetItemHeight() * 0.7);

      //GetFont()->m_dFontSize = dSize;
      //GetFont()->m_eunitFontSize = ::draw2d::unit_pixel;
      //GetFont()->m_bUpdated = false;

      select_font(pdc);

      pdc->SelectObject(br);

      for(index i = 0; i < ca; i++)
      {
         rectItem.top = rectItem.bottom;
         rectItem.bottom = rectItem.top + _001GetItemHeight();
         if(i != m_pcombo->m_iSel)
         {
            if(rectItem.contains(ptCursor))
            {
               ::draw2d::pen_sp pen(allocer());
               pen->create_solid(m_iItemHeight / 8, ARGB(230, 77, 184, 63));
               pdc->SelectObject(pen);
               pdc->DrawRectangle(rectItem);
            }
            m_pcombo->_001GetListText(i, strItem);
            pdc->draw_text(strItem, rectItem, 0);
         }
      }

      if(m_pcombo->m_iSel >= 0)
      {
         rectItem.top = (LONG) (rectClient.top + (_001GetItemHeight() * (1 + m_pcombo->m_iSel)));
         rectItem.bottom = rectItem.top + _001GetItemHeight();
         if(rectItem.contains(ptCursor))
         {
            br->create_solid(ARGB(123, 123, 149, 108));
         }
         else
         {
            br->create_solid(ARGB(184, 77, 184, 63));
         }
         pdc->SelectObject(br);
         pdc->FillRectangle(rectItem);
         br->create_solid(ARGB(255, 255, 255, 240));
         m_pcombo->_001GetListText(m_pcombo->m_iSel, strItem);
         pdc->SelectObject(br);
         pdc->draw_text(strItem, rectItem, 0);
      }


   }


   void combo_list::_002OnDrawVerisimple(::draw2d::dib * pdib)
   {

      ::count ca = m_pcombo->_001GetListCount();

      string strItem;

      rect rectClient;

      GetClientRect(rectClient);

      ::draw2d::brush_sp br(allocer());

      br->create_solid(ARGB(230,255,255,255));

      pdc->SelectObject(br);

      pdc->FillRectangle(rectClient);

      //{

      //   color ca;

      //   ca.set_rgb(RGB(227,227,210));

      //   ca.hls_rate(0.0,-0.33,-0.23);

      //   COLORREF crBorder = ca.get_rgb() | (0xff << 24);

      //   pdc->Draw3dRect(rectClient,crBorder,crBorder);

      //}

      rect rectItem;

      rectItem = rectClient;

      if(m_pcombo->m_bEdit)
      {

         rectItem.bottom = rectClient.top + _001GetItemHeight();

      }
      else
      {

         rectItem.bottom = 0;

      }

      point ptCursor;

      Session.get_cursor_pos(&ptCursor);

      ScreenToClient(&ptCursor);

      //br->create_solid(ARGB(255,84,84,77));

      //      int32_t dSize = (int32_t) ( _001GetItemHeight() * 0.7);

      //    GetFont()->m_dFontSize = dSize;
      //  GetFont()->m_eunitFontSize = ::draw2d::unit_pixel;
      //      GetFont()->m_bUpdated = false;

      select_font(pdc);

      //pdc->SelectObject(br);

      int iHover = m_iHover;

      if(iHover < 0 || iHover >= ca)
      {

         iHover = m_pcombo->_001GetCurSel();

      }

      for(index i = 0; i < ca; i++)
      {
         rectItem.top = rectItem.bottom;
         rectItem.bottom = rectItem.top + _001GetItemHeight();
         if(iHover == i)
         {
            br->create_solid(ARGB(184,77,184,63));
            pdc->SelectObject(br);
            pdc->FillRectangle(rectItem);
            br->create_solid(ARGB(255,255,255,240));
            m_pcombo->_001GetListText(i,strItem);
            pdc->SelectObject(br);
            pdc->draw_text(strItem,rectItem,0);
         }
         else
         {
            pdc->set_text_color(ARGB(255,84,84,77));
            m_pcombo->_001GetListText(i,strItem);
            pdc->draw_text(strItem,rectItem,0);
         }
      }

   }


   void combo_list::_002OnDrawSimply(::draw2d::dib * pdib)
   {

      ::count ca = m_pcombo->_001GetListCount();

      string strItem;

      rect rectClient;

      GetClientRect(rectClient);

      ::draw2d::brush_sp br(allocer());

      br->create_solid(ARGB(230,255,255,255));

      pdc->SelectObject(br);

      pdc->FillRectangle(rectClient);

      //{

      //   color ca;

      //   ca.set_rgb(RGB(227,227,210));

      //   ca.hls_rate(0.0,-0.33,-0.23);

      //   COLORREF crBorder = ca.get_rgb() | (0xff << 24);

      //   pdc->Draw3dRect(rectClient,crBorder,crBorder);

      //}

      rect rectItem;

      rectItem = rectClient;

      rectItem.bottom = rectClient.top;

      if(m_pcombo->m_bEdit)
      {

         rectItem.bottom += _001GetItemHeight();

      }

      point ptCursor;

      Session.get_cursor_pos(&ptCursor);

      ScreenToClient(&ptCursor);

      //br->create_solid(ARGB(255,84,84,77));

      //int32_t dSize = (int32_t) (_001GetItemHeight() * 0.7);

      //GetFont()->m_dFontSize = dSize;
      //GetFont()->m_eunitFontSize = ::draw2d::unit_pixel;
      //GetFont()->m_bUpdated = false;

      select_font(pdc);

      //pdc->SelectObject(br);

      int iHover = m_iHover;

      if(iHover < 0 || iHover >= ca)
      {

         iHover = m_pcombo->_001GetCurSel();

      }

      for(index i = 0; i < ca; i++)
      {
         rectItem.top = rectItem.bottom;
         rectItem.bottom = rectItem.top + _001GetItemHeight();
         if(i == iHover)
         {
            br->create_solid(ARGB(184,77,184,63));
            pdc->SelectObject(br);
            pdc->FillRectangle(rectItem);
            br->create_solid(ARGB(255,255,255,240));
            m_pcombo->_001GetListText(i,strItem);
            pdc->SelectObject(br);
            pdc->draw_text(strItem,rectItem,0);
         }
         else
         {
            pdc->set_text_color(ARGB(255,84,84,77));
            m_pcombo->_001GetListText(i,strItem);
            pdc->draw_text(strItem,rectItem,0);
         }
      }


   }

   void combo_list::query_full_size(LPSIZE lpsize) const
   {

      if (!((combo_list *)this)->IsWindow())
         return;

      ::draw2d::graphics_sp pdc(((combo_list *) this)->allocer());


      pdc->CreateCompatibleDC(NULL);

      ::draw2d::dib_sp tameshi(((combo_list *) this)->allocer());

      tameshi->create(100, 100);

      pdc->SelectObject(tameshi->get_bitmap());

    //  int32_t dSize = (int32_t) (_001GetItemHeight() * 0.7);
////
//      ((combo_list *) this)->GetFont()->m_dFontSize = dSize;
//
//      ((combo_list *) this)->GetFont()->m_eunitFontSize = ::draw2d::unit_pixel;
//
//      ((combo_list *) this)->GetFont()->m_bUpdated = false;
//
      ((combo_list *)this)->select_font(pdc);

      string strItem;

      size sz;

      lpsize->cx = 0;

      ::count ca = m_pcombo->_001GetListCount();

      for(index i = 0; i < ca; i++)
      {

         m_pcombo->_001GetListText(i, strItem);

         sz = pdc->GetTextExtent(strItem);

         if(sz.cx > lpsize->cx)
         {

            lpsize->cx = sz.cx;

         }

      }

      int iAddUp = 0;

      if(m_pcombo->m_bEdit)
      {

         iAddUp = 1;

      }

      lpsize->cy = (LONG) (_001GetItemHeight() * (m_pcombo->_001GetListCount() + iAddUp));

      //lpsize->cx += m_iBorder * 2;

      ::rect rectComboClient;

      m_pcombo->GetClientRect(rectComboClient);

      lpsize->cx = MAX(lpsize->cx,rectComboClient.width());

      //lpsize->cy += m_iBorder * 2;

   }

   int32_t combo_list::_001GetItemHeight() const
   {

      return m_iItemHeight;

   }

   bool combo_list::GetClientRect(RECT64 * lprect)
   {
      
      if(!::user::control::GetClientRect(lprect))
      {

         return false;

      }

      ::deflate_rect(lprect,m_iBorder);

      return true;

   }


   bool combo_list::keyboard_focus_is_focusable()
   {

      return true;

   }


   bool combo_list::keyboard_focus_OnKillFocus()
   {

      m_pcombo->_001ShowDropDown(false);

      return true;

   }


   bool combo_list::pre_create_window(::user::create_struct & cs)
   {

      if (cs.style & WS_BORDER)
      {
   #ifdef WINDOWSEX
         cs.dwExStyle |= WS_EX_CLIENTEDGE;
   #endif
         cs.style &= ~WS_BORDER;
      }

      cs.dwExStyle |= WS_EX_LAYERED;
      cs.dwExStyle |= WS_EX_TOOLWINDOW;
      cs.dwExStyle |= WS_EX_TOPMOST;
      //cs.dwExStyle |= WS_EX_NOACTIVATE;

      return TRUE;
   }

   void combo_list::_001OnKillFocus(signal_details * pobj)
   {

      SCAST_PTR(::message::base,pbase,pobj);
      
      pbase->m_bRet = true;

      pbase->set_lresult(0);

   }

   void combo_list::_001OnSetFocus(signal_details * pobj)
   {

      SCAST_PTR(::message::set_focus,psetfocus,pobj);

      psetfocus->m_bRet = true;

      psetfocus->set_lresult(0);

   }



   void combo_list::_001OnActivate(signal_details * pobj)
   {

      SCAST_PTR(::message::activate, pactivate, pobj);

      sp(::user::interaction) pActive = (pactivate->m_nState == WA_INACTIVE ? pactivate->m_pWndOther : this);

      if(pactivate->m_nState == WA_INACTIVE)
      {

         point ptCursor;

         Session.get_cursor_pos(&ptCursor);

         m_pcombo->ScreenToClient(&ptCursor);

         if(m_pcombo->hit_test(ptCursor) != element_drop_down)
         {

            m_pcombo->_001ShowDropDown(false);

         }


         

         //if(pActive != m_puiDeactivateTogether)
         //{

         //   ::user::wndfrm::frame::WorkSet * pset = m_puiDeactivateTogetherSet;

         //   if(pset != NULL)
         //   {

         //      pset->SetActiveFlag(FALSE);

         //   }

         //}



      }
      else
      {

         Session.set_keyboard_focus(this);


      }



         //m_pcombo->_001ShowDropDown(false);

      //}

   }

   void combo_list::_001OnMouseActivate(signal_details * pobj)
   {

//      SCAST_PTR(::message::mouse_activate, pactivate, pobj);


   }

   void combo_list::_001OnLButtonUp(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      pmouse->m_bRet = true;

   }

   void combo_list::_001OnKeyDown(signal_details * pobj)
   {

      SCAST_PTR(::message::key,pkey,pobj);

      if(pkey->m_ekey == ::user::key_escape)
      {

         m_pcombo->ShowDropDown(false);

         

      }
      else if(pkey->m_ekey == ::user::key_tab)
      {

         m_pcombo->ShowDropDown(false);

         sp(::user::elemental) pelemental = m_pcombo->keyboard_get_next_focusable();

         if(pelemental.is_set())
         {

            pelemental->keyboard_set_focus();

         }


      }
      else if(pkey->m_ekey == ::user::key_down)
      {

         m_iHover = MIN(m_iHover + 1, m_pcombo->_001GetListCount() -1);

      }
      else if(pkey->m_ekey == ::user::key_up)
      {

         m_iHover = MAX(m_iHover - 1,0);

      }

   }


   void combo_list::_001OnKeyUp(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

   }

   void combo_list::_001OnLButtonDown(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      rect rectClient;

      GetClientRect(rectClient);

      if(rectClient.contains(pt))
      {

         e_element eelement;

         index iItem = hit_test(pt, eelement);

         if(eelement == element_item)
         {

            m_pcombo->_001SetCurSel(iItem, ::action::source_user);

         }

      }

      m_pcombo->_001ShowDropDown(false);

      pobj->m_bRet = true;

   }

   void combo_list::_001OnMButtonDown(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      rect rectClient;

      GetClientRect(rectClient);

      if(rectClient.contains(pt))
      {

      }
      else
      {
         m_pcombo->_001ShowDropDown(false);
      }

      pobj->m_bRet = true;

   }

   void combo_list::_001OnRButtonDown(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      rect rectClient;

      GetClientRect(rectClient);

      if(rectClient.contains(pt))
      {

      }
      else
      {
         m_pcombo->_001ShowDropDown(false);
      }

      pobj->m_bRet = true;

   }


   void combo_list::_001OnMouseMove(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      pobj->m_bRet = true;

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      e_element eelement;

      m_iHover = hit_test(pt,eelement);

   }


   void combo_list::_001OnClose(signal_details * pobj)
   {

      post_message(WM_DESTROY);

   }
   

   index combo_list::hit_test(point pt, e_element & eelement)
   {

      ::count ca = m_pcombo->_001GetListCount();

      rect rectClient;

      GetClientRect(rectClient);

      rect rectItem;

      rectItem = rectClient;

      int iAddUp = 0;

      if(m_pcombo->m_bEdit)
      {

         iAddUp = 1;

      }

      for(int32_t i = 0; i < ca; i++)
      {

         rectItem.top = rectClient.top + (_001GetItemHeight() * (iAddUp + i));
         rectItem.bottom = rectItem.top + _001GetItemHeight();

         if(rectItem.contains(pt))
         {

            eelement = element_item;

            return i;

         }


      }

      rectItem.top = rectClient.top;
      rectItem.bottom = rectItem.top + _001GetItemHeight();

      if(rectItem.contains(pt))
      {

         eelement = element_search_edit;

         return 0;

      }

      eelement = element_none;

      return -1;

   }





} // namespace user




