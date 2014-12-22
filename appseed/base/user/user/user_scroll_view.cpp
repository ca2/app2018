#include "framework.h"


namespace user
{


   scroll_view::scroll_view(::aura::application * papp) :
      element(papp)
   {

   }


   scroll_view::~scroll_view()
   {

   }

   void scroll_view::install_message_handling(::message::dispatch * pdispatch)
   {

      ::user::scroll_control::install_message_handling(pdispatch);
      ::aura::impact::install_message_handling(pdispatch);

   }


} // namespace user







