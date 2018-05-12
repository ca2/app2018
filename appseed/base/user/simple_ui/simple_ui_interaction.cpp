﻿#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"



namespace simple_ui
{


   byte brate(double dRate, double dMin, double dMax)
   {

      return (byte)(dRate * (dMax - dMin) + dMin);

   }


   interaction::interaction():
      ::object(get_app()),
      ::user::interaction(get_app())

   {

      m_puserstyle       = NULL;

   }

   interaction::~interaction()
   {
   }


   void interaction::install_message_routing(::message::sender * psender)
   {

      ::user::interaction::install_message_routing(psender);

   }


   bool interaction::on_action(const char * pszId)
   {

      ::simple_ui::interaction * pui = GetTypedParent < ::simple_ui::interaction >();

      if (pui != NULL)
      {

         if(pui->on_action(pszId))
            return true;

      }

      return false;

   }



   bool interaction::keyboard_focus_is_focusable()
   {

      return false;

   }


//   ::user::style * interaction::get_user_style()
//   {
//
//      if (m_puserstyle != NULL)
//         return m_puserstyle;
//
//      if (GetTypedParent < ::simple_ui::interaction >() == NULL)
//         return NULL;
//
//      return GetTypedParent < ::simple_ui::interaction >()->get_style();
//
//   }


   void interaction::simple_ui_draw_back_01_old(rect m_rect, ::draw2d::graphics * pgraphics)
   {

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      COLORREF crOut;

      COLORREF crIn;

      /*   if (eschema == ::hotplugin::plugin::schema_darker)
         {

         crOut = ARGB(0, 84, 90, 80);

         crIn = ARGB(133, 84, 90, 80);

         }
         else*/
      {

         crOut = ARGB(0, 255, 255, 233);

         crIn = ARGB(133, 255, 255, 233);

      }

      int32_t iBorderH = MIN(m_rect.height() / 2, 49);

      ::draw2d::brush_sp br(allocer());

      br->CreateLinearGradientBrush(m_rect.top_left(), point(m_rect.left, m_rect.top + iBorderH), crOut, crIn);

      pgraphics->fill_rect(rect(m_rect.left, m_rect.top, (int32_t)m_rect.width(), iBorderH), br);

      br->create_solid(crIn);

      pgraphics->fill_rect(rect(m_rect.left, m_rect.top + iBorderH, (int32_t)m_rect.width(), (int32_t)m_rect.height() - (iBorderH * 2)), br);

      br->CreateLinearGradientBrush(point(m_rect.left, m_rect.bottom - iBorderH), m_rect.bottom_left(), crIn, crOut);

      pgraphics->fill_rect(rect(m_rect.left, m_rect.bottom - iBorderH, (int32_t)m_rect.width(), iBorderH), br);

   }

   void interaction::simple_ui_draw_back_01_new(rect rect, ::draw2d::graphics * pgraphics)
   {

      /*simple_solid_brush br(g, ARGB(255, 255, 255, 255));

      simple_pen pen;

      pen.create_solid(ARGB(255, 0, 0, 0));

      pgraphics->select(br);

      pgraphics->select(pen);

      m_rect.deflate(0, 0, 1, 1);

      pgraphics->rectangle(m_rect);*/

      //pgraphics->set_alpha_mode(::draw2d::alpha_mode_set);

      //pgraphics->fill_solid_rect(rect,ARGB(0,0,0,0));

   }


   void interaction::_001OnNcDraw(::draw2d::graphics * pgraphics)
   {

      ::user::interaction::_001OnNcDraw(pgraphics);

      if(m_flagNonClient.is_signalized(non_client_focus_rect) && keyboard_focus_is_focusable())
      {

         simple_ui_draw_focus_rect(pgraphics);

      }

   }


