#pragma once


#include <wchar.h>


#include "linux_graphics.h"
#include "linux_process.h"
#include "linux_cross_win_gdi.h"
#include "linux_cross_win_file.h"
#include "linux_file.h"
#include "linux_file_watcher.h"
#include "linux_cairo_keep.h"
#include "linux_window_xlib.h"



#define ENABLE_BINRELOC
//#include "c_os_binreloc.h"



DWORD get_tick_count();




#define _fileno   fileno
#define _unlink   unlink





CLASS_DECL_BASE DWORD GetLastError();
CLASS_DECL_BASE DWORD SetLastError(DWORD dw);


CLASS_DECL_BASE bool _istlead(int32_t ch);




bool c_xstart();

extern oswindow g_oswindowDesktop;



CLASS_DECL_BASE bool main_initialize();
CLASS_DECL_BASE bool main_finalize();
