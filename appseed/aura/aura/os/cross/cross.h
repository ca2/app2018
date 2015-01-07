#pragma once



#ifdef WINDOWS

typedef HANDLE HTHREAD;

#else

#ifdef cplusplus

class event;

#endif

typedef struct hthread * HTHREAD;

struct oswindow_data;

typedef struct oswindow_data * oswindow;

#endif



/*
 * Defines for the fVirt field of the Accelerator table structure.
 */
#define WIN_FVIRTKEY  TRUE          /* Assumed to be == TRUE */
#define WIN_FNOINVERT 0x02
#define WIN_FSHIFT    0x04
#define WIN_FCONTROL  0x08
#define WIN_FALT      0x10




#ifndef WINDOWSEX

#ifdef LINUX
#undef inline
#define inline inline
#endif

#include "windows/windows.h"

#endif


#if !defined(LINUX) && !defined(APPLEOS) && !defined(ANDROID) && !defined(SOLARIOS)

#include "ansios/ansios.h"

#endif
