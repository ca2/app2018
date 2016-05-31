#pragma once


#include "ace/ace/node/ansios/ansios.h"




string get_error_message(DWORD dwError);

::ace::application *     macos_instantiate_application(::ace::application * pappSystem, const char * pszId);

/////////////////////////////////////////////////////////////////////////////
// explicit initialization for general purpose classes

CLASS_DECL_ACE WINBOOL AfxInitialize(WINBOOL bDLL = FALSE, DWORD dwVersion = 0);

/////////////////////////////////////////////////////////////////////////////
// stop on a specific memory request

// Debugger hook on specified allocation request - Obsolete
CLASS_DECL_ACE void AfxSetAllocStop(LONG lRequestNumber);

// Return TRUE if memory is sane or print out what is wrong
CLASS_DECL_ACE WINBOOL AfxCheckMemory();

// Return TRUE if valid memory block of nBytes
CLASS_DECL_ACE WINBOOL AfxIsMemoryBlock(const void * p, UINT nBytes, LONG* plRequestNumber = NULL);

// helper routines for non-C++ EH implementations
// for THROW_LAST auto-delete backward compatiblity
CLASS_DECL_ACE void AfxThrowLastCleanup();

// other out-of-line helper functions
CLASS_DECL_ACE void AfxTryCleanup();


#define NULL_REF(class) (*((class *) NULL))

#define MAC_THREAD(pthread) (dynamic_cast < ::mac::thread * > (dynamic_cast < ::thread * >(pthread)))


CLASS_DECL_ACE void __trace_message(const char * lpszPrefix, signal_details * pobj);
CLASS_DECL_ACE void __trace_message(const char * lpszPrefix, LPMESSAGE lpmsg);
WINBOOL PeekMessage(LPMESSAGE lpMsg,oswindow hWnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg);
WINBOOL GetMessage(LPMESSAGE lpMsg,oswindow hWnd,UINT wMsgFilterMin,UINT wMsgFilterMax);
int32_t CLASS_DECL_ACE __mac_main(int32_t argc, char * argv[]);
CLASS_DECL_ACE void vfxThrowFileException(::ace::application * papp, ::file::exception::e_cause ecause, LONG lOsError, const char * lpszFileName = NULL);



#include "macos_factory_exchange.h"
#include "macos_thread.h"
#include "macos_os.h"


#include "macos_shell.h"

#include "macos_application.h"






#include "macos_file.h"
#include "macos_shell.h"
#include "macos_stdio_file.h"

#include "macos_dir.h"
#include "macos_file_system.h"






