//   Creator : El Barto (ef00@luc.ac.be)
//   Location : http://www.luc.ac.be/~ef00/ebgfx
//   Date : 09-04-98
//////////////////////////////////////////////////////////////////////
#include "framework.h"

byte byte_clip(double d);


byte byte_clip(double d)
{
   if(d >= 255.0)
      return 255;
   if(d <= 0.0)
      return 0;
   return (byte) d;
}

namespace draw2d
{


   dib::dib()
   {

      cx      = 0;

      cy      = 0;

      scan    = 0;

   }


   dib::~dib()
   {

   }


   ::draw2d::graphics * dib::get_graphics()
   {

      throw interface_only_exception(get_app());

   }


   ::draw2d::bitmap_sp dib::get_bitmap()
   {

      throw interface_only_exception(get_app());

   }


   ::draw2d::bitmap_sp dib::detach_bitmap()
   {

      throw interface_only_exception(get_app());

   }


   void dib::construct (int32_t cx,  int32_t cy)
   {

      UNREFERENCED_PARAMETER(cx);
      UNREFERENCED_PARAMETER(cy);

      throw interface_only_exception(get_app());

   }


   COLORREF * dib::get_data()
   {

      return m_pcolorref;

   }


   bool dib::realize(::draw2d::graphics * pdc)
   {

      UNREFERENCED_PARAMETER(pdc);
      return true;

   }


   bool dib::unrealize()
   {

      return true;

   }


   bool dib::is_realized()
   {

      return true;

   }

   bool dib::defer_realize(::draw2d::graphics * pdc)
   {

      if(is_realized())
         return true;

      return realize(pdc);

   }


   bool dib::create(class size size)
   {
      return create(size.cx, size.cy);
   }

   bool dib::create(int32_t width, int32_t height)
   {
      return create(::size(width, height));
   }

   bool dib::dc_select(bool bSelect)
   {
      UNREFERENCED_PARAMETER(bSelect);
      throw interface_only_exception(get_app());
   }

   bool dib::create(::draw2d::graphics * pdc)
   {
      ::draw2d::bitmap & bitmap = pdc->get_current_bitmap();
      if(&bitmap == NULL)
         return FALSE;

      throw not_implemented(get_app());
      /*
      BITMAP bm;
      bitmap.GetObject(sizeof(bm), &bm);
      if(!create(bm.bmWidth, bm.bmHeight))
      {
         return FALSE;
      }
      from(pdc);*/
      return TRUE;
   }

   bool dib::Destroy ()
   {
      throw interface_only_exception(get_app());
   }

   bool dib::to(::draw2d::graphics * pgraphics)
   {
      return to(pgraphics, null_point(), size());
   }

   bool dib::to(::draw2d::graphics * pgraphics, point pt)
   {
      return to(pgraphics, pt, size());
   }

   bool dib::to(::draw2d::graphics * pgraphics, class size size)
   {
      return to(pgraphics, null_point(), size);
   }

   bool dib::to(::draw2d::graphics * pgraphics, LPCRECT lpcrect)
   {
      class rect rect(lpcrect);
      return to(pgraphics, rect.top_left(), rect.size());
   }

   bool dib::to(::draw2d::graphics * pgraphics, point pt, class size size)
   {
      return to(pgraphics, pt, size, point(0, 0));
   }

   bool dib::to(::draw2d::graphics * pgraphics, point pt, class size size, point ptSrc)
   {
      UNREFERENCED_PARAMETER(pgraphics);
      UNREFERENCED_PARAMETER(pt);
      UNREFERENCED_PARAMETER(size);
      UNREFERENCED_PARAMETER(ptSrc);
      throw interface_only_exception(get_app());
   }

   bool dib::from(::draw2d::dib * pdib)
   {

      if(!create(pdib->size()))
         return false;

      map();
      pdib->map();
      try
      {
         memcpy(get_data(), pdib->get_data(), (size_t) (sizeof(COLORREF) * area()));
      }
      catch(...)
      {
         return false;
      }
      return true;
   }

   bool dib::from(::draw2d::graphics * pdc)
   {
      UNREFERENCED_PARAMETER(pdc);
      throw interface_only_exception(get_app());
   }

   bool dib::from(point ptDst, ::draw2d::graphics * pdc, point ptSrc, class size size)
   {
      return get_graphics()->from(ptDst, size, pdc, ptSrc, SRCCOPY) != FALSE;
   }

   bool dib::from(point ptDst, ::draw2d::dib * pdib, point ptSrc, class size size)
   {

      if(ptSrc.x < 0)
      {
         ptDst.x -= ptSrc.x;
         ptSrc.x = 0;
      }

      if(ptSrc.y < 0)
      {
         ptDst.y -= ptSrc.y;
         ptSrc.y = 0;
      }

      if(ptDst.x < 0)
      {
         size.cx += ptDst.x;
         ptDst.x = 0;
      }

      if(size.cx < 0)
         return true;

      if(ptDst.y < 0)
      {
         size.cy += ptDst.y;
         ptDst.y = 0;
      }

      if(size.cy < 0)
         return true;

      int xEnd = min(size.cx, min(pdib->cx - ptSrc.x, cx - ptDst.x));

      int yEnd = min(size.cy, min(pdib->cy - ptSrc.y, cy - ptDst.y));

      if(xEnd < 0)
         return false;

      if(yEnd < 0)
         return false;

      int32_t s1 = scan / sizeof(COLORREF);

      int32_t s2 = pdib->scan / sizeof(COLORREF);

      COLORREF * pdst = &m_pcolorref[s1 * ptDst.y] + ptDst.x;

      COLORREF * psrc = &pdib->m_pcolorref[s2 * ptSrc.y] + ptSrc.x;

      COLORREF * pdst2;

      COLORREF * psrc2;

      for(int y = 0; y < yEnd; y++)
      {

         pdst2 = &pdst[s1 * y];

         psrc2 = &psrc[s2 * y];

         for(int x = 0; x < xEnd; x++)
         {

            *pdst2 = *psrc2;

            pdst2++;

            psrc2++;

         }

      }

      return true;

   }

   bool dib::from_ignore_alpha(point ptDst, ::draw2d::dib * pdib, point ptSrc, class size size)
   {

      if(ptDst.x < 0)
      {
         size.cx += ptDst.x;
         ptDst.x = 0;
      }

      if(size.cx < 0)
         return true;

      if(ptDst.y < 0)
      {
         size.cy += ptDst.y;
         ptDst.y = 0;
      }

      if(size.cy < 0)
         return true;

      int xEnd = min(size.cx, min(pdib->cx - ptSrc.x, cx - ptDst.x));

      int yEnd = min(size.cy, min(pdib->cy - ptSrc.y, cy - ptDst.y));

      if(xEnd < 0)
         return false;

      if(yEnd < 0)
         return false;

      int32_t s1 = scan / sizeof(COLORREF);

      int32_t s2 = pdib->scan / sizeof(COLORREF);

      COLORREF * pdst = &m_pcolorref[s1 * ptDst.y] + ptDst.x;

      COLORREF * psrc = &pdib->m_pcolorref[s2 * ptSrc.y] + ptSrc.x;

      COLORREF * pdst2;

      COLORREF * psrc2;

      int i = 0;

      for(int y = 0; y < yEnd; y++)
      {

         pdst2 = &pdst[s1 * y];

         psrc2 = &psrc[s2 * y];

         for(int x = 0; x < xEnd; x++)
         {

            if(GetAValue(*psrc2) == 0)
            {
               i++;
            }
            else
            {
               *pdst2 = *psrc2;
            }

            pdst2++;

            psrc2++;

         }

      }

      return true;

   }

   void dib::set( int32_t R, int32_t G, int32_t B )
   {
      int32_t size=cx*cy;

      COLORREF * pcr = get_data();

      for(int32_t i=0; i < size; i++)
      {
         ((byte *) pcr)[0] = R;
         ((byte *) pcr)[1] = G;
         ((byte *) pcr)[2] = B;
         pcr++;
      }

   }

/*   void dib::Fill ( int32_t R, int32_t G, int32_t B )
   {
      COLORREF color=RGB ( B, G, R );
      int32_t size=cx*cy;

      COLORREF * pcr;

      int32_t iSize32 = size / 32;
      int32_t i;
      for (i=0; i < iSize32; i+=32 )
      {
         pcr = &get_data()[i];
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
         get_data()[i]=color;
      }
   }
   */


   void dib::ToAlpha(int32_t i)
   {
      BYTE *dst=(BYTE*)get_data();
      int32_t size=cx*cy;

      while ( size-- )
      {
         dst[3] = dst[i];
         dst+=4;
      }
   }

   void dib::from_alpha()
   {
      BYTE *dst=(BYTE*)get_data();
      int64_t size = area();

      while ( size-- )
      {
         dst[0] = dst[3];
         dst[1] = dst[3];
         dst[2] = dst[3];
         dst+=4;
      }
   }

   void dib::mult_alpha(::draw2d::dib * pdib, bool bPreserveAlpha)
   {
      UNREFERENCED_PARAMETER(pdib);
      UNREFERENCED_PARAMETER(bPreserveAlpha);

      BYTE *dst=(BYTE*)get_data();
      int64_t size = area();


      // >> 8 instead of / 255 subsequent alpha_blend operations say thanks on true_blend because (255) * (1/254) + (255) * (254/255) > 255


      while (size >= 8)
      {
         dst[0] = LOBYTE(((int32_t)dst[0] * (int32_t)dst[3])>> 8);
         dst[1] = LOBYTE(((int32_t)dst[1] * (int32_t)dst[3])>> 8);
         dst[2] = LOBYTE(((int32_t)dst[2] * (int32_t)dst[3])>> 8);

         dst[4+0] = LOBYTE(((int32_t)dst[4+0] * (int32_t)dst[4+3])>> 8);
         dst[4+1] = LOBYTE(((int32_t)dst[4+1] * (int32_t)dst[4+3])>> 8);
         dst[4+2] = LOBYTE(((int32_t)dst[4+2] * (int32_t)dst[4+3])>> 8);

         dst[8+0] = LOBYTE(((int32_t)dst[8+0] * (int32_t)dst[8+3])>> 8);
         dst[8+1] = LOBYTE(((int32_t)dst[8+1] * (int32_t)dst[8+3])>> 8);
         dst[8+2] = LOBYTE(((int32_t)dst[8+2] * (int32_t)dst[8+3])>> 8);

         dst[12+0] = LOBYTE(((int32_t)dst[12+0] * (int32_t)dst[12+3])>> 8);
         dst[12+1] = LOBYTE(((int32_t)dst[12+1] * (int32_t)dst[12+3])>> 8);
         dst[12+2] = LOBYTE(((int32_t)dst[12+2] * (int32_t)dst[12+3])>> 8);

         dst[16+0] = LOBYTE(((int32_t)dst[16+0] * (int32_t)dst[16+3])>> 8);
         dst[16+1] = LOBYTE(((int32_t)dst[16+1] * (int32_t)dst[16+3])>> 8);
         dst[16+2] = LOBYTE(((int32_t)dst[16+2] * (int32_t)dst[16+3])>> 8);

         dst[20+0] = LOBYTE(((int32_t)dst[20+0] * (int32_t)dst[20+3])>> 8);
         dst[20+1] = LOBYTE(((int32_t)dst[20+1] * (int32_t)dst[20+3])>> 8);
         dst[20+2] = LOBYTE(((int32_t)dst[20+2] * (int32_t)dst[20+3])>> 8);

         dst[24+0] = LOBYTE(((int32_t)dst[24+0] * (int32_t)dst[24+3])>> 8);
         dst[24+1] = LOBYTE(((int32_t)dst[24+1] * (int32_t)dst[24+3])>> 8);
         dst[24+2] = LOBYTE(((int32_t)dst[24+2] * (int32_t)dst[24+3])>> 8);

         dst[28+0] = LOBYTE(((int32_t)dst[28+0] * (int32_t)dst[28+3])>> 8);
         dst[28+1] = LOBYTE(((int32_t)dst[28+1] * (int32_t)dst[28+3])>> 8);
         dst[28+2] = LOBYTE(((int32_t)dst[28+2] * (int32_t)dst[28+3])>> 8);

         dst += 4 * 8;
         size -= 8;
      }
      while(size--)
      {
         dst[0] = LOBYTE(((int32_t)dst[0] * (int32_t)dst[3])>> 8);
         dst[1] = LOBYTE(((int32_t)dst[1] * (int32_t)dst[3])>> 8);
         dst[2] = LOBYTE(((int32_t)dst[2] * (int32_t)dst[3])>> 8);
         dst += 4;
      }
   }

