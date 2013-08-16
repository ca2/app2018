#pragma once


#include "ca2.h"


#if defined(_CA2_MATH_DLL)
   #define CLASS_DECL_CA2_MATH  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_CA2_MATH  CLASS_DECL_IMPORT
#endif


#include "math/kissfft/kissfft.h"