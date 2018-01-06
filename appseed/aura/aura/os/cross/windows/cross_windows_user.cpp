#include "framework.h"
#if defined(LINUX)
#include "aura/aura/os/linux/linux_user_impl.h"
#endif
#if defined(APPLEOS) || defined(LINUX) || defined(ANDROID)
#include <iconv.h>
#else
//#include "atom/iconv/include/iconv.h"
#endif


CLASS_DECL_AURA COLORREF GetSysColor(DWORD dw)
{

   switch(dw)
   {
   case COLOR_HIGHLIGHT:
      return ARGB(255, 200, 200, 196);
   case COLOR_BTNSHADOW:
      return ARGB(84, 192, 192, 187);
   case COLOR_BTNTEXT:
      return ARGB(255, 0, 0, 0);
   default:
      return ARGB(255, 0, 0, 0);
   };

}



CLASS_DECL_AURA int32_t WINAPI GetSystemMetrics(int32_t nIndex)
{

   switch(nIndex)
   {
   case SM_CXHSCROLL:
      return 16;
   case SM_CYHSCROLL:
      return 16;
   case SM_CYVSCROLL:
      return 16;
   case SM_CXVSCROLL:
      return 16;
   }

   return 0;

}



CLASS_DECL_AURA int_bool CopyRect(LPRECT prectDest, LPCRECT pcrectSrc)
{

   prectDest->left      = pcrectSrc->left;
   prectDest->top       = pcrectSrc->top;
   prectDest->right     = pcrectSrc->right;
   prectDest->bottom    = pcrectSrc->bottom;

   return true;

}

CLASS_DECL_AURA int_bool PtInRect(LPCRECT prect, POINT point)
{
   return point.x >= prect->left && point.x <= prect->right
          && point.y >= prect->top && point.y <= prect->bottom;

}

