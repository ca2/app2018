//
//  round_window.h
//  os
//
//  Created by Camilo Sasuke Tsumanuma on 6/8/13.
//  Copyright (c) 2013 ca2 Desenvolvimento de Sofware Ltda. All rights reserved.
//

#pragma once


//#include "app/appseed/axis/user/user/user_key_enum.h"


class round_window
{
public:
   
   
#ifdef __OBJC__
   
   RoundWindow * m_proundwindow;
   
#else
   
   void * m_proundwindow; // cast to RoundWindow * to use it
   
#endif


   bool m_bDirty;
   
   unsigned long long   m_uiLastUpdateBeg;
   unsigned long long   m_uiLastUpdateEnd;

   
   round_window();
   virtual ~round_window();
   
   virtual void round_window_draw(CGContextRef cgc) = 0;
   virtual void round_window_mouse_down(int iButton, double x, double y) = 0;
   virtual void round_window_mouse_up(int iButton, double x, double y) = 0;
   virtual void round_window_mouse_moved(double x, double y, unsigned long ulAppleMouseButton) = 0;
   virtual void round_window_mouse_dragged(double x, double y, unsigned long ulAppleMouseButton) = 0;
   virtual void round_window_double_click(int iButton, double x, double y) = 0;
   virtual bool round_window_key_down(unsigned int vk, unsigned int scan) = 0;
   virtual bool round_window_key_up(unsigned int vk, unsigned int scan) = 0;
   virtual bool round_window_key_down(unsigned int uiKeyCode) = 0;
   virtual bool round_window_key_up(unsigned int uiKeyCode) = 0;
   
   virtual void round_window_resized(CGRect rect) = 0;
   virtual void round_window_moved(CGPoint point) = 0;
   
   virtual void round_window_iconified() = 0;
   virtual void round_window_deiconified() = 0;

   virtual void round_window_on_show() = 0;
   virtual void round_window_on_hide() = 0;

   virtual void round_window_activate() = 0;
   virtual void round_window_deactivate() = 0;
   
   
   virtual void round_window_invalidate();
   virtual void round_window_show();
   virtual void round_window_hide();
   virtual void round_window_redraw();
   virtual void round_window_redraw_async();
   
   virtual void round_window_order_front();
   virtual void round_window_make_key_window();
   virtual void round_window_make_key_window_and_order_front();
   virtual void round_window_make_main_window();
   
   virtual void round_window_set_title(const char * pszTitle);
   virtual void round_window_get_title(char * pszTitle, int iSize);
   
   virtual void round_window_destroy();
   
};


void ns_shared_application(int argc, char *argv[]);
NSWindow * new_round_window(round_window * pwindow, CGRect rect);
void ns_app_run();
void ns_app_close();
void ui_app_run(int argc, char * argv[]);



