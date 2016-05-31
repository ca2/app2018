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


#include "ace/ace/ace/ace_cpu_architecture.h"


BEGIN_EXTERN_C
extern CLASS_DECL_ACE int g_bAura;
CLASS_DECL_ACE int __assert_failed_line(const char * lpszFileName,int iLineNumber);
CLASS_DECL_ACE int is_debugger_attached();
CLASS_DECL_ACE void debug_print(const char * psz,...);

END_EXTERN_C

CLASS_DECL_ACE int throw_assert_exception(const char * lpszFileName,int iLineNumber);

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
#define ASSERT_VALID(pOb)  ::assert_valid_object(pOb, __FILE__, __LINE__)
#else
#define ASSERT(f)
#define _ASSUME(cond)
#if defined(ANDROID) || defined(APPLEOS)
#define ASSERT_VALID(cond)
#else
#define ASSERT_VALID(cond) __noop;
#endif
#endif


#endif

#define low_byte(w)              ((BYTE)((w) & 0xff))

#ifdef VSNORD
#define argb_get_b_value(rgb)    (low_byte(rgb))
#define argb_get_g_value(rgb)    (low_byte((rgb)>>8))
#define argb_get_r_value(rgb)    (low_byte((rgb)>>16))
#define argb_get_a_value(rgb)    (low_byte((rgb)>>24))
#define RGBA(r, g, b, a)         ((COLORREF)((low_byte(b)|((WORD)(low_byte(g))<<8))|(((uint32_t)low_byte(r))<<16)|(((uint32_t)low_byte(a))<<24)))
#define ARGB(a, r, g, b)         RGBA(r, g, b, a)
#else
#define argb_get_r_value(rgb)    (low_byte(rgb))
#define argb_get_g_value(rgb)    (low_byte((rgb)>>8))
#define argb_get_b_value(rgb)    (low_byte((rgb)>>16))
#define argb_get_a_value(rgb)    (low_byte((rgb)>>24))
#define RGBA(r, g, b, a)         ((COLORREF)((low_byte(r)|((WORD)(low_byte(g))<<8))|(((uint32_t)low_byte(b))<<16)|(((uint32_t)low_byte(a))<<24)))
#define ARGB(a, r, g, b)         RGBA(r, g, b, a)
#endif

#define _(c_string_to_be_translated_from_english) c_string_to_be_translated_from_english

// risky
#ifdef __cpluplus
inline int void_ptr_is_null(const void * p)
{
#if defined(ANDROID) || defined(APPLE_IOS)
   return ((INT_PTR) p) == 0;
#else
   return ((INT_PTR)p) < 4096;
#endif
}
#else
static int void_ptr_is_null(const void * p)
{
#if defined(ANDROID) || defined(APPLE_IOS)
   return ((INT_PTR)p) == 0;
#else
   return ((INT_PTR)p) < 4096;
#endif
}
#endif

#include "ace/primitive/math/math_mkint_c.h"

#if defined(ANDROID)
#include "ace/ace/os/android/android.h"
#elif defined(MACOS)
#include "ace/ace/os/macos/macos.h"
#elif defined(LINUX)
#include "ace/ace/os/linux/linux.h"
#elif defined(METROWIN)
#include "ace/ace/os/metrowin/metrowin.h"
#elif defined(WINDOWS)
#include "ace/ace/os/windows/windows.h"
#elif defined(SOLARIS)
#include "ace/ace/os/solaris/solaris.h"
#elif defined(APPLE_IOS)
#include "ace/ace/os/ios/ios.h"
#else
#error Not Supported Operational System
#endif

#undef MIN
#undef MAX

#define INTABS(i) (((i) >= 0) ? (i) : (-i))
#define FLOATABS(f) (((f) >= 0.f) ? (f) : (-f))
#define DOUBLEABS(d) (((d) >= 0.0) ? (d) : (-d))
#ifdef __cplusplus
#define MAX(a, b) ((compare::gt((a), (b))) ? (a) : (b))
#define MIN(a, b) ((compare::lt((a), (b))) ? (a) : (b))
#include "ace/primitive/comparison/compare.h"
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


CLASS_DECL_ACE int get_ace_init();
CLASS_DECL_ACE int_bool defer_ace_init();
CLASS_DECL_ACE int_bool defer_ace_term();

BEGIN_EXTERN_C

typedef struct rdp_freerdp freerdp;

CLASS_DECL_ACE int_bool freerdp_authenticate(void * instance, char** username,char** password,char** domain, const char * pszServerName, int bInteractive);


#if !defined(O_BINARY) && !defined(WINDOWS)
# define O_BINARY 0
#endif

CLASS_DECL_ACE int32_t ansi_open(const char * psz,int32_t i);
CLASS_DECL_ACE FILE * ansi_fopen(const char * psz,const char * pszMode);
CLASS_DECL_ACE int ansi_file_flag(int iFlag);
CLASS_DECL_ACE void ansi_get_errno(int32_t * perrno);
CLASS_DECL_ACE void ansi_unlink(const char * psz);

//CLASS_DECL_ACE int module_name_get_count(const char * pszModuleName);

END_EXTERN_C



#include "ace/primitive/datetime/datetime_mktime.h"
#include "ace/primitive/api/api.h"


BEGIN_EXTERN_C

CLASS_DECL_ACE extern uint32_t g_dwStartTime;

END_EXTERN_C










