/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * DirectFB Client
 *
 * Copyright 2011 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __DFREERDP_H
#define __DFREERDP_H

#include "axis/rdpclient/config.h"
#ifdef _WIN32
#include "axis/rdpclient/client/Windows/wf_client.h"
//#elif defined(ANDROID)
//#include "axis/rdpclient/client/A"
#endif
#include <freerdp/freerdp.h>
#include <freerdp/graphics.h>
#include <freerdp/gdi/gdi.h>
#include <freerdp/codec/color.h>
#include <freerdp/channels/channels.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct ca2rdp_info ca2rdpInfo;

CLASS_DECL_AXIS_RDPCLIENT BOOL ca2rdp_pre_connect(freerdp* instance);

#ifdef _WIN32
#else
typedef uint32_t COLORREF;
#endif

namespace aura
{
   class application;
}

namespace draw2d
{
   class dib;
   class graphics;
}

namespace visual
{
   class cursor;
   void cursor_alloc(::aura::application * papp,cursor * & pcursor,int xHotspot,int yHotspot);
   ::draw2d::dib * cursor_get_dib(cursor * pcursor);
}


struct rdpclient_view_interface
{
   virtual void rdpclient_set_cursor(::visual::cursor * pcursor) = 0;
};

namespace message
{
   class base;
   class mouse;
}

namespace draw2d
{
   void dib_copy(dib * pdibthis, dib *pdib);
   void dib_alloc(::aura::application * papp, dib * & pdib);
   void dib_create(dib * pdib, int w, int h);
   unsigned int * dib_get_data(dib * pdib);
   graphics * dib_get_graphics(dib * pdib);
}

struct ca2rdp_context :
#ifdef _WIN32
   public wf_context
#elif defined(ANDROID)
   public rdpContext
#endif
{
   int width;
   int height;
   int dstBpp;



   DWORD mainThreadId;
   HANDLE thread;
   ::aura::application * m_pappRdp;
   rdpclient_view_interface * m_pviewRdp;
   
	ca2rdpInfo* ca2rdpi;
};

CLASS_DECL_AXIS_RDPCLIENT ::draw2d::graphics * ca2rdp_ctx_get_graphics(ca2rdp_context * pcontext);
CLASS_DECL_AXIS_RDPCLIENT COLORREF * ca2rdp_ctx_get_primary(ca2rdp_context * pcontext);


typedef struct ca2rdp_context ca2rdpContext;

struct ca2rdp_pointer
{
	rdpPointer pointer;
   ::visual::cursor * m_pcursor;
	UINT32 xhot;
	UINT32 yhot;
};
typedef struct ca2rdp_pointer ca2rdpPointer;

struct ca2rdp_info
{
	int read_fds;
	int err;
   ::message::base * event;
	//DFBEvent event;
	HCLRCONV clrconv;
	int x;
	int y;
	int w;
	int h;
   //DFBSurfaceDescription dsc;
	::draw2d::dib * primary;
   ::draw2d::dib * surface;
//	IDirectFBDisplayLayer* layer;
	//IDirectFBEventBuffer* event_buffer;



};


CLASS_DECL_AXIS_RDPCLIENT BOOL ca2rdp_post_connect(freerdp* instance);

#endif /* __DFREERDP_H */
