#include "framework.h"
#include "linux_user_impl.h"
#include <X11/Xatom.h>
#include <sys/stat.h>
#include <X11/extensions/xf86vmode.h> // libxxf86vm-dev
#include <X11/extensions/Xrender.h>


void oswindow_set_active_window(oswindow oswindow);

CLASS_DECL_AURA void update_application_session_cursor(void * pvoidApp, POINT ptCursor);


// Tutor Exilius Q(t)List streaming contribution
mutex * g_pmutexX11 = NULL;
list < sp(object) > * g_prunnableptrlX11 = NULL;

WINBOOL _x11_get_cursor_pos(Display * d, LPPOINT lpptCursor);

mutex * g_pmutexX = NULL;


//int get_best_ordered_monitor(::user::interaction * pui, int & l, int & t, int & cx, int & cy);
//int get_best_monitor(::user::interaction * pui, int & l, int & t, int & cx, int & cy);

extern SnLauncheeContext* g_psncontext;
Display * x11_get_display();
void wm_toolwindow(oswindow w,bool bToolWindow);
void wm_state_hidden_raw(oswindow w, bool bSet);
CLASS_DECL_AURA int_bool mq_remove_window_from_all_queues(oswindow oswindow);


WINBOOL x11_get_cursor_pos(LPPOINT lpptCursor);


Window g_windowFocus = NULL;

#ifdef LINUX
int32_t _c_XErrorHandler(Display * display, XErrorEvent * perrorevent);
#endif


int g_iIgnoreXDisplayError = 0;


void x_display_error_trap_push(SnDisplay * sndisplay, Display * display)
{

   g_iIgnoreXDisplayError++;

}


void x_display_error_trap_pop(SnDisplay * sndisplay, Display * display)
{

   synch_lock sl(g_pmutexX);

   g_iIgnoreXDisplayError--;

   if(g_iIgnoreXDisplayError == 0)
   {

      XSync(display, false);

   }

}


int32_t _c_XErrorHandler(Display * display, XErrorEvent * perrorevent)
{

   return 0;

}



struct MWMHints
{

   unsigned long flags;
   unsigned long functions;
   unsigned long decorations;
   long input_mode;
   unsigned long status;

};


#define MWM_HINTS_DECORATIONS   (1L << 1)


/* MWM decorations values */
#define MWM_DECOR_NONE          0
#define MWM_DECOR_ALL           (1L << 0)
#define MWM_DECOR_BORDER        (1L << 1)
#define MWM_DECOR_RESIZEH       (1L << 2)
#define MWM_DECOR_TITLE         (1L << 3)
#define MWM_DECOR_MENU          (1L << 4)
#define MWM_DECOR_MINIMIZE      (1L << 5)
#define MWM_DECOR_MAXIMIZE      (1L << 6)




oswindow oswindow_defer_get(Window window)
{

   return oswindow_get(window);

}



bool oswindow_remove(Display * display, Window window)
{

   single_lock slOsWindow(::osdisplay_data::s_pmutex, true);

   int_ptr iFind = oswindow_find(display, window);

   if(iFind < 0)
   {

      return false;

   }

   delete ::oswindow_data::s_pdataptra->element_at(iFind);

   ::oswindow_data::s_pdataptra->remove_at(iFind);

   return true;

}


bool oswindow_remove_message_only_window(::user::interaction_impl * puibaseMessageOnlyWindow)
{

   single_lock slOsWindow(::osdisplay_data::s_pmutex, true);

   int_ptr iFind = oswindow_find_message_only_window(puibaseMessageOnlyWindow);

   if(iFind < 0)
   {

      return false;

   }

   delete ::oswindow_data::s_pdataptra->element_at(iFind);

   ::oswindow_data::s_pdataptra->remove_at(iFind);

   return true;

}


Atom get_window_long_atom(int32_t nIndex);

// Change _NET_WM_STATE if Window is Mapped
void mapped_net_state_raw(bool add, Display * d, Window w, int iScreen, Atom state1, Atom state2)
{

   synch_lock sl(g_pmutexX);

   XClientMessageEvent xclient;

#define _NET_WM_STATE_REMOVE        0    /* remove/unset property */
#define _NET_WM_STATE_ADD           1    /* add/set property */
#define _NET_WM_STATE_TOGGLE        2    /* toggle property  */

   ZERO(xclient);
   xclient.type = ClientMessage;
   xclient.window = w;
   xclient.message_type = XInternAtom(d, "_NET_WM_STATE", False);
   xclient.format = 32;
   xclient.data.l[0] = add ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
   xclient.data.l[1] = state1;
   xclient.data.l[2] = state2;
   xclient.data.l[3] = 1; /* source indication */
   xclient.data.l[4] = 0;

   XSendEvent (d, RootWindow(d, iScreen), False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent *)&xclient);

}


void unmapped_net_state_raw(Display * d, Window w, ...)
{

   synch_lock sl(g_pmutexX);

   XEvent xevent;

   unsigned int i;

   va_list argp;

   va_start(argp, w);

   ZERO(xevent);

   array < Atom > atoms;

   while(true)
   {

      Atom atom = va_arg(argp, int);

      if(atom == 0)
      {

         break;

      }

      atoms.add(atom);

   }

   if(atoms.has_elements())
   {

      XChangeProperty(d, w, XInternAtom(d, "_NET_WM_STATE", False),
                      XA_ATOM, 32, PropModeReplace,
                      (guchar*) atoms.get_data(), atoms.get_size());
   }
   else
   {

      XDeleteProperty(d, w, XInternAtom(d, "_NET_WM_STATE", False));

   }

   va_end(argp);

}


WINBOOL x11_get_window_rect(Display * d, Window window, LPRECT lprect)
{

   XWindowAttributes attrs;

   if(!XGetWindowAttributes(d, window, &attrs))
   {

      windowing_output_debug_string("\n::x11_get_window_rect 1.1");

      return FALSE;

   }

   int x = 0;

   int y = 0;

   int screen = XDefaultScreen((Display *) d);

   Window windowRoot = RootWindow((Display *) d, screen);

   Window child;

   XTranslateCoordinates( d, window, windowRoot, 0, 0, &x, &y, &child );

   lprect->left      = x + attrs.x;
   lprect->top       = y + attrs.y;
   lprect->right     = x + attrs.x    + attrs.width;
   lprect->bottom    = y + attrs.y    + attrs.height;


   windowing_output_debug_string("\n::x11_get_window_rect 2");

   return TRUE;

}


static oswindow g_oswindowCapture;


oswindow get_capture()
{

   return g_oswindowCapture;

}


oswindow set_capture(oswindow window)
{

   synch_lock sl(g_pmutexX);

   oswindow windowOld(g_oswindowCapture);

   if(window->display() == NULL)
   {

      return NULL;

   }

   if(window->window() == None)
   {

      return NULL;

   }

   windowing_output_debug_string("\noswindow_data::SetCapture 1");

   xdisplay d(window->display());

   if(XGrabPointer(d, window->window(), False, ButtonPressMask | ButtonReleaseMask | PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime) == GrabSuccess)
   {

      g_oswindowCapture = window;

      windowing_output_debug_string("\noswindow_data::SetCapture 2");

      return windowOld;

   }

   windowing_output_debug_string("\noswindow_data::SetCapture 2.1");

   return NULL;

}


WINBOOL release_capture()
{

   synch_lock sl(g_pmutexX);

//    if(g_oswindowCapture == NULL)
//    {

//       return FALSE;

//    }

   windowing_output_debug_string("\noswindow_data::ReleaseCapture 1");

   xdisplay d(x11_get_display());

   WINBOOL bRet = XUngrabPointer(d, CurrentTime) != FALSE;

   //if(bRet)
   {

      g_oswindowCapture = NULL;

   }

   windowing_output_debug_string("\noswindow_data::ReleaseCapture 2");

   return bRet;

}


