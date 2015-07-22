//
//  c_os_cross_win_user.cpp
//  c
//
//  Created by Snow Leopard User on 06/11/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "framework.h"
//#include "ios_internal.h"




//WINBOOL SetWindowPos(oswindow hwnd, oswindow hwndInsertAfter, int x, int y, int cx, int cy, UINT uFlags)
//{
//   
////   int   value_mask = 0;
//   bool  bMove = !(uFlags & SWP_NOMOVE);
//   bool  bSize = !(uFlags & SWP_NOSIZE);
//   
//   if(bMove && bSize)
//   {
//      
//      RECT rect;
//      
//      rect.left      = x;
//      rect.top       = y;
//      rect.right     = rect.left + cx;
//      rect.bottom    = rect.top + cy;
//      
//      set_nswindow_frame(hwnd, &rect, (uFlags & SWP_SHOWWINDOW));
//      
//   }
//   else if(bSize) // bSize only
//   {
//      
//      RECT rect;
//      
//      hwnd->get_user_interaction()->GetWindowRect(&rect);
//
//      rect.right     = rect.left + cx;
//      rect.bottom    = rect.top + cy;
//      
//      set_nswindow_frame(hwnd, &rect, (uFlags & SWP_SHOWWINDOW));
//      
//   }
//   else if(bMove) // bMove only
//   {
//      
//      move_nswindow(hwnd, x, y);
//      
//   }
//   
//   /*   if(!(uFlags & SWP_NOZORDER) && hwndInsertAfter >= 0)
//    {
//    value_mask |= CWSibling;
//    values.sibling = hwndInsertAfter;
//    values.stack_mode = Above;
//    }
//    
//    XConfigureWindow(display, hwnd, value_mask, &values);
//    
//    if(uFlags & SWP_SHOWWINDOW)
//    {
//    XMapWindow(display, hwnd);
//    }
//    
//    if(!(uFlags & SWP_NOZORDER) && hwndInsertAfter < 0)
//    {
//    if(hwndInsertAfter == ZORDER_TOP || hwndInsertAfter == ZORDER_TOPMOST)
//    {
//    XRaiseWindow(display, hwnd);
//    }
//    else if(hwndInsertAfter == ZORDER_BOTTOM)
//    {
//    XLowerWindow(display, hwnd);
//    }
//    
//    }
//    
//    XCloseDisplay(display);*/
//   
//   return 1;
//   
//}