   void dib::mult_alpha()
   {

      BYTE *dst=(BYTE*)get_data();
      int64_t size = area();


      //  / 255 instead of / 255 subsequent alpha_blend operations say thanks on true_blend because (255) * (1/254) + (255) * (254/255) > 255


      while (size >= 8)
      {
         dst[0] = byte_clip(((int32_t)dst[0] * (int32_t)dst[3]) / 255);
         dst[1] = byte_clip(((int32_t)dst[1] * (int32_t)dst[3]) / 255);
         dst[2] = byte_clip(((int32_t)dst[2] * (int32_t)dst[3]) / 255);

         dst[4+0] = byte_clip(((int32_t)dst[4+0] * (int32_t)dst[4+3]) / 255);
         dst[4+1] = byte_clip(((int32_t)dst[4+1] * (int32_t)dst[4+3]) / 255);
         dst[4+2] = byte_clip(((int32_t)dst[4+2] * (int32_t)dst[4+3]) / 255);

         dst[8+0] = byte_clip(((int32_t)dst[8+0] * (int32_t)dst[8+3]) / 255);
         dst[8+1] = byte_clip(((int32_t)dst[8+1] * (int32_t)dst[8+3]) / 255);
         dst[8+2] = byte_clip(((int32_t)dst[8+2] * (int32_t)dst[8+3]) / 255);

         dst[12+0] = byte_clip(((int32_t)dst[12+0] * (int32_t)dst[12+3]) / 255);
         dst[12+1] = byte_clip(((int32_t)dst[12+1] * (int32_t)dst[12+3]) / 255);
         dst[12+2] = byte_clip(((int32_t)dst[12+2] * (int32_t)dst[12+3]) / 255);

         dst[16+0] = byte_clip(((int32_t)dst[16+0] * (int32_t)dst[16+3]) / 255);
         dst[16+1] = byte_clip(((int32_t)dst[16+1] * (int32_t)dst[16+3]) / 255);
         dst[16+2] = byte_clip(((int32_t)dst[16+2] * (int32_t)dst[16+3]) / 255);

         dst[20+0] = byte_clip(((int32_t)dst[20+0] * (int32_t)dst[20+3]) / 255);
         dst[20+1] = byte_clip(((int32_t)dst[20+1] * (int32_t)dst[20+3]) / 255);
         dst[20+2] = byte_clip(((int32_t)dst[20+2] * (int32_t)dst[20+3]) / 255);

         dst[24+0] = byte_clip(((int32_t)dst[24+0] * (int32_t)dst[24+3]) / 255);
         dst[24+1] = byte_clip(((int32_t)dst[24+1] * (int32_t)dst[24+3]) / 255);
         dst[24+2] = byte_clip(((int32_t)dst[24+2] * (int32_t)dst[24+3]) / 255);

         dst[28+0] = byte_clip(((int32_t)dst[28+0] * (int32_t)dst[28+3]) / 255);
         dst[28+1] = byte_clip(((int32_t)dst[28+1] * (int32_t)dst[28+3]) / 255);
         dst[28+2] = byte_clip(((int32_t)dst[28+2] * (int32_t)dst[28+3]) / 255);

         dst += 4 * 8;
         size -= 8;
      }
      while(size--)
      {
         dst[0] = byte_clip(((int32_t)dst[0] * (int32_t)dst[3]) / 255);
         dst[1] = byte_clip(((int32_t)dst[1] * (int32_t)dst[3]) / 255);
         dst[2] = byte_clip(((int32_t)dst[2] * (int32_t)dst[3]) / 255);
         dst += 4;
      }
   }


   void dib::div_alpha()
   {

      BYTE *dst=(BYTE*)get_data();
      int64_t size = area();


      // >> 8 instead of / 255 subsequent alpha_blend operations say thanks on true_blend because (255) * (1/254) + (255) * (254/255) > 255


/*      while (size >= 8)
      {
         dst[0] = LOBYTE(((int32_t)dst[0] * (int32_t)dst[3])>> 8);
         dst[1] = LOBYTE(((int32_t)dst[1] * (int32_t)dst[3])>> 8);
         dst[2] = LOBYTE(((int32_t)dst[2] * (int32_t)dst[3])>> 8);

         dst[4+0] = LOBYTE(((int32_t)dst[4+0] * (int32_t)dst[4+3])>> 8);
         dst[4+1] = LOBYTE(((int32_t)dst[4+1] * (int32_t)dst[4+3])>> 8);
         dst[4+2] = LOBYTE(((int32_t)dst[4+2] * (int32_t)dst[4+3])>> 8);

         dst[8+0] = LOBYTE(((int32_t)dst[8+0] * (int32_t)dst[8+3])>> 8);
         dst[8+1] = LOBYTE(((int32_t)dst[8+1] * (int32_t)dst[8+3])>> 8);
         dst[8+2] = LOBYTE(((int32_t)dst[8+2] * (int32_t)dst[8+3])>> 8);

         dst[12+0] = LOBYTE(((int32_t)dst[12+0] * (int32_t)dst[12+3])>> 8);
         dst[12+1] = LOBYTE(((int32_t)dst[12+1] * (int32_t)dst[12+3])>> 8);
         dst[12+2] = LOBYTE(((int32_t)dst[12+2] * (int32_t)dst[12+3])>> 8);

         dst[16+0] = LOBYTE(((int32_t)dst[16+0] * (int32_t)dst[16+3])>> 8);
         dst[16+1] = LOBYTE(((int32_t)dst[16+1] * (int32_t)dst[16+3])>> 8);
         dst[16+2] = LOBYTE(((int32_t)dst[16+2] * (int32_t)dst[16+3])>> 8);

         dst[20+0] = LOBYTE(((int32_t)dst[20+0] * (int32_t)dst[20+3])>> 8);
         dst[20+1] = LOBYTE(((int32_t)dst[20+1] * (int32_t)dst[20+3])>> 8);
         dst[20+2] = LOBYTE(((int32_t)dst[20+2] * (int32_t)dst[20+3])>> 8);

         dst[24+0] = LOBYTE(((int32_t)dst[24+0] * (int32_t)dst[24+3])>> 8);
         dst[24+1] = LOBYTE(((int32_t)dst[24+1] * (int32_t)dst[24+3])>> 8);
         dst[24+2] = LOBYTE(((int32_t)dst[24+2] * (int32_t)dst[24+3])>> 8);

         dst[28+0] = LOBYTE(((int32_t)dst[28+0] * (int32_t)dst[28+3])>> 8);
         dst[28+1] = LOBYTE(((int32_t)dst[28+1] * (int32_t)dst[28+3])>> 8);
         dst[28+2] = LOBYTE(((int32_t)dst[28+2] * (int32_t)dst[28+3])>> 8);

         dst += 4 * 8;
         size -= 8;
      }*/
      while(size--)
      {
         if(dst[3] == 0)
         {
            dst[0] = 0;
            dst[1] = 0;
            dst[2] = 0;
         }
         else
         {
            dst[0] = byte_clip((int32_t) dst[0] * 255 / (int32_t) dst[3]);
            dst[1] = byte_clip((int32_t) dst[1] * 255 / (int32_t) dst[3]);
            dst[2] = byte_clip((int32_t) dst[2] * 255 / (int32_t) dst[3]);
         }

         dst += 4;

      }

   }

   void dib::Map(int32_t ToRgb, int32_t FromRgb)
   {
      byte * dst= (byte *) get_data();
      int32_t size=cx*cy;

      while ( size-- )
      {
         *dst = (byte) (*dst == FromRgb ? ToRgb : *dst);
         dst+=4;
      }
   }


   void dib::ToAlphaAndFill(int32_t i, COLORREF cr)
   {
      BYTE *dst=(BYTE*)get_data();
      int32_t size=cx*cy;

      BYTE uchB = rgba_get_b(cr);
      BYTE uchG = rgba_get_g(cr);
      BYTE uchR = rgba_get_r(cr);

      while ( size-- )
      {
         dst[3] = dst[i];
         dst[0] = uchB;
         dst[1] = uchG;
         dst[2] = uchR;
         dst+=4;
      }
   }

   void dib::GrayToARGB(COLORREF cr)
   {
      BYTE *dst=(BYTE*)get_data();
      int32_t size=cx*cy;

      uint32_t dwB = rgba_get_b(cr);
      uint32_t dwG = rgba_get_g(cr);
      uint32_t dwR = rgba_get_r(cr);

      while (size-- > 0)
      {
         dst[3] = dst[0];
         dst[0] = (BYTE)(((dwB * dst[3]) / 256) & 0xff);
         dst[1] = (BYTE)(((dwG * dst[3]) / 256) & 0xff);
         dst[2] = (BYTE)(((dwR * dst[3]) / 256) & 0xff);
         dst+=4;
      }
   }


   void dib::BitBlt(dib *pdib, int32_t op)
   {
      if(op == 123) // zero dest RGB, invert alpha, and OR src RGB
      {

         COLORREF _colorref = RGB ( 0, 0, 0 ) | (255 << 24);
         COLORREF colorrefa[2];
         colorrefa[0] = _colorref;
         colorrefa[1] = _colorref;

         COLORREF _colorrefN = RGB ( 255, 255, 255) | (0 << 24);
         COLORREF colorrefaN[2];
         colorrefaN[0] = _colorrefN;

         colorrefaN[1] = _colorrefN;
#ifdef AMD64

         //x64
#else

         int32_t isize=cx*cy;
         LPDWORD lpbitsSrc= (LPDWORD) pdib->get_data();
         LPDWORD lpbitsDest= (LPDWORD) get_data();

    #ifdef WINDOWS
         _asm
         {
            emms
               mov      eax, isize
               mov      ebx, lpbitsDest
               mov      ecx, lpbitsSrc
               movq     mm0, colorrefa
               movq     mm7, colorrefaN
fill_loop:
            cmp      eax, 1
               jle      fill_last
               movq     mm1, [ebx]
            movq     mm2, [ecx]
            pandn    mm1, mm0
               pand     mm2, mm7
               por      mm1, mm2
               movq     [ebx], mm1

               sub      eax, 2
               add      ebx, 8
               add      ecx, 8

               jmp      fill_loop

fill_last:
            emms
         }
#else

         memcpy(lpbitsDest, lpbitsSrc, isize * 4);

         #endif
#endif
      }

   }


   void dib::Invert()
   {
      int32_t size=cx*cy;
      LPBYTE lpb = (LPBYTE) get_data();
      for ( int32_t i=0; i<size; i++ )
      {
         lpb[0] = 255 - lpb[0];
         lpb[1] = 255 - lpb[1];
         lpb[2] = 255 - lpb[2];
         lpb += 4;
      }
   }

   void dib::channel_invert(visual::rgba::echannel echannel)
   {
      int64_t size=area();
      LPBYTE lpb = (LPBYTE) get_data();
      lpb += ((int32_t)echannel) % 4;
      for ( int32_t i=0; i<size; i++ )
      {
         *lpb = 255 - *lpb;
         lpb += 4;
      }
   }

   void dib::channel_multiply(visual::rgba::echannel echannel, double dRate)
   {
      if(dRate < 0)
         return;
      register int64_t size = area();
      LPBYTE lpb = (LPBYTE) get_data();
      lpb += ((int32_t)echannel) % 4;
      register int32_t iDiv = 256 * 256;
      register int32_t iMul = (int32_t) (dRate * ((double) iDiv));
      register int32_t iRes;
      for(register int64_t i = 0; i < size; i++)
      {
         iRes = *lpb * iMul / iDiv;
         *lpb = (byte) (iRes > 255 ? 255 : iRes);
         lpb += 4;
      }
   }

   void dib::channel_multiply(visual::rgba::echannel echannel, ::draw2d::dib * pdib)
   {
      register int64_t size = area();
      LPBYTE lpb1 = (LPBYTE) get_data();
      LPBYTE lpb2 = (LPBYTE) pdib->get_data();
      lpb1 += ((int32_t)echannel) % 4;
      lpb2 += ((int32_t)echannel) % 4;
      for(register int64_t i = 0; i < size; i++)
      {
         *lpb1 = (BYTE)(((uint32_t)*lpb1 * (uint32_t)*lpb2) / 255);
         lpb1 += 4;
         lpb2 += 4;
      }
   }

   void dib::channel_darken(visual::rgba::echannel echannel, ::draw2d::dib * pdib)
   {
      register int64_t size = area();
      LPBYTE lpb1 = (LPBYTE) get_data();
      LPBYTE lpb2 = (LPBYTE) pdib->get_data();
      lpb1 += ((int32_t)echannel) % 4;
      lpb2 += ((int32_t)echannel) % 4;
      for(register int64_t i = 0; i < size; i++)
      {
         *lpb1 = *lpb1 < *lpb2 ? *lpb1 : *lpb2;
         lpb1 += 4;
         lpb2 += 4;
      }
   }

   void dib::channel_lighten(visual::rgba::echannel echannel, ::draw2d::dib * pdib)
   {
      register int64_t size = area();
      LPBYTE lpb1 = (LPBYTE) get_data();
      LPBYTE lpb2 = (LPBYTE) pdib->get_data();
      lpb1 += ((int32_t)echannel) % 4;
      lpb2 += ((int32_t)echannel) % 4;
      for(register int64_t i = 0; i < size; i++)
      {
         *lpb1 = *lpb1 > *lpb2 ? *lpb1 : *lpb2;
         lpb1 += 4;
         lpb2 += 4;
      }
   }

