#pragma once


namespace draw2d_cairo
{

   class CLASS_DECL_DRAW2D_CAIRO factory_exchange :
      virtual public ::object
   {
   public:
      factory_exchange(sp(base_application) papp);
      virtual ~factory_exchange();
   };


} // namespace draw2d_cairo

