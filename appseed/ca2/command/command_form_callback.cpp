#include "framework.h"

namespace command
{

   form_callback::form_callback()
   {
   }

   form_callback::~form_callback()
   {
   }

   void form_callback::on_update(::user::form * pview, ::view* pSender, LPARAM lHint, ::radix::object* phint) 
   {
      UNREFERENCED_PARAMETER(pview);
      UNREFERENCED_PARAMETER(pSender);
      UNREFERENCED_PARAMETER(lHint);
      UNREFERENCED_PARAMETER(phint);
   }


   bool form_callback::BaseOnControlEvent(::user::form * pview, ::user::control_event * pevent)
   {
      UNREFERENCED_PARAMETER(pview);
      ::user::interaction * pguie = dynamic_cast < ::user::interaction * > (this);
      if(pguie != NULL && pguie->GetParent() != NULL)
      {
         return pguie->GetParent()->BaseOnControlEvent(pevent);
      }
      return false;
   }

} // namespace command