#include "framework.h"








// default implementation for these methods (present only in Windows for Desktop Applications)


namespace draw2d
{

   HBITMAP bitmap::GetHBITMAP()
   {

      if (m_hbitmapGet != NULL)
         return m_hbitmapGet;

      m_hbitmapGet = _GetHBITMAP();

      return m_hbitmapGet;

   }

   void bitmap::ReleaseHBITMAP(HBITMAP hbitmap)
   {

      if (hbitmap == NULL)
         return;

      if (m_hbitmapGet == NULL)
         return;

      _ReleaseHBITMAP(hbitmap);

      m_hbitmapGet = NULL;

   }

   HBITMAP bitmap::_GetHBITMAP()
   {

      if (get_os_data() == NULL)
         return NULL;

      ::draw2d::dib_sp dib(allocer());

      dib->create(GetBitmapDimension());

      if (dib->area() <= 0)
         return NULL;

      ::draw2d::graphics_sp g(allocer());

      g->CreateCompatibleDC(NULL);

      g->SelectObject(this);

      dib->get_graphics()->BitBlt(null_point(), dib->m_size, g, null_point(), SRCCOPY);

      return CreateHBITMAP(dib->m_pcolorref, dib->m_iScan, dib->m_size.cx, dib->m_size.cy);

   }


   void bitmap::_ReleaseHBITMAP(HBITMAP hbitmap)
   {

      ::DeleteObject(hbitmap);

   }

} // namespace draw2d







namespace draw2d
{


   class wingdi_font_enum
   {
   public:

      HDC                     m_hdc;
      stringa &               m_stra;
      ::draw2d::font::csa &   m_csa;
      bool                    m_bRaster;
      bool                    m_bTrueType;
      bool                    m_bOther;
      wstring                 m_wstrTopicFaceName;
      int_array               m_iaCs;



      wingdi_font_enum(stringa & stra, ::draw2d::font::csa & csa, bool bRaster, bool bTrueType, bool bOther) :
         m_stra(stra),
         m_csa(csa),
         m_bRaster(bRaster),
         m_bTrueType(bTrueType),
         m_bOther(bOther)
      {

         m_iaCs.add(CHINESEBIG5_CHARSET);
         m_iaCs.add(GB2312_CHARSET);
         m_iaCs.add(SHIFTJIS_CHARSET);
         //m_iaCs.add(ANSI_CHARSET);
         m_iaCs.add(SYMBOL_CHARSET);
         //m_iaCs.add(OEM_CHARSET);
         //m_iaCs.add(DEFAULT_CHARSET);
         m_iaCs.add(HEBREW_CHARSET);
         m_iaCs.add(ARABIC_CHARSET);
         m_iaCs.add(GREEK_CHARSET);
         m_iaCs.add(TURKISH_CHARSET);
         m_iaCs.add(VIETNAMESE_CHARSET);
         m_iaCs.add(THAI_CHARSET);
         m_iaCs.add(EASTEUROPE_CHARSET);
         m_iaCs.add(RUSSIAN_CHARSET);
         m_iaCs.add(JOHAB_CHARSET);
         m_iaCs.add(HANGUL_CHARSET);
         m_iaCs.add(BALTIC_CHARSET);
         //m_iaCs.add(MAC_CHARSET);


         m_hdc = NULL;

         enumerate();

      }

      ~wingdi_font_enum()
      {

         ::DeleteDC(m_hdc);

      }

      void enumerate()
      {

         if (m_hdc == NULL)
         {

            m_hdc = ::CreateCompatibleDC(NULL);

         }

         ::EnumFontFamiliesW(m_hdc, (LPCWSTR)NULL, (FONTENUMPROCW)&wingdi_font_enum::callback, (LPARAM)this);

         wstring wstrSample;

         LOGFONTW lf;

         HFONT hfont = NULL;

         HFONT hfontOld = NULL;

         size s;

         int iMultiScript;

         size sSample;

         int64_t maxarea;

         ::draw2d::font::e_cs ecs;

         ::draw2d::font::e_cs ecsFound;

         for (index i = 0; i < m_stra.get_size(); i++)
         {

            if (m_csa[i] == ::draw2d::font::cs_ansi
               || m_csa[i] == ::draw2d::font::cs_default)

            {

               maxarea = 0;

               s.cx = 0;

               s.cy = 0;

               ecsFound = m_csa[i];

               iMultiScript = 0;

               for (index j = 0; j < m_iaCs.get_size(); j++)
               {

                  ZERO(lf);

                  lf.lfCharSet = m_iaCs[j];

                  ecs = ::draw2d::wingdi_get_cs(lf.lfCharSet);

                  wstring wstrSample = ::draw2d::font::get_sample_text(ecs);

                  if (wstrSample.get_length() > 0)
                  {

                     string strFont = m_stra[i];

                     hfont = wingdi_CreatePointFont(180, strFont, m_hdc, &lf);

                     if (hfont != NULL)
                     {

                        try
                        {

                           hfontOld = (HFONT) ::SelectObject(m_hdc, hfont);

                           if (::GetTextExtentPointW(m_hdc, wstrSample, wstrSample.get_length(), &sSample))
                           {

                              if (sSample.area() > maxarea)
                              {

                                 ecsFound = ecs;

                                 maxarea = sSample.area();

                                 s = sSample;

                                 iMultiScript++;

                              }

                           }

                        }
                        catch (...)
                        {

                        }

                     }

                  }

                  ::SelectObject(m_hdc, hfontOld);

                  ::DeleteObject(hfont);

               }

               if (iMultiScript == 1)
               {

                  m_csa[i] = ecsFound;

               }
               else if (iMultiScript > 1)
               {

                  string str = m_stra[i];

               restart_k:;

                  for (index k = i + 1; k < m_stra.get_size(); k++)
                  {
                     if (m_stra[k] == str)
                     {
                        m_stra.remove_at(k);
                        m_csa.remove_at(k);
                        goto restart_k;
                     }
                  }


               }

            }

         }

      }

