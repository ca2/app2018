//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"




namespace user
{


   place_holder::place_holder(::aura::application * papp) :
      ::object(papp)
   {
   }


   place_holder::~place_holder()
   {
   }

   void place_holder::install_message_handling(::message::dispatch * pdispatch)
   {
      ::user::interaction::install_message_handling(pdispatch);
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


   bool place_holder::hold(::user::interaction * pui)
   {

      if(pui == NULL)
         return false;

      if(m_uiptraChild.contains(pui))
         return true;

      for(index i = m_uiptraChild.get_upper_bound(); i >= 0; i--)
      {

         System.place_hold(m_uiptraChild[i]);

      }

      m_uiptraChild.remove_all();

      pui->SetParent(this);

      layout();
      
      return true;

   }


   bool place_holder::unhold(::user::interaction * pui)
   {



      return System.place_hold(pui) != NULL;

   }

   void place_holder::layout()
   {
      
      if(m_uiptraChild.get_count() <= 0)
         return;

      ::user::interaction * puiHold = m_uiptraChild[0];

      rect rectClient;

      GetClientRect(rectClient);

      if(rectClient.area() <= 0)
         return;

      rect rectWindow;

      puiHold->GetWindowRect(rectWindow);

      ScreenToClient(rectWindow);

      if(rectWindow != rectClient)
      {

         puiHold->SetWindowPos(ZORDER_TOP, rectClient, 0);

      }
      else
      {
         
         puiHold->layout();

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

      if(!pholder->::user::interaction::create_window(NULL,NULL,WS_CHILD /*__WS_DEFAULT_VIEW*/,r,GetParent(),m_id))
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

   bool place_holder::_001OnCmdMsg(::aura::cmd_msg * pcmdmsg)
   {

      // then pump through frame
      if(::user::interaction::_001OnCmdMsg(pcmdmsg))
         return TRUE;

      // then pump through parent
      sp(::user::interaction) puiParent = GetParent();
      if(puiParent != NULL && puiParent->_001OnCmdMsg(pcmdmsg))
         return TRUE;

      return false;

   }


} // namespace user
