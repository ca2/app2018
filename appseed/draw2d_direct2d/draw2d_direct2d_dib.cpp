#include "framework.h"


extern CLASS_DECL_AURA spa(::draw2d::dib) * g_pdiba;


namespace draw2d_direct2d
{


   //   Creator : El Barto (ef00@luc.ac.be)
   //   Location : http://www.luc.ac.be/~ef00/ebgfx
   //   Date : 09-04-98
   //////////////////////////////////////////////////////////////////////



   dib::dib(::aura::application * papp) :
      ::object(papp),
      m_spbitmap(allocer()),
      m_spgraphics(allocer())
   {

      g_pdiba->add(this);

      m_pcolorref = NULL;
      m_bMapped = false;
      m_bTrans = false;

   }


   COLORREF * dib::get_data() const
   {

      map();

      return m_pcolorref;

   }


   ::draw2d::bitmap_sp dib::get_bitmap() const
   {

      return m_spbitmap;
   }

   ::draw2d::bitmap_sp dib::detach_bitmap()
   {
      return m_spbitmap.detach();
   }


   void    dib::construct(int cx, int cy)
   {
      m_pcolorref = NULL;
      m_size.cx = 0;
      m_size.cy = 0;
      m_iScan = 0;
      create(cx, cy);
   }

   dib::~dib()
   {
      //      Destroy ();
   }

   bool dib::create(class size size)
   {
      return create(size.cx, size.cy);
   }

   bool dib::create(int width, int height)
   {
      if (m_spbitmap.is_set()
            && m_spbitmap->get_os_data() != NULL
            && width == m_size.cx
            && height == m_size.cy)
         return TRUE;

      Destroy();

      if (width <= 0 || height <= 0)
         return FALSE;

      ZeroMemory(&m_info, sizeof(BITMAPINFO));

      m_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      m_info.bmiHeader.biWidth = width;
      m_info.bmiHeader.biHeight = -height;
      m_info.bmiHeader.biPlanes = 1;
      m_info.bmiHeader.biBitCount = 32;
      m_info.bmiHeader.biCompression = BI_RGB;
      m_info.bmiHeader.biSizeImage = width * height * 4;

      m_spbitmap.alloc(allocer());
      m_spbitmapMap.alloc(allocer());
      m_spgraphics.alloc(allocer());
      m_spgraphicsMap.alloc(allocer());

      if (m_spbitmap.m_p == NULL || m_spbitmapMap.is_null() || m_spgraphics.is_null() || m_spgraphicsMap.is_null())
      {

         m_size.cx = 0;

         m_size.cy = 0;

         m_iScan = 0;

         return false;

      }

      m_spgraphicsMap->CreateCompatibleDC(NULL);

      //if (!m_spbitmapMap->CreateDIBSection(m_spgraphicsMap, &m_info, DIB_RGB_COLORS, (void **)&m_pcolorref, &m_iScan, NULL, NULL))
      if (!m_spbitmapMap->CreateDIBSection(m_spgraphicsMap, width, height, DIB_RGB_COLORS, (void **)&m_pcolorref, &m_iScan, NULL, NULL))
      {

         m_size.cx = 0;

         m_size.cy = 0;

         m_iScan = 0;

         return false;

      }

      if (m_spbitmapMap->get_os_data() == NULL)
      {

         Destroy();

         return false;

      }

      //HRESULT hr = m_spbitmapMap->get_typed_os_data < ID2D1Bitmap1 >(::draw2d_direct2d::bitmap::data_bitmap1)->Unmap();

      m_size.cx = width;

      m_size.cy = height;

      m_iScan = width * sizeof(COLORREF);

      realize(NULL);

      if (!is_realized())
      {

         m_size.cx = 0;

         m_size.cy = 0;

         m_iScan = 0;

         return false;

      }


      m_bMapped = false;

      //m_spgraphics->m_pdib = this;


      //realize(m_spgraphicsMap);

      ((ID2D1DeviceContext *)m_spgraphics->get_os_data())->BeginDraw();

      m_hrEndDraw = S_OK;

      m_spgraphics->m_pdibDraw2dGraphics = this;

      return true;

   }

   bool dib::dc_select(bool bSelect)
   {
      /*      if(bSelect)
      {
      return m_spgraphics->SelectObject(m_spbitmap) != NULL;
      }
      else
      {
      return m_spgraphics->SelectObject(m_hbitmapOriginal) != NULL;
      }*/
      return true;
   }

   bool dib::create(::draw2d::graphics * pgraphics)
   {
      ::draw2d::bitmap * pbitmap = dynamic_cast<::draw2d_direct2d::graphics *>(pgraphics)->get_current_bitmap();
      if (pbitmap == NULL)
         return FALSE;
      class size size = pbitmap->get_size();
      if (!create(size.cx, size.cy))
      {
         return FALSE;
      }
      from(pgraphics);
      return TRUE;
   }

   bool dib::Destroy()
   {

      if (m_bMapped)
      {
         unmap();
      }

      m_spbitmap.release();

      m_spgraphics.release();

      m_size.cx = 0;

      m_size.cy = 0;

      m_iScan = 0;

      m_pcolorref = NULL;

      return TRUE;
   }

   bool dib::to(::draw2d::graphics * pgraphics, point pt, class size size, point ptSrc)
   {

      return pgraphics->BitBlt(pt.x, pt.y, size.cx, size.cy, get_graphics(), ptSrc.x, ptSrc.y, SRCCOPY) != FALSE;

      /*  return SetDIBitsToDevice(
      (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->get_handle1(),
      pt.x, pt.y,
      size.cx, size.cy,
      ptSrc.x, ptSrc.y, ptSrc.y, cy - ptSrc.y,
      m_pcolorref, &m_info, 0)
      != FALSE; */

   }

   bool dib::from(::draw2d::graphics * pgraphics)
   {
      bool bOk = false;
      ::draw2d::bitmap_sp bitmap(get_app());
      bitmap->CreateCompatibleBitmap(pgraphics, 1, 1);
      ::draw2d::bitmap * pbitmap = pgraphics->SelectObject(bitmap);
      if (pbitmap == NULL)
         return false;
      class size size = pbitmap->get_size();
      if (!create(size))
      {
         pgraphics->SelectObject(pbitmap);
         return false;
      }
      _throw(todo(get_app()));
      //bool bOk = GetDIBits(WIN_HDC(pgraphics), (HBITMAP) pbitmap->get_os_data(), 0, cy, m_pcolorref, &(m_info), DIB_RGB_COLORS) != FALSE;
      pgraphics->SelectObject(pbitmap);
      return bOk;
   }


   bool dib::from(point ptDest, ::draw2d::graphics * pgraphics, point pt, class size sz)
   {

      return m_spgraphics->BitBlt(ptDest.x, ptDest.y, sz.cx, sz.cy, pgraphics, pt.x, pt.y, SRCCOPY) != FALSE;

   }


