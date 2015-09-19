//#include "framework.h"

//#ifdef WINDOWS
//#undef new
////#include <gdiplus.h>
//#endif
extern CLASS_DECL_CORE thread_int_ptr < DWORD_PTR > t_time1;

// pdc->GetTextExtent("->:<-"); // oh no!! omg!! The size is the size of the alien!!
#define MAGIC_PALACE_TAB_SPLT "->:<-"
#define MAGIC_PALACE_TAB_SIZE "-/-"
#define MAGIC_PALACE_TAB_TEXT "/"

namespace user
{

   tab::data::data(::aura::application * papp) :
      object(papp),
      ::data::data(papp),
      m_imagelist(papp),
      m_pen(allocer()),
      m_font(allocer()),
      m_fontUnderline(allocer()),
      m_fontBigBold(allocer()),
      m_fontBold(allocer()),
      m_brushTextHover(allocer()),
      m_brushTextSel(allocer()),
      m_brushText(allocer()),
      m_brushCloseHover(allocer()),
      m_brushCloseSel(allocer()),
      m_brushClose(allocer()),
      m_penBorder(allocer()),
      m_penBorderSel(allocer()),
      m_penBorderHover(allocer()),
      m_dcextension(papp),
      m_panea(papp)
   {

      m_brushTextHover->create_solid(ARGB(255, 0, 127, 255));
      m_brushTextSel->create_solid(ARGB(255, 0, 0, 0));
      m_brushText->create_solid(ARGB(163, 0, 0, 0));
      m_brushCloseHover->create_solid(ARGB(255, 255, 127, 0));
      m_brushCloseSel->create_solid(ARGB(255, 0, 0, 0));
      m_brushClose->create_solid(ARGB(163, 0, 0, 0));
      m_penBorderHover->create_solid(1.0, ARGB(255, 0, 0, 0));
      m_penBorderSel->create_solid(1.0, ARGB(255, 0, 0, 0));
      m_penBorder->create_solid(1.0, ARGB(163, 84, 84, 77));

      m_bEnableCloseAll = false;
      m_rectTabClient.set(0,0,0,0);


   }

   tab::data::~data()
   {
   }

   tab_pane * tab::data::get_pane_by_id(id id)
   {
      return m_panea.get_by_id(id);
   }

   ::count tab::data::get_visible_tab_count()
   {
      return m_panea.get_visible_count();
   }

   tab::tab(::aura::application * papp) :
      object(papp),
      place_holder_container(papp),
      m_dcextension(papp)
   {

      m_spdata = new data(papp);
      get_data()->m_panea.set_app(papp);
      get_data()->m_iHeightAddUp = 0;
      get_data()->m_pcallback    = NULL;
      get_data()->m_bCreated     = false;
      get_data()->m_iTabHeight   = 16;
      get_data()->m_iTabWidth    = 48;
      get_data()->m_imagelist.create(16, 16, 0, 0, 0);

      get_data()->m_iDragTab     = -1;

      m_bRestoringTabs           = true;

      m_bShowTabs                = true;

      m_bHidingTabs              = false;


      m_bNoTabs = System.directrix()->m_varTopicQuery.has_property("no_tabs");


      //m_rectBorder.set(7, 1, 7, 0);


      ::draw2d::graphics_sp spgraphics(allocer());
      spgraphics->CreateCompatibleDC(NULL);


      get_data()->m_font->create_point_font("Lucida Sans Unicode", 10);

      get_data()->m_fontBold->create_point_font("Lucida Sans Unicode", 10, FW_BOLD);

      get_data()->m_fontBigBold->create_point_font("Lucida Sans Unicode",12,FW_EXTRABOLD);

      get_data()->m_fontUnderline->create_point_font("Lucida Sans Unicode", 10, FW_NORMAL, false, true);

      _001SetVertical(false);

   }

   tab::~tab()
   {
   }

   /*bool tab::add_tab(UINT uiIdTitle, int32_t iId)
   {
      tab_pane tab_pane;
      tab_pane.m_uiId = uiIdTitle;
      tab_pane.m_istrTitleEx = L"";
      tab_pane.m_iId = iId == -1 ? get_data()->m_panea.get_size() : iId;
      if(!tab_pane.m_wstrTitle.load_string(uiIdTitle))
      {
         return false;
      }*/
   //   tab_pane.m_pcontainer = pcontainer;
     // tab_pane.m_typeinfo = NULL;
     /* tab_pane.m_iImage = -1;

      get_data()->m_panea.add(tab_pane);
      return true;
   }*/

   bool tab::set_title(::index iPane, const char * lpcsz)
   {
      get_data()->m_panea[iPane]->m_istrTitleEx = lpcsz;
      return true;
   }

   bool tab::SetTitleById(id id, const char * lpcsz)
   {
      get_data()->m_panea[get_tab_by_id(id)]->m_istrTitleEx = lpcsz;
      return true;
   }

   bool tab::set_tab(const char * lpcsz, id id, bool bVisible)
   {
      return add_tab(lpcsz, id, bVisible, true);
   }

   bool tab::add_tab(const char * lpcsz, id id, bool bVisible, bool bPermanent)
   {

      tab_pane * ppane = new tab_pane(get_app());

      ppane->m_istrTitleEx       = lpcsz;
      ppane->m_bVisible         = bVisible;
      ppane->m_bPermanent       = bPermanent;

      if(id.is_empty())
         id = get_data()->m_panea.get_size();

      ppane->m_id               = id;
      ppane->m_dib.release();
      ppane->m_pholder          = NULL;

      get_data()->m_panea.add(ppane);

      on_change_pane_count();

      return true;

   }

   bool tab::remove_tab_by_id(id id)
   {

      for(int32_t i = 0; i < get_data()->m_panea.get_count(); i++)
      {

         if(get_data()->m_panea[i]->m_id == id)
         {

            get_data()->m_panea.remove_at(i);

         }

      }

      if(get_data()->m_matchanyRestore.get_count() > 0)
      {

         var_array vara;

         data_load("restore_tab", vara);

         if(vara.remove_ci(id) > 0)
         {

            data_save("restore_tab", vara);

         }

      }

      return true;
   }

   bool tab::set_image_tab(const char * lpcszTitle, const char * pszImage, id id, bool bVisible)
   {
      return add_image_tab(lpcszTitle, pszImage, id, bVisible, true);
   }

   bool tab::add_image_tab(const char * lpcszTitle, const char * pszImage, id id, bool bVisible, bool bPermanent)
   {

      tab_pane * ppane = new tab_pane(get_app());

      if(ppane == NULL)
         return false;

      ppane->m_bVisible    = bVisible;
      ppane->m_bPermanent  = bPermanent;
      ppane->m_istrTitleEx  = lpcszTitle;

      if(id.is_empty())
         id = get_data()->m_panea.get_size();

      ppane->m_id          = id;
      ppane->m_pholder     = NULL;
      ppane->m_dib.alloc(allocer());
      ppane->m_dib.load_from_file(pszImage);

      get_data()->m_panea.add(ppane);

      on_change_pane_count();

      return true;
   }

