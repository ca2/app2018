#include "framework.h"
#include "metrowin.h"


namespace metrowin
{

   namespace axis
   {

      factory_exchange::factory_exchange(::aura::application * papp):
         ::object(papp)
      {

//         System.factory().creatable_small < file_set              >(System.template type_info < ::file::set                 >());
         System.factory().cloneable       < axis::file_system     >(System.template type_info < ::file::system              >(),1);
         System.factory().cloneable       < axis::dir             >(System.template type_info < ::file::dir::system         >(),1);
         System.factory().cloneable_small < folder_watch          >(System.template type_info < ::file::folder_watch        >());
         System.factory().creatable       < port_forward          >(System.template type_info < ::net::port_forward         >(),1);
         System.factory().creatable_small < crypto                >(System.template type_info < ::crypto::crypto            >(),1);
         System.factory().creatable_small < ip_enum               >(System.template type_info < ::net::ip_enum              >());
//         System.factory().creatable       < window_draw              >(System.template type_info < ::user::window_draw         >(),1);
  //       System.factory().creatable_large < interaction_impl         >(System.template type_info < ::user::interaction_impl    >());
         System.factory().creatable_small < copydesk                 >(System.template type_info < ::user::copydesk            >(),1);
         //      System.factory().creatable       < printer               >  (System.template type_info < ::user::printer             > ()  , 1);

    //     System.factory().creatable          < ::windows::message_queue      >(System.type_info < ::user::message_queue       >(),32);

      }


      factory_exchange::~factory_exchange()
      {
      }


   } // namespace axis


} // namespace metrowin



void __node_axis_factory_exchange(::aura::application * papp)
{

   ::metrowin::axis::factory_exchange factoryexchange(papp);

}



