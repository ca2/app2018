#include "framework.h"

#ifdef MACOS
#define ARGB_COLORREF(A, R, G, B) ARGB(A, R, G, B)
#define COLORREF_get_a_value(cr) argb_get_a_value(cr)
#define COLORREF_get_r_value(cr) argb_get_r_value(cr)
#define COLORREF_get_g_value(cr) argb_get_g_value(cr)
#define COLORREF_get_b_value(cr) argb_get_b_value(cr)
#else
#define ARGB_COLORREF(A, R, G, B) ARGB(A, B, G, R)
#define COLORREF_get_a_value(cr) argb_get_a_value(cr)
#define COLORREF_get_r_value(cr) argb_get_b_value(cr)
#define COLORREF_get_g_value(cr) argb_get_g_value(cr)
#define COLORREF_get_b_value(cr) argb_get_r_value(cr)
#endif



namespace flag
{


   void dk(::draw2d::graphics * pgraphics, double x, double y, double w, double h)
   {

      COLORREF crDenmarkRoed = ARGB(255, 200, 16, 46);

      double dx = w / 90.0;

      double dy = h / 70.0;

      pgraphics->fill_solid_rect_dim(x+0, y+0, 90.0 * dx, 70.0 * dy, ARGB(255, 255, 255, 255));

      pgraphics->fill_solid_rect_dim(x+0, y+0, 30.0*dx, 30.0*dy, crDenmarkRoed);

      pgraphics->fill_solid_rect_dim(x + 40.0*dx, y, 50 * dx, 30 * dy, crDenmarkRoed);

      pgraphics->fill_solid_rect_dim(x+0, y + 40.0 * dy, 30.0*dx, 30.0 * dy, crDenmarkRoed);

      pgraphics->fill_solid_rect_dim(x + 40.0*dx, y + 40.0 * dy, 50.0* dx, 30.0 * dy, crDenmarkRoed);

   }


   void gay_with_shades_of_grey(::draw2d::dib * pdib)
   {

      pdib->map();

      ::count w = pdib->m_size.cx;

      ::count h = pdib->m_size.cy;

      color c;

      double dw = w;

      double dh = h;

      UINT uScan;

      uScan = pdib->m_iScan / sizeof(COLORREF);

      COLORREF * pline;

      for (index i = 0; i < w; i++)
      {

         double dR, dG, dB;

         pline = pdib->m_pcolorref + i;

         double dH = (double)i / (double)dw;

         dH *= 6.0;

         double dA = dH - (double)((int32_t)dH);

         if (dH >= 3.0)
         {
            if (dH >= 4.0)
            {
               if (dH >= 5.0)
               {
                  // 5.0
                  // magenta to red
                  dR = 1.0;
                  dG = 0.0;
                  dB = 1.0 - dA;
               }
               else
               {
                  // 4.0
                  // blue to magenta
                  dR = dA;
                  dG = 0.0;
                  dB = 1.0;
               }
            }
            else
            {
               // 3.0
               // cyan to blue
               dR = 0.0;
               dG = 1.0 - dA;
               dB = 1.0;
            }
         }
         else /// if(dH >= 0.0)
         {
            if (dH >= 2.0)
            {
               // 2
               // green to cyan
               dR = 0.0;
               dG = 1.0;
               dB = dA;
            }
            else // (dH >= 0.0 && dH < 2.0)
            {
               if (dH >= 1.0)
               {
                  // 1
                  // yellow to green
                  dR = 1.0 - dA;
                  dG = 1.0;
                  dB = 0.0;
               }
               else // if(dh >= 0 && dH < 1.0);
               {
                  // 0
                  // red to yellow
                  dR = 1.0;
                  dG = dA;
                  dB = 0.0;
               }
            }
         }

         double dL = 0.5;

         for (index j = 0; j < h; j++)
         {



            double dS = 1.0 - ((double) j / dh);
#ifdef MACOS
            dS = 1.0 - dS;
#endif

            double dCMin;
            double dCAdd;
            double dSL = dS * dL;
            if (dL >= 0.5)
            {
               dCMin = dL - dS + dSL;
               dCAdd = 2.0 * dS - 2.0 * dSL;
            }
            else
            {
               dCMin = dL - dSL;
               dCAdd = 2.0 * dSL;
            }


            double _dR = (dCMin + dR * dCAdd);
            double _dG = (dCMin + dG * dCAdd);
            double _dB = (dCMin + dB * dCAdd);

            //BYTE uchR = (BYTE)primitive_color_round(m_dR * 255.0);
            //m_uchG = (BYTE)primitive_color_round(m_dG * 255.0);
            //m_uchB = (BYTE)primitive_color_round(m_dB * 255.0);


            *pline = ARGB_COLORREF(255, BYTE(_dR*255.0), BYTE(_dG*255.0), BYTE(_dB*255.0));

            pline+=uScan;

         }


      }


   }

