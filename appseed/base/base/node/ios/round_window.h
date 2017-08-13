//
//  round_window.h
//  os
//
//  Created by Camilo Sasuke Tsumanuma on 6/8/13.
//  Copyright (c) 2013 ca2 Desenvolvimento de Sofware Ltda. All rights reserved.
//

#pragma once


#include "app/appseed/axis/user/user/user_key_enum.h"


#ifdef __OBJC__
@class RoundWindow;
#endif

class round_window
{
public:


bool m_bDirty;
   
#ifdef __OBJC__
   RoundWindow * m_proundwindow;
#else
   void * m_proundwindow;
#endif
   
  
   virtual void round_window_draw(CGContextRef cgc) = 0;
   virtual void round_window_mouse_down(double x, double y) = 0;
   virtual void round_window_mouse_up(double x, double y) = 0;
   virtual void round_window_mouse_moved(double x, double y) = 0;
   virtual void round_window_mouse_dragged(double x, double y) = 0;
   virtual bool round_window_key_down(::user::e_key ekey) = 0;
   virtual bool round_window_key_up(::user::e_key ekey) = 0;
   virtual bool round_window_on_text(const char * pszText) = 0;
   
   virtual int round_window_get_x() = 0;
   virtual int round_window_get_y() = 0;
   
   virtual void round_window_resized(CGRect rect) = 0;
   virtual void round_window_moved(CGPoint point) = 0;

   virtual void round_window_activate() = 0;
   virtual void round_window_deactivate() = 0;
   
   virtual void round_window_on_show() = 0;
   virtual void round_window_on_hide() = 0;
   
   virtual void round_window_invalidate();
   virtual void round_window_show();
   virtual void round_window_hide();
   virtual void round_window_redraw();
   
   virtual void round_window_show_keyboard(bool bShow = true);
   
   virtual void round_window_set_title(const char * pszTitle);
   virtual void round_window_get_title(char * pszTitle, int iSize);

   virtual void round_window_destroy();
   
};


UIWindow * new_round_window(round_window * pwindow, CGRect rect);
void ui_application_main(int argc, char * argv[]);
void ui_app_run(int argc, char * argv[]);