   void dib::channel_from(visual::rgba::echannel echannel, ::draw2d::dib * pdib)
   {
      map();
      pdib->map();
      int64_t size = area();
      register int64_t size64 = size / 64;
      LPBYTE lpb1 = (LPBYTE) get_data();
      LPBYTE lpb2 = (LPBYTE) pdib->get_data();
      lpb1 += ((int32_t)echannel) % 4;
      lpb2 += ((int32_t)echannel) % 4;
      register int64_t i = 0;
      for(; i < size64; i++)
      {
         lpb1[4 *  0]  =  lpb2[4 *  0];
         lpb1[4 *  1]  =  lpb2[4 *  1];
         lpb1[4 *  2]  =  lpb2[4 *  2];
         lpb1[4 *  3]  =  lpb2[4 *  3];
         lpb1[4 *  4]  =  lpb2[4 *  4];
         lpb1[4 *  5]  =  lpb2[4 *  5];
         lpb1[4 *  6]  =  lpb2[4 *  6];
         lpb1[4 *  7]  =  lpb2[4 *  7];
         lpb1[4 *  8]  =  lpb2[4 *  8];
         lpb1[4 *  9]  =  lpb2[4 *  9];
         lpb1[4 * 10]  =  lpb2[4 * 10];
         lpb1[4 * 11]  =  lpb2[4 * 11];
         lpb1[4 * 12]  =  lpb2[4 * 12];
         lpb1[4 * 13]  =  lpb2[4 * 13];
         lpb1[4 * 14]  =  lpb2[4 * 14];
         lpb1[4 * 15]  =  lpb2[4 * 15];
         lpb1[4 * 16]  =  lpb2[4 * 16];
         lpb1[4 * 17]  =  lpb2[4 * 17];
         lpb1[4 * 18]  =  lpb2[4 * 18];
         lpb1[4 * 19]  =  lpb2[4 * 19];
         lpb1[4 * 20]  =  lpb2[4 * 20];
         lpb1[4 * 21]  =  lpb2[4 * 21];
         lpb1[4 * 22]  =  lpb2[4 * 22];
         lpb1[4 * 23]  =  lpb2[4 * 23];
         lpb1[4 * 24]  =  lpb2[4 * 24];
         lpb1[4 * 25]  =  lpb2[4 * 25];
         lpb1[4 * 26]  =  lpb2[4 * 26];
         lpb1[4 * 27]  =  lpb2[4 * 27];
         lpb1[4 * 28]  =  lpb2[4 * 28];
         lpb1[4 * 29]  =  lpb2[4 * 29];
         lpb1[4 * 30]  =  lpb2[4 * 30];
         lpb1[4 * 31]  =  lpb2[4 * 31];

         lpb1[4 * 32]  =  lpb2[4 * 32];
         lpb1[4 * 33]  =  lpb2[4 * 33];
         lpb1[4 * 34]  =  lpb2[4 * 34];
         lpb1[4 * 35]  =  lpb2[4 * 35];
         lpb1[4 * 36]  =  lpb2[4 * 36];
         lpb1[4 * 37]  =  lpb2[4 * 37];
         lpb1[4 * 38]  =  lpb2[4 * 38];
         lpb1[4 * 39]  =  lpb2[4 * 39];
         lpb1[4 * 40]  =  lpb2[4 * 40];
         lpb1[4 * 41]  =  lpb2[4 * 41];
         lpb1[4 * 42]  =  lpb2[4 * 42];
         lpb1[4 * 43]  =  lpb2[4 * 43];
         lpb1[4 * 44]  =  lpb2[4 * 44];
         lpb1[4 * 45]  =  lpb2[4 * 45];
         lpb1[4 * 46]  =  lpb2[4 * 46];
         lpb1[4 * 47]  =  lpb2[4 * 47];
         lpb1[4 * 48]  =  lpb2[4 * 48];
         lpb1[4 * 49]  =  lpb2[4 * 49];
         lpb1[4 * 50]  =  lpb2[4 * 50];
         lpb1[4 * 51]  =  lpb2[4 * 51];
         lpb1[4 * 52]  =  lpb2[4 * 52];
         lpb1[4 * 53]  =  lpb2[4 * 53];
         lpb1[4 * 54]  =  lpb2[4 * 54];
         lpb1[4 * 55]  =  lpb2[4 * 55];
         lpb1[4 * 56]  =  lpb2[4 * 56];
         lpb1[4 * 57]  =  lpb2[4 * 57];
         lpb1[4 * 58]  =  lpb2[4 * 58];
         lpb1[4 * 59]  =  lpb2[4 * 59];
         lpb1[4 * 60]  =  lpb2[4 * 60];
         lpb1[4 * 61]  =  lpb2[4 * 61];
         lpb1[4 * 62]  =  lpb2[4 * 62];
         lpb1[4 * 63]  =  lpb2[4 * 63];

         lpb1 += 4 * 64;
         lpb2 += 4 * 64;
      }
      i *= 64;
      for(; i < size; i++)
      {
         *lpb1 = *lpb2;
         lpb1 += 4;
         lpb2 += 4;
      }
   }


   void dib::FillGlass ( int32_t R, int32_t G, int32_t B, int32_t A )
   {
      BYTE *dst=(BYTE*)get_data();
      int32_t size=cx*cy;

      while ( size-- )
      {
         dst[0]=(BYTE)(((B-dst[0])*A+(dst[0]<<8))>>8);
         dst[1]=(BYTE)(((G-dst[1])*A+(dst[1]<<8))>>8);
         dst[2]=(BYTE)(((R-dst[2])*A+(dst[2]<<8))>>8);
         dst+=4;
      }
   }

   void dib::FillStippledGlass ( int32_t R, int32_t G, int32_t B )
   {
      COLORREF color=RGB ( B, G, R );
      int32_t w=cx;
      int32_t h=cy;

      for ( int32_t j=0; j<w; j++ )
      {
         for ( int32_t i=0; i<h; i++ )
         {
            get_data()[j*w+i]=((i+j)&0x1) ? get_data()[j*w+i] : color;
         }
      }
   }

   void dib::copy(dib * dib)
   {
      // If DibSize Wrong Re-create dib
      if(dib->size() != size())
         dib->create(size());
      // do copy
      map();
      dib->map();
      memcpy ( dib->get_data(), get_data(), cx*cy*4 );
   }


   void dib::Paste(dib * dib)
   {

      if(dib->size() != size())
         dib->create(size());

      dib->defer_realize(get_graphics());
      defer_realize(dib->get_graphics());

      map();
      dib->map();
      // If DibSize Wrong Re-create dib
      // do Paste
      memcpy ( get_data(), dib->get_data(), cx*cy*4 );

   }

   bool dib::bitmap_blend(::draw2d::graphics * pgraphics, LPCRECT lprect)
   {
      rect rect(lprect);

      return System.visual().imaging().bitmap_blend(
         pgraphics,
         rect.top_left(),
         rect.size(),
         get_graphics(),
         null_point());
   }


   bool dib::color_blend(COLORREF cr, BYTE bAlpha)
   {

      BYTE *dst=(BYTE*)get_data();
      int32_t size=cx*cy;

      uint32_t dwB = rgba_get_b(cr);
      uint32_t dwG = rgba_get_g(cr);
      uint32_t dwR = rgba_get_r(cr);

      uint32_t dwB_ = dwB << 8;
      uint32_t dwG_ = dwG << 8;
      uint32_t dwR_ = dwR << 8;

      while ( size-- )
      {
         dst[0]=(BYTE)(((dst[0]-dwB)*bAlpha+dwB_)>>8);
         dst[1]=(BYTE)(((dst[1]-dwG)*bAlpha+dwG_)>>8);
         dst[2]=(BYTE)(((dst[2]-dwG)*bAlpha+dwR_)>>8);
         dst+=4;
      }
      return true;
   }


   void dib::Blend(dib * dib, int32_t A)
   {
      if(size() != dib->size() )
         return;

      BYTE *src=(BYTE*)dib->get_data();
      BYTE *dst=(BYTE*)get_data();
      int32_t size=cx*cy;

      while ( size-- )
      {
         dst[0]=(BYTE)(((src[0]-dst[0])*A+(dst[0]<<8))>>8);
         dst[1]=(BYTE)(((src[1]-dst[1])*A+(dst[1]<<8))>>8);
         dst[2]=(BYTE)(((src[2]-dst[2])*A+(dst[2]<<8))>>8);
         dst+=4;
         src+=4;
      }
   }

   bool dib::Blend(dib *pDib, dib *DibA, int32_t A)
   {
      if(size() != pDib->size() ||
         size() != DibA->size())
         return false;

      BYTE *src=(BYTE*)pDib->get_data();
      BYTE *dst=(BYTE*)get_data();
      BYTE *alf=(BYTE*)DibA->get_data();
      int32_t size=cx*cy;

      A = 2 - A;

      while ( size-- )
      {
         dst[0]=(BYTE)(((src[0]-dst[0])*alf[A]+(dst[0]<<8))>>8);
         dst[1]=(BYTE)(((src[1]-dst[1])*alf[A]+(dst[1]<<8))>>8);
         dst[2]=(BYTE)(((src[2]-dst[2])*alf[A]+(dst[2]<<8))>>8);
         dst+=4;
         src+=4;
         alf+=4;
      }

      return true;
   }

   bool dib::blend(dib * pdib, dib * pdibRate)
   {
      if(size() != pdib->size() ||
         size() != pdibRate->size())
         return false;

      BYTE *src=(BYTE*)pdib->get_data();
      BYTE *dst=(BYTE*)get_data();
      BYTE *alf=(BYTE*)pdibRate->get_data();
      int32_t size=cx*cy;

      while(size >= 2)
      {
         dst[00] = (BYTE) (((((int32_t)src[00] - (int32_t)dst[00]) * (int32_t)alf[00]) + (int32_t)dst[00] * (int32_t)255) / 255);
         dst[01] = (BYTE) (((((int32_t)src[01] - (int32_t)dst[01]) * (int32_t)alf[01]) + (int32_t)dst[01] * (int32_t)255) / 255);
         dst[02] = (BYTE) (((((int32_t)src[02] - (int32_t)dst[02]) * (int32_t)alf[02]) + (int32_t)dst[02] * (int32_t)255) / 255);
         dst[03] = (BYTE) (((((int32_t)src[03] - (int32_t)dst[03]) * (int32_t)alf[03]) + (int32_t)dst[03] * (int32_t)255) / 255);
         dst[04] = (BYTE) (((((int32_t)src[04] - (int32_t)dst[04]) * (int32_t)alf[04]) + (int32_t)dst[04] * (int32_t)255) / 255);
         dst[05] = (BYTE) (((((int32_t)src[05] - (int32_t)dst[05]) * (int32_t)alf[05]) + (int32_t)dst[05] * (int32_t)255) / 255);
         dst[06] = (BYTE) (((((int32_t)src[06] - (int32_t)dst[06]) * (int32_t)alf[06]) + (int32_t)dst[06] * (int32_t)255) / 255);
         dst[07] = (BYTE) (((((int32_t)src[07] - (int32_t)dst[07]) * (int32_t)alf[07]) + (int32_t)dst[07] * (int32_t)255) / 255);
         dst+=4 * 2;
         src+=4 * 2;
         alf+=4 * 2;
         size-=2;
      }
      while(size > 0)
      {
         dst[00] = (BYTE) (((((int32_t)src[00] - (int32_t)dst[00]) * (int32_t)alf[00]) + (int32_t)dst[00] * (int32_t)255) / 255);
         dst[01] = (BYTE) (((((int32_t)src[01] - (int32_t)dst[01]) * (int32_t)alf[01]) + (int32_t)dst[01] * (int32_t)255) / 255);
         dst[02] = (BYTE) (((((int32_t)src[02] - (int32_t)dst[02]) * (int32_t)alf[02]) + (int32_t)dst[02] * (int32_t)255) / 255);
         dst[03] = (BYTE) (((((int32_t)src[03] - (int32_t)dst[03]) * (int32_t)alf[03]) + (int32_t)dst[03] * (int32_t)255) / 255);
         dst+=4;
         src+=4;
         alf+=4;
         size--;
      }

      return true;
   }

   void dib::Darken ( dib *dib )
   {
      if ( size()!=dib->size() )
         return;

      BYTE *src=(BYTE*)dib->get_data();
      BYTE *dst=(BYTE*)get_data();
      int32_t size=cx*cy;

      while ( size-- )
      {
         dst[0]=(BYTE)((src[0]<dst[0]) ? src[0] : dst[0]);
         dst[1]=(BYTE)((src[1]<dst[1]) ? src[1] : dst[1]);
         dst[2]=(BYTE)((src[2]<dst[2]) ? src[2] : dst[2]);
         dst+=4;
         src+=4;
      }
   }

   void dib::Difference ( dib *dib )
   {
      if ( size()!=dib->size() )
         return;

      BYTE *src=(BYTE*)dib->get_data();
      BYTE *dst=(BYTE*)get_data();
      int32_t size=cx*cy;

      while ( size-- )
      {
         int32_t Difference;
         Difference=src[0]-dst[0];
         dst[0]=(BYTE)((Difference<0) ? -Difference : Difference);
         Difference=src[1]-dst[1];
         dst[1]=(BYTE)((Difference<0) ? -Difference : Difference);
         Difference=src[2]-dst[2];
         dst[2]=(BYTE)((Difference<0) ? -Difference : Difference);
         dst+=4;
         src+=4;
      }
   }

   void dib::Lighten ( dib *dib )
   {
      if ( size()!=dib->size() )
         return;

      BYTE *src=(BYTE*)dib->get_data();
      BYTE *dst=(BYTE*)get_data();
      int32_t size=cx*cy;

      while ( size-- )
      {
         dst[0]=(BYTE)((src[0]>dst[0]) ? src[0] : dst[0]);
         dst[1]=(BYTE)((src[1]>dst[1]) ? src[1] : dst[1]);
         dst[2]=(BYTE)((src[2]>dst[2]) ? src[2] : dst[2]);
         dst+=4;
         src+=4;
      }
   }

   void dib::Multiply ( dib *dib )
   {
      if ( size()!=dib->size() )
         return;

      BYTE *src=(BYTE*)dib->get_data();
      BYTE *dst=(BYTE*)get_data();
      int32_t size=cx*cy;

      while ( size-- )
      {
         dst[0]=(BYTE)(((src[0])*(dst[0]))>>8);
         dst[1]=(BYTE)(((src[1])*(dst[1]))>>8);
         dst[2]=(BYTE)(((src[2])*(dst[2]))>>8);
         dst+=4;
         src+=4;
      }
   }

   void dib::Screen ( dib *dib )
   {
      if ( size()!=dib->size() )
         return;

      BYTE *src=(BYTE*)dib->get_data();
      BYTE *dst=(BYTE*)get_data();
      int32_t size=cx*cy;

      while ( size-- )
      {
         dst[0]=(BYTE)(255-(((255-src[0])*(255-dst[0]))>>8));
         dst[1]=(BYTE)(255-(((255-src[1])*(255-dst[1]))>>8));
         dst[2]=(BYTE)(255-(((255-src[2])*(255-dst[2]))>>8));
         dst+=4;
         src+=4;
      }
   }

   //////////////////////////////////////////////////////////////////////
   // Rectangle Functions
   //////////////////////////////////////////////////////////////////////

   void dib::copy ( dib *dib, int32_t x, int32_t y )
   {
      // Clip Rect
      int32_t px=(x>=0) ? x : 0;
      int32_t py=(y>=0) ? y : 0;
      int32_t dx=((x+dib->cx)<cx) ? dib->cx : cx-x;
      int32_t dy=((y+dib->cy)<cy) ? dib->cy : cy-y;
      dx=(x>=0) ? dx : dx + x;
      dy=(y>=0) ? dy : dy + y;

      // If Nothing to copy return
      if ( (dx<=0) || (dy<=0) )
         return;
      // If DibSize Wrong Re-create dib
      if ( (dx!=dib->cx) || (dy!=dib->cy) )
         dib->create(dx, dy);

      map();
      dib->map();

      // Prepare buffer Addresses
      COLORREF *src=get_data()+(py*cx)+px;
      COLORREF *dst=dib->get_data();

      // Do copy
      while ( dy-- )
      {
         for ( int32_t i=0; i<dx; i++ )
            dst[i]=src[i];
         src+=cx;
         dst+=dib->cx;
      }

   }

