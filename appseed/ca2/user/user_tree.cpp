#include "framework.h"


namespace user
{




   tree::tree(sp(::ca2::application) papp) :
      ca2(papp),
      ::user::scroll_view(papp),
      ::ca2::tree(papp),
      m_dcextension(papp)
   {
      m_bHoverStart = false;

      m_pitemFirstVisible        = NULL;
      m_iFirstVisibleItemLevel   = 0;
      m_pitemHover               = NULL;
      m_iClick                   = 0;
      m_iItemCount               = 0;
      m_crText                   = ARGB(255, 0, 0, 0);
      m_crTextSelected           = ARGB(255, 255, 255, 255);
      m_crTextHighlight          = ARGB(255, 102, 153, 255);
      m_crTextSelectedHighlight  = ARGB(255, 172, 213, 255);
      m_iItemHeight              = 18;
      m_iImageExpand             = -1;
      m_iImageCollapse           = -1;
      m_pimagelist               = NULL;
      m_uchHoverAlphaInit        = 0;
   }

   tree::~tree()
   {
   }


   void tree::_001OnCreate(::ca2::signal_object * pobj)
   {

      if(pobj->previous())
         return;

      if(IsWindowVisible())
      {
         RedrawWindow();
      }


      m_pimagelist = new image_list(get_app());
      m_pimagelist->create(16, 16, 0, 10, 10);

      _001SetCollapseImage("list/collapse.png");
      _001SetExpandImage("list/expand.png");


   }

   //bool tree::_001DynamicGetItemData(BaseTreeItemData & itemdata, ::ca2::tree_path & itempath, int32_t iValidateCount)
   //{
   /*   if(m_pdatainterface001 != NULL)
   {
   Carray < uint32_t, uint32_t > ida;
   Ex1TreeItemMetaData * pdata = _001StaticGetMetaData(itempath);
   if(pdata == NULL)
   return false;
   pdata->GetId001(ida);
   if(!m_pdatainterface001->_001GetItemData(itemdata, ida))
   {
   if(!_001StaticUpdateInfo(itempath, iValidateCount))
   return false;
   }
   pdata->UpdateProperExpandedUserMeta(m_pdatainterface001, iValidateCount);
   }
   else
   {
   // There is no valid data interface
   ASSERT(FALSE);
   }*/
   // return true;

   /*if(itempath.IsRoot())
   return false;
   while(!m_pdata->_001GetItemData(itemdata, itempath))
   {
   if(!_001StaticUpdateInfo(itempath, iValidateCount))
   return false;
   }
   return true;*/

   //}

   void tree::_001OnDrawBackground(::draw2d::graphics *pdc)
   {

      rect rectClient;
      GetClientRect(rectClient);

      //      ::ca2::savings & savings = System.savings();
      rect rectClientOffset = rectClient;

      rect rectClipBox;
      pdc->GetClipBox(rectClipBox);
      if(rectClipBox.is_null())
      {
         rectClipBox = rectClientOffset;
      }
      else
      {
         rectClipBox.intersect(rectClipBox, rectClientOffset);
      }

      class imaging & imaging = System.visual().imaging();

      if(System.savings().is_trying_to_save(::ca2::resource_processing))
      {
         pdc->FillSolidRect(
            rectClipBox,
            Session.get_default_color(COLOR_WINDOW));
      }
      else
      {
         imaging.color_blend(
            pdc,
            rectClipBox,
            Session.get_default_color(COLOR_WINDOW),
            196);
      }
   }

   COLORREF tree::get_background_color()
   {
      return RGB(200, 255, 255);
   }