   void interaction::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      ::user::interaction::_001OnDraw(pgraphics);

   }


   bool interaction::has_action_hover()
   {

      return false;

   }

   bool interaction::has_text_input()
   {

      return false;

   }

   COLORREF interaction::get_action_hover_border_color()
   {

      return _001GetColor(::user::color_action_hover_border_color);

   }

   void interaction::simple_ui_draw_focus_rect(::draw2d::graphics * pgraphics)
   {

      userstyle()->simple_ui_draw_focus_rect(this,pgraphics);

   }


   void interaction::simple_ui_draw_frame_window_rect(::draw2d::graphics * pgraphics)
   {

      rect rectClient;

      GetClientRect(rectClient);

      rectClient.deflate(0,0,1,1);

      ::draw2d::brush_sp b(allocer());

      b->create_solid(ARGB(255, 255, 255, 255));

      pgraphics->SelectObject(b);

      ::draw2d::pen_sp p(allocer());

      p->create_solid(1.0, ARGB(255, 0, 0, 0));

      pgraphics->SelectObject(p);

      pgraphics->rectangle(rectClient);

   }


   void interaction::simple_ui_draw_fuzzy_color_spread(::draw2d::graphics * pgraphics)
   {
      /*
      rect rectWindow;

      GetWindowRect(rectWindow);

      int i = 0;
      int j = 0;
      int iCount = rectWindow.width();
      int jCount = rectWindow.height();

      BYTE * p = (BYTE *)m_dib->get_data();

      for (i = 0; i < iCount; i++)
      {
      for (j = 0; j < jCount; j++)
      {
      double dPhase = fmod((((double) ::get_tick_count() * 360 * 0.5984 / 1000.0) + (i * 360.0 / (double)iCount) + (j * 360.0 / (double)jCount) + ((double)(sin(((double) ::get_tick_count() * 3.1415 * 2.0  *0.0484 / 1000.0) + i * 3.1415 * 2.0 * 2.0 / (double)(iCount)) * sin(((double) ::get_tick_count() * 3.1415 * 2.0  * 0.0484 / 1000.0) + j * 3.1415 * 2.0 * 2.0 / (double)(jCount)) * 360))), 360.0);
      int32_t iR;
      int32_t iG;
      int32_t iB;
      double dRate = fmod(dPhase, 60.0) / 60.0;
      int32_t iColor = (int32_t)(dRate * 155.0);
      if (dPhase < 60)
      {
      // purple to blue
      iR = 255 - iColor;
      iG = 100;
      iB = 255;
      }
      else if (dPhase < (120))
      {
      // blue to cyan
      iR = 100;
      iG = 100 + iColor;
      iB = 255;
      }
      else if (dPhase < (180))
      {
      // cyan to green
      iR = 100;
      iG = 255;
      iB = 255 - iColor;
      }
      else if (dPhase < (240))
      {
      // green to yellow
      iR = 100 + iColor;
      iG = 255;
      iB = 100;
      }
      else if (dPhase < (300))
      {
      // yellow to red
      iR = 255;
      iG = 255 - iColor;
      iB = 100;
      }
      else
      {
      // red to purple
      iR = 255;
      iG = 100;
      iB = 100 + iColor;
      }

      p[0] = iR;
      p[1] = iG;
      p[2] = iB;
      p[3] = 84;
      p += 4;

      }
      }
      */


   }


   void interaction::simple_ui_draw_dark_glass(::draw2d::graphics * pgraphics)
   {

      rect rectWindow;

      GetClientRect(rectWindow);

      pgraphics->fill_solid_rect(rectWindow, ARGB(84, 127, 127, 127));

   }



   void interaction::simple_ui_draw_pestana(::draw2d::graphics * pgraphics)
   {

      rect rectWindow;

      GetClientRect(rectWindow);

      pgraphics->set_alpha_mode(draw2d::alpha_mode_blend);

      int iCount = rectWindow.height();
      ::draw2d::pen_sp p(allocer());
      for (int i = 0; i < iCount; i += 2)
      {
         double dRate = (double)i / (double)iCount;
         dRate = 1.0 - dRate;
         byte a1 = brate(1.0 - dRate, 23, 84);
         byte r1 = brate(dRate, 23, 127);
         byte g1 = brate(dRate, 23, 127);
         byte b1 = brate(dRate, 23, 127);
         p->create_solid(1.0, ARGB(a1, r1, g1, b1));
         pgraphics->SelectObject(p);
         pgraphics->draw_line(rectWindow.left, i, rectWindow.right, i);

      }


   }



   void interaction::simple_ui_draw_auth_box(::draw2d::graphics * pgraphics)
   {

      rect rect;

      GetClientRect(rect);

      pgraphics->set_alpha_mode(draw2d::alpha_mode_blend);

      // front
      point pa[4];
      //  0       1
      //
      //
      //  3       2

      // back
      point pb[4];


      // guards
      point pc[4];


      pa[0].x = rect.left;
      pa[0].y = rect.top;
      pa[1].x = rect.right - rect.width() / 10;
      pa[1].y = rect.top + rect.height() / 10;
      pa[2].x = rect.right - rect.width() / 8;
      pa[2].y = rect.bottom - rect.height() / 10;
      pa[3].x = rect.left;
      pa[3].y = rect.bottom;


      pb[0].x = rect.left + rect.width() / 6;
      pb[0].y = rect.top + rect.height() / 6;
      pb[1].x = rect.right - rect.width() / 6;
      pb[1].y = rect.top + rect.height() / 6;
      pb[2].x = rect.right - rect.width() / 5;
      pb[2].y = rect.bottom - rect.height() / 4;
      pb[3].x = rect.left + rect.width() / 5;
      pb[3].y = rect.bottom - rect.height() / 4;

      {

         ::draw2d::brush_sp b(allocer());

         b->create_solid(ARGB(184, 220, 220, 220));

         pgraphics->SelectObject(b);

         pgraphics->fill_polygon(pb, 4);

         ::draw2d::pen_sp p(allocer());

         p->create_solid(1.0, ARGB(184, 184, 184, 184));

         pgraphics->SelectObject(p);

         pgraphics->draw_polygon(pb, 4);


      }













      // top
      memcpy(pc, pa, sizeof(pb));
      pc[2].x = pb[1].x;
      pc[2].y = pb[1].y;
      pc[3].x = pb[0].x;
      pc[3].y = pb[0].y;

      {

         ::draw2d::brush_sp b(allocer());

         b->create_solid(ARGB(177, 210, 210, 210));

         pgraphics->SelectObject(b);

         pgraphics->fill_polygon(pc, 4);

         ::draw2d::pen_sp p(allocer());

         p->create_solid(1.0, ARGB(177, 177, 177, 177));

         pgraphics->SelectObject(p);

         pgraphics->draw_polygon(pc, 4);


      }
      // left
      memcpy(pc, pa, sizeof(pb));
      pc[1].x = pb[0].x;
      pc[1].y = pb[0].y;
      pc[2].x = pb[3].x;
      pc[2].y = pb[3].y;
      {

         ::draw2d::brush_sp b(allocer());

         b->create_solid(ARGB(123, 123, 123, 123));

         pgraphics->SelectObject(b);

         pgraphics->fill_polygon(pc, 4);

         ::draw2d::pen_sp p(allocer());

         p->create_solid(1.0, ARGB(123, 84, 84, 84));

         pgraphics->SelectObject(p);

         pgraphics->draw_polygon(pc, 4);


      }
      // bottom
      memcpy(pc, pa, sizeof(pb));
      pc[0].x = pb[3].x;
      pc[0].y = pb[3].y;
      pc[1].x = pb[2].x;
      pc[1].y = pb[2].y;
      {

         ::draw2d::brush_sp b(allocer());

         b->create_solid(ARGB(149, 184, 184, 184));

         pgraphics->SelectObject(b);

         pgraphics->fill_polygon(pc, 4);

         ::draw2d::pen_sp p(allocer());

         p->create_solid(1.0, ARGB(149, 177, 177, 177));

         pgraphics->SelectObject(p);

         pgraphics->draw_polygon(pc, 4);


      }
      // right
      memcpy(pc, pa, sizeof(pb));
      pc[0].x = pb[1].x;
      pc[0].y = pb[1].y;
      pc[3].x = pb[2].x;
      pc[3].y = pb[2].y;
      {

         ::draw2d::brush_sp b(allocer());

         b->create_solid(ARGB(84, 149, 149, 149));

         pgraphics->SelectObject(b);

         pgraphics->fill_polygon(pc, 4);

         ::draw2d::pen_sp p(allocer());

         p->create_solid(1.0, ARGB(84, 123, 123, 123));

         pgraphics->SelectObject(p);

         pgraphics->draw_polygon(pc, 4);


      }





      {

         ::draw2d::pen_sp p(allocer());

         p->create_solid(1.0, ARGB(184, 84, 84, 84));

         pgraphics->SelectObject(p);

         pgraphics->draw_polygon(pa, 4);

      }


   }

   //void interaction::translate_mouse_message(int * px, int * py)
   //{

   //}


   //bool interaction::destroy_window()
   //{
   //
   //   return true;

   //}




   // the value -1 indicates outside the control,
   // other values may be control specific and are client hits
   index interaction::hit_test(point ptScreen,::user::e_element & eelement)
   {

      sp(::user::interaction) pwnd = this;

      rect rectWindow;

      pwnd->GetWindowRect(rectWindow);

      if(rectWindow.contains(ptScreen))
      {

         eelement = ::user::element_client;

         return 0;

      }
      else
      {

         eelement = ::user::element_none;

         return -1;

      }

   }


} // namespace simple_ui



