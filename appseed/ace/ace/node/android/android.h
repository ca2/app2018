#pragma once

#include "ace/node/ansios/ansios.h"


string get_error_message(DWORD dwError);

::ace::application *     win_instantiate_application(::ace::application * pappSystem, const char * pszId);

/////////////////////////////////////////////////////////////////////////////
// explicit initialization for general purpose classes

//CLASS_DECL_ACE WINBOOL AfxInitialize(WINBOOL bDLL = FALSE, DWORD dwVersion = _MFC_VER);
CLASS_DECL_ACE WINBOOL AfxInitialize(WINBOOL bDLL = FALSE, DWORD dwVersion = 0);

/////////////////////////////////////////////////////////////////////////////
// stop on a specific memory request

// Debugger hook on specified allocation request - Obsolete
CLASS_DECL_ACE void AfxSetAllocStop(LONG lRequestNumber);

// Return TRUE if memory is sane or print out what is wrong
CLASS_DECL_ACE WINBOOL AfxCheckMemory();

// Return TRUE if valid memory block of nBytes
CLASS_DECL_ACE WINBOOL AfxIsMemoryBlock(const void * p, UINT nBytes,
   LONG* plRequestNumber = NULL);

CLASS_DECL_ACE void AfxResetMsgCache();


#include "android1.h"
#include "android_implementation.h"
#include "android_factory_exchange.h"
#include "android_thread.h"
#include "android_os.h"

#include "android_dir.h"
#include "android_file_system.h"

#define NULL_REF(class) (*((class *) NULL))
// xxx CLASS_DECL_ACE WNDPROC AfxGetAfxWndProc();
#define AfxWndProc (*AfxGetAfxWndProc())

#define ANDROID_THREAD(pthread) (dynamic_cast < ::android::thread * > (dynamic_cast < ::thread * >(pthread)))

#include "android_shell.h"

//CLASS_DECL_ACE void __trace_message(const char * lpszPrefix, ::signal_details * pobj);
//CLASS_DECL_ACE void __trace_message(const char * lpszPrefix, LPMESSAGE lpmsg);

CLASS_DECL_ACE void __cdecl __pre_translate_message(::signal_details * pobj);


#include "android_application.h"



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



int32_t CLASS_DECL_ACE __android_main(int32_t argc, char * argv[]);


CLASS_DECL_ACE void vfxThrowFileException(::ace::application * papp, ::file::exception::e_cause ecause, LONG lOsError, const char * lpszFileName = NULL);


#include "android_internal.h"




