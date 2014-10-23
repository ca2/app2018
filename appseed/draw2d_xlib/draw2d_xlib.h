#ifndef BASE_DRAW2D_XLIB_h
#define BASE_DRAW2D_XLIB_h


#pragma once


#include "app/appseed/base/base/base/base.h"


class window;


#ifdef _DRAW2D_CAIRO_LIBRARY
#define CLASS_DECL_DRAW2D_XLIB  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_DRAW2D_XLIB  CLASS_DECL_IMPORT
#endif


mutex & xlib_mutex();
void init_xlib_mutex(sp(::aura::application) papp);


#include "draw2d_xlib_factory_exchange.h"
#include "draw2d_xlib_keep.h"


#include "draw2d_xlib_color.h"


#include "draw2d_xlib_object.h"
#include "draw2d_xlib_pen.h"
#include "draw2d_xlib_bitmap.h"
#include "draw2d_xlib_brush.h"
#include "draw2d_xlib_font.h"
#include "draw2d_xlib_palette.h"
#include "draw2d_xlib_region.h"
#include "draw2d_xlib_dib.h"
#include "draw2d_xlib_path.h"


#include "draw2d_xlib_graphics.h"


#pragma comment(lib, "Msimg32.lib")


#endif // BASE_DRAW2D_XLIB_h










