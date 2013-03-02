#include "framework.h"


namespace gcom
{


   namespace backview
   {


      Graphics::Graphics(Main & main) :
         ::ca::ca(main.get_app()),
         Helper(main),
         m_mutgenBack(main.get_app()),
         m_mutgenBuffer(main.get_app()),
         m_mutex3Source(main.get_app()),
         m_mutex4Transfer(main.get_app())
      {
//         m_spdrawdib.create(get_app());
//         m_spdrawdib->open();
         m_dcScreen.create(get_app());
      }

      Graphics::~Graphics()
      {
      }

      ::ca::graphics & Graphics::GetScreenDC()
      {
         return m_dcScreen;
      }

      ::ca::graphics & Graphics::GetBackDC()
      {
         return *GetDib(_graphics::DibBack)->get_graphics();
      }

      ::ca::graphics & Graphics::GetTransferDC()
      {
         GetDib(_graphics::DibTransfer)->dc_select();
         return *GetDib(_graphics::DibTransfer)->get_graphics();
      }

      ::ca::graphics & Graphics::GetFrame1DC()
      {
         return *GetDib(_graphics::DibFrame1)->get_graphics();
      }

      ::ca::graphics & Graphics::GetBufferDC()
      {
         return *GetDib(_graphics::DibBuffer)->get_graphics();
      }

      ::ca::graphics & Graphics::GetSourceDC()
      {
         return *GetDib(_graphics::DibSource)->get_graphics();
      }

      ::ca::bitmap & Graphics::GetBackBitmap()
      {
         return *GetDib(_graphics::DibBack)->get_bitmap();
      }

      ::ca::bitmap & Graphics::GetTransferBitmap()
      {
         return *GetDib(_graphics::DibTransfer)->get_bitmap();
      }

      ::ca::bitmap & Graphics::GetFrame1Bitmap()
      {
         return *GetDib(_graphics::DibFrame1)->get_bitmap();
      }

      ::ca::bitmap & Graphics::GetBufferBitmap()
      {
         return *GetDib(_graphics::DibBuffer)->get_bitmap();

      }

      ::ca::bitmap & Graphics::GetSourceBitmap()
      {
         return *GetDib(_graphics::DibSource)->get_bitmap();
      }

//      ::ca::draw_dib & Graphics::GetDrawDib()
//      {
//         return m_spdrawdib;
//      }

      void Graphics::LayoutBackBitmap(
         BITMAP *   lpbmBack)
      {
         UNREFERENCED_PARAMETER(lpbmBack);
         rect rectClient;

         HelperGetMain().GetInterface().BackViewGetClientRect(rectClient);

         int32_t cx = rectClient.width();
         int32_t cy = rectClient.height();

         single_lock sl1Back(&m_mutgenBack, TRUE);
         ::ca::graphics & dcBack = GetBackDC();
         GetDib(_graphics::DibBack)->create(cx, cy);
         dcBack.FillSolidRect(0, 0, cx, cy, ARGB(0, 0, 0, 0));
         sl1Back.unlock();
      }


      void Graphics::OnImageLoaded(::ca::dib * pdib)
      {

         single_lock sl3Source(&m_mutex3Source, TRUE);

         Main & main = HelperGetMain();

         //         ::ca::savings & savings = System.savings();

         bool bOk = false;
         // 2004-08-24
         if(pdib != NULL)
         {
            GetDib(_graphics::DibSource)->from(pdib);
            bOk = true;
         }

         if(bOk)
            main.ImageChangePostEvent(EventLoaded);
         else
            main.ImageChangePostEvent(EventLoadFailed);

      }

      void Graphics::OnDestroy()
      {

         single_lock sl1Back(&m_mutgenBack, TRUE);
         single_lock sl2Buffer(&m_mutgenBuffer, TRUE);
         single_lock sl3Source(&m_mutex3Source, TRUE);

      }