oswindow set_focus(oswindow window)
{

   synch_lock sl(g_pmutexX);

   if(window == NULL)
   {

      return NULL;

   }

   windowing_output_debug_string("\noswindow_data::SetFocus 1");

   xdisplay d(window->display());

   if(!is_window(window))
   {

      windowing_output_debug_string("\noswindow_data::SetFocus 1.1");

      return NULL;

   }

   oswindow windowOld = ::get_focus();

   if(!IsWindowVisibleRaw(window))
   {

      windowing_output_debug_string("\noswindow_data::SetFocus 1.2");

      return NULL;

   }

   if(!XSetInputFocus(d, window->window(), RevertToNone, CurrentTime))
   {

      windowing_output_debug_string("\noswindow_data::SetFocus 1.3");

      return NULL;

   }

   windowing_output_debug_string("\noswindow_data::SetFocus 2");

   return windowOld;

}


void x11_check_status(int status, unsigned long window)
{
    if (status == BadWindow) {
        printf("window id # 0x%lx does not exists!", window);
     //   exit(1);
    }

    if (status != Success) {
        printf("XGetWindowProperty failed!");
       // exit(2);
    }
}
#define MAXSTR 1000
unsigned char* x11_get_string_property(Display * display, Window window, char* property_name)
{

   unsigned char * prop;
   Atom actual_type, filter_atom;
   int actual_format, status;
   unsigned long nitems, bytes_after;

   filter_atom = XInternAtom(display, property_name, True);
   status = XGetWindowProperty(display, window, filter_atom, 0, MAXSTR, False, AnyPropertyType,
                                &actual_type, &actual_format, &nitems, &bytes_after, &prop);
   x11_check_status(status, window);
   return prop;

}

unsigned long x11_get_long_property(Display * d, Window w, char* property_name)
{
   unsigned char * prop =x11_get_string_property(d, w, property_name);
   unsigned long long_property = prop[0] + (prop[1]<<8) + (prop[2]<<16) + (prop[3]<<24);
   return long_property;
}

string x11_get_name(Display * display, Window w)
{

string str;

char* name = '\0';
int status = XFetchName(display, w, &name);
if (status >= Success)
{
    str =name;
}
XFree(name);
return str;

}




oswindow get_active_window()
{

   synch_lock sl(g_pmutexX);

   oswindow pwindow = NULL;

   windowing_output_debug_string("\n::GetFocus 1");

   b_prevent_xdisplay_lock_log = false;

   xdisplay d(x11_get_display());

   windowing_output_debug_string("\n::GetFocus 1.01");

   if(d.is_null())
   {

      windowing_output_debug_string("\n::GetFocus 1.1");

      return pwindow;

   }

   int screen = XDefaultScreen((Display *) d);

   Window windowRoot = RootWindow((Display *) d, screen);

   Window window = x11_get_long_property(d, windowRoot, "_NET_ACTIVE_WINDOW");

   pwindow = oswindow_defer_get(window);

   windowing_output_debug_string("\n::GetActiveWindow 2");

   return pwindow;

}


Window * x11_window_list(Display *disp, unsigned long * len)
{

   Atom prop = XInternAtom(disp,"_NET_CLIENT_LIST_STACKING",False);

   if(prop == NULL)
   {

      prop = XInternAtom(disp,"_NET_CLIENT_LIST",False);

   }
   if(prop == NULL)
   {

      return NULL;

   }

   Atom type;
   int form;
   unsigned long remain;
   unsigned char *list;

   errno = 0;
   if (XGetWindowProperty(disp,XDefaultRootWindow(disp),prop,0,1024,False,XA_WINDOW,
                &type,&form,len,&remain,&list) != Success) {
        output_debug_string("winlist() -- GetWinProp");
        return NULL;
    }

    return (Window*)list;
}



bool x11_window_list(Display *disp, array < Window > & windowa)
{

   unsigned long len = 0;

   Window * list = (Window*)x11_window_list(disp,&len);


   if(list == NULL)
   {

      return false;

   }

   for (int i=0;i<(int)len;i++)
   {

      windowa.add(list[i]);

   }

   XFree(list);

   return true;

}

int_bool is_window_occluded(oswindow oswindow)
{


   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::GetFocus 1");

   b_prevent_xdisplay_lock_log = false;

   xdisplay d(oswindow->display());

   windowing_output_debug_string("\n::GetFocus 1.01");

   if(d.is_null())
   {

      windowing_output_debug_string("\n::GetFocus 1.1");

      return false;

   }

   comparable_array < Window > windowa;

   if(!x11_window_list(d, windowa))
   {

      return true;

   }

   if(windowa.last() == oswindow->window())
   {

      return false;

   }

   index iFind = windowa.find_last(oswindow->window());

   if(iFind < 0)
   {

      return true;

   }

   ::rect r;

   x11_get_window_rect(d, oswindow->window(), r);

   r = oswindow->m_pimpl->m_rectParentClient;

   string strTopic = x11_get_name(d, oswindow->window());

   ::rect rTest;

   for(iFind++; iFind < windowa.get_size(); iFind++)
   {

   string strItem = x11_get_name(d, windowa[iFind]);
      ::rect rHigher;

      if(x11_get_window_rect(d, windowa[iFind], rHigher))
      {

         if(rTest.intersect(rHigher, r))
         {

            return true;

         }

      }

   }


   return false;

}


oswindow get_focus()
{

   synch_lock sl(g_pmutexX);

   oswindow pwindow = NULL;

   windowing_output_debug_string("\n::GetFocus 1");

   b_prevent_xdisplay_lock_log = false;

   xdisplay d(x11_get_display());

   windowing_output_debug_string("\n::GetFocus 1.01");

   if(d.is_null())
   {

      windowing_output_debug_string("\n::GetFocus 1.1");

      return NULL;

   }

   Window window = None;

   int revert_to = 0;

   bool bOk = XGetInputFocus(d, &window, &revert_to) != 0;

   if(!bOk)
   {

      windowing_output_debug_string("\n::GetFocus 1.2");

      return NULL;

   }

   if(window == None || window == PointerRoot)
   {

      windowing_output_debug_string("\n::GetFocus 1.3");

      return NULL;

   }

   pwindow = oswindow_defer_get(window);

   windowing_output_debug_string("\n::GetFocus 2");

   return pwindow;

}


oswindow set_active_window(oswindow window)
{

   synch_lock sl(g_pmutexX);

   {

      windowing_output_debug_string("\n::set_active_window 1");

      xdisplay d(window->display());

      XEvent xev;

      ZERO(xev);

      Window windowRoot = window->root_window_raw();

      Atom atomActiveWindow = d.intern_atom("_NET_ACTIVE_WINDOW", False);

      xev.xclient.type = ClientMessage;
      xev.xclient.send_event = True;
      xev.xclient.display = d;
      xev.xclient.window = window->window();
      xev.xclient.message_type = atomActiveWindow;
      xev.xclient.format = 32;
      xev.xclient.data.l[0] = 1;
      xev.xclient.data.l[1] = 0;
      xev.xclient.data.l[2] = 0;
      xev.xclient.data.l[3] = 0;
      xev.xclient.data.l[4] = 0;

      XSendEvent (d, windowRoot, False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);

      windowing_output_debug_string("\n::set_active_window 2");

   }

   return ::set_focus(window);

}


oswindow oswindow_get_next_found(Display * pdisplay, long *array, int iStart, int numItems)
{

   for(index i = iStart; i < numItems; i++)
   {

      if(::oswindow_find(array[i]) >= 0)
      {

         return ::oswindow_get(array[i]);

      }

   }

   return NULL;

}


oswindow oswindow_get_previous_found(Display * pdisplay, long *array, int iStart)
{

   for(index i = iStart; i >= 0; i--)
   {

      if(::oswindow_find(array[i]) >= 0)
      {

         return ::oswindow_get(array[i]);

      }

   }

   return NULL;

}


oswindow oswindow_get_next_found(Window *array, int iStart, int numItems)
{

   for(index i = iStart; i < numItems; i++)
   {

      if(::oswindow_find(array[i]) >= 0)
      {

         return ::oswindow_get(array[i]);

      }

   }

   return NULL;

}


oswindow oswindow_get_previous_found(Window *array, int iStart)
{

   for(index i = iStart; i >= 0; i--)
   {

      if(::oswindow_find(array[i]) >= 0)
      {

         return ::oswindow_get(array[i]);

      }

   }

   return NULL;

}


