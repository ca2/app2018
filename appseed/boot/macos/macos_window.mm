//
//  boot_window.cpp
//  os
//
//  Created by Camilo Sasuke Tsumanuma on 6/8/13.
//  Copyright (c) 2013 ca2 Desenvolvimento de Sofware Ltda. All rights reserved.
//
#import "macos_mm.h"



NSWindow * new_boot_window(boot_window * pwindow, CGRect rect)
{
   
   rect.origin.x     = 0;
   rect.origin.x     = 0;
   rect.size.width   = 0;
   rect.size.height  = 0;
   
   pwindow->m_proundwindow = [mm_window alloc];
   
   pwindow->m_proundwindow->m_pwindow = pwindow;
   
   return [pwindow->m_proundwindow initWithContentRect : rect styleMask : 0 backing : NSBackingStoreBuffered  defer : false ];
   
}


void ns_shared_application()
{

   [NSApplication sharedApplication];
   
}


void ns_app_run()
{

   [NSApp run];
   
}



void boot_window::boot_window_show()
{
   
   [[m_proundwindow->m_controller dd_invokeOnMainThread] showWindow : m_proundwindow];
   
}

void boot_window::boot_window_redraw()
{

   [[m_proundwindow dd_invokeOnMainThread] display ];
   
}


void boot_window::boot_window_invalidate()
{
   
//   [[m_proundwindow->m_controller dd_invokeOnMainThread] setViewsNeedDisplay : TRUE];
   
}
