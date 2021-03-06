#include "framework.h" // from "base/user/user.h"

// pgraphics->GetTextExtent("->:<-"); // oh no!! omg!! The size is the size of the alien!!
#define MAGIC_PALACE_TAB_SPLT "->:<-"
#define MAGIC_PALACE_TAB_SIZE "-/-"
#define MAGIC_PALACE_TAB_TEXT "/"





namespace wndfrm_core
{


   theme::theme(::aura::application * papp):
      object(papp),
      ::user::theme(papp)
   {

      create_translucency(::user::element_none,::user::translucency_present);
      create_point_font(::user::font_default, FONT_SANS, 12.0);

      create_point_font(::user::font_tab, FONT_SANS, 12.0);
      {
         auto font = create_point_font(::user::font_tab_hover, FONT_SANS, 12.0);
         font->m_bUnderline = true;
      }
      create_point_font(::user::font_tab_sel, FONT_SANS, 12.0, 800);
      {
         auto font = create_point_font(::user::font_tab_sel_hover, FONT_SANS, 12.0, 800);
         font->m_bUnderline = true;
      }
      create_point_font(::user::font_tab_big_bold, FONT_SANS_EX, 15.0, 800);

      create_color(::user::color_text, ARGB(255, 0, 0, 0));
      create_color(::user::color_edit_text, ARGB(255, 0, 0, 0));
      create_color(::user::color_background, ARGB(255, 255, 255, 255));
      create_color(::user::color_list_background, ARGB(255, 255, 255, 255));
      create_color(::user::color_edit_background, ARGB(255, 255, 255, 255));
      create_color(::user::color_tree_background, ARGB(255, 255, 255, 255));
      create_color(::user::color_view_background, ARGB(255, 255, 255, 255));
      create_color(::user::color_scrollbar_background, ARGB(255, 255, 255, 255));
      create_color(::user::color_tab_layout_background, ARGB(255, 255, 255, 255));
      create_color(::user::color_tab_client_background, ARGB(255, 255, 255, 255));
      create_color(::user::color_split_layout_background, ARGB(255, 255, 255, 255));
      create_color(::user::color_background_selected, ARGB(255, 155, 185, 255));
      create_color(::user::color_list_header, ARGB(255, 255, 255, 255));
      create_color(::user::color_list_header_background, ARGB(255, 0x58, 0x5C, 0x5D));
      create_color(::user::color_list_item_background, ARGB(30, 0, 0, 0));
      create_color(::user::color_list_item_background_hover, ARGB(50, 0, 0, 0));
      create_color(::user::color_list_item_background_selected, ARGB(255, 160, 210, 235));
      create_color(::user::color_list_item_background_selected_hover, ARGB(255, 160, 220, 255));
      create_color(::user::color_list_header_separator, 0);
      create_color(::user::color_toolbar_background, ARGB(255, 0xF3, 0XF5, 0xF5));
      create_color(::user::color_button_background_disabled, ARGB(255, 128, 128, 128));
      create_color(::user::color_button_background_hover, ARGB(255, 0x55, 0x99, 0xC2));
      create_color(::user::color_button_background, ARGB(255, 0x30, 0x75, 0xA0));
      create_color(::user::color_button_background_press, ARGB(255, 0x07, 0x6D, 0x91));
      create_color(::user::color_button_text_disabled, ARGB(255, 0xD0, 0xD0, 0xD0));
      create_color(::user::color_button_text_hover, ARGB(255, 255, 255, 255));
      create_color(::user::color_button_text, ARGB(255, 255, 255, 255));
      create_color(::user::color_button_text_press, ARGB(255, 255, 255, 255));

   }


   theme::~theme()
   {

   }


