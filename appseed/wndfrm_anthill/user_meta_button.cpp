#include "framework.h"

namespace user
{

   meta_button::meta_button(::aura::application * papp) :
      object(papp),
      ::user::button(papp),
      ::user::wndfrm::frame::control_box_button(papp),
      m_spregion(allocer()),
      m_pen(allocer()),
      m_brush(allocer())
   {

   }


   meta_button::~meta_button()
   {

   }

   void meta_button::_001OnClip(::draw2d::graphics * pgraphics)
   {

      ::user::wndfrm::frame::control_box_button::_001OnClip(pgraphics);

   }

   void meta_button::_001OnNcDraw(::draw2d::graphics * pgraphics)
   {

   }


   void meta_button::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      if (GetTopLevel()->frame_is_transparent() && GetTopLevel() != GetActiveWindow())
      {

         return;

      }

      rect rectClient;

      ::user::interaction::GetClientRect(rectClient);

      if (rectClient.area() <= 0)
         return;

      COLORREF crText;

      pgraphics->set_smooth_mode(::draw2d::smooth_mode_high);

      if (!is_window_enabled())
      {

         pgraphics->SelectObject(m_pcontrolbox->m_brushButtonBackDisabled);

         pgraphics->SelectObject(m_pcontrolbox->m_penButtonBackDisabled);

         crText = m_pcontrolbox->m_crButtonForeDisabled;

      }
      else if (m_iHover >= 0)
      {

         pgraphics->SelectObject(m_pcontrolbox->m_brushButtonBackSel);

         pgraphics->SelectObject(m_pcontrolbox->m_penButtonBackSel);

         crText = m_pcontrolbox->m_crButtonForeSel;

      }
      else if (Session.get_keyboard_focus() == this)
      {

         pgraphics->SelectObject(m_pcontrolbox->m_brushButtonBackFocus);

         pgraphics->SelectObject(m_pcontrolbox->m_penButtonBackFocus);

         crText = m_pcontrolbox->m_crButtonForeFocus;

      }
      else
      {

         pgraphics->SelectObject(m_pcontrolbox->m_brushButtonBack);

         pgraphics->SelectObject(m_pcontrolbox->m_penButtonBack);

         crText = m_pcontrolbox->m_crButtonFore;

      }

      class rect rectEllipse(rectClient);

      class rect rW;

      GetWindowRect(rW);

      rectEllipse.deflate(0, 0, 2, 2);

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      pgraphics->FillSolidRect(rectClient, pgraphics->m_spbrush->m_cr);

      //pgraphics->FillEllipse(rectEllipse);

      //pgraphics->DrawEllipse(rectEllipse);

      if (m_estockicon == stock_icon_none)
      {

         string str;

         get_window_text(str);

         select_font(pgraphics, font_button);

         pgraphics->set_text_color(crText);

         pgraphics->draw_text(str, rectClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

      }
      else
      {

         m_brush->create_solid(pgraphics->get_current_pen().is_set()
            ? pgraphics->get_current_pen()->m_cr : ARGB(255, 255, 255, 255));

         pgraphics->SelectObject(m_brush);

         m_pen->create_solid(1.0, pgraphics->get_current_pen().is_set()
            ? pgraphics->get_current_pen()->m_cr : ARGB(255, 255, 255, 255));

         pgraphics->SelectObject(m_pen);

         class rect rectIcon(rectEllipse);

         rectIcon.deflate(rectIcon.width() / 4, rectIcon.height() / 4);

         pgraphics->draw_stock_icon(rectIcon, m_estockicon);

         //pgraphics->FillSolidRect(rectEllipse, ARGB(255, 255, 255, 255));

      }

   }


   void meta_button::install_message_routing(::message::sender *pinterface)
   {

      ::user::button::install_message_routing(pinterface);

      IGUI_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &meta_button::_001OnShowWindow);

   }

   void meta_button::_001OnShowWindow(::message::message * pobj)
   {


      SCAST_PTR(::message::show_window, pshowwindow, pobj);


      if (pshowwindow->m_bShow)
      {

         if (m_estockicon == stock_icon_restore)
         {

            output_debug_string("restore button is being shown");

         }

      }


   }


   void meta_button::on_layout()
   {

      rect rectClient;

      GetClientRect(rectClient);

      m_spregion->create_oval(rectClient);

   }


   index meta_button::hit_test(point point, e_element & eelement)
   {

      synch_lock sl(m_pmutex);

      if (m_spregion.is_null())
      {

         eelement = element_none;

         return -1;

      }

      if (m_spregion->get_os_data() == NULL)
      {

         eelement = element_none;

         return -1;

      }

      ScreenToClient(&point);

      if (!m_spregion->contains(point))
      {

         eelement = element_none;

         return -1;

      }

      eelement = element_client;

      return 0;

   }


   bool meta_button::keyboard_focus_is_focusable()
   {

      //return false && ::user::button::keyboard_focus_is_focusable();
      
      return false;
      

   }



} // namespace user