   void dib::PasteRect ( dib *dib, int32_t x, int32_t y )
   {
      // Clip Rect
      int32_t px=(x>=0) ? x : 0;
      int32_t py=(y>=0) ? y : 0;
      int32_t dx=((x+dib->cx)<cx) ? dib->cx : cx-x;
      int32_t dy=((y+dib->cy)<cy) ? dib->cy : cy-y;
      dx=(x>=0) ? dx : dx + x;
      dy=(y>=0) ? dy : dy + y;

      // If Nothing to Paste return
      if ( (dx<=0) || (dy<=0) )
         return;

      // Prepare buffer Addresses
      COLORREF *src=dib->get_data()+((py-y)*dib->cx)+px-x;
      COLORREF *dst=get_data()+(py*cx)+px;

      // Do Paste
      while ( dy-- )
      {
         for ( int32_t i=0; i<dx; i++ )
            dst[i]=src[i];
         src+=dib->cx;
         dst+=cx;
      }
   }

   void dib::FillRect ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t R, int32_t G, int32_t B )
   {
      // Clip Rect
      int32_t px=(x>=0) ? x : 0;
      int32_t py=(y>=0) ? y : 0;
      int32_t dx=((x+w)<cx) ? w : cx-x;
      int32_t dy=((y+h)<cy) ? h : cy-y;
      dx=(x>=0) ? dx : dx + x;
      dy=(y>=0) ? dy : dy + y;

      // If Nothing to Fill return
      if ( (dx<=0) || (dy<=0) )
         return;

      // Prepare buffer Address
      COLORREF *dst=get_data()+(py*cx)+px;
      COLORREF color=RGB ( B, G, R );

      // Do Fill
      while ( dy-- )
      {
         for ( int32_t i=0; i<dx; i++ )
         {
            dst[i]=color;
         }
         dst+=cx;
      }
   }

   void dib::FillRect ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t A, int32_t R, int32_t G, int32_t B )
   {
      // Clip Rect
      int32_t px = x;
      if(w < 0)
      {
         px += w;
         w = -w;
      }

      px=(px>=0) ? px : 0;
      int32_t py = y;
      if(h < 0)
      {
         py += h;
         h = -h;
      }
      py = (py>=0) ? py : 0;
      int32_t dx;
      dx =((px+w)<cx) ? w : cx-px;
      int32_t dy;
      dy =((py+h)<cy) ? h : cy-py;
      dx=(px>=0) ? dx : dx + x;
      dy=(py>=0) ? dy : dy + y;

      // If Nothing to Fill return
      if ( (dx<=0) || (dy<=0) )
         return;

      // Prepare buffer Address
      COLORREF *dst=get_data()+(py*cx)+px;
      COLORREF color=ARGB (A, B, G, R );

      // Do Fill
      while ( dy-- )
      {
         for ( int32_t i=0; i<dx; i++ )
         {
            dst[i]=color;
         }
         dst+=cx;
      }
   }

   void dib::FillGlassRect ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t R, int32_t G, int32_t B, int32_t A )
   {
      // Clip Rect
      int32_t px=(x>=0) ? x : 0;
      int32_t py=(y>=0) ? y : 0;
      int32_t dx=((x+w)<cx) ? w : cx-x;
      int32_t dy=((y+h)<cy) ? h : cy-y;
      dx=(x>=0) ? dx : dx + x;
      dy=(y>=0) ? dy : dy + y;

      // If Nothing to FillGlass return
      if ( (dx<=0) || (dy<=0) )
         return;

      // Prepare buffer Address
      BYTE *dst=(BYTE *)get_data()+((py*cx)+px)*4;

      // Do FillGlass
      while ( dy-- )
      {
         for ( int32_t i=0; i<dx; i++ )
         {
            dst[0]=(BYTE)(((B-dst[0])*A+(dst[0]<<8))>>8);
            dst[1]=(BYTE)(((G-dst[1])*A+(dst[1]<<8))>>8);
            dst[2]=(BYTE)(((R-dst[2])*A+(dst[2]<<8))>>8);
            dst+=4;
         }
         dst+=(cx-dx)<<2;
      }
   }

   void dib::FillStippledGlassRect ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t R, int32_t G, int32_t B )
   {
      // Clip Rect
      int32_t px=(x>=0) ? x : 0;
      int32_t py=(y>=0) ? y : 0;
      int32_t dx=((x+w)<cx) ? w : cx-x;
      int32_t dy=((y+h)<cy) ? h : cy-y;
      dx=(x>=0) ? dx : dx + x;
      dy=(y>=0) ? dy : dy + y;

      // If Nothing to FillStippledGlass return
      if ( (dx<=0) || (dy<=0) )
         return;

      // Prepare buffer Address
      COLORREF *dst=get_data()+(py*cx)+px;
      COLORREF color=RGB ( B, G, R );

      // Do FillStippledGlass
      for ( int32_t j=0; j<dy; j++ )
      {
         for ( int32_t i=0; i<dx; i++ )
         {
            dst[i]=((i+j)&0x1) ? dst[i] : color;
         }
         dst+=cx;
      }
   }

   void dib::BlendRect ( dib *dib, int32_t x, int32_t y, int32_t A )
   {
      // Clip Rect
      int32_t px=(x>=0) ? x : 0;
      int32_t py=(y>=0) ? y : 0;
      int32_t dx=((x+dib->cx)<cx) ? dib->cx : cx-x;
      int32_t dy=((y+dib->cy)<cy) ? dib->cy : cy-y;
      dx=(x>=0) ? dx : dx + x;
      dy=(y>=0) ? dy : dy + y;

      // If Nothing to Blend return
      if ( (dx<=0) || (dy<=0) )
         return;

      // Prepare buffer Addresses
      BYTE *src=(BYTE *)dib->get_data()+(((py-y)*dib->cx)+px-x)*4;
      BYTE *dst=(BYTE *)get_data()+((py*cx)+px)*4;

      // Do Blend
      while ( dy-- )
      {
         for ( int32_t i=0; i<dx; i++ )
         {
            dst[0]=(BYTE)(((src[0]-dst[0])*A+(dst[0]<<8))>>8);
            dst[1]=(BYTE)(((src[1]-dst[1])*A+(dst[1]<<8))>>8);
            dst[2]=(BYTE)(((src[2]-dst[2])*A+(dst[2]<<8))>>8);
            dst+=4;
            src+=4;
         }
         dst+=(cx-dx)<<2;
         src+=(dib->cx-dx)<<2;
      }
   }

   void dib::DarkenRect ( dib *dib, int32_t x, int32_t y )
   {
      // Clip Rect
      int32_t px=(x>=0) ? x : 0;
      int32_t py=(y>=0) ? y : 0;
      int32_t dx=((x+dib->cx)<cx) ? dib->cx : cx-x;
      int32_t dy=((y+dib->cy)<cy) ? dib->cy : cy-y;
      dx=(x>=0) ? dx : dx + x;
      dy=(y>=0) ? dy : dy + y;

      // If Nothing to Darken return
      if ( (dx<=0) || (dy<=0) )
         return;

      // Prepare buffer Addresses
      BYTE *src=(BYTE *)dib->get_data()+(((py-y)*dib->cx)+px-x)*4;
      BYTE *dst=(BYTE *)get_data()+((py*cx)+px)*4;

      // Do Darken
      while ( dy-- )
      {
         for ( int32_t i=0; i<dx; i++ )
         {
            dst[0]=(BYTE)((src[0]<dst[0]) ? src[0] : dst[0]);
            dst[1]=(BYTE)((src[1]<dst[1]) ? src[1] : dst[1]);
            dst[2]=(BYTE)((src[2]<dst[2]) ? src[2] : dst[2]);
            dst+=4;
            src+=4;
         }
         dst+=(cx-dx)<<2;
         src+=(dib->cx-dx)<<2;
      }
   }

   void dib::DifferenceRect ( dib *dib, int32_t x, int32_t y )
   {
      // Clip Rect
      int32_t px=(x>=0) ? x : 0;
      int32_t py=(y>=0) ? y : 0;
      int32_t dx=((x+dib->cx)<cx) ? dib->cx : cx-x;
      int32_t dy=((y+dib->cy)<cy) ? dib->cy : cy-y;
      dx=(x>=0) ? dx : dx + x;
      dy=(y>=0) ? dy : dy + y;

      // If Nothing to Difference return
      if ( (dx<=0) || (dy<=0) )
         return;

      // Prepare buffer Addresses
      BYTE *src=(BYTE *)dib->get_data()+(((py-y)*dib->cx)+px-x)*4;
      BYTE *dst=(BYTE *)get_data()+((py*cx)+px)*4;

      // Do Difference
      while ( dy-- )
      {
         for ( int32_t i=0; i<dx; i++ )
         {
            int32_t Difference;
            Difference=src[0]-dst[0];
            dst[0]=(BYTE)((Difference<0) ? -Difference : Difference);
            Difference=src[1]-dst[1];
            dst[1]=(BYTE)((Difference<0) ? -Difference : Difference);
            Difference=src[2]-dst[2];
            dst[2]=(BYTE)((Difference<0) ? -Difference : Difference);
            dst+=4;
            src+=4;
         }
         dst+=(cx-dx)<<2;
         src+=(dib->cx-dx)<<2;
      }
   }

   void dib::LightenRect ( dib *dib, int32_t x, int32_t y )
   {
      // Clip Rect
      int32_t px=(x>=0) ? x : 0;
      int32_t py=(y>=0) ? y : 0;
      int32_t dx=((x+dib->cx)<cx) ? dib->cx : cx-x;
      int32_t dy=((y+dib->cy)<cy) ? dib->cy : cy-y;
      dx=(x>=0) ? dx : dx + x;
      dy=(y>=0) ? dy : dy + y;

      // If Nothing to Lighten return
      if ( (dx<=0) || (dy<=0) )
         return;

      // Prepare buffer Addresses
      BYTE *src=(BYTE *)dib->get_data()+(((py-y)*dib->cx)+px-x)*4;
      BYTE *dst=(BYTE *)get_data()+((py*cx)+px)*4;

      // Do Lighten
      while ( dy-- )
      {
         for ( int32_t i=0; i<dx; i++ )
         {
            dst[0]=(BYTE)((src[0]>dst[0]) ? src[0] : dst[0]);
            dst[1]=(BYTE)((src[1]>dst[1]) ? src[1] : dst[1]);
            dst[2]=(BYTE)((src[2]>dst[2]) ? src[2] : dst[2]);
            dst+=4;
            src+=4;
         }
         dst+=(cx-dx)<<2;
         src+=(dib->cx-dx)<<2;
      }
   }

   void dib::MultiplyRect ( dib *dib, int32_t x, int32_t y )
   {
      // Clip Rect
      int32_t px=(x>=0) ? x : 0;
      int32_t py=(y>=0) ? y : 0;
      int32_t dx=((x+dib->cx)<cx) ? dib->cx : cx-x;
      int32_t dy=((y+dib->cy)<cy) ? dib->cy : cy-y;
      dx=(x>=0) ? dx : dx + x;
      dy=(y>=0) ? dy : dy + y;

      // If Nothing to Multiply return
      if ( (dx<=0) || (dy<=0) )
         return;

      // Prepare buffer Addresses
      BYTE *src=(BYTE *)dib->get_data()+(((py-y)*dib->cx)+px-x)*4;
      BYTE *dst=(BYTE *)get_data()+((py*cx)+px)*4;

      // Do Multiply
      while ( dy-- )
      {
         for ( int32_t i=0; i<dx; i++ )
         {
            dst[0]=(BYTE)(((src[0])*(dst[0]))>>8);
            dst[1]=(BYTE)(((src[1])*(dst[1]))>>8);
            dst[2]=(BYTE)(((src[2])*(dst[2]))>>8);
            dst+=4;
            src+=4;
         }
         dst+=(cx-dx)<<2;
         src+=(dib->cx-dx)<<2;
      }
   }

   void dib::ScreenRect ( dib *dib, int32_t x, int32_t y )
   {
      // Clip Rect
      int32_t px=(x>=0) ? x : 0;
      int32_t py=(y>=0) ? y : 0;
      int32_t dx=((x+dib->cx)<cx) ? dib->cx : cx-x;
      int32_t dy=((y+dib->cy)<cy) ? dib->cy : cy-y;
      dx=(x>=0) ? dx : dx + x;
      dy=(y>=0) ? dy : dy + y;

      // If Nothing to Screen return
      if ( (dx<=0) || (dy<=0) )
         return;

      // Prepare buffer Addresses
      BYTE *src=(BYTE *)dib->get_data()+(((py-y)*dib->cx)+px-x)*4;
      BYTE *dst=(BYTE *)get_data()+((py*cx)+px)*4;

      // Do Screen
      while ( dy-- )
      {
         for ( int32_t i=0; i<dx; i++ )
         {
            dst[0]=(BYTE)(255-(((255-src[0])*(255-dst[0]))>>8));
            dst[1]=(BYTE)(255-(((255-src[1])*(255-dst[1]))>>8));
            dst[2]=(BYTE)(255-(((255-src[2])*(255-dst[2]))>>8));
            dst+=4;
            src+=4;
         }
         dst+=(cx-dx)<<2;
         src+=(dib->cx-dx)<<2;
      }
   }

   //////////////////////////////////////////////////////////////////////
   // Line Functions
   //////////////////////////////////////////////////////////////////////

   /*void dib::Line ( int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t R, int32_t G, int32_t B )
   {
   int32_t dx, dy, k1, k2, d, x, y;
   COLORREF color=RGB ( B, G, R );

   dx=x2-x1;
   dy=y2-y1;
   d=(dy<<1)-dx;
   k1=dy<<1;
   k2=(dy-dx)<<1;
   x=x1;
   y=y1;

   get_data()[y*cx+x]=color;
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
   get_data()[y*cx+x]=color;
   }
   }*/


   void dib::horizontal_line(int32_t y, int32_t R, int32_t G, int32_t B, int32_t A, int32_t x1, int32_t x2)
   {
      if(cx == 0)
         return;
      x1 %= cx;
      x2 %= cx;
      if(x2 < 0)
         x2 += cx;
      if(x1 < 0)
         x1 += cx;
      COLORREF color=RGB ( B, G, R ) | (A << 24);
      COLORREF * pdata = get_data() + y * cx;
      for(int32_t x = x1; x <= x2; x++)
      {
         *pdata = color;
         pdata++;
      }
   }

   void dib::Line ( int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t R, int32_t G, int32_t B )
   {
      int32_t d, x, y, ca2, ay, sx, sy, dx, dy;
      COLORREF color=RGB ( B, G, R );

      dx=x2-x1;
      ca2=abs ( dx )<<1;
      sx=(dx<0) ? -1 : 1;
      dy=y2-y1;
      ay=abs ( dy )<<1;
      sy=(dy<0) ? -1 : 1;
      x=x1;
      y=y1;

      if ( ca2>ay )
      {
         d=ay-(ca2>>1);
         while ( x!=x2 )
         {
            get_data()[y*cx+x]=color;
            if ( d>=0 )
            {
               y+=sy;
               d-=ca2;
            }
            x+=sx;
            d+=ay;
         }
      }
      else
      {
         d=ca2-(ay>>1);
         while ( y!=y2 )
         {
            get_data()[y*cx+x]=color;
            if ( d>=0 )
            {
               x+=sx;
               d-=ay;
            }
            y+=sy;
            d+=ca2;
         }
      }
   }

   void dib::LineGlass ( int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t R, int32_t G, int32_t B, int32_t A )
   {
      int32_t d, x, y, ca2, ay, sx, sy, dx, dy;
//      COLORREF color=RGB ( B, G, R );
      BYTE *dst=(BYTE *)get_data();

      dx=x2-x1;
      ca2=abs ( dx )<<1;
      sx=(dx<0) ? -1 : 1;
      dy=y2-y1;
      ay=abs ( dy )<<1;
      sy=(dy<0) ? -1 : 1;
      x=x1;
      y=y1;

      if ( ca2>ay )
      {
         d=ay-(ca2>>1);
         while ( x!=x2 )
         {
            dst[(y*cx+x)<<2]=(BYTE)(((B-dst[(y*cx+x)<<2])*A+(dst[(y*cx+x)<<2]<<8))>>8);
            dst[((y*cx+x)<<2)+1]=(BYTE)(((G-dst[((y*cx+x)<<2)+1])*A+(dst[((y*cx+x)<<2)+1]<<8))>>8);
            dst[((y*cx+x)<<2)+2]=(BYTE)(((R-dst[((y*cx+x)<<2)+2])*A+(dst[((y*cx+x)<<2)+2]<<8))>>8);
            if ( d>=0 )
            {
               y+=sy;
               d-=ca2;
            }
            x+=sx;
            d+=ay;
         }
      }
      else
      {
         d=ca2-(ay>>1);
         while ( y!=y2 )
         {
            dst[(y*cx+x)<<2]=(BYTE)(((B-dst[(y*cx+x)<<2])*A+(dst[(y*cx+x)<<2]<<8))>>8);
            dst[((y*cx+x)<<2)+1]=(BYTE)(((G-dst[((y*cx+x)<<2)+1])*A+(dst[((y*cx+x)<<2)+1]<<8))>>8);
            dst[((y*cx+x)<<2)+2]=(BYTE)(((R-dst[((y*cx+x)<<2)+2])*A+(dst[((y*cx+x)<<2)+2]<<8))>>8);
            if ( d>=0 )
            {
               x+=sx;
               d-=ay;
            }
            y+=sy;
            d+=ca2;
         }
      }
   }

   void dib::Mask(COLORREF crMask, COLORREF crInMask, COLORREF crOutMask)
   {
      COLORREF crFind = RGB(rgba_get_b(crMask), rgba_get_g(crMask), rgba_get_r(crMask));
      COLORREF crSet = RGB(rgba_get_b(crInMask), rgba_get_g(crInMask), rgba_get_r(crInMask));
      COLORREF crUnset  = RGB(rgba_get_b(crOutMask), rgba_get_g(crOutMask), rgba_get_r(crOutMask));

      int32_t size=cx*cy;

      for ( int32_t i=0; i<size; i++ )
         if(get_data()[i]== crFind)
            get_data()[i] = crSet;
         else
            get_data()[i] = crUnset;

   }

   void dib::transparent_color(color color)
   {
      COLORREF crFind = color.get_rgb();
      int64_t iSize = area();

      for (int32_t i=0; i<iSize; i++ )
         if((get_data()[i] & 0x00ffffff) == crFind)
            ((LPBYTE)&get_data()[i])[3] = 255;
         else
            ((LPBYTE)&get_data()[i])[3] = 0;
   }

   void dib::channel_mask(uchar uchFind, uchar uchSet, uchar uchUnset, visual::rgba::echannel echannel)
   {
      int32_t size = cx * cy;
      uchar * puch = (uchar * ) get_data();
      puch += ((int32_t) echannel) % 4;

      for(int32_t i = 0; i < size; i++)
      {
         if(*puch == uchFind)
            *puch = uchSet;
         else
            *puch = uchUnset;
         puch += 4;
      }
   }

   uint32_t dib::GetPixel(int32_t x, int32_t y)
   {
      uint32_t dw = *(get_data() + x + (cy - y - 1) * cx);
      return RGB(rgba_get_b(dw), rgba_get_g(dw), rgba_get_r(dw));
   }

   // too slow for animation on AMD XP gen_hon.
   // TOP SUGGESTION:
   // The gradient can´t have more then 256 levels of the most bright color
   // (white). So creating a radial fill of radius 256 and then using fasting
   // stretching algorithms is much faster than calculating radial fill.
   void dib::RadialFill(BYTE alpha, BYTE red, BYTE green, BYTE blue, int32_t xCenter, int32_t yCenter, int32_t iRadius)
   {
      if (iRadius == 0)
         return;
      /*if(version == 0)
      {

      int32_t iR = iRadius - 1;

      int32_t xL = xCenter - iR;
      int32_t xU = xCenter + iR;
      int32_t yL = yCenter - iR;
      int32_t yU = yCenter + iR;


      if(xL < 0) xL = 0;
      if(xU >= m_Size.cx) xU = m_Size.cx - 1;
      if(yL < 0) yL = 0;
      if(yU >= m_Size.cy) yU = m_Size.cy - 1;


      BYTE *dst = ((BYTE*)(get_data() + xL + yL * m_Size.cx));
      uint32_t dwAdd = ((m_Size.cx - 1 - xU) + xL) * 4;
      int32_t size=m_Size.cx*m_Size.cy;
      double iLevel;

      int32_t dx, dy;
      int32_t dx0, dy0;
      int32_t dx1, dy1;
      int32_t dx2, dy2;
      int32_t dx3, dy3;
      int32_t dx4, dy4;
      int32_t dx5, dy5;
      int32_t dx6, dy6;
      int32_t dx7, dy7;
      int32_t dx8, dy8;
      int32_t dx9, dy9;
      int32_t dxA, dyA;
      int32_t dxB, dyB;
      int32_t dxC, dyC;
      int32_t dxD, dyD;
      int32_t dxE, dyE;
      int32_t dxF, dyF;

      unsigned long dr;
      unsigned long dq;
      unsigned long dr0, dq0;
      unsigned long dr1, dq1;
      unsigned long dr2, dq2;
      unsigned long dr3, dq3;
      unsigned long dr4, dq4;
      unsigned long dr5, dq5;
      unsigned long dr6, dq6;
      unsigned long dr7, dq7;
      unsigned long dr8, dq8;
      unsigned long dr9, dq9;
      unsigned long drA, dqA;
      unsigned long drB, dqB;
      unsigned long drC, dqC;
      unsigned long drD, dqD;
      unsigned long drE, dqE;
      unsigned long drF, dqF;
      int32_t x, y;

      {
      for(y = yL; y <= yU; y++)
      {
      for(x = xL; x <= xU; x++)
      {
      dx = abs(x - xCenter);
      dy = abs(y - yCenter);
      isqrt((dx * dx) + (dy * dy), &dr, &dq);
      if(dr < iRadius)
      {
      iLevel = 1.0 - dr * 1.0 / iRadius;
      dst[0] = blue  * iLevel;
      dst[1] = green * iLevel;
      dst[2] = red   * iLevel;
      }
      dst += 4;
      }
      dst += dwAdd;
      }
      }
      }
      else if(version == 1)*/
      {

         LPBYTE lpbAlloc = (LPBYTE) malloc((iRadius * iRadius) + 4);
         LPBYTE lpb = ((LPBYTE) NULL) +  (*((uint_ptr *) &lpbAlloc[3]) & ~3);


         int32_t x, y;
         int32_t b;

//         int32_t r2 = iRadius * iRadius;

         for(y = 0; y < iRadius; y++)
         {
            for(x = y; x < iRadius; x++)
            {
               b = (int32_t) (sqrt((double) (x * x) + (y * y)) * 255 / iRadius);
               if(b > 255)
                  b = 0;
               else
                  b = 255 - b;


               lpb[x + y * iRadius] = (byte) b;
               lpb[y + x * iRadius] = (byte) b;
            }
         }


         int32_t iR = iRadius - 1;

         int32_t xL = xCenter - iR;
         int32_t xU = xCenter + iR;
         int32_t yL = yCenter - iR;
         int32_t yU = yCenter + iR;


         if(xL < 0) xL = 0;
         if(xU >= cx) xU = cx - 1;
         if(yL < 0) yL = 0;
         if(yU >= cy) yU = cy - 1;


         BYTE *dst = ((BYTE*)(get_data() + xL + yL * cx));
         uint32_t dwAdd = ((cx - 1 - xU) + xL) * 4;
//         int32_t size=cx*cy;

         int32_t dx, dy;

         // Top Left

         for(y = yL; y <= yU; y++)
         {
            for(x = xL; x <= xU; x++)
            {
               dx = abs(x - xCenter);
               dy = abs(y - yCenter);
               b = lpb[dx + dy * iRadius];
               dst[0] = byte(blue     * b / 255);
               dst[1] = byte(green    * b / 255);
               dst[2] = byte(red      * b / 255);
               dst[3] = byte(alpha    * b / 255);
               dst += 4;
            }
            dst += dwAdd;
         }

         free(lpbAlloc);
      }
   }


   void dib::RadialFill(
      BYTE alpha1, BYTE red1, BYTE green1, BYTE blue1,
      BYTE alpha2, BYTE red2, BYTE green2, BYTE blue2,
      int32_t xCenter, int32_t yCenter, int32_t iRadius)
   {
      if (iRadius == 0)
         return;
      /*if(version == 0)
      {

      int32_t iR = iRadius - 1;

      int32_t xL = xCenter - iR;
      int32_t xU = xCenter + iR;
      int32_t yL = yCenter - iR;
      int32_t yU = yCenter + iR;


      if(xL < 0) xL = 0;
      if(xU >= m_Size.cx) xU = m_Size.cx - 1;
      if(yL < 0) yL = 0;
      if(yU >= m_Size.cy) yU = m_Size.cy - 1;


      BYTE *dst = ((BYTE*)(get_data() + xL + yL * m_Size.cx));
      uint32_t dwAdd = ((m_Size.cx - 1 - xU) + xL) * 4;
      int32_t size=m_Size.cx*m_Size.cy;
      double iLevel;

      int32_t dx, dy;
      int32_t dx0, dy0;
      int32_t dx1, dy1;
      int32_t dx2, dy2;
      int32_t dx3, dy3;
      int32_t dx4, dy4;
      int32_t dx5, dy5;
      int32_t dx6, dy6;
      int32_t dx7, dy7;
      int32_t dx8, dy8;
      int32_t dx9, dy9;
      int32_t dxA, dyA;
      int32_t dxB, dyB;
      int32_t dxC, dyC;
      int32_t dxD, dyD;
      int32_t dxE, dyE;
      int32_t dxF, dyF;

      unsigned long dr;
      unsigned long dq;
      unsigned long dr0, dq0;
      unsigned long dr1, dq1;
      unsigned long dr2, dq2;
      unsigned long dr3, dq3;
      unsigned long dr4, dq4;
      unsigned long dr5, dq5;
      unsigned long dr6, dq6;
      unsigned long dr7, dq7;
      unsigned long dr8, dq8;
      unsigned long dr9, dq9;
      unsigned long drA, dqA;
      unsigned long drB, dqB;
      unsigned long drC, dqC;
      unsigned long drD, dqD;
      unsigned long drE, dqE;
      unsigned long drF, dqF;
      int32_t x, y;

      {
      for(y = yL; y <= yU; y++)
      {
      for(x = xL; x <= xU; x++)
      {
      dx = abs(x - xCenter);
      dy = abs(y - yCenter);
      isqrt((dx * dx) + (dy * dy), &dr, &dq);
      if(dr < iRadius)
      {
      iLevel = 1.0 - dr * 1.0 / iRadius;
      dst[0] = blue  * iLevel;
      dst[1] = green * iLevel;
      dst[2] = red   * iLevel;
      }
      dst += 4;
      }
      dst += dwAdd;
      }
      }
      }
      else if(version == 1)*/
      {

         LPBYTE lpbAlloc = (LPBYTE) malloc((iRadius * iRadius) + 4);
         LPBYTE lpb = ((LPBYTE) NULL) +  (*((uint_ptr *) &lpbAlloc[3]) & ~3);


         int32_t x, y;
         int32_t b;

//         int32_t r2 = iRadius * iRadius;

         for(y = 0; y < iRadius; y++)
         {
            for(x = y; x < iRadius; x++)
            {
               b = (int32_t) (sqrt((double) (x * x) + (y * y)) * 255 / iRadius);
               if(b > 255)
                  b = 0;
               else
                  b = ~b;


               lpb[x + y * iRadius] = (byte) b;
               lpb[y + x * iRadius] = (byte) b;
            }
         }


         int32_t iR = iRadius - 1;

         int32_t xL = xCenter - iR;
         int32_t xU = xCenter + iR;
         int32_t yL = yCenter - iR;
         int32_t yU = yCenter + iR;


         if(xL < 0) xL = 0;
         if(xU >= cx) xU = cx - 1;
         if(yL < 0) yL = 0;
         if(yU >= cy) yU = cy - 1;


         BYTE *dst = ((BYTE*)(get_data() + xL + yL * cx));
         uint32_t dwAdd = ((cx - 1 - xU) + xL) * 4;
//         int32_t size=cx*cy;

         int32_t dx, dy;

         BYTE bComp;

         // Top Left

         for(y = yL; y <= yU; y++)
         {
            for(x = xL; x <= xU; x++)
            {
               dx = abs(x - xCenter);
               dy = abs(y - yCenter);
               b = lpb[dx + dy * iRadius];
               bComp = (byte) ~b;
               dst[0] = byte(((blue1  * b) + (blue2  * bComp)) / 255);
               dst[1] = byte(((green1 * b) + (green2 * bComp)) / 255);
               dst[2] = byte(((red1   * b) + (red2   * bComp)) / 255);
               dst[3] = byte(((alpha1 * b) + (alpha2 * bComp)) / 255);
               dst += 4;
            }
            dst += dwAdd;
         }

         free(lpbAlloc);
      }
   }



   void dib::SetIconMask(::visual::icon * picon, int32_t cx, int32_t cy)
   {

      throw todo(get_app());

      // xxx todo create(cx, cy);

      if(cx <= 0 || cy <= 0)
         return;




      // White blend dib
      dib dib1;

      throw todo(get_app());

      // xxx todo dib1.create(cx, cy);

      dib1.Fill(0, 255, 255, 255);

#ifdef METROWIN

      throw not_implemented(get_app());

#else

      dib1.get_graphics()->DrawIcon(
         0, 0,
         picon,
         cx, cy,
         0,
         NULL,
         DI_IMAGE | DI_MASK);

#endif

      // Black blend dib
      ::draw2d::dib_sp spdib2(allocer());


      throw todo(get_app());

      // xxx todo spdib2->create(cx, cy);
      spdib2->Fill(0, 0, 0, 0);

#ifdef METROWIN

      throw not_implemented(get_app());

#else

      spdib2->get_graphics()->DrawIcon(
         0, 0,
         picon,
         cx, cy,
         0,
         NULL,
         DI_IMAGE | DI_MASK);

#endif

      // Mask dib
      dib dibM;
      throw todo(get_app());

      // xxx todo dibM.create(cx, cy);

#ifdef METROWIN

      throw not_implemented(get_app());

#else

      dibM.get_graphics()->DrawIcon(
         0, 0,
         picon,
         cx, cy,
         0,
         NULL,
         DI_MASK);

#endif

      BYTE * r1=(BYTE*)dib1.get_data();
      BYTE * r2=(BYTE*)spdib2->get_data();
      BYTE * srcM=(BYTE*)dibM.get_data();
      BYTE * dest=(BYTE*)get_data();
      int32_t iSize = cx*cy;

      BYTE b;
      BYTE bMax;
      while ( iSize-- > 0)
      {
         if(srcM[0] == 255)
         {
            bMax = 0;
         }
         else
         {
            bMax = 0;
            b =(BYTE)(r1[0]  - r2[0]);
            bMax = max(b, bMax);
            b =(BYTE)(r1[1]  - r2[1]);
            bMax = max(b, bMax);
            b =(BYTE)(r1[2]  - r2[2]);
            bMax = max(b, bMax);
            bMax = 255 - bMax;
         }
         dest[0]  =  bMax;
         dest[1]  =  bMax;
         dest[2]  =  bMax;
         dest     += 4;
         srcM     += 4;
         r1       += 4;
         r2       += 4;
      }



   }

   void dib::rotate(dib * pdib, double dAngle, double dScale)
   {
      // ::draw2d::dib_sp spdib(allocer());
      //   spdib->Paste(this);

      int32_t l = max(cx, cy);


      int32_t jmax = min(l, cy / 2);
      int32_t jmin = - jmax;
      int32_t imax = min(l, cx / 2);
      int32_t imin = - imax;


      int32_t joff = cy / 2;
      int32_t ioff = cx / 2;

      //int32_t iAngle = iStep % 360;
      //int32_t iAngle = iStep;
      //int32_t iAngle = 1;
      //int32_t k = 0;

      /*     for ( int32_t j=jmin; j<jmax; j++ )
      {
      for ( int32_t i=imin; i<imax; i++ )
      {
      int32_t x, y;

      // A Combination of a 2d Translation/rotation/Scale Matrix
      x=int32_t(cos10(i, iAngle) - sin10(j, iAngle)) + ioff;
      y=int32_t(sin10(i, iAngle) + cos10(j, iAngle)) + joff;
      get_data()[(j+joff)*cx+(i+ioff)]=
      spdib->get_data()[abs(y%cy)*cx+abs(x%cx)];
      //k++;
      }
      (j+joff)*cx+(i+ioff)
      }*/

      int32_t k = 0;
      double dCos = ::cos(dAngle * pi() / 180.0) * dScale;
      double dSin = ::sin(dAngle * pi() / 180.0) * dScale;
      int32_t cx1 = cx - 1;
      int32_t cy1 = cy - 1;
      for ( int32_t j=jmin; j<jmax; j++ )
      {
         for ( int32_t i=imin; i<imax; i++ )
         {
            int32_t x, y;

            // A Combination of a 2d Translation/rotation/Scale Matrix
            //x=abs((int32_t(dCos * i - dSin * j) + ioff) % cx);
            //y=abs((int32_t(dSin * i + dCos * j) + joff) % cy);

            x = (int32_t) abs((dCos * i - dSin * j) + ioff);
            y = (int32_t) abs((dSin * i + dCos * j) + joff);

            if((x / cx) % 2 == 0)
            {
               x %= cx;
            }
            else
            {
               x = cx1 - (x % cx);
            }

            if((y / cy) % 2 == 0)
            {
               y %= cy;
            }
            else
            {
               y = cy1 - (y % cy);
            }



            get_data()[(j+joff)*cx+(i+ioff)]=
               pdib->get_data()[y * cx + x];
            k++;
         }
      }
   }


   void dib::Rotate034(dib * pdib, double dAngle, double dScale)
   {

      int32_t l = max(cx, cy);


      int32_t jmax = min(l, cy / 2);
      int32_t jmin = - jmax;
      int32_t imax = min(l, cx / 2);
      int32_t imin = - imax;


      if((cy % 2) == 1)
         jmax++;

      if((cx % 2) == 1)
         imax++;

      int32_t joff = cy / 2;
      int32_t ioff = cx / 2;


      int32_t k = 0;
      double dCos = ::cos(dAngle * pi() / 180.0) * dScale;
      double dSin = ::sin(dAngle * pi() / 180.0) * dScale;
      int32_t cx1 = cx - 1;
      int32_t cy1 = cy - 1;
      for ( int32_t j=jmin; j<jmax; j++ )
      {
         for ( int32_t i=imin; i<imax; i++ )
         {
            int32_t x, y;

            // A Combination of a 2d Translation/rotation/Scale Matrix
            //x=abs((int32_t(dCos * i - dSin * j) + ioff) % cx);
            //y=abs((int32_t(dSin * i + dCos * j) + joff) % cy);

            x = (int32_t) abs((dCos * i - dSin * j) + ioff);
            y = (int32_t) abs((dSin * i + dCos * j) + joff);

            if((x / cx) % 2 == 0)
            {
               x %= cx;
            }
            else
            {
               x = cx1 - (x % cx);
            }

            if((y / cy) % 2 == 0)
            {
               y %= cy;
            }
            else
            {
               y = cy1 - (y % cy);
            }



            get_data()[(j+joff)*cx+(i+ioff)]=
               pdib->get_data()[y * cx + x];
            k++;
         }
      }
   }

   void dib::rotate(dib * pdib, LPCRECT lpcrect, double dAngle, double dScale)
   {
      // ::draw2d::dib_sp spdib(allocer());
      //   spdib->Paste(this);



      rect rect(lpcrect);

      int32_t cx = rect.width();
      int32_t cy = rect.height();

      int32_t l = max(cx, cy);

      int32_t jmax = min(l, cy / 2);
      int32_t jmin = - jmax;
      int32_t imax = min(l, cx / 2);
      int32_t imin = - imax;


      int32_t joff = cy / 2 + rect.left;
      int32_t ioff = cx / 2 + rect.top;

      //int32_t iAngle = iStep % 360;
      //int32_t iAngle = iStep;
      //int32_t iAngle = 1;
      //int32_t k = 0;

      /*     for ( int32_t j=jmin; j<jmax; j++ )
      {
      for ( int32_t i=imin; i<imax; i++ )
      {
      int32_t x, y;

      // A Combination of a 2d Translation/rotation/Scale Matrix
      x=int32_t(cos10(i, iAngle) - sin10(j, iAngle)) + ioff;
      y=int32_t(sin10(i, iAngle) + cos10(j, iAngle)) + joff;
      get_data()[(j+joff)*cx+(i+ioff)]=
      spdib->get_data()[abs(y%cy)*cx+abs(x%cx)];
      //k++;
      }
      (j+joff)*cx+(i+ioff)
      }*/

      int32_t k = 0;
      double dCos = ::cos(dAngle * pi() / 180.0) * dScale;
      double dSin = ::sin(dAngle * pi() / 180.0) * dScale;
      int32_t cx1 = cx - 1;
      int32_t cy1 = cy - 1;
      for ( int32_t j=jmin; j<jmax; j++ )
      {
         for ( int32_t i=imin; i<imax; i++ )
         {
            int32_t x, y;

            // A Combination of a 2d Translation/rotation/Scale Matrix
            //x=abs((int32_t(dCos * i - dSin * j) + ioff) % cx);
            //y=abs((int32_t(dSin * i + dCos * j) + joff) % cy);

            x = (int32_t) abs((dCos * i - dSin * j) + ioff);
            y = (int32_t) abs((dSin * i + dCos * j) + joff);

            if((x / cx) % 2 == 0)
            {
               x %= cx;
            }
            else
            {
               x = cx1 - (x % cx);
            }

            if((y / cy) % 2 == 0)
            {
               y %= cy;
            }
            else
            {
               y = cy1 - (y % cy);
            }



            get_data()[(j+joff)*cx+(i+ioff)]=
               pdib->get_data()[y * cx + x];
            k++;
         }
      }
   }

   /*int32_t dib::cos(int32_t i, int32_t iAngle)
   {
   return (int32_t) (((_int64) i * CosN[iAngle]) >> 32);
   }

   int32_t dib::sin(int32_t i, int32_t iAngle)
   {
   return (int32_t) (((_int64) i * SinN[iAngle]) >> 32);
   }*/


   void dib::FillByte(uchar uch)
   {

      if(area() <= 0 || get_data() == NULL)
         return;

      memset(get_data(), uch, (::primitive::memory_size) (area() * sizeof(COLORREF)));

   }

   void dib::Fill(int32_t level)
   {

      if(level == 0)
      {

         zero(m_pcolorref, (size_t) area() * sizeof(COLORREF));

      }
      else
      {

#ifdef WINDOWS

         FillMemory(m_pcolorref, (size_t) area() * sizeof(COLORREF), level);

#else

         memset(m_pcolorref, level, (size_t) area() * sizeof(COLORREF));

#endif

      }

   }

   void dib::Fill (int32_t A, int32_t R, int32_t G, int32_t B )
   {
      COLORREF color = RGB ( B, G, R ) | (A << 24);
      int32_t size=cx*cy;

      COLORREF * pcr;

      int32_t iSize32 = size / 32;
      int32_t i;
      for (i=0; i < iSize32; i+=32 )
      {
         pcr = &get_data()[i];
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
         get_data()[i]=color;
      }

   }

   COLORREF dib::GetAverageColor()
   {
      double dR = 0.0;
      double dG = 0.0;
      double dB = 0.0;
      int32_t iRLine;
      int32_t iGLine;
      int32_t iBLine;
      double dDiv = cx * cy;
      if(dDiv > 0)
      {
         LPBYTE lpb = (LPBYTE) get_data();
         for (int32_t y = 0; y < cy; y++)
         {
            iRLine = 0;
            iGLine = 0;
            iBLine = 0;
            for (int32_t x = 0; x < cx; x++)
            {
               iRLine += lpb[2];
               iGLine += lpb[1];
               iBLine += lpb[0];
               lpb += 4;
            }
            dR += iRLine / dDiv;
            dG += iGLine / dDiv;
            dB += iBLine / dDiv;
         }
         int32_t iR = (int32_t) dR;
         int32_t iG = (int32_t) dG;
         int32_t iB = (int32_t) dB;
         return RGB(iR, iG, iB);
      }
      else
      {
         return 0;
      }

   }


   void dib::do_xor(dib * pdib)
   {
      if(cx != pdib->cx
         || cy != pdib->cy)
      {
         return;
      }
      int32_t iCount = cx * cy;
      LPDWORD lpd1 = (LPDWORD) get_data();
      LPDWORD lpd2 = (LPDWORD) pdib->get_data();
      for(int32_t i = 0; i < iCount; i++)
      {
         *lpd1 = *lpd1 ^ *lpd2;
         lpd1++;
         lpd2++;
      }
   }

   void dib::create_frame(::size size, int32_t iFrameCount)
   {
      int32_t iSliceCount = (int32_t) sqrt((double) iFrameCount);
      int32_t iFrameWidth = size.cx / iSliceCount;
      int32_t iFrameHeight = size.cy / iSliceCount;
      create(iFrameWidth, iFrameHeight);
   }

   void dib::set_frame1(void * lpdata, int32_t iFrame, int32_t iFrameCount)
   {
      int32_t iSliceCount = (int32_t) sqrt((double) iFrameCount);
      if(iSliceCount == 0)
         iSliceCount = 1;
      int32_t iFrameWidth = cx / iSliceCount;
      int32_t iFrameHeight = cy / iSliceCount;
      int32_t iX = iFrame % iSliceCount;
      int32_t iY = iFrame / iSliceCount;
      COLORREF * lpDest = &get_data()[iFrameWidth * iX + iY * iFrameHeight * cx];
      COLORREF * lpSrc = (COLORREF *) lpdata;
      COLORREF * lpDestLine;
      for(int32_t y = 0; y < iFrameHeight; y++)
      {
         lpDestLine = &lpDest[y * cx];
         for(int32_t x = 0; x < iFrameWidth; x++)
         {
            *lpDestLine = *lpSrc;
            lpDestLine++;
            lpSrc++;
         }
      }
   }

   void dib::set_frame2(void * lpdata, int32_t iFrame, int32_t iFrameCount)
   {
      int32_t iSliceCount = (int32_t) sqrt((double) iFrameCount);
      if(iSliceCount == 0)
         iSliceCount = 1;
      int32_t iFrameWidth = cx / iSliceCount;
      int32_t iFrameHeight = cy / iSliceCount;
      int32_t iX = iFrame % iSliceCount;
      int32_t iY = iFrame / iSliceCount;
      COLORREF * lpDest = &get_data()[iFrameWidth * iX + iY * iFrameHeight * cx];
      COLORREF * lpSrc = (COLORREF *) lpdata;
      COLORREF * lpDestLine;
      for(int32_t y = iFrameHeight - 1; y >= 0; y--)
      {
         lpDestLine = &lpDest[y * cx];
         for(int32_t x = 0; x < iFrameWidth; x++)
         {
            *lpDestLine = *lpSrc;
            lpDestLine++;
            lpSrc++;
         }
      }
   }

   void dib::xor_dib_frame2(void * lpdata, int32_t iFrame, int32_t iFrameCount)
   {
      int32_t iSliceCount = (int32_t) sqrt((double) iFrameCount);
      if(iSliceCount == 0)
         iSliceCount = 1;
      int32_t iFrameWidth = cx / iSliceCount;
      int32_t iFrameHeight = cy / iSliceCount;
      int32_t iX = iFrame % iSliceCount;
      int32_t iY = iFrame / iSliceCount;
      COLORREF * lpDest = &get_data()[iFrameWidth * iX + iY * iFrameHeight * cx];
      COLORREF * lpSrc = (COLORREF *) lpdata;
      COLORREF * lpDestLine;
      for(int32_t y = iFrameHeight - 1; y >= 0; y--)
      {
         lpDestLine = &lpDest[y * cx];
         for(int32_t x = 0; x < iFrameWidth; x++)
         {
            *lpDestLine ^= *lpSrc;
            lpDestLine++;
            lpSrc++;
         }
      }
   }

   void dib::get_frame(void * lpdata, int32_t iFrame, int32_t iFrameCount)
   {
      int32_t iSliceCount = (int32_t) sqrt((double) iFrameCount);
      int32_t iFrameWidth = cx / iSliceCount;
      int32_t iFrameHeight = cy / iSliceCount;
      int32_t iX = iFrame % iSliceCount;
      int32_t iY = iFrame / iSliceCount;
      COLORREF * lpSrc = &get_data()[iFrameWidth * iX + iY * iFrameHeight *  cx];
      COLORREF * lpDest = (COLORREF *) lpdata;
      COLORREF * lpSrcLine;
      for(int32_t y = 0; y < iFrameHeight; y++)
      {
         lpSrcLine = &lpSrc[y * cx];
         for(int32_t x = 0; x < iFrameWidth; x++)
         {
            *lpDest = *lpSrcLine;
            lpDest++;
            lpSrcLine++;
         }
      }
   }

   bool dib::is_rgb_black()
   {
      int32_t iSize = cx * cy;
      COLORREF * lp = get_data();
      for(int32_t i = 0; i < iSize; i++)
      {
         if((*lp & 0x00FFFFFF) != 0)
            return false;
         lp++;
      }
      return true;
   }

   void dib::DivideRGB(int32_t iDivide)
   {
      if(iDivide == 0)
      {
         return;
      }
      int32_t iCount = cx * cy;
      LPBYTE lp = ((LPBYTE) get_data());
      int32_t i = 0;
      int32_t iCount1 = iCount - iCount % 8;
      for(; i < iCount1; i++)
      {
         lp[0] /= (byte) iDivide;
         lp[1] /= (byte) iDivide;
         lp[2] /= (byte) iDivide;

         lp[4] /= (byte) iDivide;
         lp[5] /= (byte) iDivide;
         lp[6] /= (byte) iDivide;

         lp[8] /= (byte) iDivide;
         lp[9] /= (byte) iDivide;
         lp[10] /= (byte) iDivide;

         lp[12] /= (byte) iDivide;
         lp[13] /= (byte) iDivide;
         lp[14] /= (byte) iDivide;

         lp[16] /= (byte) iDivide;
         lp[17] /= (byte) iDivide;
         lp[28] /= (byte) iDivide;

         lp[20] /= (byte) iDivide;
         lp[21] /= (byte) iDivide;
         lp[22] /= (byte) iDivide;

         lp[24] /= (byte) iDivide;
         lp[25] /= (byte) iDivide;
         lp[26] /= (byte) iDivide;

         lp[28] /= (byte) iDivide;
         lp[29] /= (byte) iDivide;
         lp[30] /= (byte) iDivide;

         lp += 4 * 8;
      }
      for(; i < iCount; i++)
      {
         lp[0] /= (byte) iDivide;
         lp[1] /= (byte) iDivide;
         lp[2] /= (byte) iDivide;
         lp +=4;
      }
   }

   void dib::DivideARGB(int32_t iDivide)
   {
      if(iDivide == 0)
      {
         return;
      }
      int32_t iCount = cx * cy;
      LPBYTE lp = ((LPBYTE) get_data());
      for(int32_t i = 0; i < iCount; i++)
      {
         lp[0] /= (byte) iDivide;
         lp[1] /= (byte) iDivide;
         lp[2] /= (byte) iDivide;
         lp[3] /= (byte) iDivide;
         lp +=4;
      }
   }

   void dib::DivideA(int32_t iDivide)
   {
      if(iDivide == 0)
      {
         return;
      }
      int32_t iCount = cx * cy;
      LPBYTE lp = ((LPBYTE) get_data());
      for(int32_t i = 0; i < iCount; i++)
      {
         lp[3] /= (byte) iDivide;
         lp +=4;
      }
   }


   void dib::stretch_dib(dib * pdib)
   {
      UNREFERENCED_PARAMETER(pdib);
      throw interface_only_exception(get_app());
   }

   int32_t dib::cos(int32_t i, int32_t iAngle)
   {
      UNREFERENCED_PARAMETER(i);
      UNREFERENCED_PARAMETER(iAngle);
      throw interface_only_exception(get_app());
   }

   int32_t dib::sin(int32_t i, int32_t iAngle)
   {
      UNREFERENCED_PARAMETER(i);
      UNREFERENCED_PARAMETER(iAngle);
      throw interface_only_exception(get_app());
   }

   int32_t dib::cos10(int32_t i, int32_t iAngle)
   {
      UNREFERENCED_PARAMETER(i);
      UNREFERENCED_PARAMETER(iAngle);
      throw interface_only_exception(get_app());
   }

   int32_t dib::sin10(int32_t i, int32_t iAngle)
   {
      UNREFERENCED_PARAMETER(i);
      UNREFERENCED_PARAMETER(iAngle);
      throw interface_only_exception(get_app());
   }