   void tab::remove_tab(::index iPane, bool bVisible)
   {

      if(iPane < 0 || iPane >= get_data()->m_panea.get_size())
         return;

      if(bVisible)
      {
         for(int32_t i = 0; iPane >= 0 && i < get_data()->m_panea.get_count(); i++)
         {
            if(get_data()->m_panea[i]->m_bVisible)
            {
               if(iPane <= 0)
               {
                  get_data()->m_panea.remove_at(i);
                  on_change_pane_count();
                  break;
               }
               else
               {
                  iPane--;
               }
            }
         }
      }
      else
      {
         get_data()->m_panea.remove_at(iPane);
         on_change_pane_count();
      }
   }


   bool tab::defer_handle_full_screen_show_tabs(bool bLayout)
   {

      //sp(::user::wndfrm::frame::WorkSetClientInterface) pinterface = GetTopLevelFrame();
      //
      //if (pinterface != NULL && pinterface->m_bInitialFramePosition)
      //{
      //   return false;
      //}

      bool bNeedLayout = false;

      if(m_bNoTabs)
      {

         if(m_bShowTabs)
         {

            m_bShowTabs = false;

            if (bLayout)
            {

               layout();

            }

            bNeedLayout = true;

         }

         return bNeedLayout;

      }

      if(m_bShowTabs)
      {

         if(GetParentFrame()->WfiIsFullScreen())
         {

            rect rectTab(get_data()->m_rectTab);

            ClientToScreen(rectTab);

            point ptCursor;

            Session.get_cursor_pos(&ptCursor);

            bool bShowTabs = rectTab.contains(ptCursor);

            if(bShowTabs)
            {

               m_bHidingTabs = false;

            }
            else if(m_bHidingTabs)
            {

               if(get_tick_count() - m_dwHidingTabs > 884)
               {

                  m_bHidingTabs = false;

                  m_bShowTabs = false;

                  if (bLayout)
                  {

                     layout();

                  }

                  bNeedLayout = true;

               }

            }
            else
            {

               m_bHidingTabs = true;

               m_dwHidingTabs = get_tick_count();

            }

         }

      }
      else
      {

         if(!GetParentFrame()->WfiIsFullScreen())
         {

            m_bShowTabs = true;

            if (bLayout)
            {

               layout();

            }

            bNeedLayout = true;

         }
         else
         {

            rect rectWindow;

            GetWindowRect(rectWindow);

            bool bShowTabs;

            point ptCursor;

            Session.get_cursor_pos(&ptCursor);

            if(get_data()->m_bVertical)
            {
               bShowTabs = ptCursor.x <= rectWindow.left;
            }
            else
            {
               bShowTabs = ptCursor.y <= rectWindow.top;
            }

            m_bShowTabs = bShowTabs;

            if(bShowTabs)
            {

               if (bLayout)
               {

                  layout();

               }

               bNeedLayout = true;

            }

         }

      }

      return bNeedLayout;

   }


   void tab::_001OnDraw(::draw2d::graphics * pdc)
   {

      defer_handle_full_screen_show_tabs();

      if(m_bNoTabs || !m_bShowTabs)
         return;

      if(get_data()->is_locked())
         return;
      
      if(m_puserschema == NULL)
      {
         
         _001OnDrawStandard(pdc);

         return;

      }
      

      m_puserschema->_001TabOnDrawSchema01(pdc,this);
//      _001OnDrawSchema01(pdc);

   }


