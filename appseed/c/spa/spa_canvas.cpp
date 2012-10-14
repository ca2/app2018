#include "framework.h"


#ifdef LINUX
#include "c/linux/c_os_cross_win_gdi_internal.h"
#endif


#include <math.h>


namespace spa
{

   canvas::canvas()
   {

      m_iMode = 0;

#ifdef SUPORTA_TELA_AVANCADA

      m_iModeCount = 6;

#else

      m_iModeCount = 4;

#endif

   }

   canvas::~canvas()
   {
   }

//#define SUPORTA_TELA_AVANCADA

#ifdef SUPORTA_TELA_AVANCADA
class canvas_zero
{
public:
   DWORD m_dwCurZero;
   DWORD m_dwNextZero;
   int m_iMaxSize;
   int m_iLast;
   simple_array < POINT > m_pta;
   simple_int_array m_ia;
   RECT m_rect;
   HBITMAP m_hbm;
   HDC m_hdc;
   HBITMAP m_hbmZero;
   HBITMAP m_hbmZeroOld;
   HDC m_hdcZero;
   COLORREF * m_pdata;
   COLORREF * m_pdataZero;
   canvas_zero();
   void prepare(HDC hdc, LPCRECT lpcrect);
   void on_paint(simple_graphics & g, LPCRECT lpcrect);
   virtual void zero(HDC hdc, POINT pt, int iSize, int iStep);
};

#endif

int canvas::increment_mode()
{

   m_iMode++;

   if(m_iMode >= m_iModeCount)
      m_iMode = 0;

   return m_iMode;

}

void canvas::on_paint(simple_graphics & g, LPCRECT lpcrect)
{
   int iMode = m_iMode;
#ifdef SUPORTA_TELA_AVANCADA
   static canvas_zero czero;
   #endif

   static vsstring s_strLastStatus;
   RECT rect = *lpcrect;
//   int cx = lpcrect->right - lpcrect->left;
//   int cy = lpcrect->bottom - lpcrect->top;
//   int iThankYouHeight = 30;

   simple_font font;

   font.create_point(100, "Lucida Sans Unicode", g);

   simple_font fontBold;

   font.create_point_bold(100, "Lucida Sans Unicode", 1, g);

   g.select(font);

   if(iMode == 0 || iMode == 1 || iMode == 2 || iMode == 3)
   {

      simple_pen pen;

      pen.create_solid(1, RGB(84, 84, 77));

      simple_brush brush;

      if(iMode == 0 || iMode == 1)
      {
         brush.from_stock(NULL_BRUSH);
      }
      else
      {
         brush.create_solid(RGB(84, 84, 84));
      }

      g.select(pen);

      g.select(brush);

      g.rectangle(lpcrect);

   }
   SIZE size = g.get_text_extent("CCpp");
//   int iLineCount = (rect.bottom - 30 - iThankYouHeight) / size.cy;
   if(iMode == 5) // if(m_bHealingSurface)
   {
#ifdef SUPORTA_TELA_AVANCADA
      int iCount = max(1, cx / 25);
      int jCount = max(1, cy / 25);

      for(int i = 0; i < iCount; i++)
      {
         for(int j = 0; j < jCount; j++)
         {
            //int iPhase =
            // 400
            //* (0.5 + (sin_dup(((double) ::get_tick_count() * 3.1415 * 2.0  * 0.084 / 1000.0) + ((double) (i) * 3.1415 * 2.0 / (double) (iCount))) / 2.0));
            //* (0.5 + (cos_dup(((double) ::get_tick_count() * 3.1415 * 2.0  * 0.084 / 1000.0) + ((double) (0) * 3.1415 * 2.0 / (double) (jCount))) / 2.0));
            double dPhase = fmod_dup((((double) ::get_tick_count() * 360  * 0.5984 / 1000.0) + (i * 360.0 / (double) iCount)  + (j * 360.0 / (double) jCount) + ((double) (sin_dup(((double) ::get_tick_count() * 3.1415 * 2.0  *0.0484 / 1000.0)+i * 3.1415 * 2.0 * 2.0 / (double) (iCount)) * sin_dup(((double) ::get_tick_count() * 3.1415 * 2.0  * 0.0484 / 1000.0)+j * 3.1415 * 2.0 * 2.0 / (double) (jCount)) * 360))), 360.0);
            int iR;
            int iG;
            int iB;
            double dRate = fmod_dup(dPhase, 60.0) / 60.0;
            int iColor = (int) (dRate * 155.0);
            if(dPhase < 60)
            {
               // purple to blue
               iR = 255 - iColor;
               iG = 100;
               iB = 255;
            }
            else if(dPhase < (120))
            {
               // blue to cyan
               iR = 100;
               iG = 100 + iColor;
               iB = 255;
            }
            else if(dPhase < (180))
            {
               // cyan to green
               iR = 100;
               iG = 255;
               iB = 255 - iColor;
            }
            else if(dPhase < (240))
            {
               // green to yellow
               iR = 100 + iColor;
               iG = 255;
               iB = 100;
            }
            else if(dPhase < (300))
            {
               // yellow to red
               iR = 255;
               iG = 255 - iColor;
               iB = 100;
            }
            else
            {
               // red to purple
               iR = 255;
               iG = 100;
               iB = 100 + iColor;
            }

            double z = ((int) (360 - (dPhase - 240 + ::get_tick_count() * 360  * 0.1984 / 1000.0))) % 360;
            if(dPhase == 240)
               z = 360;

            int iInflate = (int) (z * 33.0 / 360.0);

            rect.left = cx * i / iCount + 8 - iInflate;
            rect.right = cx * (i + 1) / iCount - 8 + iInflate;
            rect.top = cy * j / jCount + 8 - iInflate;
            rect.bottom = cy * (j + 1) / jCount - 8 + iInflate;

            // ca2 colors: green and yellow and black and white
            ::FillSolidRect_dup(hdc, &rect, RGB(iR, iG, iB));
         }
      }
#endif
   }
   else if(iMode == 2) // else // !m_bHealingSurface => "Surgery Internals"
   {

      g.set_text_color(RGB(0xCC, 0xCC, 0xCC));

      size_t iRefresh = 884;
      size_t iEat = 8;
      const char * psz = "development message so international english file \"C:\\ca2\\install.log\" excerpt  ::::::::";
      g.text_out(10, 10 + size.cy * 2, psz, (int) (strlen_dup(psz) - iEat + 1 + ((::get_tick_count() / (iRefresh - 277) % iEat))));
      DWORD dwRead;
//      int iLineMin = 5;
  //    int iLine = ((rect.bottom - 10) / size.cy) - 1;
      if(rect.bottom - rect.top >= size.cy)
      {
         HANDLE hfile = ::CreateFile(dir::ca2("install.log"), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
         if(hfile != INVALID_HANDLE_VALUE)
         {
            int iTell = ::SetFilePointer(hfile, 0, NULL, SEEK_END);
            iTell--;
            vsstring strLine;
            int iSkip = 0;
//            ::SetBkMode(hdc, TRANSPARENT);
            bool bNormal = false;
            bool bBold = false;
            bool bPreNormal = false;
            bool bStart = false;
            while(iTell > 0 && !bStart && !(bNormal && bBold))
            {
               ::SetFilePointer(hfile, iTell, NULL, SEEK_SET);
               char ch;
               if(!ReadFile(hfile, &ch,  1, &dwRead, NULL))
                  break;
               if(dwRead <= 0)
                  break;

               if(ch == '\r')
               {
                  iSkip++;
               }
               else if(ch == '\n')
               {
                  iSkip++;
               }
               else if(iSkip > 0)
               {
                  iSkip = 0;
                  strLine.trim();
                  if(strLine == "--")
                  {
                     bStart = true;
                  }
                  else if(strLine.begins_ci("***") && !bBold && strLine.length() > 0 && bNormal && bPreNormal)
                  {
                     bBold = true;
                     strLine = strLine.substr(3);
                     g.select(fontBold);
                     g.text_out(10, 10 + size.cy * 3, strLine);
                  }
                  else if(!strLine.begins_ci("***") && strLine.length() > 0 && !bNormal && !bBold && bPreNormal)
                  {
                     bNormal = true;
                     g.select(font);
                     g.text_out(10, 10 + size.cy * 4, strLine);
                  }
                  else if(strLine.length() > 0 && !bPreNormal && !bBold && !bNormal)
                  {
                     bPreNormal = true;
                     //::SelectObject(hdc, hfont);
                     //::TextOutU_dup(hdc, 10, 10 + size.cy * 4, strLine, strLine.length());
                  }
                  strLine = ch;
               }
               else
               {
                  strLine = ch + strLine;
               }
               iTell--;
            }
            ::CloseHandle(hfile);
         }
      }
   }
   else if(iMode == 3) // else // !m_bHealingSurface => "Surgery Internals"
   {

      g.set_text_color(RGB(0xCC, 0xCC, 0xCC));

      size_t iRefresh = 884;
      size_t iEat = 8;
      const char * psz = "development message so international english last lines of file \"C:\\ca2\\install.log\" ::::::::";
      g.text_out(10, 10 + size.cy * 2, psz, (int) (strlen_dup(psz) - iEat + 1 + ((::get_tick_count() / (iRefresh - 277) % iEat))));
      g.select(fontBold);
      g.text_out( 10, 10 + size.cy * 3, s_strLastStatus);
      DWORD dwRead;
      int iLineMin = 5;
      int iLine = ((rect.bottom - 10) / size.cy) - 1;
      if(rect.bottom - rect.top >= size.cy)
      {
         HANDLE hfile = ::CreateFileA(dir::ca2("install.log"), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
         if(hfile != INVALID_HANDLE_VALUE)
         {
            int iTell = ::SetFilePointer(hfile, 0, NULL, SEEK_END);
            iTell--;
            vsstring strLine;
            int iSkip = 0;
            while(iTell > 0 && iLine >= iLineMin)
            {
               ::SetFilePointer(hfile, iTell, NULL, SEEK_SET);
               char ch;
               if(!ReadFile(hfile, &ch,  1, &dwRead, NULL))
                  break;
               if(dwRead <= 0)
                  break;
               if(ch == '\r')
               {
                  if(iSkip >= 2)
                  {
                     iSkip = 0;
                     iLine--;
                  }
                  iSkip++;
               }
               else if(ch == '\n')
               {
                  if(iSkip >= 2)
                  {
                     iSkip = 0;
                     iLine--;
                  }
                  iSkip++;
               }
               else if(iSkip > 0)
               {
                  iSkip = 0;
                  if(strLine.begins_ci("***"))
                  {
                     strLine = strLine.substr(3);
                     if(strLine == s_strLastStatus)
                     {
                        goto skip_text_out1;
                     }
                     s_strLastStatus = strLine;
                     g.select(fontBold);
                  }
                  else
                  {
                     index iFind = strLine.find('\t');
                     if(iFind >= 0)
                     {
                        strLine = strLine.substr(iFind + 1);
                     }
                     g.select(font);
                  }
                  iLine--;
                  g.text_out(10, 10 + iLine * size.cy, strLine);
                  skip_text_out1:
                  strLine = ch;
               }
               else
               {
                  strLine = ch + strLine;
               }
               iTell--;
            }
            ::CloseHandle(hfile);
            if(iLine >= iLineMin && strLine.length() > 0)
            {
               iLine--;
               g.text_out(10, 10 + iLine * size.cy, strLine);
            }
         }
      }
   }
   else if(iMode == 4) // if(m_bHealingSurface)
   {
#ifdef SUPORT_TELA_AVANCADA
      czero.on_paint(g, lpcrect);
#endif
   }

}












   #ifdef SUPORTA_TELA_AVANCADA


canvas_zero::canvas_zero()
{
   m_rect.left = 0;
   m_rect.top = 0;
   m_rect.right = 0;
   m_rect.bottom = 0;
   m_dwCurZero = ::get_tick_count();
   m_dwNextZero = ::get_tick_count();
   m_iMaxSize = 33;
   m_iLast = 0;
}

void canvas_zero::prepare(HDC hdc, LPCRECT lpcrect)
{
   m_rect.left    = lpcrect->left;
   m_rect.top     = lpcrect->top;
   m_rect.right   = lpcrect->right;
   m_rect.bottom  = lpcrect->bottom;

   int cx = m_rect.right - m_rect.left;
   int cy = m_rect.bottom - m_rect.top;

   BITMAPINFO m_Info;

	m_Info.bmiHeader.biSize=sizeof (BITMAPINFOHEADER);
	m_Info.bmiHeader.biWidth=cx;
	m_Info.bmiHeader.biHeight=cy;
	m_Info.bmiHeader.biPlanes=1;
	m_Info.bmiHeader.biBitCount=32;
	m_Info.bmiHeader.biCompression=BI_RGB;
	m_Info.bmiHeader.biSizeImage=cx*cy*4;

   LPDWORD lpdata;

	m_hbm = CreateDIBSection ( NULL, &m_Info, DIB_RGB_COLORS, (void **)&m_pdata, NULL, NULL );
   m_hdc = ::CreateCompatibleDC(NULL);
   ::SelectObject(m_hdc, m_hbm);
   FillSolidRect_dup(m_hdc, lpcrect, RGB(0, 0, 0));

   /*int iMid = (m_rect.bottom + m_rect.top) / 2;

   HPEN hpen = ::CreatePen(PS_SOLID, 1, RGB(0x7c, 0x7c, 0x7c));

   ::SelectObject(m_hdc, hpen);

   MoveToEx(m_hdc, m_rect.left, iMid, NULL);
   LineTo(m_hdc, m_rect.right, iMid);
   ::DeleteObject(hpen);*/

}

void canvas_zero::on_paint(simple_graphics & gPaint, LPCRECT lpcrect)
{
   HDC hdc = m_hdc;

   if(m_rect.left != lpcrect->left
   || m_rect.top != lpcrect->top
   || m_rect.right != lpcrect->right
   || m_rect.bottom != lpcrect->bottom)
   {
      prepare(hdc, lpcrect);
   }

   int cx = m_rect.right - m_rect.left;
   int cy = m_rect.bottom - m_rect.top;


   int iTimeFactor = 100; // greater faster

   if(::get_tick_count() >= m_dwNextZero)
   {
      POINT pt;
      pt.x = (rand_dup() * cx / RAND_MAX) + lpcrect->left;
      pt.y = (rand_dup() * cy / RAND_MAX) + lpcrect->top;
      m_pta.insert_at(0, pt);
      m_ia.insert_at(0, ((rand_dup() * max(cx, cy) / 4 / RAND_MAX) + 23));
      m_dwCurZero = ::get_tick_count();
      m_dwNextZero = m_dwCurZero + m_ia[m_ia.get_count() - 1] * 1000 / iTimeFactor + 1984 + 1977;
      m_iLast = 0;
      int iSize = m_ia[0];
      int w = iSize * 2 + 1;
      int h = iSize * 2 + 1;
      BITMAPINFO m_Info;

      ZeroMemory(&m_Info, sizeof (BITMAPINFO));

	   m_Info.bmiHeader.biSize=sizeof (BITMAPINFOHEADER);
	   m_Info.bmiHeader.biWidth=w;
	   m_Info.bmiHeader.biHeight=-h;
	   m_Info.bmiHeader.biPlanes=1;
	   m_Info.bmiHeader.biBitCount=32;
	   m_Info.bmiHeader.biCompression=BI_RGB;
	   m_Info.bmiHeader.biSizeImage=w*h*4;

      LPDWORD lpdata;

	   m_hbmZero = CreateDIBSection ( NULL, &m_Info, DIB_RGB_COLORS, (void **)&m_pdataZero, NULL, NULL );
      m_hdcZero = ::CreateCompatibleDC(NULL);
      m_hbmZeroOld = (HBITMAP) ::SelectObject(m_hdcZero, m_hbmZero);
      FillSolidRect_dup(m_hdcZero, lpcrect, RGB(0, 0, 0));
   }





   int i = 1;
/*   for(; i < m_pta.size(); i++)
   {
      POINT pt = m_pta[i];
      int iSize = m_ia[i];
      zero(hdc, pt, iSize, iSize);
   }*/

         BLENDFUNCTION bf;
         bf.BlendOp     = AC_SRC_OVER;
         bf.BlendFlags  = 0;
         bf.SourceConstantAlpha = 255;
         bf.AlphaFormat = AC_SRC_ALPHA;

   bool bReady = false;
   POINT pt;
   int iSize;
   int iCurStep;
   if(m_pta.get_count() > 0)
   {
      pt = m_pta[0];
      iSize = m_ia[0];
      iCurStep = (::get_tick_count() - m_dwCurZero) * iTimeFactor / 1000;
      iCurStep = max(2, min(iCurStep, iSize));
      zero(hdc, pt, iSize, iCurStep);
      if(iCurStep == iSize)
      {
         if(m_iLast <= iCurStep)
         {
            ::AlphaBlend(m_hdc, pt.x - iSize / 2, pt.y - iSize / 2, iSize * 2 + 1, iSize * 2 + 1, m_hdcZero, 0, 0, iSize * 2 + 1, iSize * 2 + 1, bf);
         }
         bReady = true;
      }
      else
      {
      }
      m_iLast = iCurStep + 1;
   }
   else
   {
   }

   ::BitBlt(hdcPaint, 0, 0, cx, cy, m_hdc, 0, 0, SRCCOPY);

   if(!bReady)
   {
      ::AlphaBlend(hdcPaint, pt.x - iSize / 2, pt.y - iSize / 2, iSize * 2 + 1, iSize * 2 + 1, m_hdcZero, 0, 0, iSize * 2 + 1, iSize * 2 + 1, bf);
      //::AlphaBlend(hdcPaint, 0, 0, iSize, iSize, m_hdcZero, 0, 0, iSize, iSize, bf);
   }

}


void canvas_zero::zero(HDC hdc, POINT pt, int iSize, int iStep)
{
   int cx = m_rect.right - m_rect.left;
   int cy = m_rect.bottom - m_rect.top;

   int w = iSize * 2 + 1;
   int radius = iStep - 1;
   for(int j = -iSize; j < iSize; j++)
   {
      for(int k = -iSize; k < iSize; k++)
      {
         double dj = j;
         double dk = k;
         double distance = sqrt_dup((dj * dj + dk * dk));
         if(distance < radius)
         {
            double dLum = (double) (distance) / (double) radius;
            dLum = pow_dup(dLum, 0.2);
            int x = j + iSize;
            int y = k + iSize;
            int iAlpha = dLum * 255.0;
            if(iAlpha > 255)
               iAlpha = 255;
            if(iAlpha < 0)
               iAlpha = 0;
            int iLum = 255 - iAlpha;
            m_pdataZero[x + y * w] = RGB(iLum, iLum, iLum) | (iLum << 24);
         }
      }
   }

}

#endif


} // namespace spa
