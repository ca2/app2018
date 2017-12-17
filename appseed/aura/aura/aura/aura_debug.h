﻿#pragma once


#ifndef __AXIS_AXIS_DEBUG_H__
#define __AXIS_AXIS_DEBUG_H__





#define _NORMAL_BLOCK 1


CLASS_DECL_AURA int32_t DECL_C debug_report(
int32_t _ReportType,
const char * _Filename,
int32_t _LineNumber,
const char * _ModuleName,
const char * _Format,
...);


CLASS_DECL_AURA int32_t DECL_C debug_report(
int32_t _ReportType,
const wchar_t * _Filename,
int32_t _LineNumber,
const wchar_t * _ModuleName,
const wchar_t * _Format,
...);


//////////////////////////////////////////////////////////////////////////////////
//
//    Debug Reporting
//
typedef void * _HFILE; /* file handle pointer */

#define _CRT_WARN           0
#define _CRT_ERROR          1
#define _CRT_ASSERT         2
#define _CRT_ERRCNT         3

#define _CRTDBG_MODE_FILE      0x1
#define _CRTDBG_MODE_DEBUG     0x2
#define _CRTDBG_MODE_WNDW      0x4
#define _CRTDBG_REPORT_MODE    -1

#ifndef WINDOWS

#if defined(_M_IX86)
#define _CRTDBG_INVALID_HFILE ((_HFILE)-1)
#define _CRTDBG_HFILE_ERROR   ((_HFILE)-2)
#define _CRTDBG_FILE_STDOUT   ((_HFILE)-4)
#define _CRTDBG_FILE_STDERR   ((_HFILE)-5)
#define _CRTDBG_REPORT_FILE   ((_HFILE)-6)
#else
#define _CRTDBG_INVALID_HFILE ((_HFILE)(int64_t)-1)
#define _CRTDBG_HFILE_ERROR   ((_HFILE)(int64_t)-2)
#define _CRTDBG_FILE_STDOUT   ((_HFILE)(int64_t)-4)
#define _CRTDBG_FILE_STDERR   ((_HFILE)(int64_t)-5)
#define _CRTDBG_REPORT_FILE   ((_HFILE)(int64_t)-6)
#endif

#endif


#define _CRT_RPTHOOK_INSTALL  0
#define _CRT_RPTHOOK_REMOVE   1


#define _CRTDBG_ALLOC_MEM_DF        0x01  /* Turn on debug allocation */
#define _CRTDBG_DELAY_FREE_MEM_DF   0x02  /* Don't actually free memory */
#define _CRTDBG_CHECK_ALWAYS_DF     0x04  /* Check heap every alloc/dealloc */
#define _CRTDBG_RESERVED_DF         0x08  /* Reserved - do not use */
#define _CRTDBG_CHECK_CRT_DF        0x10  /* Leak check/diff CRT blocks */
#define _CRTDBG_LEAK_CHECK_DF       0x20  /* Leak check at program exit */

/*
 * Some bit values for _crtDbgFlag which correspond to frequencies for checking
 * the the heap.
 */
#define _CRTDBG_CHECK_EVERY_16_DF   0x00100000  /* check heap every 16 heap ops */
#define _CRTDBG_CHECK_EVERY_128_DF  0x00800000  /* check heap every 128 heap ops */
#define _CRTDBG_CHECK_EVERY_1024_DF 0x04000000  /* check heap every 1024 heap ops */
/*
We do not check the heap by default at this point because the cost was too high
for some applications. You can still turn this feature on manually.
 */
#define _CRTDBG_CHECK_DEFAULT_DF    0

#define _CRTDBG_REPORT_FLAG         -1    /* Query bitflag status */

#define _BLOCK_TYPE(block)          (block & 0xFFFF)
#define _BLOCK_SUBTYPE(block)       (block >> 16 & 0xFFFF)


/****************************************************************************
 *
 * Memory state
 *
 ***************************************************************************/

/* Memory block identification */
#define _FREE_BLOCK      0
#define _NORMAL_BLOCK    1
#define _CRT_BLOCK       2
#define _IGNORE_BLOCK    3
#define _CLIENT_BLOCK    4
#define _MAX_BLOCKS      5



struct MEMORY_BLOCK_HEADER;

typedef struct _MEMORY_STATE
{

   struct MEMORY_BLOCK_HEADER * pBlockHeader;
   size_t lCounts[_MAX_BLOCKS];
   size_t lSizes[_MAX_BLOCKS];
   size_t lHighWaterCount;
   size_t lTotalCount;

} MEMORY_STATE;


/****************************************************************************
 *
 * Declarations, prototype and function-like macros
 *
 ***************************************************************************/

/* _STATIC_ASSERT is for enforcing boolean/integral conditions at compile time.
   Since it is purely a compile-time mechanism that generates no code, the check
   is left in even if DEBUG is not defined. */

#ifndef _STATIC_ASSERT
#define _STATIC_ASSERT(expr) typedef char __static_assert_t[ (expr) ]
#endif

#ifndef DEBUG

////////////////////////////////////////////////////////////////////////////////////
//
//    Debug OFF