oswindow get_window(oswindow windowParam, int iParentHood)
{

   synch_lock sl(g_pmutexX);

   oswindow window = windowParam;

   if(window == NULL)
   {

      return NULL;

   }

   windowing_output_debug_string("\n::get_window 1");

   xdisplay d(window->display());

   Window w = window->window();

   if(iParentHood == GW_HWNDFIRST || iParentHood == GW_HWNDLAST || iParentHood == GW_HWNDNEXT || iParentHood == GW_HWNDPREV)
   {

      window = ::GetParent(window);

      if(window == NULL)
      {

         window = windowParam;

         w = window->get_parent_handle();

         Atom a = XInternAtom(windowParam->display(), "_NET_CLIENT_LIST_STACKING", False);

         Atom actualType;

         int format;

         unsigned long numItems, bytesAfter;

         unsigned char *data =0;

         int status = XGetWindowProperty(
                      windowParam->display(),
                      RootWindow(windowParam->display(), windowParam->m_iScreen),
                      a,
                      0L,
                      1024,
                      false,
                      AnyPropertyType,
                      &actualType,
                      &format,
                      &numItems,
                      &bytesAfter,
                      &data);

         if (status >= Success && numItems)
         {

            long * array = (long*) data;

            switch(iParentHood)
            {
            case GW_CHILD:
            case GW_HWNDFIRST:
            {

               if(data == NULL)
               {

                  windowing_output_debug_string("\n::get_window 2");

                  return NULL;

               }

               window = oswindow_get_next_found(windowParam->display(), array, 0, numItems);

            }
            break;
            case GW_HWNDLAST:
            {

               if(data == NULL)
               {

                  windowing_output_debug_string("\n::get_window 3");

                  return NULL;

               }

               window = oswindow_get_previous_found(windowParam->display(), array, numItems - 1);

            }
            break;
            case GW_HWNDNEXT:
            case GW_HWNDPREV:
            {

               if(data == NULL) // ????
               {

                  windowing_output_debug_string("\n::get_window 4");

                  return NULL;

               }

               int iFound = -1;

               for(int i = 0; i < numItems; i++)
               {

                  if(array[i] == windowParam->window())
                  {

                     iFound = i;

                     break;

                  }

               }

               if(iFound < 0)
               {

                  XFree(data);

                  windowing_output_debug_string("\n::get_window 5");

                  return NULL;

               }

               if(iParentHood == GW_HWNDNEXT)
               {

                  if(iFound + 1 >= numItems)
                  {

                     XFree(data);

                     windowing_output_debug_string("\n::get_window 6");

                     return NULL;

                  }

                  window = ::oswindow_get_next_found(windowParam->display(), array, iFound + 1, numItems);

               }
               else
               {

                  if(iFound - 1 < 0)
                  {

                     XFree(data);

                     windowing_output_debug_string("\n::get_window 7");

                     return NULL;

                  }

                  window = ::oswindow_get_previous_found(windowParam->display(), array, iFound - 1);

               }

            }

            }

            XFree(data);

            windowing_output_debug_string("\n::get_window 8");

            return window;

         }
      }

   }

   Window root = 0;
   Window parent = 0;
   Window * pchildren = NULL;
   uint32_t ncount = 0;


   XQueryTree(window->display(), w, &root, &parent, &pchildren, &ncount);

   switch(iParentHood)
   {
   case GW_CHILD:
   case GW_HWNDFIRST:
   {

      if(pchildren == NULL)
      {

         windowing_output_debug_string("\n::get_window 9");

         return NULL;

      }

      window = ::oswindow_get_next_found(pchildren, 0, ncount);

   }
   break;
   case GW_HWNDLAST:
   {

      if(pchildren == NULL)
      {

         windowing_output_debug_string("\n::get_window 10");

         return NULL;

      }

      window = ::oswindow_get_previous_found(pchildren, ncount - 1);

   }
   break;
   case GW_HWNDNEXT:
   case GW_HWNDPREV:
   {

      if(pchildren == NULL) // ????
      {

         windowing_output_debug_string("\n::get_window 11");

         return NULL;

      }

      int iFound = -1;

      for(int i = 0; i < ncount; i++)
      {
         if(pchildren[i] == windowParam->window())
         {
            iFound = i;
            break;
         }
      }

      if(iFound < 0)
      {

         windowing_output_debug_string("\n::get_window 12");

         return NULL;

      }

      if(iParentHood == GW_HWNDNEXT)
      {

         if(iFound + 1 >= ncount)
         {

            windowing_output_debug_string("\n::get_window 13");

            return NULL;

         }

         window = ::oswindow_get_next_found(pchildren, iFound + 1, ncount);

      }
      else
      {

         if(iFound - 1 < 0)
         {

            windowing_output_debug_string("\n::get_window 14");

            return NULL;

         }

         window = ::oswindow_get_previous_found(pchildren, iFound - 1);

      }

   }

   }

   if(pchildren != NULL)
      XFree(pchildren);

   windowing_output_debug_string("\n::get_window 15");

   return window;

}



WINBOOL destroy_window(oswindow window)
{

   synch_lock sl(g_pmutexX);

   //single_lock sl(&user_mutex(), true);

   if(!is_window(window) && !window->is_destroying())
      return FALSE;

   Display * pdisplay = window->display();

   Window win = window->window();

   oswindow_data * pdata = (oswindow_data *) (void *) window;

   bool bIs = is_window(window);

   sp(::user::interaction) pui = window->m_pimpl->m_pui;

   pui->send_message(WM_DESTROY, 0, 0);

   mq_remove_window_from_all_queues(window);

   pui->send_message(WM_NCDESTROY, 0, 0);

   oswindow_remove(window->m_pimpl);

   oswindow_remove(pdisplay, win);

   windowing_output_debug_string("\n::DestroyWindow 1");

   xdisplay d(pdisplay);

   XUnmapWindow(pdisplay, win);

   XDestroyWindow(pdisplay, win);

   windowing_output_debug_string("\n::DestroyWindow 2");

   return true;

}


WINBOOL is_window(oswindow oswindow)
{

   if(::oswindow_data::s_pdataptra->find_first(oswindow) < 0)
   {

      return FALSE;

   }

   return TRUE;

}


oswindow g_oswindowDesktop;


Display * x11_get_display();


bool c_xstart()
{

   if(!XInitThreads())
   {

      return false;

   }

   Display * dpy = x11_get_display();

   g_oswindowDesktop = oswindow_get(dpy, DefaultRootWindow(dpy));

   XSelectInput(g_oswindowDesktop->display(), g_oswindowDesktop->window(), StructureNotifyMask);

   return true;

}


oswindow get_desktop_window()
{

   return g_oswindowDesktop;

}


/* Copyright (c) 2012 the authors listed at the following URL, and/or
the authors of referenced articles or incorporated external code:
http://en.literateprograms.org/Hello_World_(C,_Cairo)?action=history&offset=20070528220552

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to uxse, cxopy, mxodify, mxerge, pxublish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Retrieved from: http://en.literateprograms.org/Hello_World_(C,_Cairo)?oldid=10388
*/


#define SIZEX 100
#define SIZEY  50


void message_box_paint(::draw2d::graphics * pgraphics, stringa & stra, bool_array  & baTab, int_array  & ya,SIZE * psize)
{

   synch_lock sl(g_pmutexX);

   pgraphics->fill_solid_rect(::rect(*psize), RGB(84, 90, 80));

   draw2d::brush_sp pen(pgraphics->allocer());

   pen->create_solid(0);

   for(index i = 0; i < stra.get_count(); i++)
   {
      pgraphics->text_out(10.0 + 50.0 + (baTab[i] ? 25.0 : 0), 10.0 + 50.0 + ya[i], stra[i]);
   }



}

#define _NET_WM_STATE_REMOVE        0    // remove/unset property
#define _NET_WM_STATE_ADD           1    // add/set property
#define _NET_WM_STATE_TOGGLE        2    // toggle property

