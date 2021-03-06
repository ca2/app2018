#include "framework.h"


namespace user
{


   place_holder::place_holder(::aura::application * papp) :
      ::object(papp)
   {

      m_flagNonClient.unsignalize(non_client_background);
      m_flagNonClient.unsignalize(non_client_focus_rect);

   }


   place_holder::~place_holder()
   {
   }


   void place_holder::install_message_routing(::message::sender * psender)
   {

      ::user::interaction::install_message_routing(psender);

      IGUI_MSG_LINK(WM_SHOWWINDOW, psender, this, &place_holder::_001OnShowWindow);

   }


   void place_holder::_001OnShowWindow(::message::message * pobj)
   {

      SCAST_PTR(::message::show_window, pshowwindow, pobj);

      ::user::interaction * phold = NULL;

      if (m_uiptraChild.get_size() > 0)
      {

         phold = m_uiptraChild[0];

      }

      if(phold != NULL)
      {

         if (pshowwindow->m_bShow && !phold->is_this_visible())
         {

            phold->ShowWindow(SW_SHOW);

         }

      }

   }


   bool place_holder::can_merge(::user::interaction * pui)
   {

      if(m_uiptraChild.contains(pui))
         return false;

      if(m_uiptraChild.get_count() == 0)
         return false;

      return m_uiptraChild[0]->can_merge(pui);

   }


   bool place_holder::merge(::user::interaction * pui)
   {

      if(!can_merge(pui))
         return false;

      if(!m_uiptraChild[0]->merge(pui))
         return false;

      return true;

   }


   bool place_holder::is_holding(::user::interaction * pui)
   {

      return m_uiptraChild.contains(pui);

   }


   void place_holder::on_remove_child(::user::interaction * pui)
   {

      if (GetParent() != NULL)
      {

         GetParent()->on_remove_place_holder_child(pui);

      }

   }


   void place_holder::on_hide_child(::user::interaction * pui)
   {

      if (GetParent() != NULL)
      {

         GetParent()->on_hide_place_holder_child(pui);

      }

   }


   bool place_holder::hold(::user::interaction * pui)
   {

      if(pui == NULL)
         return false;

      if(m_uiptraChild.contains(pui))
         return true;

      for(index i = m_uiptraChild.get_upper_bound(); i >= 0; i--)
      {

         Application.place_hold(m_uiptraChild[i]);

      }

      m_uiptraChild.remove_all();

      pui->SetParent(this);

      //on_layout();

      return true;

   }


   bool place_holder::unhold(::user::interaction * pui)
   {



      return System.place_hold(pui) != NULL;

   }

   void place_holder::on_layout()
   {

      if (m_uiptraChild.get_count() <= 0)
      {

         return;

      }

      ::user::interaction * puiHold = m_uiptraChild[0];

      rect rectClient;

      GetClientRect(rectClient);

      if (rectClient.area() <= 0)
      {

         return;

      }

      rect rectWindow;

      puiHold->GetWindowRect(rectWindow);

      ScreenToClient(rectWindow);

      puiHold->SetWindowPos(0, rectClient, SWP_NOZORDER);

      if (is_this_visible())
      {

         puiHold->ShowWindow(SW_SHOW);

      }

   }


   bool place_holder::create_window(const RECT & rect, ::user::place_holder_container * pcontainer,id id)
   {

      return ::user::interaction::create_window(NULL, NULL, WS_VISIBLE | WS_CHILD /*__WS_DEFAULT_VIEW*/, rect, pcontainer, id) != FALSE;

   }

   sp(place_holder) place_holder::create_shadow_clone()
   {

      sp(place_holder) pholder = canew(place_holder(get_app()));

      if(pholder.is_null())
      {

         return NULL;

      }

      ::rect r;

      GetWindowRect(r);

      if(!pholder->::user::interaction::create_window(NULL,NULL,WS_VISIBLE | WS_CHILD /*__WS_DEFAULT_VIEW*/,r,GetParent(),m_id))
      {

         return NULL;

      }

      return pholder;

   }


   interaction * place_holder::get_hold()
   {

      if(m_uiptraChild.get_count() <= 0)
         return NULL;

      return m_uiptraChild[0];

   }


   void place_holder::_001DrawThis(::draw2d::graphics * pgraphics)
   {

      UNREFERENCED_PARAMETER(pgraphics);

   }


   void place_holder::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      UNREFERENCED_PARAMETER(pgraphics);

   }

   void place_holder::_001OnNcDraw(::draw2d::graphics * pgraphics)
   {

      UNREFERENCED_PARAMETER(pgraphics);

   }


   int32_t place_holder_ptra::hold(::user::interaction * pui)
   {

      int32_t count = 0;

      for(int32_t i = 0; i < this->get_count(); i++)
      {
         try
         {
            if(this->element_at(i)->hold(pui))
            {
               count++;
            }
         }
         catch(...)
         {
         }
      }

      return count;

   }




   int32_t place_holder_ptra::unhold(::user::interaction * pui)
   {

      int32_t count = 0;

      for(int32_t i = 0; i < this->get_count(); i++)
      {
         try
         {
            if(this->element_at(i)->unhold(pui))
            {
               count++;
            }
         }
         catch(...)
         {
         }
      }

      return count;

   }

   bool place_holder::is_place_holder()
   {
      return true;
   }

   bool place_holder::_001IsBackgroundBypass()
   {

      return true;

   }


   void place_holder::route_command_message(::user::command * pcommand)
   {

      // then pump through frame
      ::user::interaction::route_command_message(pcommand);

      if(pcommand->m_bRet)
         return;

      // then pump through parent
      sp(::user::interaction) puiParent = GetParent();

      if (puiParent != NULL)
      {

         puiParent->route_command_message(pcommand);

         if (pcommand->m_bRet)
            return;

      }

   }


} // namespace user
