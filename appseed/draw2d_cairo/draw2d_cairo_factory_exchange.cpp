#include "framework.h"


namespace draw2d_cairo
{


   factory_exchange::factory_exchange(::aura::application * papp) :
      element(papp)
   {

      init_cairo_mutex(papp);

      System.factory().cloneable_large < dib                   >  (System.type_info < ::draw2d::dib                 > ());
      System.factory().cloneable_large < graphics              >  (System.type_info < ::draw2d::graphics            > ());
      System.factory().cloneable_large < bitmap                >  (System.type_info < ::draw2d::bitmap              > ());
      System.factory().cloneable_large < pen                   >  (System.type_info < ::draw2d::pen                 > ());
      System.factory().cloneable_large < brush                 >  (System.type_info < ::draw2d::brush               > ());
      System.factory().cloneable_large < palette               >  (System.type_info < ::draw2d::palette             > ());
      System.factory().cloneable_large < region                >  (System.type_info < ::draw2d::region              > ());
      System.factory().cloneable_large < font                  >  (System.type_info < ::draw2d::font                > ());
      System.factory().cloneable_large < path                  >  (System.type_info < ::draw2d::path                > ());

   }


   factory_exchange::~factory_exchange()
   {

   }


} // namespace win


extern "C"
void ca2_factory_exchange(::aura::application * papp)
{
   draw2d_cairo::factory_exchange factoryexchange(papp);
}


