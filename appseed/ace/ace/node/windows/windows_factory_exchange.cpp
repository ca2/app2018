//#include "framework.h"
//#include "windows.h"


namespace ace
{


   namespace windows
   {


      factory_exchange::factory_exchange(::ace::application * papp):
         object(papp)
      {


         // axis will put things in the axis, (at least in this case... :-)
         System.factory().creatable_small    < ::windows::file_system   >(System.type_info < ::file::system              >(),1);
         System.factory().creatable_small    < ::windows::dir           >(System.type_info < ::file::dir::system         >(),1);

         System.factory().cloneable_large    < ::windows::stdio_file         >(System.type_info < ::file::text_buffer         >());
         System.factory().cloneable_large    < ::windows::file               >(System.type_info < ::file::binary_buffer       >(),true);
         System.factory().cloneable_large    < ::windows::file               >(System.type_info < ::file::stream_buffer       >(),true);
//         System.factory().creatable          < ::windows::application        >(System.type_info < ::ace::application         >(),1);
         System.factory().cloneable_small    < ::windows::folder_watch       >(System.type_info < ::file::folder_watch        >());
         //System.factory().creatable_large    < ::thread             >(System.type_info < ::thread               >());
         System.factory().creatable          < ::windows::os                 >(System.type_info < ::ace::os                  >(),1);
         System.factory().creatable          < ::windows::pipe               >(System.type_info < ::process::pipe             >(),32);
         System.factory().creatable          < ::windows::process            >(System.type_info < ::process::process          >(),32);

         System.factory().creatable_small    < ::windows::tts::speaker       >(System.type_info < ::tts::speaker              >());
         System.factory().creatable_small    < ::windows::console            >(System.type_info < ::console::window           >());



         ::windows::shell::theWindowsShell.Initialize();

      }


      factory_exchange::~factory_exchange()
      {

      }


   } // namespace windows


} // namespace ace


void __node_ace_factory_exchange(::ace::application * papp)
{

   ::ace::windows::factory_exchange factoryexchange(papp);

}



