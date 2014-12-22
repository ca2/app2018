#pragma once


#ifndef CA2_APP_AXIS_AXIS_AXIS_AXIS_H
#define CA2_APP_AXIS_AXIS_AXIS_AXIS_H


#include "aura/aura/aura.h"





#ifdef _AXIS_STATIC
   #define CLASS_DECL_AXIS
#elif defined(_AXIS_LIBRARY)
   #define CLASS_DECL_AXIS  CLASS_DECL_EXPORT
   #define LIBIDN_BUILDING
#else
   #define CLASS_DECL_AXIS  CLASS_DECL_IMPORT
#endif


#include "axis/axis/os/os.h"

//#include "axis_number.h"


#ifdef AURA_LANGUAGE


#ifdef cplusplus


#include "axis_c.h"
#include "axis_c_cpp.h"


#else


BEGIN_EXTERN_C


#include "axis_c.h"


END_EXTERN_C


#endif


#else


#if defined(__M)


#include "axis_c.h"
#include "axis_c_objc.h"


#elif defined(__MM)


#include "axis_c.h"
#include "axis_c_objc.h"
#include "axis_c_objcpp.h"


#elif defined(__cplusplus_winrt)


#include "axis_c.h"
#include "axis_c_cpp.h"
#include "axis_c_winrt.h"


#else


#error "This is a not supported, hope, yet, non converging babylon language."


#endif


#endif


#define new AURA_NEW



#endif // CA2_APP_AXIS_AXIS_AXIS_AXIS_H




