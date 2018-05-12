﻿#include "framework.h"
//#include "base/user/user.h"


//#include <math.h>


namespace hotplugin
{


   void plugin::on_bare_paint_veri_discreet(::draw2d::graphics * pgraphics,const RECT & lprect)
   {

      double dRate = get_progress_rate();

      if(dRate <= 0.0)
      {

         return;

      }

      RECT rectWindow;
      GetWindowRect(&rectWindow);
      //  int32_t cx = rectWindow.right - rectWindow.left;
      //int32_t cy = rectWindow.bottom - rectWindow.top;
      RECT rect = lprect;



      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
      int h = 33;
      int m = 49 * 2;

      int top;
      int bottom;
      int left;
      int right;

      if(::height(rect) < h)
      {
         top = rect.top;
         bottom = rect.bottom;
      }
      else
      {
         top = rect.top + ::height(rect) / 2 - h / 2;
         bottom = rect.top + ::height(rect) / 2 + h / 2;
      }

      if(::width(rect) < m)
      {
         left = rect.left;
         right = rect.right;
      }
      else
      {

         left = rect.left + MIN(m / 2,::width(rect) / 2);
         right = rect.right - MIN(m / 2,::width(rect) / 2);

      }

      ::rect rectBar(left,top,right,bottom);




      ::draw2d::pen_sp pen(allocer());

      pen->create_null();

      ::draw2d::brush_sp brush(allocer(),ARGB(184,49,49,49));

      pgraphics->SelectObject(pen);

      pgraphics->SelectObject(brush);

      ::rect rectClient;

      GetClientRect(rectClient);


      ::rect rectProgress(rectBar);

      rectProgress.right = (LONG) (rectProgress.left + rectBar.width() * MIN(1.0, MAX(0.0, dRate)));

      ::rect rectProgressComplement(rectBar);

      rectProgressComplement.left = rectProgress.right;

      pgraphics->rectangle(rectProgressComplement);

      {

         BYTE uchR,uchG,uchB;
         ::draw2d::brush_sp br(allocer());
         get_progress_color(uchR,uchG,uchB,dRate,0);
         br->create_solid(ARGB(184,uchR,uchG,uchB));
         pgraphics->fill_rect(rectProgress,br);

      }

      pen->create_solid(1.0,ARGB(149,149,149,142));

      pgraphics->draw_rect(rectBar, pen);

      ::draw2d::font_sp f(allocer());

      f->create_pixel_font("Calibri",18);

      pgraphics->SelectObject(f);

      string strProgress;

      strProgress.Format("%0.3f%%",dRate * 100.0);

      rectBar.left +=4;

      rectBar.top += 5;

      pgraphics->set_text_color(ARGB(255, 255, 255, 255));

      pgraphics->set_text_rendering(::draw2d::text_rendering_clear_type_grid_fit);

      pgraphics->text_out(rectBar.left, rectBar.top, m_strStatus + " : " + strProgress + " : " + m_strStatus2);

   }


} // namespace hotplugin


