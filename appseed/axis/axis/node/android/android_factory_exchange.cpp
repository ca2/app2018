#include "framework.h"


namespace android
{


   namespace axis
   {


      factory_exchange::factory_exchange(::aura::application * papp):
         element(papp)
      {

         System.factory().creatable_large < file_set              >(System. type_info < ::file::set          >());
         System.factory().creatable < file_system                 >(System. type_info < ::file::system       >(),1);
         System.factory().creatable < application                 >(System. type_info < ::aura::application  >(),1);
         System.factory().creatable < dir                         >(System. type_info < ::file::dir::system        >(),1);
         System.factory().creatable < window_draw                 >(System. type_info < ::user::window_draw        >(),1);
         System.factory().creatable_large < thread                >(System. type_info < ::thread             >());
         System.factory().creatable_large < interaction_impl      >(System. type_info < ::user::interaction_impl             >());
         System.factory().creatable < os                          >(System. type_info < ::axis::os                 >(),1);
         //System.factory().creatable < port_forward              >  (System. type_info < ::ca2::port_forward      > (), 1);
         System.factory().creatable_small < copydesk              >(System. type_info < ::axis::copydesk          >(),1);
         System.factory().creatable_small < crypto                >(System. type_info < ::crypto::crypto             >(),1);
         System.factory().creatable_small < ip_enum               >(System. type_info < ::net::ip_enum           >());

      }


      factory_exchange::~factory_exchange()
      {

      }


   } // namespace axis


} // namespace android


extern "C"
void __node_axis_factory_exchange(::aura::application * papp)
{

   ::android::axis::factory_exchange factoryexchange(papp);

}


