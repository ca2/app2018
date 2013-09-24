#pragma once


#include "metrowin_file.h"
#include "metrowin_thread.h"


/*#define GetAValue(rgb)      (LOBYTE((rgb)>>24))
#define ARGB(a, r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)|(((DWORD)(BYTE)(a))<<24)))*/


//CLASS_DECL_c int_bool os_initialize();
//CLASS_DECL_c int_bool os_finalize();


CLASS_DECL_c int_bool main_initialize();
CLASS_DECL_c int_bool main_finalize();


CLASS_DECL_c void set_main_thread(HANDLE hThread);
CLASS_DECL_c void set_main_thread_id(UINT uiThread);


CLASS_DECL_c HTHREAD get_main_thread();
CLASS_DECL_c UINT   get_main_thread_id();


CLASS_DECL_c void attach_thread_input_to_main_thread(int_bool bAttach);



#ifdef __cplusplus

CLASS_DECL_c string normalize_path(const char * lpcszPath);


string key_to_char(WPARAM wparam, LPARAM lparam);

#endif

extern CLASS_DECL_c int g_iMouse;

#define GW_HWNDFIRST        0
#define GW_HWNDLAST         1
#define GW_HWNDNEXT         2
#define GW_HWNDPREV         3
#define GW_OWNER            4
#define GW_CHILD            5
#if(WINVER <= 0x0400)
#define GW_MAX              5
#else
#define GW_ENABLEDPOPUP     6
#define GW_MAX              6
#endif


//CLASS_DECL_c int WINAPI GetSystemMetrics(int i);

CLASS_DECL_c oswindow WINAPI GetFocus();
CLASS_DECL_c oswindow WINAPI SetFocus(oswindow oswindow);

CLASS_DECL_c oswindow WINAPI GetCapture();
CLASS_DECL_c oswindow WINAPI SetCapture(oswindow oswindow);
CLASS_DECL_c oswindow WINAPI ReleaseCapture();


