//#include "framework.h"


namespace filehandler
{


   view::view(::aura::application * papp) :
      ::object(papp),
      html_form(papp),
      html_view(papp),
      m_document(papp),
      m_spfont(allocer())
   {

      m_spfont->create_point_font("Geneva",14.0);

   }


   void view::install_message_handling(::message::dispatch * pinterface)
   {

      ::user::form::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &view::_001OnCreate);
//      
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &view::_001OnLButtonUp);

   }


   void view::refresh()
   {
      
      m_plistWorking = canew(list(get_app()));
      
      m_plistWorking->parse(&System.filehandler(), ::file::path(m_strName).extension());
      
      m_plist = m_plistWorking;

      m_plistWorking = NULL;

      layout();

   }

   
   void view::_001OnTimer(::timer * ptimer)
   {

      html_view::_001OnTimer(ptimer);;

      if(ptimer->m_nIDEvent == 8888)
      {

         refresh();

      }

   }


   void view::_001OnCreate(signal_details * pobj)
   {

      if(pobj->previous())
         return;

      SetTimer(8888, 5000, NULL);

   }

   void view::item::parse(const char * pszApp)
   {
      m_strApp = pszApp;
   }

   void view::list::parse(handler * phandler, const char * pszTopic)
   {
      
      remove_all();

      stringa straApp;

      phandler->get_extension_app(straApp, pszTopic);

      item item(get_app());

      for(int32_t i = 0; i < straApp.get_count(); i++)
      {
         item.parse(straApp[i]);
         item.m_iIndex = i;
         add(canew(class item(item)));
      }

   }

   view::item::item(::aura::application * papp) :
      ::object(papp)
   {

   }

   void view::item::draw(sp(view) pview, ::draw2d::graphics * pdc, list * plist)
   {

      UNREFERENCED_PARAMETER(plist);

      COLORREF cr;
      sp(::aura::application) papp = pview->get_app();
      bool bHover = pview->m_iHover == m_iIndex;
      cr = bHover ? ARGB(255, 230, 255, 230) : ARGB(255, 200, 255, 200);
      if(!Sess(papp).savings().is_trying_to_save(::aura::resource_processing)
         && !Sess(papp).savings().is_trying_to_save(::aura::resource_display_bandwidth)
         && !Sess(papp).savings().is_trying_to_save(::aura::resource_memory))
      {
         class imaging & imaging = Sys(papp).visual().imaging();
         imaging.color_blend(pdc, m_rectItem, cr, 127);
      }
      else
      {
         pdc->FillSolidRect(m_rectItem, cr);
      }
      cr = bHover ? ARGB(255, 150, 255, 150) : ARGB(255, 50, 255, 50);
      pdc->FillSolidRect(m_rectStatusImage, cr);
      COLORREF cr1;
      COLORREF cr2;
      if(bHover)
      {
         cr = ARGB(255, 0, 100, 0);
         cr1 = ARGB(255, 120, 255, 150);
         cr2 = ARGB(255, 23, 200, 33);
      }
      else
      {
         cr = ARGB(255, 40, 40, 40);
         cr1 = ARGB(255, 100, 100, 100);
         cr2 = ARGB(255, 10, 10, 10);
      }
      pdc->Draw3dRect(m_rectItem, cr1, cr2);
      cr |= 0xff000000;
      ::draw2d::brush_sp brushText(allocer());
      brushText->create_solid(cr);
      //pdc->set_color(cr);
      pdc->draw_text(m_strApp, m_rectName, DT_LEFT | DT_BOTTOM);
   }


   view::list::list(::aura::application * papp) :
      ::object(papp)
   {
      m_iItemHeight = 30;
   }

   void view::list::layout(LPCRECT lpcrect)
   {
      int32_t top = lpcrect->top;
      for(int32_t i = 0; i < get_count(); i++)
      {
         item & item = operator()(i);
         item.m_rectItem.left = lpcrect->left;
         item.m_rectItem.right = lpcrect->right;
         item.m_rectItem.top = top;
         item.m_rectItem.bottom = item.m_rectItem.top + m_iItemHeight;
         top = item.m_rectItem.bottom + 2;
         item.m_rectStatusImage = item.m_rectItem;
         item.m_rectStatusImage.right = item.m_rectStatusImage.left + m_iItemHeight;
         item.m_rectStatusImage.deflate(2, 2);
         item.m_rectName = item.m_rectItem;
         item.m_rectName.left = item.m_rectStatusImage.right;
         item.m_rectName.deflate(2, 2);
         
      }
   }

   void view::list::draw(sp(view) pview, ::draw2d::graphics * pdc)
   {
      for(int32_t i = 0; i < get_count(); i++)
      {
         element_at(i)->draw(pview, pdc, this);
      }
   }


   void view::layout()
   {

      if (m_plist.is_null())
         return;

      layout_list(m_plist);

      
   }

   void view::layout_list(list * plist)
   {
      
      ::draw2d::memory_graphics pdc(allocer());

      rect rectClient;

      GetClientRect(rectClient);

      plist->layout(rectClient);

   }


   void view::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      if (m_plist.is_null())
         return;

      select_font(pgraphics);

      m_plist->draw(this,pgraphics);


   }



   void view::_001OnLButtonUp(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      e_element eelement;

      index iItem = hit_test(pmouse->m_pt, eelement);

      if(iItem >= 0)
      {

         /*
         var varRequest;

         varRequest = "app://" + m_list[iItem].m_strApp + "/" + m_strName;

         Session.request(varRequest);
         */

         sp(::create) createcontext(allocer());

         createcontext->m_spCommandLine->m_strApp = m_plist->operator [](iItem)->m_strApp;

         createcontext->m_spCommandLine->m_varFile                = m_strName;

         Session.request_create(createcontext);

         //varFile = createcontext->m_spCommandLine->m_varFile;

      }

   }

   index view::hit_test(point pt, e_element & eelement)
   {


      if (m_plist.is_null())
         return -1;

      ScreenToClient(&pt);
      for(int32_t i = 0; i < m_plist->get_count(); i++)
      {
         if(m_plist->element_at(i)->m_rectName.contains(pt))
         {
            eelement = element_text;
            return i;
         }
         if (m_plist->element_at(i)->m_rectStatusImage.contains(pt))
         {
            eelement = element_status_image;
            return i;
         }
         if (m_plist->element_at(i)->m_rectItem.contains(pt))
         {
            eelement = element_area;
            return i;
         }

      }

      eelement = element_none;

      return -1;

   }


   bool view::get_font(::draw2d::font_sp & font)
   {
      
      font = m_spfont;

      return true;

   }

} // namespace filehandler