Atom * wm_get_list_raw(oswindow w, Atom atomList, unsigned long int * items);
int wm_test_state(oswindow w, const char * pszNetStateFlag);
int wm_test_state_raw(oswindow w, const char * pszNetStateFlag);
int wm_test_list_raw(oswindow w, Atom atomList, Atom atomFlag);
bool wm_add_remove_list_raw(oswindow w, Atom atomList, Atom atomFlag, bool bSet);


void wm_add_remove_state_mapped_raw(oswindow w, const char * pszNetStateFlag, bool bSet)
{

   synch_lock sl(g_pmutexX);

   Display * display = w->display();

   Window window = w->window();

   Window windowRoot = DefaultRootWindow(display);

   Atom atomFlag = XInternAtom(display, pszNetStateFlag, true);

   if(atomFlag == None )
   {

      windowing_output_debug_string("ERROR: cannot find atom for " + string(pszNetStateFlag) + "!\n");

      return;

   }

   Atom atomNetState = XInternAtom(display, "_NET_WM_STATE", false);

   if( atomNetState == None )
   {

      windowing_output_debug_string("ERROR: cannot find atom for _NET_WM_STATE !\n");

      return;

   }

   if(wm_test_list_raw(w, atomNetState, atomFlag))
   {

      if(bSet)
      {

         return;

      }

   }
   else
   {

      if(!bSet)
      {

         return;

      }

   }

   XClientMessageEvent xclient;

   ZERO(xclient);

   xclient.type            = ClientMessage;
   xclient.window          = window;
   xclient.message_type    = atomNetState;
   xclient.format          = 32;
   xclient.data.l[0]       = bSet ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
   xclient.data.l[1]       = atomFlag;
   xclient.data.l[2]       = 0;
   xclient.data.l[3]       = 0;
   xclient.data.l[4]       = 0;

   XSendEvent(display, windowRoot, False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent *)&xclient );

}


void wm_add_remove_state_mapped(oswindow w, const char * pszNetStateFlag, bool bSet)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::wm_add_remove_state_mapped 1");

   xdisplay d(w->display());

   wm_add_remove_state_mapped_raw(w, pszNetStateFlag, bSet);

   windowing_output_debug_string("\n::wm_add_remove_state_mapped 2");

}


void wm_add_remove_state_unmapped_raw(oswindow w, const char * pszNetStateFlag, bool bSet)
{

   synch_lock sl(g_pmutexX);

   Display * display = w->display();

   Window window = w->window();

   int iScreen = DefaultScreen(display);

   Window windowRoot = RootWindow(display, iScreen);

   Atom atomFlag = XInternAtom(display, pszNetStateFlag, 1);

   if(atomFlag == None)
   {

      windowing_output_debug_string("ERROR: cannot find atom for " + string(pszNetStateFlag) + "!\n");

      return;

   }

   Atom atomNetState = XInternAtom(display, "_NET_WM_STATE", 1);

   if( atomNetState == None )
   {

      windowing_output_debug_string("ERROR: cannot find atom for _NET_WM_STATE !\n");

      return;

   }

   wm_add_remove_list_raw(w, atomNetState, atomFlag, bSet);

}


void wm_add_remove_state_unmapped(oswindow w, const char * pszNetStateFlag, bool bSet)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::wm_add_remove_state_unmapped 1");

   xdisplay d(w->display());

   wm_add_remove_state_unmapped_raw(w, pszNetStateFlag, bSet);

   windowing_output_debug_string("\n::wm_add_remove_state_unmapped 2");

}


void wm_add_remove_state_raw(oswindow w, const char * pszState, bool bSet)
{

   synch_lock sl(g_pmutexX);

   if(IsWindowVisibleRaw(w))
   {

      wm_add_remove_state_mapped_raw(w, pszState, bSet);

   }
   else
   {

      wm_add_remove_state_unmapped_raw(w, pszState, bSet);

   }

}


void wm_add_remove_state(oswindow w, const char * pszState, bool bSet)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::wm_add_remove_state 1");

   xdisplay d(w->display());

   wm_add_remove_state_raw(w, pszState, bSet);

   windowing_output_debug_string("\n::wm_add_remove_state 2");

}


void wm_state_above_raw(oswindow w, bool bSet)
{

   synch_lock sl(g_pmutexX);

   wm_add_remove_state_raw(w, "_NET_WM_STATE_HIDDEN", false);

   wm_add_remove_state_raw(w, "_NET_WM_STATE_BELOW", false);

   wm_add_remove_state_raw(w, "_NET_WM_STATE_ABOVE", bSet);

}


void wm_state_below_raw(oswindow w, bool bSet)
{

   synch_lock sl(g_pmutexX);

   wm_add_remove_state_raw(w, "_NET_WM_STATE_HIDDEN", false);

   wm_add_remove_state_raw(w, "_NET_WM_STATE_ABOVE", false);

   wm_add_remove_state_raw(w, "_NET_WM_STATE_BELOW", bSet);

}


void wm_state_hidden_raw(oswindow w, bool bSet)
{

   synch_lock sl(g_pmutexX);

   wm_add_remove_state_raw(w, "_NET_WM_STATE_BELOW", false);

   wm_add_remove_state_raw(w, "_NET_WM_STATE_ABOVE", false);

   wm_add_remove_state_raw(w, "_NET_WM_STATE_HIDDEN", bSet);

}


void wm_state_above(oswindow w, bool bSet)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::wm_state_above 1");

   xdisplay d(w->display());

   wm_state_above_raw(w, bSet);

   windowing_output_debug_string("\n::wm_state_above 2");

}


void wm_state_below(oswindow w, bool bSet)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::wm_state_below 1");

   xdisplay d(w->display());

   wm_state_above_raw(w, bSet);

   windowing_output_debug_string("\n::wm_state_below 2");

}


void wm_state_hidden(oswindow w, bool bSet)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::wm_state_hidden 1");

   xdisplay d(w->display());

   wm_state_hidden_raw(w, bSet);

   windowing_output_debug_string("\n::wm_state_hidden 2");

}


void wm_toolwindow(oswindow w, bool bToolWindow)
{

   sync_x11([=]()
   {

      windowing_output_debug_string("\n::wm_toolwindow 1");

      xdisplay d(w->display());

      if(d.is_null())
      {

         windowing_output_debug_string("\n::wm_toolwindow 1.1");

         fflush(stdout);

         return;

      }

      Window window = w->window();

      Window windowRoot = d.default_root_window();

      Atom atomWindowType = d.intern_atom("_NET_WM_WINDOW_TYPE", False);

      if(atomWindowType != None)
      {

         Atom atomWindowTypeValue;

         if(bToolWindow)
         {

            atomWindowTypeValue = d.intern_atom("_NET_WM_WINDOW_TYPE_SPLASH", False);

         }
         else
         {

            atomWindowTypeValue = d.intern_atom("_NET_WM_WINDOW_TYPE_NORMAL", False);

         }

         if(atomWindowType != None)
         {

            XChangeProperty(d, window, atomWindowType, XA_ATOM, 32, PropModeReplace, (unsigned char *) &atomWindowTypeValue, 1);

         }

      }

      wm_add_remove_state_raw(w, "_NET_WM_STATE_SKIP_TASKBAR", bToolWindow);

      windowing_output_debug_string("\n::wm_toolwindow 2");

   });

}


void wm_nodecorations(oswindow w, int bMap)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::wm_nodecorations 1");

   xdisplay d(w->display());

   if(d.is_null())
   {

      return;

   }

   Window window = w->window();

   Window windowRoot = d.default_root_window();

   Atom atomMotifHints = d.intern_atom("_MOTIF_WM_HINTS", True);

   if(atomMotifHints != None)
   {

      MWMHints hints = {MWM_HINTS_DECORATIONS, 0, MWM_DECOR_NONE, 0, 0};

      XChangeProperty(d, window, atomMotifHints, atomMotifHints, 32, PropModeReplace, (unsigned char *)&hints, sizeof(MWMHints) / 4);

   }

   if(bMap)
   {

      XUnmapWindow(d, window);

      XMapWindow(d, window);

   }

   windowing_output_debug_string("\n::wm_nodecorations 2");

}


WINBOOL IsWindowVisibleRaw(Display * display, Window window);