      void enum_cs(const WCHAR * pwsz)
      {

         m_wstrTopicFaceName = pwsz;

         ::EnumFontFamiliesW(m_hdc, pwsz, (FONTENUMPROCW)&wingdi_font_enum::callback_cs, (LPARAM)this);

      }

      static BOOL CALLBACK callback(LPLOGFONTW lplf, LPNEWTEXTMETRICW lpntm, DWORD FontType, LPVOID p);
      static BOOL CALLBACK callback_cs(LPLOGFONTW lplf, LPNEWTEXTMETRICW lpntm, DWORD FontType, LPVOID p);

   };


   BOOL CALLBACK wingdi_font_enum::callback(LPLOGFONTW lplf, LPNEWTEXTMETRICW lpntm, DWORD dwFontType, LPVOID p)
   {

      wingdi_font_enum * penum = (wingdi_font_enum *)p;

      if (wcslen(lplf->lfFaceName) > 1 && lplf->lfFaceName[0] == '@')
      {

         //output_debug_string("jhenghei");

      }
      else if (dwFontType & RASTER_FONTTYPE)
      {

         if (penum->m_bRaster)
         {

            penum->enum_cs(lplf->lfFaceName);

         }

      }
      else if (dwFontType & TRUETYPE_FONTTYPE)
      {

         if (penum->m_bTrueType)
         {

            penum->enum_cs(lplf->lfFaceName);

         }

      }
      else
      {

         if (penum->m_bOther)
         {

            penum->enum_cs(lplf->lfFaceName);

         }

      }

      return TRUE;

   }

   BOOL CALLBACK wingdi_font_enum::callback_cs(LPLOGFONTW lplf, LPNEWTEXTMETRICW lpntm, DWORD dwFontType, LPVOID p)
   {

      wingdi_font_enum * penum = (wingdi_font_enum *)p;

      bool bAdd = false;

      if (dwFontType & RASTER_FONTTYPE)
      {

         if (penum->m_bRaster)
         {

            bAdd = true;

         }

      }
      else if (dwFontType & TRUETYPE_FONTTYPE)
      {

         if (penum->m_bTrueType)
         {

            bAdd = true;

         }

      }
      else
      {

         if (penum->m_bOther)
         {

            bAdd = true;

         }

      }

      if (bAdd)
      {

         string str(lplf->lfFaceName);

         ::draw2d::font::e_cs ecs = ::draw2d::wingdi_get_cs(lplf->lfCharSet);

         if (penum->m_wstrTopicFaceName.get_length() != wcslen(lplf->lfFaceName))
         {

            string strTopic(penum->m_wstrTopicFaceName);

            index iFindTopic = -1;

            while ((iFindTopic = penum->m_stra.find_first_ci(strTopic, iFindTopic + 1)) >= 0)
            {

               if (penum->m_csa[iFindTopic] == ecs)
               {

                  penum->m_stra.remove_at(iFindTopic);
                  penum->m_csa.remove_at(iFindTopic);

                  break;

               }

            }


         }

         index iFind = penum->m_stra.find_first_ci(str);


         if (iFind >= 0)
         {

            if (penum->m_csa[iFind] == ::draw2d::font::cs_ansi
               || penum->m_csa[iFind] == ::draw2d::font::cs_default)
            {

               penum->m_csa[iFind] = ecs;

               if (ecs != ::draw2d::font::cs_ansi && ecs != ::draw2d::font::cs_default)
               {

                  output_debug_string("test");

               }

            }
            else
            {


               while ((iFind = penum->m_stra.find_first_ci(str, iFind + 1)) >= 0)
               {

                  if (penum->m_csa[iFind] == ecs)
                  {

                     break;

                  }

               }



            }

         }

         if (iFind < 0)
         {

            penum->m_stra.add(lplf->lfFaceName);
            penum->m_csa.add(ecs);

         }


      }

      return TRUE;

   }


