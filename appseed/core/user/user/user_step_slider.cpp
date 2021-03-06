#include "framework.h"


namespace user
{


   step_slider::step_slider(::aura::application * papp) :
      object(papp),
      ::user::interaction(papp)
   {

      m_iHover = 0x80000000;
      m_bHover = false;

   }

   step_slider::~step_slider()
   {
   }


   bool step_slider::create_window(const RECT & rect, ::user::interaction * pwndParent, id id)
   {

      return ::user::interaction::create_window(NULL, NULL, WS_CHILD, rect, pwndParent, id) != FALSE;

   }


   void step_slider::install_message_routing(::message::sender * psender)
   {
      ::user::interaction::install_message_routing(psender);
      IGUI_MSG_LINK(WM_CREATE, psender, this, &step_slider::_001OnCreate);
//      //IGUI_MSG_LINK(WM_TIMER, psender, this, &step_slider::_001OnTimer);
      IGUI_MSG_LINK(WM_LBUTTONDOWN, psender, this, &step_slider::_001OnLButtonDown);
      IGUI_MSG_LINK(WM_LBUTTONUP, psender, this, &step_slider::_001OnLButtonUp);
      IGUI_MSG_LINK(WM_MOUSEMOVE, psender, this, &step_slider::_001OnMouseMove);
   }

   void step_slider::_001OnCreate(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
//      SCAST_PTR(::message::create, pcreate, pobj);

   }

   void step_slider::_001OnTimer(::timer * ptimer)
   {
      ::user::interaction::_001OnTimer(ptimer);;
      if(ptimer->m_nIDEvent == 1)
      {
         if(m_bHover)
         {
            UpdateHover();
            if(!m_bHover)
               KillTimer(1);
         }
      }
   }

   void step_slider::_001OnLButtonDown(::message::message * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      m_iLButtonDown = hit_test(pmouse->m_pt);

   }

   void step_slider::_001OnLButtonUp(::message::message * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      int64_t iLButtonUp = hit_test(pmouse->m_pt);

      if(iLButtonUp == m_iLButtonDown)
      {

         m_scalar.set(iLButtonUp);

      }

   }

   void step_slider::_001OnMouseMove(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
//      SCAST_PTR(::message::mouse, pmouse, pobj);
      SetTimer(1, 200, NULL);
      m_bHover = true;
      UpdateHover();
   }


   void step_slider::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      

      if(m_scalar.is_null())
         return;

      rect rectClient;
      GetClientRect(rectClient);
      class imaging & imaging = Application.imaging();

      byte bAlpha = (byte) (128.0 * get_alpha());

      imaging.color_blend(
         pgraphics,
         rectClient,
         RGB(150, 200, 255),
         bAlpha);

      int64_t iMin = m_scalar.minimum();
      int64_t iMax = m_scalar.maximum();
      int64_t iVal = m_scalar;

      rect rect;
      for(int64_t i = iMin; i <= iMax; i++)
      {
         GetStepRect(rect, i, iMin, iMax, rectClient);
         if(i == iVal)
         {
            if(i == m_iHover)
            {
               imaging.color_blend(
                  pgraphics,
                  rect,
                  RGB(255, 255, 240),
                  bAlpha);
            }
            else
            {
               imaging.color_blend(
                  pgraphics,
                  rect,
                  RGB(255, 255, 150),
                  bAlpha);
            }
         }
         else
         {
            if(i == m_iHover)
            {
               imaging.color_blend(
                  pgraphics,
                  rect,
                  RGB(255, 180, 180),
                  bAlpha);
            }
            else
            {
               imaging.color_blend(
                  pgraphics,
                  rect,
                  RGB(100, 100, 255),
                  bAlpha);
            }
         }
      }
   }


   void step_slider::GetStepHoverRect(LPRECT lprect, int64_t iStep, int64_t iMin, int64_t iMax, LPCRECT lpcrectClient)
   {

      if((iMax - iMin) == 0)
         return;

      lprect->top = lpcrectClient->top;
      lprect->bottom = lpcrectClient->bottom;
      double dWidth = ((double)::width(lpcrectClient)) / (iMax - iMin);
      lprect->left = (LONG) (dWidth * (iStep - iMin));
      lprect->right = (LONG) (dWidth * (iStep - iMin + 1));


   }


   void step_slider::GetStepRect(LPRECT lprect, int64_t iStep, int64_t iMin, int64_t iMax, LPCRECT lpcrectClient)
   {

      if((iMax - iMin) == 0)
         return;

      GetStepHoverRect(lprect, iStep, iMin, iMax, lpcrectClient);
      int32_t halfm = (lprect->right - lprect->left - 2) / 2;
      lprect->left +=  halfm;
      lprect->right -=  halfm;

   }

   int64_t step_slider::hit_test(point point)
   {

      rect rectClient;

      GetClientRect(rectClient);

      if (rectClient.width() == 0)
         return -1;

      int64_t iMin, iMax;

      iMin = m_scalar.minimum();

      iMax = m_scalar.maximum();

      return iMin + (((point.x - rectClient.left) * (iMax - iMin)) / rectClient.width());

   }


   void step_slider::UpdateHover()
   {
      point pt;
      Session.get_cursor_pos(&pt);
      ScreenToClient(&pt);
      m_iHover = hit_test(pt);
      set_need_redraw();
      rect rectClient;
      GetClientRect(rectClient);
      m_bHover = rectClient.contains(pt) != FALSE;
   }

} // namespace user