void wm_iconify_window(oswindow w)
{

   synch_lock sl(g_pmutexX);

   xdisplay d(w->display());

   windowing_output_debug_string("\n::wm_iconify_window 1");

   if(d.is_null())
   {

      windowing_output_debug_string("\n::wm_iconify_window 1.1");

      return;

   }

   Window window = w->window();

   int iScreen = DefaultScreen(d.operator Display *());

   if(IsWindowVisibleRaw(d, window))
   {

      XIconifyWindow(d, window, iScreen);

   }
   else
   {

      if(w->m_pimpl->m_pui->get_appearance() !=::user::appearance_iconic)
      {

         w->m_pimpl->m_pui->set_appearance(::user::appearance_iconic);

      }

      unmapped_net_state_raw(d, window, d.intern_atom("_NET_WM_STATE_HIDDEN", False));

   }

   windowing_output_debug_string("\n::wm_iconify_window 2");

}


WINBOOL IsWindowVisibleRaw(Display * display, Window window)
{

   synch_lock sl(g_pmutexX);

   XWindowAttributes attr;

   if(!XGetWindowAttributes(display, window, &attr))
   {

      return false;

   }

   return attr.map_state == IsViewable;

}


WINBOOL IsWindowVisibleRaw(oswindow w)
{

   synch_lock sl(g_pmutexX);

   Display * display = w->display();

   Window window = w->window();

   return IsWindowVisibleRaw(display, window);

}


Atom * wm_get_list_raw(oswindow w, Atom atomList, unsigned long int * pnum_items)
{

   synch_lock sl(g_pmutexX);

   if(atomList == None )
   {

      return NULL;

   }

   Display * display = w->display();

   Window window = w->window();

   Atom actual_type;

   int actual_format;

   unsigned long int bytes_after;

   Atom *atoms = NULL;

   XGetWindowProperty(display, window, atomList, 0, 1024, False, XA_ATOM, &actual_type, &actual_format, pnum_items, &bytes_after, (unsigned char**)&atoms);

   return atoms;

}


int wm_test_list_raw(oswindow w, Atom atomList, Atom atomFlag)
{

   synch_lock sl(g_pmutexX);

   Display * display = w->display();

   Window window = w->window();

   Atom actual_type;

   int actual_format;

   unsigned long i, num_items;

   Atom *atoms = wm_get_list_raw(w, atomList, &num_items);

   if(atoms == NULL)
   {

      return 0;

   }

   bool bFind = false;

   for(i = 0; i < num_items; i++)
   {

      if(atoms[i] == atomFlag)
      {

         bFind = true;

         break;

      }

   }

   XFree(atoms);

   return bFind ? 1 : 0;

}


int wm_test_state_raw(oswindow w, const char * pszNetStateFlag)
{

   synch_lock sl(g_pmutexX);

   Atom atomFlag = XInternAtom(w->display(), pszNetStateFlag, 1);

   if( atomFlag == None )
   {

      windowing_output_debug_string("ERROR: cannot find atom for " + string(pszNetStateFlag) + "!\n");

      return 0;

   }

   Atom atomNetState = XInternAtom(w->display(), "_NET_WM_STATE", 1);

   if( atomNetState == None )
   {

      windowing_output_debug_string("ERROR: cannot find atom for _NET_WM_STATE !\n");

      return 0;

   }

   return wm_test_list_raw(w, atomNetState, atomFlag);

}


int wm_test_state(oswindow w, const char * pszNetStateFlag)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::wm_test_state 1");

   xdisplay d(w->display());

   if(d.is_null())
   {

      windowing_output_debug_string("\n::wm_test_state 1.1");

      return 0;

   }

   int i = wm_test_state_raw(w, pszNetStateFlag);

   windowing_output_debug_string("\n::wm_test_state 2");

   return i;

}


bool wm_add_remove_list_raw(oswindow w, Atom atomList, Atom atomFlag, bool bSet)
{

   synch_lock sl(g_pmutexX);

   if( atomFlag == None )
   {

      return false;

   }

   if( atomList == None )
   {

      return false;

   }

   Display * display = w->display();

   Window window = w->window();

   int scr = DefaultScreen(display);

   Window rootw = RootWindow(display,scr);

   if(bSet)
   {

      if(!wm_test_list_raw(w, atomList, atomFlag))
      {

         XChangeProperty(display,window,atomList,XA_ATOM,32,PropModeAppend, (unsigned char *)&atomFlag,1);

      }

   }
   else
   {

      unsigned long num_items;

      Atom * plist = wm_get_list_raw(w, atomList, &num_items);

      if(plist == NULL)
      {

         return false;

      }

      int iFind = -1;

      int i;

      for(i = 0; i < num_items; i++)
      {

         if(plist[i] == atomFlag)
         {

            iFind = i;

            break;

         }

      }

      if(iFind >= 0)
      {

         memmove(&plist[iFind], &plist[iFind + 1], (num_items - iFind - 1) *sizeof(Atom));

         XChangeProperty(display,window,atomList,XA_ATOM,32,PropModeReplace, (unsigned char *)plist,num_items-1);

      }

      XFree(plist);

   }

   return true;

}


::user::interaction * oswindow_interaction(oswindow oswindow)
{

   if (is_ptr_null(oswindow, sizeof(*oswindow)))
   {

      return NULL;

   }

   if (is_ptr_null(oswindow->m_pimpl, sizeof(*oswindow->m_pimpl)))
   {

      return NULL;

   }

   return oswindow->m_pimpl->m_pui;

}


::user::interaction_impl * oswindow_get(oswindow oswindow)
{

   if (is_null(oswindow))
   {

      return NULL;

   }

#ifdef WINDOWSEX

   cslock slOsWindow(::user::g_pcsImpl);

   return ::user::g_pmapImpl->operator[](oswindow);

#else

   return oswindow->m_pimpl;

#endif

}


bool x11_process_message(osdisplay_data * pdata, Display * pdisplay);
void x11_post_message(MESSAGE & msg);


bool g_bSkipMouseMessageInXcess = true;
DWORD g_dwLastMotion = 0;
DWORD g_dwMotionSkipTimeout = 23;


extern bool b_prevent_xdisplay_lock_log;

void x11_idle(int iSleep)
{

   synch_lock sl(g_pmutexX11);

   if(g_prunnableptrlX11->has_elements())
   {

      do
      {

         sp(object) pobject = g_prunnableptrlX11->pop_front();

         sl.unlock();

         pobject->run();

         pobject->interlockedlong()++;

         sl.lock();

      }
      while(g_prunnableptrlX11->has_elements());

   }
   else
   {

      Sleep(iSleep);

      {

         point ptCursor(0, 0);

         if(x11_get_cursor_pos(&ptCursor))
         {

            synch_lock sl(::oswindow_data::s_pmutex);

            for(auto & p : *::oswindow_data::s_pdataptra)
            {

               update_application_session_cursor(p->m_pimpl->get_app(), ptCursor);

            }

         }

      }

      oswindow_set_active_window(get_active_window());

   }


}


void __axis_x11_thread(osdisplay_data * pdata)
{

   Display * display = pdata->display();

   if(display == NULL)
   {

      return;

   }

   int x11_fd = ConnectionNumber(display);

   fd_set in_fds;

   MESSAGE msg;

   struct timeval tv;

   bool bPending;

   bool bCa2Processed = true;

//   {
//
//      xdisplay d(display);
//
//
//bool      bPending = XPending(display);
//
////      XSetWindowAttributes attributes = {};
//
////      attributes.event_mask = PropertyChangeMask;
//
////      XChangeWindowAttributes(d, d.default_root_window(), CWEventMask, &attributes);
//
//      XSelectInput(d, d.default_root_window(), PropertyChangeMask);
//
//   }

   XEvent e;

   while(::aura::system::g_p != NULL && ::get_thread_run())
   {

      bPending = false;

      {

         synch_lock sl(g_pmutexX);

         {

            b_prevent_xdisplay_lock_log = true;

            windowing_output_debug_string("\n::__axis_x11_thread 1");

            xdisplay d(display);

            bPending = XPending(display);

            b_prevent_xdisplay_lock_log = false;

            windowing_output_debug_string("\n::__axis_x11_thread 2");

         }

         if(bPending)
         {

            if(!bCa2Processed)
            {

               windowing_output_debug_string("\n::__axis_x11_thread A");

               xdisplay d(display);

               XNextEvent(display, &e);

               windowing_output_debug_string("\n::__axis_x11_thread B");

            }

            bCa2Processed = x11_process_message(pdata, display);

            sl.unlock();

            if(!bCa2Processed)
            {

               x11_idle(50);

            }

         }
         else
         {

            sl.unlock();

            bCa2Processed = true;

            // Attention: Game
            // TODO: implement ability to change the event check time resolution.
            x11_idle(50);

         }

      }

   }

}


