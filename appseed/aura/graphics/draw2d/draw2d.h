#pragma once


#include "draw2d_fwd.h"
#include "draw2d_const.h"


namespace draw2d
{

   CLASS_DECL_AURA void copy_colorref(int cx,int cy,COLORREF * pcolorrefDst,int iStrideDst,COLORREF * pcolorrefSrc,int iStrideSrc);


} // namespace draw2d

#if defined(CUBE)


void draw2d_factory_exchange(::aura::application * papp);


#endif









#include "draw2d_object.h"
#include "draw2d_pen.h"
#include "draw2d_bitmap.h"
#include "draw2d_brush.h"
#include "draw2d_font.h"
#include "draw2d_palette.h"
#include "draw2d_region.h"
#include "draw2d_igui.h"
#include "draw2d_dib.h"
//#include "ca2_draw_dib.h"
#include "draw2d_path.h"


namespace aura
{


   class str_context;


}


#include "aura/aura/aura/aura_draw_context.h"
#include "draw2d_graphics.h"