CLASS_DECL_AURA int_bool SetRect(LPRECT prect, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{

   prect->left = x1;
   prect->top = y1;

   prect->right = x2;
   prect->bottom = y2;
   return true;
}


CLASS_DECL_AURA int_bool SetRectEmpty(LPRECT prect)
{

   SetRect(prect, 0, 0, 0, 0);
   return true;
}


CLASS_DECL_AURA int_bool EqualRect(LPCRECT prect1, LPCRECT prect2)
{
   return prect1->left == prect2->left
          &&  prect1->top == prect2->top
          &&  prect1->right == prect2->right
          &&  prect1->bottom == prect2->bottom;
}


CLASS_DECL_AURA int_bool InflateRect(LPRECT prect, int32_t x, int32_t y)
{
   prect->left -= x;
   prect->top -= y;
   prect->right += x;
   prect->bottom += y;
   return true;
}

CLASS_DECL_AURA int_bool OffsetRect(LPRECT prect, int32_t x, int32_t y)
{
   prect->left += x;
   prect->top += y;
   prect->right += x;
   prect->bottom += y;
   return true;
}

/*
CLASS_DECL_AURA bool x_intersect_rect(LPRECT prect, LPCRECT prect1, LPCRECT prect2)
{
   prect->left = MAX(prect1->left, prect2->left);
   prect->right = MIN(prect1->right, prect2->right);
   return prect->right >= prect->left;
}

CLASS_DECL_AURA bool y_intersect_rect(LPRECT prect, LPCRECT prect1, LPCRECT prect2)
{
   prect->top = MAX(prect1->top, prect2->top);
   prect->bottom = MIN(prect1->bottom, prect2->bottom);
   return prect->bottom >= prect->top;
}
*/

CLASS_DECL_AURA int_bool IntersectRect(LPRECT prect, LPCRECT prect1, LPCRECT prect2)
{
   int_bool bXIntersects = x_intersect_rect(prect, prect1, prect2);
   int_bool bYIntersects = y_intersect_rect(prect, prect1, prect2);
   return bXIntersects && bYIntersects;
}


CLASS_DECL_AURA int_bool UnionRect(LPRECT prect, LPCRECT prect1, LPCRECT prect2)
{
   prect->left = MIN(prect1->left, prect2->left);
   prect->top = MIN(prect1->top, prect2->top);
   prect->right = MAX(prect1->right, prect2->right);
   prect->bottom = MAX(prect1->bottom, prect2->bottom);
   return ((prect->right - prect->left) > 0) && ((prect->bottom - prect->top) > 0);
}


CLASS_DECL_AURA int_bool SubtractRect(LPRECT prect, LPCRECT prect1, LPCRECT prect2)
{
   RECT rect;
   int_bool bXIntersects = x_intersect_rect(&rect, prect1, prect2);
   int_bool bYIntersects = y_intersect_rect(&rect, prect1, prect2);
   if(bXIntersects)
   {
      prect->left    = rect.left;
      prect->right   = rect.right;
   }
   if(bYIntersects)
   {
      prect->top    = rect.top;
      prect->bottom   = rect.bottom;
   }
   return bXIntersects || bYIntersects;
}


#ifndef METROWIN

string iconv_charset_from_windows_code_page(UINT CodePage)
{

   switch(CodePage)
   {
   case 1252:
      return "CP1252";
   case 437:
      return "CP437";
   default:
      return "UTF-8";

   }

}

int32_t
WINAPI
MultiByteToWideChar(
UINT     CodePage,
DWORD    dwFlags,
LPCSTR   lpMultiByteStr,
int32_t      cbMultiByte,
LPWSTR  lpWideCharStr,
int32_t      cchWideChar)
{
   string str(lpMultiByteStr, cbMultiByte);

   if(CodePage == CP_UTF8)
   {

      str = string(lpMultiByteStr, cbMultiByte);

      wstring wstr(str);

      if(cchWideChar == 0)
      {
         if(cbMultiByte < 0)
         {
            return (int32_t) (wstr.get_length() + 1);
         }
         else
         {
            return (int32_t) wstr.get_length();
         }
      }

      int32_t iLen = (int32_t) MIN(cchWideChar, wstr.get_length());

      if(lpWideCharStr != NULL)
      {

         wcsncpy_dup(lpWideCharStr, wstr, iLen);

         if(cchWideChar > 0 && cbMultiByte < 0)
         {

            lpWideCharStr[iLen] = L'\0';

         }

      }

      if(cbMultiByte < 0)
      {
         return iLen + 1;
      }
      else
      {
         return iLen;
      }

   }
   else
   {

      size_t sIn;

      if(cbMultiByte < 0)
         sIn = strlen(lpMultiByteStr);
      else
         sIn = cbMultiByte;

      if(cchWideChar <= 0)
      {

         wstring wstr;

         size_t sOut = sIn * 4;

         wstr.alloc(sOut);

         sOut *= sizeof(unichar);

         unichar * lpsz = (unichar *) (const unichar *) wstr;

         lpWideCharStr = lpsz;

         iconv_t iconvPlease = iconv_open("UTF-16LE", iconv_charset_from_windows_code_page(CodePage));

         size_t sOutIn = sOut;

         iconv(iconvPlease, (char **) &lpMultiByteStr, &sIn, (char **) &lpsz, &sOut);

         iconv_close(iconvPlease);

         return (int32_t) (((sOutIn - sOut) / sizeof(unichar)) + (cbMultiByte < 0 ? 1 : 0));

      }
      else
      {

         unichar * lpsz = (unichar *) (const unichar *) lpWideCharStr;

         size_t sOut = cchWideChar * sizeof(unichar);

         iconv_t iconvPlease = iconv_open("UTF-16LE", iconv_charset_from_windows_code_page(CodePage));

         size_t sOutIn = sOut;

         iconv(iconvPlease, (char **) &lpMultiByteStr, &sIn, (char **) &lpsz, &sOut);

         iconv_close(iconvPlease);

         return (int32_t) (((sOutIn - sOut) / sizeof(unichar))  + (cbMultiByte < 0 ? 1 : 0));

      }

   }





}



int32_t
WINAPI
WideCharToMultiByte(
UINT     CodePage,
DWORD    dwFlags,
LPCWSTR  lpWideCharStr,
int32_t      cchWideChar,
LPSTR   lpMultiByteStr,
int32_t      cbMultiByte,
LPCSTR   lpDefaultChar,
LPBOOL  lpUsedDefaultChar)
{

   wstring wstr(lpWideCharStr, cchWideChar);

   string str;




   if(CodePage == CP_UTF8)
   {

      LPSTR lpsz = str.GetBufferSetLength(utf8_len(wstr));

      utf16_to_utf8(lpsz, wstr);

   }
   else
   {

      size_t sIn = wstr.get_length() * 2;

      LPSTR lpsz = str.GetBufferSetLength(sIn);

      size_t sOut = wstr.get_length() * 8;

      string strCodePage = iconv_charset_from_windows_code_page(CodePage);

      iconv_t iconvPlease = iconv_open(strCodePage, "UTF-16LE");

      iconv(iconvPlease, (char **) &lpWideCharStr, &sIn, &lpsz, &sOut);

      iconv_close(iconvPlease);

   }

   str.ReleaseBuffer();

   if(cbMultiByte == 0)
   {
      if(cchWideChar < 0)
      {
         return (int32_t) str.get_length() + 1;
      }
      else
      {
         return (int32_t) str.get_length();
      }
   }

   int32_t iLen = (int32_t) MIN(cbMultiByte, str.get_length());

   if(lpMultiByteStr != NULL)
   {

      strncpy(lpMultiByteStr, str, iLen);

      if(cbMultiByte > 0 && cchWideChar < 0)
      {

         lpMultiByteStr[iLen] = '\0';

      }

   }

   if(cchWideChar < 0)
   {
      return iLen + 1;
   }
   else
   {
      return iLen;
   }

}


CLASS_DECL_AURA string get_system_error_message(uint32_t dwError)
{

   string str;

   return str;

}


#endif


// please include ansios_message_loop.
//CLASS_DECL_AURA int_bool TranslateMessage(const MESSAGE * pmsg)
//{
//
//   if(pmsg == NULL)
//      return FALSE;
//
//   if(pmsg->hwnd == NULL)
//      return FALSE;
//
//   if(pmsg->hwnd->m_pimpl == NULL)
//      return FALSE;
//
//   if (pmsg->hwnd->m_pimpl->m_pui == NULL)
//      return FALSE;
//
//   return FALSE;
//
//}
//
// please include ansios_message_loop.
//CLASS_DECL_AURA LRESULT DispatchMessage(const MESSAGE * pmsg)
//{
//
//   if(pmsg == NULL)
//      return 0;
//
//   if(pmsg->hwnd == NULL)
//      return 0;
//
//   if(pmsg->hwnd->m_pimpl == NULL)
//      return 0;
//
//   if (pmsg->hwnd->m_pimpl->m_pui == NULL)
//      return 0;
//
//   return pmsg->hwnd->m_pimpl->m_pui->send_message(pmsg->message,pmsg->wParam,pmsg->lParam);
//
//}
//





CLASS_DECL_AURA int_bool IsRectEmpty(LPCRECT lpcrect)
{

   return lpcrect == NULL || lpcrect->right <=lpcrect->left || lpcrect->bottom <= lpcrect->top;

}






WINBOOL IsChild(oswindow oswindowParent, ::oswindow oswindowcandidateChildOrDescendant)
{
   return oswindowParent->is_child(oswindowcandidateChildOrDescendant);
}

oswindow WINAPI GetParent(::oswindow oswindow)
{
   return oswindow->get_parent();
}

oswindow WINAPI SetParent(::oswindow oswindow, ::oswindow oswindowNewParent)
{
   return oswindow->set_parent(oswindowNewParent);
}

WINBOOL ShowWindow(::oswindow oswindow, int nCmdShow)
{
   return oswindow->show_window(nCmdShow);
}

LONG WINAPI GetWindowLongA(::oswindow oswindow, int nIndex)
{
   return (LONG) oswindow->m_pimpl->get_window_long(nIndex);
}

LONG WINAPI SetWindowLongA(::oswindow oswindow, int nIndex, LONG l)
{
   return (LONG) oswindow->m_pimpl->set_window_long(nIndex, (LONG) l);
}

LONG_PTR WINAPI GetWindowLongPtrA(::oswindow oswindow, int nIndex)
{
   return oswindow->get_window_long(nIndex);
}

LONG_PTR WINAPI SetWindowLongPtrA(::oswindow oswindow, int nIndex, LONG_PTR l)
{
   return oswindow->set_window_long(nIndex, (int)l);
}

WINBOOL WINAPI ClientToScreen(::oswindow oswindow, LPPOINT lppoint)
{
   return oswindow->client_to_screen(lppoint);
}

WINBOOL WINAPI ScreenToClient(::oswindow oswindow, LPPOINT lppoint)
{
   return oswindow->screen_to_client(lppoint);
}

WINBOOL WINAPI IsIconic(::oswindow oswindow)
{
   return oswindow->is_iconic();
}

WINBOOL WINAPI IsWindowVisible(::oswindow oswindow)
{
   return oswindow->is_window_visible();
}


#ifndef METROWIN

#define GetWindowLong GetWindowLongA
#define SetWindowLong SetWindowLongA

#endif

CLASS_DECL_AURA WINBOOL IsWindow(oswindow oswindow);

