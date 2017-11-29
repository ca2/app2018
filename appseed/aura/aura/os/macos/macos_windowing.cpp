//
//  macos_window.cpp
//  aura
//
//  Created by Camilo Sasuke Tsumanuma on 2013-09-17.
//
//

#include "framework.h"
#include "macos_window_impl.h"


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


void oswindow_data::set_user_interaction_impl(::user::interaction_impl * pimpl)
{

   if(is_null())
      _throw(simple_exception(get_app(), "error, m_pdata cannot be NULL to ::oswindow::set_user_interaction"));

   m_pimpl = pimpl;

}


::user::interaction * oswindow_data::get_user_interaction_base()
{

   if(is_null())
   {
      
      return NULL;
      
   }

   if(m_pimpl == NULL)
   {
      
      return NULL;
      
   }
   
   return m_pimpl->m_pui;

}


::user::interaction * oswindow_data::get_user_interaction_base() const
{
   
   if(is_null())
   {
      
      return NULL;
      
   }
   
   if(m_pimpl == NULL)
   {
      
      return NULL;
      
   }
   
   return m_pimpl->m_pui;
   
}


::user::interaction * oswindow_data::get_user_interaction()
{
   
   if(is_null())
   {
      
      return NULL;
      
   }
   
   if(m_pimpl == NULL)
   {
      
      return NULL;
      
   }
   
   return m_pimpl->m_pui;
   
}


::user::interaction * oswindow_data::get_user_interaction() const
{
   
   if(is_null())
   {
      
      return NULL;
      
   }
   
   if(m_pimpl == NULL)
   {
      
      return NULL;
      
   }
   
   return m_pimpl->m_pui;
   
}


oswindow oswindow_data::get_parent()
{

   if(is_null())
      return NULL;

   return m_pimpl->m_pui->GetParentHandle();

}


oswindow oswindow_data::set_parent(oswindow oswindow)
{

   if(is_null())
      return NULL;

   ::oswindow oswindowOldParent = get_parent();

   if(oswindow == NULL
      || oswindow->m_pimpl == NULL)
   {

      m_pimpl->SetParent(NULL);

   }
   else
   {

      m_pimpl->SetParent(oswindow->m_pimpl->m_pui);

   }

   return oswindowOldParent;

}



bool oswindow_data::is_child(::oswindow oswindow)
{
    
    if (oswindow == NULL || oswindow->m_pimpl == NULL || oswindow->m_pimpl->m_pui == NULL)
    {
        
        return false;
        
    }
    
    if (m_pimpl == NULL || m_pimpl->m_pui == NULL)
    {
        
        return false;
        
    }
    
    return m_pimpl->m_pui->IsChild(oswindow->m_pimpl->m_pui);
    
}


int_ptr oswindow_data::get_window_long_ptr(int_ptr iIndex)
{

   if(is_null())
      return 0;

   if(m_plongmap == NULL)
      return 0;

   return m_plongmap->operator[]((int) iIndex);

}


