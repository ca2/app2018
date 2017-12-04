//
//  macos_window.cpp
//  aura
//
//  Created by Camilo Sasuke Tsumanuma on 2013-09-17.
//
//

#include "framework.h"
#include "ios_window_impl.h"


oswindow_dataptra * g_poswindowdataptra = NULL;


int oswindow_find(nswindow window)
{

   for(int i = 0; i < g_poswindowdataptra->get_count(); i++)
   {
      if(g_poswindowdataptra->element_at(i)->m_nswindow == window)
      {
         return i;
      }
   }

   return -1;

}

oswindow_data * oswindow_get(nswindow window)
{

   int_ptr iFind = oswindow_find(window);

   if(iFind >= 0)
      return g_poswindowdataptra->element_at(iFind);

   ::oswindow_data * pdata = new ::oswindow_data;

   pdata->m_nswindow    = window;

   g_poswindowdataptra->add(pdata);

   return pdata;

}


oswindow_data::oswindow_data()
{

   m_nswindow  = NULL;
   m_pimpl       = NULL;
   m_plongmap  = new int_to_int;

}


oswindow_data::oswindow_data(nswindow window)
{

   m_nswindow  = window;
   m_pimpl       = NULL;
   m_plongmap  = new int_to_int;

}


oswindow_data::oswindow_data(const oswindow_data & oswindow)
{

   m_nswindow  = oswindow.m_nswindow;
   m_pimpl       = oswindow.m_pimpl;
   m_plongmap  = oswindow.m_plongmap;

}


oswindow_data::~oswindow_data()
{

   delete m_plongmap;

}


oswindow_data & oswindow_data::operator = (const oswindow_data & oswindow)
{

   if(&oswindow != this)
   {

      m_nswindow  = oswindow.m_nswindow;
      m_pimpl       = oswindow.m_pimpl;
      m_plongmap  = oswindow.m_plongmap;

   }

   return *this;

}


bool oswindow_remove(nswindow window)
{

   int_ptr iFind = oswindow_find(window);

   if(iFind < 0)
      return false;

   g_poswindowdataptra->remove_at(iFind);

   return true;

}


void oswindow_data::set_user_interaction(::user::interaction_impl * pui)
{

   if(::is_null(*this))
      _throw(simple_exception(get_app(), "error, m_pdata cannot be NULL to ::oswindow::set_user_interaction"));

   m_pimpl = pui;

}


::user::interaction_impl * oswindow_data::get_user_interaction_base()
{

   if(::is_null(*this))
      return NULL;

   return m_pimpl;

}

::user::interaction_impl * oswindow_data::get_user_interaction_base() const
{

   if(::is_null(*this))
      return NULL;

   return m_pimpl;

}

::user::interaction_impl * oswindow_data::get_user_interaction()
{

   if(::is_null(*this))
      return NULL;

   if(m_pimpl == NULL)
      return NULL;

   return m_pimpl;

}

::user::interaction_impl * oswindow_data::get_user_interaction() const
{

   if(::is_null(*this))
      return NULL;

   if(m_pimpl == NULL)
      return NULL;

   return m_pimpl;

}


oswindow oswindow_data::get_parent()
{

   if(::is_null(this))
      return NULL;

   if(m_pimpl->GetParent() == NULL)
      return NULL;

   return m_pimpl->GetParent()->get_handle();

}


oswindow oswindow_data::set_parent(oswindow oswindow)
{

   if(::is_null(this))
      return NULL;

   ::oswindow oswindowOldParent = get_parent();

//   if(oswindow == NULL
//      || oswindow->m_pimpl == NULL)
//   {
//
//      m_pimpl->set_parent_base(NULL);
//
//   }
//   else
//   {
//
//      m_pimpl->set_parent_base(oswindow->m_pimpl);
//
//   }

   return oswindowOldParent;

}


int32_t oswindow_data::get_window_long(int32_t iIndex)
{

   if(::is_null(*this))
      return 0;

   if(m_plongmap == NULL)
      return 0;

   return m_plongmap->operator[](iIndex);

}


int32_t oswindow_data::set_window_long(int32_t iIndex, int32_t iNewLong)
{

   if(::is_null(*this))
      return 0;

   if(m_plongmap == NULL)
      return 0;

   int32_t iLong = m_plongmap->operator[](iIndex);

   m_plongmap->operator[](iIndex) = iNewLong;

   return iLong;

}