      bool Graphics::RenderBufferLevel2()
      {
         Main & main = HelperGetMain();

         Interface & iface = main.GetInterface();

         rect rectClient;
         iface.BackViewGetClientRect(rectClient);

         rect rectScreen = rectClient;
         iface.BackViewClientToScreen(rectScreen);

         int32_t cx = rectClient.width();
         int32_t cy = rectClient.height();

         //         const int32_t ciBufferBitmapNotCreated = 1;
         //         const int32_t ciBackBitmapNotCreated = 2;
         //         const int32_t ciBufferBitmapNotSelected = 3;
         //         const int32_t ciBackBitmapNotSelected = 4;
         //         const int32_t ciBufferBitmapInfoNotAvailable = 5;
         //         const int32_t ciScaledBitmapInfoNotAvailable = 6;

         single_lock sl2Buffer(&m_mutgenBuffer, TRUE);
         single_lock sl3Source(&m_mutex3Source, TRUE);

         //HelperGetMain().DeferCheckLayout();

         //         ::ca::graphics & dcScreen = GetScreenDC();
         ::ca::graphics & dcBuffer = GetBufferDC();
         ::ca::graphics & dcSource = GetSourceDC();
         //         ::ca::bitmap & bmpBuffer = GetBufferBitmap();


         //dcSource.FillSolidRect(200, 200, 500, 500, ARGB(255, 255, 0, 0));


         //         ::ca::dib * pdibSource = GetDib(_graphics::DibSource);

         if(dcSource.get_os_data() == NULL)
            return false;

         if(&dcBuffer == NULL)
            return false;

         if(dcBuffer.get_os_data() == NULL)
            return false;

         try
         {   
            //sl2Buffer.lock();

            dcBuffer.FillSolidRect(rectClient, ARGB(0, 0, 0, 0));

#ifdef WINDOWSEX

            HENHMETAFILE hemf = main.GetInterface().BackViewGetFillingMetaFile();
            if(hemf != NULL)
            {
               ENHMETAHEADER emh;

               ::GetEnhMetaFileHeader(
                  hemf,
                  sizeof(emh),
                  &emh);

               dcBuffer.FillSolidRect(rectClient, ARGB(0, 0, 0, 0));

               rect rectMeta(0, 0, emh.rclBounds.right - emh.rclBounds.left, emh.rclBounds.bottom - emh.rclBounds.top);
               rectMeta.FitOnCenterOf(rect(0, 0, 64, 64));

               int32_t iW = rectMeta.width();
               int32_t iH = rectMeta.height();
               int32_t iXMod = (cx - iW) / 2;
               int32_t iYMod = (cy - iH) / 2;
               int32_t iXOffset =  iXMod;
               int32_t iYOffset =  iYMod;
               iXOffset %= iW;
               while(iXOffset > 0)
               {
                  iXOffset -= iW;
               }
               iYOffset %= iH;
               while(iYOffset > 0)
               {
                  iYOffset -= iH;
               }
               int32_t iCount = (iXMod < 0) ? 1 : cx / iW + (iXOffset == 0 ? 0 : 2);
               int32_t jCount = (iYMod < 0) ? 1 : cy / iH + (iYOffset == 0 ? 0 : 2);

               rectMeta.left = iXOffset;
               for(int32_t i = 0; i < iCount; i++)
               {
                  rectMeta.top = iYOffset;
                  for(int32_t j = 0; j < jCount; j++)
                  {
                     rectMeta.SetBottomRightSize(iW, iH);
                     dcBuffer.PlayMetaFile(hemf, rectMeta);
                     rectMeta.top += iH;
                  }
                  rectMeta.left += iW;
               }
            }
#else

            throw todo(get_app());

#endif

            //sl3Source.lock();
            ::ca::bitmap & bmpSource = GetSourceBitmap();

            if(bmpSource.get_os_data() != NULL 
               && GetDib(_graphics::DibSource)->area() > 0)
            {
               if(main.IsFullScreen())
               {
                  dcBuffer.FillSolidRect(
                     rectClient,
                     GetDib(_graphics::DibSource)->GetAverageColor());
               }
               else
               {
                  dcBuffer.FillSolidRect(
                     rectClient,
                     GetDib(_graphics::DibSource)->GetAverageColor());
               }

               class size sizeSource = bmpSource.get_size();


               int32_t finalX;
               int32_t finalY;
               int32_t finalW;
               int32_t finalH;

               ImageChange & imagechange = main.GetImageChange();

               double dRate;
               if(imagechange.m_eplacement == ImagePlacementZoomAll)
               {
                  dRate = min((double) cx / sizeSource.cx, (double) cy / sizeSource.cy);
                  finalW = (int32_t) (sizeSource.cx * dRate);
                  finalH = (int32_t)(sizeSource.cy * dRate);
                  finalX = (cx - finalW) / 2;
                  finalY = (cy - finalH) / 2;
               }
               else if (imagechange.m_eplacement == ImagePlacementZoomExtend)
               {
                  dRate = max((double) cx / sizeSource.cx, (double) cy / sizeSource.cy);
                  finalW = (int32_t) (sizeSource.cx * dRate);
                  finalH = (int32_t) (sizeSource.cy * dRate);
                  finalX = (cx - finalW) / 2;
                  finalY = (cy - finalH) / 2;
               }
               else if (imagechange.m_eplacement == ImagePlacementStretch)
               {
                  finalW = (int32_t) cx;
                  finalH = (int32_t) cy;
                  finalX = 0;
                  finalY = 0;
               }


               if(imagechange.m_eplacement == ImagePlacementTile)
               {

                  int32_t iW = sizeSource.cx;
                  int32_t iH = sizeSource.cy;
                  int32_t iXMod = (cx - iW) / 2;
                  int32_t iYMod = (cy - iH) / 2;
                  int32_t iXOffset =  iXMod;
                  int32_t iYOffset =  iYMod;
                  while(iXOffset > 0)
                  {
                     iXOffset -= iW;
                  }
                  while(iYOffset > 0)
                  {
                     iYOffset -= iH;
                  }
                  int32_t iCount = (iXMod < 0) ? 1 : cx / iW + (iXOffset == 0 ? 0 : 2);
                  int32_t jCount = (iYMod < 0) ? 1 : cy / iH + (iYOffset == 0 ? 0 : 2);
                  for(int32_t i = 0; i < iCount; i++)
                  {
                     int32_t iX = iXOffset + iW * i;
                     for(int32_t j = 0; j < jCount; j++)
                     {
                        int32_t iY = iYOffset + iH * j;
                        dcBuffer.BitBlt(
                           iX, iY,
                           iW, iH,
                           &dcSource,
                           0, 0,
                           SRCCOPY);
                     }
                  }

                  m_rectFinalPlacement.left     = iXOffset;
                  m_rectFinalPlacement.top      = iYOffset;
                  m_rectFinalPlacement.right    = iXOffset + iW * iCount;
                  m_rectFinalPlacement.bottom   = iXOffset + iW * jCount;

               }
               else if(imagechange.m_eplacement == ImagePlacementZoomAll ||
                  imagechange.m_eplacement == ImagePlacementZoomExtend ||
                  imagechange.m_eplacement == ImagePlacementStretch)
               {

                  int32_t srcX = 0;
                  int32_t srcY = 0;
                  int32_t srcW = sizeSource.cx;
                  int32_t srcH = sizeSource.cy;

                  dcBuffer.SetStretchBltMode(HALFTONE);
                  dcBuffer.StretchBlt(
                     finalX, finalY,
                     finalW, finalH,
                     &dcSource,
                     srcX, srcY,
                     srcW, srcH,
                     SRCCOPY);
                  m_rectFinalPlacement.left     = finalX;
                  m_rectFinalPlacement.top      = finalY;
                  m_rectFinalPlacement.right    = finalX + finalW;
                  m_rectFinalPlacement.bottom   = finalY + finalH;

               }
               GetDib(_graphics::DibFinal)->create(
                  m_rectFinalPlacement.width(),
                  m_rectFinalPlacement.height());
               GetDib(_graphics::DibFinal)->get_graphics()->SetStretchBltMode(HALFTONE);
               GetDib(_graphics::DibFinal)->get_graphics()->StretchBlt(
                  0,
                  0,
                  m_rectFinalPlacement.width(),
                  m_rectFinalPlacement.height(),
                  GetDib(_graphics::DibBuffer)->get_graphics(),
                  m_rectFinalPlacement.left,
                  m_rectFinalPlacement.top,
                  m_rectFinalPlacement.width(),
                  m_rectFinalPlacement.height(),
                  SRCCOPY);

               /*            else if(m_lineSongLabel.GetVisible() && ! m_bCompactMode)
               {
               System.visual().imaging().color_blend(&dcBuffer, 0, m_lineSongLabel.m_iTop, cx, m_lineSongLabel.m_iBottom - m_lineSongLabel.m_iTop, RGB(255, 255, 255), 127);
               }*/
            }
         }
         catch(int32_t)
         {
         }
         //GetDib(100)->from(&dcBuffer, &bmpBuffer);
         return true;
      }

