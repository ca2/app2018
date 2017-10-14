#include "framework.h"


#include "antialias_8bit.cpp"
#include "detect_8bit_borders.cpp"
#ifdef SMALLCODE
#if defined(WINDOWSEX)
#include "visual_gdiplus_image.cpp"
#include "visual_gdiplus_image_gif.cpp"
#include "visual_windows_image.cpp"
#endif
#else
#include "visual_freeimage_image.cpp"
#include "visual_freeimage.cpp"
#endif
#include "load_gif_frame.cpp"
#include "visual_api.cpp"
#include "visual_buffered_graphics.cpp"
#include "visual_dib.cpp"
#include "visual_fastblur.cpp"
#include "visual_font.cpp"
#include "visual_font_department.cpp"
#include "visual_font_enumeration.cpp"
#include "visual_font_list_data.cpp"
#include "visual_glyph.cpp"
#include "visual_glyph_set.cpp"
#include "visual_graphics_extension.cpp"
#include "visual_image_list.cpp"
#include "visual_imaging.cpp"
#include "visual_rgb_quad_array.cpp"
#include "visual_visual.cpp"
#include "visual_water_routine.cpp"


