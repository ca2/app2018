#pragma once



#include <wchar.h>

#include "solaris_graphics.h"
#include "solaris_http_lib.h"
#include "solaris_http.h"
#include "solaris_process.h"
#include "solaris_cross_win_gdi.h"
#include "solaris_cross_win_file.h"
#include "solaris_file.h"
#include "solaris_file_watcher.h"
#include "solaris_cairo_keep.h"
#include "solaris_window_cairo.h"



#define ENABLE_BINRELOC
//#include "c_os_binreloc.h"



DWORD get_tick_count();




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




union semun
{

   int32_t val;
   struct semid_ds * buf;
   USHORT * array;

};



CLASS_DECL_BASE DWORD GetLastError();
CLASS_DECL_BASE DWORD SetLastError(DWORD dw);


CLASS_DECL_BASE bool _istlead(int32_t ch);


oswindow GetCapture();
oswindow SetCapture(oswindow window);
WINBOOL ReleaseCapture();
oswindow SetFocus(oswindow window);
oswindow GetFocus();
oswindow GetWindow(oswindow window, int iParentHood);
oswindow GetActiveWindow();
oswindow SetActiveWindow(oswindow window);
WINBOOL DestroyWindow(oswindow window);
oswindow GetDesktopWindow();



class osdisplay_dataptra :
   public array < osdisplay_data * >
{
public:


};


class oswindow_dataptra :
   public array < oswindow_data * >
{
public:


};



bool c_xstart();

extern oswindow g_oswindowDesktop;