extern bool b_prevent_xdisplay_lock_log;


bool x11_process_message(osdisplay_data * pdata, Display * display)
{

   synch_lock sl(g_pmutexX);

   XEvent e;

   MESSAGE msg;

   ZERO(msg);

   {

      windowing_output_debug_string("\n::process_message 1");

      b_prevent_xdisplay_lock_log = true;

      xdisplay d(display);

      // confirm Pending

      bool bReallyPending = XPending(d);

      if(!bReallyPending)
      {

         return true;

      }

      XNextEvent(display, &e);

      b_prevent_xdisplay_lock_log = false;

      windowing_output_debug_string("\n::process_message 2");

   }

   bool bRet = false;

   msg.hwnd = oswindow_defer_get(display, e.xany.window);

   msg.time = get_tick_count();

   if(msg.hwnd == NULL)
   {

      windowing_output_debug_string("\n::process_message A");

      xdisplay d(display);

      // not going to process

      windowing_output_debug_string("!");

      // put back

      XPutBackEvent(display, &e);

      // not processed

      windowing_output_debug_string("\n::process_message B");

      return false;

   }

   if(e.type == Expose)
   {

      if(e.xexpose.count == 0)
      {

         msg.message       = WM_PAINT;
         msg.hwnd          = oswindow_get(display, e.xexpose.window);
         msg.lParam        = 0;
         msg.wParam        = 0;

         x11_post_message(msg);

      }

   }
   else if(e.type == PropertyNotify)
   {

//      {
//
//         xdisplay d(display);
//
//         if(e.xany.window == d.default_root_window())
//         {
//
//            Atom atom = XInternAtom(d, "_NET_ACTIVE_WINDOW", False);
//
//            if(atom == e.xproperty.atom)
//            {
//
//               oswindow_set_active_window(get_active_window());
//
//            }
//
//            return true;
//
//         }
//
//      }

      if(msg.hwnd != NULL && msg.hwnd->m_pimpl != NULL)
      {

         ::user::interaction * pui = msg.hwnd->m_pimpl->m_pui;

         if(pui != NULL)
         {

            ::user::interaction_impl_base * pimpl = pui->m_pimpl;

            bool bHandled = false;

            if(pimpl != NULL)
            {

               if(pui->m_eappearance == ::user::appearance_iconic && !msg.hwnd->is_iconic())
               {

                  //file_put_contents_dup("/home/camilo/xxx.txt", "");

                  // 1111111111111111111111111111111111111111111

                  pui->m_eappearance = ::user::appearance_none;

                  pui->fork([=]()
                  {

                     if(pui->m_eappearanceBefore == ::user::appearance_iconic)
                     {

                        pui->_001OnDeiconify(::user::appearance_normal);

                     }
                     else
                     {

                        pui->_001OnDeiconify(pui->m_eappearanceBefore);

                     }

                  });

                  bHandled = true;

               }

               if(pui->m_eappearance != ::user::appearance_iconic
                     && pui->m_eappearance != ::user::appearance_none
                     && msg.hwnd->is_iconic())
               {

                  pui->set_appearance(::user::appearance_iconic);

               }

               if(pui->m_eappearanceRequest == ::user::appearance_full_screen
                     && pui->m_eappearance != ::user::appearance_full_screen
                     && !msg.hwnd->is_iconic())
               {

                  pui->set_appearance(::user::appearance_full_screen);

               }

            }

         }

      }

   }
   else if(e.type == MapNotify || e.type == UnmapNotify)
   {

      msg.message       = WM_SHOWWINDOW;
      msg.wParam        = e.type == MapNotify;
      msg.lParam        = 0;

      x11_post_message(msg);

      if(e.type == MapNotify)
      {

         if(msg.hwnd != NULL && msg.hwnd->m_pimpl != NULL)
         {

            ::user::interaction * pui = msg.hwnd->m_pimpl->m_pui;

            pui->post_pred([=]()
            {

               pui->ModifyStyle(0, WS_VISIBLE);

            });

            if(pui->m_eappearance == ::user::appearance_iconic && !msg.hwnd->is_iconic())
            {

               // file_put_contents_dup("/home/camilo/xxx.txt", "");

               // 2222222222222222222222222222222222222222222

               pui->m_eappearance = ::user::appearance_none;

               pui->fork([=]()
               {

                  if(pui->m_eappearanceBefore == ::user::appearance_iconic)
                  {

                     pui->_001OnDeiconify(::user::appearance_normal);

                  }
                  else
                  {

                     pui->_001OnDeiconify(pui->m_eappearanceBefore);

                  }

               });

               // bHandled = true;

            }

         }

      }
      else
      {

         if(msg.hwnd != NULL && msg.hwnd->m_pimpl != NULL)
         {

            sp(::user::interaction) pui = msg.hwnd->m_pimpl->m_pui;

            pui->post_pred([=]()
            {

               pui->ModifyStyle(WS_VISIBLE, 0, 0);

            });

         }

      }

   }
   else if(e.type == ConfigureNotify)
   {

      ::user::interaction_impl_base * pimpl = msg.hwnd->m_pimpl;

      if(pimpl != NULL)
      {

         ::user::interaction * pui = pimpl->m_pui;

         bool bHandled = false;

         if(pui != NULL)
         {

            synch_lock sl(pui->m_pmutex);

            if(pui->m_eappearance == ::user::appearance_iconic && !msg.hwnd->is_iconic())
            {

               //file_put_contents_dup("/home/camilo/xxx.txt", "");

               // 33333333333333333333333333333333333333333333333333333333333333

               pui->m_eappearance = ::user::appearance_none;

               pui->fork([=]()
               {

                  if(pui->m_eappearanceBefore == ::user::appearance_iconic)
                  {

                     pui->_001OnDeiconify(::user::appearance_normal);

                  }
                  else
                  {

                     pui->_001OnDeiconify(pui->m_eappearanceBefore);

                  }

               });

               bHandled = true;

            }

            if(!bHandled)
            {

               rect64 rectWindow;

               rectWindow.left = e.xconfigure.x;

               rectWindow.top = e.xconfigure.y;

               rectWindow.right = rectWindow.left + e.xconfigure.width;

               rectWindow.bottom = rectWindow.top + e.xconfigure.height;

               msg.hwnd->m_rect = rectWindow;

               pimpl->m_rectParentClient = rectWindow;

               pimpl->m_pui->set_need_redraw();

               pimpl->m_pui->set_need_layout();

            }

            if(pui->m_eappearanceRequest == ::user::appearance_full_screen
                  && pui->m_eappearance != ::user::appearance_full_screen
                  && !msg.hwnd->is_iconic())
            {

               pui->set_appearance(::user::appearance_full_screen);

            }

         }

      }

      if(g_oswindowDesktop != NULL && e.xconfigure.window == g_oswindowDesktop->window())
      {

         for(int j = 0; j < ::oswindow_data::s_pdataptra->get_count(); j++)
         {

            ::oswindow_data::s_pdataptra->element_at(j)->m_pimpl->m_pui->post_message(WM_DISPLAYCHANGE);

         }

      }

   }
   else if(e.type == ButtonPress || e.type == ButtonRelease)
   {

      bRet                 = true;

      if(e.xbutton.type == ButtonPress)
      {

         if(e.xbutton.button == Button1)
         {

            msg.message = WM_LBUTTONDOWN;

         }
         else if(e.xbutton.button == Button2)
         {

            msg.message = WM_MBUTTONDOWN;

         }
         else if(e.xbutton.button == Button3)
         {

            msg.message = WM_RBUTTONDOWN;

         }
         else
         {

            bRet = false;

         }

      }
      else if(e.xbutton.type == ButtonRelease)
      {

         if(e.xbutton.button == Button1)
         {

            msg.message = WM_LBUTTONUP;

         }
         else if(e.xbutton.button == Button2)
         {

            msg.message = WM_MBUTTONUP;

         }
         else if(e.xbutton.button == Button3)
         {

            msg.message = WM_RBUTTONUP;

         }
         else
         {

            bRet = false;

         }

      }
      else
      {

         bRet = false;

      }

      if(bRet)
      {

         msg.hwnd          = oswindow_get(display, e.xbutton.window);

         msg.wParam        = 0;

         msg.lParam        = MAKELONG(e.xbutton.x_root, e.xbutton.y_root);

         synch_lock sl(pdata->m_pmutexInput);

         pdata->m_messsageaInput.add(msg);

      }

   }
   else if(e.type == KeyPress || e.type == KeyRelease)
   {

      oswindow w = oswindow_get(display, e.xexpose.window);

      bRet                 = true;

      if(e.xkey.type == KeyPress)
      {

         msg.message = WM_KEYDOWN;

      }
      else if(e.xkey.type == KeyRelease)
      {

         msg.message = WM_KEYUP;

      }
      else
      {

         bRet = false;

      }

      if(bRet)
      {

         msg.hwnd          = oswindow_get(display, e.xbutton.window);

         msg.wParam        = e.xkey.keycode;

         msg.lParam        = MAKELONG(0, e.xkey.keycode);

         synch_lock sl(pdata->m_pmutexInput);

         pdata->m_messsageaInput.add(msg);

      }

   }
   else if(e.type == MotionNotify)
   {

      WPARAM wparam(0);

      if(e.xmotion.state & Button1Mask)
      {

         wparam |= MK_LBUTTON;

      }

      msg.hwnd          = oswindow_get(display, e.xbutton.window);
      msg.message       = WM_MOUSEMOVE;
      msg.wParam        = wparam;
      msg.lParam        = MAKELONG(e.xmotion.x_root, e.xmotion.y_root);

      synch_lock sl(pdata->m_pmutexInput);

      pdata->m_messsageaInput.add(msg);

   }
   else if(e.type == DestroyNotify)
   {

      msg.hwnd          = oswindow_get(display, e.xdestroywindow.window);
      msg.message       = WM_DESTROY;

      x11_post_message(msg);

   }
   else if(e.type == FocusIn)
   {

      msg.message       = WM_SETFOCUS;

      msg.hwnd->m_bHasFocus = true;

      //msg.wParam = (WPARAM) oswindow_get(display(), e.xfocus.window);

//      Window wFocus = 0;
//
//      int revert_to_return = 0;
//
//      int iStatus = XGetInputFocus(display, &wFocus, &revert_to_return);
//
//      //if(iStatus == Success)
//      if(iStatus)
//      {
//
//         if(wFocus == e.xfocus.window)
//         {
//
//            output_debug_string("A\n");
//
//         }
//         else
//         {
//
//            output_debug_string("B " + ::str::from(wFocus));
//
//            g_windowFocus = wFocus;
//
//         }
//
//         if(wFocus == g_windowFocus)
//         {
//
//            output_debug_string("C\n");
//
//         }
//         else
//         {
//
//            output_debug_string("D " + ::str::from(wFocus));
//
//            g_windowFocus = wFocus;
//
//         }
//
//      }
//
      synch_lock sl(pdata->m_pmutexInput);

      pdata->m_messsageaInput.add(msg);

   }
   else if(e.type == FocusOut)
   {

      msg.message       = WM_KILLFOCUS;

      msg.hwnd->m_bHasFocus = false;

      Window wFocus = 0;

      int revert_to_return = 0;

      XGetInputFocus(display, &wFocus, &revert_to_return);

      msg.wParam = (WPARAM) oswindow_get(wFocus);

      synch_lock sl(pdata->m_pmutexInput);

      pdata->m_messsageaInput.add(msg);

   }

   // processed

   return true;

}