   void shades_of_luminance(::draw2d::dib * pdib, double dH, double dS)
   {

      pdib->map();

      ::count w = pdib->m_size.cx;

      ::count h = pdib->m_size.cy;

      color c;

      double dh = h;

      UINT uScan;


      //dS = 1.0 - ((double)j / dh);

      uScan = pdib->m_iScan / sizeof(COLORREF);

      COLORREF * pline;

      double dR, dG, dB;

      dH *= 6.0;

      double dA = dH - (double)((int32_t)dH);

      if (dH >= 3.0)
      {
         if (dH >= 4.0)
         {
            if (dH >= 5.0)
            {
               // 5.0
               // magenta to red
               dR = 1.0;
               dG = 0.0;
               dB = 1.0 - dA;
            }
            else
            {
               // 4.0
               // blue to magenta
               dR = dA;
               dG = 0.0;
               dB = 1.0;
            }
         }
         else
         {
            // 3.0
            // cyan to blue
            dR = 0.0;
            dG = 1.0 - dA;
            dB = 1.0;
         }
      }
      else /// if(dH >= 0.0)
      {
         if (dH >= 2.0)
         {
            // 2
            // green to cyan
            dR = 0.0;
            dG = 1.0;
            dB = dA;
         }
         else // (dH >= 0.0 && dH < 2.0)
         {
            if (dH >= 1.0)
            {
               // 1
               // yellow to green
               dR = 1.0 - dA;
               dG = 1.0;
               dB = 0.0;
            }
            else // if(dh >= 0 && dH < 1.0);
            {
               // 0
               // red to yellow
               dR = 1.0;
               dG = dA;
               dB = 0.0;
            }
         }
      }

      for (index j = 0; j < h; j++)
      {
         double dL = 1.0 - ((double) j / dh);

#ifdef MACOS
         dL = 1.0 - dL;
#endif

         //double dS = 1.0 - ((double)j / dh);

         double dCMin;
         double dCAdd;
         double dSL = dS * dL;
         if (dL >= 0.5)
         {
            dCMin = dL - dS + dSL;
            dCAdd = 2.0 * dS - 2.0 * dSL;
         }
         else
         {
            dCMin = dL - dSL;
            dCAdd = 2.0 * dSL;
         }


         double _dR = (dCMin + dR * dCAdd);
         double _dG = (dCMin + dG * dCAdd);
         double _dB = (dCMin + dB * dCAdd);

         //BYTE uchR = (BYTE)primitive_color_round(m_dR * 255.0);
         //m_uchG = (BYTE)primitive_color_round(m_dG * 255.0);
         //m_uchB = (BYTE)primitive_color_round(m_dB * 255.0);

         pline = pdib->m_pcolorref + uScan * j;
         COLORREF cr = ARGB_COLORREF(255, BYTE(_dR*255.0), BYTE(_dG*255.0), BYTE(_dB*255.0));
         for (index i = 0; i < w; i++)
         {

            *pline = cr;




            pline ++;

         }


      }


   }

}

namespace userex
{


   color_view::color_view(::aura::application * papp) :
      ::object(papp),
      m_editRed(papp),
      m_editGreen(papp),
      m_editBlue(papp),
      m_editHex(papp),
      m_dib(allocer()),
      m_dibTemplate(allocer()),
      m_dibLuminance(allocer())
   {

      m_bCompact = true;

      m_strView = "color_sel";

      m_bLButtonPressed = false;

   }


   color_view::~color_view()
   {


   }


   void color_view::assert_valid() const
   {

      ::user::impact::assert_valid();

   }


   void color_view::dump(dump_context & dumpcontext) const
   {

      ::user::impact::dump(dumpcontext);

   }


   void color_view::install_message_routing(::message::sender * psender)
   {

      ::user::impact::install_message_routing(psender);

      IGUI_MSG_LINK(WM_CREATE, psender, this, &color_view::_001OnCreate);
      IGUI_MSG_LINK(WM_MOUSEMOVE, psender, this, &color_view::_001OnMouseMove);
      IGUI_MSG_LINK(WM_LBUTTONDOWN, psender, this, &color_view::_001OnLButtonDown);
      IGUI_MSG_LINK(WM_LBUTTONUP, psender, this, &color_view::_001OnLButtonUp);

      m_dibTemplate->create(2048, 2048);

      ::flag::gay_with_shades_of_grey(m_dibTemplate);

      m_dibLuminance->create(100, 100);

   }


   void color_view::on_update(::user::impact * pSender, LPARAM lHint, object* pHint)
   {

      ::user::impact::on_update(pSender, lHint, pHint);

   }


   void color_view::on_control_event(::user::control_event * pevent)
   {

      ::user::impact::on_control_event(pevent);

   }


   void color_view::_001OnCreate(::message::message * pmessage)
   {

      GetParentFrame()->m_id += ".color_sel";

      pmessage->previous();

      if(get_document()->m_pviewTopic == NULL)
      {

         get_document()->m_pviewTopic = this;

      }

      set_window_text("__CoLoR_");

      if(GetTopLevelFrame() != NULL)
      {

         GetTopLevelFrame()->ModifyStyleEx(0, WS_EX_TOOLWINDOW);

      }

   }


   color color_view::get_color()
   {

      color color;

      color.set_hls(m_hls);

      color.m_uchA = 255;

      return color.get_rgba();

   }


