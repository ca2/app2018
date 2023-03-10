#pragma once




#include "exception_call_stack.h"

#include "exception_verisimple.h"

#include "exception_exception.h"


#include "exception_base.h"

#include "exception_simple.h"
#include "exception_cancel.h"

#include "exception_memory.h"
#include "exception_invalid_argument.h"

#include "exception_standard_translator.h"
#include "exception_standard.h"


namespace win
{

   CLASS_DECL_AURA string error_message(uint32_t dwError);

}


namespace exception
{

   CLASS_DECL_AURA void throw_interface_only(::aura::application * papp);
   CLASS_DECL_AURA void throw_not_implemented(::aura::application * papp);

}



#include "exception_error.h"
#include "exception_overflow_error.h"
#include "exception_runtime_error.h"
#include "exception_range_error.h"


#include "exception_not_implemented.h"
#include "exception_void_implementation.h"
#include "exception_interface_only.h"
#include "exception_not_installed.h"
#include "exception_not_supported.h"
#include "exception_exit.h"
#include "exception_not_licensed.h"
#include "exception_temporary.h"
#include "exception_installing.h"

#include "exception_assert.h"
#include "exception_misc.h"



#include "exception_io.h"
#include "exception_resource.h"
#include "exception_user.h"





#include "exception_hresult.h"




#ifndef ___JUMPBUF
// Use portable 'jmp_buf' defined by ANSI by default.
#define ___JUMPBUF jmp_buf
#endif

namespace datetime
{


   class time_span;
   class time;


} // namespace datetime


class dump_context;

// ::datetime::time_span diagnostics and serialization
CLASS_DECL_AURA dump_context & operator<<(dump_context & dumpcontext, const ::datetime::time_span & dateSpanSrc);
CLASS_DECL_AURA dump_context & operator<<(dump_context & dumpcontext, const ::datetime::time & dateSrc);



#if defined(DEBUG) && !defined(___NO_DEBUG_CRT)



// Return TRUE if memory is sane or print out what is wrong
CLASS_DECL_AURA bool __check_memory();

// Return TRUE if valid memory block of nBytes
CLASS_DECL_AURA bool __is_memory_block(const void * p,UINT nBytes,LONG* plRequestNumber = NULL);



#define afxMemDF _crtDbgFlag

enum e_memdbg // memory debug/diagnostic flags
{
   memdbg_alloc            = _DEBUG_ALLOC_MEM_DF,         // turn on debugging allocator
   memdbg_delay_free       = _DEBUG_DELAY_FREE_MEM_DF,         // delay freeing memory
   memdbg_check_always     = _DEBUG_CHECK_ALWAYS_DF,          // __check_memory on every alloc/free
   memdbg_check_every16    = _DEBUG_CHECK_EVERY_16_DF,
   memdbg_check_every128   = _DEBUG_CHECK_EVERY_128_DF,
   memdbg_check_every1024  = _DEBUG_CHECK_EVERY_1024_DF,
   memdbg_check_default    = _DEBUG_CHECK_DEFAULT_DF
};

#define __output_debug_string TRACE

// turn on/off tracking for a int16_t while
CLASS_DECL_AURA bool __enable_memory_tracking(bool bTrack);

// Turn on/off the global flag gen_MemoryLeakOverride. if bEnable is TRUE
// then further calls to __enable_memory_tracking() wont change the current
// memory tracking state, until __enable_memory_leak_override(bool bEnable)
// is called again with bEnable == FALSE.
CLASS_DECL_AURA bool __enable_memory_leak_override(bool bEnable);



#include "exception_memory_state.h"

// Enumerate allocated objects or runtime classes
/*void __do_for_all_objects(void (c_cdecl *pfn)(object* pObject, void * pContext),
   void * pContext);
void ::core::DoForAllClasses(void (c_cdecl *pfn)(sp(type) pClass,
   void * pContext), void * pContext);*/


#else

// non-DEBUG_ALLOC version that assume everything is OK
#define AURA_NEW new
#define __check_memory() TRUE
#define __is_memory_block(p, nBytes) TRUE
#define __enable_memory_tracking(bTrack) FALSE
#define __enable_memory_leak_override(bEnable) TRUE
#define __output_debug_string(lpsz) ::output_debug_string(lpsz)


#endif // DEBUG



#define __stack_dump_TARGET_TRACE                     0x0001
#define __stack_dump_TARGET_CLIPBOARD 0x0002
#define __stack_dump_TARGET_BOTH                      0x0003
#define __stack_dump_TARGET_ODS                       0x0004
#ifdef DEBUG
#define __stack_dump_TARGET_DEFAULT           __stack_dump_TARGET_TRACE
#else
#define __stack_dump_TARGET_DEFAULT           __stack_dump_TARGET_CLIPBOARD
#endif

void __dump_stack(uint32_t dwFlags = __stack_dump_TARGET_DEFAULT);

#include "exception_dump_context.h"

#ifdef DEBUG
extern CLASS_DECL_AURA bool g_bTraceEnabled;
#endif

#ifdef DEBUG
#define _DUMP( exp ) (void)(*::aura::system::g_p->m_pdumpcontext<<exp)
#else
#define _DUMP( exp )
#endif



#ifdef DEBUG


#define DEBUG_NOTE __FILE__

#ifdef __MCRTDBG
#define AURA_NEW new
#else
#define AURA_NEW new(DEBUG_NOTE, __LINE__)
#endif

#define THREAD_NOTE __get_thread_note()
#define SET_THREAD_NOTE(x) __set_thread_note(x);


#else


#define SET_THREAD_NOTE(x)