   void tree::_001OnDraw(::draw2d::graphics *pdc)
   {

      rect rectClient;

      GetClientRect(rectClient);



      point ptCursor;

      Session.get_cursor_pos(&ptCursor);
      DWORD dwHoverIn = 384;
      DWORD dwHoverOut = 1284;
      ScreenToClient(&ptCursor);
      bool bTreeHover = rectClient.contains(ptCursor);
      if(bTreeHover)
      {
         if(!m_bHoverStart)
         {
            m_bHoverStart = true;
            m_uchHoverAlphaInit = m_uchHoverAlpha;
            m_dwHoverStart = get_tick_count();
         }
         if(get_tick_count() - m_dwHoverStart > dwHoverIn)
         {
            m_uchHoverAlpha = 255;
         }
         else
         {
            DWORD dwCurve =  (DWORD) (255.0 * (1.0 - pow(2.718281, -3.3 * (get_tick_count() - m_dwHoverStart) / dwHoverIn)));
            if(m_uchHoverAlphaInit + dwCurve > 255)
               m_uchHoverAlpha = 255;
            else
               m_uchHoverAlpha =  (BYTE) (m_uchHoverAlphaInit + dwCurve);
         }
      }
      else
      {
         if(m_bHoverStart)
         {
            m_bHoverStart = false;
            m_uchHoverAlphaInit = m_uchHoverAlpha;
            m_dwHoverEnd = get_tick_count();
         }

         if(get_tick_count() - m_dwHoverEnd  > dwHoverOut)
         {
            m_uchHoverAlpha = 0;
         }
         else
         {
            DWORD dwCurve =  (DWORD) (255.0 * (1.0 - pow(2.718281, -3.3 * (get_tick_count() - m_dwHoverEnd) / dwHoverOut)));
            if(m_uchHoverAlphaInit < dwCurve)
               m_uchHoverAlpha = 0;
            else
               m_uchHoverAlpha = (BYTE) (m_uchHoverAlphaInit - dwCurve);
         }
      }
      //      ::ca2::savings & savings = System.savings();

      ///      ::draw2d::graphics * pDCBuffer = pdc;

      //      point ptOriginalViewportOrg = pdc->GetViewportOrg();
      //
      //      rect rectClientOffset = rectClient;

      /*rect rectClipBox;
      pdc->GetClipBox(rectClipBox);
      if(rectClipBox.is_null())
      {
      rectClipBox = rectClientOffset;
      }
      else
      {
      rectClipBox.intersect(rectClipBox, rectClientOffset);
      }*/

      //       class imaging & imaging = System.visual().imaging();

      if(_001IsTranslucent())
      {
         class imaging & imaging = System.visual().imaging();
         //_001DrawBackground(pdc, rectClipBox);
         imaging.color_blend(
            pdc,
            rectClient,
            get_background_color(),
            127);
      }
      else
      {
         //      if(m_pgdibuffer != NULL
         //         && !TwiHasTranslucency())
         {
            /*         ::draw2d::graphics * pdcBack = m_pgdibuffer->GetBuffer();
            if(pdcBack != NULL)
            {
            pdc->BitBlt(
            rectClient.left,
            rectClient.top,
            rectClient.width(),
            rectClient.height(),
            pdcBack,
            rectClient.left,
            rectClient.top,
            SRCCOPY);
            }*/
         }
         //else
         {
            //if(System.savings().is_trying_to_save(::ca2::resource_processing))
            {
               pdc->FillSolidRect(
                  rectClient,
                  get_background_color());
            }
            /*         else
            {
            imaging.color_blend(
            pdc,
            rectClipBox,
            Session.get_default_color(COLOR_WINDOW),
            196);
            }*/
            /*         if(m_pgdibuffer != NULL)
            {
            ::draw2d::graphics * pdcBack = m_pgdibuffer->GetBuffer();
            if(pdcBack != NULL)
            {
            pdcBack->BitBlt(
            rectClient.left,
            rectClient.top,
            rectClient.width(),
            rectClient.height(),
            pdc,
            rectClient.left,
            rectClient.top,
            SRCCOPY);
            }
            }*/
         }
      }

      if(m_spdata.is_null())
         return;

      if(m_spdata->is_in_use())
         return;

      pdc->OffsetViewportOrg((int32_t) -m_scrollinfo.m_ptScroll.x, (int32_t) -(m_scrollinfo.m_ptScroll.y % _001GetItemHeight()));


      //   BaseTreeItemData itemdata;
      /*      ::ca2::tree_path itempath;*/
      ::user::tree_draw_item drawitemdata;
      drawitemdata.m_pdc = pdc;
      drawitemdata.m_iIndentation = _001GetIndentation();
      //   drawitemdata.m_pdata = &itemdata;
      //      drawitemdata.m_ppath = &itempath;
      drawitemdata.m_iItemHeight = 18;
      GetClientRect(drawitemdata.m_rectClient);


      //   _001OnDrawBackground(pdc);

      sp(::ca2::tree_item) pitem = m_pitemFirstVisible;



      index iLevel = m_iFirstVisibleItemLevel;
      index iItem = m_iFirstVisibleItemProperIndex;
      index iIndex = 0;


      for(;pitem != NULL; pitem = pitem->get_item(::ca2::TreeNavigationProperForward, &iLevel))
      {
         drawitemdata.m_pitem = pitem;
         drawitemdata.m_iItem = iItem;
         drawitemdata.m_rect = drawitemdata.m_rectClient;
         drawitemdata.m_rect.left = (LONG) (drawitemdata.m_iIndentation * iLevel);
         drawitemdata.m_rect.top += (LONG) (iIndex * drawitemdata.m_iItemHeight);
         drawitemdata.m_rect.bottom = (LONG) (drawitemdata.m_rect.top + drawitemdata.m_iItemHeight);
         drawitemdata.m_rect.right = m_iCurrentViewWidth;
         if(pitem->m_dwState & ::ca2::tree_item_state_selected)
         {
            //         drawitemdata.m_dwMetaData |= 1;
         }
         else
         {
            //       drawitemdata.m_dwMetaData &= ~1;
         }
         _001DrawItem(drawitemdata);
         iItem++;
         iIndex++;
      }
      /*int32_t iValidateItemCount = 30;
      int32_t iItem = 0;
      if(_001DynamicGetItemData(itemdata, m_itempathFirstVisible, iValidateItemCount))
      {
      m_iItemCount = m_pmetadataRoot->GetDescendantCount();
      itempath = m_itempathFirstVisible;
      while(true)
      {
      drawitemdata.m_iItem = iItem;
      iLevel = itempath.m_iaPath.get_size() - 1;
      drawitemdata.m_rect = drawitemdata.m_rectClient;
      drawitemdata.m_rect.left = drawitemdata.m_iIndentation * iLevel;
      drawitemdata.m_rect.top += iItem * drawitemdata.m_iItemHeight;
      drawitemdata.m_rect.bottom = drawitemdata.m_rect.top + drawitemdata.m_iItemHeight;
      Ex1TreeItemMetaData * pdata = _001StaticGetMetaData(itempath);
      if(pdata == m_pmetadataSelected)
      {
      itemdata.m_dwMetaData |= 1;
      }
      else
      {
      itemdata.m_dwMetaData &= ~1;
      }
      _001DrawItem(drawitemdata);
      iItem = _001StaticMoveNextItem(itempath, iItem);
      if(iItem < 0)
      break;
      if(!_001DynamicGetItemData(itemdata, itempath, iValidateItemCount))
      break;
      iValidateItemCount--;
      }
      }*/

   }