int_ptr oswindow_data::set_window_long_ptr(int_ptr iIndex, int_ptr iNewLong)
{

   if(is_null())
      return 0;

   if(m_plongmap == NULL)
      return 0;

   int_ptr iLong = m_plongmap->operator[]((int) iIndex);

//   if (iIndex == GWL_EXSTYLE)
//   {
//
//      defer_dock_application(iLong & WS_EX_TOOLWINDOW);
//
//   }

   m_plongmap->operator[]((int) iIndex) = (int) iNewLong;

   return iLong;

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


::user::interaction_impl * window_from_handle(oswindow oswindow)
{

   if(oswindow == NULL)
      return NULL;

   return oswindow->m_pimpl;

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
   
   synch_lock sl(g_poswindowdataptra->m_pmutex);

   oswindow windowOld(g_oswindowActive);
   
   if(window == NULL)
   {
      
      g_oswindowActive = NULL;
      
      return windowOld;
      
   }

   if(window->window() == NULL)
   {
      
      g_oswindowActive = NULL;
      
      return windowOld;
      
   }

   g_oswindowActive = window;

   return windowOld;

}





oswindow GetWindow(oswindow window, int iParentHood)
{
   return NULL;
}




void defer_dock_application(bool bDock)
{

   if (bDock)
   {

      if (!nsapp_activation_policy_is_regular())
      {

         nsapp_activation_policy_regular();

      }

   }
   else
   {

      if (!nsapp_activation_policy_is_accessory())
      {

         nsapp_activation_policy_accessory();

      }

   }

}


WINBOOL DestroyWindow(oswindow w)
{
   
   UNREFERENCED_PARAMETER(w);
   
   return 1;
   
}





bool oswindow_data::is_null() const
{
   
   return ::is_null(this);
   
}






//
//  c_os_cross_win_user.cpp
//  c
//
//  Created by Snow Leopard User on 06/11/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "framework.h"



#ifdef DEBUG
#undef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>

#if DEBUG
#undef DEBUG
#define DEBUG
#else
#undef DEBUG
#endif


//
// 
// WINBOOL SetWindowPos(oswindow hwnd, oswindow hwndInsertAfter, int x, int y, int cx, int cy, UINT uFlags)
// {
// 
// //   int   value_mask = 0;
// bool  bMove = !(uFlags & SWP_NOMOVE);
// bool  bSize = !(uFlags & SWP_NOSIZE);
// 
// if(bMove && bSize)
// {
// 
// RECT rect;
// 
// rect.left      = x;
// rect.top       = y;
// rect.right     = rect.left + cx;
// rect.bottom    = rect.top + cy;
// 
// set_nswindow_frame(hwnd, &rect, (uFlags & SWP_SHOWWINDOW));
// 
// }
// else if(bSize) // bSize only
// {
// 
// RECT rect;
// 
// GetWindowRect(hwnd, &rect);
// 
// rect.right     = rect.left + cx;
// rect.bottom    = rect.top + cy;
// 
// set_nswindow_frame(hwnd, &rect, (uFlags & SWP_SHOWWINDOW));
// 
// }
// else if(bMove) // bMove only
// {
// 
// move_nswindow(hwnd, x, y);
// 
// }
// 
// if(!(uFlags & SWP_NOZORDER))
// {
// 
// int_ptr iInsertAfter = (int_ptr) hwndInsertAfter;
// 
// if(iInsertAfter == ZORDER_TOP || iInsertAfter == ZORDER_TOPMOST)
// {
// 
// order_front_nswindow(hwnd);
// 
// }
// 
// }
// 
// }


/*   if(!(uFlags & SWP_NOZORDER) && hwndInsertAfter >= 0)
 {
 value_mask |= CWSibling;
 values.sibling = hwndInsertAfter;
 values.stack_mode = Above;
 }
 
 XConfigureWindow(display, hwnd, value_mask, &values);
 
 if(uFlags & SWP_SHOWWINDOW)
 {
 XMapWindow(display, hwnd);
 }
 
 if(!(uFlags & SWP_NOZORDER) && hwndInsertAfter < 0)
 {
 if(hwndInsertAfter == ZORDER_TOP || hwndInsertAfter == ZORDER_TOPMOST)
 {
 XRaiseWindow(display, hwnd);
 }
 else if(hwndInsertAfter == ZORDER_BOTTOM)
 {
 XLowerWindow(display, hwnd);
 }
 
 }
 
 XCloseDisplay(display);*/

//   return 1;

//}

//int MessageBoxANative(oswindow hwnd, const char * message, const char * header, unsigned int message_type );
/*
 
 class set_boot_native_message_box
 {
 public:
 
 set_boot_native_message_box()
 {
 
 g_messageboxa = &MessageBoxANative;
 
 }
 
 } g_set_boot_native_message_box;
 
 int MessageBoxANative(oswindow hwnd, const char * message, const char * header, unsigned int message_type )
 {
 
 //convert the strings from char* to CFStringRef
 CFStringRef  header_ref  = CFStringCreateWithCString(NULL,  header, kCFStringEncodingUTF8);
 CFStringRef message_ref  = CFStringCreateWithCString(NULL, message, kCFStringEncodingUTF8);
 
 CFOptionFlags result;  //result code from the message box
 
 CFStringRef button1;
 CFStringRef button2;
 CFStringRef button3 = nullptr;
 CFStringRef buttonSwap;
 
 CFOptionFlags flags = 0;
 
 if((message_type & MB_ICONQUESTION) == MB_ICONQUESTION)
 {
 flags |= kCFUserNotificationStopAlertLevel;
 }
 else if((message_type & MB_ICONEXCLAMATION) == MB_ICONEXCLAMATION)
 {
 flags |= kCFUserNotificationCautionAlertLevel;
 }
 else if((message_type & MB_ICONASTERISK) == MB_ICONASTERISK)
 {
 flags |= kCFUserNotificationNoteAlertLevel;
 }
 else
 {
 flags |= kCFUserNotificationPlainAlertLevel;
 }
 
 if((message_type & MB_YESNOCANCEL) == MB_YESNOCANCEL)
 {
 button1 = CFStringCreateWithCString(NULL,     "Yes", kCFStringEncodingUTF8);
 button2 = CFStringCreateWithCString(NULL,      "No", kCFStringEncodingUTF8);
 button3 = CFStringCreateWithCString(NULL,  "Cancel", kCFStringEncodingUTF8);
 }
 else if((message_type & MB_YESNO) == MB_YESNO)
 {
 button1 = CFStringCreateWithCString(NULL,     "Yes", kCFStringEncodingUTF8);
 button2 = CFStringCreateWithCString(NULL,      "No", kCFStringEncodingUTF8);
 
 }
 else if((message_type & MB_YESNO) == MB_OKCANCEL)
 {
 button1 = CFStringCreateWithCString(NULL,      "OK", kCFStringEncodingUTF8);
 button2 = CFStringCreateWithCString(NULL,      "Cancel", kCFStringEncodingUTF8);
 
 }
 else
 {
 button1 = NULL;
 button2 = NULL;
 button3 = NULL;
 }
 
 if((message_type & MB_DEFBUTTON1) == MB_DEFBUTTON1)
 {
 }
 else if((message_type & MB_DEFBUTTON2) == MB_DEFBUTTON2)
 {
 buttonSwap     = button1;
 button1        = button2;
 button2        = buttonSwap;
 }
 else if((message_type & MB_DEFBUTTON3) == MB_DEFBUTTON3)
 {
 buttonSwap     = button1;
 button1        = button3;
 button3        = buttonSwap;
 }
 else
 {
 flags |= kCFUserNotificationNoDefaultButtonFlag;
 }
 
 //launch the message box
 CFUserNotificationDisplayAlert(
 0, // no timeout
 flags, //change it depending message_type flags ( MB_ICONASTERISK.... etc.)
 NULL, //icon url, use default, you can change it depending message_type flags
 NULL, //not used
 NULL, //localization of strings
 header_ref, //header text
 message_ref, //message text
 button1, //default "ok" text in button
 button2, //alternate button title
 button3, //other button title, NULL--> no other button
 &result //response flags
 );
 
 //Clean up the strings
 CFRelease(header_ref    );
 CFRelease(message_ref   );
 if(button1 != NULL)
 CFRelease(button1);
 if(button2 != NULL)
 CFRelease(button2);
 if(button3 != NULL)
 CFRelease(button3);
 
 int iButton = 0;
 
 if((message_type & MB_DEFBUTTON2) == MB_DEFBUTTON2)
 {
 if(result == kCFUserNotificationDefaultResponse)
 {
 iButton = 2;
 }
 else if(result == kCFUserNotificationAlternateResponse)
 {
 iButton = 1;
 }
 else if(result == kCFUserNotificationOtherResponse)
 {
 iButton = 3;
 }
 }
 else if((message_type & MB_DEFBUTTON3) == MB_DEFBUTTON3)
 {
 if(result == kCFUserNotificationDefaultResponse)
 {
 iButton = 3;
 }
 else if(result == kCFUserNotificationAlternateResponse)
 {
 iButton = 1;
 }
 else if(result == kCFUserNotificationOtherResponse)
 {
 iButton = 2;
 }
 }
 else
 {
 if(result == kCFUserNotificationDefaultResponse)
 {
 iButton = 1;
 }
 else if(result == kCFUserNotificationAlternateResponse)
 {
 iButton = 2;
 }
 else if(result == kCFUserNotificationOtherResponse)
 {
 iButton = 3;
 }
 }
 
 //Convert the result
 if((message_type & MB_YESNOCANCEL) == MB_YESNOCANCEL)
 {
 if(iButton == 1)
 return IDYES;
 else if(iButton == 2)
 return IDNO;
 else
 return IDCANCEL;
 }
 else if((message_type & MB_YESNO) == MB_YESNO)
 {
 if(iButton == 1)
 return IDYES;
 else
 return IDNO;
 }
 else if((message_type & MB_OKCANCEL) == MB_OKCANCEL)
 {
 if(iButton == 1)
 return IDOK;
 else
 return IDCANCEL;
 }
 else
 {
 return MB_OK;
 }
 
 }
 */

WINBOOL RedrawWindow(oswindow hWnd, CONST RECT *lprcUpdate, HRGN hrgnUpdate, UINT flags)
{
   
   return TRUE;
   
}




const char * mm_keyboard_input_source();

string keyboard_input_source()
{
   const char * psz = mm_keyboard_input_source();
   string str =psz;
   free((void *) psz);
   return str;
}



CLASS_DECL_AURA ::user::interaction_impl * oswindow_get(oswindow oswindow)
{
    
    if(oswindow == NULL)
    {
        
        return NULL;
        
    }
    
    return oswindow->m_pimpl;
    
}


