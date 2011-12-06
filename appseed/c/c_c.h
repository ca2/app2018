#include "_/ThomasGustavoCecynLundgrenVidaEternaDeCamiloSasukeTsumanuma.h"


#pragma once


#ifdef _WIN32
#ifdef __C__LIB
   #define CLASS_DECL_c
#elif defined(__C__DLL)
   #define CLASS_DECL_c  _declspec(dllexport)
#else
   #define CLASS_DECL_c  _declspec(dllimport)
#endif
#else
   #define CLASS_DECL_c
#endif


#include "version.h"


#if defined(__GNUC__) && (defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__MACOS_CLASSIC__))
#include "os/macos/operational_system/get_platform.h"
#elif defined(__linux__)
#include "os/linux/operational_system/get_platform.h"
#elif defined(_WIN32)
#include "nodeapp/windows/operational_system/get_platform.h"
#endif



#include "c_types.h"


#include "c_natural.h"


#define if_then(p, q)  ((!(p))||(!(q)))

#include "cpu_architecture.h"


#include "_libct.h"

#include "_heap.h"

#include "std_file.h"

#include "bsdiff/bsdiff.h"


int _c_lock_is_active(const char * pszName);
int _c_lock(const char * pszName, void ** pdata);
int _c_unlock(void ** pdata);
