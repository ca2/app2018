#pragma once


#include "aura/aura/aura/aura.h" // System.ftlibrary()


#if defined(LINUX)

#define USE_PANGO

#endif

#include "cairo-features.h"
#include "cairo.h"
#include "cairo-ft.h"
#ifdef WINDOWSEX
#include "cairo-win32.h"
#endif


mutex * cairo_mutex();
void init_cairo_mutex(::aura::application * papp);
void destroy_cairo_mutex();


#ifdef _DRAW2D_CAIRO_LIBRARY
#define CLASS_DECL_DRAW2D_CAIRO  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_DRAW2D_CAIRO  CLASS_DECL_IMPORT
#endif



#include "draw2d_cairo_factory_exchange.h"
#include "draw2d_cairo_keep.h"


#include "draw2d_cairo_object.h"
#include "draw2d_cairo_pen.h"
#include "draw2d_cairo_bitmap.h"
#include "draw2d_cairo_brush.h"
#include "draw2d_cairo_font.h"
#include "draw2d_cairo_palette.h"
#include "draw2d_cairo_region.h"
#include "draw2d_cairo_dib.h"
#include "draw2d_cairo_path.h"


#include "draw2d_cairo_graphics.h"


#pragma comment(lib, "Msimg32.lib")


