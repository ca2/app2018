#include "framework.h"

//CLASS_DECL_CORE COLORREF Session.get_default_color(uint32_t dw);

namespace user
{

   list_header::list_header(::aura::application * papp) :
      object(papp),
      m_font(allocer()),
      m_dcextension(papp)
   {
      m_plistctrlinterface = NULL;
      m_bTrack = false;
      m_bLButtonDown = false;
      m_bHover = false;
      m_font->create_point_font("Verdana", 20.0);
   }

   list_header::~list_header()
   {

   }

   void list_header::SetBaseListCtrlInterface(list *pinterface)
   {
      m_plistctrlinterface = pinterface;
   }

   void list_header::DrawItem(::draw2d::item * pdrawitem)
   {

      ::draw2d::graphics * pgraphics = pdrawitem->m_pgraphics;

      rect rectColumn = pdrawitem->rcItem;

      int32_t iColumn = pdrawitem->itemID;

      list * plist = m_plistctrlinterface;

      string str;

      str = System.load_string(plist->_001GetColumnTextId(ItemToColumnKey(iColumn)));
      //m_font->create_point_font("Verdana", 10.0);


      pgraphics->SelectObject(m_font);

//      pgraphics->SetBkMode(TRANSPARENT);

      pgraphics->set_text_color(_001GetColor(::user::color_list_header));
      int i = plist->_001GetDrawTextFlags(plist->m_eview);

      pgraphics->_DrawText(str,str.get_length(),rectColumn,i);

   }



   index list_header::MapItemToOrder(index iItem)
   {
//#ifdef WINDOWSEX
  //    HDITEM hditem;


      //hditem.mask = HDI_ORDER| HDI_TEXT;

    //  hditem.mask = HDI_ORDER;

   //   char pszText[1024];

   //   hditem.pszText = pszText;
   //   hditem.cchTextMax = 1024;

   //   get_item(iItem, &hditem);

   //   return hditem.iOrder;
      return iItem;

//#else

      //throw todo(get_app());

//#endif

   }


   bool list_header::GetItemRect(LPRECT lprect, e_element eelement, index iItem)
   {

      if(iItem < 0)
         return false;

      list * plist = m_plistctrlinterface;
      if(iItem >= plist->_001GetColumnCount())
         return false;

      int32_t x;
      if(plist->m_bGroup && plist->m_bLateralGroup)
      {
         x = plist->m_iLateralGroupWidth;
      }
      else
      {
         x = 0;
      }
      int32_t xLast = x;
      draw_list_item item(plist);
      for(int32_t i = 0; i <= iItem; i++)
      {
         xLast = x;
         item.m_iWidthColumn = ItemToColumnKey(i);
         plist->_001GetColumnWidth(&item);
         if(item.m_bOk)
         {
            x += item.m_iColumnWidth;
         }
      }

      rect rect;

      GetClientRect(rect);
      rect.left = xLast;
      rect.right = x;

      if(eelement == element_item)
      {
         *lprect = rect;
         return true;
      }

      if(eelement == ElementItemBox)
      {
         rect.right -= GetDividerWidth();
         *lprect = rect;
         return true;
      }

      if(eelement == ElementDivider)
      {
         rect.left = rect.right - GetDividerWidth();
         *lprect = rect;
         return true;
      }
      return true;
   }

   bool list_header::GetItemRect(
      LPRECT lprect,
      e_element eelementLButtonDown,
      index iItemLButtonDown,
      e_element eelement,
      index iItem)
   {
      if(iItem < 0)
         return false;
      list * plist = m_plistctrlinterface;
      if(iItem >= plist->_001GetColumnCount())
         return false;


      if(eelementLButtonDown == ElementItemBox
         && eelement == ElementItemBox)
      {
         if(iItem == iItemLButtonDown)
         {
            class rect rectA;
            if(!GetItemRect(rectA, ElementItemBox, iItem - 1))
            {
               GetItemRect(rectA, ElementItemBox, iItem);
            }

            class rect rect;
            GetItemRect(rect, ElementItemBox, iItem);

            class rect rectB;
            if(!GetItemRect(rectA, ElementItemBox, iItem + 1))
            {
               GetItemRect(rectA, ElementItemBox, iItem);
            }

            rect.left = rectA.left + rectA.width() / 2;
            rect.right = rectB.left + rectB.width() / 2;
            *lprect = rect;
            return true;
         }
         else if(iItem <= iItemLButtonDown - 1)
         {
            rect rectA;
            if(!GetItemRect(rectA, ElementItemBox, iItem - 1))
            {
               GetItemRect(rectA, ElementItemBox, iItem);
            }

            rect rect;
            GetItemRect(rect, ElementItemBox, iItem);

            rect.left = rectA.left + rectA.width() / 2;
            rect.right = rect.left + rect.width() / 2;
            *lprect = rect;
            return true;
         }
         else if(iItem >= iItemLButtonDown + 1)
         {
            rect rectB;
            if(!GetItemRect(rectB, ElementItemBox, iItem + 1))
            {
               GetItemRect(rectB, ElementItemBox, iItem);
            }

            rect rect;
            GetItemRect(rect, ElementItemBox, iItem);

            rect.left = rect.left + rect.width() / 2;
            rect.right = rectB.left + rectB.width() / 2;
            *lprect = rect;
            return true;
         }
         return false;
      }
      else
      {
         return GetItemRect(lprect, eelement, iItem);
      }

   }