   CLASS_DECL_BASE void wingdi_enum_fonts(stringa & stra, ::draw2d::font::csa & csa, bool bRaster, bool bTrueType, bool bOther)
   {

      wingdi_font_enum fonts(stra, csa, bRaster, bTrueType, bOther);

      stra.quick_sort([&](index i1, index i2) {
         csa.swap(i1, i2);
      });

   }


   //CLASS_DECL_BASE void wingdi_enum_cs(string str, ::draw2d::font::csa & csa, bool bRaster, bool bTrueType, bool bOther)
   //{

   //   stringa stra;

   //   wingdi_font_enum fonts(stra, csa, bRaster, bTrueType, bOther);

   //   HDC hdc = ::CreateCompatibleDC(NULL);

   //   wstring wstr(str);

   //   ::EnumFontFamiliesW(hdc, (LPCWSTR)wstr, (FONTENUMPROCW)&wingdi_font_enum::callback, (LPARAM)&fonts);

   //   ::DeleteDC(hdc);

   //}



   font::e_cs wingdi_get_cs(int iCs)
   {

      if (iCs == CHINESEBIG5_CHARSET)
      {

         return font::cs_chinesebig5;

      }
      else if (iCs == GB2312_CHARSET)
      {

         return font::cs_gb2312;

      }
      else if (iCs == SHIFTJIS_CHARSET)
      {

         return font::cs_shiftjis;

      }
      else if (iCs == ANSI_CHARSET)
      {

         return font::cs_ansi;

      }
      else if (iCs == SYMBOL_CHARSET)
      {

         return font::cs_symbol;

      }
      else if (iCs == OEM_CHARSET)
      {

         return font::cs_default;

      }
      else if (iCs == DEFAULT_CHARSET)
      {

         return font::cs_default;

      }
      else if (iCs == HEBREW_CHARSET)
      {

         return font::cs_hebrew;

      }
      else if (iCs == ARABIC_CHARSET)
      {

         return font::cs_arabic;

      }
      else if (iCs == GREEK_CHARSET)
      {

         return font::cs_greek;

      }
      else if (iCs == TURKISH_CHARSET)
      {

         return font::cs_turkish;

      }
      else if (iCs == VIETNAMESE_CHARSET)
      {

         return font::cs_vietnamese;

      }
      else if (iCs == THAI_CHARSET)
      {

         return font::cs_thai;

      }
      else if (iCs == EASTEUROPE_CHARSET)
      {

         return font::cs_easteurope;

      }
      else if (iCs == RUSSIAN_CHARSET)
      {

         return font::cs_russian;

      }
      else if (iCs == JOHAB_CHARSET)
      {

         return font::cs_johab;

      }
      else if (iCs == HANGUL_CHARSET)
      {

         return font::cs_hangul;

      }
      else if (iCs == BALTIC_CHARSET)
      {

         return font::cs_baltic;

      }
      else if (iCs == MAC_CHARSET)
      {

         return font::cs_mac;

      }
      else
      {

         output_debug_string("OTHER CHAR SET");

      }

      return font::cs_default;

   }


} // namespace draw2d









