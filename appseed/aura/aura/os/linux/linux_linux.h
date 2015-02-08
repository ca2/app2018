#pragma once


#include <wchar.h>


#include "linux_graphics.h"
#include "linux_process.h"
#include "linux_cross_win_gdi.h"
#include "linux_cross_win_file.h"
#include "linux_file.h"
#include "linux_file_watcher.h"
#include "linux_cross_win_gdi_internal.h"


#define ENABLE_BINRELOC



inline DWORD get_tick_count();




#define _fileno   fileno
#define _unlink   unlink





CLASS_DECL_AURA DWORD GetLastError();
CLASS_DECL_AURA DWORD SetLastError(DWORD dw);


CLASS_DECL_AURA bool _istlead(int32_t ch);




bool c_xstart();

extern oswindow g_oswindowDesktop;



CLASS_DECL_AURA bool main_initialize();
CLASS_DECL_AURA bool main_finalize();





#include "linux.inl"



