#include "framework.h"
#include <GdiPlus.h>


void simple_password::draw_this(HDC hdc)
{

   Gdiplus::Graphics graphics2(hdc);

   graphics2.SetCompositingMode(Gdiplus::CompositingModeSourceOver);

   Gdiplus::SolidBrush br(Gdiplus::Color(184, 255, 255, 240));

   graphics2.FillRectangle(&br, m_rect.left, m_rect.top, width(&m_rect), height(&m_rect));

   if(get_focus() == this)
   {

      Gdiplus::Pen pen(Gdiplus::Color(255, 84, 77, 255), 3.0f);

      graphics2.DrawRectangle(&pen, m_rect.left, m_rect.top, width(&m_rect), height(&m_rect));

   }
   else
   {

      Gdiplus::Pen pen(Gdiplus::Color(255, 149, 149, 123), 1.0f);

      graphics2.DrawRectangle(&pen, m_rect.left, m_rect.top, width(&m_rect), height(&m_rect));

   }

   Gdiplus::SolidBrush b(Gdiplus::Color(223, 49, 49, 23));

   Gdiplus::Font f(L"Geneva", height(&m_rect), 0, Gdiplus::UnitPixel);

   int iLen = m_strText.get_length();

   wchar_t * pwsz = (wchar_t *) ca2_alloc((iLen + 1) * 2);

   pwsz[iLen] = L'\0';
   while(iLen > 0)
   {
      iLen--;
      pwsz[iLen] = L'*';
   }

   graphics2.DrawString(pwsz, wcslen_dup(pwsz), &f, Gdiplus::PointF(m_rect.left + 2, m_rect.top), &b);

   ca2_free(pwsz);

}