CLASS_DECL_BASE HBITMAP CreateAlphaBitmapV5(::draw2d::dib * pdib)
{
   //   HDC hMemDC;
   DWORD dwWidth, dwHeight;
   BITMAPV5HEADER bi;
   // HBITMAP hOldBitmap;
   HBITMAP hBitmap;
   void *lpBits;
   HCURSOR hAlphaCursor = NULL;

   dwWidth = pdib->m_size.cx;  // width of the Bitmap V5 Dib bitmap
   dwHeight = pdib->m_size.cy;  // height of the Bitmap V5 Dib bitmap

   ZeroMemory(&bi, sizeof(BITMAPV5HEADER));
   bi.bV5Size = sizeof(BITMAPV5HEADER);
   bi.bV5Width = dwWidth;
   bi.bV5Height = -(LONG)dwHeight;
   bi.bV5Planes = 1;
   bi.bV5BitCount = 32;
   bi.bV5Compression = BI_BITFIELDS;
   // The following mask specification specifies a supported 32 BPP
   // alpha format for Windows XP.
   bi.bV5RedMask = 0x00FF0000;
   bi.bV5GreenMask = 0x0000FF00;
   bi.bV5BlueMask = 0x000000FF;
   bi.bV5AlphaMask = 0xFF000000;

   HDC hdc;
   hdc = GetDC(NULL);

   // Create the DIB section with an alpha channel.
   hBitmap = CreateDIBSection(hdc, (BITMAPINFO *)&bi, DIB_RGB_COLORS, (void **)&lpBits, NULL, (DWORD)0);

   //hMemDC = CreateCompatibleDC(hdc);
   ReleaseDC(NULL, hdc);

   // Draw something on the DIB section.
   //hOldBitmap = (HBITMAP)SelectObject(hMemDC,hBitmap);
   //PatBlt(hMemDC,0,0,dwWidth,dwHeight,WHITENESS);
   //SetTextColor(hMemDC,RGB(0,0,0));
   //SetBkMode(hMemDC,TRANSPARENT);
   //TextOut(hMemDC,0,9,"rgba",4);
   //SelectObject(hMemDC,hOldBitmap);
   //DeleteDC(hMemDC);

   // Set the alpha values for each pixel in the cursor so that
   // the complete cursor is semi-transparent.

   int iStrideDst = dwWidth * sizeof(COLORREF);

   ::draw2d::copy_colorref(pdib->m_size.cx, pdib->m_size.cy, (COLORREF *)lpBits, iStrideDst, pdib->m_pcolorref, pdib->m_iScan);

   return hBitmap;

}


HCURSOR CreateAlphaIcon(::draw2d::dib * pdib, bool bIcon, int xHotSpot, int yHotSpot)
{

   HBITMAP hBitmap = ::CreateAlphaBitmapV5(pdib);

   // Create an empty mask bitmap.
   HBITMAP hMonoBitmap = CreateBitmap(pdib->m_size.cx, pdib->m_size.cy, 1, 1, NULL);

   if (bIcon)
   {
      xHotSpot = 0;
      yHotSpot = 0;
   }

   ICONINFO ii;
   ii.fIcon = bIcon ? TRUE : FALSE;  // Change fIcon to TRUE to create an alpha icon
   ii.xHotspot = xHotSpot;
   ii.yHotspot = yHotSpot;
   ii.hbmMask = hMonoBitmap;
   ii.hbmColor = hBitmap;

   // Create the alpha cursor with the alpha DIB section.
   HICON hicon = CreateIconIndirect(&ii);

   DeleteObject(hBitmap);
   DeleteObject(hMonoBitmap);

   return hicon;
}


HCURSOR CreateAlphaCursor(::draw2d::dib * pdib, int xHotSpot, int yHotSpot)
{

   return (HCURSOR) ::CreateAlphaIcon(pdib, false, xHotSpot, yHotSpot);

}



// CreateHITMAP (adapted)
// Adapted from 
// Marius Bancila's Blog
// C++,.NET,Windows programming & others
// (CAmiloST's opinion [Sato's opinion] why 's and .NET?, isn't ++ enough or needed more .Net or exquisite type of coffe?!?!?! or snake or something else?!?!
// Marius Bancila
// Software Developer
// Microsoft MVP VisualC++
// Cofounder of Codexpert
// www.mariusbancila.ro
// http://mariusbancila.ro/blog/tag/wic/
// Display images as you type in C++
// Marius Bancila C++ 2011 - 08 - 04 1 comment

CLASS_DECL_BASE HBITMAP CreateHBITMAP(COLORREF * pdata, int stride, int cx, int cy)
{

   HBITMAP hbmp = NULL;

   BITMAPINFO bminfo;

   ZeroMemory(&bminfo, sizeof(bminfo));
   bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   bminfo.bmiHeader.biWidth = cx;
   bminfo.bmiHeader.biHeight = -((LONG)cy);
   bminfo.bmiHeader.biPlanes = 1;
   bminfo.bmiHeader.biBitCount = 32;
   bminfo.bmiHeader.biCompression = BI_RGB;

   COLORREF * pvImageBits = NULL;

   HDC hdcScreen = GetDC(NULL);

   hbmp = CreateDIBSection(hdcScreen, &bminfo, DIB_RGB_COLORS, (void **)&pvImageBits, NULL, 0);

   ReleaseDC(NULL, hdcScreen);

   if (hbmp == NULL)
      goto Return;

   int bmStride = cx * sizeof(COLORREF);

   if (stride == bmStride)
   {

      memcpy(pvImageBits, pdata, cy * stride);

   }
   else
   {

      byte * psrc = (byte *)pdata;

      byte * pdst = (byte *)pvImageBits;

      for (index i = 0; i < cy; i++)
      {

         memcpy(pvImageBits, pdata, bmStride);

      }

   }

Return:

   return hbmp;

}





