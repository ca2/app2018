#pragma once


#include "aura/aura/node/ansios/ansios.h"




string get_error_message(DWORD dwError);

::aura::application *     macos_instantiate_application(::aura::application * pappSystem, const char * pszId);



CLASS_DECL_AURA void vfxThrowFileException(sp(::aura::application) papp, int32_t cause, LONG lOsError, const char * lpszFileName = NULL);

CLASS_DECL_AURA ::file::exception * get_FileException(::aura::application * papp, ::file::exception::e_cause cause, LONG lOsError, const char * lpszFileName = NULL);



/////////////////////////////////////////////////////////////////////////////
// explicit initialization for general purpose classes

CLASS_DECL_AURA WINBOOL AfxInitialize(WINBOOL bDLL = FALSE, DWORD dwVersion = 0);

/////////////////////////////////////////////////////////////////////////////
// stop on a specific memory request

// Debugger hook on specified allocation request - Obsolete
CLASS_DECL_AURA void AfxSetAllocStop(LONG lRequestNumber);

// Return TRUE if memory is sane or print out what is wrong
CLASS_DECL_AURA WINBOOL AfxCheckMemory();

// Return TRUE if valid memory block of nBytes
CLASS_DECL_AURA WINBOOL AfxIsMemoryBlock(const void * p, UINT nBytes, LONG* plRequestNumber = NULL);

// helper routines for non-C++ EH implementations
// for THROW_LAST auto-delete backward compatiblity
CLASS_DECL_AURA void AfxThrowLastCleanup();

// other out-of-line helper functions
CLASS_DECL_AURA void AfxTryCleanup();


#define NULL_REF(class) (*((class *) NULL))

#define MAC_THREAD(pthread) (dynamic_cast < ::mac::thread * > (dynamic_cast < ::thread * >(pthread)))


CLASS_DECL_AURA void __trace_message(const char * lpszPrefix, ::message::message * pobj);
CLASS_DECL_AURA void __trace_message(const char * lpszPrefix, LPMESSAGE lpmsg);
WINBOOL PeekMessage(LPMESSAGE lpMsg,oswindow hWnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg);
WINBOOL GetMessage(LPMESSAGE lpMsg,oswindow hWnd,UINT wMsgFilterMin,UINT wMsgFilterMax);
int32_t CLASS_DECL_AURA __mac_main(int32_t argc, char * argv[]);
CLASS_DECL_AURA void vfxThrowFileException(::aura::application * papp, ::file::exception::e_cause ecause, LONG lOsError, const char * lpszFileName = NULL);



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






void ns_app_terminate();
void ns_create_alias(const char * pszTarget, const char * pszSource);