      void Graphics::UpdateObjects()
      {
         rect rectClient;
         Interface & iface = HelperGetMain().GetInterface();
         iface.BackViewGetClientRect(rectClient);
         int32_t cx = rectClient.width();
         int32_t cy = rectClient.height();

         single_lock sl1Back(&m_mutgenBack, TRUE);
         single_lock sl2Buffer(&m_mutgenBuffer, TRUE);
         single_lock sl3Source(&m_mutex3Source, TRUE);


         ::ca::graphics & spgraphicsScreen    = GetScreenDC();

         if(spgraphicsScreen.get_os_data() != NULL)
         {
            spgraphicsScreen.DeleteDC();
         }
         spgraphicsScreen.CreateDC("DISPLAY", NULL, NULL, NULL);

         GetDib(_graphics::DibBack)->create(cx, cy); // Back
         GetDib(_graphics::DibBack)->Fill(0, 0, 0, 0);

         GetDib(_graphics::DibBuffer)->create(cx, cy); // buffer
         GetDib(_graphics::DibBuffer)->Fill(0, 0, 0, 0);

         GetDib(_graphics::DibTransfer)->create(cx, cy); // Transfer
         GetDib(_graphics::DibTransfer)->Fill(0, 0, 0, 0);

         GetDib(_graphics::DibFrame1)->create(cx, cy); // Frame1
         GetDib(_graphics::DibFrame1)->Fill(0, 0, 0, 0);

      }

