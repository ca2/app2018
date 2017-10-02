//
//  ios_app.cpp
//  aura
//
//  Created by Camilo Sasuke Tsumanuma on 10/07/15.
//
//




ui_window_ptr new_round_window(plane_system * psystem, CGRect rect);




//
//  app_system.cpp
//  app
//
//  Created by Camilo Sasuke Tsumanuma on 12/05/14.
//  Copyright (c) 2014 Camilo Sasuke Tsumanuma. All rights reserved.
//







plane_system * new_system(const char * pszId)
{
   
   on_init_thread();
   
   
   return new ::plane_system(pszId);
   
   
   
}

::base::system * create_base_system()
{
   
   ::aura::system * paurasystem = create_aura_system();
   
   if(paurasystem == NULL)
      return NULL;
      
   ::base::system * pbasesystem = dynamic_cast<::base::system *>(paurasystem);
   
   if(pbasesystem == NULL)
   {
      
      ::aura::del(paurasystem);
      
      return NULL;
      
   }
   
   return pbasesystem;
   
}


plane_system::plane_system(const char * pszId)
{
   
   m_psystem = create_base_system();
   
   m_psystem->m_possystemwindow->m_pui = new ::user::interaction(m_psystem);
   
   ::command::command * pinitmaindata  = new ::command::command;
   
   pinitmaindata->m_strCommandLine        = pszId;
   
   m_psystem->startup_command(pinitmaindata);
   
}


ui_window_ptr init_part_2ex(plane_system * psystem, CGRect rect)
{
   
   return psystem->init_part_2ex(rect);
   
}


ui_window_ptr plane_system::init_part_2ex(CGRect rect)
{
   
   //m_psystem->m_window = ios_start_window(this, rect);
   
   //int nReturnCode = 0;
   
   manual_reset_event ev(m_psystem);
   
   m_psystem->m_peventReady = &ev;
   
   if(!m_psystem->begin_synch())
      return NULL;
   
   ev.wait();
   
   ui_window_ptr pwindow = new_round_window(this, rect);
   
   //::user::native_window_initialize initialize;
   
   //initialize.pwindow = this;
   //initialize.window = m_window.Get();
   
   //m_psystem->m_posdata->m_pui->initialize(&initialize);
   
   //m_psystem->m_ptwf = create_twf_2ex();
   
   //m_psystem->m_ptwf->twf_start();
   
   stringa straLibrary = m_psystem->handler()->m_varTopicQuery["app"].stra();
   
   for(int i = 0; i < m_psystem->handler()->m_varTopicQuery["app"].array_get_count(); i++)
   {
      string strApp = m_psystem->handler()->m_varTopicQuery["app"].at(i);
   }
   
   straLibrary.replace("\\", "_");
   straLibrary.replace("-", "_");
   
   for(int i = 0; i < straLibrary.get_count(); i++)
   {
      string strLibrary = straLibrary[i];
      strsize iFind = strLibrary.find("/");
      if(iFind >= 0)
         strLibrary = strLibrary.Left(iFind) + '_' + strLibrary.Mid(iFind + 1);
      iFind = strLibrary.find("/", iFind + 1);
      if(iFind >= 0)
         strLibrary.Truncate(iFind);
      m_psystem->m_mapAppLibrary[m_psystem->handler()->m_varTopicQuery["app"][i]] = strLibrary;
   }
   
   for(int i = 0; i < m_psystem->handler()->m_varTopicQuery["app"].get_count(); i++)
   {
      string strApp = m_psystem->handler()->m_varTopicQuery["app"][i];
   }
   
   m_psystem->m_mapAppLibrary["app/ca2/cube"] = "ca2";
   m_psystem->m_mapAppLibrary["app/ca2/bergedge"] = "ca2";
   
   return pwindow;
}


void system_begin_main(plane_system * psystem)
{
   
   psystem->begin();
  
}

void plane_system::begin()
{
   
   m_psystem->begin();
   
}


round_window * ios_start_window(plane_system * psystem, CGRect rect)
{
   
   ::user::native_window_initialize initialize;
   
   initialize.m_rect.left = rect.origin.x;
   initialize.m_rect.top = rect.origin.y;
   initialize.m_rect.right = rect.origin.x + rect.size.width;
   initialize.m_rect.bottom = rect.origin.x + rect.size.height;
   
   psystem->m_psystem->m_possystemwindow->m_pui->initialize_native_window(&initialize);
   
   return psystem->m_psystem->m_possystemwindow->m_pui->m_pimpl.cast < ::ios::interaction_impl > ();
   
}


int ios_initialize_window(round_window * proundwindow, UIWindow * window)
{
   
   ::ios::interaction_impl * puiimpl = dynamic_cast < ::ios::interaction_impl * > (proundwindow);
   
   puiimpl->m_oswindow = oswindow_get(window);
   
   puiimpl->m_oswindow->set_user_interaction(puiimpl);
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
   
   if(puiimpl->m_pui != NULL)
   {
      
      if(!puiimpl->m_pui->pre_create_window(cs))
      {
         
         puiimpl->PostNcDestroy();
         
         return FALSE;
         
      }
      
   }
   else
   {
      
      if (!puiimpl->pre_create_window(cs))
      {
         
         puiimpl->PostNcDestroy();
         
         return FALSE;
         
      }
      
   }
   
   if(cs.hwndParent == NULL)
   {
      
      cs.style &= ~WS_CHILD;
      
   }
   
//   puiimpl->m_pui->m_pthread = ::get_thread();
   
   puiimpl->send_message(WM_CREATE, 0, (LPARAM) &cs);
   
   ::rect rectMainScreen;
   
   GetMainScreenRect(rectMainScreen);
   
   puiimpl->SetPlacement(rectMainScreen);
   
   return TRUE;
   
}




ui_window_ptr new_round_window(plane_system * psystem, CGRect rect)
{
   
   round_window * pwindow = ios_start_window(psystem, rect);
   
   UIWindow * window = new_round_window(pwindow, rect);
   
   ios_initialize_window(pwindow, window);
   
   return window;
   
}