void x11_post_message(MESSAGE & msg)
{

   try
   {

      if(msg.hwnd == NULL)
      {

         ::aura::system::g_p->post_message(msg.message, msg.wParam, msg.lParam);

      }
      else
      {

         if(msg.hwnd != NULL && msg.hwnd->m_pimpl != NULL && msg.hwnd->m_pimpl->m_pui != NULL)
         {

            ::user::interaction * pui = msg.hwnd->m_pimpl->m_pui;

            pui->post_message(msg.message, msg.wParam, msg.lParam);

         }

      }

   }
   catch(...)
   {

   }

}


void __axis_x11_input_thread(osdisplay_data * pdata)
{

   single_lock sl(pdata->m_pmutexInput);

   MESSAGE msg;

   bool bOk;

   DWORD dwLastMouseMove;

   while(::aura::system::g_p != NULL && ::get_thread_run())
   {

      bOk = false;

      {

         sl.lock();

         restart:

         if(pdata->m_messsageaInput.has_elements())
         {

            msg = pdata->m_messsageaInput.first();

            if(msg.message == WM_MOUSEMOVE)
            {

               if(msg.time - dwLastMouseMove < 5
               || (pdata->m_messsageaInput.get_size() > 1
               && pdata->m_messsageaInput[1].message != WM_MOUSEMOVE))
               {

                  pdata->m_messsageaInput.remove_at(0);

                  goto restart;

               }

               dwLastMouseMove = msg.time;

            }

            pdata->m_messsageaInput.remove_at(0);

            bOk = true;

         }

         sl.unlock();

      }

      if(bOk)
      {

         synch_lock sl(::oswindow_data::s_pmutex);

         ::user::interaction * pui = msg.hwnd->m_pimpl->m_pui;

         if(pui != NULL)
         {

            sl.unlock();

            pui->send_message(msg.message, msg.wParam, msg.lParam);

         }

      }
      else
      {

         Sleep(25);

      }

   }

}


namespace user
{


   LRESULT interaction_child::send_x11_event(void * pvoidEvent)
   {

      XEvent * pevent = (XEvent *) pvoidEvent;

      smart_pointer < ::message::base > spbase;

      spbase = Session.get_message_base(pvoidEvent, m_pui);

      try
      {

         sp(::user::interaction) pui = m_pui;

         while(pui != NULL && pui->GetParent() != NULL)
         {

            try
            {

               pui->pre_translate_message(spbase);

            }
            catch(...)
            {

               break;

            }

            if(spbase->m_bRet)
            {

               return spbase->get_lresult();

            }

            try
            {

               pui = pui->GetParent();

            }
            catch(...)
            {

               break;

            }

         }

      }
      catch(...)
      {

      }

      message_handler(spbase);

      return spbase->get_lresult();

   }


} // namespace user


namespace aura
{


   sp(::message::base) application::get_message_base(void * pevent,::user::interaction * pwnd)
   {

      _throw(todo(get_app()));

      return NULL;

   }


} // namespace aura


WINBOOL SetWindowPos(oswindow hwnd, oswindow hwndInsertAfter, int32_t x, int32_t y, int32_t cx, int32_t cy, UINT nFlags)
{

   synch_lock sl(g_pmutexX);

   return hwnd->set_window_pos(hwndInsertAfter, x, y, cx, cy, nFlags);

}





WINBOOL GetWindowRect(oswindow hwnd, LPRECT lprect)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::GetWindowRect 1");

   xdisplay d(hwnd->display());

   if(d.is_null())
   {

      windowing_output_debug_string("\n::x11_GetWindowRect 1.1");

      return FALSE;

   }

   return x11_get_window_rect(d, hwnd->window(), lprect);


}


WINBOOL GetClientRect(oswindow window, LPRECT lprect)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::x11_GetWindowRect 1");

   xdisplay d(window->display());

   if(d.is_null())
   {

      windowing_output_debug_string("\n::x11_GetWindowRect 1.1");

      return FALSE;

   }

   XWindowAttributes attr;

   if(XGetWindowAttributes(d, window->window(), &attr) == 0)
   {

      windowing_output_debug_string("\n::x11_GetWindowRect 1.2");

      return FALSE;

   }

   lprect->left      = 0;
   lprect->top       = 0;
   lprect->right     = lprect->left    + attr.width;
   lprect->bottom    = lprect->top     + attr.height;

   windowing_output_debug_string("\n::x11_GetWindowRect 2");

   return TRUE;

}