   void color_view::set_color(color color)
   {

      color.get_hls(m_hls);

   }


   void color_view::on_mouse(point pt)
   {

      synch_lock sl(m_pmutex);

      if (pt.y >= m_rectColors.bottom)
         return;
      if (pt.x < m_rectColors.left)
         return;
      if (pt.y < m_rectColors.top)
         return;

      if (pt.x < m_rectColors.center().x)
      {

         m_dib->map();


         pt -= m_rectColors.top_left();

         COLORREF cr = m_dib->m_pcolorref[pt.x + (m_dib->m_iScan / sizeof(COLORREF)) * m_dib->line(pt.y)];

         color c(ARGB(COLORREF_get_a_value(cr), COLORREF_get_r_value(cr), COLORREF_get_g_value(cr), COLORREF_get_b_value(cr)));

         color::hls hls;

         c.get_hls(hls);

         m_hls.m_dH = hls.m_dH;

         m_hls.m_dS = hls.m_dS;

         rebuild_luminance();

         ::user::control_event ev;

         ev.m_eevent = ::user::event_after_change_cur_hover;

         ev.m_id = m_strView;

         ev.m_puie = this;

         on_control_event(&ev);

      }
      else if (pt.x < m_rectColors.center().x + m_rectColors.width() / 8)
      {

         m_dibLuminance->map();

         pt -= size(m_rectColors.center().x, m_rectColors.top);

//         COLORREF cr = m_dibLuminance->m_pcolorref[pt.x + (m_dib->m_iScan / sizeof(COLORREF)) * pt.y];

         m_hls.m_dL = 1.0 - ((double) pt.y / (double) m_dib->m_size.cy);

         ::user::control_event ev;

         ev.m_eevent = ::user::event_after_change_cur_hover;

         ev.m_id = m_strView;

         ev.m_puie = this;

         on_control_event(&ev);

      }

   }

   void color_view::rebuild_luminance()
   {

      ::flag::shades_of_luminance(m_dibLuminance, m_hls.m_dH, m_hls.m_dS);

   }

   void color_view::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      rect rC;

      GetClientRect(rC);

      pgraphics->SetStretchBltMode(HALFTONE);

      rect r1;

      r1.top_left() = m_rectColors.top_left();

      r1.set_size(m_dib->m_size);

      rect r2 = m_dib->rect();

      pgraphics->draw(r1, m_dib->g(), r2);

      r1.top_left() = m_rectColors.top_left() + size(m_dib->m_size.cx-1, 0);

      r1.set_size(m_dibLuminance->m_size);

      r2 = m_dibLuminance->rect();

      pgraphics->draw(r1, m_dibLuminance->g(), r2);

      r1.top_left() = m_rectColors.top_left() + size(m_dib->m_size.cx - 1 + m_dibLuminance->m_size.cx - 1, 0);

      r1.set_size(m_rectColors.right - r1.left, m_dib->m_size.cy);

      pgraphics->fill_solid_rect(r1, get_color());

   }

   void color_view::_001OnLButtonDown(::message::message * pmessage)
   {
      SCAST_PTR(::message::mouse, pmouse, pmessage);
      point pt = pmouse->m_pt;
      ScreenToClient(pt);
      on_mouse(pt);
      pmouse->m_bRet = true;
      SetCapture();
      m_bLButtonPressed = true;
   }


   void color_view::_001OnLButtonUp(::message::message * pmessage)
   {
      SCAST_PTR(::message::mouse, pmouse, pmessage);
      point pt = pmouse->m_pt;
      ScreenToClient(pt);
      on_mouse(pt);
      pmouse->m_bRet = true;
      ReleaseCapture();
      m_bLButtonPressed = false;

      ::user::control_event ev;

      ev.m_eevent = ::user::event_after_change_cur_sel;

      ev.m_id = m_strView;

      ev.m_puie = this;

      on_control_event(&ev);



   }


   void color_view::_001OnMouseMove(::message::message * pmessage)
   {
      SCAST_PTR(::message::mouse, pmouse, pmessage);

      if (m_bLButtonPressed)
      {
         point pt = pmouse->m_pt;
         ScreenToClient(pt);
         on_mouse(pt);
         pmouse->m_bRet = true;
         ReleaseCapture();

      }
   }

   void color_view::on_layout()
   {

      ::user::impact::on_layout();

      rect rectClient;

      GetClientRect(rectClient);

      if (rectClient.area() <= 0)
      {

         return;

      }

      rect rectColors;

      GetClientRect(rectColors);

      if(!m_bCompact)
      {

         rectColors.left = rectClient.center().x;
         rectColors.bottom = rectClient.center().y;

         rectColors.deflate(rectClient.width() / 16, rectClient.height() / 16);

      }

      m_rectColors = rectColors;

      m_dib->create(m_rectColors.width() / 2, m_rectColors.height());

      m_dib->g()->draw(m_dib->rect(), m_dibTemplate->get_graphics(), m_dibTemplate->rect());

      m_dibLuminance->create(m_rectColors.width() / 8, m_rectColors.height());

      rebuild_luminance();

   }


} // namespace userex