////////////////////////////////////////////////////////////////////////////////////
//
//  We allow our basic _ASSERT macros to be overridden by pre-existing definitions.
//    This is not the ideal mechanism, but is helpful in some scenarios and helps avoid
//    multiple definition problems

#ifndef _ASSERT
#define _ASSERT(expr) ((void)0)
#endif

#ifndef _ASSERTE
#define _ASSERTE(expr) ((void)0)
#endif

#ifndef _ASSERT_EXPR
#define _ASSERT_EXPR(expr, expr_str) (()0)
#endif

//
//  DEBUG Off
//
/////////////////////////////


#elif !defined(__MCRTDBG) && !defined(__VLD)


/////////////////////////////
//
//  DEBUG ON
//


///////////////////////////////////////////////////////////////
//
//    Asserts
//    We use !! below to ensure that any overloaded operators used to evaluate expr do not end up at operator ||
//
#undef _ASSERT_EXPR
#define _ASSERT_EXPR(expr, msg) \
        (void) ((!!(expr)) || \
                (1 != debug_report(_CRT_ASSERT, _T(__FILE__), __LINE__, NULL, msg)) || \
                (debug_break(), 0))


#ifndef _ASSERT
#define _ASSERT(expr)   _ASSERT_EXPR((expr), NULL)
#endif

#ifndef _ASSERTE
#define _ASSERTE(expr)  _ASSERT_EXPR((expr), _T("##expr##"))
#endif


#if !defined(WINDOWSEX) && !defined(METROWIN)
#define debug_break() debug_break()
#endif

//
//  DEBUG ON
//
/////////////////////////////

#if MEMDLEAK

#define MEMORY_ALLOC(size) memory_alloc_dbg(size , 1923, __FILE__ , __LINE__ )

#define memory_alloc(size) MEMORY_ALLOC(size)

#endif



#endif



CLASS_DECL_AURA int32_t FUNCTION_DEBUGBOX(const char * pszMessage, const char * pszTitle, int32_t iFlags);

inline int32_t FUNCTION_XXDEBUGBOX(const char * pszMessage, const char * pszTitle, int32_t iFlags)
{

   UNREFERENCED_PARAMETER(pszMessage);
   UNREFERENCED_PARAMETER(pszTitle);
   UNREFERENCED_PARAMETER(iFlags);

   return 0;

}

#define debug_box FUNCTION_DEBUGBOX
#define xxdebug_box FUNCTION_XXDEBUGBOX




void CLASS_DECL_AURA TRACELASTERROR();



CLASS_DECL_AURA string FormatMessageFromSystem(uint32_t dwError);


#endif // __AXIS_AXIS_DEBUG_H__












#ifdef MEMDLEAK

CLASS_DECL_AURA string get_mem_info_report1();
CLASS_DECL_AURA ::count get_mem_info(int32_t ** ppiUse, const char *** ppszFile, const char *** pszCallStack, uint32_t ** ppuiLine, size_t ** ppsize);
CLASS_DECL_AURA ::count get_mem_info2(int32_t ** ppiUse, const char *** ppszFile, DWORD64 ** ppuiStack[64], int64_t ** ppiStack, int32_t ** ppiLine, int64_t ** ppiSize);



#endif



enum e_level
{

   level_info,
   level_warning,
   level_error,
   level_debug

};


CLASS_DECL_AURA void os_trace(e_level elevel, const char * pszTag, const char * pszMessage);
CLASS_DECL_AURA void trace(e_level elevel, const char * pszTag, const char * psz, const char * pszFile = NULL, int iLine = -1);




#define LOG(level, tag) ::plain_text_stream_log_line(level, tag, __FILE__, __LINE__)


#define cinfo LOG(level_info, LOG_TAG)
#define cwarn LOG(level_warning, LOG_TAG)
#define cerror LOG(level_error, LOG_TAG)
#define cdebug LOG(level_debug, LOG_TAG)


#define thisinfo LOG(level_info, LOG_THIS_TAG) << LOG_MEMBER_PREFIX << "; "
#define thiswarn LOG(level_warning, LOG_THIS_TAG)  << LOG_MEMBER_PREFIX << "; "
#define thiserr LOG(level_error, LOG_THIS_TAG) << LOG_MEMBER_PREFIX << "; "
#define thisdebug LOG(level_debug, LOG_THIS_TAG) << LOG_MEMBER_PREFIX << "; "

#define thisstart thisinfo << "start "
#define thisexit thisinfo << "exit " << str_flag_ifnumberparenthesizeandspace
#define thisok thisinfo << "ok " << str_flag_ifnumberparenthesizeandspace
#define thisexc thiserr << "exception " << str_flag_ifnumberparenthesizeandspace
#define thisexcall thiserr << "exception(...) " << str_flag_ifnumberparenthesizeandspace
#define thisfail thiserr << "failure " << str_flag_ifnumberparenthesizeandspace
#define thisend thisinfo << "end "



extern CLASS_DECL_AURA mutex * g_pmutexMemoryCounters;

CLASS_DECL_AURA bool memcnts();

CLASS_DECL_AURA ::file::path memcnts_base_path();

template < typename T >
::file::path memcnts_path(T * pthis);

template < typename T >
void memcnts_inc(T * pthis);

template < typename T >
void memcnts_dec(T * pthis);