WINBOOL ca2_GetClientRect(oswindow window, LPRECT lprect)
{

   synch_lock sl(window->m_pimpl->m_pui->m_pmutex);

   ::copy(lprect, window->m_pimpl->m_rectParentClient);

   OffsetRect(lprect, -lprect->left, -lprect->top);

   return TRUE;

}



WINBOOL x11_get_cursor_pos(LPPOINT lpptCursor)
{


   Window root_return;
   Window child_return;
   int32_t win_x_return;
   int32_t win_y_return;
   uint32_t mask_return;

   b_prevent_xdisplay_lock_log = true;

   windowing_output_debug_string("\n::GetCursorPos 1");

   xdisplay d(x11_get_display());

   if(d.is_null())
   {

      windowing_output_debug_string("\n::GetCursorPos 1.1");

      return FALSE;

   }

   XQueryPointer(d, d.default_root_window(), &root_return, &child_return, &lpptCursor->x, &lpptCursor->y, &win_x_return, &win_y_return, & mask_return);

   b_prevent_xdisplay_lock_log = false;

   windowing_output_debug_string("\n::GetCursorPos 2");

   return TRUE;

}



WINBOOL GetCursorPos(LPPOINT lpptCursor)
{

   main_sync([&]()
   {

      synch_lock sl(g_pmutexX);

      return x11_get_cursor_pos(lpptCursor);

   });

   return TRUE;

}




::user::interaction * get_system_window_interaction(::os_system_window * psystemwindow)
{

   return NULL;

}



void wm_full_screen(oswindow w, LPCRECT lpcrect)
{

   w->full_screen(lpcrect);

}



CLASS_DECL_AURA void defer_dock_application(bool bDock)
{

   UNREFERENCED_PARAMETER(bDock);

}







bool os_init_windowing()
{

   //g_pmutexX = new mutex();

   //g_pmutexX = NULL;

   mutex * g_pmutexX11 = new mutex();

   g_prunnableptrlX11 = new list < sp(object) >();

   set_TranslateMessage(&axis_TranslateMessage);

   set_DispatchMessage(&axis_DispatchMessage);

   oswindow_data::s_pdataptra = new oswindow_dataptra;

   oswindow_data::s_pmutex = new mutex;

   osdisplay_data::s_pdataptra = new osdisplay_dataptra;

   osdisplay_data::s_pmutex = new mutex;

   XSetErrorHandler(_c_XErrorHandler);

}


void os_term_windowing()
{

   ::aura::del(osdisplay_data::s_pmutex);

   ::aura::del(osdisplay_data::s_pdataptra);

   ::aura::del(oswindow_data::s_pmutex);

   ::aura::del(oswindow_data::s_pdataptra);

}


int xlib_error_handler(Display * d, XErrorEvent * e)
{

   if(e->request_code == 12)
   {

      if(e->error_code == BadValue)
      {

      }

   }

   char sz[1024];

   XGetErrorText(d, e->error_code, sz, sizeof(sz));

   fputs(sz, stderr);

   abort();

}


i64 oswindow_id(oswindow w)
{

   return w->window();

}


WINBOOL SetCursor(oswindow window, HCURSOR hcursor)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::x11_GetWindowRect 1");

   xdisplay d(window->display());

   if(d.is_null())
   {

      windowing_output_debug_string("\n::x11_GetWindowRect 1.1");

      return FALSE;

   }

   XDefineCursor(d, window->window(), hcursor);

   return TRUE;

}


XImage * _x11_create_image(Display * pdisplay, ::draw2d::dib * pdib)
{

   unsigned char * image32 = (unsigned char *) pdib->m_pcolorref;

   int width = pdib->m_size.cx;

   int height = pdib->m_size.cy;

   int depth = 32; // works fine with depth = 24

   int bitmap_pad = 32; // 32 for 24 and 32 bpp, 16, for 15&16

   int bytes_per_line = pdib->m_iScan; // number of bytes in the client image between the start of one scanline and the start of the next

   XImage * pimage = XCreateImage(pdisplay, CopyFromParent, depth, ZPixmap, 0, image32, width, height, bitmap_pad, bytes_per_line);

   return pimage;

}


XImage * x11_create_image(::draw2d::dib * pdib)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::x11_GetWindowRect 1");

   xdisplay d(x11_get_display());

   if(d.is_null())
   {

      windowing_output_debug_string("\n::x11_GetWindowRect 1.1");

      return NULL;

   }

   return _x11_create_image(d, pdib);

}


Pixmap _x11_create_pixmap(Display * pdisplay, ::draw2d::dib * pdib)
{

   XImage * pimage = _x11_create_image(pdisplay, pdib);

   if(pimage == NULL)
   {

      return NULL;

   }

   int depth = 32; // works fine with depth = 24

   Pixmap pixmap = XCreatePixmap(pdisplay, XDefaultRootWindow(pdisplay), pdib->m_size.cx, pdib->m_size.cy, depth);

   XGCValues gcvalues;

   GC gc = XCreateGC(pdisplay, pixmap, 0, &gcvalues);

   XPutImage(pdisplay, pixmap, gc, pimage, 0, 0, 0, 0,pdib->m_size.cx, pdib->m_size.cy);

   XFreeGC(pdisplay, gc);

   return pixmap;

}


Pixmap x11_create_pixmap(::draw2d::dib * pdib)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::x11_GetWindowRect 1");

   xdisplay d(x11_get_display());

   if(d.is_null())
   {

      windowing_output_debug_string("\n::x11_GetWindowRect 1.1");

      return NULL;

   }

   return _x11_create_pixmap(d, pdib);

}


Picture _xrender_create_picture(oswindow window, ::draw2d::dib * pdib)
{

   Pixmap pixmap = _x11_create_pixmap(window->display(), pdib);

   if(pixmap == NULL)
   {

      return NULL;

   }


//	bool hasNamePixmap = false;
//
//	int event_base, error_base;
//
//	if (XCompositeQueryExtension(window->display(), &event_base, &error_base))
//	{
//
//		int major = 0;
//
//		int minor = 2;
//
//		XCompositeQueryVersion(window->display(), &major, &minor);
//
//		if(major > 0 || minor >= 2 )
//		{
//
//         hasNamePixmap = true;
//
//      }
//
//	}

	XWindowAttributes attr;

	XGetWindowAttributes(window->display(), window->window(), &attr);

	XRenderPictFormat * pformat = XRenderFindVisualFormat(window->display(), attr.visual);

	bool hasAlpha             = (pformat->type == PictTypeDirect && pformat->direct.alphaMask);
	int x                     = 0;
	int y                     = 0;
	int width                 = pdib->m_size.cx;
	int height                = pdib->m_size.cy;

	XRenderPictureAttributes pa = {};
	//pa.subwindow_mode = IncludeInferiors; // Don't clip child widgets

	Picture picture = XRenderCreatePicture(window->display(), pixmap, pformat, CPSubwindowMode, &pa);

	return picture;

}


Picture xrender_create_picture(oswindow window, ::draw2d::dib * pdib)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::x11_GetWindowRect 1");

   xdisplay d(window->display());

   if(d.is_null())
   {

      windowing_output_debug_string("\n::x11_GetWindowRect 1.1");

      return NULL;

   }

   return _xrender_create_picture(window, pdib);

}


HCURSOR CreateAlphaCursor(oswindow window, ::draw2d::dib * pdib, int xHotSpot, int yHotSpot)
{

   synch_lock sl(g_pmutexX);

   windowing_output_debug_string("\n::x11_GetWindowRect 1");

   xdisplay d(window->display());

   if(d.is_null())
   {

      windowing_output_debug_string("\n::x11_GetWindowRect 1.1");

      return NULL;

   }

   Picture picture = _xrender_create_picture(window, pdib);

   HCURSOR hcursor = XRenderCreateCursor(d, picture, xHotSpot, yHotSpot);

   return hcursor;

}



