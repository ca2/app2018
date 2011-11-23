#include "StdAfx.h"

namespace win
{

   factory_exchange::factory_exchange(::ca::application * papp) :
      ca(papp)
   {
      System.factory().cloneable_large < WinStdioFile         >  (::ca::get_type_info < ::ex1::text_file         > ());
      System.factory().cloneable_large < WinFile              >  (::ca::get_type_info < ::ex1::file              > ());
      System.factory().cloneable_large < WinFileSet           >  (::ca::get_type_info < ::ex2::file_set          > ());
      System.factory().cloneable < WinFileSystem        >  (::ca::get_type_info < ::ex1::file_system       > (), 1);
      System.factory().cloneable_small < WinFileException     >  (::ca::get_type_info < ::ex1::file_exception    > ());
      System.factory().cloneable < WinResource          >  (::ca::get_type_info < ::ex1::resource          > (), 1);
      System.factory().cloneable < application          >  (::ca::get_type_info < ::ex2::application       > (), 1);
      System.factory().cloneable < dir                  >  (::ca::get_type_info < ::ca::dir::system             > (), 1);
      System.factory().cloneable_small < folder_watch         >  (::ca::get_type_info < ::ex2::folder_watch      > ());
      System.factory().cloneable_large < dib                  >  (::ca::get_type_info < ::ca::dib                > ());
      System.factory().cloneable_large < graphics             >  (::ca::get_type_info < ::ca::graphics           > ());
      System.factory().cloneable_large < bitmap               >  (::ca::get_type_info < ::ca::bitmap             > ());
      System.factory().cloneable_large < pen                  >  (::ca::get_type_info < ::ca::pen                > ());
      System.factory().cloneable_large < brush                >  (::ca::get_type_info < ::ca::brush              > ());
      System.factory().cloneable_large < palette              >  (::ca::get_type_info < ::ca::palette            > ());
      System.factory().cloneable_large < region                  >  (::ca::get_type_info < ::ca::region                > ());
      System.factory().cloneable_large < font                 >  (::ca::get_type_info < ::ca::font               > ());
      System.factory().creatable < window_draw          >  (::ca::get_type_info < ::ca::window_draw        > (), 1);
      System.factory().creatable_large < thread               >  (::ca::get_type_info < ::ca::thread             > ());
      System.factory().creatable_large < window               >  (::ca::get_type_info < ::ca::window             > ());
      System.factory().cloneable_small < draw_dib             >  (::ca::get_type_info < ::ca::draw_dib           > ());
      System.factory().creatable < osi                  >  (::ca::get_type_info < ::ca::osi                > (), 1);
      System.factory().creatable < port_forward         >  (::ca::get_type_info < ::ca4::port_forward             > (), 1);
      ::win::dib::s_initialize();
   }

   factory_exchange::~factory_exchange()
   {
   }

} // namespace win



void ca2_factory_exchange(::ca::application * papp)
{
   win::factory_exchange factoryexchange(papp);
}