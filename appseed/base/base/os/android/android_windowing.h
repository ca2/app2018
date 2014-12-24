#pragma once


namespace boot
{


   class thread_base;
   class interaction_impl;


} // namespace boot




typedef ptr_array < oswindow_data > oswindow_dataptra;










#pragma once


// define portable types for 32-bit / 64-bit OS
//typedef int32_t WINBOOL;
typedef uint8_t BYTE;
//typedef uint16_t WORD;
typedef uint32_t DWORD;
//typedef int32_t LONG;


#undef FAR
#undef  NEAR
#define FAR
#define NEAR
#ifndef CONST
#define CONST               const
#endif


#ifdef __cplusplus



struct hthread;

namespace ca2
{


   class thread_base;


} // namespace ca2


//class oswindow_data;


//typedef oswindow_data * oswindow;



namespace user
{


   class interaction;
   class interaction;


} // namespace user


//class oswindow_dataptra;


class event;


class mutex;


//typedef hthread * HTHREAD;


class oswindow_data;


CLASS_DECL_AURA int32_t oswindow_find_message_only_window(::user::interaction * puibaseMessageWindow);
//CLASS_DECL_AURA int32_t oswindow_find(Display * pdisplay,Window window);
//CLASS_DECL_AURA int32_t oswindow_find(Window window);
CLASS_DECL_AURA oswindow_data * oswindow_get_message_only_window(::user::interaction * puibaseMessageWindow);
//CLASS_DECL_AURA oswindow_data * oswindow_get(Display * pdisplay,Window window,Visual * pvisual = NULL,int iDepth = -1,int iScreen = -1,Colormap colormap = None);
//CLASS_DECL_AURA oswindow_data * oswindow_get(Window window);
//CLASS_DECL_AURA oswindow oswindow_defer_get(Window w);
//CLASS_DECL_AURA bool oswindow_remove(Display * pdisplay,Window window);
CLASS_DECL_AURA bool oswindow_remove_message_only_window(::user::interaction * puibaseMessageOnlyWindow);




bool IsChild(oswindow oswindowParent,::oswindow oswindowcandidateChildOrDescendant);
oswindow GetParent(::oswindow oswindow);
oswindow SetParent(::oswindow oswindow,::oswindow oswindowNewParent);
bool ShowWindow(::oswindow oswindow,int32_t nCmdShow);
LONG GetWindowLongA(::oswindow oswindow,int32_t nIndex);
LONG SetWindowLongA(::oswindow oswindow,int32_t nIndex,LONG l);
bool ClientToScreen(::oswindow oswindow,LPPOINT lppoint);
bool ScreenToClient(::oswindow oswindow,LPPOINT lppoint);
int32_t IsIconic(::oswindow oswindow);
int32_t IsWindowVisible(::oswindow oswindow);


#define GetWindowLong GetWindowLongA
#define SetWindowLong SetWindowLongA



CLASS_DECL_AURA bool IsWindow(oswindow oswindow);


#endif
