//
//  os_windowing.h
//  aura
//
//  Created by Camilo Sasuke Tsumanuma on 10/12/17.
//
#pragma once


#if defined METROWIN && defined(__cplusplus_winrt)

class CLASS_DECL_AURA os_system_window
{
public:

   //bool                                         m_bWindowSizeChange;

   sp(::user::interaction)                      m_pui;

   ::aura::system_window ^                      m_pwindow;

   os_system_window();

};

#elif defined(APPLE_IOS)

class os_system_window
{
public:

   sp(::user::interaction)                      m_pui;

};

#elif defined(VSNORD)

class os_system_window
{
public:

   sp(::user::interaction)                      m_pui;

   oswindow                                     m_oswindow;

   os_system_window();
   ~os_system_window();

};

#else

class os_system_window;

#endif


::user::interaction * get_system_window_interaction(::os_system_window * psystemwindow);

#ifdef APPLE_IOS

#include "ios/ios_window.h"

#endif