   bool theme::_001TabOnDrawSchema01(::draw2d::graphics * pgraphics,::user::tab * ptab)
   {

      class rect rect;
      class rect rectBorder;
      class rect rectText;
      class rect rectClient;
      class rect rectIcon;
      class rect rectClose;

      ptab->get_data()->m_pen->create_solid(1,RGB(32,32,32));

      pgraphics->set_text_rendering_hint(::draw2d::text_rendering_hint_anti_alias_grid_fit);

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      int32_t iTab = -1;

      ::draw2d::brush_sp brushText;

      for(int32_t iPane = 0; iPane < ptab->get_data()->m_panea.get_size(); iPane++)
      {

         ::user::tab_pane & pane = ptab->get_data()->m_panea(iPane);

         if(!pane.m_bTabPaneVisible)
            continue;

         iTab++;

         if(!ptab->get_element_rect(iTab,rect, ::user::element_tab))
            continue;

         if(!ptab->get_element_rect(iTab,rectBorder, ::user::element_border))
            continue;

         if(!ptab->get_element_rect(iTab,rectClient, ::user::element_client))
            continue;

         bool bTextRect = ptab->get_element_rect(iTab, rectText, ::user::element_text);

         if(ptab->get_data()->m_bVertical)
         {

            if(ptab->get_element_rect(iTab,rectIcon, ::user::element_icon))
            {

               pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

               pane.m_dib->bitmap_blend(pgraphics,rectIcon);

            }

            ::draw2d::path_sp path(allocer());

            if(ptab->get_data()->m_idaSel.contains(pane.m_id))
            {

               path->add_line(rectBorder.right,rectBorder.bottom,rectBorder.left + 1,rectBorder.bottom);

               path->add_line(rectBorder.left,rectBorder.top - (rectBorder.left - rectClient.left));

               path->add_line(rectClient.left,rectBorder.top);

               path->add_line(rectBorder.right,rectBorder.top);

               path->end_figure(false);

               pane.m_brushFillSel->CreateLinearGradientBrush(rectBorder.top_left(),rectBorder.bottom_left(),ARGB(230,235,235,230),ARGB(250,255,255,250));

               pgraphics->SelectObject(pane.m_brushFillSel);

               pgraphics->fill_path(path);

               pgraphics->SelectObject(ptab->get_data()->m_penBorderSel);

               pgraphics->draw_path(path);

               if (iTab == ptab->m_iHover && ptab->m_eelementHover != ::user::element_close_tab_button && (ptab->m_eelementHover < ::user::element_split || ptab->m_eelementHover >(::user::element_split + 100)))
               {

                  pgraphics->set_font(ptab->_001GetFont(::user::font_tab_sel_hover));

               }
               else
               {

                  pgraphics->set_font(ptab->_001GetFont(::user::font_tab_sel));

               }

               brushText = ptab->get_data()->m_brushTextSel;

            }
            else
            {

               //path->start_figure();

               path->add_line(rectBorder.right,rectBorder.bottom,rectBorder.left + 1,rectBorder.bottom);
               path->add_line(rectBorder.left,rectBorder.top - (rectBorder.left - rectClient.left));
               path->add_line(bTextRect ? rectText.left : rectBorder.left,rectBorder.top);
               path->add_line(rectBorder.right,rectBorder.top);
               path->add_line(rectBorder.right,rectBorder.bottom);

               path->end_figure(true);

               if(iTab == ptab->m_iHover && ptab->m_eelementHover != ::user::element_close_tab_button && (ptab->m_eelementHover < ::user::element_split || ptab->m_eelementHover >(::user::element_split + 100)))
               {

                  pane.m_brushFillHover->CreateLinearGradientBrush(rectBorder.top_left(),rectBorder.bottom_left(),ARGB(230,215,215,210),ARGB(250,235,235,230));

                  pgraphics->SelectObject(pane.m_brushFillHover);

                  pgraphics->fill_path(path);

                  pgraphics->SelectObject(ptab->get_data()->m_penBorderHover);

                  pgraphics->draw_path(path);

                  pgraphics->set_font(ptab->_001GetFont(::user::font_tab_hover));

                  brushText = ptab->get_data()->m_brushTextHover;

               }
               else
               {

                  pane.m_brushFill->CreateLinearGradientBrush(rectBorder.top_left(),rectBorder.bottom_left(),ARGB(230,175,175,170),ARGB(250,195,195,190));

                  pgraphics->SelectObject(pane.m_brushFill);

                  pgraphics->fill_path(path);

                  pgraphics->SelectObject(ptab->get_data()->m_penBorder);

                  pgraphics->draw_path(path);

                  pgraphics->set_font(ptab->_001GetFont(::user::font_tab));

                  brushText = ptab->get_data()->m_brushText;

               }

            }

         }
         else
         {

            if(ptab->get_element_rect(iTab,rectIcon, ::user::element_icon))
            {

               pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

               pane.m_dib->bitmap_blend(pgraphics,rectIcon);

            }

            ::draw2d::path_sp path(allocer());

            if(ptab->get_data()->m_idaSel.contains(pane.m_id))
            {

               path->add_line(rectBorder.left,rectClient.bottom,rectBorder.left,rectBorder.top);

               path->add_line(rectClient.right,rectBorder.top);

               path->add_line(rectBorder.right,rectBorder.top + (rectBorder.right - rectClient.right));

               path->add_line(rectBorder.right - 1,rectClient.bottom);

               path->end_figure(false);

               pane.m_brushFillSel->CreateLinearGradientBrush(rectBorder.top_left(),rectBorder.bottom_left(),ARGB(230,235,235,230),ARGB(250,255,255,250));

               pgraphics->SelectObject(pane.m_brushFillSel);

               pgraphics->fill_path(path);

               ptab->get_data()->m_penBorderSel->create_solid(1.0,ARGB(255,0,0,0));

               pgraphics->SelectObject(ptab->get_data()->m_penBorderSel);

               pgraphics->draw_path(path);

               if (iTab == ptab->m_iHover && ptab->m_eelementHover != ::user::element_close_tab_button && (ptab->m_eelementHover < ::user::element_split || ptab->m_eelementHover >(::user::element_split + 100)))
               {

                  pgraphics->set_font(ptab->_001GetFont(::user::font_tab_sel_hover));

               }
               else
               {

                  pgraphics->set_font(ptab->_001GetFont(::user::font_tab_sel));

               }

               brushText = ptab->get_data()->m_brushTextSel;

            }
            else
            {

               //path->begin_figure(true, ::draw2d::fill_mode_winding);

               path->add_line(rectBorder.left,rectClient.bottom,rectBorder.left,rectBorder.top);

               path->add_line(rectClient.right,rectBorder.top);

               path->add_line(rectBorder.right,rectBorder.top + (rectBorder.right - rectClient.right));

               path->add_line(rectBorder.right - 1,rectClient.bottom);

               path->end_figure(true);

               if(iTab == ptab->m_iHover && ptab->m_eelementHover != ::user::element_close_tab_button && (ptab->m_eelementHover < ::user::element_split || ptab->m_eelementHover >(::user::element_split + 100)))
               {

                  pane.m_brushFillHover->CreateLinearGradientBrush(rectBorder.top_left(),rectBorder.bottom_left(),ARGB(230,215,215,210),ARGB(250,235,235,230));

                  pgraphics->SelectObject(pane.m_brushFillHover);

                  pgraphics->fill_path(path);

                  pgraphics->SelectObject(ptab->get_data()->m_penBorderHover);

                  pgraphics->draw_path(path);

                  pgraphics->set_font(ptab->_001GetFont(::user::font_tab_hover));

                  brushText = ptab->get_data()->m_brushTextHover;

               }
               else
               {

                  pane.m_brushFill->CreateLinearGradientBrush(rectBorder.top_left(),rectBorder.bottom_left(),ARGB(230,175,175,170),ARGB(250,195,195,190));

                  pgraphics->SelectObject(pane.m_brushFill);

                  pgraphics->fill_path(path);

                  pgraphics->SelectObject(ptab->get_data()->m_penBorder);

                  pgraphics->draw_path(path);

                  pgraphics->set_font(ptab->_001GetFont(::user::font_tab));

                  brushText = ptab->get_data()->m_brushTextSel;

               }

            }

         }

         if(bTextRect)
         {

            _001OnTabPaneDrawTitle(pane,ptab,pgraphics,rectText,brushText);

         }

         if(ptab->get_element_rect(iTab,rectClose, ::user::element_close_tab_button))
         {

            pgraphics->set_font(ptab->_001GetFont(::user::font_tab_big_bold));

            if(iTab == ptab->m_iHover && ptab->m_eelementHover == ::user::element_close_tab_button)
            {

               brushText = ptab->get_data()->m_brushCloseHover;

            }
            else
            {

               brushText = ptab->get_data()->m_brushClose;

            }

            pgraphics->SelectObject(brushText);

            pgraphics->draw_text("x",rectClose,DT_CENTER | DT_VCENTER);

         }

      }

      return true;

   }


