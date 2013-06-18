//
//  c_os_cross_win_gdi.mm
//  c
//
//  Created by Camilo Sasuke Tsumanuma on 12/22/11.
//  Copyright (c) 2011 ca2 Desenvolvimento de Sofware Ltda. All rights reserved.
//

#include "ca_mm.h"


CGContextRef get_nswindow_cgcontext(oswindow oswindow)
{

    return (CGContextRef) [[oswindow->window() graphicsContext] graphicsPort];

}


WINBOOL get_nswindow_rect(oswindow oswindow, LPRECT lprect)
{
   
    NSRect rect = [oswindow->window() frame];
    
    lprect->left        = rect.origin.x;
    lprect->top         = rect.origin.y;
    lprect->right       = rect.origin.x + rect.size.width;
    lprect->bottom      = rect.origin.y + rect.size.height;
    
    return 1;
    
}


void GetMainScreenRect(LPRECT lprect)
{

   NSRect rect = [[NSScreen mainScreen] frame];

   lprect->left        = rect.origin.x;
   lprect->top         = rect.origin.y;
   lprect->right       = rect.origin.x + rect.size.width;
   lprect->bottom      = rect.origin.y + rect.size.height;
   
}



WINBOOL SetForegroundWindow(oswindow window)
{
   
   if(!::IsWindow(window))
      return FALSE;
   
   [window->window() orderFrontRegardless];
   
   return TRUE;
   
}


WINBOOL BringWindowToTop(oswindow window)
{
   
   if(!::IsWindow(window))
      return FALSE;
   
   [window->window() orderFrontRegardless];
   
   return TRUE;
   
}