   void tree::_001DrawItem(tree_draw_item & data)
   {
      rect rect;

      sp(tree) ptree = this;
      sp(::ca2::tree_item) pitem = data.m_pitem;

      sp(image_list) pimagelistItem = pitem->get_image_list();
      sp(image_list) pimagelistTree = get_image_list();

      bool bSelected    = ptree->is_tree_item_selected(pitem);
      bool bHover       = ptree->is_tree_item_hover(pitem);



      if(m_uchHoverAlpha > 0)
      {

         if(ptree != NULL && pimagelistTree != NULL && data.m_pitem->m_dwState & ::ca2::tree_item_state_expandable)
         {

            _001GetItemElementRect(rect, data, tree_element_expand_box);

            int32_t iImage;
            if(data.m_pitem->m_dwState & ::ca2::tree_item_state_expanded)
            {
               iImage = (int32_t) ptree->m_iImageCollapse;
            }
            else
            {
               iImage = (int32_t) ptree->m_iImageExpand;
            }
            pimagelistTree->draw(data.m_pdc, iImage, rect.top_left(), 0, m_uchHoverAlpha);
         }
      }


      //      ::ca2::savings & savings = System.savings();
      if(bSelected) // selected
      {
         if(System.savings().is_trying_to_save(::ca2::resource_processing))
         {
            data.m_pdc->FillSolidRect(
               data.m_rect,
               RGB(96,96,96));
         }
         else
         {
            class rect rectUnion;
            _001GetItemElementRect(
               rect,
               data,
               tree_element_image);
            rectUnion = rect;
            _001GetItemElementRect(
               rect,
               data,
               tree_element_text);
            rectUnion.unite(rect, rectUnion);
            class imaging & imaging = System.visual().imaging();
            COLORREF crTranslucid = RGB(0, 0, 0);
            imaging.color_blend(
               data.m_pdc,
               rectUnion.left, rectUnion.top,
               rectUnion.width(), rectUnion.height(),
               crTranslucid, 127);
         }
      }

      if(pimagelistItem != NULL)
      {
         int32_t iImage = (int32_t) data.m_pitem->get_image();
         if(iImage >= 0)
         {
            _001GetItemElementRect(
               rect,
               data,
               tree_element_image);
            pimagelistItem->draw(
               data.m_pdc,
               iImage,
               rect.top_left(),
               0);
         }
      }


      string strItem = data.m_pitem->get_text();

      if(strItem.has_char() && _001GetItemElementRect(rect, data, tree_element_text))
      {

         ::draw2d::brush_sp brushText(allocer());

         if(bSelected) // selected
         {
            if(bHover)
            {
               brushText->create_solid(m_crTextSelectedHighlight);
            }
            else
            {
               brushText->create_solid(m_crTextSelected);
            }
         }
         else
         {
            if(bHover)
            {
               brushText->create_solid(m_crTextHighlight);
            }
            else
            {
               brushText->create_solid(m_crText);
            }
         }


         data.m_pdc->SelectObject(brushText);


         ::draw2d::font_sp font(allocer());


         font->operator=(*System.visual().font_central().GetListCtrlFont());
         font->set_bold();
         data.m_pdc->set_font(font);
         m_dcextension._DrawText(data.m_pdc, strItem, &rect, DT_LEFT | DT_BOTTOM);

      }

   }