   void theme::_001OnTabPaneDrawTitle(::user::tab_pane & pane,::user::tab * ptab,::draw2d::graphics * pgraphics,LPCRECT lpcrect,::draw2d::brush_sp & brushText)
   {

      stringa & straTitle = pane.m_straTitle;

      pgraphics->SelectObject(brushText);

      if(straTitle.get_count() <= 1)
      {

         pgraphics->_DrawText(pane.get_title(),*lpcrect,DT_LEFT | DT_BOTTOM | DT_NOPREFIX);

      }
      else
      {

         ::rect rectText(lpcrect);

         ::draw2d::font_sp font;
         font = pgraphics->get_current_font();
         size sSep = ptab->get_data()->m_sizeSep;
         ::rect rectEmp;
         for(index i = 0; i < straTitle.get_size(); i++)
         {
            string str = straTitle[i];
            size s = pane.m_sizeaText[i];
            rectText.right =rectText.left + s.cx;
            pgraphics->_DrawText(str,rectText,DT_LEFT | DT_BOTTOM | DT_NOPREFIX);
            rectText.left += s.cx;
            if(i < straTitle.get_upper_bound())
            {
               rectText.right = rectText.left + sSep.cx;
               rectEmp = rectText;
               rectEmp.deflate(1,1);
               ::draw2d::e_alpha_mode emode = pgraphics->m_ealphamode;
               pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
               if(ptab->m_eelementHover == (int)::user::element_split + i)
               {
                  pgraphics->fill_solid_rect(rectEmp,ARGB(128,149,184,255));
                  pgraphics->SelectObject(ptab->get_data()->m_brushTextHover);
               }
               else
               {
                  //pgraphics->fill_solid_rect(rectEmp,ARGB(128,208,223,233));
                  pgraphics->SelectObject(ptab->get_data()->m_brushText);
               }
               pgraphics->set_font(ptab->_001GetFont(::user::font_tab_big_bold));
               pgraphics->set_alpha_mode(emode);
               pgraphics->_DrawText(MAGIC_PALACE_TAB_TEXT,rectText,DT_CENTER | DT_VCENTER | DT_NOPREFIX);
               rectText.left += sSep.cx;
               pgraphics->selectFont(font);
               pgraphics->SelectObject(brushText);
            }
         }

      }

   }


