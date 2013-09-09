#include "framework.h"


namespace draw2d_direct2d
{


   factory_exchange::factory_exchange(base_application * papp) :
      element(papp)
   {

      System.factory().cloneable_large < dib                   >  (System.type_info < ::draw2d::dib               > ());
      System.factory().cloneable_large < graphics              >  (System.type_info < ::draw2d::graphics          > ());
      System.factory().cloneable_large < graphics_path         >  (System.type_info < ::draw2d::path              > ());
      System.factory().cloneable_large < bitmap                >  (System.type_info < ::draw2d::bitmap            > ());
      System.factory().cloneable_large < pen                   >  (System.type_info < ::draw2d::pen               > ());
      System.factory().cloneable_large < brush                 >  (System.type_info < ::draw2d::brush             > ());
      System.factory().cloneable_large < palette               >  (System.type_info < ::draw2d::palette           > ());
      System.factory().cloneable_large < region                >  (System.type_info < ::draw2d::region            > ());
      System.factory().cloneable_large < font                  >  (System.type_info < ::draw2d::font              > ());
      

      draw2d_direct2d_initialize();

   }


   factory_exchange::~factory_exchange()
   {

   }


#if defined(METROWIN)

   void factory_exchange::draw2d_direct2d_initialize()
   {

   }

#endif


} // namespace draw2d_direct2d


void ca2_factory_exchange(sp(base_application) papp)
{

   draw2d_direct2d::factory_exchange factoryexchange(papp);

}



