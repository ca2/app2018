#ifndef MFREERDP_H
#define MFREERDP_H

typedef struct mf_context mfContext;

#include <freerdp/freerdp.h>
#include <freerdp/client/file.h>
#include <freerdp/api.h>
#include <freerdp/freerdp.h>

#include <freerdp/gdi/gdi.h>
#include <freerdp/gdi/dc.h>
#include <freerdp/gdi/region.h>
#include <freerdp/rail/rail.h>
#include <freerdp/cache/cache.h>
#include <freerdp/channels/channels.h>

#include <winpr/crt.h>
#include <winpr/synch.h>
#include <winpr/thread.h>

#include "MRDPView.h"
#include "Keyboard.h"
#include <AppKit/NSView.h>

struct mf_context
{
	rdpContext context;
	DEFINE_RDP_CLIENT_COMMON();

	void* view;
	WINBOOL view_ownership;

	int width;
	int height;
	int offset_x;
	int offset_y;
	int fs_toggle;
	int fullscreen;
	int percentscreen;
	char window_title[64];
	int client_x;
	int client_y;
	int client_width;
	int client_height;

	HANDLE stopEvent;
	HANDLE keyboardThread;
	enum APPLE_KEYBOARD_TYPE appleKeyboardType;

	HGDI_DC hdc;
	UINT16 srcBpp;
	UINT16 dstBpp;
	freerdp* instance;

	DWORD mainThreadId;
	DWORD keyboardThreadId;
	WINBOOL disconnect;
	WINBOOL sw_gdi;

	rdpFile* connectionRdpFile;

	// Keep track of window size and position, disable when in fullscreen mode.
	WINBOOL disablewindowtracking;

	// These variables are required for horizontal scrolling.
	WINBOOL updating_scrollbars;
	WINBOOL xScrollVisible;
	int xMinScroll;       // minimum horizontal scroll value
	int xCurrentScroll;   // current horizontal scroll value
	int xMaxScroll;       // maximum horizontal scroll value

	// These variables are required for vertical scrolling.
	WINBOOL yScrollVisible;
	int yMinScroll;       // minimum vertical scroll value
	int yCurrentScroll;   // current vertical scroll value
	int yMaxScroll;       // maximum vertical scroll value
};

#endif // MFREERDP_H