   bool list_header::hit_test(POINT point, e_element & eelement, index & iItemParam)
   {
      list * plist = m_plistctrlinterface;
      rect rect;
      for(int32_t iItem = 0; iItem < plist->_001GetColumnCount(); iItem++)
      {
         if(GetItemRect(rect, ElementItemBox, iItem))
         {
            if(rect.contains(point))
            {
               iItemParam = iItem;
               eelement = ElementItemBox;
               return true;
            }
         }
         if(GetItemRect(rect, ElementDivider, iItem))
         {
            if(rect.contains(point))
            {
               iItemParam = iItem;
               eelement = ElementDivider;
               return true;
            }
         }
      }
      return false;
   }

   bool list_header::hit_test(POINT point, e_element eelementLButtonDown, index iItemLButtonDown, e_element & eelement, index & iItemParam)
   {
      list * plist = m_plistctrlinterface;
      rect rect;
      for(int32_t iItem = 0; iItem < plist->_001GetColumnCount(); iItem++)
      {
         if(GetItemRect(rect, eelementLButtonDown, iItemLButtonDown, ElementItemBox, iItem))
         {
            if(rect.contains(point))
            {
               iItemParam = iItem;
               eelement = ElementItemBox;
               return true;
            }
         }
         if(GetItemRect(rect, ElementDivider, iItem))
         {
            if(rect.contains(point))
            {
               iItemParam = iItem;
               eelement = ElementDivider;
               return true;
            }
         }
      }
      return false;
   }


   index list_header::ItemToColumnKey(index iItem)
   {
      
      list * plist = m_plistctrlinterface;

      return plist->_001MapColumnToOrder(iItem);

   }


   bool list_header::DIDDXLayout(bool bSave)
   {
      
      bool bFail = false;
      
      if(!DIDDXColumn(bSave))
         bFail = true;
   
      return !bFail;

   }


   bool list_header::DIDDXColumn(bool bSave)
   {

      string strDataAddUp;

      strDataAddUp = ".local://";

      string str;

      int_array iaWidth;

      str = strDataAddUp+ "::user::list_column_width";

      draw_list_item item(m_plistctrlinterface);

      if (bSave)
      {

         for (index iColumn = 0; iColumn < m_plistctrlinterface->_001GetColumnCount(); iColumn++)
         {

            item.m_iWidthColumn = iColumn;

            m_plistctrlinterface->_001GetColumnWidth(&item);

            if (item.m_bOk)
            {

               iaWidth.add(item.m_iColumnWidth);

            }
            else
            {

               iaWidth.add(-1);

            }

         }

         if (!data_save(str + m_plistctrlinterface->m_dataid.m_id, iaWidth))
            return false;

      }
      else
      {

         if (data_load(str + m_plistctrlinterface->m_dataid.m_id, iaWidth))
         {

            ::count c = MIN(iaWidth.get_count(), m_plistctrlinterface->_001GetColumnCount());

            for (index iColumn = 0; iColumn < c; iColumn++)
            {

               m_plistctrlinterface->_001SetColumnWidth(iColumn, MAX(iaWidth[iColumn], 50));

            }

         }

      }

      return true;

   }


