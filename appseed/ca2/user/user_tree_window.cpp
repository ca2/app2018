#include "framework.h"


namespace user
{


   tree_window::tree_window(sp(::application) papp) :
      element(papp),
      ::user::interaction(papp),
      ::user::scroll_view(papp),
      ::user::tree(papp)
   {
   }

   tree_window::~tree_window()
   {
   }

   void tree_window::install_message_handling(::ca2::message::dispatch * pinterface)
   {


      ::user::interaction::install_message_handling(pinterface);
      ::user::tree::install_message_handling(pinterface);


   }


} // namespace user