//int MessageBoxANative(oswindow hwnd, const char * message, const char * header, unsigned int message_type );
//
//
//class set_boot_native_message_box
//{
//public:
//   
//   set_boot_native_message_box()
//   {
//
//      g_messageboxa = &MessageBoxANative;
//      
//   }
//   
//} g_set_boot_native_message_box;
//
//
//int MessageBoxANative(oswindow hwnd, const char * message, const char * header, unsigned int message_type )
//{
//    
// 
//   
//   //convert the strings from char* to CFStringRef
//   CFStringRef  header_ref  = CFStringCreateWithCString(NULL,  header, kCFStringEncodingUTF8);
//   CFStringRef message_ref  = CFStringCreateWithCString(NULL, message, kCFStringEncodingUTF8);
//   
//   CFOptionFlags result;  //result code from the message box
//   
//   CFStringRef button1;
//   CFStringRef button2;
//   CFStringRef button3;
//   CFStringRef buttonSwap;
//   
//   CFOptionFlags flags = 0;
//   
//   if((message_type & MB_ICONQUESTION) == MB_ICONQUESTION)
//   {
//      flags |= kCFUserNotificationStopAlertLevel;
//   }
//   else if((message_type & MB_ICONEXCLAMATION) == MB_ICONEXCLAMATION)
//   {
//      flags |= kCFUserNotificationCautionAlertLevel;
//   }
//   else if((message_type & MB_ICONASTERISK) == MB_ICONASTERISK)
//   {
//      flags |= kCFUserNotificationNoteAlertLevel;
//   }
//   else
//   {
//      flags |= kCFUserNotificationPlainAlertLevel;
//   }
//   
//   if((message_type & MB_YESNOCANCEL) == MB_YESNOCANCEL)
//   {
//      button1 = CFStringCreateWithCString(NULL,     "Yes", kCFStringEncodingUTF8);
//      button2 = CFStringCreateWithCString(NULL,      "No", kCFStringEncodingUTF8);
//      button3 = CFStringCreateWithCString(NULL,  "Cancel", kCFStringEncodingUTF8);
//   }
//   else if((message_type & MB_YESNO) == MB_YESNO)
//   {
//      button1 = CFStringCreateWithCString(NULL,     "Yes", kCFStringEncodingUTF8);
//      button2 = CFStringCreateWithCString(NULL,      "No", kCFStringEncodingUTF8);
//
//   }
//   else if((message_type & MB_YESNO) == MB_OKCANCEL)
//   {
//      button1 = CFStringCreateWithCString(NULL,      "OK", kCFStringEncodingUTF8);
//      button2 = CFStringCreateWithCString(NULL,      "Cancel", kCFStringEncodingUTF8);
//      
//   }
//   else
//   {
//      button1 = NULL;
//      button2 = NULL;
//      button3 = NULL;
//   }
//   
//   if((message_type & MB_DEFBUTTON1) == MB_DEFBUTTON1)
//   {
//   }
//   else if((message_type & MB_DEFBUTTON2) == MB_DEFBUTTON2)
//   {
//      buttonSwap     = button1;
//      button1        = button2;
//      button2        = buttonSwap;
//   }
//   else if((message_type & MB_DEFBUTTON3) == MB_DEFBUTTON3)
//   {
//      buttonSwap     = button1;
//      button1        = button3;
//      button3        = buttonSwap;
//   }
//   else
//   {
//      flags |= kCFUserNotificationNoDefaultButtonFlag;
//   }
//   
//   //launch the message box
//   CFUserNotificationDisplayAlert(
//                                  0, // no timeout
//                                  flags, //change it depending message_type flags ( MB_ICONASTERISK.... etc.)
//                                  NULL, //icon url, use default, you can change it depending message_type flags
//                                  NULL, //not used
//                                  NULL, //localization of strings
//                                  header_ref, //header text 
//                                  message_ref, //message text
//                                  button1, //default "ok" text in button
//                                  button2, //alternate button title
//                                  button3, //other button title, NULL--> no other button
//                                  &result //response flags
//                                  );
//   
//   //Clean up the strings
//   CFRelease(header_ref    );
//   CFRelease(message_ref   );
//   if(button1 != NULL)
//      CFRelease(button1);
//   if(button2 != NULL)
//      CFRelease(button2);
//   if(button3 != NULL)
//      CFRelease(button3);
//                
//    int iButton = 0;
//
//    if((message_type & MB_DEFBUTTON2) == MB_DEFBUTTON2)
//    {
//       if(result == kCFUserNotificationDefaultResponse)
//       {
//          iButton = 2;
//       }
//       else if(result == kCFUserNotificationAlternateResponse)
//       {
//          iButton = 1;
//       }
//       else if(result == kCFUserNotificationOtherResponse)
//       {
//          iButton = 3;
//       }
//    }
//    else if((message_type & MB_DEFBUTTON3) == MB_DEFBUTTON3)
//    {
//       if(result == kCFUserNotificationDefaultResponse)
//       {
//          iButton = 3;
//       }
//       else if(result == kCFUserNotificationAlternateResponse)
//       {
//          iButton = 1;
//       }
//       else if(result == kCFUserNotificationOtherResponse)
//       {
//          iButton = 2;
//       }
//    }
//    else
//    {
//       if(result == kCFUserNotificationDefaultResponse)
//       {
//          iButton = 1;
//       }
//       else if(result == kCFUserNotificationAlternateResponse)
//       {
//          iButton = 2;
//       }
//       else if(result == kCFUserNotificationOtherResponse)
//       {
//          iButton = 3;
//       }
//   }
//   
//   //Convert the result
//   if((message_type & MB_YESNOCANCEL) == MB_YESNOCANCEL)
//   {
//      if(iButton == 1)
//         return IDYES;
//      else if(iButton == 2)
//         return IDNO;
//      else
//         return IDCANCEL;
//   }
//   else if((message_type & MB_YESNO) == MB_YESNO)
//   {
//      if(iButton == 1)
//         return IDYES;
//      else
//         return IDNO;
//   }
//   else if((message_type & MB_OKCANCEL) == MB_OKCANCEL)
//   {
//      if(iButton == 1)
//         return IDOK;
//      else
//         return IDCANCEL;
//   }
//   else
//   {
//      return MB_OK;
//   }
//     
// 
//    
//    
//    return 0;
//   
//}
//*/
//
//
//WINBOOL RedrawWindow(oswindow hWnd, CONST RECT *lprcUpdate, HRGN hrgnUpdate, UINT flags)
//{
//   
//   return TRUE;
//   
//}
//
//
//

int ui_MessageBoxA(void * oswindow, const char * pszMessageParam, const char * pszHeaderParam, unsigned int uType );



int MessageBoxANative(oswindow hwnd, const char * message, const char * header, unsigned int message_type )
{
   
   string strMessage(message); // string "absorbs" NULL pointers into ""
   
   string strHeader(header); // string "absorbs" NULL pointers into ""
   
   return ui_MessageBoxA((void *) hwnd, strMessage, strHeader, message_type);
   
}






int MessageBoxW(oswindow hwnd, const char16_t * message, const char16_t * header, unsigned int message_type )
{
   
   string strMessage(message); // string "absorbs" wide strings into utf8
   
   string strHeader(header); // string "absorbs" wide strings into utf8
   
   return MessageBoxANative(hwnd, strMessage, strHeader, message_type);
   
}