   bool theme::_001OnTabLayout(::user::tab * ptab)
   {

      if (!ptab->get_data()->m_bCreated)
      {

         return false;

      }

      auto * pdata = ptab->get_data();

      if (pdata->m_bVertical)
      {

         pdata->m_rectMargin.set(2, 3, 1, 0);

         pdata->m_rectBorder.set(11, 1, 6, 0);

         pdata->m_rectTextMargin.set(3, 0, 1, 0);

         pdata->m_iTabHeight += pdata->m_rectBorder.top + pdata->m_rectBorder.bottom +
                                pdata->m_rectMargin.top + pdata->m_rectMargin.bottom;

         pdata->m_iTabWidth += pdata->m_rectBorder.left + pdata->m_rectBorder.right +
                               pdata->m_rectMargin.left + pdata->m_rectMargin.right;

      }
      else
      {

         pdata->m_rectMargin.set(3, 2, 0, 1);

         pdata->m_rectBorder.set(0, 1, 11, 6);

         pdata->m_rectTextMargin.set(3, 0, 1, 0);

         pdata->m_iTabHeight += pdata->m_rectBorder.top + pdata->m_rectBorder.bottom +
                                pdata->m_rectMargin.top + pdata->m_rectMargin.bottom;

         pdata->m_iTabWidth += pdata->m_rectBorder.left + pdata->m_rectBorder.right +
                               pdata->m_rectMargin.left + pdata->m_rectMargin.right;

      }

      ptab->defer_handle_auto_hide_tabs(false);

      ::draw2d::memory_graphics pgraphics(allocer());

      pgraphics->SelectObject(ptab->_001GetFont(::user::font_tab_sel));

      ptab->m_dcextension.GetTextExtent(pgraphics,MAGIC_PALACE_TAB_SIZE,ptab->get_data()->m_sizeSep);

      if(ptab->get_data()->m_bVertical)
      {
         int32_t iTabWidth = 16;
         int32_t iTabHeight = 8;
         int32_t cx;
         int32_t cy;
         for(int32_t iPane = 0; iPane < ptab->get_data()->m_panea.get_size(); iPane++)
         {

            ::user::tab_pane & tab_pane = ptab->get_data()->m_panea(iPane);

            if(!tab_pane.m_bTabPaneVisible)
               continue;

            string str = tab_pane.get_title();

            tab_pane.do_split_layout(ptab->m_dcextension,pgraphics);

            ::size size;

            ptab->m_dcextension.GetTextExtent(pgraphics,str,size);



            if(tab_pane.m_dib.is_set())
            {
               size.cx += tab_pane.m_dib->m_size.cx + 2;
               size.cy = MAX(size.cy,tab_pane.m_dib->m_size.cy);
            }
            cx = size.cx + 2;

            if(!tab_pane.m_bPermanent)
            {
               cx += 2 + 16 + 2;
            }

            if(cx > iTabWidth)
            {
               iTabWidth = cx;
            }
            cy = size.cy + 2;
            if(cy > iTabHeight)
            {
               iTabHeight = cy;
            }
         }

         // close tab button
         cy = 2 + 16 + 2;
         if(cy > iTabHeight)
         {
            iTabHeight = cy;
         }

         iTabWidth += ptab->get_data()->m_rectBorder.left + ptab->get_data()->m_rectBorder.right +
                      ptab->get_data()->m_rectMargin.left + ptab->get_data()->m_rectMargin.right +
                      ptab->get_data()->m_rectTextMargin.left + ptab->get_data()->m_rectTextMargin.right;

         ptab->get_data()->m_iTabWidth = iTabWidth;

         iTabHeight += ptab->get_data()->m_rectBorder.top + ptab->get_data()->m_rectBorder.bottom +
                       ptab->get_data()->m_rectMargin.top + ptab->get_data()->m_rectMargin.bottom +
                       ptab->get_data()->m_rectTextMargin.top + ptab->get_data()->m_rectTextMargin.bottom;

         ptab->get_data()->m_iTabHeight = iTabHeight;

         rect rectClient;
         ptab->GetClientRect(rectClient);

         ptab->get_data()->m_rectTab.left       = rectClient.left;
         ptab->get_data()->m_rectTab.top        = rectClient.top;
         ptab->get_data()->m_rectTab.right      = ptab->get_data()->m_rectTab.left + ptab->get_data()->m_iTabWidth;
         ptab->get_data()->m_rectTab.bottom     = rectClient.bottom;

         /*      m_pui->SetWindowPos(
         ZORDER_TOP,
         m_rectTab.left,
         m_rectTab.top,
         m_rectTab.width(),
         m_rectTab.height(),
         0);*/

         ptab->get_data()->m_rectTabClient.left       = ptab->m_bShowTabs ? ptab->get_data()->m_rectTab.right : rectClient.left;
         ptab->get_data()->m_rectTabClient.top        = ptab->get_data()->m_rectTab.top;
         ptab->get_data()->m_rectTabClient.right      = rectClient.right;
         ptab->get_data()->m_rectTabClient.bottom     = ptab->get_data()->m_rectTab.bottom;

      }
      else
      {
         int32_t iTabHeight = 16;
         int32_t cy;
         ::draw2d::graphics_sp graphics(allocer());
         graphics->CreateCompatibleDC(NULL);
         ::draw2d::graphics * pgraphics = graphics;
         pgraphics->SelectObject(ptab->_001GetFont(::user::font_tab_sel));

         rect rectClient;
         ptab->GetClientRect(rectClient);
         int x = rectClient.left;

         int32_t ixAdd;
         for(int32_t iPane = 0; iPane < ptab->get_data()->m_panea.get_size(); iPane++)
         {

            ::user::tab_pane & tab_pane = ptab->get_data()->m_panea(iPane);

            if(!tab_pane.m_bTabPaneVisible)
               return false;

            string str = tab_pane.get_title();

            tab_pane.do_split_layout(ptab->m_dcextension,pgraphics);

            size size;

            ptab->m_dcextension.GetTextExtent(pgraphics,str,size);

            if(tab_pane.m_dib.m_p != NULL)
            {
               size.cy = MAX(size.cy,tab_pane.m_dib->size().cy);
            }
            cy = size.cy + 2;

            if(cy > iTabHeight)
            {
               iTabHeight = cy;
            }

            tab_pane.m_pt.x = x;
            tab_pane.m_pt.y = rectClient.top;


            //            string str = tab_pane.get_title();

            //            size size;

            ixAdd = 5;

            if(tab_pane.m_dib.is_set())
            {
               //::image_list::info ii;
               ixAdd += tab_pane.m_dib->m_size.cx + 2;
            }

            if(!tab_pane.m_bPermanent)
            {
               ixAdd += 2 + 16 + 2;
            }




            tab_pane.m_size.cx = size.cx + ixAdd
                                 + ptab->get_data()->m_rectBorder.left + ptab->get_data()->m_rectBorder.right
                                 + ptab->get_data()->m_rectMargin.left + ptab->get_data()->m_rectMargin.right
                                 + ptab->get_data()->m_rectTextMargin.left + ptab->get_data()->m_rectTextMargin.right;

            x += tab_pane.m_size.cx;
         }

         // close tab button
         cy = 2 + 16 + 2;
         if(cy > iTabHeight)
         {
            iTabHeight = cy;
         }

         iTabHeight += ptab->get_data()->m_rectBorder.top + ptab->get_data()->m_rectBorder.bottom +
                       ptab->get_data()->m_rectMargin.top + ptab->get_data()->m_rectMargin.bottom + ptab->get_data()->m_iHeightAddUp;

         ptab->get_data()->m_iTabHeight = iTabHeight;

         for(int32_t iPane = 0; iPane < ptab->get_data()->m_panea.get_size(); iPane++)
         {

            ::user::tab_pane & tab_pane = ptab->get_data()->m_panea(iPane);

            tab_pane.m_size.cy = iTabHeight;

         }




         ptab->get_data()->m_rectTab.left       = rectClient.left;
         ptab->get_data()->m_rectTab.top        = rectClient.top;
         ptab->get_data()->m_rectTab.right      = rectClient.right;
         ptab->get_data()->m_rectTab.bottom     = ptab->get_data()->m_rectTab.top + ptab->get_data()->m_iTabHeight;

         /*      SetWindowPos(
         ZORDER_TOP,
         m_rectTab.left,
         m_rectTab.top,
         m_rectTab.width(),
         m_rectTab.height(),
         0);*/

         rect & rectTabClient = ptab->get_data()->m_rectTabClient;

         rectTabClient.left       = ptab->get_data()->m_rectTab.left;
         rectTabClient.top        = ptab->m_bShowTabs ? ptab->get_data()->m_rectTab.bottom : rectClient.top;
         rectTabClient.right      = ptab->get_data()->m_rectTab.right;
         rectTabClient.bottom     = rectClient.bottom;

         //TRACE0("rectTabClient");
      }

      for(int32_t iPane = 0; iPane < ptab->get_data()->m_panea.get_size(); iPane++)
      {

         if(iPane != ptab->_001GetSel())
         {

            ptab->layout_pane(iPane);

         }

      }

      ptab->layout_pane(ptab->_001GetSel(), ptab->IsWindowVisible());

      return true;

   }


