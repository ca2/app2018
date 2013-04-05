#include "framework.h"


namespace user
{

   place_holder_container::place_holder_container(::ca::applicationsp papp) :
      ca(papp),
      m_holdera(papp)
   {
   }

   place_holder_container::~place_holder_container()
   {
   }

   void place_holder_container::install_message_handling(::ca::message::dispatch * pdispatch)
   {
      ::user::control::install_message_handling(pdispatch);
   }

   sp(place_holder)place_holder_container::get_new_place_holder()
   {
      m_holdera.add(new place_holder(get_app()));
      if(m_holdera.last_element().is_null())
         return ::null();
      if(!m_holdera.last_element()->create((sp(::user::place_holder_container))this, m_holdera.get_upper_bound()))
      {
         m_holdera.remove_last();
         return ::null();
      }
      return m_holdera.last_element();
   }

   bool place_holder_container::remove_place_holder(sp(place_holder) pholder)
   {
      bool bRemove = m_holdera.remove(pholder) > 0;
      return bRemove;
   }

   sp(place_holder) place_holder_container::hold(sp(::user::interaction) pui)
   {
      sp(place_holder) pholder = get_new_place_holder();
      if(!on_hold(pui, pholder))
      {
         remove_place_holder(pholder);
         return ::null();
      }
      return pholder;
   }


   bool place_holder_container::unhold(sp(::user::interaction) pui)
   {
      for(int32_t i = 0; i < m_holdera.get_count(); i++)
      {
         if(m_holdera[i].is_holding(pui))
         {
            if(on_unhold(pui, m_holdera(i)))
            {
               m_holdera.remove_at(i);
               return true;
            }
            return false;
         }
      }
      return false;
   }

   bool place_holder_container::on_hold(sp(::user::interaction) pui, sp(place_holder) pholder)
   {
      if(pholder->can_merge(pui))
      {
         return pholder->merge(pui);
      }
      else
      {
         return pholder->hold(pui);
      }
   }

   bool place_holder_container::on_unhold(sp(::user::interaction) pui, sp(place_holder) pholder)
   {
      return pholder->unhold(pui);
   }


   bool place_holder_container::create(sp(::user::interaction) puiParent, id id)
   {
      return ::database::user::interaction::create(::null(), ::null(), 0 /*__WS_DEFAULT_VIEW*/, rect(0,0,0,0), puiParent, id) != FALSE;
   }

   place_holder_ptra place_holder_container_ptra::hold(sp(::user::interaction) pui)
   {
      place_holder_ptra holderptra;
      sp(place_holder) pholder;
      for(int32_t i = 0; i < this->get_count(); i++)
      {
         pholder = this->element_at(i)->hold(pui);
         if(pholder != ::null())
         {
            holderptra.add(pholder);
         }
      }
      return holderptra;
   }

   int32_t place_holder_container_ptra::unhold(sp(::user::interaction) pui)
   {
      int32_t count = 0;
      for(int32_t i = 0; i < this->get_count(); i++)
      {
         if(this->element_at(i)->unhold(pui))
         {
            count++;
         }
      }
      return count;
   }

} // namespace user
