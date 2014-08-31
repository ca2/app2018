#include "framework.h"


namespace simple_ui
{


   tap::tap(sp(::aura::application) papp) :
      element(papp)
   {

      m_bAutoStockIconPenWidth   = true;
      m_dStockIconPenWidth       = 2.0;
      m_estockicon               = stock_icon_none;

      m_id                       = "submit";
      m_bDown                    = false;
      m_bMouseMove               = false;

   }


   tap::~tap()
   {
   }


   IMPL_IMH(tap,::simple_ui::interaction)
      MSG_CREATE
      MSG_KEYDOWN
      MSG_LBUTTONDOWN
      MSG_LBUTTONUP
      MSG_MOUSEMOVE
   END_IMH


   void tap::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      //draw_simple(hdc);

      simple_ui_draw_volume(pgraphics);

   }



   void tap::_001OnLButtonDown(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse,pmouse,pobj);
      
      pmouse->m_bRet = true;
      
      if(keyboard_focus_is_focusable())
      {

         keyboard_set_focus();
         
      }
      else
      {
         
         GetWindow()->show_keyboard(false);
         
      }

      m_bDown = true;

   }

   void tap::_001OnLButtonUp(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse,pmouse,pobj);

      pmouse->m_bRet = true;

      if (m_bDown)
      {

         m_bDown = false;

         on_action(m_id);

      }

   }

   void tap::_001OnMouseMove(signal_details * pobj)
   {

      SCAST_PTR(::message::mouse,pmouse,pobj);

      pmouse->m_bRet = true;
      m_bMouseMove = true;

      //m_pplugin->redraw();


   }

   
   bool tap::keyboard_focus_is_focusable()
   {
      
#ifdef APPLE_IOS
      
      return false;
      
#else
      
      return true;
      
#endif
      
   }
   

   bool tap::is_hover()
   {

      rect rectWindow;

      GetWindowRect(&rectWindow);

      POINT ptCursor;

      Session.get_cursor_pos(&ptCursor);

      bool bHover = rectWindow.contains(ptCursor);

      if (!bHover)
         m_bMouseMove = false;

      return bHover;


   }



   void tap::simple_ui_draw_simple(::draw2d::graphics * pgraphics)
   {

      {

         pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

#if CA2_PLATFORM_VERSION == CA2_BASIS

         ::draw2d::brush_sp br(allocer(), ARGB(184, 255, 184, 240));

#else

         ::draw2d::brush_sp br(allocer(), ARGB(184, 184, 255, 177));

#endif

         rect rectClient;

         GetClientRect(rectClient);

         pgraphics->FillRect(rectClient, br);

      }

     simple_ui_draw_text(pgraphics);

   }


   void tap::simple_ui_draw_volume(::draw2d::graphics * pgraphics)
   {

      {



         pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

         COLORREF crOut;

         COLORREF crIn;

         COLORREF crBorderOut;

         COLORREF crBorderIn;

         rect rectClient;

         GetClientRect(rectClient);


         if (is_hover() || m_bDown || m_bMouseMove)
         {

#if CA2_PLATFORM_VERSION == CA2_BASIS

            crOut = ARGB(184 + 49, 255, 230, 255);

            crIn = ARGB(255, 255, 133, 255);

            crBorderOut = ARGB(184, 150, 100, 150);

            crBorderIn = ARGB(184, 255, 240, 255);

#else

            crOut = ARGB(184 + 49, 230, 255, 225);

            crIn = ARGB(255, 133, 255, 77 + 49);

            crBorderOut = ARGB(184, 100, 150, 100);

            crBorderIn = ARGB(184, 240, 255, 235);

#endif

         }
         else
         {

#if CA2_PLATFORM_VERSION == CA2_BASIS

            crOut = ARGB(184, 255, 210, 255);

            crIn = ARGB(255, 255, 184 + 49, 255);

            crBorderOut = ARGB(184, 90, 20, 90);

            crBorderIn = ARGB(184, 255, 240, 255);

#else

            crOut = ARGB(184, 210, 255, 205);

            crIn = ARGB(255, 133, 255, 77 + 49);

            crBorderOut = ARGB(184, 20, 90, 20);

            crBorderIn = ARGB(184, 240, 255, 235);

#endif

         }

         if(m_estockicon != stock_icon_none)
         {
            
            ::draw2d::brush_sp br(allocer());

            br->create_solid(crBorderIn);

            pgraphics->SelectObject(br);

            pgraphics->FillRect(rectClient,br);

            ::draw2d::pen_sp pen(allocer());

            pen->create_solid(m_dStockIconPenWidth,crBorderOut);

            pgraphics->SelectObject(pen);

            pgraphics->draw_stock_icon(rectClient,m_estockicon);

            return;

         }

         int32_t iBorderH = height(&rectClient) / 2;

         ::draw2d::brush_sp br(allocer());

         br->CreateLinearGradientBrush(point(rectClient.left, rectClient.top - 1), point(rectClient.left, rectClient.top + iBorderH + 2), crOut, crIn);

         pgraphics->FillRect(rect(rectClient.left + 1, rectClient.top + 1, (int32_t)width(&rectClient), iBorderH), br);

         br->CreateLinearGradientBrush(point(rectClient.left, rectClient.top + iBorderH - 1), point(rectClient.left, rectClient.top + iBorderH * 2 + 2), crIn, crOut);

         pgraphics->FillRect(rect(rectClient.left + 1, rectClient.top + iBorderH, rectClient.left + (int32_t)width(&rectClient), rectClient.top + iBorderH + iBorderH), br);

         /*Gdiplus::Pen pen1(crBorderOut);

         graphics2.DrawRectangle(&pen1, rectClient.left, rectClient.top, width(&rectClient), iBorderH * 2);*/

         ::draw2d::pen_sp pen(pgraphics, 1.0, crBorderIn);

         pgraphics->DrawRect(rect(rectClient.left + 1, rectClient.top + 1, rectClient.left + (int32_t)width(&rectClient) - 2, rectClient.top + iBorderH * 2 - 2), pen);

      }


      if(m_estockicon == stock_icon_none)
      {

         simple_ui_draw_text(pgraphics);

      }

   }


   void tap::simple_ui_draw_text(::draw2d::graphics * pgraphics)
   {

      rect rectClient;

      GetClientRect(rectClient);

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

#if CA2_PLATFORM_VERSION == CA2_BASIS

      ::draw2d::brush_sp b(allocer(), ARGB(223, 84, 49, 77));

#else

      ::draw2d::brush_sp b(allocer(), ARGB(223, 49, 84, 23));

#endif

      pgraphics->SelectObject(b);

      //float fMargin = (height(&rectClient) * ((1.0f - 0.7f) / 2.0f));

      float fMargin = (height(&rectClient) * ((1.0f - 0.84f) / 2.0f));

      rectClient.deflate((int32_t) fMargin, (int32_t) fMargin);

      ::draw2d::font_sp f(allocer());

      f->create_pixel_font("Geneva", (int32_t)height(rectClient)* 0.7);

      pgraphics->SelectObject(f);

      pgraphics->draw_text(get_window_text(), rectClient, DT_LEFT | DT_BOTTOM);

   }


   void tap::_001OnKeyDown(signal_details * pobj)
   {

      SCAST_PTR(::message::key,pkey,pobj);

      ::user::e_key iKey = pkey->m_ekey;

      if (iKey == ::user::key_return || iKey == ::user::key_space)
      {

         on_action(m_id);

         pkey->m_bRet = true;

      }
      else if (iKey == ::user::key_tab)
      {

         sp(::user::interaction) pui = keyboard_get_next_focusable();

         if(pui.is_set())
            pui->keyboard_set_focus();

         pkey->m_bRet = true;

      }
      else
      {

         pkey->m_bRet = false;

      }

   }


   void tap::layout()
   {

      rect rectClient;

      GetClientRect(rectClient);

      if(m_bAutoStockIconPenWidth)
      {

         m_dStockIconPenWidth = rectClient.minimum_natural_dimension() / 12.5;

      }

   }


} // namespace simple_ui



