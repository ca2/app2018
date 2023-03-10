
#include "framework.h" // from ""axis/user/user.h"
#include "linux.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>

// Ubuntu apt-get install libxmu-dev
// CentOS yum install libXmu-devel
// openSUSE zypper install libxmu-devel
//#include "xcdef.h"
//#include "xcprint.h"
//#include "xclib.h"


string os_get_clipboard_text();

/*Ķ
*
*
*  xclib.c - xclip library to look after xlib mechanics for xclip
*  Copyright (C) 2001 Kim Saunders
*  Copyright (C) 2007-2008 Peter ��strand
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

/*
 *
 *
 *  xcdef.h - definitions for use throughout xclip
 *  Copyright (C) 2001 Kim Saunders
 *  Copyright (C) 2007-2008 Peter Åstrand
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* output level constants */
#define OSILENT  0
#define OQUIET   1
#define OVERBOSE 2

/* generic true/false constants for stuff */
#define F 0     /* false... */
#define T 1     /* true...  */

/* true/false string constants */
#define SF "F"	/* false */
#define ST "T"	/* true  */

/*
 *
 *
 *  xclib.h - header file for functions in xclib.c
 *  Copyright (C) 2001 Kim Saunders
 *  Copyright (C) 2007-2008 Peter Åstrand
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <X11/Xlib.h>



namespace linux
{


   copydesk::copydesk(::aura::application * papp) :
      ::object(papp),
      ::user::copydesk(papp)
   {
   }

   copydesk::~copydesk()
   {
   }

   bool copydesk::has_filea()
   {
   }


   void copydesk::get_filea(::file::patha & patha)
   {
      int32_t iCount = get_file_count();
      if(iCount <= 0)
         return;
//      if(!m_p->OpenClipboard())
      //       return;
      _throw(todo(get_app()));
      /* HDROP hdrop = (HDROP) ::GetClipboardData(CF_HDROP);
      string str;
      for(int32_t i = 0; i < iCount; i++)
      {
         UINT uiLen = ::DragQueryFileW(hdrop, i, NULL, 0);
         unichar * lpwsz = (unichar *) malloc(sizeof(unichar) * (uiLen + 1));
         ::DragQueryFileW(hdrop, i, lpwsz, uiLen + 1);
         stra.add(::str::international::unicode_to_utf8(lpwsz));
         free(lpwsz);
      }
      ::CloseClipboard();
      */
   }

   void copydesk::set_filea(const ::file::patha & patha)
   {

      //ASSERT(m_p->IsWindow());

      strsize iLen = 0;

      for(int32_t i = 0; i < patha.get_size(); i++)
      {
         iLen += ::str::international::utf8_to_unicode_count(patha[i]) + 1;
      }

      _throw(todo(get_app()));

      /* xxx

            HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(DROPFILES) + (iLen + 1) * sizeof(WCHAR));
            LPDROPFILES pDropFiles = (LPDROPFILES) ::GlobalLock(hglbCopy);
            pDropFiles->pFiles = sizeof(DROPFILES);
            pDropFiles->pt.x = pDropFiles->pt.y = 0;
            pDropFiles->fNC = TRUE;
            pDropFiles->fWide = TRUE; // ANSI charset

            ASSERT(m_p->IsWindow());
            LPTSTR lptstrCopy = (char *) pDropFiles;
            lptstrCopy += pDropFiles->pFiles;
            unichar * lpwstrCopy = (unichar *) lptstrCopy;
            for(int32_t i = 0; i < stra.get_size(); i++)
            {
               ASSERT(m_p->IsWindow());
               ::str::international::utf8_to_unicode(lpwstrCopy, ::str::international::utf8_to_unicode_count(stra[i]) + 1, stra[i]);
               ASSERT(m_p->IsWindow());
               lpwstrCopy += (stra[i].get_length() + 1);
            }
            ASSERT(m_p->IsWindow());
            *lpwstrCopy = '\0';    // null character
            ASSERT(m_p->IsWindow());
            ::GlobalUnlock(hglbCopy);
            ASSERT(m_p->IsWindow());
            if(!m_p->OpenClipboard())
            {
               ::GlobalFree(hglbCopy);
               return;
            }
            EmptyClipboard();
            SetClipboardData(CF_HDROP, hglbCopy);
            VERIFY(::CloseClipboard());

      */

   }



   bool copydesk::initialize()
   {


      if(!::user::copydesk::initialize())
         return false;

      return true;

   }


   bool copydesk::finalize()
   {

      bool bOk;

      bOk = ::user::copydesk::finalize();

//      if(::window_sp::is_set() && ::window_sp::m_p->IsWindow())
      {
         //       bOk = ::window_sp::m_p->DestroyWindow() != FALSE;
      }
      //  else
      {
         bOk = false;
      }

      return bOk;

   }

   void copydesk::set_plain_text(const char * psz)
   {
///      ASSERT(m_p->IsWindow());
      //   int32_t iLen = 0;


      string str;

      str = ::str::international::utf8_to_unicode(psz);

      //ASSERT(m_p->IsWindow());
      //if(!m_p->OpenClipboard())
      {
         return;
      }

      _throw(todo(get_app()));

      /*

            EmptyClipboard();


            count iCount = ::str::international::utf8_to_unicode_count(str) + 1;
            HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, iCount * sizeof(WCHAR));
            unichar * lpwstrCopy  = (unichar *) ::GlobalLock(hglbCopy);
            ::str::international::utf8_to_unicode(lpwstrCopy, iCount, str);
            ::GlobalUnlock(hglbCopy);

            HGLOBAL hglbCopy2 = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(CHAR) * (strlen(psz) + 1));
            char * lpstrCopy  = (char *) ::GlobalLock(hglbCopy2);
            strcpy(lpstrCopy, psz);
            ::GlobalUnlock(hglbCopy2);


            SetClipboardData(CF_UNICODETEXT, hglbCopy);
            SetClipboardData(CF_TEXT, hglbCopy2);
            VERIFY(::CloseClipboard());

      */

   }


   string copydesk::get_plain_text()
   {

      return os_get_clipboard_text();

   }

   bool copydesk::desk_to_dib(::draw2d::dib * pdib)
   {
//      if(!m_p->OpenClipboard())
      //       return false;
      bool bOk = false;
      _throw(todo(get_app()));
      /* xxx
            HBITMAP hbitmap = (HBITMAP) ::GetClipboardData(CF_BITMAP);
            try
            {
               ::draw2d::bitmap_sp bitmap(get_app());
               bitmap->Attach(hbitmap);
               //HDC hdc = ::CreateCompatibleDC(NULL);
               //::draw2d::graphics_sp g(get_app());
               //g->Attach(hdc);
               //::draw2d::graphics * pgraphics = Application.graphics_from_os_data(hdc);
               //g->SelectObject(hbitmap);
             //  BITMAP bm;
               //::GetObjectA(hbitmap, sizeof(bm), &bm);
               //if(!pdib->create(bm.bmWidth, bm.bmHeight))
                 // return false;
               ::draw2d::graphics_sp g(get_app());
               g->SelectObject(bitmap);
               size sz = bitmap->GetBitmapDimension();
               if(pdib->create(sz))
               {
                  bOk = pdib->get_graphics()->BitBlt(0, 0, sz.cx, sz.cy, g, 0, 0, SRCCOPY) != FALSE;
               }
            }
            catch(...)
            {
            }
            ::DeleteObject((HGDIOBJ) hbitmap);
            //::DeleteDC(hdc);
            ::CloseClipboard();

      */

      return bOk;
   }


} // namespace linux




