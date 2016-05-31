#include "framework.h"
#include "ca_os_cross_win_gdi_internal.h"


device_context::device_context()
{


   m_display   = NULL;
   m_d         = 0;
   m_gc        = NULL;
   m_hwnd      = NULL;



}



HDC GetDC(oswindow hwnd)
{


   HDC hdc = new device_context;

   hdc->m_display    = XOpenDisplay(NULL);
   hdc->m_hwnd       = hwnd;
   hdc->m_d          = (Drawable) (hwnd == NULL || hwnd->window() == NULL ? DefaultRootWindow(hdc->m_display) : hwnd->window());
   hdc->m_gc         = XCreateGC(hdc->m_display, hdc->m_d, 0, 0);

   return hdc;

}


HDC GetWindowDC(oswindow hwnd)
{

   return GetDC(hwnd);
}


WINBOOL ReleaseDC(oswindow hwnd, HDC hdc)
{


   mutex_lock sl(user_mutex(), true);


   if(hdc == NULL)
      return FALSE;

   XFreeGC(hdc->m_display, hdc->m_gc);
   XCloseDisplay(hdc->m_display);

   delete hdc;
   return TRUE;

}


WINBOOL GetClientRect(oswindow hwnd, LPRECT lprect)
{

   mutex_lock sl(user_mutex(), true);

   XWindowAttributes attrs;

   /* Fill attribute structure with information about root window */

   if(XGetWindowAttributes(hwnd->display(), hwnd->window(), &attrs) == 0)
   {

      return FALSE;

   }

   lprect->left      = 0;
   lprect->top       = 0;
   lprect->right     = lprect->left    + attrs.width;
   lprect->bottom    = lprect->top     + attrs.height;

   return TRUE;

}


WINBOOL GetWindowRect(oswindow hwnd, LPRECT lprect)
{



   mutex_lock sl(user_mutex(), true);

   if(!IsWindow(hwnd))
      return FALSE;


   bool bDestroying = hwnd->m_bDestroying;
   Display * pdisplay = hwnd->display();
   Window window = hwnd->window();


   if(pdisplay == NULL)
    return FALSE;

   XWindowAttributes attrs;


   XSync(hwnd->display(), False);

   /* Fill attribute structure with information about root window */

   if(!XGetWindowAttributes(hwnd->display(), hwnd->window(), &attrs))
   {
      return FALSE;
   }

   int x;
   int y;
   Window child;

   if(!XTranslateCoordinates(hwnd->display(), hwnd->window(), DefaultRootWindow(hwnd->display()), 0, 0, &x, &y, &child))
   {
      return FALSE;
   }

   lprect->left      = x;
   lprect->top       = y;
   lprect->right     = x    + attrs.width;
   lprect->bottom    = y    + attrs.height;

   return TRUE;

}



int32_t FillRect(HDC hdc, const RECT * lprc, HBRUSH hbr)
{

   mutex_lock sl(user_mutex(), true);


   XFillRectangle(hdc->m_display, hdc->m_d, hdc->m_gc, lprc->left, lprc->top, lprc->right - lprc->left, lprc->bottom - lprc->top);
   return 1;
}


HDC BeginPaint(oswindow hwnd, PAINTSTRUCT * ps)
{

   HDC hdc = GetDC(hwnd);

   GetClientRect(hwnd, &ps->rcPaint);

   return hdc;

}


WINBOOL EndPaint(oswindow hwnd, PAINTSTRUCT * ps)
{

   return ReleaseDC(hwnd, ps->hdc);

}


WINBOOL GetCursorPos(LPPOINT lpptCursor)
{

   //mutex_lock sl(user_mutex(), true);


   Window root_return;
   Window child_return;
   int32_t win_x_return;
   int32_t win_y_return;
   uint32_t mask_return;

   xdisplay display;

   display.open(NULL);

   if(display == NULL)
        return FALSE;

   XQueryPointer(display, display.default_root_window(), &root_return, &child_return, &lpptCursor->x, &lpptCursor->y, &win_x_return, &win_y_return, & mask_return);

   return TRUE;

}



WINBOOL SetWindowPos(oswindow hwnd, oswindow hwndInsertAfter, int32_t x, int32_t y, int32_t cx, int32_t cy, UINT uFlags)
{

   mutex_lock sl(user_mutex(), true);


   xdisplay display(hwnd->display());


   int32_t value_mask = 0;

   XWindowChanges values;

   if(!(uFlags & SWP_NOMOVE))
   {
      value_mask |= CWX | CWY;
      values.x = x;
      values.y = y;
   }

   if(!(uFlags & SWP_NOSIZE))
   {
      value_mask |= CWWidth | CWHeight;
      values.width = cx;
      values.height = cy;
   }

   if(!(uFlags & SWP_NOZORDER) && hwndInsertAfter >= 0)
   {
      value_mask |= CWSibling;
      values.sibling = hwndInsertAfter->window();
      values.stack_mode = Above;
   }

   XConfigureWindow(hwnd->display(), hwnd->window(), value_mask, &values);

   if(uFlags & SWP_SHOWWINDOW)
   {
      XMapWindow(hwnd->display(), hwnd->window());
   }

   if(!(uFlags & SWP_NOZORDER) && hwndInsertAfter < 0)
   {
      if(hwndInsertAfter->window() == ZORDER_TOP || hwndInsertAfter->window() == ZORDER_TOPMOST)
      {
         XRaiseWindow(hwnd->display(), hwnd->window());
      }
      else if(hwndInsertAfter->window() == ZORDER_BOTTOM)
      {
         XLowerWindow(hwnd->display(), hwnd->window());
      }

   }

   //XCloseDisplay(display);

   return 1;

}



int32_t _c_XErrorHandler(Display * display, XErrorEvent * perrorevent)
{
   return 0;
}