   bool theme::_001DrawSimpleScrollBar(::draw2d::graphics * pgraphics, simple_scroll_bar * pbar)
   {

      COLORREF crBackground = pbar->_001GetColor(::user::color_scrollbar_background);

      rect rectClient;

      pbar->GetClientRect(rectClient);

      if ((crBackground & ARGB(255, 0, 0, 0)) != 0)
      {

         pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

         if (Session.savings().is_trying_to_save(::aura::resource_processing))
         {

            pgraphics->fill_solid_rect(rectClient, ARGB(255, 255, 255, 255));

         }
         else
         {

            pgraphics->fill_solid_rect(rectClient, ARGB(255, 210, 210, 210));

         }

      }

      rect rectTrack;

      pbar->GetTrackRect(rectTrack);

      class ::rect rectWindow;

      pbar->GetWindowRect(rectWindow);

      COLORREF crBorder = pbar->scrollbar_border_color(::user::element_scrollbar_rect);

      pbar->m_penDraw->create_solid(1, crBorder);

      COLORREF crTrack = pbar->scrollbar_color(::user::element_scrollbar_rect);

      pbar->m_brushDraw->create_solid(crTrack);

      pgraphics->SelectObject(pbar->m_penDraw);

      pgraphics->SelectObject(pbar->m_brushDraw);

      pgraphics->rectangle(rectTrack);

      if (pbar->m_bTracking || (bool)pbar->oprop("tracking_on"))
      {

         DWORD dwFadeIn = 490;

         DWORD dwFadeOut = 490;

         byte uchAlpha = MAX(0, MIN(255, pbar->oprop("tracking_alpha").u32()));

         if (pbar->m_bTracking)
         {

            if (!(bool)pbar->oprop("tracking_on"))
            {

               pbar->oprop("tracking_on") = true;
               pbar->oprop("tracking_start") = (uint32_t)(get_tick_count() + uchAlpha * dwFadeIn / 255);
               pbar->oprop("tracking_fade_in") = true;
               pbar->oprop("tracking_fade_out") = false;
               pbar->oprop("tracking_simple") = System.math().RandRange(1, 2) == 1;
               //oprop("tracking_window") = canew(trw(get_app()));
            }
         }
         else
         {
            if (!(bool)pbar->oprop("tracking_fade_out"))
            {
               pbar->oprop("tracking_fade_in") = false;
               pbar->oprop("tracking_fade_out") = true;
               pbar->oprop("tracking_start") = (uint32_t)(get_tick_count() + (255 - uchAlpha) * dwFadeOut / 255);
            }

         }

         point pt1 = rectTrack.top_left() + pbar->m_ptTrackOffset;

         pbar->ClientToScreen(pt1);

         point pt2;

         Session.get_cursor_pos(&pt2);

         pbar->ClientToScreen(pt1);

         //      oprop("tracking_window").cast < trw >()->pt1 = pt1;

         //    oprop("tracking_window").cast < trw >()->pt2 = pt2;

         if ((bool)pbar->oprop("tracking_fade_in"))
         {
            DWORD dwFade = get_tick_count() - pbar->oprop("tracking_start").u32();
            if (dwFade < dwFadeIn)
            {
               uchAlpha = (byte)MIN(255, MAX(0, (dwFade * 255 / dwFadeIn)));
            }
            else
            {
               uchAlpha = 255;
               pbar->oprop("tracking_fade_in") = false;
            }

         }
         else if ((bool)pbar->oprop("tracking_fade_out"))
         {
            DWORD dwFade = get_tick_count() - pbar->oprop("tracking_start").u32();
            if (dwFade < dwFadeOut)
            {
               uchAlpha = (byte)(255 - MIN(255, MAX(0, (dwFade * 255 / dwFadeOut))));
            }
            else
            {
               uchAlpha = 0;
               pbar->oprop("tracking_on") = false;
               pbar->oprop("tracking_fade_out") = false;
            }

         }
         else
         {
            uchAlpha = 255;
         }

         ::rect rectMachineThumb;

         bool bSimple = (bool)pbar->oprop("tracking_simple");

         if (bSimple)
         {

            int iSize = rectTrack.get_size().get_normal(pbar->m_eorientation) * 6 / 8;

            rectMachineThumb.top_left() = rectTrack.top_left() + pbar->m_ptTrackOffset - size(iSize / 2, iSize / 2);

            rectMachineThumb.bottom_right() = rectMachineThumb.top_left() + size(iSize, iSize);

            ::rect rectIntersect;

            rectIntersect.intersect(rectMachineThumb, rectTrack);

            int32_t iArea = (int32_t)(MAX(1, rectIntersect.area()));

            rectMachineThumb.inflate(1 + iSize * (iSize * iSize) * 4 / (iArea * 5), 1 + iSize * (iSize * iSize) * 2 / (iArea * 3));

            pbar->draw_mac_thumb_simple(pgraphics, rectMachineThumb, rectTrack, uchAlpha);

         }
         else
         {

            int iSize = rectTrack.get_size().get_normal(pbar->m_eorientation);

            rectMachineThumb.top_left() = rectTrack.top_left() + pbar->m_ptTrackOffset - size(iSize / 2, iSize / 2);

            rectMachineThumb.bottom_right() = rectMachineThumb.top_left() + size(iSize, iSize);

            rectMachineThumb.assign_normal(rectTrack, pbar->m_eorientation);

            rectMachineThumb.constraint_v7(rectTrack);

            rectMachineThumb.deflate(1, 1);

            pbar->draw_mac_thumb_dots(pgraphics, rectMachineThumb, rectTrack, uchAlpha);

         }

         pbar->oprop("tracking_alpha") = uchAlpha;

      }



      ::draw2d::pen_sp penGrip(allocer());

      penGrip->create_solid(2.0, pbar->scrollbar_lite_border_color(::user::element_scrollbar_rect));

      pgraphics->SelectObject(penGrip);

      point ptCenter = rectTrack.center();

      if (pbar->m_eorientation == orientation_horizontal)
      {


         pgraphics->move_to(ptCenter.x - 5, ptCenter.y - 5);
         pgraphics->line_to(ptCenter.x - 5, ptCenter.y + 5);
         pgraphics->move_to(ptCenter.x, ptCenter.y - 5);
         pgraphics->line_to(ptCenter.x, ptCenter.y + 5);
         pgraphics->move_to(ptCenter.x + 5, ptCenter.y - 5);
         pgraphics->line_to(ptCenter.x + 5, ptCenter.y + 5);


      }
      else
      {
         pgraphics->move_to(ptCenter.x - 5, ptCenter.y - 5);
         pgraphics->line_to(ptCenter.x + 5, ptCenter.y - 5);
         pgraphics->move_to(ptCenter.x - 5, ptCenter.y);
         pgraphics->line_to(ptCenter.x + 5, ptCenter.y);
         pgraphics->move_to(ptCenter.x - 5, ptCenter.y + 5);
         pgraphics->line_to(ptCenter.x + 5, ptCenter.y + 5);

      }


      ::draw2d::pen_sp penArrow(allocer());

      penArrow->create_solid(1.0, pbar->scrollbar_lite_border_color(::user::element_scrollbar_rectA));

      pgraphics->SelectObject(penArrow);

      pbar->m_brushDraw->create_solid(pbar->scrollbar_color(::user::element_scrollbar_rectA));

      pgraphics->SelectObject(pbar->m_brushDraw);

      pgraphics->rectangle(pbar->m_rectA);

      penArrow->create_solid(1.0, pbar->scrollbar_lite_border_color(::user::element_scrollbar_rectB));

      pgraphics->SelectObject(penArrow);

      pbar->m_brushDraw->create_solid(pbar->scrollbar_color(::user::element_scrollbar_rectB));

      pgraphics->SelectObject(pbar->m_brushDraw);

      pgraphics->rectangle(pbar->m_rectB);

      ::rect rect;

      if (pbar->m_eelement == ::user::element_scrollbar_pageA || pbar->m_eelementHover == ::user::element_scrollbar_pageA)
      {

         pbar->GetPageARect(rectClient, rectTrack, rect);

         pbar->m_brushDraw->create_solid(pbar->scrollbar_color(::user::element_scrollbar_pageA));

         pgraphics->SelectObject(pbar->m_brushDraw);

         pgraphics->fill_rect(rect);

      }
      else if (pbar->m_eelement == ::user::element_scrollbar_pageB || pbar->m_eelementHover == ::user::element_scrollbar_pageB)
      {

         pbar->GetPageBRect(rectClient, rectTrack, rect);

         pbar->m_brushDraw->create_solid(pbar->scrollbar_color(::user::element_scrollbar_pageB));

         pgraphics->SelectObject(pbar->m_brushDraw);

         pgraphics->fill_rect(rect);


      }


      penArrow->m_elinecapBeg = ::draw2d::pen::line_cap_round;
      penArrow->m_elinecapEnd = ::draw2d::pen::line_cap_round;
      penArrow->m_elinejoin = ::draw2d::pen::line_join_round;

      penArrow->create_solid(1.0, pbar->scrollbar_lite_border_color(::user::element_scrollbar_rectA));

      pgraphics->SelectObject(penArrow);

      pgraphics->Polyline(pbar->m_ptaA, 3);

      penArrow->create_solid(1.0, pbar->scrollbar_lite_border_color(::user::element_scrollbar_rectB));

      pgraphics->SelectObject(penArrow);

      pgraphics->Polyline(pbar->m_ptaB, 3);



      return true;



   }


