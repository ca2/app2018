#pragma once

namespace draw2d_opengl
{

   class CLASS_DECL_DRAW2D_OPENGL factory_exchange :
      virtual public ::object
   {
   public:
      factory_exchange(::aura::application * papp);
      virtual ~factory_exchange();
   };

} // namespace draw2d_opengl