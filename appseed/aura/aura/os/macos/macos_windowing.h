//
//  macos_window.h
//  aura
//
//  Created by Camilo Sasuke Tsumanuma on 2013-09-17.
//
//
#pragma once


namespace user
{
   
   
   class interaction;
   class interaction_impl;
   
   
} // namespace user


#ifdef __MM


typedef void * int_ptr_to_int_ptr;


#else

struct NSWindow;

#endif


typedef NSWindow * nswindow;


class CLASS_DECL_AURA oswindow_data
{
public:
   
   
   nswindow                      m_nswindow;
   ::user::interaction_impl *    m_pimpl;
   
   
   int_ptr_to_int_ptr *          m_plongmap;

   bool                          m_bNsWindowRect;
   RECT                          m_rectNsWindow;
   
   
   oswindow_data();
   oswindow_data(nswindow window);
   oswindow_data(const oswindow_data & oswindow);
   ~oswindow_data();
   
   
   operator void * ()
   {
      
      return this;
      
   }
   
   operator void * () const
   {
      
      return (void *) this;
      
   }
   
#ifdef __MM
   
   operator ::nswindow ()
   {
      return window();
   }
   
#endif
   
   oswindow_data & operator = (const oswindow_data & window);
   
   bool operator == (const void * p) const
   {
      return this == p;
   }
   bool operator != (const void * p) const
   {
      return this != p;
   }
   
   //bool is_null();
   
   nswindow window() const
   {
      return is_null() ? NULL : m_nswindow;
   }
   
   
   void set_user_interaction_impl(::user::interaction_impl * pimpl);
   ::user::interaction * get_user_interaction_base();
   ::user::interaction * get_user_interaction_base() const;
   ::user::interaction * get_user_interaction();
   ::user::interaction * get_user_interaction() const;
   
   
   void post_nc_destroy();
   
   
   bool is_child(oswindow oswindowCandidateChildOrDescendant); // or descendant
   oswindow get_parent();
   oswindow set_parent(oswindow oswindowNewParent);
   long get_state();
   bool is_iconic();
   bool iconify();
   bool is_window_visible();
   bool show_window(int32_t nCmdShow);
   int_ptr get_window_long_ptr(int_ptr nIndex);
   int_ptr set_window_long_ptr(int_ptr nIndex, int_ptr l);
   bool client_to_screen(POINT * lppoint);
   bool screen_to_client(POINT * lppoint);
   
   
   
   bool is_null() const;
   
};





int oswindow_find(nswindow window);
oswindow_data * oswindow_get(nswindow window);
bool oswindow_remove(nswindow window);




//inline int IsChild(oswindow oswindowParent, ::oswindow oswindowCandidateChildOrDescendant)
//{
//   return oswindowParent->is_child(oswindowCandidateChildOrDescendant);
//}

//inline oswindow GetParent(::oswindow window)
//{
//   return window->get_parent();
//}


oswindow get_window(oswindow window, int iParentHood);

//inline oswindow SetParent(::oswindow window, ::oswindow oswindowNewParent)
//{
//   return window->set_parent(oswindowNewParent);
//}

//inline int ShowWindow(::oswindow window, int32_t nCmdShow)
//{
//   return window->show_window(nCmdShow);
//}

//inline int32_t GetWindowLongA(::oswindow window, int32_t nIndex)
//{
//   return (int32_t) window->get_window_long(nIndex);
//}
//
//inline int32_t SetWindowLongA(::oswindow window, int32_t nIndex, int32_t l)
//{
//   return (int32_t) window->set_window_long(nIndex, l);
//}
//
//inline int ClientToScreen(::oswindow window, POINT * lppoint)
//{
//   return window->client_to_screen(lppoint);
//}
//
//inline int ScreenToClient(::oswindow window, POINT * lppoint)
//{
//   return window->screen_to_client(lppoint);
//}
//
//inline int32_t IsIconic(::oswindow window)
//{
//   return window->is_iconic();
//}
//
//inline int32_t IsWindowVisible(::oswindow window)
//{
//   return window->is_window_visible();
//}


#define GetWindowLong GetWindowLongA
#define SetWindowLong SetWindowLongA

int GetMainScreenRect(LPRECT lprect);

int GetScreenRect(LPRECT lprect, int iMonitor);

int GetScreenCount();

int GetWkspaceRect(LPRECT lprect, int iMonitor);

int GetWkspaceCount();



//#define HWND_MESSAGE ((::oswindow_data *) (int_ptr) 1)


WINBOOL destroy_window(oswindow w);


oswindow get_capture();
oswindow set_capture(oswindow window);
WINBOOL release_capture();
oswindow set_focus(oswindow window);
oswindow get_focus();
oswindow get_active_window();
oswindow set_active_window(oswindow window);
WINBOOL enable_window(oswindow hwnd, WINBOOL bEnable);

//void GetMainScreenRect(LPRECT lprect);

WINBOOL SetForegroundWindow(oswindow window);
WINBOOL BringWindowToTop(oswindow window);

void ns_redraw_window(oswindow window);



void copy(CGRect & rect, LPCRECT lpcrect);
void copy(LPRECT lprect, const CGRect & rectSrc);
void window_copy(CGRect & rect, LPCRECT lpcrect);
void window_copy(LPRECT lprect, const CGRect & rectSrc);
#ifdef MM
void copy(NSRect & rect, LPCRECT lpcrect);
void copy(LPRECT lprect, const NSRect & rectSrc);
void window_copy(NSRect & rect, LPCRECT lpcrect);
void window_copy(LPRECT lprect, const NSRect & rectSrc);
#endif



