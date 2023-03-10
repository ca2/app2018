#include "framework.h"


namespace user
{


   elastic_slider::elastic_slider(::aura::application * papp) :
      object(papp),
      ::user::interaction(papp)
   {

      m_daScalar.allocate(100);
      m_daScalar.set(0.0);

      m_bSlide = false;
      m_iScalar = 0;

   }

   elastic_slider::~elastic_slider()
   {
   }


   void elastic_slider::install_message_routing(::message::sender * psender)
   {
      ::user::interaction::install_message_routing(psender);
      IGUI_MSG_LINK(WM_CREATE, psender, this, &elastic_slider::_001OnCreate);
      ////IGUI_MSG_LINK(WM_TIMER, psender, this, &elastic_slider::_001OnTimer);
      IGUI_MSG_LINK(WM_LBUTTONDOWN, psender, this, &elastic_slider::_001OnLButtonDown);
      IGUI_MSG_LINK(WM_LBUTTONUP, psender, this, &elastic_slider::_001OnLButtonUp);
      IGUI_MSG_LINK(WM_MOUSEMOVE, psender, this, &elastic_slider::_001OnMouseMove);
   }

   void elastic_slider::_001OnCreate(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      SetTimer(84 +77 +51 +33 + 23 + 49, 50, NULL);
   }

   void elastic_slider::_001OnTimer(::timer * ptimer)
   {

      ::user::interaction::_001OnTimer(ptimer);;

      if(ptimer->m_nIDEvent == 84 + 77 + 51 + 33 + 23 + 49)
      {
         double dScalar = CalcScalar();

         if(m_bSlide || dScalar > 0.001)
         {

            Slide();

         }
         else
         {
            UpdatePosition();
         }
      }

   }


   void elastic_slider::_001OnLButtonDown(::message::message * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);
      rect rect;
      GetSliderRect(rect);
      point pt(pmouse->m_pt);
      ScreenToClient(&pt);
      if(rect.contains(pt))
      {

         CalcTension(pt);
         SetCapture();
         m_dwLastTime = ::get_tick_count();
         m_daScalar.set(0.0);
         m_iScalar = 0;
         m_bSlide = true;
         pmouse->m_bRet = true;
         pmouse->set_lresult(1);
      }
   }

   void elastic_slider::_001OnLButtonUp(::message::message * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);
      if(m_bSlide)
      {

         m_bSlide= false;
         Slide();
         pmouse->m_bRet = true;
         pmouse->set_lresult(1);
      }
   }

   void elastic_slider::_001OnMouseMove(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
//      SCAST_PTR(::message::mouse, pmouse, pobj);
   }

   void elastic_slider::Slide()
   {

      UpdatePosition();

      m_pscalarVelocity->set_rate(CalcScalar(),scalar_set);

   }


   void elastic_slider::CalcTension(point & pt)
   {
      rect rectClient;
      GetClientRect(rectClient);
      if (rectClient.width() == 0)
      {
         m_dTensionPosition = 1.0;
      }

      else
      {
         m_dTensionPosition = ((double) pt.x / (double) rectClient.width());
      }
   }

   void elastic_slider::CalcTension()
   {
      point pt;
      Session.get_cursor_pos(&pt);
      ScreenToClient(&pt);
      CalcTension(pt);
   }
   double elastic_slider::GetForce()
   {
      return m_dTensionPosition - m_dPosition;
   }

   double elastic_slider::CalcScalar()
   {
      uint32_t dwTime = ::get_tick_count();
      if(dwTime - m_dwLastTime < 30)
         return m_daScalar.GetMean();
      CalcTension();
      double dScalar;
      if(m_bSlide)
      {
         double dForce = GetForce();
         double dDeltaTime = dwTime - m_dwLastTime;
         double dFilterLastScalar = m_daScalar.GetMean();
         double dRate = 1.0 / 100.0;
         dScalar = dForce * dDeltaTime * dRate + dFilterLastScalar;
      }
      else
      {
         dScalar = 0.0;
      }
      m_daScalar[m_iScalar] =  dScalar;
      m_iScalar = (m_iScalar + 1) % m_daScalar.get_size();
      m_dwLastTime = dwTime;
      return m_daScalar.GetMean(); // Low Pass Filter
   }

   void elastic_slider::SetStreamingVelocityMode(scalar_base * pscalarVelocity,scalar_base * pscalarPosition)
   {

      m_escalar = scalar_streaming_velocity;

      m_pscalarVelocity = pscalarVelocity;

      m_pscalarPosition = pscalarPosition;

   }

   void elastic_slider::UpdatePosition()
   {

      SetSliderPos(m_pscalarPosition->get_rate(0.0));

   }


   void elastic_slider::SetSliderPos(double dPos)
   {
      if(dPos < 0.0)
         dPos = 0.0;
      else if(dPos > 1.0)
         dPos = 1.0;
      m_dPosition = dPos;
      set_need_redraw();
   }


   void elastic_slider::_001OnDraw(::draw2d::graphics * pgraphics)
   {



      rect rectClient;
      GetClientRect(rectClient);

      class imaging & imaging = Application.imaging();

      byte bAlpha = (byte) (128.0 * get_alpha());

      imaging.color_blend(
      pgraphics,
      rectClient,
      RGB(250, 255, 255),
      bAlpha);

      rect rect;
      GetSliderRect(rect);

      pgraphics->draw3d_rect(rect,ARGB(bAlpha,255,255,255),ARGB(bAlpha,255,255,255));
      rect.deflate(1, 1);
      pgraphics->draw3d_rect(rect,ARGB(bAlpha,255,255,0),ARGB(bAlpha,255,255,0));
      rect.deflate(1, 1);
      pgraphics->draw3d_rect(rect,ARGB(bAlpha,255,255,255),ARGB(bAlpha,255,255,255));
      if(m_bSlide)
      {
         pgraphics->move_to(rect.center());
         point pt;
         Session.get_cursor_pos(pt);
         ScreenToClient(pt);
         pgraphics->line_to(pt);
      }
   }


   void elastic_slider::GetSliderRect(rect & rect)
   {
      class rect rectClient;
      GetClientRect(rectClient);

      int32_t iWidth = 16;
      rect.top = rectClient.top;
      rect.bottom = rectClient.bottom;
      rect.left = (LONG) MIN(rectClient.right, m_dPosition * (rectClient.width() - iWidth));
      rect.right = (LONG) MIN(rectClient.right, m_dPosition * ((rectClient.width() - iWidth)) + iWidth);
   }

} // namespace user
