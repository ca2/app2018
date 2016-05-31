#include "framework.h"


namespace ios
{

namespace ace
   {
   factory_exchange::factory_exchange(::ace::application * papp) :
      ::object(papp)
   {

      System.factory().cloneable_large < stdio_file           >   (System.type_info < ::file::text_buffer     > ());
      System.factory().cloneable_large < file                 >   (System.type_info < ::file::binary_buffer   > ());
//      System.factory().creatable_large < file_set             >   (System.type_info < ::file::set             > ());
      System.factory().cloneable < file_system                >   (System.type_info < ::file::system          > (), 1);
//    System.factory().cloneable_small < file_exception       >   (System.type_info < ::file::exception   > ());
//    System.factory().cloneable <    WinResource             >   (System.type_info < ::ca2::resource         > (), 1);
      System.factory().cloneable < application                >   (System.type_info < ::ace::application      > (), 1);
      System.factory().cloneable < dir                        >   (System.type_info < ::file::dir::system     > (), 1);
//    System.factory().cloneable_small < folder_watch         >   (System.type_info < ::ca2::folder_watch     > ());
//      System.factory().creatable < window_draw                >   (System.type_info < ::user::window_draw     > (), 1);
      System.factory().creatable_large < thread               >   (System.type_info < ::thread_impl                > ());
//      System.factory().creatable_large < window               >   (System.type_info < ::window          > ());
      System.factory().creatable < os                         >   (System.type_info < ::ace::os              > (), 1);
//      System.factory().creatable < port_forward               >   (System.type_info < ::net::port_forward     > (), 1);
//      System.factory().creatable < crypto                      >   (System.type_info < ::crypto::crypto           > (), 1);
//      System.factory().creatable < copydesk                   >   (System.type_info < ::core::copydesk        > (), 1);
  //    System.factory().creatable < ::ios2::printer                   >   (System.type_info < ::user::printer        > (), 1);

   }


   factory_exchange::~factory_exchange()
   {

   }

   } //   namespace ace

} // namespace ios



void __node_aura_factory_exchange(::ace::application * papp)
{

   ios::ace::factory_exchange factoryexchange(papp);

}



