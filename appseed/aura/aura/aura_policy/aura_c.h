/////////////////////////////////////////////////////////////////////////////////////////
//
//   All Pure and Aural ca2 and God and Gods and Goddssesses and devils's blessings
//
//   C - C++0 - C++ pureness
//
//   C => in ca2 scope
//
//   Thank you God for C pureness!! Cheap.
//


#pragma once


#include "aura/aura/aura/aura_cpu_architecture.h"


#include "aura/aura/message/message_global.h"

BEGIN_EXTERN_C


CLASS_DECL_AURA void windowing_output_debug_string(const char * pszDebugString);


CLASS_DECL_AURA void c_function_call(void * p);

extern CLASS_DECL_AURA int g_bAura;
CLASS_DECL_AURA int __assert_failed_line(const char * lpszFileName,int iLineNumber);
CLASS_DECL_AURA int is_debugger_attached();
CLASS_DECL_AURA void debug_print(const char * psz,...);

END_EXTERN_C

CLASS_DECL_AURA int throw_assert_exception(const char * lpszFileName,int iLineNumber);

#ifndef __cplusplus

//#include <assert.h>

#define ASSERT assert

#else
#ifdef DEBUG

//#define ASSERT(f)          DEBUG_ONLY(() ((f) || !::__assert_failed_line(THIS_FILE, __LINE__) || (debug_break(), 0)))
#define ASSERT(f)          ((void) ((f) || (is_debugger_attached() && !::__assert_failed_line(__FILE__, __LINE__) && (::debug_break(), 0)) || (!is_debugger_attached() && (throw_assert_exception(__FILE__, __LINE__), 0))))
/* see core headers for commentary on this */
/* We use the name _ASSUME to avoid name clashes */
#define _ASSUME(cond)       do { bool _gen__condVal=!!(cond); ASSERT(_gen__condVal); __analysis_assume(_gen__condVal); } while(0)
//#define ASSERT_VALID(pOb)  DEBUG_ONLY((::assert_valid_object(pOb, THIS_FILE, __LINE__)))
#define ASSERT_VALID(pOb)  ::__assert_valid_object(pOb, __FILE__, __LINE__)
#else
#define ASSERT(f)
#define _ASSUME(cond)
#if defined(ANDROID)
#define ASSERT_VALID(cond)
#elif defined(APPLEOS)
#define ASSERT_VALID(cond)
#else
#define ASSERT_VALID(cond) __noop;
#endif
#endif


#endif

#define low_byte(w)              ((BYTE)((w) & 0xff))

#ifdef VSNORD
//#ifdef __arm__
#define argb_get_b_value(rgb)    (low_byte(rgb))
#define argb_get_g_value(rgb)    (low_byte((rgb)>>8))
#define argb_get_r_value(rgb)    (low_byte((rgb)>>16))
#define argb_get_a_value(rgb)    (low_byte((rgb)>>24))
//#define RGBA(r, g, b, a)         ((COLORREF)((low_byte(b)|((WORD)(low_byte(g))<<8))|(((uint32_t)low_byte(r))<<16)|(((uint32_t)low_byte(a))<<24)))
//#define ARGB(a, r, g, b)         RGBA(r, g, b, a)
#define RGBA(r, g, b, a)         ((COLORREF)((low_byte(r)|((WORD)(low_byte(g))<<8))|(((uint32_t)low_byte(b))<<16)|(((uint32_t)low_byte(a))<<24)))
#define ARGB(a, r, g, b)         RGBA(r, g, b, a)
#define REASSEMBLE_ARGB(a, r, g, b)      ARGB(a, b, g, r)
//#else
//#define argb_get_b_value(rgb)    (low_byte(rgb))
//#define argb_get_g_value(rgb)    (low_byte((rgb)>>8))
//#define argb_get_r_value(rgb)    (low_byte((rgb)>>16))
//#define argb_get_a_value(rgb)    (low_byte((rgb)>>24))
//#define RGBA(r, g, b, a)         ((COLORREF)((low_byte(r)|((WORD)(low_byte(g))<<8))|(((uint32_t)low_byte(b))<<16)|(((uint32_t)low_byte(a))<<24)))
//#define ARGB(a, r, g, b)         RGBA(r, g, b, a)
//#endif
#else
#define argb_get_r_value(rgb)    (low_byte(rgb))
#define argb_get_g_value(rgb)    (low_byte((rgb)>>8))
#define argb_get_b_value(rgb)    (low_byte((rgb)>>16))
#define argb_get_a_value(rgb)    (low_byte((rgb)>>24))
#define RGBA(r, g, b, a)         ((COLORREF)((low_byte(r)|((WORD)(low_byte(g))<<8))|(((uint32_t)low_byte(b))<<16)|(((uint32_t)low_byte(a))<<24)))
#define ARGB(a, r, g, b)         RGBA(r, g, b, a)
#define REASSEMBLE_ARGB(a, r, g, b)      ARGB(a, r, g, b)
#endif

CLASS_DECL_AURA int is_ptr_null(const void * p, size_t s);