   void tab::_001OnDrawStandard(::draw2d::graphics * pdc)
   {

      class rect rect;
      class rect rectBorder;
      class rect rectText;
      class rect rectClient;
      class rect rectIcon;
      class rect rectClose;

      get_data()->m_pen->create_solid(1,RGB(32,32,32));

      pdc->set_text_rendering(::draw2d::text_rendering_anti_alias_grid_fit);

      pdc->set_alpha_mode(::draw2d::alpha_mode_blend);

      pdc->FillSolidRect(get_data()->m_rectTab, ARGB(0xc0, 250, 255, 255));

      pdc->set_alpha_mode(::draw2d::alpha_mode_set);

      int32_t iVisiblePane = 0;

      ::draw2d::brush_sp brushText(allocer());

      for(int32_t iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
      {

         tab_pane & tab_pane = get_data()->m_panea(iPane);

         if(!tab_pane.m_bVisible)
            continue;



         if(!get_element_rect(iVisiblePane, rect, element_tab))
            continue;

         if(!get_element_rect(iVisiblePane, rectBorder, element_border))
            continue;

         if(!get_element_rect(iVisiblePane, rectClient, element_client))
            continue;


         if(get_data()->m_bVertical)
         {
            if(get_element_rect(iVisiblePane, rectIcon, element_icon))
            {
               pdc->set_alpha_mode(::draw2d::alpha_mode_blend);
               tab_pane.m_dib->bitmap_blend(pdc, rectIcon);
            }

            if(get_data()->m_iaSel.contains(iPane))
            {

               ::draw2d::pen_sp pen(allocer());

               pen->create_solid(1.0, ARGB(255, 0, 0, 0));

               pdc->SelectObject(pen);

               pdc->MoveTo(rectBorder.right, rectBorder.bottom);
               pdc->LineTo(rectBorder.left + 1, rectBorder.bottom);
               pdc->LineTo(rectBorder.left, rectBorder.top - (rectBorder.left - rectClient.left));
               pdc->LineTo(rectClient.left, rectBorder.top);
               pdc->LineTo(rectBorder.right, rectBorder.top);

               pdc->set_font(get_data()->m_fontBold);

               brushText = get_data()->m_brushTextSel;

            }
            else
            {

               ::draw2d::pen_sp pen(allocer());

               pen->create_solid(1.0, ARGB(255, 0, 0, 0));

               pdc->SelectObject(pen);

               pdc->MoveTo(rectBorder.right, rectBorder.bottom);
               pdc->LineTo(rectBorder.left + 1, rectBorder.bottom);
               pdc->LineTo(rectBorder.left, rectBorder.top - (rectBorder.left - rectClient.left));
               pdc->LineTo(rectText.left, rectBorder.top);
               pdc->LineTo(rectBorder.right, rectBorder.top);
               pdc->LineTo(rectBorder.right, rectBorder.bottom);
               if(iVisiblePane == m_iHover && m_eelementHover != element_close_tab_button)
               {
                  pdc->set_font(get_data()->m_fontUnderline);
                  brushText = get_data()->m_brushClose;
               }
               else
               {
                  pdc->set_font(get_data()->m_font);
                  brushText = get_data()->m_brushText;
               }
            }


         }
         else
         {

            if(get_element_rect(iVisiblePane, rectIcon, element_icon))
            {
               pdc->set_alpha_mode(::draw2d::alpha_mode_blend);
               tab_pane.m_dib->bitmap_blend(pdc, rectIcon);
            }

            if(get_data()->m_iaSel.contains(iPane))
            {

               ::draw2d::pen_sp pen(allocer());

               pen->create_solid(1.0, ARGB(255, 0, 0, 0));

               pdc->SelectObject(pen);

               pdc->MoveTo(rectBorder.left, rectClient.bottom);
               //pdc->LineTo(rectBorder.left, rectText.bottom);
               pdc->LineTo(rectBorder.left, rectBorder.top);
               pdc->LineTo(rectClient.right, rectBorder.top);
               pdc->LineTo(rectBorder.right, rectBorder.top + (rectBorder.right - rectClient.right));
               pdc->LineTo(rectBorder.right - 1, rectClient.bottom);
               //pdc->LineTo(rect.right, rectText.bottom);
               pdc->set_font(get_data()->m_fontBold);
               brushText->create_solid(ARGB(255, 0, 0, 0));
            }
            else
            {

               ::draw2d::pen_sp pen(allocer());

               pen->create_solid(1.0, ARGB(255, 0, 0, 0));

               pdc->SelectObject(pen);

               //pdc->MoveTo(rect.left, rectBorder.bottom);
               //pdc->LineTo(rect.right, rectBorder.bottom);
               pdc->MoveTo(rectBorder.left, rectClient.bottom);
               pdc->LineTo(rectBorder.left, rectBorder.top);
               pdc->LineTo(rectClient.right, rectBorder.top);
               pdc->LineTo(rectBorder.right, rectBorder.top + (rectBorder.right - rectClient.right));
               pdc->LineTo(rectBorder.right - 1, rectClient.bottom);
               pdc->LineTo(rectBorder.left, rectClient.bottom);

               if(iVisiblePane == m_iHover && m_eelementHover != element_close_tab_button)
               {
                  pdc->set_font(get_data()->m_fontUnderline);
                  brushText->create_solid(ARGB(255, 0, 127, 255));
               }
               else
               {
                  pdc->set_font(get_data()->m_font);
                  brushText = get_data()->m_brushText;
               }
            }

         }

         if(get_element_rect(iVisiblePane, rectText, element_text))
         {

            pdc->SelectObject(brushText);

            pdc->_DrawText(tab_pane.get_title(), tab_pane.get_title().get_length(), rectText, DT_LEFT | DT_BOTTOM);

         }

         if(get_element_rect(iVisiblePane, rectClose, element_close_tab_button))
         {
            pdc->set_font(get_data()->m_fontBold);
            if(iVisiblePane == m_iHover && m_eelementHover == element_close_tab_button)
            {
               brushText = get_data()->m_brushCloseSel;
               brushText->create_solid(ARGB(0xff, 255, 127, 0));
            }
            else
            {
               brushText->create_solid(ARGB(0xff, 0, 0, 0));
            }
            pdc->SelectObject(brushText);
            pdc->draw_text("x", rectClose, DT_CENTER | DT_VCENTER);
         }


         iVisiblePane++;

      }
      //pdc->SelectObject(hOldPen);

   }


   void tab::get_title(int iPane,stringa & stra)
   {

      tab_pane & tab_pane = get_data()->m_panea(iPane);

      stra = tab_pane.m_straTitle;

   }

   void tab::GetTabClientRect(LPRECT lprect)
   {
      *lprect = get_data()->m_rectTabClient;
   }

   void tab::GetTabClientRect(RECT64 * lprect)
   {
      rect rectClient;
      GetClientRect(rectClient);
      rect64 rect64(rectClient);
      *lprect = rect64;
   }

   void tab::layout()
   {


      if(m_puserschema != NULL)
      {
         
         m_puserschema->_001OnTabLayout(this);
         
         return;

      }

      {

//         DWORD dwTime2 = ::get_tick_count();

         //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
         //TRACE("usertab::layout call time1= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
      }
      if(!get_data()->m_bCreated)
         return;

      defer_handle_full_screen_show_tabs(false);

      ::draw2d::memory_graphics pdc(allocer());
      pdc->SelectObject(get_data()->m_fontBold);

      m_dcextension.GetTextExtent(pdc,MAGIC_PALACE_TAB_SIZE,get_data()->m_sizeSep);



      if(get_data()->m_bVertical)
      {
         int32_t iTabWidth = 16;
         int32_t iTabHeight = 8;
         int32_t cx;
         int32_t cy;
         for(int32_t iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
         {

            tab_pane & tab_pane = get_data()->m_panea(iPane);

            if(!tab_pane.m_bVisible)
               continue;


            string str = tab_pane.get_title();

            tab_pane.do_split_layout(m_dcextension, pdc);

            ::size size;

            m_dcextension.GetTextExtent(pdc, str, size);



            if(tab_pane.m_dib.is_set())
            {
               size.cx += tab_pane.m_dib->m_size.cx+ 2;
               size.cy = MAX(size.cy, tab_pane.m_dib->m_size.cy);
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

         iTabWidth += get_data()->m_rectBorder.left + get_data()->m_rectBorder.right +
               get_data()->m_rectMargin.left + get_data()->m_rectMargin.right +
               get_data()->m_rectTextMargin.left + get_data()->m_rectTextMargin.right;

         get_data()->m_iTabWidth = iTabWidth;

         iTabHeight += get_data()->m_rectBorder.top + get_data()->m_rectBorder.bottom +
               get_data()->m_rectMargin.top + get_data()->m_rectMargin.bottom +
               get_data()->m_rectTextMargin.top + get_data()->m_rectTextMargin.bottom;

         get_data()->m_iTabHeight = iTabHeight;

         rect rectClient;
         GetClientRect(rectClient);

         get_data()->m_rectTab.left       = rectClient.left;
         get_data()->m_rectTab.top        = rectClient.top;
         get_data()->m_rectTab.right      = get_data()->m_rectTab.left + get_data()->m_iTabWidth;
         get_data()->m_rectTab.bottom     = rectClient.bottom;

   /*      m_pui->SetWindowPos(
            ZORDER_TOP,
            m_rectTab.left,
            m_rectTab.top,
            m_rectTab.width(),
            m_rectTab.height(),
            0);*/

         get_data()->m_rectTabClient.left       = m_bShowTabs ? get_data()->m_rectTab.right : rectClient.left;
         get_data()->m_rectTabClient.top        = get_data()->m_rectTab.top;
         get_data()->m_rectTabClient.right      = rectClient.right;
         get_data()->m_rectTabClient.bottom     = get_data()->m_rectTab.bottom;

      }
      else
      {
         int32_t iTabHeight = 16;
         int32_t cy;
         ::draw2d::graphics_sp graphics(allocer());
         graphics->CreateCompatibleDC(NULL);
         ::draw2d::graphics * pdc = graphics;
         pdc->SelectObject(get_data()->m_fontBold);

         rect rectClient;
         GetClientRect(rectClient);
         int x = rectClient.left;

         int32_t ixAdd;
         for(int32_t iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
         {

            tab_pane & tab_pane = get_data()->m_panea(iPane);

            if(!tab_pane.m_bVisible)
               return;

            string str = tab_pane.get_title();

            tab_pane.do_split_layout(m_dcextension,pdc);

            size size;

            m_dcextension.GetTextExtent(pdc, str, size);

            if(tab_pane.m_dib.m_p != NULL)
            {
               size.cy = MAX(size.cy, tab_pane.m_dib->size().cy);
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

            if (tab_pane.m_dib.is_set())
            {
               //::image_list::info ii;
               ixAdd += tab_pane.m_dib->m_size.cx + 2;
            }

            if (!tab_pane.m_bPermanent)
            {
               ixAdd += 2 + 16 + 2;
            }




            tab_pane.m_size.cx = size.cx + ixAdd
                                     + get_data()->m_rectBorder.left + get_data()->m_rectBorder.right
                                     + get_data()->m_rectMargin.left + get_data()->m_rectMargin.right
                                     + get_data()->m_rectTextMargin.left + get_data()->m_rectTextMargin.right;

            x += tab_pane.m_size.cx;
         }

         // close tab button
         cy = 2 + 16 + 2;
         if(cy > iTabHeight)
         {
            iTabHeight = cy;
         }

         iTabHeight += get_data()->m_rectBorder.top + get_data()->m_rectBorder.bottom +
               get_data()->m_rectMargin.top + get_data()->m_rectMargin.bottom + get_data()->m_iHeightAddUp;

         get_data()->m_iTabHeight = iTabHeight;

         for (int32_t iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
         {

            tab_pane & tab_pane = get_data()->m_panea(iPane);

            tab_pane.m_size.cy = iTabHeight;

         }




         get_data()->m_rectTab.left       = rectClient.left;
         get_data()->m_rectTab.top        = rectClient.top;
         get_data()->m_rectTab.right      = rectClient.right;
         get_data()->m_rectTab.bottom     = get_data()->m_rectTab.top + get_data()->m_iTabHeight;

   /*      SetWindowPos(
            ZORDER_TOP,
            m_rectTab.left,
            m_rectTab.top,
            m_rectTab.width(),
            m_rectTab.height(),
            0);*/

         rect & rectTabClient = get_data()->m_rectTabClient;

         rectTabClient.left       = get_data()->m_rectTab.left;
         rectTabClient.top        = m_bShowTabs ? get_data()->m_rectTab.bottom : rectClient.top;
         rectTabClient.right      = get_data()->m_rectTab.right;
         rectTabClient.bottom     = rectClient.bottom;

         //TRACE0("rectTabClient");
      }

            {

//               DWORD dwTime2 = ::get_tick_count();

               //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
               //TRACE("usertab::layout call time2= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
      }


      for(int32_t iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
      {

         if(iPane != _001GetSel())
         {

            layout_pane(iPane);

         }

      }

            {

//               DWORD dwTime2 = ::get_tick_count();

               //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
               //TRACE("usertab::layout call time3= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
            }


      layout_pane(_001GetSel());

      RedrawWindow();

   }

   void tab::layout_pane(index iPane)
   {

      sp(place_holder) pholder = get_tab_holder(iPane);

      if(pholder != NULL)
      {

         rect rectChild;

         GetTabClientRect(rectChild);

         rect rectWindow;

         pholder->GetWindowRect(rectWindow);

         ScreenToClient(rectWindow);

         pholder->SetWindowPos(ZORDER_TOP, rectChild.left, rectChild.top, rectChild.width(), rectChild.height(),
            _001GetSel() == iPane ? (pholder->IsWindowVisible() ? 0 : SWP_SHOWWINDOW) : 0);

         //pholder->layout();

      }

   }

   void tab::_001OnLButtonDown(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);
      class point point = pmouse->m_pt;

      index iPane = hit_test(point, m_eelement);

      get_data()->m_bDrag = false;
      if(iPane >= 0)
      {
         index iSel = _001GetSel();
         if(m_eelement == element_close_tab_button)
         {
            get_data()->m_iDragTab = iPane;
            pmouse->m_bRet = true;
            pmouse->set_lresult(1);
         }
         else if(iPane != iSel)
         {
            get_data()->m_iDragTab = iPane;
            SetTimer(5432187, 840, NULL);
            pmouse->m_bRet = true;
            pmouse->set_lresult(1);
         }
      }
   }


   void tab::_001OnLButtonUp(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);
      class point point = pmouse->m_pt;

      e_element eelement;

      index iPane = hit_test(point, eelement);

      KillTimer(5432187);
      if(iPane >= 0 && get_data()->m_iDragTab == iPane && m_eelement == eelement)
      {
         if(eelement == element_close_tab_button)
         {
            _001OnTabClose(iPane);
         }
         else
         {
            _001OnTabClick(iPane);
         }
         pmouse->m_bRet = true;
         pmouse->set_lresult(1);
      }
      else
      {
         get_data()->m_iDragTab = -1;
      }
   }

   void tab::_001OnMouseMove(signal_details * pobj)
   {
//      SCAST_PTR(::message::mouse, pmouse, pobj);
//      class point point = pmouse->m_pt;
      if(get_data()->m_iDragTab >= 0)
      {
         if(get_data()->m_pcallback != NULL)
         {
            get_data()->m_pcallback->_001DropTargetWindowRelay(this);
         }
      }
/*      if(m_iHover < 0)
      {
         track_mouse_hover();
      }

      index iHover = hit_test(point, m_eelementHover);

      if(iHover != m_iHover)
      {
         m_iHover = iHover;
         RedrawWindow();
      }*/

   }


   void tab::_001OnMouseLeave(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      m_iHover = -1;
      //GetParent()->RedrawWindow();
      pbase->set_lresult(0);
      pobj->m_bRet = true;
   }

   bool tab::get_element_rect(::index iTabParam, LPRECT lprect, e_element eelement)
   {
      if(iTabParam < 0 || iTabParam >= GetTabCount())
         return false;
      if(eelement == element_border)
      {
         if(!get_element_rect(iTabParam, lprect, element_tab))
            return false;
         deflate(lprect, get_data()->m_rectMargin);
         return true;
      }
      if(eelement == element_client)
      {
         if(!get_element_rect(iTabParam, lprect, element_border))
            return false;
         deflate(lprect, get_data()->m_rectBorder);
         return true;
      }
      if(eelement == element_icon)
      {
         if(get_data()->m_panea[iTabParam]->m_dib.is_null())
            return false;
         if(!get_element_rect(iTabParam, lprect, element_client))
            return false;
         lprect->right = lprect->left + get_data()->m_panea[iTabParam]->m_dib->m_size.cx;
         lprect->bottom = lprect->top + get_data()->m_panea[iTabParam]->m_dib->m_size.cy;
         return true;
      }
      else if(eelement == element_text)
      {
         if(!get_element_rect(iTabParam, lprect, element_client))
            return false;
         if(get_data()->m_panea[iTabParam]->m_dib.is_set())
         {
            lprect->left += get_data()->m_panea[iTabParam]->m_dib->m_size.cx + 2;
         }
         if(!get_data()->m_panea[iTabParam]->m_bPermanent)
         {
            lprect->right -= 2 + 16 + 2;
         }
         deflate(lprect, get_data()->m_rectTextMargin);
         return true;
      }
      else if(eelement == element_close_tab_button)
      {
         if(get_data()->m_panea[iTabParam]->m_bPermanent)
            return false;
         if(get_data()->get_visible_tab_count() <= 1 && !get_data()->m_bEnableCloseAll)
            return false;
         if(!get_element_rect(iTabParam, lprect, element_client))
            return false;
         lprect->right  = lprect->right;
         lprect->left   = lprect->right - 20;
         lprect->top    = lprect->bottom - 20;
         return true;
      }

      if(eelement != element_tab)
         return false;

      if(get_data()->m_bVertical)
      {
         ASSERT(iTabParam >= 0);
         ASSERT(iTabParam < GetTabCount());
         ::draw2d::memory_graphics pdc(allocer());
         rect rect = get_data()->m_rectTab;
         rect.bottom = rect.top;

         int32_t iPreviousVisibleTabCount = 0;
         {
            index iPane = iTabParam;
            for(int32_t i = 0; iPane > 0 && i < get_data()->m_panea.get_count(); i++)
            {
               if(get_data()->m_panea[i]->m_bVisible)
               {
                 iPane--;
                  iPreviousVisibleTabCount++;
               }
            }
         }

         lprect->left   = rect.left;
         lprect->top    = rect.top + iPreviousVisibleTabCount * get_data()->m_iTabHeight;
         lprect->right  = rect.right;
         lprect->bottom = lprect->top + get_data()->m_iTabHeight;

      }
      else
      {
         tab_pane & tab_pane = get_data()->m_panea(iTabParam);
         lprect->left = tab_pane.m_pt.x;
         lprect->top = tab_pane.m_pt.y;
         lprect->right = tab_pane.m_pt.x + tab_pane.m_size.cx;
         lprect->bottom = tab_pane.m_pt.y + tab_pane.m_size.cy;
         ///*ASSERT(iTabParam >= 0);
         //ASSERT(iTabParam < GetTabCount());
         ////::draw2d::graphics_sp graphics(allocer());
         ////graphics->CreateCompatibleDC(NULL);

         ////HDC hdc = ::CreateCompatibleDC(NULL);
         ////Gdiplus::Graphics * pg = new Gdiplus::Graphics(hdc);
         ////delete pg;
         ////::DeleteDC(hdc);
         //::draw2d::graphics * pdc = graphics;
         //rect rect = get_data()->m_rectTab;
         //rect.right = rect.left;
         //int32_t ixAdd;
         ////return false;
         //for(int32_t iPane = 0; iPane < iTabParam + 1; iPane++)
         //{

         //   rect.left = rect.right;

         //   tab_pane & tab_pane = get_data()->m_panea(iPane);

         //   if(!tab_pane.m_bVisible)
         //      continue;

         //   //string str = tab_pane.get_title();

         //   //size size;

         //   ixAdd = 5;

         //   if(tab_pane.m_dib.is_set())
         //   {
         //      //::image_list::info ii;
         //      ixAdd += tab_pane.m_dib->m_size.cx + 2;
         //   }

         //   if(!tab_pane.m_bPermanent)
         //   {
         //      ixAdd += 2 + 16 + 2;
         //   }

         //   /*if(get_data()->m_iaSel.contains(iPane))
         //   {
         //      pdc->SelectObject(get_data()->m_fontBold);
         //   }
         //   else
         //   {
         //      pdc->SelectObject(get_data()->m_font);
         //   }
         //   m_dcextension.GetTextExtent(
         //      pdc,
         //      str,
         //      size);*/
         //   rect.right = rect.left + ixAdd + size.cx +
         //      get_data()->m_rectBorder.left + get_data()->m_rectBorder.right +
         //      get_data()->m_rectMargin.left + get_data()->m_rectMargin.right +
         //      get_data()->m_rectTextMargin.left + get_data()->m_rectTextMargin.right;

         //}*/
         //*lprect = rect;
      }
      return true;
   }

   int_ptr tab::GetTabCount()
   {
      return get_data()->m_panea.get_size();
   }

   index tab::hit_test(point pt, e_element & eelement)
   {
      ScreenToClient(&pt);
      rect rect;
      for(int32_t iPane = 0; iPane < get_data()->m_panea.get_size(); iPane++)
      {
         tab_pane & p = *get_data()->m_panea[iPane];

         if(p.m_straTitle.get_size() > 1)
         {

            ::rect rectText;

            if(get_element_rect(iPane,rectText,element_text) && rectText.contains(pt))
            {
               for(int iTitle = 0; iTitle < p.m_straTitle.get_size(); iTitle++)
               {
                  rectText.left += p.m_sizeaText[iTitle].cx;
                  rectText.right = rectText.left + get_data()->m_sizeSep.cx;
                  if(rectText.contains(pt))
                  {
                     eelement = (e_element)((int)element_split + iTitle);
                     return iPane;
                  }
                  rectText.left += get_data()->m_sizeSep.cx;
               }
            }

         }

         if(get_element_rect(iPane, rect, element_close_tab_button) && rect.contains(pt))
         {
            eelement = element_close_tab_button;
            return iPane;
         }
         if(get_element_rect(iPane, rect, element_tab) && rect.contains(pt))
         {
            eelement = element_tab;
            return iPane;
         }
      }
      return -1;
   }

   ::window_sp tab::GetNotifyWnd()
   {
#ifdef METROWIN
      return NULL;

#else
      ::window_sp pwnd;
   //   if((pwnd = m_pui->GetOwner()) != NULL)
     //    return pwnd;
      if((pwnd = GetParent()->get_wnd()) != NULL)
         return pwnd;
      return NULL;
#endif
   }

   /*
   bool tab::create(::user::interaction * pinterface, UINT uiId)
   {
      if(!m_pui->create(
         pinterface,
         uiId))
         return false;
      m_bCreated = true;
      layout();
      return true;
   }
   */

   void tab::_001OnCreate(signal_details * pobj)
   {

      SCAST_PTR(::message::base, pbase, pobj);

      if(pobj->previous())
         return;

      m_puserschema = Session.m_puserschema;
   //  m_pimagelist = new image_list(get_app());

      get_data()->m_bCreated = true;

      pbase->set_lresult(0);

      //post_message(WM_USER + 1342);

      restore_tabs();

   }


   void tab::restore_tabs()
   {

      keep < bool > keepRestoringTabs(&m_bRestoringTabs, true, false, true);

      if(get_data()->m_matchanyRestore.get_count() > 0)
      {

         var_array vara;

         if(data_load("restore_tab",vara))
         {

            open_tabs(vara);

         }

      }

   }

   void tab::install_message_handling(::message::dispatch *pinterface)
   {
      ::user::control::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN , pinterface, this, &tab::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP   , pinterface, this, &tab::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_MOUSEMOVE   , pinterface, this, &tab::_001OnMouseMove);
      IGUI_WIN_MSG_LINK(WM_MOUSELEAVE  , pinterface, this, &tab::_001OnMouseLeave);
      IGUI_WIN_MSG_LINK(WM_CREATE      , pinterface, this, &tab::_001OnCreate);
      IGUI_WIN_MSG_LINK(::base::application::APPM_LANGUAGE, pinterface, this, &tab::_001OnAppLanguage);
      IGUI_WIN_MSG_LINK(message_start_tab_drag,pinterface,this,&tab::_001OnStartTabDrag);
      ////IGUI_WIN_MSG_LINK(WM_TIMER, pinterface, this, &tab::_001OnTimer);
   }

   void tab::_001SetSel(::index iSel)
   {

      {

         synch_lock lock(get_data()->m_pmutex);

         get_data()->m_iaSel.remove_all();

         get_data()->m_iaSel.add(iSel);

      }

      on_change_pane_count();

      on_show_view();

   }


   void tab::_001AddSel(::index iSel)
   {
      
      synch_lock lock(get_data()->m_pmutex);
      
      get_data()->m_iaSel.add(iSel);
      
      on_change_pane_count();
      
   }


   tab_pane::tab_pane(::aura::application * papp) :
      object(papp),
      m_brushFill(allocer()),
      m_brushFillSel(allocer()),
      m_brushFillHover(allocer()),
      m_istrTitleEx(papp)
   {
      m_bVisible     = true;
      m_bPermanent   = false;
      m_pholder      = NULL;
   }

   tab_pane::tab_pane(const tab_pane & tab_pane) :
      m_istrTitleEx(tab_pane.get_app())
   {
      operator = (tab_pane);
   }

   tab_pane::~tab_pane()
   {
   }

   tab_pane & tab_pane::operator = (const tab_pane & tab_pane)
   {

      if(this != &tab_pane)
      {
         m_id              = tab_pane.m_id;
         m_istrTitleEx     = tab_pane.m_istrTitleEx;
         m_dib             = tab_pane.m_dib;
         m_pholder         = tab_pane.m_pholder;
         m_bVisible        = tab_pane.m_bVisible;
         m_bPermanent      = tab_pane.m_bPermanent;
      }

      return *this;

   }

   string tab_pane::get_title()
   {

      return m_istrTitleEx;

   }

   void tab_pane::do_split_layout(::visual::graphics_extension & dc, ::draw2d::graphics * pdc)
   {

      stringa & straTitle = m_straTitle;

      straTitle.explode(MAGIC_PALACE_TAB_SPLT,get_title());

      m_sizeaText.set_size(straTitle.get_count());

      for(int iTitle = 0; iTitle < straTitle.get_count(); iTitle++)
      {
         
         dc.GetTextExtent(pdc,straTitle[iTitle],m_sizeaText[iTitle]);

      }

   }




   tab_pane_array::tab_pane_array(::aura::application * papp) :
      object(papp)
   {
   }

   tab_pane_array::~tab_pane_array()
   {
   }



   tab_pane * tab_pane_array::get_by_id(id id)
   {
      for(int32_t i = 0; i < this->get_count(); i++)
      {
         if(this->element_at(i)->m_id == id)
         {
            return element_at(i);
         }
      }
      return NULL;
   }

   ::count tab_pane_array::get_visible_count()
   {

      ::count count = 0;

      for(int32_t i = 0; i < this->get_count(); i++)
      {
         if(this->element_at(i)->m_bVisible)
         {
            count++;
         }
      }

      return count;
   }

   void tab::_001OnAppLanguage(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      layout();
   }



   void tab::_001SetTabCallback(tab_callback * pcallback)
   {
      if(get_data()->m_pcallback != NULL)
      {
         delete get_data()->m_pcallback;
      }
      get_data()->m_pcallback = pcallback;;

   }

   ::index tab::_001GetSel()
   {
      if(get_data()->m_iaSel.get_size() == 1)
      {
         return get_data()->m_iaSel[0];
      }
      else
      {
         return -1;
      }
   }

   ::count tab::_001GetPaneCount()
   {
      return get_data()->m_panea.get_count();
   }


   bool tab::_001IsAddTab(::index iPane)
   {
      if(get_data()->m_pcallback != NULL)
      {
         return get_data()->m_pcallback->_001IsAddTab(this, iPane);
      }
      else
      {
         return false;
      }
   }

   void tab::_001OnTabClick(::index iPane)
   {
      if(get_data()->m_pcallback != NULL)
      {
         get_data()->m_pcallback->_001OnTabClick(this, iPane);
      }
      else
      {
         _001SelectTab(iPane);
      }
   }

   void tab::_001OnTabClose(::index iPane)
   {
      if(get_data()->m_pcallback != NULL)
      {
         get_data()->m_pcallback->_001OnTabClose(this, iPane);
      }
      else
      {
         _001CloseTab(iPane);
      }
   }

   bool tab::show_tab_by_id(id id, bool bShow)
   {
      tab_pane * ppane = get_pane_by_id(id);
      if(ppane == NULL)
         return false;

      ppane->m_bVisible = bShow;

      layout();
      return true;
   }

   bool tab::show_tab(::index iTab, bool bShow)
   {
      tab_pane * ppane = get_pane(iTab, !bShow);
      if(ppane == NULL)
         return !bShow;

      ppane->m_bVisible = bShow;

      layout();

      return bShow;
   }


   ::user::interaction * tab::get_tab_window(::index iPane, bool bVisible)
   {
      sp(place_holder) pholder = get_tab_holder(iPane, bVisible);
      if(pholder == NULL)
         return NULL;
      return pholder->get_hold();
   }

   ::user::place_holder * tab::get_tab_holder(::index iPane, bool bVisible)
   {
      tab_pane * ppane = get_pane(iPane, bVisible);
      if(ppane == NULL)
         return NULL;
      return ppane->m_pholder;
   }

   tab_pane * tab::get_pane(::index iPane, bool bVisible)
   {
      if(iPane < 0 || iPane >= get_data()->m_panea.get_count())
         return NULL;
      if(bVisible)
      {
         for(int32_t i = 0; iPane >= 0 && i < get_data()->m_panea.get_count(); i++)
         {
            if(get_data()->m_panea[i]->m_bVisible)
            {
               if(iPane <= 0)
               {
                  return get_data()->m_panea.element_at(i);
               }
               else
               {
                  iPane--;
               }
            }
         }
         return NULL;
      }
      else
      {
         return get_data()->m_panea.element_at(iPane);
      }
   }


   void tab::_001SelectTab(::index iPane)
   {
//      if(get_tab_holder(_001GetSel()) != NULL)
  //    {
    //     get_tab_holder(_001GetSel())->ShowWindow(SW_HIDE);
     // }
      _001SetSel(iPane);
     // if(get_tab_holder(_001GetSel()) != NULL)
      //{
        // get_tab_holder(_001GetSel())->ShowWindow(SW_SHOW);
     // }
      layout();
   }

   void tab::on_show_view()
   {
      if(get_data()->m_pcallback != NULL && dynamic_cast < ::user::tab * > (get_data()->m_pcallback) != this)
      {
         get_data()->m_pcallback->_001OnShowTab(this);
      }
   }

   void tab:: _001SetVertical(bool bVertical)
   {
      get_data()->m_bVertical = bVertical;

      if(get_data()->m_bVertical)
      {
         get_data()->m_rectMargin.set(2, 3, 1, 0);
         get_data()->m_rectBorder.set(11, 1, 6, 0);
         get_data()->m_rectTextMargin.set(3, 0, 1, 0);

         get_data()->m_iTabHeight += get_data()->m_rectBorder.top + get_data()->m_rectBorder.bottom +
               get_data()->m_rectMargin.top + get_data()->m_rectMargin.bottom;

         get_data()->m_iTabWidth += get_data()->m_rectBorder.left + get_data()->m_rectBorder.right +
               get_data()->m_rectMargin.left + get_data()->m_rectMargin.right;
      }
      else
      {
         get_data()->m_rectMargin.set(3, 2, 0, 1);
         get_data()->m_rectBorder.set(0, 1, 11, 6);
         get_data()->m_rectTextMargin.set(3, 0, 1, 0);

         get_data()->m_iTabHeight += get_data()->m_rectBorder.top + get_data()->m_rectBorder.bottom +
               get_data()->m_rectMargin.top + get_data()->m_rectMargin.bottom;

         get_data()->m_iTabWidth += get_data()->m_rectBorder.left + get_data()->m_rectBorder.right +
               get_data()->m_rectMargin.left + get_data()->m_rectMargin.right;
      }
   }

   void tab::_001ConnectParent(::message::dispatch * pinterface)
   {
      UNREFERENCED_PARAMETER(pinterface);
   }

   id tab::get_cur_tab_id()
   {
      return get_id_by_tab(_001GetSel());
   }


   ::index tab::get_tab_by_id(id id)
   {
      for(::index i = 0; i < get_data()->m_panea.get_size(); i++)
      {
         if(get_data()->m_panea[i]->m_id == id)
            return i;
      }
      return -1;
   }


   id tab::get_id_by_tab(::index iPane, bool bVisible)
   {
      if(bVisible)
      {
         for(int32_t i = 0; iPane >= 0 && i < get_data()->m_panea.get_count(); i++)
         {
            if(get_data()->m_panea[i]->m_bVisible)
            {
               if(iPane <= 0)
               {
                  return get_data()->m_panea[i]->m_id;
               }
               else
               {
                  iPane--;
               }
            }
         }
         return id();
      }
      else
      {
         if(iPane >= 0 && iPane < get_data()->m_panea.get_size())
            return get_data()->m_panea[iPane]->m_id;
         else
            return id();
      }

   }


   bool tab::set_cur_tab_by_id(id id, sp(::create) pcreatecontext)
   {

      try
      {

         m_spcreatecontext = pcreatecontext;

         ::index iPane = get_tab_by_id(id);

         if(iPane == -1)
         {

            add_tab("", id);

            iPane = get_tab_by_id(id);

            if(iPane == -1)
               return false;

         }

         _001SetSel(iPane);

      }
      catch(::exit_exception & e)
      {

         throw e;

      }
      catch(::exception::exception &)
      {

         return false;

      }
      catch(...)
      {

         return false;

      }

      m_spcreatecontext = (sp(::create)) NULL;

      return true;

   }



   e_position tab::DragHitTest(point pt)
   {
      rect rect;
      GetDragRect(rect, position_top);
      if(rect.contains(pt))
      {
         return position_top;
      }
      GetDragRect(rect, position_bottom);
      if(rect.contains(pt))
      {
         return position_bottom;
      }
      GetDragRect(rect, position_left);
      if(rect.contains(pt))
      {
         return position_left;
      }
      GetDragRect(rect, position_right);
      if(rect.contains(pt))
      {
         return position_right;
      }
      return position_none;
   }

   void tab::GetDragRect(LPRECT lprect, e_position eposition)
   {
      switch(eposition)
      {
      case position_top:
         {
            lprect->top = get_data()->m_rectTabClient.top;
            lprect->left = get_data()->m_rectTabClient.left + get_data()->m_rectTabClient.width() / 3;
            lprect->right = lprect->left + get_data()->m_rectTabClient.width() / 3;
            lprect->bottom = lprect->top + get_data()->m_rectTabClient.height() / 3;
         }
         break;
      case position_left:
         {
            lprect->top = get_data()->m_rectTabClient.top + get_data()->m_rectTabClient.height() / 3;
            lprect->left = get_data()->m_rectTabClient.left;
            lprect->right = lprect->left + get_data()->m_rectTabClient.width() / 3;
            lprect->bottom = lprect->top + get_data()->m_rectTabClient.height() / 3;
         }
         break;
      case position_right:
         {
            lprect->top = get_data()->m_rectTabClient.top + get_data()->m_rectTabClient.height() / 3;
            lprect->left = get_data()->m_rectTabClient.left + get_data()->m_rectTabClient.width() * 2 / 3;
            lprect->right = lprect->left + get_data()->m_rectTabClient.width() / 3;
            lprect->bottom = lprect->top + get_data()->m_rectTabClient.height() / 3;
         }
         break;
      case position_bottom:
         {
            lprect->top = get_data()->m_rectTabClient.top + get_data()->m_rectTabClient.height() * 2 / 3;
            lprect->left = get_data()->m_rectTabClient.left + get_data()->m_rectTabClient.width() / 3;
            lprect->right = lprect->left + get_data()->m_rectTabClient.width() / 3;
            lprect->bottom = lprect->top + get_data()->m_rectTabClient.height() / 3;
         }
         break;
         default:
            break;
      }
   }



   void tab::_001OnDropTab(::index iPane, e_position eposition)
   {
      if(get_data()->m_pcallback != NULL)
      {
         get_data()->m_pcallback->_001OnDropTab(iPane, eposition);
      }
   }


   ::user::interaction * tab::get_view_uie()
   {
      return NULL;
   }


   id tab::get_current_id()
   {
      return get_cur_tab_id();
   }


   void tab::_001OnTimer(::timer * ptimer)
   {

      control::_001OnTimer(ptimer);;

      if(ptimer->m_nIDEvent == 5432187)
      {
         get_data()->m_bDrag = true;
         KillTimer(5432187);
         post_message(message_start_tab_drag);

      }

   }
   
   
   void tab::_001OnStartTabDrag(::signal_details * pobj)
   {

      if(get_data()->m_pcallback != NULL)
      {

         get_data()->m_pcallback->_001DropTargetWindowInitialize(this);

      }

   }

   void tab::_000OnMouse(::message::mouse * pmouse)
   {
      if(m_bShowTabs)
      {
         // these try catchs are needed for multi threading supporting: multi threaded windows: an endeavour
         // Now I understand why Microsoft (TM) Windows (R) windows are single threaded.
         // to debug, enable catch exceptions in debugger
         try
         {
            (m_pimpl->*m_pimpl->m_pfnDispatchWindowProc)(dynamic_cast < signal_details * > (pmouse));
            if(pmouse->get_lresult() != 0)
               return;
         }
         catch(...)
         {
         }
      }
      else if(pmouse->m_uiMessage == WM_MOUSEMOVE)
      {
      }
      sp(::user::interaction) pui = top_child();
//      int32_t iSize;
      try
      {
         while(pui != NULL)
         {
            try
            {
               if(pui->IsWindowVisible() && pui->_001IsPointInside(pmouse->m_pt))
               {
                  try
                  {
                     pui->_000OnMouse(pmouse);
                     if(pmouse->m_bRet)
                        return;
                  }
                  catch(...)
                  {
                  }
               }
               pui = pui->under_sibling();
            }
            catch(...)
            {
            }
         }
      }
      catch(...)
      {
      }
   }

   tab::data * tab::get_data()
   {
      return m_spdata;
   }

   tab_pane * tab::get_pane_by_id(id id)
   {
      return get_data()->get_pane_by_id(id);
   }

   tab_pane * tab::ensure_pane_by_id(id id)
   {
      ensure_tab_by_id(id);
      return get_data()->get_pane_by_id(id);
   }

   void tab::ensure_tab_by_id(id id)
   {
      UNREFERENCED_PARAMETER(id);
      ::exception::throw_not_implemented(get_app());
   }

   void tab::on_change_pane_count()
   {
      if(m_bRestoringTabs)
         return;

      if(get_data()->m_matchanyRestore.get_count() > 0)
      {
         var_array vara;
         get_restore_tab(vara);
         data_save("restore_tab", vara);
      }
   }

   void tab::get_text_id(stringa & stra)
   {
      get_presuffixed_ci_id(stra, NULL, NULL);
   }

   void tab::get_prefixed_ci_id(stringa & stra, const char * pszPrefix)
   {
      get_presuffixed_ci_id(stra, pszPrefix, NULL);
   }

   void tab::get_suffixed_ci_id(stringa & stra, const char * pszSuffix)
   {
      get_presuffixed_ci_id(stra, NULL, pszSuffix);
   }

   void tab::get_presuffixed_ci_id(stringa & stra, const char * pszPrefix, const char * pszSuffix)
   {
      string strPrefix(pszPrefix);
      string strSuffix(pszSuffix);
      string strPath;
      tab_pane_array & panea = get_data()->m_panea;
      for(int32_t i = 0; i < panea.get_count(); i++)
      {
         tab_pane & tab_pane = panea(i);
         strPath = tab_pane.m_id;
         if(strPrefix.is_empty() || ::str::begins_ci(strPath, strPrefix))
         {
            if(strSuffix.is_empty() || ::str::ends_ci(strPath, strSuffix))
            {
               stra.add(strPath);
            }
         }
      }
   }

   void tab::get_begins_ci_eat_id(stringa & stra, const char * pszPrefix)
   {
      get_begins_ends_ci_eat_id(stra, pszPrefix, NULL);
   }

   void tab::get_ends_ci_eat_id(stringa & stra, const char * pszSuffix)
   {
      get_begins_ends_ci_eat_id(stra, NULL, pszSuffix);
   }

   void tab::get_begins_ends_ci_eat_id(stringa & stra, const char * pszPrefix, const char * pszSuffix)
   {
      string strPrefix(pszPrefix);
      string strSuffix(pszSuffix);
      string strPath;
      tab_pane_array & panea = get_data()->m_panea;
      for(int32_t i = 0; i < panea.get_count(); i++)
      {
         tab_pane & tab_pane = panea(i);
         strPath = tab_pane.m_id;
         if(strPrefix.is_empty() || ::str::begins_ci(strPath, strPrefix))
         {
            if(strSuffix.is_empty() || ::str::ends_ci(strPath, strSuffix))
            {
               stra.add(strPath);
            }
         }
      }
   }

   void tab::get_restore_tab(var_array & vara)
   {
      ::core::match::any  & matchany = get_data()->m_matchanyRestore;
      if(matchany.get_count() == 0)
         return;
      var varId;
      tab_pane_array & panea = get_data()->m_panea;
      for(int32_t i = 0; i < panea.get_count(); i++)
      {
         varId = panea[i]->m_id;
         if(matchany.matches(varId))
         {
            vara.add(varId);
         }
      }
   }

   bool tab::has_restore_tab()
   {
      ::core::match::any  & matchany = get_data()->m_matchanyRestore;
      if(matchany.get_count() == 0)
         return false;
      var varId;
      tab_pane_array & panea = get_data()->m_panea;
      for(int32_t i = 0; i < panea.get_count(); i++)
      {
         varId = panea[i]->m_id;
         if(matchany.matches(varId))
         {
            return true;
         }
      }
      return false;
   }

   void tab::open_tabs(const var_array & vara)
   {
      stringa stra;
      for(int32_t i = 0; i < vara.get_count(); i++)
      {
         // ODOW : TODO : should create bergedgewrapper to open bergedge inside a window.
         if(vara[i].get_type() == var::type_string && vara[i].get_string() == "app:bergedge")
            continue;
         if(vara[i].get_type() == var::type_string && vara[i].get_string() == "app:")
            continue;
         //if(stra.add_unique(vara[i]) >= 0)
         //{
            //ensure_tab_by_id(stra.last());

         set_cur_tab_by_id(vara[i].get_id());
         //}
      }
   }

   void tab::_001CloseTab(::index iTab)
   {
      remove_tab_by_id(get_id_by_tab(iTab));
   }

} // namespace core


