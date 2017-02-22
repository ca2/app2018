#include "framework.h"
#include "macos.h"


namespace macos
{
namespace aura
{



   factory_exchange::factory_exchange(::aura::application * papp) :
      ::object(papp)
   {

      System.factory().cloneable_large < stdio_file           >   (System.type_info < ::file::text_file     > ());
      System.factory().cloneable_large < file                 >   (System.type_info < ::file::binary_file   > ());
      System.factory().cloneable_large < file                 >   (System.type_info < ::file::file   > ());
//      System.factory().cloneable < application                >   (System.type_info < ::aura::application     > (), 1);
       System.factory().creatable_large < ::thread               >   (System.type_info < ::thread           > ());
      System.factory().creatable < os                         >   (System.type_info < ::aura::os              > (), 1);
      System.factory().creatable < ansios::process            >   (System.type_info < ::process::process      > (), 1);
      System.factory().creatable < ansios::pipe               >   (System.type_info < ::process::pipe         > (), 1);
      System.factory().creatable < macos::dir                 >   (System.type_info < ::file::dir::system     > (), 1);
      System.factory().creatable < macos::file_system         >   (System.type_info < ::file::system          > (), 1);

   }


   factory_exchange::~factory_exchange()
   {

   }


} // namespace aura

} // namespace macos



void __node_aura_factory_exchange(::aura::application * papp)
{

   ::macos::aura::factory_exchange factoryexchange(papp);


}