/*   int32_t dib::cx
   {
      throw interface_only_exception(get_app());
   }

   int32_t dib::cy
   {
      throw interface_only_exception(get_app());
   }

   int64_t dib::area()
   {
      return ((int64_t) cx) * ((int64_t)cy);
   }

   size dib::size()
   {
      return size64(cx, cy);
   }*/

   double dib::pi()
   {
      return atan(1.0)*4.0;
   }


   void dib::fill_channel(int32_t C, visual::rgba::echannel echannel)
   {
      UNREFERENCED_PARAMETER(C);
      UNREFERENCED_PARAMETER(echannel);
      throw interface_only_exception(get_app());
   }


   void dib::channel_copy(visual::rgba::echannel echannelDst, visual::rgba::echannel echannelSrc)
   {

      map();

      echannelDst = (visual::rgba::echannel) (((int32_t) echannelDst) % 4);
      echannelSrc = (visual::rgba::echannel) (((int32_t) echannelSrc) % 4);

      if(echannelDst == echannelSrc)
         return;

      byte * pdataDst = (byte *) get_data() + ((int32_t)echannelDst);

      byte * pdataSrc = (byte *) get_data() + ((int32_t)echannelSrc);

      for(int32_t y = 0; y < cy; y++)
      {

         byte * pdst = &pdataDst[scan * y];

         byte * psrc = &pdataSrc[scan * y];

         for(int32_t x = 0; x < cx; x++)
         {

            *pdst = *psrc;

            pdst += 4;

            psrc += 4;

         }
      }
   }

   void dib::channel_copy(visual::rgba::echannel echannelDst, visual::rgba::echannel echannelSrc, draw2d::dib * pdib)
   {

      if(size() != pdib->size())
         return;

      map();

      if(m_pcolorref == NULL)
         return;

      pdib->map();

      if(pdib->m_pcolorref == NULL)
         return;

      echannelDst = (visual::rgba::echannel) (((int32_t) echannelDst) % 4);
      echannelSrc = (visual::rgba::echannel) (((int32_t) echannelSrc) % 4);

      byte * pdataDst = (byte *) get_data() + ((int32_t)echannelDst);

      byte * pdataSrc = (byte *) pdib->get_data() + ((int32_t)echannelDst);

      for(int32_t y = 0; y < cy; y++)
      {

         byte * pdst = &pdataDst[scan * y];

         byte * psrc = &pdataSrc[pdib->scan * y];

         for(int32_t x = 0; x < cx; x++)
         {

            *pdst = *psrc;

            pdst += 4;

            psrc += 4;

         }
      }
   }

   void dib::write(::file::byte_output_stream & ostream)
   {
      ostream << (int32_t) cx;
      ostream << (int32_t) cy;
      if(area() <= 0)
         return;
      map();
      int wc = cx * sizeof(COLORREF);
      for(int32_t i = 0; i < cy; i++)
      {
         ostream.write(&((byte *) get_data())[scan * i], wc);
      }
   }

   void dib::read(::file::byte_input_stream & istream)
   {
      int32_t width;
      int32_t height;
      istream >> width;
      if(width < 0)
         return;
      istream >> height;
      if(height < 0)
         return;
      if((width * height) <= 0)
         return;
      if(!create(width, height))
         throw 0;
      map();
      int wc = width * sizeof(COLORREF);
      for(int32_t i = 0; i < cy; i++)
      {
         istream.read(&((byte *) get_data())[scan * i], wc);
      }
   }

   void dib::set_rgb ( int32_t R, int32_t G, int32_t B )
   {
      UNREFERENCED_PARAMETER(R);
      UNREFERENCED_PARAMETER(G);
      UNREFERENCED_PARAMETER(B);
      throw not_implemented(get_app());
   }

   bool dib::rgb_from(::draw2d::dib * pdib)
   {
      if(!create(pdib->size()))
         return false;
      try
      {
         byte * puchSrc = (byte *) get_data();
         byte * puchDst = (byte *) pdib->get_data();
         int64_t iArea = pdib->area();
         while(iArea > 0)
         {
            *puchDst++ = *puchSrc++;
            *puchDst++ = *puchSrc++;
            *puchDst++ = *puchSrc++;
            puchDst++;
            puchSrc++;
            iArea--;
         }
      }
      catch(...)
      {
         return false;
      }
      return true;
   }

   bool dib::from(::draw2d::graphics * pgraphics, FIBITMAP * pfibitmap, bool bUnloadFI)
   {
      throw interface_only_exception(get_app());
      return false;
   }

   bool dib::pixelate(int32_t iSize)
   {

      if(iSize <= 1)
         return true;



      int32_t w = cx;
      int32_t h = cy;

      int32_t xCount = w / iSize;
      int32_t yCount = h / iSize;



      int32_t iDiv;
      int32_t iDiv2;

      int32_t a;
      int32_t r;
      int32_t g;
      int32_t b;
      int32_t a2;
      int32_t r2;
      int32_t g2;
      int32_t b2;
      bool bFirst;
      COLORREF * pdata = get_data();
      int32_t x1;
      int32_t y1;
      for(int32_t x = 0; x < xCount; x++)
      {
         x1 = x * iSize;
         for(int32_t y = 0; y < yCount; y++)
         {
            y1 = y * iSize;
            a = 0;
            r = 0;
            g = 0;
            b = 0;
            a2 = 0;
            r2 = 0;
            g2 = 0;
            b2 = 0;
            iDiv = 0;
            iDiv2 = 0;
            bFirst = true;
            for(int32_t i = 0; i < iSize; i++)
            {
               for(int32_t j = 0; j < iSize; j++)
               {
                  COLORREF cr = pdata[x1 + i + (y1 + j) * w];
                  a += GetAValue(cr);
                  r += GetRValue(cr);
                  g += GetGValue(cr);
                  b += GetBValue(cr);
                  iDiv++;
                  if(iDiv >= 64)
                  {
                     a2 = (a2 * iDiv2 + a / iDiv) / (iDiv2 + 1);
                     r2 = (r2 * iDiv2 + r / iDiv) / (iDiv2 + 1);
                     g2 = (g2 * iDiv2 + g / iDiv) / (iDiv2 + 1);
                     b2 = (b2 * iDiv2 + b / iDiv) / (iDiv2 + 1);
                     a = 0;
                     r = 0;
                     g = 0;
                     b = 0;
                     iDiv = 0;
                     iDiv2++;
                  }
               }
            }
            if(iDiv > 0)
            {
               a2 = (a2 * iDiv2 + a / iDiv) / (iDiv2 + 1);
               r2 = (r2 * iDiv2 + r / iDiv) / (iDiv2 + 1);
               g2 = (g2 * iDiv2 + g / iDiv) / (iDiv2 + 1);
               b2 = (b2 * iDiv2 + b / iDiv) / (iDiv2 + 1);
            }
            COLORREF cr = ARGB(a2, r2, g2, b2);
            for(int32_t i = 0; i < iSize; i++)
            {
               for(int32_t j = 0; j < iSize; j++)
               {
                  pdata[x1 + i + (y1 + j) * w] = cr;
               }
            }
         }

      }


      if(w % iSize != 0)
      {
         int32_t x = xCount;
         int32_t x1 = x * iSize;
         int32_t iMax = w - xCount * iSize;
         for(int32_t y = 0; y < yCount; y++)
         {
            y1 = y * iSize;
            a = 0;
            r = 0;
            g = 0;
            b = 0;
            a2 = 0;
            r2 = 0;
            g2 = 0;
            b2 = 0;
            iDiv = 0;
            iDiv2 = 0;
            bFirst = true;
            for(int32_t i = 0; i < iMax; i++)
            {
               for(int32_t j = 0; j < iSize; j++)
               {
                  COLORREF cr = pdata[x1 + i + (y1 + j) * w];
                  a += GetAValue(cr);
                  r += GetRValue(cr);
                  g += GetGValue(cr);
                  b += GetBValue(cr);
                  iDiv++;
                  if(iDiv >= 64)
                  {
                     a2 = (a2 * iDiv2 + a / iDiv) / (iDiv2 + 1);
                     r2 = (r2 * iDiv2 + r / iDiv) / (iDiv2 + 1);
                     g2 = (g2 * iDiv2 + g / iDiv) / (iDiv2 + 1);
                     b2 = (b2 * iDiv2 + b / iDiv) / (iDiv2 + 1);
                     a = 0;
                     r = 0;
                     g = 0;
                     b = 0;
                     iDiv = 0;
                     iDiv2++;
                  }
               }
            }
            if(iDiv > 0)
            {
               a2 = (a2 * iDiv2 + a / iDiv) / (iDiv2 + 1);
               r2 = (r2 * iDiv2 + r / iDiv) / (iDiv2 + 1);
               g2 = (g2 * iDiv2 + g / iDiv) / (iDiv2 + 1);
               b2 = (b2 * iDiv2 + b / iDiv) / (iDiv2 + 1);
            }
            COLORREF cr = ARGB(a2, r2, g2, b2);
            for(int32_t i = 0; i < iMax; i++)
            {
               for(int32_t j = 0; j < iSize; j++)
               {
                  pdata[x1 + i + (y1 + j) * w] = cr;
               }
            }
         }

      }

      if(h % iSize != 0)
      {
         int32_t y = yCount;
         int32_t y1 = y * iSize;
         int32_t jMax = h - yCount * iSize;
         for(int32_t x = 0; x < xCount; x++)
         {
            x1 = x * iSize;
            a = 0;
            r = 0;
            g = 0;
            b = 0;
            a2 = 0;
            r2 = 0;
            g2 = 0;
            b2 = 0;
            iDiv = 0;
            iDiv2 = 0;
            bFirst = true;
            for(int32_t i = 0; i < iSize; i++)
            {
               for(int32_t j = 0; j < jMax; j++)
               {
                  COLORREF cr = pdata[x1 + i + (y1 + j) * w];
                  a += GetAValue(cr);
                  r += GetRValue(cr);
                  g += GetGValue(cr);
                  b += GetBValue(cr);
                  iDiv++;
                  if(iDiv >= 64)
                  {
                     a2 = (a2 * iDiv2 + a / iDiv) / (iDiv2 + 1);
                     r2 = (r2 * iDiv2 + r / iDiv) / (iDiv2 + 1);
                     g2 = (g2 * iDiv2 + g / iDiv) / (iDiv2 + 1);
                     b2 = (b2 * iDiv2 + b / iDiv) / (iDiv2 + 1);
                     a = 0;
                     r = 0;
                     g = 0;
                     b = 0;
                     iDiv = 0;
                     iDiv2++;
                  }
               }
            }
            if(iDiv > 0)
            {
               a2 = (a2 * iDiv2 + a / iDiv) / (iDiv2 + 1);
               r2 = (r2 * iDiv2 + r / iDiv) / (iDiv2 + 1);
               g2 = (g2 * iDiv2 + g / iDiv) / (iDiv2 + 1);
               b2 = (b2 * iDiv2 + b / iDiv) / (iDiv2 + 1);
            }
            COLORREF cr = ARGB(a2, r2, g2, b2);
            for(int32_t i = 0; i < iSize; i++)
            {
               for(int32_t j = 0; j < jMax; j++)
               {
                  pdata[x1 + i + (y1 + j) * w] = cr;
               }
            }
         }

      }

      if(w % iSize != 0)
      {
         int32_t x = xCount;
         int32_t x1 = x * iSize;
         int32_t iMax = w - xCount * iSize;
         int32_t y = yCount;
         int32_t y1 = y * iSize;
         int32_t jMax = h - yCount * iSize;
         x1 = x * iSize;
         a = 0;
         r = 0;
         g = 0;
         b = 0;
         a2 = 0;
         r2 = 0;
         g2 = 0;
         b2 = 0;
         iDiv = 0;
         iDiv2 = 0;
         bFirst = true;
         for(int32_t i = 0; i < iMax; i++)
         {
            for(int32_t j = 0; j < jMax; j++)
            {
               COLORREF cr = pdata[x1 + i + (y1 + j) * w];
               a += GetAValue(cr);
               r += GetRValue(cr);
               g += GetGValue(cr);
               b += GetBValue(cr);
               iDiv++;
               if(iDiv >= 64)
               {
                  a2 = (a2 * iDiv2 + a / iDiv) / (iDiv2 + 1);
                  r2 = (r2 * iDiv2 + r / iDiv) / (iDiv2 + 1);
                  g2 = (g2 * iDiv2 + g / iDiv) / (iDiv2 + 1);
                  b2 = (b2 * iDiv2 + b / iDiv) / (iDiv2 + 1);
                  a = 0;
                  r = 0;
                  g = 0;
                  b = 0;
                  iDiv = 0;
                  iDiv2++;
               }
            }
         }
         if(iDiv > 0)
         {
            a2 = (a2 * iDiv2 + a / iDiv) / (iDiv2 + 1);
            r2 = (r2 * iDiv2 + r / iDiv) / (iDiv2 + 1);
            g2 = (g2 * iDiv2 + g / iDiv) / (iDiv2 + 1);
            b2 = (b2 * iDiv2 + b / iDiv) / (iDiv2 + 1);
         }
         COLORREF cr = ARGB(a2, r2, g2, b2);
         for(int32_t i = 0; i < iMax; i++)
         {
            for(int32_t j = 0; j < jMax; j++)
            {
               pdata[x1 + i + (y1 + j) * w] = cr;
            }
         }
      }

      return true;

   }


   void dib::map(bool bApplyAlphaTransform)
   {

      UNREFERENCED_PARAMETER(bApplyAlphaTransform);

   }


   void dib::unmap()
   {

   }


   void dib::set_mapped()
   {

   }

    void dib::draw_bitmap(int32_t dx, int32_t dy, FT_Bitmap * bitmap, FT_Int x, FT_Int y)
    {

        FT_Int  i, j, p, q;
        FT_Int  x_max = x + bitmap->width;
        FT_Int  y_max = y + bitmap->rows;

        map();

        for ( i = x, p = 0; i < x_max; i++, p++ )
        {
            for ( j = y, q = 0; j < y_max; j++, q++ )
            {
                if(i < 0 || j < 0 || i >= cx || j >= cy)
                    continue;

                int32_t a = bitmap->buffer[q * bitmap->width + p];

                *((COLORREF *)&((byte *)get_data())[(dy + j) * scan + (dx + i) * 4]) = ARGB(a, 0, 0, 0);

            }
        }

    }

   
   bool dib::update_window(::ca2::window * pwnd, signal_details * pobj)
   {
      
      UNREFERENCED_PARAMETER(pwnd);
      UNREFERENCED_PARAMETER(pobj);
      
      // default implementation does nothing, dib should be now updated (before calling update window)
      // and ready to be queried if post queried
      
//      throw interface_only_exception(get_app());
      
      return true;
      
   }
   

   bool dib::print_window(::ca2::window * pwnd, signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pwnd);
      UNREFERENCED_PARAMETER(pobj);
      throw interface_only_exception(get_app());
   }


   void dib::gradient_fill(COLORREF clr1, COLORREF clr2, POINT pt1, POINT pt2)
   {

      double dx = pt2.x - pt1.x;

      double dy = pt1.y - pt2.y;

      if(dx == 0.0 && dy == 0.0)
      {
         Fill(
            byte_clip(GetAValue(clr1) * 0.5 + GetAValue(clr2) * 0.5),
            byte_clip(GetRValue(clr1) * 0.5 + GetRValue(clr2) * 0.5),
            byte_clip(GetGValue(clr1) * 0.5 + GetGValue(clr2) * 0.5),
            byte_clip(GetBValue(clr1) * 0.5 + GetBValue(clr2) * 0.5));
      }
      else if(dx == 0.0)
      {

         gradient_horizontal_fill(clr1, clr2, pt1.y, pt2.y);

      }
      else if(dy == 0.0)
      {

         gradient_vertical_fill(clr1, clr2, pt1.x, pt2.x);

      }
      else
      {

//         int x1 = min(pt1.x, pt2.x);

  //       int x2 = max(pt1.x, pt2.x);

    //     int y1 = min(pt1.y, pt2.y);

      //   int y2 = max(pt1.y, pt2.y);

//         int top = y1;
//
  //       int left = x1;
//
  //       int right = cx - x2;
          //
       //  int bottom = cy - y2;

         int dim = max(cx, cy);

         double angle = atan2(dy, dx);

         ::draw2d::dib_sp dib(allocer());

         if(abs(dx) > abs(dy))
         {

            double sin = ::sin(angle);

            dib->create((int32_t) (dim / sin), (int32_t) (dim / sin));

            dib->gradient_horizontal_fill(clr1, clr2, pt1.y, pt2.y);

            dib->rotate(this, -angle, 1.0);

         }
         else
         {

            double cos = ::cos(angle);

            dib->create((int32_t) (dim / cos), (int32_t) (dim / cos));

            dib->gradient_vertical_fill(clr1, clr2, pt1.x, pt2.x);

            dib->rotate(this, System.math().GetPi() - angle, 1.0);

         }

      }


   }

   void dib::gradient_horizontal_fill(COLORREF clr1, COLORREF clr2, int start, int end)
   {

      if(end < start)
      {
         ::sort::swap(&start, &end);
         ::sort::swap(&clr1, &clr2);
      }

      end = min(end, cy - 1);
      COLORREF clr = clr1;
      byte * pb = (byte *) m_pcolorref;
      COLORREF * pdata;
      int line = 0;
      for(; line < start; line++)
      {
         pdata = (COLORREF *) &pb[scan * line];
         for(int row = 0; row < cx; row++)
         {
            *pdata = clr;
            pdata++;
         }
      }
      double d;
      for(; line < end; line++)
      {

         d = ((double) (line - start)) / ((double) (end - start));

         clr = ARGB(
                  byte_clip(GetAValue(clr1) * (1.0 - d) + GetAValue(clr2) * d),
                  byte_clip(GetRValue(clr1) * (1.0 - d) + GetRValue(clr2) * d),
                  byte_clip(GetGValue(clr1) * (1.0 - d) + GetGValue(clr2) * d),
                  byte_clip(GetBValue(clr1) * (1.0 - d) + GetBValue(clr2) * d));

         pdata = (COLORREF *) &pb[scan * line];
         for(int row = 0; row < cx; row++)
         {
            *pdata = clr;
            pdata++;
         }
      }
      clr = clr2;
      for(; line < cy; line++)
      {
         pdata = (COLORREF *) &pb[scan * line];
         for(int row = 0; row < cx; row++)
         {
            *pdata = clr;
            pdata++;
         }
      }
   }

   void dib::gradient_vertical_fill(COLORREF clr1, COLORREF clr2, int start, int end)
   {

      if(end < start)
      {
         ::sort::swap(&start, &end);
         ::sort::swap(&clr1, &clr2);
      }

      end = min(end, cx - 1);
      COLORREF clr = clr1;
      byte * pb = (byte *) m_pcolorref;
      COLORREF * pdata;
      int row = 0;
      for(; row < start; row++)
      {
         pdata = (COLORREF *) &pb[sizeof(COLORREF) * row];
         for(int line = 0; line < cy; line++)
         {
            *pdata = clr;
            pdata+=scan;
         }
      }
      double d;
      for(; row < end; row++)
      {

         d = ((double) (row - start)) / ((double) (end - start));

         clr = ARGB(
                  byte_clip(GetAValue(clr1) * (1.0 - d) + GetAValue(clr2) * d),
                  byte_clip(GetRValue(clr1) * (1.0 - d) + GetRValue(clr2) * d),
                  byte_clip(GetGValue(clr1) * (1.0 - d) + GetGValue(clr2) * d),
                  byte_clip(GetBValue(clr1) * (1.0 - d) + GetBValue(clr2) * d));

         pdata = (COLORREF *) &pb[sizeof(COLORREF) * row];
         for(int line = 0; line < cx; line++)
         {
            *pdata = clr;
            pdata+=scan;
         }
      }
      clr = clr2;
      for(; row < cx; row++)
      {
         pdata = (COLORREF *) &pb[sizeof(COLORREF) * row];
         for(int line = 0; line < cx; line++)
         {
            *pdata = clr;
            pdata+=scan;
         }
      }
   }

   void dib::gradient_horizontal_fill(COLORREF clr1, COLORREF clr2)
   {
      gradient_horizontal_fill(clr1, clr2, 0, cy - 1);
   }

   void dib::gradient_vertical_fill(COLORREF clr1, COLORREF clr2)
   {
      gradient_vertical_fill(clr1, clr2, 0, cx - 1);
   }

} // namespace draw2d




