#include "framework.h"
#include "visual.h"


#include <math.h>

//#ifndef WINDOWS
//#include "freeimage/Source/FreeImage.h"
//#include "graphics/visual/visual_FreeImageFileProc.h"
//#endif

#include "visual_icon.cpp"
#include "visual_cursor.cpp"

#ifdef WINDOWSEX

#undef new
#define min MIN
#define max MAX
#include <gdiplus.h>
#undef min
#undef max
#define new AURA_NEW


//void fastblur(::draw2d::dib * pimg, int32_t radius);

#elif defined(METROWIN)
#include <wincodec.h>
#include <Shcore.h>

#endif



//O volume na unidade C n�o tem nome.
 //O N�mero de S�rie do Volume � 72AC-5E14

 //Pasta de C:\nordroid\app\appseed\axis\graphics\visual

//#include "visual_api.cpp"
//#include "visual_buffered_graphics.cpp"
//#include "visual_dib.cpp"
//#include "visual_fastblur.cpp"
//#include "visual_font.cpp"
///#include "visual_font_enumeration.cpp"
//#include "visual_font_list_data.cpp"
//#include "visual_font_department.cpp"
//#ifndef WINDOWS
//#include "visual_FreeImageFileProc.cpp"
//#endif
//#include "visual_glyph.cpp"
//#include "visual_glyph_set.cpp"
//#include "visual_graphics_extension.cpp"
//#include "visual_image_list.cpp"
//#include "visual_imaging.cpp"
//#include "visual_rgb_quad_array.cpp"
//#include "visual_visual.cpp"
//#include "visual_water_routine.cpp"
////2015-01-04  19:44                 0 _visual.cpp"
////              16 arquivo(s)        285.564 bytes
////               0 pasta(s)   19.197.042.688 bytes dispon�veis
//
//
//
//#include "antialias_8bit.cpp"
//#include "detect_8bit_borders.cpp"
//#include "freeimage.cpp"
//#include "load_gif_frame.cpp"
//
//