   index tree::_001GetIndentation()
   {
      return 24;
   }




   void tree::_001OnMouseMove(::ca2::signal_object * pobj)
   {
      track_mouse_leave();

      UpdateHover();
      pobj->m_bRet = true;
   }

   void tree::_001OnMouseLeave(::ca2::signal_object * pobj)
   {
      m_pitemHover = NULL;
      _001RedrawWindow();
      pobj->m_bRet = true;
   }

   void tree::_001OnLButtonDown(::ca2::signal_object * pobj)
   {
      SCAST_PTR(::ca2::message::mouse, pmouse, pobj)
         /*if(System.get_focus_guie() != this)
         {
         System.set_active_guie(this);
         }*/

      pmouse->previous();
      pobj->m_bRet = true;
      pmouse->set_lresult(1);
   }

   void tree::_001OnLButtonDblClk(::ca2::signal_object * pobj)
   {
      SCAST_PTR(::ca2::message::mouse, pmouse, pobj)
         m_iClick = 2;

      _001OnClick((UINT) pmouse->m_nFlags, pmouse->m_pt);
      sp(::ca2::tree_item) pitem;
      ::user::e_tree_element eelement;
      pitem = _001HitTest(pmouse->m_pt, eelement);
      if(pitem != NULL)
      {
         if(eelement == tree_element_expand_box ||
            eelement == tree_element_image ||
            eelement == tree_element_text)
         {
            _001ExpandItem(pitem, !(pitem->m_dwState & ::ca2::tree_item_state_expanded));
         }
      }


      /* trans   window_id wndidNotify = _001GetNotifyWnd();

      LRESULT lresult = 0;

      if(wndidNotify)
      {
      NMLISTVIEW nm;
      nm.hdr.idFrom = pwnd->GetDlgCtrlId();
      nm.hdr.code =   NM_DBLCLK;
      nm.hdr.oswindowFrom = pwnd->GetSafeoswindow_();
      lresult = ::SendMessage(
      wndidNotify,
      WM_NOTIFY,
      nm.hdr.idFrom,
      (LPARAM) &nm);


      }*/

      // trans   pobj->m_bRet = lresult != 0;
   }