#endif


CLASS_DECL_AURA string __get_thread_note();
CLASS_DECL_AURA void __set_thread_note(const char * pszNote);



#ifdef DEBUG

#define DEBUG_ONLY(f)      f

#else // DEBUG

#define DEBUG_ONLY(f)

#endif // !DEBUG*/



#include "exception_debug.h"



// Debug ASSERTs then throws. Retail throws if condition not met
#define ENSURE_THROW(cond, exception)   \
   do { int32_t _gen__condVal=!!(cond); ASSERT(_gen__condVal); if (!(_gen__condVal)){exception;} } while (false)
#define ENSURE(cond)      ENSURE_THROW(cond, _throw( invalid_argument_exception(get_app()) ))
#define ENSURE_ARG(cond)   ENSURE_THROW(cond, _throw( invalid_argument_exception(get_app()) ))

// Debug ASSERT_VALIDs then throws. Retail throws if pOb is NULL
#define ENSURE_VALID_THROW(pOb, exception)   \
   do { ASSERT_VALID(pOb); if (!(pOb)){exception;} } while (false)
#define ENSURE_VALID(pOb)   ENSURE_VALID_THROW(pOb, _throw( invalid_argument_exception(get_app()) ))

#define ASSERT_POINTER(p, type) \
   ASSERT(((p) != NULL) && __is_valid_address((p), sizeof(type), FALSE))

#define ASSERT_NULL_OR_POINTER(p, type) \
   ASSERT(((p) == NULL) || __is_valid_address((p), sizeof(type), FALSE))


#if defined(__arm__)
#  define UNUSED_ALWAYS(x)
#elif defined(__GNUC__)
#  define UNUSED_ALWAYS(x) __attribute__((__unused__))
#elif defined(_MSC_VER)
#  define UNUSED_ALWAYS(x)
#else
#  define UNUSED_ALWAYS(x) x
#endif


#ifdef DEBUG
#  define UNUSED(x) x
#else
#  define UNUSED(x) UNUSED_ALWAYS(x)
#endif


#ifdef DEBUG
#define REPORT_EXCEPTION(pException, szMsg) \
   do { \
      string str; \
      if (pException->get_error_message(str, 0)) \
         TRACE(::aura::trace::category_AppMsg, 0, "%s (%s:%d)\n%s\n", szMsg, __FILE__, __LINE__, str); \
      else \
         TRACE(::aura::trace::category_AppMsg, 0, "%s (%s:%d)\n", szMsg, __FILE__, __LINE__); \
      ASSERT(FALSE); \
   } while (0)
#else
#define REPORT_EXCEPTION(pException, szMsg) \
   do { \
      string strMsg; \
      char  szErrorMessage[512]; \
      if (pException->get_error_message(szErrorMessage, sizeof(szErrorMessage)/sizeof(*szErrorMessage), 0)) \
         strMsg.Format("%s (%s:%d)\n%s", szMsg, __FILE__, __LINE__, szErrorMessage); \
      else \
         strMsg.Format("%s (%s:%d)", szMsg, __FILE__, __LINE__); \
      System.simple_message_box(strMsg); \
   } while (0)
#endif



#define EXCEPTION_IN_DTOR(pException) \
   do { \
      REPORT_EXCEPTION((pException), "Exception thrown in destructor"); \
      delete pException; \
   } while (0)

#define __BEGIN_DESTRUCTOR try {
#define __END_DESTRUCTOR   } catch (::exception::base *pException) { EXCEPTION_IN_DTOR(pException); }



#define C_RUNTIME_ERROR_CHECK(expr) ::c_runtime_error_check(expr)
#ifndef C_RUNTIME_ERRORCHECK_SPRINTF
#define C_RUNTIME_ERRORCHECK_SPRINTF(expr) \
do { \
   errno_t _saveErrno = errno; \
   errno = 0; \
   (expr); \
   if(0 != errno) \
   { \
      ::c_runtime_error_check(errno); \
   } \
   else \
   { \
      errno = _saveErrno; \
   } \
} while (0)
#endif // C_RUNTIME_ERRORCHECK_SPRINTF


CLASS_DECL_AURA errno_t c_runtime_error_check(errno_t error);
CLASS_DECL_AURA void __cdecl __clearerr_s(FILE *stream);
















/////////////////////////////////////////////////////////////////////////////
// Diagnostic support
CLASS_DECL_AURA int __assert_failed_line(const char * lpszFileName, int nLine);
//CLASS_DECL_AURA void c_cdecl __trace(const char * lpszFormat, ...);
CLASS_DECL_AURA void __assert_valid_object(const object* pOb, const char * lpszFileName, int32_t nLine);
CLASS_DECL_AURA void __dump(const object* pOb);

#define THIS_FILE          __FILE__

//#ifdef DEBUG
#define TRACE ::aura::trace_add_file_and_line(m_papp, __FILE__, __LINE__)
#define APPTRACE ::aura::trace_add_file_and_line(papp, __FILE__, __LINE__)
//#else
//#ifdef WINDOWS
//#define TRACE(...) __noop
//#define APPTRACE(...) __noop
//#else
//#define TRACE(...)
//#define APPTRACE(...)
//#endif
//#endif









#include "exception_last_error.h"

// mrs/src as of 2012-08-18
// cyaxis/os/exception
#include "exception_os.h"
#include "exception_invalid_index.h"
#include "exception_invalid_handle.h"
#include "exception_operation_canceled.h"


#include "exception_invalid_character.h"

#include "exception_todo.h"

#include "exception_system.h"


#include "exception_parsing.h"