   //   void dib::Fill(int A, int R, int G, int B)
   //   {
   //
   //      map();
   //
   //      COLORREF color = ARGB(A, B, G, R);
   //
   //      COLORREF * pcr;
   //
   //      for(int y = 0; y < cy; y++)
   //      {
   //
   //         pcr = (COLORREF *) &((byte *) m_pcolorref)[scan * y];
   //
   //         for(int x = 0; x < cx; x++)
   //         {
   //
   //            *pcr     = color;
   //
   //            pcr++;
   //
   //         }
   //
   //      }
   //
   //   }
   //
   //
   //   void dib::set_rgb(int R, int G, int B)
   //   {
   //
   //      map();
   //
   //      byte * pdata = (byte *) m_pcolorref;
   //
   //      byte * p;
   //
   //      for(int y = 0; y < cy; y++)
   //      {
   //
   //         p = &pdata[scan * y];
   //
   //         for(int x = 0; x < cx; x++)
   //         {
   //
   //            p[0]     = R;
   //
   //            p[1]     = G;
   //
   //            p[2]     = B;
   //
   //            p += 4;
   //
   //         }
   //
   //      }
   //
   //   }
   //
   //
   //   void dib::ToAlpha(int i)
   //   {
   //
   //      BYTE * dst = (BYTE *) m_pcolorref;
   //
   //      int size = cx * cy;
   //
   //      while(size--)
   //      {
   //         dst[3] = dst[i];
   //         dst += 4;
   //      }
   //
   //   }
   //
   //   void dib::from_alpha()
   //   {
   //
   //      BYTE * dst = (BYTE*) m_pcolorref;
   //
   //      int64_t size = cx * cy;
   //
   //      while ( size-- )
   //      {
   //         dst[0] = dst[3];
   //         dst[1] = dst[3];
   //         dst[2] = dst[3];
   //         dst+=4;
   //      }
   //
   //   }
   //
   //   //DIB = DIB * SRC_ALPHA
   //
   //   void dib::mult_alpha(::draw2d::dib * pdibWork, bool bPreserveAlpha)
   //   {
   //      ::draw2d::dib::mult_alpha(pdibWork, bPreserveAlpha);
   //      return ;
   //      /*
   //      if(area() <= 0)
   //      return;
   //
   //      //return ::draw2d::dib::mult_alpha(NULL, true);
   //      ::draw2d::dib_sp dibWork;
   //
   //      if(pdibWork == NULL)
   //      {
   //      dibWork.create(get_app());
   //      pdibWork = dibWork;
   //      }
   //
   //      if(pdibWork->create(cx, cy))
   //      return;
   //
   //      pdibWork->FillByte(0);
   //
   //      pdibWork->channel_from(visual::rgba::channel_alpha, this);
   //
   //      pdibWork->channel_invert(visual::rgba::channel_alpha);
   //
   //
   //      BLENDFUNCTION bf;
   //
   //      bf.BlendOp = AC_SRC_OVER;
   //      bf.BlendFlags = 0;
   //      bf.SourceConstantAlpha = 255;
   //      bf.AlphaFormat = AC_SRC_ALPHA;
   //
   //      get_graphics()->alpha_blend(size(), pdibWork->get_graphics(), bf);
   //
   //      if(bPreserveAlpha)
   //      {
   //
   //      pdibWork->channel_invert(visual::rgba::channel_alpha);
   //
   //      channel_from(visual::rgba::channel_alpha, pdibWork);
   //
   //      }
   //
   //      */
   //
   //   }
   //
   //   void dib::Map(int ToRgb, int FromRgb)
   //   {
   //
   //      BYTE * dst = (BYTE *) m_pcolorref;
   //
   //      int size = cx * cy;
   //
   //      while ( size-- )
   //      {
   //
   //         *dst = (byte) (*dst == FromRgb ? ToRgb : *dst);
   //
   //         dst += 4;
   //
   //      }
   //
   //   }
   //
   //
   //   void dib::ToAlphaAndFill(int i, COLORREF cr)
   //   {
   //
   //      BYTE * dst = (BYTE *) m_pcolorref;
   //
   //      int size = cx * cy;
   //
   //      BYTE uchB = rgba_get_b(cr);
   //      BYTE uchG = rgba_get_g(cr);
   //      BYTE uchR = rgba_get_r(cr);
   //
   //      while ( size-- )
   //      {
   //         dst[3] = dst[i];
   //         dst[0] = uchB;
   //         dst[1] = uchG;
   //         dst[2] = uchR;
   //         dst+=4;
   //      }
   //   }
   //
   //   void dib::GrayToARGB(COLORREF cr)
   //   {
   //
   //      BYTE * dst = (BYTE *) m_pcolorref;
   //
   //      int size = cx * cy;
   //
   //      DWORD dwB = rgba_get_b(cr);
   //      DWORD dwG = rgba_get_g(cr);
   //      DWORD dwR = rgba_get_r(cr);
   //
   //      while (size-- > 0)
   //      {
   //         dst[3] = dst[0];
   //         dst[0] = (BYTE)(((dwB * dst[3]) / 256) & 0xff);
   //         dst[1] = (BYTE)(((dwG * dst[3]) / 256) & 0xff);
   //         dst[2] = (BYTE)(((dwR * dst[3]) / 256) & 0xff);
   //         dst+=4;
   //      }
   //   }
   //
   //
   //   void dib::BitBlt(::draw2d::dib *pdib, int op)
   //   {
   //      if(op == 123) // zero dest RGB, invert alpha, and OR src RGB
   //      {
   //
   //         int isize = cx * cy;
   //
   //         LPDWORD lpbitsSrc = (LPDWORD) pdib->m_pcolorref;
   //         LPDWORD lpbitsDst = (LPDWORD) m_pcolorref;
   //
   //         COLORREF _colorref = RGB ( 0, 0, 0 ) | (255 << 24);
   //         COLORREF colorrefa[2];
   //
   //         colorrefa[0] = _colorref;
   //         colorrefa[1] = _colorref;
   //
   //         COLORREF _colorrefN = RGB ( 255, 255, 255) | (0 << 24);
   //         COLORREF colorrefaN[2];
   //
   //         colorrefaN[0] = _colorrefN;
   //         colorrefaN[1] = _colorrefN;
   //
   //#ifdef AMD64
   //
   //         //x64
   //#else
   //         _asm
   //         {
   //            emms
   //               mov      eax, isize
   //               mov      ebx, lpbitsDst
   //               mov      ecx, lpbitsSrc
   //               movq     mm0, colorrefa
   //               movq     mm7, colorrefaN
   //fill_loop:
   //            cmp      eax, 1
   //               jle      fill_last
   //               movq     mm1, [ebx]
   //            movq     mm2, [ecx]
   //            pandn    mm1, mm0
   //               pand     mm2, mm7
   //               por      mm1, mm2
   //               movq     [ebx], mm1
   //
   //               sub      eax, 2
   //               add      ebx, 8
   //               add      ecx, 8
   //
   //               jmp      fill_loop
   //
   //fill_last:
   //            emms
   //         }
   //#endif
   //      }
   //
   //   }
   //
   //
   //   void dib::Invert()
   //   {
   //
   //      int size = cx * cy;
   //
   //      LPBYTE lpb = (LPBYTE) m_pcolorref;
   //
   //      for ( int i=0; i<size; i++ )
   //      {
   //
   //         lpb[0] = 255 - lpb[0];
   //
   //         lpb[1] = 255 - lpb[1];
   //
   //         lpb[2] = 255 - lpb[2];
   //
   //         lpb += 4;
   //
   //      }
   //
   //   }
   //
   //   void dib::channel_invert(visual::rgba::echannel echannel)
   //   {
   //
   //      int64_t size   = area();
   //
   //      register int64_t size64 = size / 64;
   //
   //      LPBYTE lpb = (LPBYTE) m_pcolorref;
   //
   //      lpb += ((int)echannel) % 4;
   //
   //      register int64_t i = 0;
   //
   //      for(; i < size64; i++)
   //      {
   //         lpb[4 *  0] = 255 - lpb[4 *  0];
   //         lpb[4 *  1] = 255 - lpb[4 *  1];
   //         lpb[4 *  2] = 255 - lpb[4 *  2];
   //         lpb[4 *  3] = 255 - lpb[4 *  3];
   //         lpb[4 *  4] = 255 - lpb[4 *  4];
   //         lpb[4 *  5] = 255 - lpb[4 *  5];
   //         lpb[4 *  6] = 255 - lpb[4 *  6];
   //         lpb[4 *  7] = 255 - lpb[4 *  7];
   //         lpb[4 *  8] = 255 - lpb[4 *  8];
   //         lpb[4 *  9] = 255 - lpb[4 *  9];
   //         lpb[4 * 10] = 255 - lpb[4 * 10];
   //         lpb[4 * 11] = 255 - lpb[4 * 11];
   //         lpb[4 * 12] = 255 - lpb[4 * 12];
   //         lpb[4 * 13] = 255 - lpb[4 * 13];
   //         lpb[4 * 14] = 255 - lpb[4 * 14];
   //         lpb[4 * 15] = 255 - lpb[4 * 15];
   //         lpb[4 * 16] = 255 - lpb[4 * 16];
   //         lpb[4 * 17] = 255 - lpb[4 * 17];
   //         lpb[4 * 18] = 255 - lpb[4 * 18];
   //         lpb[4 * 19] = 255 - lpb[4 * 19];
   //         lpb[4 * 20] = 255 - lpb[4 * 20];
   //         lpb[4 * 21] = 255 - lpb[4 * 21];
   //         lpb[4 * 22] = 255 - lpb[4 * 22];
   //         lpb[4 * 23] = 255 - lpb[4 * 23];
   //         lpb[4 * 24] = 255 - lpb[4 * 24];
   //         lpb[4 * 25] = 255 - lpb[4 * 25];
   //         lpb[4 * 26] = 255 - lpb[4 * 26];
   //         lpb[4 * 27] = 255 - lpb[4 * 27];
   //         lpb[4 * 28] = 255 - lpb[4 * 28];
   //         lpb[4 * 29] = 255 - lpb[4 * 29];
   //         lpb[4 * 30] = 255 - lpb[4 * 30];
   //         lpb[4 * 31] = 255 - lpb[4 * 31];
   //
   //         lpb[4 * 32] = 255 - lpb[4 * 32];
   //         lpb[4 * 33] = 255 - lpb[4 * 33];
   //         lpb[4 * 34] = 255 - lpb[4 * 34];
   //         lpb[4 * 35] = 255 - lpb[4 * 35];
   //         lpb[4 * 36] = 255 - lpb[4 * 36];
   //         lpb[4 * 37] = 255 - lpb[4 * 37];
   //         lpb[4 * 38] = 255 - lpb[4 * 38];
   //         lpb[4 * 39] = 255 - lpb[4 * 39];
   //         lpb[4 * 40] = 255 - lpb[4 * 40];
   //         lpb[4 * 41] = 255 - lpb[4 * 41];
   //         lpb[4 * 42] = 255 - lpb[4 * 42];
   //         lpb[4 * 43] = 255 - lpb[4 * 43];
   //         lpb[4 * 44] = 255 - lpb[4 * 44];
   //         lpb[4 * 45] = 255 - lpb[4 * 45];
   //         lpb[4 * 46] = 255 - lpb[4 * 46];
   //         lpb[4 * 47] = 255 - lpb[4 * 47];
   //         lpb[4 * 48] = 255 - lpb[4 * 48];
   //         lpb[4 * 49] = 255 - lpb[4 * 49];
   //         lpb[4 * 50] = 255 - lpb[4 * 50];
   //         lpb[4 * 51] = 255 - lpb[4 * 51];
   //         lpb[4 * 52] = 255 - lpb[4 * 52];
   //         lpb[4 * 53] = 255 - lpb[4 * 53];
   //         lpb[4 * 54] = 255 - lpb[4 * 54];
   //         lpb[4 * 55] = 255 - lpb[4 * 55];
   //         lpb[4 * 56] = 255 - lpb[4 * 56];
   //         lpb[4 * 57] = 255 - lpb[4 * 57];
   //         lpb[4 * 58] = 255 - lpb[4 * 58];
   //         lpb[4 * 59] = 255 - lpb[4 * 59];
   //         lpb[4 * 60] = 255 - lpb[4 * 60];
   //         lpb[4 * 61] = 255 - lpb[4 * 61];
   //         lpb[4 * 62] = 255 - lpb[4 * 62];
   //         lpb[4 * 63] = 255 - lpb[4 * 63];
   //
   //         lpb += 4 * 64;
   //      }
   //      i *= 64;
   //      for(; i < size; i++ )
   //      {
   //         *lpb = 255 - *lpb;
   //         lpb += 4;
   //      }
   //   }
   //   void dib::channel_multiply(visual::rgba::echannel echannel, double dRate)
   //   {
   //      if(dRate < 0)
   //         return;
   //      map();
   //      register int64_t size = area();
   //      LPBYTE lpb = (LPBYTE) get_data();
   //      if(lpb == NULL)
   //         return;
   //      lpb += ((int)echannel) % 4;
   //      register int iDiv = 256 * 256;
   //      register int iMul = (int) (dRate * ((double) iDiv));
   //      register int iRes;
   //      for(register int64_t i = 0; i < size; i++)
   //      {
   //         iRes = *lpb * iMul / iDiv;
   //         *lpb = (byte) (iRes > 255 ? 255 : iRes);
   //         lpb += 4;
   //      }
   //   }
   //
   //   void dib::FillGlass ( int R, int G, int B, int A )
   //   {
   //
   //      BYTE * dst = (BYTE *) m_pcolorref;
   //
   //      int size = cx * cy;
   //
   //      while ( size-- )
   //      {
   //         dst[0]=(BYTE)(((B-dst[0])*A+(dst[0]<<8))>>8);
   //         dst[1]=(BYTE)(((G-dst[1])*A+(dst[1]<<8))>>8);
   //         dst[2]=(BYTE)(((R-dst[2])*A+(dst[2]<<8))>>8);
   //         dst+=4;
   //      }
   //
   //   }
   //
   //   void dib::FillStippledGlass ( int R, int G, int B )
   //   {
   //
   //      COLORREF color=RGB ( B, G, R );
   //
   //      int w = cx;
   //      int h = cy;
   //
   //      for ( int j=0; j<w; j++ )
   //      {
   //         for ( int i=0; i<h; i++ )
   //         {
   //            m_pcolorref[j*w+i]=((i+j)&0x1) ? m_pcolorref[j*w+i] : color;
   //         }
   //      }
   //   }
   //
   //
   //   void dib::Paste ( ::draw2d::dib * pdib )
   //   {
   //      // If DibSize Wrong Re-create dib
   //      if ( (cx!=pdib->cx) || (cy!=pdib->cy) )
   //         create ( pdib->cx, pdib->cy );
   //      if(pdib->m_pcolorref == NULL)
   //         return;
   //      map();
   //      if(m_pcolorref != NULL)
   //      {
   //         // do Paste
   //         memcpy ( m_pcolorref, pdib->m_pcolorref, cx*cy*4 );
   //      }
   //   }
   //
   //   bool dib::color_blend(COLORREF cr, BYTE bAlpha)
   //   {
   //
   //      BYTE *dst=(BYTE*)m_pcolorref;
   //      int size=cx*cy;
   //
   //      DWORD dwB = rgba_get_b(cr);
   //      DWORD dwG = rgba_get_g(cr);
   //      DWORD dwR = rgba_get_r(cr);
   //
   //      DWORD dwB_ = dwB << 8;
   //      DWORD dwG_ = dwG << 8;
   //      DWORD dwR_ = dwR << 8;
   //
   //      while ( size-- )
   //      {
   //         dst[0]=(BYTE)(((dst[0]-dwB)*bAlpha+dwB_)>>8);
   //         dst[1]=(BYTE)(((dst[1]-dwG)*bAlpha+dwG_)>>8);
   //         dst[2]=(BYTE)(((dst[2]-dwG)*bAlpha+dwR_)>>8);
   //         dst+=4;
   //      }
   //      return true;
   //   }
   //
   //
   //   void dib::Blend (::draw2d::dib * pdib, int A )
   //   {
   //      if ( size()!=pdib->size() )
   //         return;
   //
   //      BYTE *src=(BYTE*)pdib->m_pcolorref;
   //      BYTE *dst=(BYTE*)m_pcolorref;
   //      int size=cx*cy;
   //
   //      while ( size-- )
   //      {
   //         dst[0]=(BYTE)(((src[0]-dst[0])*A+(dst[0]<<8))>>8);
   //         dst[1]=(BYTE)(((src[1]-dst[1])*A+(dst[1]<<8))>>8);
   //         dst[2]=(BYTE)(((src[2]-dst[2])*A+(dst[2]<<8))>>8);
   //         dst+=4;
   //         src+=4;
   //      }
   //   }

