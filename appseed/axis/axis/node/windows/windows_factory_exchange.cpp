#include "axis/user/user.h"


namespace axis
{


   namespace windows
   {


      factory_exchange::factory_exchange(sp(::aura::application) papp):
         element(papp)
      {

         System.factory().creatable          < ::windows::window_draw        >(System.type_info < ::user::window_draw         >(),1);
         System.factory().creatable_large    < ::windows::interaction_impl   >(System.type_info < ::user::interaction_impl    >());
         System.factory().creatable_small    < ::windows::copydesk           >(System.type_info < ::axis::copydesk            >(),1);
         System.factory().creatable_large    < ::windows::print_job          >(System.type_info < ::user::print_job           >());
         System.factory().creatable          < ::windows::message_queue      >(System.type_info < ::aura::message_queue       >(), 32);

         ::windows::shell::theWindowsShell.Initialize();

      }


      factory_exchange::~factory_exchange()
      {

      }


   } // namespace windows


} // namespace axis


void __node_axis_factory_exchange(sp(::aura::application) papp)
{

   ::axis::windows::factory_exchange factoryexchange(papp);

}