::user::interaction_impl * window_from_handle(oswindow oswindow)
{

   if(oswindow == NULL)
      return NULL;

   return oswindow->m_pimpl;

}

static oswindow g_oswindowCapture;


oswindow GetCapture()
{
   return g_oswindowCapture;
}

oswindow SetCapture(oswindow window)
{

   oswindow windowOld(g_oswindowCapture);

   if(window->window() == NULL)
      return NULL;

   g_oswindowCapture = window;

   /*   if(XGrabPointer(window.display(), window.window(), False, ButtonPressMask | ButtonReleaseMask | PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None, 8CurrentTime) == GrabSuccess)
    {

    g_oswindowCapture = window;

    return windowOld;

    }*/

   return windowOld;

}


WINBOOL ReleaseCapture()
{

   //ifGetCapture().display() == NULL)
   // return FALSE;

   //  WINBOOL bRet = XUngrabPointer(GetCapture().display(), CurrentTime) != FALSE;

   WINBOOL bRet = TRUE;

   if(bRet)
      g_oswindowCapture = NULL;

   return bRet;

}


oswindow GetFocus();


oswindow SetFocus(oswindow window)
{

   if(!IsWindow(window))
      return NULL;

   oswindow windowOld = ::GetFocus();

   return windowOld;

}

oswindow GetFocus()
{

   //isplay * pdisplay = XOpenDisplay(NULL);

   //   Window window = None;

   //   int revert_to = 0;

   //bool bOk = XGetInputFocus(pdisplay, &window, &revert_to) != 0;

   //XCloseDisplay(pdisplay);

   //   if(!bOk)
   //    return NULL;

   // if(window == None || window == PointerRoot)
   //  return NULL;

   //return oswindow::defer_get(window);
   return NULL;

}




static oswindow g_oswindowActive = NULL;


oswindow GetActiveWindow()
{

   return g_oswindowActive;

}


void DeactivateWindow(oswindow window)
{
   
   synch_lock sl(g_poswindowdataptra->m_pmutex);
   
   if(GetActiveWindow() != window)
   {
      
      return;
      
   }
   
   SetActiveWindow(NULL);
   
}


oswindow SetActiveWindow(oswindow window)
{

   oswindow windowOld(g_oswindowActive);

   if(window->window() == NULL)
      return NULL;

   g_oswindowActive = window;

   return windowOld;

}


oswindow GetWindow(oswindow window, int iParentHood)
{

   return NULL;

}




WINBOOL ui_SetWindowPos(oswindow hwnd, oswindow hwndInsertAfter, int x, int y, int cx, int cy, UINT uFlags)
{
   if(hwnd == NULL)
      return FALSE;
   if(hwnd->m_pimpl == NULL)
      return FALSE;
   return hwnd->m_pimpl->SetWindowPos((int_ptr)hwndInsertAfter, x, y, cx, cy, uFlags);

}



WINBOOL GetWindowRect(oswindow hwnd, LPRECT lprect)
{
   if(void_ptr_is_null(hwnd))
      return FALSE;
   if(void_ptr_is_null(lprect))
      return FALSE;
   lprect->left =hwnd->m_x;
   lprect->top = hwnd->m_y;
   lprect->right = hwnd->m_x + hwnd->m_cx;
   lprect->bottom = hwnd->m_y + hwnd->m_cy;
   return TRUE;
}

WINBOOL SetWindowRect(oswindow hwnd, LPRECT lprect)
{
   
   if(hwnd == NULL)
      return FALSE;
   
hwnd->m_x = lprect->left;
hwnd->m_y = lprect->top;
hwnd->m_cx = lprect->right - lprect->left ;
hwnd->m_cy = lprect->bottom - lprect->top;
   
   return TRUE;
   
}






void defer_dock_application(bool bDock)
{
   
//   if (bDock)
//   {
//      
//      if (!nsapp_activation_policy_is_regular())
//      {
//         
//         nsapp_activation_policy_regular();
//         
//      }
//      
//   }
//   else
//   {
//      
//      if (!nsapp_activation_policy_is_accessory())
//      {
//         
//         nsapp_activation_policy_accessory();
//         
//      }
//      
//   }
//   
}


int_bool DestroyWindow(oswindow window)
{
   
   UNREFERENCED_PARAMETER(window);
   
   return true;
   
}
