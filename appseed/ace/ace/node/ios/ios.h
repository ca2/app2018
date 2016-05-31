#pragma once


#define SECURITY_WIN32


#include "app/appseed/ace/ace/ace/ace.h"


////#define CLASS_DECL_ACE

//struct plane_system
//{
//
//   ::plane::system *    m_psystem;
//
//   plane_system(const char * pszId);
//
//   UIWindow * init_part_2ex(CGRect rect);
//
//   void begin();
//
//};


string get_error_message(DWORD dwError);

::ace::application *     ios_instantiate_application(::ace::application * pappSystem, const char * pszId);

/////////////////////////////////////////////////////////////////////////////
// explicit initialization for general purpose classes

//CLASS_DECL_ACE WINBOOL AfxInitialize(WINBOOL bDLL = FALSE, DWORD dwVersion = _MFC_VER);
CLASS_DECL_ACE WINBOOL AfxInitialize(WINBOOL bDLL = FALSE, DWORD dwVersion = 0);

/////////////////////////////////////////////////////////////////////////////
// stop on a specific memory request

// Debugger hook on specified allocation request - Obsolete
CLASS_DECL_ACE void AfxSetAllocStop(LONG lRequestNumber);

// Return TRUE if memory is sane or print out what is wrong
CLASS_DECL_ACE bool __check_memory();

// Return TRUE if valid memory block of nBytes
CLASS_DECL_ACE WINBOOL AfxIsMemoryBlock(const void * p, UINT nBytes,
                                        LONG* plRequestNumber = NULL);

// helper routines for non-C++ EH implementations
// for THROW_LAST auto-delete backward compatiblity
CLASS_DECL_ACE void AfxThrowLastCleanup();

// other out-of-line helper functions
CLASS_DECL_ACE void AfxTryCleanup();


/////////////////////////////////////////////////////////////////////////////
// Global implementation helpers

// window creation hooking
//CLASS_DECL_ACE void AfxHookWindowCreate(::user::interaction * pWnd);
//CLASS_DECL_ACE WINBOOL AfxUnhookWindowCreate();
//CLASS_DECL_ACE void AfxResetMsgCache();
//
//// for backward compatibility to previous versions
//#define _AfxHookWindowCreate    AfxHookWindowCreate
//#define _AfxUnhookWindowCreate  AfxUnhookWindowCreate

#include "ios_implementation.h"
#include "ios_dir.h"
#include "ios_file_system.h"
//#include "ios_file_set.h"
#include "ios_file.h"
#include "ios_stdio_file.h"
#include "ios_factory_exchange.h"
//#include "ios_window_draw.h"
#include "ios_thread.h"
//#include "ios_window.h"
#include "ios_os.h"
//#include "ios_port_forward.h"
//#include "ios_copydesk.h"
//#include "ios_crypto.h"
//#include "ios_ip_enum.h"

#define NULL_REF(class) (*((class *) NULL))

#define IOS_THREAD(pthread) (dynamic_cast < ::ios::thread * > (dynamic_cast < ::thread * >(pthread)))

#include "ios_shell.h"

CLASS_DECL_ACE void __trace_message(const char * lpszPrefix, signal_details * pobj);
CLASS_DECL_ACE void __trace_message(const char * lpszPrefix, LPMESSAGE lpmsg);


//#include "ios_printer.h"

#include "ios_application.h"



WINBOOL PeekMessage(
                    LPMESSAGE lpMsg,
                    oswindow hWnd,
                    UINT wMsgFilterMin,
                    UINT wMsgFilterMax,
                    UINT wRemoveMsg);

WINBOOL GetMessage(
                   LPMESSAGE lpMsg,
                   oswindow hWnd,
                   UINT wMsgFilterMin,
                   UINT wMsgFilterMax);



int32_t CLASS_DECL_ACE __ios_main(int32_t argc, char * argv[]);


CLASS_DECL_ACE void vfxThrowFileException(::ace::application * papp, ::file::exception::e_cause ecause, LONG lOsError, const char * lpszFileName = NULL);

CLASS_DECL_ACE ::file::exception * get_FileException(::ace::application * papp, ::file::exception::e_cause ecause, LONG lOsError, const char * lpszFileName = NULL);



namespace ios
{


   class thread;

   class IOS
   {
      int32_t function();
   };



} // namespace ios



// Placed on frame for EXCEPTION linkage, or ::exception::ace cleanup
struct CLASS_DECL_ACE __exception_link
{
   __exception_link* m_pLinkPrev;    // previous top, next in handler chain
   ::exception::base * m_pException;   // current exception (NULL in try block)

   __exception_link();       // for initialization and linking
   ~__exception_link()       // for cleanup and unlinking
   {
      //__try_cleanup();
   };
};

// Exception global state - never access directly
struct CLASS_DECL_ACE __EXCEPTION_CONTEXT
{
   __exception_link* m_pLinkTop;

   // Note: most of the exception context is now in the __exception_link
};