// risky
//#ifdef __cpluplus
//inline int void_ptr_is_null(const void * p)
//{
//#if defined(ANDROID) || defined(APPLE_IOS)
//   return ((INT_PTR) p) == 0;
//#else
//   return ((INT_PTR)p) < 4096;
//#endif
//}
//#else
//static int void_ptr_is_null(const void * p)
//{
//#if defined(ANDROID) || defined(APPLE_IOS)
//   return ((INT_PTR)p) == 0;
//#else
//   return ((INT_PTR)p) < 4096;
//#endif
//}
//#endif


#define void_ptr_is_null(p) (((UINT_PTR)(void *) p) < 4096)

#ifdef __cpluplus
template < typename T >
inline int type_is_null(const T * p)
{
   return (((UINT_PTR)(void *)p) < MAX(4096, sizeof(T));
}
#endif

#include "aura/primitive/math/math_mkint_c.h"

       // C-includes
       #include "aura/aura/os/os.h"


#undef MIN
#undef MAX

#define INTABS(i) (((i) >= 0) ? (i) : (-i))
#define FLOATABS(f) (((f) >= 0.f) ? (f) : (-f))
#define DOUBLEABS(d) (((d) >= 0.0) ? (d) : (-d))
#ifdef __cplusplus
#define MAX(a, b) ((comparison::gt((a), (b))) ? (a) : (b))
#define MIN(a, b) ((comparison::lt((a), (b))) ? (a) : (b))
#include "aura/primitive/comparison/compare.h"
#else
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#define LIM(a, min, max) MIN(max, MAX(min, a))
#define SORT_LIM(x,minmax,maxmin) ((minmax) < (maxmin) ? LIM(x,minmax,maxmin) : LIM(x,maxmin,minmax))
#define CLIP_USHRT(x) LIM(x,0,USHRT_MAX)

#ifdef WINDOWS
#include <intsafe.h>
#else
#ifndef LODWORD
#define LODWORD(l)           ((uint32_t)(((uint64_t)(l)) & 0xffffffff))
#endif
#ifndef HIDWORD
#define HIDWORD(l)           ((uint32_t)((((uint64_t)(l)) >> 32) & 0xffffffff))
#endif
#endif

#define MAKELONG64(a, b)      (((uint64_t)(((uint32_t)(((uint64_t)(a)) & 0xffffffff)) | ((uint64_t)((uint32_t)(((uint64_t)(b)) & 0xffffffff))) << 32)))


#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int32_t)(int16_t)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int32_t)(int16_t)HIWORD(lp))
#endif


#define GET_X_LPARAM64(lp)                        ((int32_t)(int16_t)LODWORD(lp))
#define GET_Y_LPARAM64(lp)                        ((int32_t)(int16_t)HIDWORD(lp))


       CLASS_DECL_AURA int get_aura_init();
CLASS_DECL_AURA int_bool defer_aura_init();
CLASS_DECL_AURA int_bool defer_aura_term();

BEGIN_EXTERN_C

typedef struct rdp_freerdp freerdp;

CLASS_DECL_AURA int_bool freerdp_authenticate(void * instance, char** username,char** password,char** domain, const char * pszServerName, int bInteractive);


#if !defined(O_BINARY) && !defined(WINDOWS)
# define O_BINARY 0
#endif

CLASS_DECL_AURA int32_t ansi_open(const char * psz,int32_t i);
CLASS_DECL_AURA FILE * ansi_fopen(const char * psz,const char * pszMode);
CLASS_DECL_AURA int ansi_file_flag(int iFlag);
CLASS_DECL_AURA void ansi_get_errno(int32_t * perrno);
CLASS_DECL_AURA void ansi_unlink(const char * psz);

//CLASS_DECL_AURA int module_name_get_count(const char * pszModuleName);


CLASS_DECL_AURA DWORD get_last_error();
CLASS_DECL_AURA void set_last_error(DWORD dw);

int32_t
CLASS_DECL_AURA
MultiByteToWideChar2(
UINT     CodePage,
DWORD    dwFlags,
LPCSTR   lpMultiByteStr,
int32_t      cbMultiByte,
LPWSTR  lpWideCharStr,
int32_t      cchWideChar);

//xxx linux WINAXISAPI
int32_t
CLASS_DECL_AURA
WideCharToMultiByte2(
UINT     CodePage,
DWORD    dwFlags,
LPCWSTR  lpWideCharStr,
int32_t      cchWideChar,
LPSTR   lpMultiByteStr,
int32_t      cbMultiByte,
LPCSTR   lpDefaultChar,
LPBOOL  lpUsedDefaultChar);


END_EXTERN_C
           
           
#if !defined(APPLEOS)
int ftruncate(int file, file_size_t len);
#endif

CLASS_DECL_AURA int32_t get_os_thread_priority(int32_t iCa2Priority);
CLASS_DECL_AURA int32_t get_os_priority_class(int32_t iCa2Priority);
CLASS_DECL_AURA int32_t get_os_thread_scheduling_priority(int32_t iCa2Priority);
CLASS_DECL_AURA int32_t get_os_class_scheduling_priority(int32_t iCa2Priority);




#include "aura/primitive/datetime/datetime_mktime.h"
#include "aura/primitive/api/api.h"


//#include "aura/charguess/charguess_c.h"

#include "aura/multimedia/multimedia_c.h"



BEGIN_EXTERN_C

CLASS_DECL_AURA extern uint32_t g_dwStartTime;

END_EXTERN_C