   void list_header::install_message_handling(::message::dispatch *pinterface)
   {
      ::user::box::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &list_header::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &list_header::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_LBUTTONDBLCLK, pinterface, this, &list_header::_001OnLButtonDblClk);
      IGUI_WIN_MSG_LINK(WM_MOUSEMOVE, pinterface, this, &list_header::_001OnMouseMove);
   }

   void list_header::_001OnLButtonDown(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);
      point ptCursor = pmouse->m_pt;
      ScreenToClient(&ptCursor);
      if(hit_test(ptCursor, m_eelementLButtonDown, m_iItemLButtonDown))
      {
         m_bLButtonDown = true;
      }

      pmouse->m_bRet = false;
   }

   void list_header::_001OnLButtonUp(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);
      list * plist = m_plistctrlinterface;
      point ptCursor = pmouse->m_pt;
      ScreenToClient(&ptCursor);
      if(m_bLButtonDown)
      {
         m_bLButtonDown = false;
         e_element eelement;
         index iItem;
         if(hit_test(ptCursor, eelement, iItem))
         {
            if(m_eelementLButtonDown == ElementItemBox
               && eelement == ElementItemBox)
            {
               if(iItem == m_iItemLButtonDown)
               {
                  // This is a single click in a header item
                  plist->_001OnListHeaderItemClick(iItem);
               }
               else // iItem != m_iItemLButtonDown
               {
                  // The header item has been dragged

                  int_ptr iKeyA = plist->m_columna.OrderToKey(iItem);
                  int_ptr iKeyB = plist->m_columna.OrderToKey(iItem);
                  int_ptr iOrderA = plist->m_columna._001GetByKey(iKeyA)->m_iOrder;
                  int_ptr iOrderB = plist->m_columna._001GetByKey(iKeyB)->m_iOrder;
                  plist->m_columna._001GetByKey(iKeyA)->m_iOrder = iOrderB;
                  plist->m_columna._001GetByKey(iKeyB)->m_iOrder = iOrderA;
                  plist->_001OnColumnChange();
                  plist->DISaveOrder();
                  plist->Redraw();
                  Redraw();
               }
            }
            else if(m_eelementLButtonDown == ElementDivider)
            {
               rect rect;
               GetItemRect(rect, element_item, m_iItemLButtonDown);
               rect.right = ptCursor.x;
               int32_t iNewWidth = rect.width();
               plist->_001SetColumnWidth(m_iItemLButtonDown, MAX(0, iNewWidth));
               plist->Redraw();
               Redraw();
            }

         }
      }

      pmouse->m_bRet = false;
   }



   void list_header::_001OnMouseMove(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);
      point ptCursor = pmouse->m_pt;
      ScreenToClient(&ptCursor);
      list * plist = m_plistctrlinterface;
      e_element eelement;
      index iItem;
      if(hit_test(ptCursor, eelement, iItem))
      {
         if(!m_bTrack)
         {
            m_bTrack = true;
            // trans SetCapture();
         }
         m_bHover = true;
         m_eelementHover = eelement;
         m_iItemHover = iItem;
      }
      else
      {
         if(m_bTrack)
         {
            m_bTrack = false;
            Session.ReleaseCapture();
         }
         if(m_bHover)
         {
            m_bHover = false;
         }
      }

      if(m_bLButtonDown)
      {
           if(m_eelementLButtonDown == ElementDivider)
         {
            rect rect;
            GetItemRect(rect, element_item, m_iItemLButtonDown);
            rect.right = ptCursor.x;
            int32_t iNewWidth = rect.width();
            plist->_001SetColumnWidth(m_iItemLButtonDown, MAX(0, iNewWidth));
            plist->Redraw();
            Redraw();
         }
      }


      if(m_bHover && m_eelementHover == ElementDivider)
      {
         pmouse->m_ecursor = ::visual::cursor_size_horizontal;
      }
      else
      {
         pmouse->m_ecursor = ::visual::cursor_arrow;
      }

      pmouse->m_bRet = false;
   }


   void list_header::_001OnLButtonDblClk(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);
      point ptCursor = pmouse->m_pt;
      ScreenToClient(&ptCursor);
      list * plist = m_plistctrlinterface;
      e_element eelement;
      index iItem;
      if(hit_test(
         ptCursor,
         eelement,
         iItem)
         )
      {
         if(eelement == ElementItemBox)
         {
            plist->_001OnListHeaderItemDblClk(iItem);
         }
      }

      pmouse->m_bRet = false;
   }

   void list_header::_001OnDraw(::draw2d::graphics * pgraphics)
   {


      //pgraphics->OffsetViewportOrg(0, 0);
      

      rect rectClient;

      GetClientRect(rectClient);

      rect rectUpdate(rectClient);

      rectUpdate.bottom;
      rect rectClipBox;

      pgraphics->GetClipBox(rectClipBox);

      if(rectClipBox.is_empty())
      {
         rectClipBox = rectClient;
      }

      rectUpdate.intersect(rectUpdate, rectClipBox);

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      pgraphics->FillSolidRect(rectUpdate, _001GetColor(::user::color_list_header_background));

      ::draw2d::item drawitem;
      drawitem.m_pgraphics = pgraphics;
      list * plist = m_plistctrlinterface;
      rect rectDivider;
      ::draw2d::pen_sp p(allocer());
      p->create_solid(1.0, Session.get_default_color(COLOR_BTNSHADOW));
      for(int32_t iItem = 0; iItem < plist->_001GetColumnCount(); iItem++)
      {
         drawitem.itemID = iItem;
         GetItemRect(&drawitem.rcItem, ElementItemBox, iItem);
         DrawItem(&drawitem);
         GetItemRect(rectDivider, ElementDivider, iItem);
         pgraphics->SelectObject(p);
         pgraphics->MoveTo(rectDivider.left, rectDivider.top);
         pgraphics->LineTo(rectDivider.left, rectDivider.bottom);
         //pgraphics->Draw3dRect(rectDivider, Session.get_default_color(COLOR_BTNSHADOW), Session.get_default_color(COLOR_BTNHIGHLIGHT));
      }

   }

   int32_t list_header::GetDividerWidth()
   {
      return 4;
   }


   point list_header::get_parent_viewport_offset()
   {

      sp(::user::interaction) puser = GetParent();

      if(puser.is_null())
         return point(0,0);

      puser = puser->GetParent();

      if(puser.is_null())
         return point(0,0);

      return puser->get_viewport_offset();

   }

} // namespace user
