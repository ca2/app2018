#include "framework.h"
#include <math.h>


namespace draw2d_quartz2d
{
   
   
   //   Creator : El Barto (ef00@luc.ac.be)
   //   Location : http://www.luc.ac.be/~ef00/ebgfx
   //   Date : 09-04-98
   //////////////////////////////////////////////////////////////////////
   
   
   
/*   float dib::Cosines[360];
   float dib::Sines[360];
   
   
   int64_t dib::CosN[360]; // * 1 << 31
   int64_t dib::SinN[360];
   
   
   int64_t dib::Cos10N[10]; // until 10 degressa
   int64_t dib::Sin10N[10]; // more precision * 1 << 34
   
   
   double dib::dPi;*/
   
   
   dib::dib(::aura::application * papp) :
      object(papp),
      m_spbitmap(allocer()),
      m_spgraphics(allocer())
   {
    
      m_bMapped            = false;
      
   }
   
   dib::dib(const dib & dib) :
      object(dib.get_app()),
      m_spbitmap(allocer()),
      m_spgraphics(allocer())
   {
      
      m_bMapped            = false;
      
      if(dib.area() > 0)
      {
      
         ::draw2d::dib::from(&dib);
         
      }
      
   }
   
   void    dib::construct (int32_t cx,  int32_t cy)
   {
      
      m_pcolorref    = NULL;
      m_size.cx      = 0;
      m_size.cy      = 0;
      m_iScan        = 0;
      
      create(cx, cy);
      
   }
   
   
   dib::~dib ()
   {
      
      Destroy ();
      
   }

   
   COLORREF * dib::get_data() const
   {
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
   
   
   bool dib::create(class size size)
   {
      
      return create(size.cx, size.cy);
      
   }
   
   
   bool dib::create(int32_t width, int32_t height)
   {
      
      if(m_spbitmap.is_set()
         && m_spbitmap->get_os_data() != NULL
         && m_spgraphics.is_set()
         && m_spgraphics->get_os_data() != NULL
         && width == m_size.cx
         && height == m_size.cy)
      {
         
         return true;
         
      }
      
      Destroy();
      
      if(width <= 0 || height <= 0)
      {
         
         return false;
         
      }
      
      m_spbitmap.alloc(allocer());
      
      m_spgraphics.alloc(allocer());
      
      if(m_spbitmap.is_null() || m_spgraphics.is_null())
      {

         Destroy();
         
         return false;
         
      }
      
      if(!m_spbitmap->CreateDIBSection(NULL, width, height, DIB_RGB_COLORS, (void **) &m_pcolorref, &m_iScan, NULL,  0) || m_spbitmap->get_os_data() == NULL)
      {
         
         Destroy();
         
         return false;
         
      }
      
      m_spgraphics->SelectObject(m_spbitmap);
      
      if(m_spgraphics->get_os_data() == NULL)
      {

         Destroy();
         
         return false;
         
      }

      m_spgraphics->m_pdibDraw2dGraphics = this;

      m_size.cx = width;
         
      m_size.cy = height;
      
      m_sizeAlloc = m_size;
      
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
      
      ::draw2d::bitmap_sp pbitmap = pgraphics->get_current_bitmap();
      
      if(pbitmap == NULL)
      {
         
         return false;
         
      }
      
      ::size size = pbitmap->get_size();
      
      if(!create(size.cx, size.cy))
      {
         
         return false;
         
      }
      
      from(null_point(), pgraphics, null_point(), size);
      
      return true;
      
   }
   
   
   bool dib::Destroy ()
   {
      
      m_spbitmap.release();
      
      m_spgraphics.release();
      
      m_size.cx            = 0;
      
      m_size.cy            = 0;
      
      m_pcolorref          = NULL;
      
      m_iScan              = 0;
      
      return true;
      
   }
   
   
   bool dib::to(::draw2d::graphics * pgraphics, point pt, ::size size, point ptSrc)
   {
      
      return pgraphics->BitBlt(pt.x, pt.y, size.cx, size.cy, get_graphics(), ptSrc.x, ptSrc.y, SRCCOPY) != FALSE;
      
   }
   
    
//   bool dib::from(::draw2d::graphics * pgraphics)
//   {
//       
//      return pgraphics->BitBlt(0, 0, size.cx, size.cy,
//      
//   }
   
   
   bool dib::from(point ptDest, ::draw2d::graphics * pgraphics, point pt, class size sz)
   {
      return m_spgraphics->BitBlt(ptDest.x, ptDest.y, sz.cx, sz.cy, pgraphics, pt.x, pt.y, SRCCOPY) != FALSE;
   }
   

   
   void dib::SetIconMask(::visual::icon * picon, int32_t cx, int32_t cy)
   {
      create(cx, cy);
      
      if(cx <= 0 || cy <= 0)
         return;
      
      
      
      
      // White blend dib
      dib dib1(get_app());
      dib1.create(cx, cy);
      dib1.set(255, 255, 255);
      
      dib1.m_spgraphics->DrawIcon(
                                  0, 0,
                                  picon,
                                  cx, cy,
                                  0,
                                  NULL,
                                  DI_IMAGE | DI_MASK);
      
      // Black blend dib
      ::draw2d::dib_sp spdib2(allocer());
      spdib2->create(cx, cy);
      spdib2->Fill(0, 0, 0, 0);
      
      spdib2->get_graphics()->DrawIcon(
                                       0, 0,
                                       picon,
                                       cx, cy,
                                       0,
                                       NULL,
                                       DI_IMAGE | DI_MASK);
      
      // Mask dib
      dib dibM(get_app());
      dibM.create(cx, cy);
      
      dibM.m_spgraphics->DrawIcon(
                                  0, 0,
                                  picon,
                                  cx, cy,
                                  0,
                                  NULL,
                                  DI_MASK);
      
      BYTE * r1=(BYTE*)dib1.m_pcolorref;
      BYTE * r2=(BYTE*)spdib2->get_data();
      BYTE * srcM=(BYTE*)dibM.m_pcolorref;
      BYTE * dest=(BYTE*)m_pcolorref;
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
            bMax = MAX(b, bMax);
            b =(BYTE)(r1[1]  - r2[1]);
            bMax = MAX(b, bMax);
            b =(BYTE)(r1[2]  - r2[2]);
            bMax = MAX(b, bMax);
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
   void dib::set_mapped()
   {
      m_bMapped = true;
   }
   
   
   void dib::stretch_dib(::draw2d::dib * pdib)
   {
      
      ::draw2d::dib::stretch_dib(pdib);
      
   }
   
   
   ::draw2d::graphics * dib::get_graphics() const
   {
      
      unmap();
      
      return m_spgraphics;
      
   }
   
   
   void dib::map(bool bApplyTransform) const
   {
      
      //if(m_bMapped)
      //   return;
      
      ((dib *) this)->m_bMapped = true;
      
   }
   
   bool dib::detach(::draw2d::dib * pdib)
   {
      
      sp(dib) dib = pdib;
      
      m_spgraphics = dib->m_spgraphics;
      m_spbitmap = dib->m_spbitmap;
      
      ::draw2d::dib::detach(pdib);
      
      dib->m_spbitmap.release();
      dib->m_spgraphics.release();
      
      return true;
      
   }

   
   void dib::unmap() const
   {
      
//      if(((dib *) this)->m_spgraphics.is_set())
//      {
//      
//         ((dib *) this)->m_spgraphics->SelectObject(((dib *) this)->m_spbitmap);
//         
//         ((dib *) this)->m_spgraphics->m_pdibDraw2dGraphics = (dib *) this;
//         
//      }
      
      ((dib *) this)->m_bMapped = false;
      
   }
   
   
   bool dib::blend(point ptDst, ::draw2d::dib * pdibSrc, point ptSrc, class size size, byte bA)
   {
      
      dib * pdibDst = this;
      
      pdibDst->map();
      
      pdibSrc->map();
      
      ptDst += m_pt;
      
      if (ptSrc.x < 0)
      {
         ptDst.x -= ptSrc.x;
         ptSrc.x = 0;
      }
      
      if (ptSrc.y < 0)
      {
         ptDst.y -= ptSrc.y;
         ptSrc.y = 0;
      }
      
      if (ptDst.x < 0)
      {
         size.cx += ptDst.x;
         ptDst.x = 0;
      }
      
      if (size.cx < 0)
         return true;
      
      if (ptDst.y < 0)
      {
         size.cy += ptDst.y;
         ptDst.y = 0;
      }
      
      if (size.cy < 0)
         return true;
      
      int xEnd = MIN(size.cx, MIN(pdibSrc->m_size.cx - ptSrc.x, pdibDst->m_size.cx - ptDst.x));
      
      int yEnd = MIN(size.cy, MIN(pdibSrc->m_size.cy - ptSrc.y, pdibDst->m_size.cy - ptDst.y));
      
      if (xEnd < 0)
         return false;
      
      if (yEnd < 0)
         return false;
      
      int32_t scanDst = pdibDst->m_iScan;
      
      int32_t scanSrc = pdibSrc->m_iScan;
      
      
      byte * pdst2;
      
      byte * psrc2;
      
#ifdef APPLEOS
      byte * pdst = &((byte *)pdibDst->m_pcolorref)[scanDst * (pdibDst->m_size.cy - ptDst.y - yEnd) + ptDst.x * sizeof(COLORREF)];
      
      byte * psrc = &((byte *)pdibSrc->m_pcolorref)[scanSrc * (pdibSrc->m_size.cy - ptSrc.y - yEnd) + ptSrc.x * sizeof(COLORREF)];
      
#else
      
      byte * pdst = &((byte *)pdibDst->m_pcolorref)[scanDst * ptDst.y + ptDst.x * sizeof(COLORREF)];
      
      byte * psrc = &((byte *)pdibSrc->m_pcolorref)[scanSrc *  ptSrc.y + ptSrc.x * sizeof(COLORREF)];
      
#endif
      
//      bool bFontListBlend = true;
//      
//      if (bFontListBlend)
//      {
//         
//         if (bA == 0)
//         {
//            
//         }
//         else if (bA == 255)
//         {
//            
//            for (int y = 0; y < yEnd; y++)
//            {
//               
//               pdst2 = &pdst[scanDst * y];
//               
//               psrc2 = &psrc[scanSrc * y];
//               
//               //memcpy(pdst2, psrc2, xEnd * 4);
//               for (int x = 0; x < xEnd; x++)
//               {
//                  
//                  //*pdst2 = *psrc2;
//                  
//                  //pdst2[0] = (psrc2[0] + (pdst2[0] * (255 - psrc2[3])) / 255);
//                  //pdst2[1] = (psrc2[1] + (pdst2[1] * (255 - psrc2[3])) / 255);
//                  //pdst2[2] = (psrc2[2] + (pdst2[2] * (255 - psrc2[3])) / 255);
//                  //pdst2[3] = (psrc2[3] + (pdst2[3] * (255 - psrc2[3])) / 255);
//                  byte acomplement = ~psrc2[3];
//                  pdst2[0] = psrc2[0] + ((pdst2[0] * (acomplement)) >> 8);
//                  pdst2[1] = psrc2[1] + ((pdst2[1] * (acomplement)) >> 8);
//                  pdst2[2] = psrc2[2] + ((pdst2[2] * (acomplement)) >> 8);
//                  pdst2[3] = psrc2[3] + ((pdst2[3] * (acomplement)) >> 8);
//                  
//                  
//                  
//                  pdst2 += 4;
//                  
//                  psrc2 += 4;
//                  
//               }
//               //pdst2 += xEnd;
//               //psrc2 += xEnd;
//               
//            }
//         }
//         else
//         {
//            for (int y = 0; y < yEnd; y++)
//            {
//               
//               pdst2 = &pdst[scanDst * y];
//               
//               psrc2 = &psrc[scanSrc * y];
//               
//               //memcpy(pdst2, psrc2, xEnd * 4);
//               for (int x = 0; x < xEnd; x++)
//               {
//                  
//                  //*pdst2 = *psrc2;
//                  
//                  //pdst2[0] = (psrc2[0] + (pdst2[0] * (255 - psrc2[3])) / 255);
//                  //pdst2[1] = (psrc2[1] + (pdst2[1] * (255 - psrc2[3])) / 255);
//                  //pdst2[2] = (psrc2[2] + (pdst2[2] * (255 - psrc2[3])) / 255);
//                  //pdst2[3] = (psrc2[3] + (pdst2[3] * (255 - psrc2[3])) / 255);
//                  //byte acomplement = (~psrc2[3] * bA) >> 8;
//                  //pdst2[0] = psrc2[0] + ((pdst2[0] * (acomplement)) >> 8);
//                  //pdst2[1] = psrc2[1] + ((pdst2[1] * (acomplement)) >> 8);
//                  //pdst2[2] = psrc2[2] + ((pdst2[2] * (acomplement)) >> 8);
//                  //pdst2[3] = psrc2[3] + ((pdst2[3] * (acomplement)) >> 8);
//                  byte acomplement = (~psrc2[3] * bA) >> 8;
//                  pdst2[0] = clip_byte(((psrc2[0] * bA) + (pdst2[0] * acomplement)) >> 8);
//                  pdst2[1] = clip_byte(((psrc2[1] * bA) + (pdst2[1] * acomplement)) >> 8);
//                  pdst2[2] = clip_byte(((psrc2[2] * bA) + (pdst2[2] * acomplement)) >> 8);
//                  pdst2[3] = clip_byte(((psrc2[3] * bA) + (pdst2[3] * acomplement)) >> 8);
//                  
//                  
//                  
//                  pdst2 += 4;
//                  
//                  psrc2 += 4;
//                  
//               }
//               //pdst2 += xEnd;
//               //psrc2 += xEnd;
//               
//            }
//            
//         }
//         
//         // bFontListData
//         
//      }
//      else
      {
         
         // !bFontListData
         // bBouncingBall...
         
         if (bA == 0)
         {
            
         }
         else if (bA == 255)
         {
            
            for (int y = 0; y < yEnd; y++)
            {
               
               pdst2 = &pdst[scanDst * y];
               
               psrc2 = &psrc[scanSrc * y];
               
               //memcpy(pdst2, psrc2, xEnd * 4);
               for (int x = 0; x < xEnd; x++)
               {
                  
                  //*pdst2 = *psrc2;
                  
                  //pdst2[0] = (psrc2[0] + (pdst2[0] * (255 - psrc2[3])) / 255);
                  //pdst2[1] = (psrc2[1] + (pdst2[1] * (255 - psrc2[3])) / 255);
                  //pdst2[2] = (psrc2[2] + (pdst2[2] * (255 - psrc2[3])) / 255);
                  //pdst2[3] = (psrc2[3] + (pdst2[3] * (255 - psrc2[3])) / 255);
                  byte a = pdst2[3];
                  byte alpha = psrc2[3];
                  if (a == 0)
                  {
                     
                  }
                  else if(alpha == 0)
                  {
                     
                     *((COLORREF *)pdst2) = 0;
                     
                  }
                  else
                  {
                     
                     //int d0 = pdst2[0] * 255 / a;
                     //int d1 = pdst2[1] * 255 / a;
                     //int d2 = pdst2[2] * 255 / a;
                     
                     //int s0 = psrc2[0] * 255 / alpha;
                     //int s1 = psrc2[1] * 255 / alpha;
                     //int s2 = psrc2[2] * 255 / alpha;
                     
                     //d0 = ((s0 * a) + (d0 * alpha)) >> 8;
                     //d1 = ((s1 * a) + (d1 * alpha)) >> 8;
                     //d2 = ((s2 * a) + (d2 * alpha)) >> 8;
                     //pdst2[3] = ((psrc2[3] * a) + (pdst2[3] * alpha)) >> 8;
                     
                     //pdst[0] = (d0 * pdst2[3]) >> 8;
                     //pdst[1] = (d1 * pdst2[3]) >> 8;
                     //pdst[2] = (d2 * pdst2[3]) >> 8;
                     
                     //pdst2[0] = psrc2[0] + ((pdst2[0] * (acomplement)) >> 8);
                     //pdst2[1] = psrc2[1] + ((pdst2[1] * (acomplement)) >> 8);
                     //pdst2[2] = psrc2[2] + ((pdst2[2] * (acomplement)) >> 8);
                     //pdst2[3] = psrc2[3] + ((pdst2[3] * (acomplement)) >> 8);
                     
                     pdst2[0] = (pdst2[0] * alpha) >> 8;
                     pdst2[1] = (pdst2[1] * alpha) >> 8;
                     pdst2[2] = (pdst2[2] * alpha) >> 8;
                     pdst2[3] = (pdst2[3] * alpha) >> 8;
                  }
                  
                  
                  
                  pdst2 += 4;
                  
                  psrc2 += 4;
                  
               }
               //pdst2 += xEnd;
               //psrc2 += xEnd;
               
            }
         }
         else
         {
            for (int y = 0; y < yEnd; y++)
            {
               
               pdst2 = &pdst[scanDst * y];
               
               psrc2 = &psrc[scanSrc * y];
               
               //memcpy(pdst2, psrc2, xEnd * 4);
               for (int x = 0; x < xEnd; x++)
               {
                  
                  //*pdst2 = *psrc2;
                  
                  //pdst2[0] = (psrc2[0] + (pdst2[0] * (255 - psrc2[3])) / 255);
                  //pdst2[1] = (psrc2[1] + (pdst2[1] * (255 - psrc2[3])) / 255);
                  //pdst2[2] = (psrc2[2] + (pdst2[2] * (255 - psrc2[3])) / 255);
                  //pdst2[3] = (psrc2[3] + (pdst2[3] * (255 - psrc2[3])) / 255);
                  //byte acomplement = (~psrc2[3] * bA) >> 8;
                  //pdst2[0] = psrc2[0] + ((pdst2[0] * (acomplement)) >> 8);
                  //pdst2[1] = psrc2[1] + ((pdst2[1] * (acomplement)) >> 8);
                  //pdst2[2] = psrc2[2] + ((pdst2[2] * (acomplement)) >> 8);
                  //pdst2[3] = psrc2[3] + ((pdst2[3] * (acomplement)) >> 8);
                  byte acomplement = (~psrc2[3] * bA) >> 8;
                  pdst2[0] = clip_byte(((psrc2[0] * bA) + (pdst2[0] * acomplement)) >> 8);
                  pdst2[1] = clip_byte(((psrc2[1] * bA) + (pdst2[1] * acomplement)) >> 8);
                  pdst2[2] = clip_byte(((psrc2[2] * bA) + (pdst2[2] * acomplement)) >> 8);
                  pdst2[3] = clip_byte(((psrc2[3] * bA) + (pdst2[3] * acomplement)) >> 8);
                  
                  
                  
                  pdst2 += 4;
                  
                  psrc2 += 4;
                  
               }
               //pdst2 += xEnd;
               //psrc2 += xEnd;
               
            }
            
         }
         
      }
      
      return true;
      
   }
   
   

} // namespace draw2d_quartz2d





