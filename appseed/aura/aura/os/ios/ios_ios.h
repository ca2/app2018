//
//  os.h
//  aura
//
//  Created by Camilo Sasuke Tsumanuma on 08/07/15.
//
//

#pragma once




#include "ios_file_watcher.h"



#include <stdarg.h>
#include <wchar.h>

#include "ios_process.h"
#include "ios_cross_win_gdi.h"
#include "ios_file_watcher.h"
//#include "ios_thread.h"



//#define ENABLE_BINRELOC


DWORD get_tick_count();

//#define ARGB(a, r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)|(((DWORD)(BYTE)(a))<<24)))


#define _fileno   fileno
#define _unlink   unlink


/*
 #define STATUS_WAIT_0            ((DWORD   ) 0x00000000L)
 #define STATUS_ABANDONED_WAIT_0  ((DWORD   ) 0x00000080L)
 #define STATUS_USER_APC          ((DWORD   ) 0x000000C0L)
 #define STATUS_TIMEOUT           ((DWORD   ) 0x00000102L)
 */

#define WAIT_FAILED              ((DWORD   ) 0xFFFFFFFFL)
#define WAIT_OBJECT_0            ((DWORD   ) STATUS_WAIT_0 + 0)

#define WAIT_ABANDONED           ((DWORD   ) STATUS_ABANDONED_WAIT_0 + 0)
#define WAIT_ABANDONED_0         ((DWORD   ) STATUS_ABANDONED_WAIT_0 + 0)

#define WAIT_IO_COMPLETION                   STATUS_USER_APC

// based on Windows implementation and also a good limit for iterations with nanosleep and multiple locks in objects up to MAXIMUM_WAIT_OBJECTS in a single call
#define MAXIMUM_WAIT_OBJECTS      64


/*WINBOOL
 WINAPI
 __win_CloseThread(
 HANDLE hThread
 );
 */

oswindow get_capture();
oswindow set_capture(oswindow window);
WINBOOL release_capture();
oswindow set_focus(oswindow window);
oswindow get_focus();
oswindow get_active_window();
oswindow set_active_window(oswindow window);


//void GetMainScreenRect(LPRECT lprect);

WINBOOL SetForegroundWindow(oswindow window);
WINBOOL BringWindowToTop(oswindow window);

void ns_redraw_window(oswindow window);





void ui_application_main(int argc, char * argv[]);
void ui_app_run(int argc, char * argv[]);


