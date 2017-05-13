#pragma once 




///#include "visual_const.h"
//#include "visual_icon.h"
//#include "visual_cursor.h"


#include "visual_dib.h"
#include "visual_fastblur.h"
#include "visual_rgb_quad_array.h"
#include "visual_imaging.h"
#include "visual_image_list.h"
#include "visual_water_routine.h"

#include "visual_api.h"
#include "visual_graphics_extension.h"
#include "visual_glyph.h"
#include "visual_glyph_set.h"
#include "visual_font.h"
#include "visual_font_enumeration.h"
#include "visual_font_list_data.h"
#include "visual_font_department.h"
#include "visual_dib_pal.h"

#include "visual_buffered_graphics.h"


#include "visual_visual.h"




CLASS_DECL_AXIS bool TextOutU(HDC hdc, int32_t x, int32_t y, const char * lpString, int32_t ca);
CLASS_DECL_AXIS bool GetTextExtentPoint32U(HDC hdc, const char * lpString, int32_t ca, LPSIZE psizl);
CLASS_DECL_AXIS int32_t  DrawTextU(HDC hdc, const char * lpchText, int32_t cchText, LPRECT lprc, UINT format);

