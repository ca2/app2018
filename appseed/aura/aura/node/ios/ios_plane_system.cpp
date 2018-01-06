//
//  ios_plane_system.cpp
//  ios_aura
//
//  Created by Camilo Sasuke Tsumanuma on 03/10/17.
//

#include "framework.h"






//
//  app_system.cpp
//  app
//
//  Created by Camilo Sasuke Tsumanuma on 12/05/14.
//  Copyright (c) 2014 Camilo Sasuke Tsumanuma. All rights reserved.
//


plane_system * create_plane_system()
{
   
   return new ::plane_system();
   
}

plane_system::plane_system()
{
   
   m_psystem = ::aura::system::g_p;

   m_psystem->m_pplanesystem = this;

   m_psystem->initialize_native_window1();

}


ui_window_ptr plane_system::plane_system_initialize(CGRect rect)
{
   
   ::user::native_window_initialize initialize;
   
   initialize.m_rect.left = rect.origin.x;
   initialize.m_rect.top = rect.origin.y;
   initialize.m_rect.right = rect.origin.x + rect.size.width;
   initialize.m_rect.bottom = rect.origin.x + rect.size.height;
   
   m_psystem->m_possystemwindow->m_pui->initialize_native_window(&initialize);
   
   ::ios::interaction_impl * pimpl = m_psystem->m_possystemwindow->m_pui->m_pimpl.cast < ::ios::interaction_impl > ();

   UIWindow * window = new_round_window(pimpl, rect);
   
   pimpl->m_oswindow = oswindow_get(window);
   
   pimpl->m_oswindow->set_user_interaction_impl(pimpl);
   
   ::user::create_struct cs;
   cs.dwExStyle = 0;
   cs.lpszClass = 0;
   cs.lpszName = NULL;
   cs.style = 0;
   cs.x = 0;
   cs.y = 0;
   cs.cx = 0;
   cs.cy = 0;
   //      cs.hwndParent = hWndParent;
   //   cs.hMenu = hWndParent == NULL ? NULL : nIDorHMenu;
   cs.hMenu = NULL;
   //      cs.hInstance = System.m_hInstance;
   cs.lpCreateParams = NULL;
   
   if(pimpl->m_pui != NULL)
   {
      
      if(!pimpl->m_pui->pre_create_window(cs))
      {
         
         pimpl->PostNcDestroy();
         
         return FALSE;
         
      }
      
   }
   else
   {
      
      if (!pimpl->pre_create_window(cs))
      {
         
         pimpl->PostNcDestroy();
         
         return FALSE;
         
      }
      
   }
   
   if(cs.hwndParent == NULL)
   {
      
      cs.style &= ~WS_CHILD;
      
   }
   
   //   puiimpl->m_pui->m_pthread = ::get_thread();
   
   pimpl->send_message(WM_CREATE, 0, (LPARAM) &cs);
   
   ::rect rectMainScreen;
   
   GetMainScreenRect(rectMainScreen);
   
   pimpl->SetPlacement(rectMainScreen);
   
   return window;

 }   

void plane_system::plane_system_begin()
{
   
}