   void tree::_001OnLButtonUp(::ca2::signal_object * pobj)
   {
      SCAST_PTR(::ca2::message::mouse, pmouse, pobj)
         m_iClick++;
      m_uiLButtonUpFlags = (UINT) pmouse->m_nFlags;
      m_ptLButtonUp = pmouse->m_pt;
      perform_click();
      //SetTimer(TimerClick, 500, NULL);
      pobj->m_bRet = true;
      pmouse->set_lresult(1);
   }

   void tree::perform_click()
   {
      UINT nFlags = m_uiLButtonUpFlags;
      point point = m_ptLButtonUp;


      _001OnClick(nFlags, point);
      //xxx         TwiRedraw();
      sp(::ca2::tree_item) pitem;
      ::user::e_tree_element eelement;
      ScreenToClient(&point);
      pitem = _001HitTest(point, eelement);
      if(pitem != NULL)
      {
         if(eelement == tree_element_expand_box ||
            eelement == tree_element_image ||
            eelement == tree_element_text)
         {
            _001ExpandItem(pitem, !(pitem->m_dwState & ::ca2::tree_item_state_expanded));
         }
         if(eelement == tree_element_image ||
            eelement == tree_element_text)
         {
            _001OnOpenItem(pitem);
         }
      }


      /* trans         window_id  wndidNotify = _001GetNotifyWnd();


      LRESULT lresult = 0;

      if(wndidNotify)
      {
      NMLISTVIEW nm;
      nm.hdr.idFrom = pwnd->GetDlgCtrlId();
      nm.hdr.code =   NM_CLICK;
      nm.hdr.oswindowFrom = pwnd->GetSafeoswindow_();
      lresult = ::SendMessage(
      wndidNotify,
      WM_NOTIFY,
      nm.hdr.idFrom,
      (LPARAM) &nm);


      }*/

   }

   void tree::_001OnTimer(::ca2::signal_object * pobj)
   {
      SCAST_PTR(::ca2::message::timer, ptimer, pobj)

         //         return; //xxxtimer

         if(ptimer->m_nIDEvent == TimerHover)
         {
            UpdateHover();
         }
         else
            if(ptimer->m_nIDEvent == TimerClick)
            {
               KillTimer(TimerClick);
               if(m_iClick == 1)
               {
                  m_iClick = 0;


                  perform_click();

               }
               else
               {
                  m_iClick = 0;
               }


            }
   }

   /*window_id tree::_001GetNotifyWnd()
   {
   sp(::ca2::window) pwnd = get_guie();

   window_id wndidNotify = pwnd->get_owner()->GetSafeoswindow_();
   if(wndidNotify == NULL)
   wndidNotify = pwnd->get_parent()->GetSafeoswindow_();
   return wndidNotify;
   }*/

   void tree::_001OnClick(UINT uiFlags, point point)
   {
      UNREFERENCED_PARAMETER(point);
      //      if(uiFlags & MK_SHIFT)
      //    {
      //     if(uiFlags & MK_CONTROL)
      //   {
      // }
      //}
      //else
      {

         //      Ex1TreeItemMetaData * pdata = _001HitTest(point);
         //      if(pdata != NULL)
         //      {
         //       m_pmetadataSelected = pdata;
         //   }
      }
   }