   bool theme::get_color(COLORREF & cr, ::user::e_color ecolor, ::user::style_context * pcontext)
   {

      if (ecolor == ::user::color_background)
      {

         if (pcontext->m_pstyleSource->get_control_type() != ::user::control_type_none)
         {

            cr = ARGB(80, 255, 255, 255);

         }
         else
         {

            cr = 0;

         }

         return true;

      }
      else if (ecolor == ::user::color_background_hover)
      {

         cr = ARGB(120, 110, 100, 200);

         return true;

      }
      else if (ecolor == ::user::color_split_layout_background)
      {

         cr = ARGB(0, 0, 0, 0);

         return true;

      }
      else if (ecolor == ::user::color_list_background)
      {

         cr = ARGB(80, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_edit_background)
      {

         cr = ARGB(80, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_edit_background_selected)
      {

         cr = ARGB(120, 0, 0, 0);

         return true;

      }
      else if (ecolor == ::user::color_text)
      {

         cr = ARGB(255, 0, 0, 0);

         return true;

      }
      else if (ecolor == ::user::color_text_selected)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_background_selected)
      {

         cr = ARGB(128, 0, 0, 0);

         return true;

      }
      else if (ecolor == ::user::color_text_highlight)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_background_highlight)
      {

         cr = ARGB(215, 60, 140, 255);

         return true;

      }
      else if (ecolor == ::user::color_text_selected_highlight)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_background_selected_highlight)
      {

         cr = ARGB(215, 20, 100, 205);

         return true;

      }
      else if (ecolor == ::user::color_edit_text)
      {

         cr = ARGB(255, 0, 0, 0);

         return true;

      }
      else if (ecolor == ::user::color_edit_text_selected)
      {

         cr = ARGB(255, 0, 0, 0);

         return true;

      }
      else if (ecolor == ::user::color_tree_background)
      {

         cr = ARGB(80, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_view_background)
      {

         cr = ARGB(80, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_scrollbar_background)
      {

         cr = ARGB(80, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_list_header)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_list_header_background)
      {

         cr = ARGB(255, 0x58, 0x5C, 0x5D);

         return true;

      }
      else if (ecolor == ::user::color_list_item_background)
      {

         cr = ARGB(90, 0, 0, 0);

         return true;

      }
      else if (ecolor == ::user::color_list_header_separator)
      {

         cr = 0;

         return true;

      }
      else if (ecolor == ::user::color_toolbar_background)
      {

         cr = ARGB(80, 0xF3, 0XF5, 0xF5);

         return true;


      }
      else if (ecolor == ::user::color_button_background_disabled)
      {

         cr = ARGB(255, 128, 128, 128);

         return true;


      }
      else if (ecolor == ::user::color_button_background_hover)
      {

         cr = ARGB(255, 0x55, 0x99, 0xC2);

         return true;

      }
      else if (ecolor == ::user::color_button_background)
      {

         cr = ARGB(255, 0x30, 0x75, 0xA0);

         return true;
      }
      else if (ecolor == ::user::color_button_background_press)
      {

         cr = ARGB(255, 0x07, 0x6D, 0x91);

         return true;
      }
      else if (ecolor == ::user::color_button_text_disabled)
      {

         cr = ARGB(255, 0x80, 0x80, 0x80);

         return true;

      }
      else if (ecolor == ::user::color_button_text_hover)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_button_text)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_button_text_press)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }

      return ::user::theme::get_color(cr, ecolor, pcontext);

   }


} // namespace wndfrm_core









