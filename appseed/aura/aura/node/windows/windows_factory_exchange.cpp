#include "framework.h"
//#include "windows.h"


namespace aura
{


   namespace windows
   {


      factory_exchange::factory_exchange(::aura::application * papp):
         object(papp)
      {


         // axis will put things in the axis, (at least in this case... :-)
         System.factory().creatable_small    < ::windows::file_system   >(System.type_info < ::file::system              >(),1);
         System.factory().creatable_small    < ::windows::dir           >(System.type_info < ::file::dir::system         >(),1);

         System.factory().cloneable_large    < ::windows::stdio_file         >(System.type_info < ::file::text_file         >());
         System.factory().cloneable_large    < ::windows::file               >(System.type_info < ::file::binary_file       >(),true);
         System.factory().cloneable_large    < ::windows::file               >(System.type_info < ::file::file       >(),true);
//         System.factory().creatable          < ::windows::application        >(System.type_info < ::aura::application         >(),1);
         System.factory().cloneable_small    < ::windows::folder_watch       >(System.type_info < ::file::folder_watch        >());
         //System.factory().creatable_large    < ::thread             >(System.type_info < ::thread               >());
         System.factory().creatable          < ::windows::os                 >(System.type_info < ::aura::os                  >(),1);
         System.factory().creatable          < ::windows::pipe               >(System.type_info < ::process::pipe             >(),32);
         System.factory().creatable          < ::windows::process            >(System.type_info < ::process::process          >(),32);

         System.factory().creatable_small    < ::windows::tts::speaker       >(System.type_info < ::tts::speaker              >());
         System.factory().creatable_small    < ::windows::console            >(System.type_info < ::console::window           >());
         System.factory().creatable_small < ::windows::crypto                   >(System.type_info < ::crypto::crypto       >(), 1);
         System.factory().creatable_small < ::windows::ip_enum                  >(System.type_info < ::net::ip_enum         >());



         ::windows::shell::theWindowsShell.Initialize();

      }


      factory_exchange::~factory_exchange()
      {

      }


   } // namespace windows


} // namespace aura


void __node_aura_factory_exchange(::aura::application * papp)
{

   ::aura::windows::factory_exchange factoryexchange(papp);

}




