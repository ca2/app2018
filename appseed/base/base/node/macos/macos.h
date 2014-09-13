#pragma once


#define SECURITY_WIN32



#define CLASS_DECL_mac


string get_error_message(DWORD dwError);

sp(::aura::application)     mac_instantiate_application(sp(::aura::application) pappSystem, const char * pszId);

/////////////////////////////////////////////////////////////////////////////
// explicit initialization for general purpose classes

//CLASS_DECL_mac WINBOOL AfxInitialize(WINBOOL bDLL = FALSE, DWORD dwVersion = _MFC_VER);
CLASS_DECL_mac WINBOOL AfxInitialize(WINBOOL bDLL = FALSE, DWORD dwVersion = 0);

/////////////////////////////////////////////////////////////////////////////
// stop on a specific primitive::memory request

// Debugger hook on specified allocation request - Obsolete
CLASS_DECL_mac void AfxSetAllocStop(LONG lRequestNumber);

// Return TRUE if primitive::memory is sane or print out what is wrong
CLASS_DECL_mac bool __check_memory();

// Return TRUE if valid primitive::memory block of nBytes
CLASS_DECL_mac WINBOOL AfxIsMemoryBlock(const void * p, UINT nBytes,
                                        LONG* plRequestNumber = NULL);

// helper routines for non-C++ EH implementations
// for THROW_LAST auto-delete backward compatiblity
CLASS_DECL_mac void AfxThrowLastCleanup();

// other out-of-line helper functions
CLASS_DECL_mac void AfxTryCleanup();


/////////////////////////////////////////////////////////////////////////////
// Global implementation helpers

// window creation hooking
CLASS_DECL_mac void AfxHookWindowCreate(::user::interaction * pWnd);
CLASS_DECL_mac WINBOOL AfxUnhookWindowCreate();
CLASS_DECL_mac void AfxResetMsgCache();

// for backward compatibility to previous versions
#define _AfxHookWindowCreate    AfxHookWindowCreate
#define _AfxUnhookWindowCreate  AfxUnhookWindowCreate

#pragma once


// from wincore.cpp
extern CLASS_DECL_mac const char _afxWnd[];           // simple child windows/controls
extern CLASS_DECL_mac const char _afxWndControlBar[]; // controls with gray backgrounds
extern CLASS_DECL_mac const char _afxWndMDIFrame[];
extern CLASS_DECL_mac const char _afxWndFrameOrView[];
extern CLASS_DECL_mac const char _afxWndOleControl[];


/////////////////////////////////////////////////////////////////////////////
// Special helpers

CLASS_DECL_mac void AfxCancelModes(oswindow hWndRcvr);
CLASS_DECL_mac WINBOOL AfxHelpEnabled();  // determine if ID_HELP handler exists
CLASS_DECL_mac WINBOOL AfxCustomLogFont(UINT nIDS,LOGFONT* pLogFont);
CLASS_DECL_mac WINBOOL AfxGetPropSheetFont(string & strFace,WORD& wSize,WINBOOL bWizard);

CLASS_DECL_mac WINBOOL _AfxIsComboBoxControl(oswindow hWnd,UINT nStyle);
CLASS_DECL_mac WINBOOL _AfxCheckCenterDialog(const char * lpszResource);
CLASS_DECL_mac WINBOOL _AfxCompareClassName(oswindow hWnd,const char * lpszClassName);
CLASS_DECL_mac oswindow _AfxChildWindowFromPoint(oswindow,POINT);

// for determining version of COMCTL32.DLL
#define VERSION_WIN4    MAKELONG(0, 4)
#define VERSION_IE3             MAKELONG(70, 4)
#define VERSION_IE4             MAKELONG(71, 4)
#define VERSION_IE401   MAKELONG(72, 4)
#define VERSION_6      MAKELONG(0, 6)
extern int _afxComCtlVersion;
DWORD _AfxGetComCtlVersion();

#include "macos_dir.h"
#include "macos_factory_exchange.h"
#include "macos_port_forward.h"
#include "macos_crypto.h"
#include "macos_ip_enum.h"

#define NULL_REF(class) (*((class *) NULL))
// xxx CLASS_DECL_mac WNDPROC AfxGetAfxWndProc();
#define AfxWndProc (*AfxGetAfxWndProc())

#define MAC_THREAD(pthread) (dynamic_cast < ::mac::thread * > (dynamic_cast < ::thread * >(pthread)))
#define MAC_WINDOW(pwnd) (dynamic_cast < ::mac::interaction_impl * > (dynamic_cast < ::user::interaction_impl * >(pwnd)))


CLASS_DECL_mac void __trace_message(const char * lpszPrefix, signal_details * pobj);
CLASS_DECL_mac void __trace_message(const char * lpszPrefix, LPMESSAGE lpmsg);

CLASS_DECL_mac void AfxProcessWndProcException(::exception::base*, signal_details * pobj);
CLASS_DECL_mac void __cdecl __pre_translate_message(signal_details * pobj);



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



int32_t CLASS_DECL_mac __mac_main(int32_t argc, char * argv[]);


CLASS_DECL_mac void vfxThrowFileException(sp(::aura::application) papp, int32_t cause, LONG lOsError, const char * lpszFileName = NULL);



#include "macos_dir.h"
#include "macos_file_system.h"
#include "macos_file_set.h"