   //bool dib::Blend(::draw2d::dib *pdib, ::draw2d::dib *pdibA, int A)
   //{

   //   if(size() != pdib->size() ||
   //      size() != pdibA->size())
   //      return false;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   BYTE *alf=(BYTE*)pdibA->m_pcolorref;
   //   int size=cx*cy;

   //   A = 2 - A;

   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(((src[0]-dst[0])*alf[A]+(dst[0]<<8))>>8);
   //      dst[1]=(BYTE)(((src[1]-dst[1])*alf[A]+(dst[1]<<8))>>8);
   //      dst[2]=(BYTE)(((src[2]-dst[2])*alf[A]+(dst[2]<<8))>>8);
   //      dst+=4;
   //      src+=4;
   //      alf+=4;
   //   }

   //   return true;
   //}

   //void dib::Darken (::draw2d::dib * pdib )
   //{
   //   if ( size()!=pdib->size() )
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=cx*cy;

   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)((src[0]<dst[0]) ? src[0] : dst[0]);
   //      dst[1]=(BYTE)((src[1]<dst[1]) ? src[1] : dst[1]);
   //      dst[2]=(BYTE)((src[2]<dst[2]) ? src[2] : dst[2]);
   //      dst+=4;
   //      src+=4;
   //   }
   //}

   //void dib::Difference (::draw2d::dib * pdib )
   //{
   //   if ( size()!=pdib->size() )
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=cx*cy;

   //   while ( size-- )
   //   {
   //      int Difference;
   //      Difference=src[0]-dst[0];
   //      dst[0]=(BYTE)((Difference<0) ? -Difference : Difference);
   //      Difference=src[1]-dst[1];
   //      dst[1]=(BYTE)((Difference<0) ? -Difference : Difference);
   //      Difference=src[2]-dst[2];
   //      dst[2]=(BYTE)((Difference<0) ? -Difference : Difference);
   //      dst+=4;
   //      src+=4;
   //   }
   //}

   //void dib::Lighten (::draw2d::dib * pdib )
   //{
   //   if ( size()!=pdib->size() )
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=cx*cy;

   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)((src[0]>dst[0]) ? src[0] : dst[0]);
   //      dst[1]=(BYTE)((src[1]>dst[1]) ? src[1] : dst[1]);
   //      dst[2]=(BYTE)((src[2]>dst[2]) ? src[2] : dst[2]);
   //      dst+=4;
   //      src+=4;
   //   }
   //}


   //void dib::Multiply (::draw2d::dib * pdib )
   //{
   //   if ( size()!=pdib->size() )
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=cx*cy;

   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(((src[0])*(dst[0]))>>8);
   //      dst[1]=(BYTE)(((src[1])*(dst[1]))>>8);
   //      dst[2]=(BYTE)(((src[2])*(dst[2]))>>8);
   //      dst+=4;
   //      src+=4;
   //   }
   //}

   //void dib::Screen (::draw2d::dib * pdib )
   //{
   //   if ( size()!=pdib->size() )
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=cx*cy;

   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(255-(((255-src[0])*(255-dst[0]))>>8));
   //      dst[1]=(BYTE)(255-(((255-src[1])*(255-dst[1]))>>8));
   //      dst[2]=(BYTE)(255-(((255-src[2])*(255-dst[2]))>>8));
   //      dst+=4;
   //      src+=4;
   //   }
   //}

   ////////////////////////////////////////////////////////////////////////
   //// rectangle Functions
   ////////////////////////////////////////////////////////////////////////

   //void dib::copy (::draw2d::dib * pdib, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<cx) ? pdib->cx : cx-x;
   //   int dy=((y+pdib->cy)<cy) ? pdib->cy : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to copy return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;
   //   // If DibSize Wrong Re-create dib
   //   if ( (dx!=pdib->cx) || (dy!=pdib->cy) )
   //      pdib->create ( dx, dy );

   //   // Prepare buffer Addresses
   //   COLORREF *src=m_pcolorref+(py*cx)+px;
   //   COLORREF *dst=pdib->m_pcolorref;

   //   // Do copy
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //         dst[i]=src[i];
   //      src+=cx;
   //      dst+=pdib->cx;
   //   }
   //}

   //void dib::PasteRect (::draw2d::dib * pdib, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<cx) ? pdib->cx : cx-x;
   //   int dy=((y+pdib->cy)<cy) ? pdib->cy : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Paste return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   COLORREF *src=pdib->m_pcolorref+((py-y)*pdib->cx)+px-x;
   //   COLORREF *dst=m_pcolorref+(py*cx)+px;

   //   // Do Paste
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //         dst[i]=src[i];
   //      src+=pdib->cx;
   //      dst+=cx;
   //   }
   //}

   //void dib::fill_rect ( int x, int y, int w, int h, int R, int G, int B )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+w)<cx) ? w : cx-x;
   //   int dy=((y+h)<cy) ? h : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Fill return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Address
   //   COLORREF *dst=m_pcolorref+(py*cx)+px;
   //   COLORREF color=RGB ( B, G, R );

   //   // Do Fill
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[i]=color;
   //      }
   //      dst+=cx;
   //   }
   //}

   //void dib::FillGlassRect ( int x, int y, int w, int h, int R, int G, int B, int A )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+w)<cx) ? w : cx-x;
   //   int dy=((y+h)<cy) ? h : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to FillGlass return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Address
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*cx)+px)*4;

   //   // Do FillGlass
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(((B-dst[0])*A+(dst[0]<<8))>>8);
   //         dst[1]=(BYTE)(((G-dst[1])*A+(dst[1]<<8))>>8);
   //         dst[2]=(BYTE)(((R-dst[2])*A+(dst[2]<<8))>>8);
   //         dst+=4;
   //      }
   //      dst+=(cx-dx)<<2;
   //   }
   //}

   //void dib::FillStippledGlassRect ( int x, int y, int w, int h, int R, int G, int B )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+w)<cx) ? w : cx-x;
   //   int dy=((y+h)<cy) ? h : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to FillStippledGlass return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Address
   //   COLORREF *dst=m_pcolorref+(py*cx)+px;
   //   COLORREF color=RGB ( B, G, R );

   //   // Do FillStippledGlass
   //   for ( int j=0; j<dy; j++ )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[i]=((i+j)&0x1) ? dst[i] : color;
   //      }
   //      dst+=cx;
   //   }
   //}

   //void dib::BlendRect (::draw2d::dib * pdib, int x, int y, int A )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<cx) ? pdib->cx : cx-x;
   //   int dy=((y+pdib->cy)<cy) ? pdib->cy : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Blend return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*cx)+px)*4;

   //   // Do Blend
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(((src[0]-dst[0])*A+(dst[0]<<8))>>8);
   //         dst[1]=(BYTE)(((src[1]-dst[1])*A+(dst[1]<<8))>>8);
   //         dst[2]=(BYTE)(((src[2]-dst[2])*A+(dst[2]<<8))>>8);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //void dib::DarkenRect (::draw2d::dib * pdib, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<cx) ? pdib->cx : cx-x;
   //   int dy=((y+pdib->cy)<cy) ? pdib->cy : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Darken return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*cx)+px)*4;

   //   // Do Darken
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)((src[0]<dst[0]) ? src[0] : dst[0]);
   //         dst[1]=(BYTE)((src[1]<dst[1]) ? src[1] : dst[1]);
   //         dst[2]=(BYTE)((src[2]<dst[2]) ? src[2] : dst[2]);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //void dib::DifferenceRect (::draw2d::dib * pdib, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<cx) ? pdib->cx : cx-x;
   //   int dy=((y+pdib->cy)<cy) ? pdib->cy : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Difference return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*cx)+px)*4;

   //   // Do Difference
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         int Difference;
   //         Difference=src[0]-dst[0];
   //         dst[0]=(BYTE)((Difference<0) ? -Difference : Difference);
   //         Difference=src[1]-dst[1];
   //         dst[1]=(BYTE)((Difference<0) ? -Difference : Difference);
   //         Difference=src[2]-dst[2];
   //         dst[2]=(BYTE)((Difference<0) ? -Difference : Difference);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //void dib::LightenRect (::draw2d::dib * pdib, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<cx) ? pdib->cx : cx-x;
   //   int dy=((y+pdib->cy)<cy) ? pdib->cy : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Lighten return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*cx)+px)*4;

   //   // Do Lighten
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)((src[0]>dst[0]) ? src[0] : dst[0]);
   //         dst[1]=(BYTE)((src[1]>dst[1]) ? src[1] : dst[1]);
   //         dst[2]=(BYTE)((src[2]>dst[2]) ? src[2] : dst[2]);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //void dib::MultiplyRect (::draw2d::dib * pdib, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<cx) ? pdib->cx : cx-x;
   //   int dy=((y+pdib->cy)<cy) ? pdib->cy : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Multiply return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*cx)+px)*4;

   //   // Do Multiply
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(((src[0])*(dst[0]))>>8);
   //         dst[1]=(BYTE)(((src[1])*(dst[1]))>>8);
   //         dst[2]=(BYTE)(((src[2])*(dst[2]))>>8);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //void dib::ScreenRect (::draw2d::dib * pdib, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<cx) ? pdib->cx : cx-x;
   //   int dy=((y+pdib->cy)<cy) ? pdib->cy : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Screen return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*cx)+px)*4;

   //   // Do Screen
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(255-(((255-src[0])*(255-dst[0]))>>8));
   //         dst[1]=(BYTE)(255-(((255-src[1])*(255-dst[1]))>>8));
   //         dst[2]=(BYTE)(255-(((255-src[2])*(255-dst[2]))>>8));
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //////////////////////////////////////////////////////////////////////
   // Line Functions
   //////////////////////////////////////////////////////////////////////

   /*void dib::Line ( int x1, int y1, int x2, int y2, int R, int G, int B )
   {
   int dx, dy, k1, k2, d, x, y;
   COLORREF color=RGB ( B, G, R );

   dx=x2-x1;
   dy=y2-y1;
   d=(dy<<1)-dx;
   k1=dy<<1;
   k2=(dy-dx)<<1;
   x=x1;
   y=y1;

   m_pcolorref[y*cx+x]=color;
   while (x<dx)
   {
   if (d<=0)
   {
   d+=k1;
   x++;
   }
   else
   {
   d+=k2;
   x++;
   y++;
   }
   m_pcolorref[y*cx+x]=color;
   }
   }*/

   //void dib::Line ( int x1, int y1, int x2, int y2, int R, int G, int B )
   //{
   //   int d, x, y, ax, ay, sx, sy, dx, dy;
   //   COLORREF color=RGB ( B, G, R );

   //   dx=x2-x1;
   //   ax=abs ( dx )<<1;
   //   sx=(dx<0) ? -1 : 1;
   //   dy=y2-y1;
   //   ay=abs ( dy )<<1;
   //   sy=(dy<0) ? -1 : 1;
   //   x=x1;
   //   y=y1;

   //   if ( ax>ay )
   //   {
   //      d=ay-(ax>>1);
   //      while ( x!=x2 )
   //      {
   //         m_pcolorref[y*cx+x]=color;
   //         if ( d>=0 )
   //         {
   //            y+=sy;
   //            d-=ax;
   //         }
   //         x+=sx;
   //         d+=ay;
   //      }
   //   }
   //   else
   //   {
   //      d=ax-(ay>>1);
   //      while ( y!=y2 )
   //      {
   //         m_pcolorref[y*cx+x]=color;
   //         if ( d>=0 )
   //         {
   //            x+=sx;
   //            d-=ay;
   //         }
   //         y+=sy;
   //         d+=ax;
   //      }
   //   }
   //}

   //void dib::LineGlass ( int x1, int y1, int x2, int y2, int R, int G, int B, int A )
   //{
   //   int d, x, y, ax, ay, sx, sy, dx, dy;
   //   //      COLORREF color=RGB ( B, G, R );
   //   BYTE *dst=(BYTE *)m_pcolorref;

   //   dx=x2-x1;
   //   ax=abs ( dx )<<1;
   //   sx=(dx<0) ? -1 : 1;
   //   dy=y2-y1;
   //   ay=abs ( dy )<<1;
   //   sy=(dy<0) ? -1 : 1;
   //   x=x1;
   //   y=y1;

   //   if ( ax>ay )
   //   {
   //      d=ay-(ax>>1);
   //      while ( x!=x2 )
   //      {
   //         dst[(y*cx+x)<<2]=(BYTE)(((B-dst[(y*cx+x)<<2])*A+(dst[(y*cx+x)<<2]<<8))>>8);
   //         dst[((y*cx+x)<<2)+1]=(BYTE)(((G-dst[((y*cx+x)<<2)+1])*A+(dst[((y*cx+x)<<2)+1]<<8))>>8);
   //         dst[((y*cx+x)<<2)+2]=(BYTE)(((R-dst[((y*cx+x)<<2)+2])*A+(dst[((y*cx+x)<<2)+2]<<8))>>8);
   //         if ( d>=0 )
   //         {
   //            y+=sy;
   //            d-=ax;
   //         }
   //         x+=sx;
   //         d+=ay;
   //      }
   //   }
   //   else
   //   {
   //      d=ax-(ay>>1);
   //      while ( y!=y2 )
   //      {
   //         dst[(y*cx+x)<<2]=(BYTE)(((B-dst[(y*cx+x)<<2])*A+(dst[(y*cx+x)<<2]<<8))>>8);
   //         dst[((y*cx+x)<<2)+1]=(BYTE)(((G-dst[((y*cx+x)<<2)+1])*A+(dst[((y*cx+x)<<2)+1]<<8))>>8);
   //         dst[((y*cx+x)<<2)+2]=(BYTE)(((R-dst[((y*cx+x)<<2)+2])*A+(dst[((y*cx+x)<<2)+2]<<8))>>8);
   //         if ( d>=0 )
   //         {
   //            x+=sx;
   //            d-=ay;
   //         }
   //         y+=sy;
   //         d+=ax;
   //      }
   //   }
   //}

   //void dib::Mask(COLORREF crMask, COLORREF crInMask, COLORREF crOutMask)
   //{
   //   COLORREF crFind = RGB(rgba_get_b(crMask), rgba_get_g(crMask), rgba_get_r(crMask));
   //   COLORREF crSet = RGB(rgba_get_b(crInMask), rgba_get_g(crInMask), rgba_get_r(crInMask));
   //   COLORREF crUnset  = RGB(rgba_get_b(crOutMask), rgba_get_g(crOutMask), rgba_get_r(crOutMask));

   //   int size=cx*cy;

   //   for ( int i=0; i<size; i++ )
   //      if(m_pcolorref[i]== crFind)
   //         m_pcolorref[i] = crSet;
   //      else
   //         m_pcolorref[i] = crUnset;

   //}

   //void dib::transparent_color(color color)
   //{
   //   COLORREF crFind = color.get_rgb();
   //   int64_t size = area();

   //   for ( int i=0; i<size; i++ )
   //      if((m_pcolorref[i] & 0x00ffffff) == crFind)
   //         ((LPBYTE)&m_pcolorref[i])[3] = 255;
   //      else
   //         ((LPBYTE)&m_pcolorref[i])[3] = 0;
   //}

   //void dib::channel_mask(unsigned char uchFind, unsigned char uchSet, unsigned char uchUnset, visual::rgba::echannel echannel)
   //{
   //   int size = cx * cy;
   //   unsigned char * puch = (unsigned char * ) m_pcolorref;
   //   puch += ((int) echannel) % 4;

   //   for(int i = 0; i < size; i++)
   //   {
   //      if(*puch == uchFind)
   //         *puch = uchSet;
   //      else
   //         *puch = uchUnset;
   //      puch += 4;
   //   }
   //}

   //uint32_t dib::GetPixel(int x, int y)
   //{
   //   DWORD dw = *(m_pcolorref + x + (cy - y - 1) * cx);
   //   return RGB(rgba_get_b(dw), rgba_get_g(dw), rgba_get_r(dw));
   //}

   //// too slow for animation on AMD XP gen_hon.
   //// TOP SUGGESTION:
   //// The gradient can�t have more then 256 levels of the most bright color
   //// (white). So creating a radial fill of radius 256 and then using fasting
   //// stretching algorithms is much faster than calculating radial fill.
   //void dib::RadialFill(BYTE alpha, BYTE red, BYTE green, BYTE blue, int xCenter, int yCenter, int iRadius)
   //{
   //   if (iRadius == 0)
   //      return;
   //   /*if(version == 0)
   //   {

   //   int iR = iRadius - 1;

   //   int xL = xCenter - iR;
   //   int xU = xCenter + iR;
   //   int yL = yCenter - iR;
   //   int yU = yCenter + iR;


   //   if(xL < 0) xL = 0;
   //   if(xU >= cx) xU = cx - 1;
   //   if(yL < 0) yL = 0;
   //   if(yU >= cy) yU = cy - 1;


   //   BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * cx));
   //   DWORD dwAdd = ((cx - 1 - xU) + xL) * 4;
   //   int size=cx*cy;
   //   double iLevel;

   //   int dx, dy;
   //   int dx0, dy0;
   //   int dx1, dy1;
   //   int dx2, dy2;
   //   int dx3, dy3;
   //   int dx4, dy4;
   //   int dx5, dy5;
   //   int dx6, dy6;
   //   int dx7, dy7;
   //   int dx8, dy8;
   //   int dx9, dy9;
   //   int dxA, dyA;
   //   int dxB, dyB;
   //   int dxC, dyC;
   //   int dxD, dyD;
   //   int dxE, dyE;
   //   int dxF, dyF;

   //   unsigned long dr;
   //   unsigned long dq;
   //   unsigned long dr0, dq0;
   //   unsigned long dr1, dq1;
   //   unsigned long dr2, dq2;
   //   unsigned long dr3, dq3;
   //   unsigned long dr4, dq4;
   //   unsigned long dr5, dq5;
   //   unsigned long dr6, dq6;
   //   unsigned long dr7, dq7;
   //   unsigned long dr8, dq8;
   //   unsigned long dr9, dq9;
   //   unsigned long drA, dqA;
   //   unsigned long drB, dqB;
   //   unsigned long drC, dqC;
   //   unsigned long drD, dqD;
   //   unsigned long drE, dqE;
   //   unsigned long drF, dqF;
   //   int x, y;

   //   {
   //   for(y = yL; y <= yU; y++)
   //   {
   //   for(x = xL; x <= xU; x++)
   //   {
   //   dx = abs(x - xCenter);
   //   dy = abs(y - yCenter);
   //   isqrt((dx * dx) + (dy * dy), &dr, &dq);
   //   if(dr < iRadius)
   //   {
   //   iLevel = 1.0 - dr * 1.0 / iRadius;
   //   dst[0] = blue  * iLevel;
   //   dst[1] = green * iLevel;
   //   dst[2] = red   * iLevel;
   //   }
   //   dst += 4;
   //   }
   //   dst += dwAdd;
   //   }
   //   }
   //   }
   //   else if(version == 1)*/
   //   {

   //      LPBYTE lpbAlloc = (LPBYTE) malloc((iRadius * iRadius) + 4);
   //      LPBYTE lpb = (LPBYTE) (((int) lpbAlloc + 3) & ~3);


   //      int x, y;
   //      int b;

   //      //         int r2 = iRadius * iRadius;

   //      for(y = 0; y < iRadius; y++)
   //      {
   //         for(x = y; x < iRadius; x++)
   //         {
   //            b = (int) (sqrt((double) (x * x) + (y * y)) * 255 / iRadius);
   //            if(b > 255)
   //               b = 0;
   //            else
   //               b = 255 - b;


   //            lpb[x + y * iRadius] = (byte) b;
   //            lpb[y + x * iRadius] = (byte) b;
   //         }
   //      }


   //      int iR = iRadius - 1;

   //      int xL = xCenter - iR;
   //      int xU = xCenter + iR;
   //      int yL = yCenter - iR;
   //      int yU = yCenter + iR;


   //      if(xL < 0) xL = 0;
   //      if(xU >= cx) xU = cx - 1;
   //      if(yL < 0) yL = 0;
   //      if(yU >= cy) yU = cy - 1;


   //      BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * cx));
   //      DWORD dwAdd = ((cx - 1 - xU) + xL) * 4;
   //      //         int size=cx*cy;

   //      int dx, dy;

   //      // Top Left

   //      for(y = yL; y <= yU; y++)
   //      {
   //         for(x = xL; x <= xU; x++)
   //         {
   //            dx = abs(x - xCenter);
   //            dy = abs(y - yCenter);
   //            b = lpb[dx + dy * iRadius];
   //            dst[0] = (byte) (blue     * b / 255);
   //            dst[1] = (byte) (green    * b / 255);
   //            dst[2] = (byte) (red      * b / 255);
   //            dst[3] = (byte) (alpha    * b / 255);
   //            dst += 4;
   //         }
   //         dst += dwAdd;
   //      }

   //      free(lpbAlloc);
   //   }
   //}


   //void dib::RadialFill(
   //   BYTE alpha1, BYTE red1, BYTE green1, BYTE blue1,
   //   BYTE alpha2, BYTE red2, BYTE green2, BYTE blue2,
   //   int xCenter, int yCenter, int iRadius)
   //{
   //   if (iRadius == 0)
   //      return;
   //   /*if(version == 0)
   //   {

   //   int iR = iRadius - 1;

   //   int xL = xCenter - iR;
   //   int xU = xCenter + iR;
   //   int yL = yCenter - iR;
   //   int yU = yCenter + iR;


   //   if(xL < 0) xL = 0;
   //   if(xU >= cx) xU = cx - 1;
   //   if(yL < 0) yL = 0;
   //   if(yU >= cy) yU = cy - 1;


   //   BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * cx));
   //   DWORD dwAdd = ((cx - 1 - xU) + xL) * 4;
   //   int size=cx*cy;
   //   double iLevel;

   //   int dx, dy;
   //   int dx0, dy0;
   //   int dx1, dy1;
   //   int dx2, dy2;
   //   int dx3, dy3;
   //   int dx4, dy4;
   //   int dx5, dy5;
   //   int dx6, dy6;
   //   int dx7, dy7;
   //   int dx8, dy8;
   //   int dx9, dy9;
   //   int dxA, dyA;
   //   int dxB, dyB;
   //   int dxC, dyC;
   //   int dxD, dyD;
   //   int dxE, dyE;
   //   int dxF, dyF;

   //   unsigned long dr;
   //   unsigned long dq;
   //   unsigned long dr0, dq0;
   //   unsigned long dr1, dq1;
   //   unsigned long dr2, dq2;
   //   unsigned long dr3, dq3;
   //   unsigned long dr4, dq4;
   //   unsigned long dr5, dq5;
   //   unsigned long dr6, dq6;
   //   unsigned long dr7, dq7;
   //   unsigned long dr8, dq8;
   //   unsigned long dr9, dq9;
   //   unsigned long drA, dqA;
   //   unsigned long drB, dqB;
   //   unsigned long drC, dqC;
   //   unsigned long drD, dqD;
   //   unsigned long drE, dqE;
   //   unsigned long drF, dqF;
   //   int x, y;

   //   {
   //   for(y = yL; y <= yU; y++)
   //   {
   //   for(x = xL; x <= xU; x++)
   //   {
   //   dx = abs(x - xCenter);
   //   dy = abs(y - yCenter);
   //   isqrt((dx * dx) + (dy * dy), &dr, &dq);
   //   if(dr < iRadius)
   //   {
   //   iLevel = 1.0 - dr * 1.0 / iRadius;
   //   dst[0] = blue  * iLevel;
   //   dst[1] = green * iLevel;
   //   dst[2] = red   * iLevel;
   //   }
   //   dst += 4;
   //   }
   //   dst += dwAdd;
   //   }
   //   }
   //   }
   //   else if(version == 1)*/
   //   {

   //      LPBYTE lpbAlloc = (LPBYTE) malloc((iRadius * iRadius) + 4);
   //      LPBYTE lpb = (LPBYTE) (((int) lpbAlloc + 3) & ~3);


   //      int x, y;
   //      int b;

   //      //         int r2 = iRadius * iRadius;

   //      for(y = 0; y < iRadius; y++)
   //      {
   //         for(x = y; x < iRadius; x++)
   //         {
   //            b = (int) (sqrt((double) (x * x) + (y * y)) * 255 / iRadius);
   //            if(b > 255)
   //               b = 0;
   //            else
   //               b = ~b;


   //            lpb[x + y * iRadius] = (byte) b;
   //            lpb[y + x * iRadius] = (byte) b;
   //         }
   //      }


   //      int iR = iRadius - 1;

   //      int xL = xCenter - iR;
   //      int xU = xCenter + iR;
   //      int yL = yCenter - iR;
   //      int yU = yCenter + iR;


   //      if(xL < 0) xL = 0;
   //      if(xU >= cx) xU = cx - 1;
   //      if(yL < 0) yL = 0;
   //      if(yU >= cy) yU = cy - 1;


   //      BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * cx));
   //      DWORD dwAdd = ((cx - 1 - xU) + xL) * 4;
   //      //         int size=cx*cy;

   //      int dx, dy;

   //      BYTE bComp;

   //      // Top Left

   //      for(y = yL; y <= yU; y++)
   //      {
   //         for(x = xL; x <= xU; x++)
   //         {
   //            dx = abs(x - xCenter);
   //            dy = abs(y - yCenter);
   //            b = lpb[dx + dy * iRadius];
   //            bComp = (byte) ~b;
   //            dst[0] = (byte) (((blue1  * b) + (blue2  * bComp)) / 255);
   //            dst[1] = (byte) (((green1 * b) + (green2 * bComp)) / 255);
   //            dst[2] = (byte) (((red1   * b) + (red2   * bComp)) / 255);
   //            dst[3] = (byte) (((alpha1 * b) + (alpha2 * bComp)) / 255);
   //            dst += 4;
   //         }
   //         dst += dwAdd;
   //      }

   //      free(lpbAlloc);
   //   }
   //}

   void dib::SetIconMask(::visual::icon * picon, int cx, int cy)
   {
      create(cx, cy);

      if (cx <= 0 || cy <= 0)
         return;




      // White blend dib
      dib dib1(get_app());
      dib1.create(cx, cy);
      dib1.Fill(0, 255, 255, 255);

#ifdef WINDOWSEX

      dib1.m_spgraphics->DrawIcon(
      0, 0,
      picon,
      cx, cy,
      0,
      NULL,
      DI_IMAGE | DI_MASK);

#else

      _throw(todo(get_app()));

#endif

      // Black blend dib
      ::draw2d::dib_sp spdib2(allocer());
      spdib2->create(cx, cy);
      spdib2->Fill(0, 0, 0, 0);

#ifdef WINDOWSEX
      spdib2->get_graphics()->DrawIcon(
      0, 0,
      picon,
      cx, cy,
      0,
      NULL,
      DI_IMAGE | DI_MASK);
#else
      spdib2->get_graphics()->DrawIcon(
      0, 0,
      picon,
      cx, cy,
      0,
      NULL,
      0);
#endif

      // Mask dib
      dib dibM(get_app());
      dibM.create(cx, cy);

      dibM.m_spgraphics->DrawIcon(
      0, 0,
      picon,
      cx, cy,
      0,
      NULL,
      0);
      //         DI_MASK);

      BYTE * r1 = (BYTE*)dib1.m_pcolorref;
      BYTE * r2 = (BYTE*)spdib2->get_data();
      BYTE * srcM = (BYTE*)dibM.m_pcolorref;
      BYTE * dest = (BYTE*)m_pcolorref;
      int iSize = cx*cy;

      BYTE b;
      BYTE bMax;
      while (iSize-- > 0)
      {
         if (srcM[0] == 255)
         {
            bMax = 0;
         }
         else
         {
            bMax = 0;
            b = (BYTE)(r1[0] - r2[0]);
            bMax = MAX(b, bMax);
            b = (BYTE)(r1[1] - r2[1]);
            bMax = MAX(b, bMax);
            b = (BYTE)(r1[2] - r2[2]);
            bMax = MAX(b, bMax);
            bMax = 255 - bMax;
         }
         dest[0] = bMax;
         dest[1] = bMax;
         dest[2] = bMax;
         dest += 4;
         srcM += 4;
         r1 += 4;
         r2 += 4;
      }



   }

   //void dib::rotate(::draw2d::dib * pdib, double dAngle, double dScale)
   //{
   //   // ::draw2d::dib_sp spdib(get_app());
   //   //   spdib->Paste(this);

   //   /*      int cx = cx;
   //   int cy = cy; */

   //   int l = MAX(cx, cy);


   //   int jmax = MIN(l, cy / 2);
   //   int jmin = - jmax;
   //   int imax = MIN(l, cx / 2);
   //   int imin = - imax;


   //   int joff = cy / 2;
   //   int ioff = cx / 2;

   //   //int iAngle = iStep % 360;
   //   //int iAngle = iStep;
   //   //int iAngle = 1;
   //   //int k = 0;

   //   /*     for ( int j=jmin; j<jmax; j++ )
   //   {
   //   for ( int i=imin; i<imax; i++ )
   //   {
   //   int x, y;

   //   // A Combination of a 2d Translation/rotation/Scale Matrix
   //   x=int(cos10(i, iAngle) - sin10(j, iAngle)) + ioff;
   //   y=int(sin10(i, iAngle) + cos10(j, iAngle)) + joff;
   //   m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //   spdib->m_pcolorref[abs(y%cy)*cx+abs(x%cx)];
   //   //k++;
   //   }
   //   (j+joff)*cx+(i+ioff)
   //   }*/

   //   int k = 0;
   //   double dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   double dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   int cx1 = cx - 1;
   //   int cy1 = cy - 1;
   //   for ( int j=jmin; j<jmax; j++ )
   //   {
   //      for ( int i=imin; i<imax; i++ )
   //      {
   //         int x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((int(dCos * i - dSin * j) + ioff) % cx);
   //         //y=abs((int(dSin * i + dCos * j) + joff) % cy);

   //         x = (int) abs((dCos * i - dSin * j) + ioff);
   //         y = (int) abs((dSin * i + dCos * j) + joff);

   //         if((x / cx) % 2 == 0)
   //         {
   //            x %= cx;
   //         }
   //         else
   //         {
   //            x = cx1 - (x % cx);
   //         }

   //         if((y / cy) % 2 == 0)
   //         {
   //            y %= cy;
   //         }
   //         else
   //         {
   //            y = cy1 - (y % cy);
   //         }



   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            pdib->m_pcolorref[y * cx + x];
   //         k++;
   //      }
   //   }
   //}


   //void dib::Rotate034(::draw2d::dib * pdib, double dAngle, double dScale)
   //{

   //   /*      int cx = cx;
   //   int cy = cy;*/

   //   int l = MAX(cx, cy);


   //   int jmax = MIN(l, cy / 2);
   //   int jmin = - jmax;
   //   int imax = MIN(l, cx / 2);
   //   int imin = - imax;


   //   if((cy % 2) == 1)
   //      jmax++;

   //   if((cx % 2) == 1)
   //      imax++;

   //   int joff = cy / 2;
   //   int ioff = cx / 2;


   //   int k = 0;
   //   double dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   double dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   int cx1 = cx - 1;
   //   int cy1 = cy - 1;
   //   for ( int j=jmin; j<jmax; j++ )
   //   {
   //      for ( int i=imin; i<imax; i++ )
   //      {
   //         int x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((int(dCos * i - dSin * j) + ioff) % cx);
   //         //y=abs((int(dSin * i + dCos * j) + joff) % cy);

   //         x = (int) abs((dCos * i - dSin * j) + ioff);
   //         y = (int) abs((dSin * i + dCos * j) + joff);

   //         if((x / cx) % 2 == 0)
   //         {
   //            x %= cx;
   //         }
   //         else
   //         {
   //            x = cx1 - (x % cx);
   //         }

   //         if((y / cy) % 2 == 0)
   //         {
   //            y %= cy;
   //         }
   //         else
   //         {
   //            y = cy1 - (y % cy);
   //         }



   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            pdib->m_pcolorref[y * cx + x];
   //         k++;
   //      }
   //   }
   //}

   //void dib::rotate(::draw2d::dib * pdib, LPCRECT lpcrect, double dAngle, double dScale)
   //{
   //   // ::draw2d::dib_sp spdib(get_app());
   //   //   spdib->Paste(this);



   //   rect rect(lpcrect);

   //   int cx = rect.width();
   //   int cy = rect.height();

   //   int l = MAX(cx, cy);

   //   int jmax = MIN(l, cy / 2);
   //   int jmin = - jmax;
   //   int imax = MIN(l, cx / 2);
   //   int imin = - imax;


   //   int joff = cy / 2 + rect.left;
   //   int ioff = cx / 2 + rect.top;

   //   //int iAngle = iStep % 360;
   //   //int iAngle = iStep;
   //   //int iAngle = 1;
   //   //int k = 0;

   //   /*     for ( int j=jmin; j<jmax; j++ )
   //   {
   //   for ( int i=imin; i<imax; i++ )
   //   {
   //   int x, y;

   //   // A Combination of a 2d Translation/rotation/Scale Matrix
   //   x=int(cos10(i, iAngle) - sin10(j, iAngle)) + ioff;
   //   y=int(sin10(i, iAngle) + cos10(j, iAngle)) + joff;
   //   m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //   spdib->m_pcolorref[abs(y%cy)*cx+abs(x%cx)];
   //   //k++;
   //   }
   //   (j+joff)*cx+(i+ioff)
   //   }*/

   //   int k = 0;
   //   double dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   double dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   int cx1 = this->cx - 1;
   //   int cy1 = this->cy - 1;
   //   for ( int j=jmin; j<jmax; j++ )
   //   {
   //      for ( int i=imin; i<imax; i++ )
   //      {
   //         int x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((int(dCos * i - dSin * j) + ioff) % cx);
   //         //y=abs((int(dSin * i + dCos * j) + joff) % cy);

   //         x = (int) abs((dCos * i - dSin * j) + ioff);
   //         y = (int) abs((dSin * i + dCos * j) + joff);

   //         if((x / cx) % 2 == 0)
   //         {
   //            x %= cx;
   //         }
   //         else
   //         {
   //            x = cx1 - (x % cx);
   //         }

   //         if((y / cy) % 2 == 0)
   //         {
   //            y %= cy;
   //         }
   //         else
   //         {
   //            y = cy1 - (y % cy);
   //         }



   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            pdib->m_pcolorref[y * cx + x];
   //         k++;
   //      }
   //   }
   //}

   /*int dib::cos(int i, int iAngle)
   {
   return (int) (((_int64) i * CosN[iAngle]) >> 32);
   }

   int dib::sin(int i, int iAngle)
   {
   return (int) (((_int64) i * SinN[iAngle]) >> 32);
   }*/




   /*void dib::Fill (int A, int R, int G, int B )
   {
   COLORREF color = RGB ( B, G, R ) | (A << 24);
   int size=stride*cy;

   COLORREF * pcr;

   map();

   int iSize32 = size / 32;
   int i;
   for (i=0; i < iSize32; i+=32 )
   {
   pcr = &m_pcolorref[i];
   pcr[0] = color;
   pcr[1] = color;
   pcr[2] = color;
   pcr[3] = color;
   pcr[4] = color;
   pcr[5] = color;
   pcr[6] = color;
   pcr[7] = color;
   pcr[8] = color;
   pcr[9] = color;
   pcr[10] = color;
   pcr[11] = color;
   pcr[12] = color;
   pcr[13] = color;
   pcr[14] = color;
   pcr[15] = color;
   pcr[16] = color;
   pcr[17] = color;
   pcr[18] = color;
   pcr[19] = color;
   pcr[20] = color;
   pcr[21] = color;
   pcr[22] = color;
   pcr[23] = color;
   pcr[24] = color;
   pcr[25] = color;
   pcr[26] = color;
   pcr[27] = color;
   pcr[28] = color;
   pcr[29] = color;
   pcr[30] = color;
   pcr[31] = color;
   }

   for (i=0; i<size; i++ )
   {
   m_pcolorref[i]=color;
   }

   }*/


   //COLORREF dib::GetAverageColor()
   //{
   //   double dR = 0.0;
   //   double dG = 0.0;
   //   double dB = 0.0;
   //   int iRLine;
   //   int iGLine;
   //   int iBLine;
   //   double dDiv = cx * cy;
   //   if(dDiv > 0)
   //   {
   //      LPBYTE lpb = (LPBYTE) m_pcolorref;
   //      for (int y = 0; y < cy; y++)
   //      {
   //         iRLine = 0;
   //         iGLine = 0;
   //         iBLine = 0;
   //         for (int x = 0; x < cx; x++)
   //         {
   //            iRLine += lpb[2];
   //            iGLine += lpb[1];
   //            iBLine += lpb[0];
   //            lpb += 4;
   //         }
   //         dR += iRLine / dDiv;
   //         dG += iGLine / dDiv;
   //         dB += iBLine / dDiv;
   //      }
   //      int iR = (int) dR;
   //      int iG = (int) dG;
   //      int iB = (int) dB;
   //      return RGB(iR, iG, iB);
   //   }
   //   else
   //   {
   //      return 0;
   //   }

   //}


   //void dib::xor(::draw2d::dib * pdib)
   //{
   //   if(cx != pdib->cx
   //      || cy != pdib->cy)
   //   {
   //      return;
   //   }
   //   int iCount = cx * cy;
   //   LPDWORD lpd1 = (LPDWORD) m_pcolorref;
   //   LPDWORD lpd2 = (LPDWORD) pdib->m_pcolorref;
   //   for(int i = 0; i < iCount; i++)
   //   {
   //      *lpd1 = *lpd1 ^ *lpd2;
   //      lpd1++;
   //      lpd2++;
   //   }
   //}

   //void dib::create_frame(class size size, int iFrameCount)
   //{
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   int iFrameWidth = size.cx / iSliceCount;
   //   int iFrameHeight = size.cy / iSliceCount;
   //   create(iFrameWidth, iFrameHeight);
   //}

   //void dib::set_frame1(void * lpdata, int iFrame, int iFrameCount)
   //{
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   int iFrameWidth = cx / iSliceCount;
   //   int iFrameHeight = cy / iSliceCount;
   //   int iX = iFrame % iSliceCount;
   //   int iY = iFrame / iSliceCount;
   //   COLORREF * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * cx];
   //   COLORREF * lpSrc = (COLORREF *) lpdata;
   //   COLORREF * lpDestLine;
   //   for(int y = 0; y < iFrameHeight; y++)
   //   {
   //      lpDestLine = &lpDest[y * cx];
   //      for(int x = 0; x < iFrameWidth; x++)
   //      {
   //         *lpDestLine = *lpSrc;
   //         lpDestLine++;
   //         lpSrc++;
   //      }
   //   }
   //}

   //void dib::set_frame2(void * lpdata, int iFrame, int iFrameCount)
   //{
   //   if(lpdata == NULL)
   //      return;
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   int iFrameWidth = cx / iSliceCount;
   //   int iFrameHeight = cy / iSliceCount;
   //   int iX = iFrame % iSliceCount;
   //   int iY = iFrame / iSliceCount;
   //   COLORREF * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * cx];
   //   COLORREF * lpSrc = (COLORREF *) lpdata;
   //   COLORREF * lpDestLine;
   //   for(int y = iFrameHeight - 1; y >= 0; y--)
   //   {
   //      lpDestLine = &lpDest[y * cx];
   //      for(int x = 0; x < iFrameWidth; x++)
   //      {
   //         *lpDestLine = *lpSrc;
   //         lpDestLine++;
   //         lpSrc++;
   //      }
   //   }
   //}

   //void dib::xor_dib_frame2(void * lpdata, int iFrame, int iFrameCount)
   //{
   //   if(lpdata == NULL)
   //      return;
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   int iFrameWidth = cx / iSliceCount;
   //   int iFrameHeight = cy / iSliceCount;
   //   int iX = iFrame % iSliceCount;
   //   int iY = iFrame / iSliceCount;
   //   COLORREF * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * cx];
   //   COLORREF * lpSrc = (COLORREF *) lpdata;
   //   COLORREF * lpDestLine;
   //   for(int y = iFrameHeight - 1; y >= 0; y--)
   //   {
   //      lpDestLine = &lpDest[y * cx];
   //      for(int x = 0; x < iFrameWidth; x++)
   //      {
   //         *lpDestLine ^= *lpSrc;
   //         lpDestLine++;
   //         lpSrc++;
   //      }
   //   }
   //}

   //void dib::get_frame(void * lpdata, int iFrame, int iFrameCount)
   //{
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   int iFrameWidth = cx / iSliceCount;
   //   int iFrameHeight = cy / iSliceCount;
   //   int iX = iFrame % iSliceCount;
   //   int iY = iFrame / iSliceCount;
   //   COLORREF * lpSrc = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight *  cx];
   //   COLORREF * lpDest = (COLORREF *) lpdata;
   //   COLORREF * lpSrcLine;
   //   for(int y = 0; y < iFrameHeight; y++)
   //   {
   //      lpSrcLine = &lpSrc[y * cx];
   //      for(int x = 0; x < iFrameWidth; x++)
   //      {
   //         *lpDest = *lpSrcLine;
   //         lpDest++;
   //         lpSrcLine++;
   //      }
   //   }
   //}

   //bool dib::is_rgb_black()
   //{
   //   int iSize = cx * cy;
   //   COLORREF * lp = m_pcolorref;
   //   for(int i = 0; i < iSize; i++)
   //   {
   //      if((*lp & 0x00FFFFFF) != 0)
   //         return false;
   //      lp++;
   //   }
   //   return true;
   //}

   //void dib::DivideRGB(int iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   int iCount = cx * cy;
   //   LPBYTE lp = ((LPBYTE) m_pcolorref);
   //   int i = 0;
   //   int iCount1 = iCount - iCount % 8;
   //   for(; i < iCount1; i++)
   //   {
   //      lp[0] /= (byte) iDivide;
   //      lp[1] /= (byte) iDivide;
   //      lp[2] /= (byte) iDivide;

   //      lp[4] /= (byte) iDivide;
   //      lp[5] /= (byte) iDivide;
   //      lp[6] /= (byte) iDivide;

   //      lp[8] /= (byte) iDivide;
   //      lp[9] /= (byte) iDivide;
   //      lp[10] /= (byte) iDivide;

   //      lp[12] /= (byte) iDivide;
   //      lp[13] /= (byte) iDivide;
   //      lp[14] /= (byte) iDivide;

   //      lp[16] /= (byte) iDivide;
   //      lp[17] /= (byte) iDivide;
   //      lp[28] /= (byte) iDivide;

   //      lp[20] /= (byte) iDivide;
   //      lp[21] /= (byte) iDivide;
   //      lp[22] /= (byte) iDivide;

   //      lp[24] /= (byte) iDivide;
   //      lp[25] /= (byte) iDivide;
   //      lp[26] /= (byte) iDivide;

   //      lp[28] /= (byte) iDivide;
   //      lp[29] /= (byte) iDivide;
   //      lp[30] /= (byte) iDivide;

   //      lp += 4 * 8;
   //   }
   //   for(; i < iCount; i++)
   //   {
   //      lp[0] /= (byte) iDivide;
   //      lp[1] /= (byte) iDivide;
   //      lp[2] /= (byte) iDivide;
   //      lp +=4;
   //   }
   //}

   //void dib::DivideARGB(int iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   int iCount = cx * cy;
   //   LPBYTE lp = ((LPBYTE) m_pcolorref);
   //   for(int i = 0; i < iCount; i++)
   //   {
   //      lp[0] /= (byte) iDivide;
   //      lp[1] /= (byte) iDivide;
   //      lp[2] /= (byte) iDivide;
   //      lp[3] /= (byte) iDivide;
   //      lp +=4;
   //   }
   //}

   //void dib::DivideA(int iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   int iCount = cx * cy;
   //   LPBYTE lp = ((LPBYTE) m_pcolorref);
   //   for(int i = 0; i < iCount; i++)
   //   {
   //      lp[3] /= (byte) iDivide;
   //      lp +=4;
   //   }
   //}


   void dib::stretch_dib(::draw2d::dib * pdib)
   {

      pdib->unmap();

      unmap();

      if (pdib->get_bitmap() == NULL)
      {

         return;

      }

      if (pdib->get_bitmap()->get_os_data() == NULL)
      {

         return;

      }

      D2D1_RECT_F rectDest = D2D1::RectF(0, 0, (FLOAT)m_size.cx, (FLOAT)m_size.cy);

      D2D1_RECT_F rectSource = D2D1::RectF(0, 0, (FLOAT)pdib->m_size.cx, (FLOAT)pdib->m_size.cy);

      ((ID2D1RenderTarget *)m_spgraphics->get_os_data())->DrawBitmap(((ID2D1Bitmap1 *)pdib->get_bitmap()->get_os_data()), rectDest, 1.0, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rectSource);

      /*
      ::StretchDIBits(
      SP_HDC(m_spgraphics),
      0, 0,
      cx, cy,
      0, 0,
      pdib->cx, pdib->cy,
      pdib->m_pcolorref,
      &pdib->m_info,
      DIB_RGB_COLORS,
      SRCCOPY);*/

   }

   ::draw2d::graphics * dib::get_graphics() const
   {
      unmap();
      return m_spgraphics;
   }

   //double dib::pi()
   //{
   //   return dPi;
   //}

   //void dib::fill_channel(int intensity, visual::rgba::echannel echannel)
   //{
   //   int offset = ((int)echannel) % 4;
   //   int size=cx*cy;

   //   BYTE * pb;

   //   int iSize32 = size / 32;
   //   int i;
   //   for (i=0; i < iSize32; i+=32 )
   //   {
   //      pb = ((BYTE * ) &m_pcolorref[i]) + offset;
   //      pb[0 * 4] = (byte) intensity;
   //      pb[1 * 4] = (byte) intensity;
   //      pb[2 * 4] = (byte) intensity;
   //      pb[3 * 4] = (byte) intensity;
   //      pb[4 * 4] = (byte) intensity;
   //      pb[5 * 4] = (byte) intensity;
   //      pb[6 * 4] = (byte) intensity;
   //      pb[7 * 4] = (byte) intensity;
   //      pb[8 * 4] = (byte) intensity;
   //      pb[9 * 4] = (byte) intensity;
   //      pb[10 * 4] = (byte) intensity;
   //      pb[11 * 4] = (byte) intensity;
   //      pb[12 * 4] = (byte) intensity;
   //      pb[13 * 4] = (byte) intensity;
   //      pb[14 * 4] = (byte) intensity;
   //      pb[15 * 4] = (byte) intensity;
   //      pb[16 * 4] = (byte) intensity;
   //      pb[17 * 4] = (byte) intensity;
   //      pb[18 * 4] = (byte) intensity;
   //      pb[19 * 4] = (byte) intensity;
   //      pb[20 * 4] = (byte) intensity;
   //      pb[21 * 4] = (byte) intensity;
   //      pb[22 * 4] = (byte) intensity;
   //      pb[23 * 4] = (byte) intensity;
   //      pb[24 * 4] = (byte) intensity;
   //      pb[25 * 4] = (byte) intensity;
   //      pb[26 * 4] = (byte) intensity;
   //      pb[27 * 4] = (byte) intensity;
   //      pb[28 * 4] = (byte) intensity;
   //      pb[29 * 4] = (byte) intensity;
   //      pb[30 * 4] = (byte) intensity;
   //      pb[31 * 4] = (byte) intensity;
   //   }

   //   for (i=0; i<size; i++ )
   //   {
   //      *(((BYTE * ) &m_pcolorref[i]) + offset) = (byte) intensity;
   //   }
   //}


   //int dib::cos(int i, int iAngle)
   //{
   //   return (int) (((_int64) i * CosN[iAngle]) >> 31);
   //}

   //int dib::sin(int i, int iAngle)
   //{
   //   return (int) (((_int64) i * SinN[iAngle]) >> 31);
   //}

   //int dib::cos10(int i, int iAngle)
   //{
   //   return (int) (((_int64) i * Cos10N[iAngle]) >> 34);
   //}

   //int dib::sin10(int i, int iAngle)
   //{
   //   return (int) (((_int64) i * Sin10N[iAngle]) >> 34);
   //}

   void dib::map(bool bApplyAlphaTransform) const
   {

      ::draw2d::lock draw2dlock;

      if (m_bMapped)
         return;

      if (m_spbitmapMap.is_null() || m_spbitmap.is_null())
         return;

      //dynamic_cast <::draw2d_direct2d::graphics *> (((dib *) this)->m_spgraphics.m_p)->SaveClip();

      ((dib *) this)->m_hrEndDraw = ((ID2D1DeviceContext *)m_spgraphics->get_os_data())->EndDraw();

      HRESULT hr = S_OK;

      if (m_spbitmap->get_os_data() != NULL)
      {


         hr = m_spbitmapMap->get_typed_os_data < ID2D1Bitmap1 >(::draw2d_direct2d::bitmap::data_bitmap1)->CopyFromBitmap(NULL, (ID2D1Bitmap *)m_spbitmap->get_os_data(), NULL);

      }

      sp(::draw2d_direct2d::bitmap) pb = m_spbitmap;

      ZERO(pb->m_map);

      hr = m_spbitmapMap->get_typed_os_data < ID2D1Bitmap1 >(::draw2d_direct2d::bitmap::data_bitmap1)->Map(D2D1_MAP_OPTIONS_READ, &pb->m_map);

      if (FAILED(hr) || pb->m_map.bits == NULL)
         _throw(simple_exception(get_app(), ""));

      ((dib *) this)->m_pcolorref = (COLORREF *)pb->m_map.bits;

      ((dib *) this)->m_iScan = pb->m_map.pitch;

      int compare_scan = m_size.cx * sizeof(COLORREF);

      int64_t i = m_iScan * m_size.cy / sizeof(COLORREF);

      if (bApplyAlphaTransform)
      {



         //byte * p = ((byte *)m_pcolorref);
         //while (i > 0)
         //{
         //   if (p[3] == 0)
         //   {
         //      p[0] = 0;
         //      p[1] = 0;
         //      p[2] = 0;
         //   }
         //   else
         //   {
         //      p[0] = (p[0] * 255 / p[3]);
         //      p[1] = (p[1] * 255 / p[3]);
         //      p[2] = (p[2] * 255 / p[3]);
         //   }
         //   p += 4;
         //   i--;
         //}

         ((dib *) this)->m_bTrans = true;

      }

      ((dib *) this)->m_bMapped = true;

   }

   void dib::unmap() const
   {

      ::draw2d::lock draw2dlock;

      if (!m_bMapped)
         return;

      if (m_spbitmapMap.is_null() || m_spbitmap.is_null())
         return;

      try
      {

         if (m_spbitmap->get_os_data() == NULL)
         {

            HRESULT hr = m_spbitmapMap->get_typed_os_data < ID2D1Bitmap1 >(::draw2d_direct2d::bitmap::data_bitmap1)->Unmap();

            ((dib *) this)->m_pcolorref = NULL;

            ((dib *) this)->m_bMapped = false;

            return;

         }

         int64_t iArea = area();

         byte * p = (byte *)m_pcolorref;

         int64_t i = m_iScan * m_size.cy / sizeof(COLORREF);

         if (m_bTrans)
         {

            //while(i > 0)
            //{
            //   p[0] = (p[0] * p[3] / 255);
            //   p[1] = (p[1] * p[3] / 255);
            //   p[2] = (p[2] * p[3] / 255);
            //   p += 4;
            //   i--;
            //}

         }


         D2D1_RECT_U srcRect;

         srcRect.left = 0;
         srcRect.right = m_size.cx;
         srcRect.top = 0;
         srcRect.bottom = m_size.cy;

         //memset(m_pcolorref, 127, scan * cy / 2);

         HRESULT hr = ((ID2D1Bitmap *)m_spbitmap->get_os_data())->CopyFromMemory(&srcRect, m_pcolorref, m_iScan);
         //zero(&METROWIN_BITMAP(m_spbitmapMap.m_p)->m_map, sizeof(METROWIN_BITMAP(m_spbitmapMap.m_p)->m_map));

         hr = m_spbitmapMap->get_typed_os_data < ID2D1Bitmap1 >(::draw2d_direct2d::bitmap::data_bitmap1)->Unmap();

         ((dib *) this)->m_pcolorref = NULL;

         if (FAILED(hr))
         {

            ((dib *) this)->m_bMapped = false;

            _throw(simple_exception(get_app(), ""));

         }

         m_spgraphics->SelectObject(m_spbitmap);

      }
      catch (...)
      {
      }

      if (SUCCEEDED(m_hrEndDraw))
      {

         ((ID2D1DeviceContext *)m_spgraphics->get_os_data())->BeginDraw();

         ///dynamic_cast <::draw2d_direct2d::graphics *> (((dib *) this)->m_spgraphics.m_p)->RestoreClip();

      }

      ((dib *) this)->m_bMapped = false;
      ((dib *) this)->m_bTrans = false;

   }

   /*   int dib::cx
   {
   return cx;
   }

   int dib::cy
   {
   return cy;
   }*/


   bool dib::defer_realize(::draw2d::graphics * pgraphics) const
   {

      if (is_realized())
      {
         return true;
      }

      return realize(pgraphics);

   }

   bool dib::realize(::draw2d::graphics * pgraphics) const
   {

      ::draw2d::lock draw2dlock;

      if (is_realized())
         unrealize();

      if (is_realized())
         return false;

      ((dib *) this)->m_spbitmap.alloc(((dib *) this)->allocer());
      ((dib *) this)->m_spgraphics.alloc(((dib *) this)->allocer());

      if (((dib *) this)->m_spbitmap.is_null() || ((dib *) this)->m_spbitmapMap.is_null() || ((dib *) this)->m_spgraphics.is_null() || ((dib *) this)->m_spgraphicsMap.is_null())
      {
         return false;
      }

      ::draw2d_direct2d::graphics * pgraphicsSrc = dynamic_cast <::draw2d_direct2d::graphics *> (pgraphics);

      ::draw2d_direct2d::graphics * pgraphicsDst = dynamic_cast <::draw2d_direct2d::graphics *> (((dib *) this)->m_spgraphics.m_p);

      ::draw2d_direct2d::bitmap * pbitmap = dynamic_cast <::draw2d_direct2d::bitmap *> (((dib *) this)->m_spbitmap.m_p);

      pgraphicsDst->m_pbitmaprendertarget = nullptr;

      pgraphicsDst->m_iType = 11;

      D2D1_SIZE_U sizeu = D2D1::SizeU(m_size.cx, m_size.cy);
      D2D1_PIXEL_FORMAT pixelformat;

      pixelformat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      pixelformat.format = DXGI_FORMAT_B8G8R8A8_UNORM;

      Microsoft::WRL::ComPtr < ID2D1RenderTarget > t;

      pgraphicsDst->CreateCompatibleDC(NULL);

      t = pgraphicsDst->m_prendertarget;

      HRESULT hr = t->CreateCompatibleRenderTarget(NULL, &sizeu, &pixelformat, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE, &pgraphicsDst->m_pbitmaprendertarget);

      if (pgraphicsDst->m_pbitmaprendertarget == NULL)
         return false;

      if (FAILED(pgraphicsDst->m_pbitmaprendertarget.As(&pgraphicsDst->m_prendertarget)))
      {
         pgraphicsDst->m_pbitmaprendertarget = nullptr;
         return false;
      }

      if (FAILED(pgraphicsDst->m_pbitmaprendertarget.As(&pgraphicsDst->m_pdevicecontext)))
      {
         pgraphicsDst->m_pbitmaprendertarget = nullptr;
         pgraphicsDst->m_prendertarget = nullptr;
         return false;
      }

      pgraphicsDst->m_pbitmaprendertarget->GetBitmap(&pbitmap->m_pbitmap);

      if (pbitmap->m_pbitmap == NULL)
      {
         ((dib *) this)->m_spgraphics.release();
         return false;
      }

      if (pgraphicsDst->m_spbitmap.is_null())
         pgraphicsDst->m_spbitmap.alloc(((dib *) this)->allocer());

      pgraphicsDst->m_spbitmap->attach(pbitmap->m_pbitmap.Get());

      D2D1_POINT_2U p;

      p.x = 0;
      p.y = 0;

      D2D1_RECT_U srcRect;

      srcRect.left = 0;
      srcRect.right = m_size.cx;
      srcRect.top = 0;
      srcRect.bottom = m_size.cy;

      return true;

   }


   bool dib::unrealize() const
   {

      if (!is_realized())
         return false;

      //((ID2D1DeviceContext *)((dib *) this)->m_spgraphics->get_os_data())->EndDraw();

      D2D1_POINT_2U p;

      p.x = 0;
      p.y = 0;

      D2D1_RECT_U srcRect;

      srcRect.left = 0;
      srcRect.right = m_size.cx;
      srcRect.top = 0;
      srcRect.bottom = m_size.cy;

      HRESULT hr = ((ID2D1Bitmap *)((dib *) this)->m_spbitmapMap->get_os_data())->CopyFromBitmap(&p, ((ID2D1Bitmap *)((dib *) this)->m_spbitmapMap->get_os_data()), &srcRect);

      ((dib *) this)->m_spgraphics.release();

      return true;

   }


   bool dib::is_realized() const
   {

      if (((dib *) this)->m_spgraphics.is_null() || ((dib *) this)->m_spgraphics->get_os_data() == NULL)
         return false;

      return true;

   }

   /*   int dib::scan()
   {

   return scan;

   }*/

   //#if defined(WINDOWSEX)


   bool dib::update_window(::aura::draw_interface * pwnd, ::message::message * pobj, bool bTransferBuffer)
   {

      rect64 rectWindow;

      pwnd->GetWindowRect(rectWindow);

      m_spgraphics->SetViewportOrg(0, 0);

      map(false);

      class rect rect(rectWindow);

      //      Application.window_graphics_update_window(pwnd->get_window_graphics(),pwnd->get_handle(),m_pcolorref,rect,m_size.cx, m_size.cy, m_iScan, bTransferBuffer);

      return true;

   }


   bool dib::blend(point ptDst, ::draw2d::dib * pdibSrc, point ptSrc, class size sizeParam, byte bA)
   {

      //if (bA < 255)
      {

         return ::draw2d::dib::blend(ptDst, pdibSrc, ptSrc, sizeParam, bA);

      }

      class size size(sizeParam);

      ::draw2d::lock draw2dlock;

      try
      {

         ::draw2d::graphics * pgraphicsSrc = pdibSrc->get_graphics();

         if (pgraphicsSrc == NULL)
            return FALSE;

         if (pgraphicsSrc->get_current_bitmap() == NULL)
            return FALSE;

         if (pgraphicsSrc->get_current_bitmap()->get_os_data() == NULL)
            return FALSE;

         if (get_graphics() != NULL &&
               get_graphics()->get_current_bitmap() != NULL &&
               get_graphics()->get_current_bitmap()->get_os_data() != NULL)
         {

            D2D1_SIZE_U sz = ((ID2D1Bitmap *)get_graphics()->get_current_bitmap()->get_os_data())->GetPixelSize();

            if (natural(size.cx + ptDst.x) > sz.width)
               size.cx = sz.width - ptDst.x;

            if (natural(size.cy + ptDst.y) > sz.height)
               size.cy = sz.height - ptDst.y;

         }

         bool bSmallerSourceRegion = false;

         ::size sizeSource;

         {

            D2D1_SIZE_U sz = ((ID2D1Bitmap *)pgraphicsSrc->get_current_bitmap()->get_os_data())->GetPixelSize();

            sizeSource.cx = sz.width;

            sizeSource.cy = sz.height;

            if (natural(size.cx + ptSrc.x) > sz.width)
            {

               size.cx = sz.width - ptSrc.x;

               bSmallerSourceRegion = true;

            }

            if (natural(size.cy + ptSrc.y) < sz.height)
            {

               size.cy = sz.height - ptSrc.y;

               bSmallerSourceRegion = true;

            }

         }

         if (ptDst.x > 0 || ptDst.y > 0)
         {

            bSmallerSourceRegion = true;

         }

         D2D1_RECT_F rectDst = D2D1::RectF((float)ptDst.x, (float)ptDst.y, (float)(ptDst.x + size.cx), (float)(ptDst.y + size.cy));

         D2D1_RECT_F rectSrc = D2D1::RectF((float)ptSrc.x, (float)ptSrc.y, (float)(ptSrc.x + size.cx), (float)(ptSrc.y + size.cy));

         HRESULT hr = ((ID2D1DeviceContext *)pgraphicsSrc->get_os_data())->EndDraw();

         ::draw2d_direct2d::graphics * pgraphicsDst = dynamic_cast <::draw2d_direct2d::graphics *> (get_graphics());

         pgraphicsDst->set_alpha_mode(::draw2d::alpha_mode_set);

         {

            D2D1_POINT_2F p;
            p.x = rectDst.left;
            p.y = rectDst.top;

            pgraphicsDst->m_pdevicecontext->DrawImage((ID2D1Bitmap *)pgraphicsSrc->get_current_bitmap()->get_os_data(), p, rectSrc, pgraphicsDst->m_interpolationmode, D2D1_COMPOSITE_MODE_DESTINATION_IN);

         }

         if(bSmallerSourceRegion)
         {

            ::draw2d::savedc k1(pgraphicsDst);

            class rect rDst;
            rDst.left = ptDst.x;
            rDst.top = ptDst.y;
            rDst.right = ptDst.x + size.cx;
            rDst.bottom = ptDst.y + size.cy;

            pgraphicsDst->ExcludeClipRect(rDst);

            D2D1_RECT_F r1;
            r1.left = (FLOAT) (ptDst.x);
            r1.top = (FLOAT)(ptDst.y);
            r1.right = (FLOAT)(ptDst.x + sizeParam.cx);
            r1.bottom = (FLOAT)(ptDst.y + sizeParam.cy);

            D2D1_RECT_F r2;
            r2.left = (FLOAT)(ptSrc.x);
            r2.top = (FLOAT)(ptSrc.y);
            r2.right = (FLOAT)(ptSrc.x + sizeParam.cx);
            r2.bottom = (FLOAT)(ptSrc.y + sizeParam.cy);

            pgraphicsDst->m_pdevicecontext->DrawBitmap((ID2D1Bitmap *)pgraphicsSrc->get_current_bitmap()->get_os_data(), r1,bA / 255.0f, pgraphicsDst->m_interpolationmode, r2);

         }
         else
         {

            output_debug_string("opt.out.exc.draw");

         }

         //hr = m_prendertarget->Flush();
         pgraphicsDst->flush();

         if (SUCCEEDED(hr))
         {

            ((ID2D1DeviceContext *)pgraphicsSrc->get_os_data())->BeginDraw();

            //dynamic_cast <::draw2d_direct2d::graphics *> (pgraphicsSrc)->RestoreClip();

         }

         return true;

      }
      catch (...)
      {
         return FALSE;
      }

   }


   /*

   bool dib::print_window(window * pwnd, ::message::message * pobj)
   {


   SCAST_PTR(::message::base, pbase, pobj);

   if(pbase->m_wparam == NULL)
   return false;

   m_spgraphics->attach((HDC) pbase->m_wparam);

   rect rectx;

   ::draw2d::bitmap * pbitmap = m_spgraphics->get_current_bitmap();

   ::GetCurrentObject((HDC) pbase->m_wparam, OBJ_BITMAP);

   //      uint32_t dw = ::get_last_error();
   class size size = pbitmap->get_size();

   rectx.left = 0;
   rectx.top = 0;
   rectx.right = size.cx;
   rectx.bottom = size.cy;

   try
   {

   rect rectWindow;

   pwnd->GetWindowRect(rectWindow);

   ::draw2d::dib_sp dib(allocer());

   if(!dib->create(rectWindow.bottom_right()))
   return false;

   ::draw2d::graphics * pgraphics = dib->get_graphics();

   if(pgraphics->get_os_data() == NULL)
   return false;

   rect rectPaint;
   rect rectUpdate;
   rectUpdate = rectWindow;
   rectPaint = rectWindow;
   rectPaint.offset(-rectPaint.top_left());
   m_spgraphics->SelectClipRgn(NULL);
   if(pwnd->m_pui != NULL && pwnd->m_pui != this)
   {
   pwnd->m_pui->_001OnDeferPaintLayeredWindowBackground(pgraphics);
   }
   else
   {
   pwnd->_001OnDeferPaintLayeredWindowBackground(pgraphics);
   }
   m_spgraphics->SelectClipRgn(NULL);
   m_spgraphics-> SetViewportOrg(point(0, 0));
   pwnd->_000OnDraw(pgraphics);
   m_spgraphics->SetViewportOrg(point(0, 0));
   //(dynamic_cast<::win::graphics * >(pgraphics))->fill_solid_rect(rectUpdate.left, rectUpdate.top, 100, 100, 255);
   m_spgraphics->SelectClipRgn(NULL);
   m_spgraphics->SetViewportOrg(point(0, 0));

   m_spgraphics->SelectClipRgn( NULL);
   m_spgraphics->BitBlt(rectPaint.left, rectPaint.top,
   rectPaint.width(), rectPaint.height(),
   pgraphics, rectUpdate.left, rectUpdate.top,
   SRCCOPY);

   }
   catch(...)
   {
   }
   m_spgraphics->fill_solid_rect(rectx, RGB(255, 255, 255));
   pobj->m_bRet = true;
   pbase->set_lresult(0);

   return true;
   }

   */


   //#endif

   void dib::tint(::draw2d::dib * pdib, int32_t R, int32_t G, int32_t B)
   {

      if (!create(pdib->m_size))
      {

         return;

      }

      ::rect rectDib1(null_point(), pdib->m_size);

      get_graphics()->fill_solid_rect(rectDib1, ARGB(255, R, G, B));

      sp(::draw2d_direct2d::graphics) pgraphicsDib1 = get_graphics();

      sp(::draw2d_direct2d::graphics) pgraphicsDib2 = pdib->get_graphics();

      ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->EndDraw();

      pgraphicsDib1->m_pdevicecontext->DrawImage(
      (ID2D1Bitmap *)pgraphicsDib2->get_current_bitmap()->get_os_data(),
      D2D1::Point2F(0.f, 0.f),
      d2d1::rectf(rectDib1),
      D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
      D2D1_COMPOSITE_MODE_DESTINATION_IN);

      set_alpha_mode(::draw2d::alpha_mode_blend);

      ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->BeginDraw();

   }

} // namespace draw2d_direct2d









