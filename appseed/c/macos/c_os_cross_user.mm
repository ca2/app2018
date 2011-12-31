#include <Cocoa/Cocoa.h>
#include "c_os_internal.h"


BOOL IsWindowVisible(HWND hwnd)
{
   
   return 1;
   
}


BOOL IsIconic(HWND hwnd)
{
   
   [((NSWindow * ) hwnd->m_pnswindow) miniaturize : 0];
   
   return 1;
   
   
}



BOOL set_nswindow_frame(HWND hwnd, LPCRECT lpcrect, int iDisplay)
{

   NSRect rect;
   
   rect.origin.x     = lpcrect->left;
   rect.origin.y     = lpcrect->bottom;
   rect.size.width   = lpcrect->right     -     lpcrect->left;
   rect.size.height  = lpcrect->bottom    -     lpcrect->top;
   
   
   [((NSWindow * ) hwnd->m_pnswindow) setFrame : rect display : iDisplay];
   
   return 1;
   
}



BOOL move_nswindow(HWND hwnd, int x, int y)
{
   
   NSPoint point;
   
   point.x = x;
   point.y = y;
   
   [((NSWindow * ) hwnd->m_pnswindow) setFrameTopLeftPoint : point];
   
   return 1;
   
   
}
