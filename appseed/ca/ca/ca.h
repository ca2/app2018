#pragma once


#include "app/appseed/c/c.h"


#include "nodeapp/operational_system/operational_system.h"




#ifdef WINDOWS
#ifdef __CA__DLL
   #define CLASS_DECL_ca  _declspec(dllexport)
#else
   #define CLASS_DECL_ca  _declspec(dllimport)
#endif
#else
   #define CLASS_DECL_ca
#endif



namespace plane
{

   class application;


} // namespace plane


#undef App
#define planeApp(pcaapp) (*pcaapp->m_pappThis)
#define planeApplication (planeApp(m_papp))
#define App(pcaapp) planeApp(pcaapp)
#define Application (App(m_papp))



#if defined(WIN32) && !defined(VC6) && ! defined(VC71)
#include <vadefs.h>
#else
#include <stdarg.h>
#endif


#include "ca_base.h"


#pragma warning(disable: 4250)







extern "C"
{

   #include "ca/api/rotate.h"

} // extern "C"

#define rgba_get_r(rgba)      ((byte)(rgba & 0xff))
#define rgba_get_g(rgba)      ((byte)((rgba >> 8) & 0xff))
#define rgba_get_b(rgba)      ((byte)((rgba >> 16) & 0xff))
#define rgba_get_a(rgba)      ((byte)((rgba >> 24) & 0xff))

#ifdef WINDOWS
#include <crtdbg.h>
#else
#include "api/debug.h"
#endif


#if defined(MACOS)
#include "macos/macos.h"
#elif defined(LINUX)
#include "linux/os.h"
#endif


#include "ca/primitive/count.h"

#include "ca/api/muldiv64.h"

#include "ca/api/atol.h"

#include "ca/api/str.h"


#include "zlib/zconf.h"
#include "zlib/zlib.h"
#include "zlib/zutil.h"
#include "sqlite/sqlite.h"



#include "libcharguess/libcharguess.h"