      EImagePlacement Graphics::GetDefaultPlacement()
      {

         EImagePlacement eplacement = ImagePlacementZoomAll;

#ifdef WINDOWSEX

         ::ca::bitmap & bmpSource = GetSourceBitmap();

         class size size = bmpSource.get_size();

         if(size.cx < (GetSystemMetrics(SM_CXSCREEN) / 2) &&
            size.cy < (GetSystemMetrics(SM_CYSCREEN) / 2))
         {

            eplacement = ImagePlacementTile;

         }
         else
         {

            eplacement = ImagePlacementZoomAll;

         }

#endif

         return eplacement;

      }

      ::ca::dib * Graphics::GetDib(int32_t iIndex)
      {
         ::ca::dib * pdib;
         if(m_mapDib.Lookup(iIndex, pdib))
         {
            return pdib;
         }
         else
         {
            ::ca::dib_sp spdib(get_app());
            OnCreateDib(spdib, iIndex);
            m_mapDib.set_at(iIndex, spdib);
            ::ca::add_ref(spdib.m_p);
            return spdib;
         }
      }

      void Graphics::BufferToBack()
      {
         single_lock sl1Back(&m_mutgenBack, FALSE);
         single_lock sl2Buffer(&m_mutgenBuffer, FALSE);
         if(!sl1Back.lock(millis(25)))
            return;
         if(!sl2Buffer.lock(millis(25)))
            return;
         GetDib(_graphics::DibBack)->copy(GetDib(_graphics::DibBuffer));
      }

      void Graphics::BackToTransfer()
      {

         single_lock sl1Back(&m_mutgenBack, FALSE);

         if(!sl1Back.lock(millis(25)))
            return;

         GetDib(_graphics::DibTransfer)->copy(GetDib(_graphics::DibBack));

      }
      void Graphics::OnCreateDib(::ca::dib *pdib, int32_t iIndex)
      {
         switch(iIndex)
         {
         case 1977:
            {
               // Radial Fill (Center Full alpha, Radius = 256 pixels)
               pdib->create(512, 512);
               pdib->RadialFill(255, 0, 0, 0, 
                  0, 0, 0, 0,
                  255, 255, 256);
            }
            break;
         case 1984:
            {
               // Radial Fill (Border Full alpha, Radius = 256 pixels)
               pdib->create(511, 511);
               pdib->RadialFill(0, 0, 0, 0,
                  255, 0, 0, 0, 255, 255, 256);
            }
            break;
         default:
            break;
         }
      }
      void Graphics::GetFinalPlacement(LPRECT lprect)
      {
         *lprect = m_rectFinalPlacement;

      }


   } // namespace backview


} // namespace gcom