   sp(::ca2::tree_item) tree::_001HitTest(POINT pt, ::user::e_tree_element & eelement)
   {
      index iy = pt.y;

      index iItem = -1;

      index iItemHeight = _001GetItemHeight();

      if(iItemHeight != 0)
      {
         iItem = (int32_t) ((iy + m_scrollinfo.m_ptScroll.y) / iItemHeight);
      }

      if(iItem < 0)
         return NULL;


      //   if(iItem >= _001StaticGetItemCount())
      //      return false;
      index iLevel;
      sp(::ca2::tree_item) pitem = get_proper_item(iItem, &iLevel);

      if(pitem == NULL)
         return NULL;

      index x = (int32_t) (pt. x - _001GetIndentation() * iLevel + m_scrollinfo.m_ptScroll.x);
      if(x >= 0 && x < 16)
         eelement = tree_element_expand_box;
      if(x >= 18 && x < 34)
         eelement = tree_element_image;
      if(x >= 36)
         eelement = tree_element_text;

      return pitem;


   }

   int32_t tree::_001GetItemHeight()
   {
      return m_iItemHeight;
   }

   void tree::install_message_handling(::ca2::message::dispatch * pdispatch)
   {
      ::user::window_interface::install_message_handling(pdispatch);
      ::user::scroll_view::install_message_handling(pdispatch);
      IGUI_WIN_MSG_LINK(WM_CREATE        , pdispatch, this, &tree::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_LBUTTONDBLCLK , pdispatch, this, &tree::_001OnLButtonDblClk);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP     , pdispatch, this, &tree::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN   , pdispatch, this, &tree::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_MOUSEMOVE     , pdispatch, this, &tree::_001OnMouseMove);
      IGUI_WIN_MSG_LINK(WM_MOUSELEAVE    , pdispatch, this, &tree::_001OnMouseLeave);
      IGUI_WIN_MSG_LINK(WM_HSCROLL       , pdispatch, this, &tree::_001OnHScroll);
      IGUI_WIN_MSG_LINK(WM_VSCROLL       , pdispatch, this, &tree::_001OnVScroll);
      IGUI_WIN_MSG_LINK(WM_TIMER         , pdispatch, this, &tree::_001OnTimer);
   }

   void tree::_001GetViewRect(LPRECT lprect)
   {

      rect rectClient;
      GetClientRect(rectClient);
      lprect->left = 0;
      lprect->top = 0;
      lprect->right = m_iCurrentViewWidth;
      lprect->bottom = (LONG) (get_proper_item_count() * _001GetItemHeight());


   }

   bool tree::_001GetItemElementRect(
      LPRECT lprect,
      ::user::tree_draw_item &drawitem,
      ::user::e_tree_element eelement)
   {
      switch(eelement)
      {
      case tree_element_expand_box:
         {
            lprect->left   = drawitem.m_rect.left;
            lprect->right  = min(lprect->left + 16, drawitem.m_rect.right);
            lprect->top    = drawitem.m_rect.top;
            lprect->bottom = drawitem.m_rect.bottom;
         }
         break;
      case tree_element_image:
         {
            lprect->left   = drawitem.m_rect.left + 18;
            lprect->right  = min(lprect->left + 16, drawitem.m_rect.right);
            lprect->top    = drawitem.m_rect.top;
            lprect->bottom = drawitem.m_rect.bottom;
         }
         break;
      case tree_element_text:
         {
            lprect->left   = drawitem.m_rect.left + 38;
            lprect->right  = drawitem.m_rect.right;
            lprect->top    = drawitem.m_rect.top;
            lprect->bottom = drawitem.m_rect.bottom;
         }
         break;
      }
      return true;
   }

   void tree::_001SelectItem(sp(::ca2::tree_item)pitem)
   {
      if(pitem != NULL)
      {
         pitem->set_selection();
      }
   }

   void tree::_001ExpandItem(sp(::ca2::tree_item)pitem, bool bExpand, /* = true */ bool bRedraw, /*=true*/ bool bLayout /*=true*/)
   {
      ::ca2::data::writing writing(::ca2::data_container::m_spdata);
      UNREFERENCED_PARAMETER(bLayout);
      if(bExpand)
      {
         if(!(pitem->m_dwState & ::ca2::tree_item_state_expanded))
         {

            _001OnItemExpand(pitem);

            pitem->m_dwState |= ::ca2::tree_item_state_expanded;

            // scroll properly to show the highest possible number
            // of children while trying to preserve the old position and
            // never passing
            index iLevel = 0;
            index iParentIndex = get_proper_item_index(pitem, &iLevel);

            index iLastChildIndex = iParentIndex + pitem->get_proper_descendant_count();

            index iLastVisibleIndex = (index) (m_scrollinfo.m_ptScroll.y / _001GetItemHeight() + _001GetVisibleItemCount());

            index iObscured; // obscured proper descendants
            iObscured = iLastChildIndex  - iLastVisibleIndex;

            if(iObscured > 0)
            {
               index iNewScroll = (int32_t) (m_scrollinfo.m_ptScroll.y + iObscured * _001GetItemHeight());
               if(iNewScroll > (iParentIndex * _001GetItemHeight()))
                  iNewScroll = (iParentIndex * _001GetItemHeight());
               m_scrollinfo.m_ptScroll.y = (LONG) max(iNewScroll, 0);
               //            _001SetYScroll(max(iNewScroll, 0), false);
               //m_pscrollbarVert->_001SetScrollPos(m_scrollinfo.m_ptScroll.y);
            }
         }
      }
      else
      {
         pitem->m_dwState &= ~::ca2::tree_item_state_expanded;
      }

      layout();
      if(bRedraw)
      {
         _001RedrawWindow();
      }

   }

   void tree::_001OnItemExpand(sp(::ca2::tree_item) pitem)
   {
      if(pitem->get_tree() != this)
      {
         dynamic_cast < ::user::tree * > (pitem->get_tree().m_p)->_001OnItemExpand(pitem);
      }
   }

   void tree::_001OnItemCollapse(sp(::ca2::tree_item)pitem)
   {
      if(pitem->get_tree() != this)
      {
         dynamic_cast < ::user::tree * > (pitem->get_tree().m_p)->_001OnItemCollapse(pitem);
      }
   }

   void tree::_001OnVScroll(::ca2::signal_object * pobj)
   {

      pobj->previous();


   }


   void tree::_001OnHScroll(::ca2::signal_object * pobj)
   {

      pobj->previous();

   }

   void tree::_001OnUpdateScrollPosition()
   {

      scroll_view::_001OnUpdateScrollPosition();

      //      HeaderCtrlLayout();

      //      CacheHint();

      m_pitemFirstVisible = CalcFirstVisibleItem(
         m_iFirstVisibleItemLevel,
         m_iFirstVisibleItemProperIndex);


      UpdateHover();

   }


   /*   void tree::SetScrollSizes()
   {
   rect64 rectTotal;

   _001GetViewRect(&rectTotal);

   size sizeTotal = rectTotal.size();

   m_scrollinfo.m_sizeTotal = sizeTotal;

   rect rectViewClient;
   _001GetViewClientRect(&rectViewClient);

   m_scrollinfo.m_sizeTotal = sizeTotal;
   m_scrollinfo.m_sizePage = rectViewClient.size();


   if(m_scrollinfo.m_ptScroll.y > (m_scrollinfo.m_sizeTotal.cy - m_scrollinfo.m_sizePage.cy))
   {
   m_scrollinfo.m_ptScroll.y = (m_scrollinfo.m_sizeTotal.cy - m_scrollinfo.m_sizePage.cy);
   }


   }*/

   void tree::layout()
   {


      SetScrollSizes();

      m_pitemFirstVisible = CalcFirstVisibleItem(m_iFirstVisibleItemLevel, m_iFirstVisibleItemProperIndex);

      m_iCurrentViewWidth = _001CalcCurrentViewWidth();

      _001LayoutScrollBars();

   }



   void tree::_001OnOpenItem(sp(::ca2::tree_item)pitem)
   {
      if(pitem->get_tree() != this)
      {
         dynamic_cast < tree * > (pitem->get_tree().m_p)->_001OnOpenItem(pitem);
      }
   }

   void tree::UpdateHover()
   {
      point pt;
      System.get_cursor_pos(&pt);
      ScreenToClient(&pt);
      ::user::e_tree_element eelement;
      sp(::ca2::tree_item) pitem = _001HitTest(pt, eelement);
      if(eelement != tree_element_image &&
         eelement != tree_element_text)
      {
         pitem = NULL;
      }
      if(pitem != m_pitemHover)
      {
         m_pitemHover = pitem;
         _001RedrawWindow();
      }
      /*if(pitem != NULL)
      {
      SetTimer(TimerHover, 250, NULL);
      }
      else
      {
      KillTimer(TimerHover);
      }*/
   }

   void tree::_001SetExpandImage(const char * pszMatter)
   {
      m_iImageExpand = m_pimagelist->add_matter(pszMatter);
   }

   void tree::_001SetCollapseImage(const char * pszMatter)
   {
      m_iImageCollapse = m_pimagelist->add_matter(pszMatter);
   }

   ::count tree::_001GetVisibleItemCount()
   {
      if(_001GetItemHeight() == 0)
         return 0;

      rect rect;
      GetClientRect(rect);

      return rect.height() / _001GetItemHeight() - 1;
   }

   int32_t tree::_001CalcCurrentViewWidth()
   {
      rect rectClient;
      GetClientRect(rectClient);

      ::count iCount = _001GetVisibleItemCount();
      int32_t iMaxWidth = rectClient.width();
      int32_t iWidth;
      index iLevel = m_iFirstVisibleItemLevel;
      index iIndent  = _001GetIndentation();
      sp(::ca2::tree_item) pitem = m_pitemFirstVisible;
      if(pitem == NULL)
         return iMaxWidth;
      for(int32_t i = 0; i < iCount; i++)
      {
         iWidth = (int32_t) (200 + iIndent * iLevel);
         if(iWidth > iMaxWidth)
         {
            iMaxWidth = iWidth;
         }
         pitem = pitem->get_item(::ca2::TreeNavigationProperForward, &iLevel);
         if(pitem == NULL)
            break;
      }
      return iMaxWidth;
   }

   /*
   void tree::_001SetYScroll(int32_t y, bool bRedraw)
   {
   m_ptScroll.y = y;

   m_pitemFirstVisible = CalcFirstVisibleItem(
   m_iFirstVisibleItemLevel,
   m_iFirstVisibleItemProperIndex);

   if(bRedraw)
   {
   _001RedrawWindow();
   }
   }
   */

   sp(::ca2::tree_item) tree::CalcFirstVisibleItem(index & iLevel, index & iProperIndex)
   {
      index nOffset;
      if(_001GetItemHeight() == 0)
         return NULL;
      nOffset = m_scrollinfo.m_ptScroll.y / _001GetItemHeight();

      sp(::ca2::tree_item) pitem = get_base_item();

      iLevel = 0;
      iProperIndex = 0;

      for(;;)
      {
         pitem = pitem->get_item(::ca2::TreeNavigationProperForward, &iLevel);
         if(pitem == NULL)
            break;
         if(nOffset <= 0)
         {
            break;
         }
         nOffset--;
         iProperIndex++;
      }

      return pitem;
   }

   void tree::_001OnTreeDataChange()
   {
      layout();
   }

   sp(image_list) tree::get_image_list()
   {
      return m_pimagelist;
   }

   bool tree::can_merge(sp(::user::interaction) pui)
   {
      return base < tree >::bases(pui) && !m_treeptra.contains(dynamic_cast < tree * > (pui.m_p));
   }


   bool tree::merge(sp(::user::interaction) pui)
   {


      sp(tree) ptree =  (pui.m_p);

      if(!insert_item(NULL, ptree->get_base_item(), ::ca2::RelativeLastChild, get_base_item()))
         return false;

      m_treeptra.add(ptree);

      return true;


   }


   int32_t tree::get_wheel_scroll_delta()
   {


      return 3 * m_iItemHeight;


   }




} // namespace ca2


