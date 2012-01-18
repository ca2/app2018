#include "StdAfx.h"
#include <math.h>


namespace gcom
{

   namespace backview
   {

   #define HIDOUBLETOINT(d) (((d) > (int) (d)) ? ((int) (d)) + 1 : (int) (d))


      TransitionEffect::TransitionEffect(Main & view) :
         ::ca::ca(view.get_app()),
         Helper(view),
         m_tool001(view)
      {
         m_bRun = true;
         m_eventThreadExit.ResetEvent();
         m_bDestroy = false;
         m_pthreadRunStep = NULL;
         m_tool001.m_iStep = 0;
         m_bTransitionStepRunActive = false;
         m_bTransitionStepPostActive = false;
         m_dwTransitionStepLastRunTime = GetTickCount();
         m_dwTransitionStepPostActive = GetTickCount();
         m_dwLastTransitionTime = GetTickCount();

         m_dwLastTransitionTime = GetTickCount();

         int iStart  = (int) TransitionEffectFirst;
         int iEnd    = (int) TransitionEffectLast;

         //iStart  = (int) TransitionEffectLinearFadingTopBottom;
         //iEnd    = (int) TransitionEffectLinearFadingRightLeft;

         //iStart  = (int) TransitionEffectAccumulLinearFadingTopBottom;
         //iEnd    = (int) TransitionEffectAccumulLinearFadingRightLeft;

         //iStart   = (int) TransitionEffectFlyInTopBottom;
         //iEnd     = (int) TransitionEffectFlyInRightTop;

         //iStart   = (int) TransitionEffectWipeBottom;
         //iEnd     = (int) TransitionEffectWipeIn;

         //iStart   = (int) TransitionEffectScaleBottom;
         //iEnd     = (int) TransitionEffectScaleCenter;

         //iStart   = (int) TransitionEffectpixelate_TopBottom;
         //iEnd     = (int) TransitionEffectpixelate_RightLeft;

         //iStart   = (int) TransitionEffectSquarypixelate_

         //iStart   = (int) TransitionEffectVisual;
         //iEnd     = (int) TransitionEffectVisual;

         iStart      = (int) TransitionEffectRadialUnveil;
         iEnd      = (int) TransitionEffectRadialUnveil;


         for(int i = iStart; i <= iEnd; i++)
         {
            m_etypea.add((ETransitionEffect) i);
         }

         //iStart = iEnd = (int) VisualEffectPixelExplosion;
         //iStart = iEnd = (int) VisualEffectAlphaPixelExplosion;
         //iStart = iEnd = (int) VisualEffectPixelExplosion2;
         
         //iStart = iEnd = (int) VisualEffectRotateEx8;
         //iStart = iEnd = (int) VisualEffectRotateEx4;
         //iStart = iEnd = (int) VisualEffectRain1;
         //iStart = iEnd = (int) VisualEffectRotateEx5;
         //iStart = iEnd = (int) VisualEffectRotateEx6;
         //iStart = iEnd = (int) VisualEffectRotateEx7;

         //iStart = iEnd = (int) VisualEffectExpand8;
         //iStart = iEnd = (int) VisualEffectExpand4;
         //iStart = iEnd = (int) VisualEffectExpand5;
         //iStart = iEnd = (int) VisualEffectExpand6;
         //iStart = iEnd = (int) VisualEffectExpand7;

         //iStart = iEnd = (int) VisualEffectRotateBlend;
         //iStart = iEnd = (int) VisualEffectNoPrecisionRotateBlend;
         //iStart = iEnd = (int) VisualEffectNoPrecisionRotatecolor_blend_;
         //iStart = iEnd = (int) VisualEffectNoPrecisionRotateTrackcolor_blend_;
         //iStart = iEnd = (int) VisualEffectRotateEx1;
         //iStart = iEnd = (int) VisualEffectRotateEx2;
         //iStart = iEnd = (int) VisualEffectRotateEx3;

         iStart   = (int) VisualEffectPixelExplosion;
         iEnd     = (int) VisualEffectRotateEx3;

         for(int i = iStart; i <= iEnd; i++)
         {
            m_iaVisual.add(i);
         }
         

         m_bInitialized    = false;
         m_bActive         = false;
         m_iType           = 0;
         m_iVisual         = 0;
      }

      TransitionEffect::~TransitionEffect()
      {
         m_eventThreadExit.ResetEvent();
         m_bDestroy = true;
         m_eventThreadExit.wait(seconds(2));
      }

      void TransitionEffect::StepBack()
      {
         m_bActive = false;

         Main & main = HelperGetMain();

         int iTypeOld = m_iType;
         int iTypeNew = m_iType;
         if(m_etypea[iTypeOld] == TransitionEffectVisual)
         {
            m_iVisual--;
            if(m_iVisual < 0)
            {
               m_iVisual = 0;
               iTypeNew = iTypeOld - 1;
            }
         }
         else
         {
             iTypeNew = iTypeOld - 1;
         }
         if(iTypeNew < 0)
         {
            iTypeNew = m_etypea.get_size() - 1;
            if(m_etypea[iTypeNew] == TransitionEffectVisual)
            {
               m_iVisual = main.GetVisualEffect().GetEffectCount() - 1;
            }
         }
         if(iTypeNew >= m_etypea.get_size())
            iTypeNew = m_etypea.get_size() - 1;
         if(m_etypea[iTypeNew] == TransitionEffectVisual &&
            m_etypea[iTypeOld] != TransitionEffectVisual)
         {
            m_iVisual = main.GetVisualEffect().GetEffectCount() - 1;
         }

         if(iTypeNew < 0 || iTypeNew >= m_etypea.get_size())
            m_iType = 0;
         else
            m_iType = iTypeNew;

      }

      void TransitionEffect::StepNext()
      {
         m_bActive = false;

         Main & main = HelperGetMain();

         int iTypeOld = m_iType;
         int iTypeNew = m_iType;
         if(m_etypea[iTypeOld] == TransitionEffectVisual)
         {
            m_iVisual++;
            if(m_iVisual >= m_iaVisual.get_size())
            {
               m_iVisual = 0;
               iTypeNew = iTypeOld + 1;
            }
         }
         else
         {
             iTypeNew = iTypeOld + 1;
         }
         
         if(iTypeNew >= m_etypea.get_size())
         {
            iTypeNew = 0;
            if(m_etypea[iTypeNew] == TransitionEffectVisual)
            {
               m_iVisual = 0;
            }
         }
         if(iTypeNew >= m_etypea.get_size())
            iTypeNew = m_etypea.get_size() - 1;
         if(m_etypea[iTypeNew] == TransitionEffectVisual &&
            m_etypea[iTypeOld] != TransitionEffectVisual)
         {
            m_iVisual = 0;
         }

         if(iTypeNew < 0 || iTypeNew >= m_etypea.get_size())
            m_iType = 0;
         else
            m_iType = iTypeNew;
         
      }

      void TransitionEffect::Initialize()
      {
         Main & main = HelperGetMain();

         Graphics & graphics = main.GetGraphics();

         //ASSERT(graphics.GetBufferDC().get_os_data() != NULL);

         single_lock sl1Back(&graphics.m_mutex1Back, FALSE);
//         ::ca::graphics & dcBack = graphics.GetBackDC();
//         ::ca::graphics & dcFrame1 = graphics.GetFrame1DC();


         main.OnAfterImageLoaded();
         m_bInitialized = true;
         m_bActive = true;

         HelperGetMain().DeferCheckLayout();
         m_eventStartTransition.SetEvent();

      }

      void TransitionEffect::_Final()
      {
         m_bActive = false;


         m_tool001.Finalize();
         m_bInitialized = false;

      }

      void TransitionEffect::Restart()
      {

         rect rectClient;

         Main & main = HelperGetMain();

         Interface & iface = main.GetInterface();
         iface.BackViewGetClientRect(rectClient);

         int cx = rectClient.width();
         int cy = rectClient.height();

         m_tool001.m_iStep = 1;
         m_tool001.m_iStepCount = -1;
         m_dwDelay = 200;

         m_tool001.Initialize(m_etypea[m_iType], cx, cy, *this);

         main.OnAfterImageLoaded();


      }

      bool TransitionEffect::TestEnd()
      {
         if(m_tool001.m_iStepCount > 1
            && m_tool001.m_iStep >= m_tool001.m_iStepCount)
         {
            m_bActive = false;
            return true;
         }

         if(!m_bActive)
            return true ;

         return false;

      }

      void TransitionEffect::End()
      {
         Main & main = HelperGetMain();
         main.ImageChangePostEvent(EventTransitionEffectFinished);

      }

      void TransitionEffect::Reset()
      {
         m_tool001.m_ia.remove_all();
         m_tool001.m_iStep = 0;
      }

      void TransitionEffect::RunStepProcLevel2()
      {

//         Main & viewinterface = HelperGetMain();

         keeper <bool> keepTransitionStepRunActive(
            &m_bTransitionStepRunActive,
            true,
            false,
   true);


         int iRepeat = CalcRepeatCount();


         rect_array recta;

         while(!TestEnd() && iRepeat > 0)
         {
            RunStepProcLevel1(recta);
            iRepeat--;
         }

         /*rect_array * precta = new rect_array;
         precta->copy(recta);
         viewinterface.GetInterface().BackViewPostMessage(
            Interface::BackViewWparamUpdateScreenBaseRectArray,
            (LPARAM) precta);*/

         m_dwLastTransitionTime = GetTickCount();
         m_dwTransitionStepLastRunTime = GetTickCount();
         m_bTransitionStepPostActive = false;

         if(TestEnd())
         {
            End();
         }

      }

      void TransitionEffect::RunStepProcLevel1(rect_array & recta)
      {

//         Main & viewinterface = HelperGetMain();

         if(TestEnd())
            return;

         if(m_tool001.m_iStep == 1
            && m_tool001.m_ia.get_size() <= 0)
         {
            RenderBuffer(recta);
         }
         else
         {
            if(m_tool001.m_ia.get_size() <= 0)
            {
               RenderBuffer(recta);
            }
            else
            {
               rect rect;
               for(int i = 0; i < 1 && m_tool001.m_ia.get_size(); i++)
               {
                  RenderBuffer(recta);
               }
            }
         }

         //Sleep(min(1984, m_dwDelay));

         m_tool001.m_iStep++;
      }

   /*   void TransitionEffect::RunStep_()
      {
         m_evRunStep.SetEvent();
      }*/


      void TransitionEffect::RenderBuffer(rect_array & recta)
      {

         if(!m_bInitialized)
            Initialize();


         recta.remove_all();



         Main & main = HelperGetMain();

         if(m_etypea[m_iType] == TransitionEffectVisual)
         {
            main.GetVisualEffect().RenderBuffer(recta);
            return;
         }

         Interface & iface = main.GetInterface();
         rect rectClient;
         iface.BackViewGetClientRect(rectClient);

         int cx = rectClient.width();
         int cy = rectClient.height();

         class imaging & imaging = System.imaging();

         Graphics & graphics = main.GetGraphics();

         ::ca::draw_dib & drawdib = graphics.GetDrawDib();

//         ASSERT(graphics.GetBufferDC().get_os_data() != NULL);

         single_lock sl1Back(&graphics.m_mutex1Back, FALSE);
         single_lock sl2Buffer(&graphics.m_mutex2Buffer, FALSE);
         ::ca::graphics & dcBack = graphics.GetBackDC();
         ::ca::graphics & dcBuffer = graphics.GetBufferDC();
         ::ca::graphics & dcFrame1 = graphics.GetFrame1DC();

         HelperGetMain().DeferCheckLayout();

         ::ca::bitmap & bitmapBuffer = graphics.GetBufferBitmap();
         ::ca::dib * pdibBuffer = graphics.GetDib(_graphics::DibBuffer);
         ::ca::dib * pdibBack = graphics.GetDib(_graphics::DibBack);
         if(dcBack.get_os_data() == NULL)
         {
            End();
            return;
         }
         if(dcBuffer.get_os_data() == NULL)
         {
            End();
            return;
         }

         if(!sl1Back.lock(millis(184)))
            return;
         if(!sl2Buffer.lock(millis(184)))
            return;
         dcBack.SelectClipRgn(NULL);
         dcBuffer.SelectClipRgn(NULL);

         if(bitmapBuffer.get_os_data() == NULL)
         {
            End();
            return;
         }

         class size  sizeBack2 = bitmapBuffer.get_size();

         rect rectUpdate;
         switch(m_etypea[m_iType])
         {
         case TransitionEffectSingleStep:
            {
               if(m_tool001.m_iStep == m_tool001.m_iStepCount / 2)
               {
                  rectUpdate = rectClient;
                  drawdib.draw(
                     &dcBack,
                     rectUpdate.left, rectUpdate.top,
                     rectUpdate.width(), rectUpdate.height(),
                     graphics.GetDib(_graphics::DibBuffer),
                     rectUpdate.left, rectUpdate.top,
                     rectUpdate.width(), rectUpdate.height(),
                     SRCCOPY);
                  recta.add(rectUpdate);
               }
            }
            break;
         case TransitionEffectSquarypixelate_:
            {
               if(m_tool001.m_iStep > 0)
               {
                  if(m_tool001.m_pointa.get_size() <= 0)
                     break;
                  int iIndex = System.math().RandRange(
                           0,
                           m_tool001.m_pointa.get_size() - 1);



                  if(m_tool001.m_iStep <= 0)
                           break;;
                  point point;
                  try
                  {
                           point = m_tool001.m_pointa.element_at(iIndex);
                           if(m_tool001.m_iStep <= 0)
                              break;
                           m_tool001.m_pointa.remove_at(iIndex);
                  }
                  catch(...)
                  {
                           break;
                  }
                  POINT pointa[4];
                  m_tool001.GetRotateRect(point.x, point.y, pointa);
                  ::ca::region_sp rgnClip(get_app());
                  rgnClip->CreatePolygonRgn(
                           pointa,
                           4,
                           WINDING);

                  //dcBack.SelectClipRgn(NULL);
                  dcBack.SelectClipRgn(rgnClip);
                  m_tool001.GetSimplePolyBox(&rectUpdate, pointa, 4);

                  /*               if(lprect != NULL)
                  {
                  *lprect = rectUpdate;
               }*/

                  dcBack.BitBlt(
                     rectUpdate.left, rectUpdate.top,
                     rectUpdate.width(), rectUpdate.height(),
                     &dcBuffer,
                     rectUpdate.left, rectUpdate.top,
                     SRCCOPY);
                  /*drawdib.draw(
                     dcBack,
                     rectUpdate.left, rectUpdate.top,
                     rectUpdate.width(), rectUpdate.height(),
                     graphics.GetDib(_graphics::DibBuffer),
                     rectUpdate.left, rectUpdate.top,
                     rectUpdate.width(), rectUpdate.height(),
                     SRCCOPY);*/
                  dcBack.SelectClipRgn(NULL);
                  recta.add(rectUpdate);
               }
            }
            break;
         case TransitionEffectHexagonalpixelate_:
            {
               if(m_tool001.m_iStep > 0)
               {
                  int iIndex = System.math().RandRange(
                     0,
                     m_tool001.m_pointa.get_size() - 1);

                  if(m_tool001.m_iStep <= 0)
                     break;
                  point point;
                  try
                  {
                     point = m_tool001.m_pointa.element_at(iIndex);
                     if(m_tool001.m_iStep <= 0)
                        break;
                     m_tool001.m_pointa.remove_at(iIndex);
                  }
                  catch(...)
                  {
                     break;
                  }
                  POINT pointa[6];
                  m_tool001.GetRotateHexagon(point.x, point.y, pointa);
                  ::ca::region_sp rgnClip(get_app());
                  rgnClip->CreatePolygonRgn(
                     pointa,
                     6,
                     WINDING);
                  dcBack.SelectClipRgn(rgnClip);
                  m_tool001.GetSimplePolyBox(rectUpdate, pointa, 6);

                  /*               if(lprect != NULL)
                  {
                  *lprect = rectUpdate;
                     }*/

                  dcBack.BitBlt(
                     rectUpdate.left, rectUpdate.top,
                     rectUpdate.width(), rectUpdate.height(),
                     &dcBuffer,
                     rectUpdate.left, rectUpdate.top,
                     SRCCOPY);
                  dcBack.SelectClipRgn(NULL);
               }
               recta.add(rectUpdate);
            }
            break;
         case TransitionEffectCirclypixelate_:
         case TransitionEffectEllipsoidalpixelate_:
            {
               int finalX = 0;
               int finalY = 0;
               int finalW = cx;
               int finalH = cy;

               const int tilesx = max(4, m_tool001.m_data.m_tiles.m_iTilesX);
               const int tilesy = max(4, m_tool001.m_data.m_tiles.m_iTilesY);
//               const int tiles = max(16, m_tool001.m_data.m_tiles.m_iTiles);


               if(m_tool001.m_iStep > 0)
               {
                  if(m_tool001.m_ia.get_size() > 0)
                  {
                     int iIndex = System.math().RandRange(
                        0,
                        m_tool001.m_ia.get_size() - 1);

                     //                  SendMessage(WM_USER, USERMESSAGE_WPARAM_RAND_LPINT, (LPARAM) &iRand);

                     /*                  int iIndex =
                     m_math.LinearMap(
                     0, m_tool001.m_ia.get_size() - 1,
                     iRand, 0, RAND_MAX);*/
                     if(m_tool001.m_iStep <= 0)
                        break;
                     int iStep = 0;
                     try
                     {
                        iStep = m_tool001.m_ia.element_at(iIndex);
                        if(m_tool001.m_iStep <= 0)
                           break;
                        m_tool001.m_ia.remove_at(iIndex);
                     }
                     catch(...)
                     {
                        break;
                     }

                     int i = iStep % tilesx;
                     int j = iStep / tilesx;
                     double dUpdateH = (double) finalH / tilesy;
                     double dUpdateW = (double) finalW / tilesx;
                     double dX = dUpdateW * i;
                     double dY = dUpdateH * j;
                     rectUpdate.set(
                        (int) dX,
                        (int) dY,
                        (int) (dX + dUpdateW + 1.0),
                        (int) (dY + dUpdateH + 1.0));
                  }
                  else
                  {
                     rectUpdate.set(finalX, finalY, finalX + finalW, finalY + finalH);
                  }
                  /*               if(lprect != NULL)
                  {
                  *lprect = rectUpdate;
                     }*/

                  ::ca::region_sp rgnClip(get_app());
                  if(m_etypea[m_iType] == TransitionEffectCirclypixelate_
                     || m_etypea[m_iType] == TransitionEffectEllipsoidalpixelate_)
                  {
                     rect rectEllipse;
                     if(m_etypea[m_iType] == TransitionEffectCirclypixelate_)
                     {
                        m_tool001.FindRandomEnglobingCircle(rectUpdate, rectEllipse, 64);
                     }
                     else
                     {
                        m_tool001.FindRandomEnglobingEllipse(rectUpdate, rectEllipse, 64);
                     }
                     rectUpdate = rectEllipse;
                     rgnClip->CreateEllipticRgnIndirect(rectUpdate);
                     dcBack.SelectClipRgn(rgnClip);
                     dcBack.BitBlt(
                        rectUpdate.left, rectUpdate.top,
                        rectUpdate.width(), rectUpdate.height(),
                        &dcBuffer,
                        rectUpdate.left, rectUpdate.top,
                        SRCCOPY);
                     dcBack.SelectClipRgn(NULL);
                  }
                  else
                  {
                     dcBack.BitBlt(
                        rectUpdate.left, rectUpdate.top,
                        rectUpdate.width(), rectUpdate.height(),
                        &dcBuffer,
                        rectUpdate.left, rectUpdate.top,
                        SRCCOPY);

                  }
                  recta.add(rectUpdate);
               }

            }
            break;
         case TransitionEffectLinearFadingBottomTop:
         case TransitionEffectLinearFadingTopBottom:
         case TransitionEffectLinearFadingLeftRight:
         case TransitionEffectLinearFadingRightLeft:
            {

               int finalX = 0;
               int finalY = 0;
               int finalW = cx;
               int finalH = cy;

               ::rect & rectUpdate = m_tool001.m_rect;

               if(m_tool001.m_iStep == 1)
               {

                  graphics.GetDib(_graphics::DibTemp1)->from(pdibBack);

               }

               pdibBack->from(graphics.GetDib(_graphics::DibTemp1));

               double dRate = ((double) (m_tool001.m_iStep + 1.0)) / ((double) m_tool001.m_iStepCount);

               rectUpdate.set(finalX, finalY, finalX + finalW, finalY + finalH);

               dcBack.set_alpha_mode(::ca::alpha_mode_blend);

               dcBack.alpha_blend(rectUpdate.size(), &dcBuffer, dRate);

               recta.add(rectUpdate);

            }
            break;
         case TransitionEffectAccumulLinearFadingBottomTop:
         case TransitionEffectAccumulLinearFadingTopBottom:
         case TransitionEffectAccumulLinearFadingLeftRight:
         case TransitionEffectAccumulLinearFadingRightLeft:
            {
               int finalX = 0;
               int finalY = 0;
               int finalW = cx;
               int finalH = cy;

               ::rect & rectUpdate = m_tool001.m_rect;

               const int iTileCount = m_tool001.m_data.m_sliceframe.m_iTileCount;
               //            const int iFrameCount = m_tool001.m_data.m_sliceframe.m_iFrameCount;
               const int iTileMax = iTileCount - 1;
               int iGroupCount = m_tool001.m_data.m_sliceframe.m_iGroupCount;

               int iIndex = m_tool001.m_iStep - 1;
               iIndex = min(iIndex, m_tool001.m_pointa.get_size() -1);
               if(iIndex >= 0)
               {
                  point point = m_tool001.m_pointa.element_at(iIndex);
                  int iTile = point.x;
                  //int iFrame = point.y;
                  int iGroup = point.y;
                  //int iGroup = iTile - iFrame;
                  double dRate = (double) iTile / iTileMax;
                  double dRateMinus = (double) (iTile - 1) / iTileMax;



                  //int iFrameComplement = iFrameCount - iFrame;
                  //int iFrameExp = iFrameComplement * iFrameComplement * iFrameComplement;
                  //int iFrameStd = iFrame * 2;
                  //               int iFrameStd = iFrame;
                  //>int iFrameEx = iFrameStd * iFrameStd * iFrameStd;
                  //int iFrameEx = iFrameStd * iFrameStd;
                  //               int iFrameEx = iFrameStd;
                  //               int iFrameStdCount = iFrameCount - 1;
                  //int iFrameCountEx = iFrameStdCount * iFrameStdCount * iFrameStdCount;
                  //int iFrameCountEx = iFrameStdCount * iFrameStdCount;
                  //               int iFrameCountEx = iFrameStdCount;
                  double dAlpha = m_tool001.m_dAlpha;
                  /*            double dAlpha = viewinterface.m_math.LinearMap(
                  (double) 35, (double) 255,
                  (double) iFrameEx,
                  (double) 0, (double) iFrameCountEx);*/
                  //dAlpha /= 8.0;

                  //m_dwDelay = (DWORD) min(10, dAlpha * 10 / 255);
                //  m_dwDelay = 0;

                  int iAlphaAccumul = 0;;


                  if(m_tool001.m_iStep > 0
                     && m_tool001.m_iStepCount > 0)
                  {
                     m_tool001.Start(
                        cx, cy,
                        dRateMinus, dRate);
                     m_tool001.Go();
                     iAlphaAccumul = m_tool001.m_ia.element_at(iTile);
                     //                  if(iFrame == iFrameStdCount)
                     if(iGroup == 0)
                     {
                        dAlpha = 255.0;
                     }
                     else
                     {
                        //dAlpha = max(1.0, 255.0 / pow((double) (iGroup), 2.0));
               //         dAlpha = 255.0 * pow((double) (iGroupCount - iGroup) / iGroupCount, 2.5);
   //
     //                               if(iFrame == iFrameCount - 1)
       //        {
         //         dAlpha = 255.0;
           //    }
             //  else
               {
                  double dExp = 3.4;
                  double dMin = 4.0;
                  dAlpha = 140.0 * ::pow((iGroupCount - 1.0 - iGroup) / (iGroupCount - 1.0), dExp) + dMin;
               }

                        /*                     dAlpha = iAlphaAccumul / 2.0 + max(5.0, 255.0 / iFrameCount);
                        dAlpha = dAlpha / 255.0;
                        dAlpha *= dAlpha;
                        dAlpha *= dAlpha;
                        dAlpha *= 255.0;
                        dAlpha = iAlphaAccumul / 2.0 + max(2.0, 127.0 / iFrameCount);
                        dAlpha = max(2.0, min(255.0, dAlpha));*/
                     }
                     if(iAlphaAccumul <= 255)
                     {
                        iAlphaAccumul = (int) ((dAlpha * 255.0 +
                           iAlphaAccumul * (255.0 - dAlpha)) / 255);
                     }
                     if(iAlphaAccumul >= 255)
                     {
                        m_tool001.m_ia.element_at(iTile) = 256;
                     }
                     else
                     {
                        m_tool001.m_ia.element_at(iTile) = iAlphaAccumul;
                     }
                  }
                  else
                  {
                     rectUpdate.set(finalX, finalY, finalX + finalW, finalY + finalH);
                  }
                  /*            if(lprect != NULL)
                  {
                  *lprect = rectUpdate;
               }*/



                  if(iAlphaAccumul >= 0
                     && iAlphaAccumul <= 255)
                  {
   /*                  imaging.bitmap_blend(
                        &dcBack,
                        rectUpdate.left,
                        rectUpdate.top,
                        rectUpdate.width(),
                        rectUpdate.height(),
                        &dcBuffer,
                        rectUpdate.left,
                        rectUpdate.top,
                        (BYTE) dAlpha);*/

                     graphics.GetDib(_graphics::DibTemp1)->create(rectUpdate.width(), rectUpdate.height());
                     graphics.GetDib(_graphics::DibTemp2)->create(rectUpdate.width(), rectUpdate.height());

                     graphics.GetDib(_graphics::DibTemp1)->get_graphics()->BitBlt(
                        0, 0,
                        rectUpdate.width(),
                        rectUpdate.height(),
                        &dcBack,
                        rectUpdate.left,
                        rectUpdate.top,
                        SRCCOPY);

                     graphics.GetDib(_graphics::DibTemp2)->get_graphics()->BitBlt(
                        0, 0,
                        rectUpdate.width(),
                        rectUpdate.height(),
                        &dcBuffer,
                        rectUpdate.left,
                        rectUpdate.top,
                        SRCCOPY);

                     imaging.bitmap_blend(
                        graphics.GetDib(_graphics::DibTemp1)->get_graphics(),
                        null_point(),
                        rectUpdate.size(),
                        graphics.GetDib(_graphics::DibTemp2)->get_graphics(),
                        null_point(),
                        (BYTE) dAlpha);

                     dcBack.from(
                        rectUpdate.top_left(),
                        rectUpdate.size(),
                        graphics.GetDib(_graphics::DibTemp1)->get_graphics(),
                        null_point(),
                        SRCCOPY);


                  }
                  recta.add(rectUpdate);
               }
            }
            break;
         case TransitionEffectFlyInBottomTop:
         case TransitionEffectFlyInTopBottom:
         case TransitionEffectFlyInLeftRight:
         case TransitionEffectFlyInRightLeft:
         case TransitionEffectFlyInLeftTop:
         case TransitionEffectFlyInLeftBottom:
         case TransitionEffectFlyInRightBottom:
         case TransitionEffectFlyInRightTop:
            {
//               int finalX = 0;
//               int finalY = 0;
//               int finalW = cx;
//               int finalH = cy;

               const int iFrameCount = max(1, m_tool001.m_data.m_sliceframe.m_iFrameCount);
               const int iFrameMax = iFrameCount -1;



               int iFrame = m_tool001.m_iStep - 1;
               double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);
               double dRate = 1.0 - dFrameStd * dFrameStd;
               double dComplementRate = 1.0 - dRate;

               rect rectBound;
               graphics.GetFinalPlacement(rectBound);

               rect rect(rectBound);

               Space space;
               space.Deviate(rect, rectBound, GetDirection(m_etypea[m_iType]), dComplementRate);

               class rect rectIntersect;
               rectIntersect.intersect(rect, rectBound);

               dcBack.BitBlt(
                  rectIntersect.left,
                  rectIntersect.top,
                  rectIntersect.width(),
                  rectIntersect.height(),
                  &dcBuffer,
                  rectIntersect.left - rect.left + rectBound.left,
                  rectIntersect.top - rect.top + rectBound.top,
                  SRCCOPY);

               recta.add(rectIntersect);

            }
            break;
         case TransitionEffectWipeTop:
         case TransitionEffectWipeBottom:
         case TransitionEffectWipeRight:
         case TransitionEffectWipeLeft:
            {
//               int finalX = 0;
//               int finalY = 0;
//               int finalW = cx;
//               int finalH = cy;

               const int iFrameCount = max(4, m_tool001.m_data.m_sliceframe.m_iFrameCount);
               const int iFrameMax = iFrameCount;

               int iFrame = m_tool001.m_iStep - 1;
               double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);
               double dFrameStdPlus = 1.0 - ((double) (iFrame + 1) / iFrameMax);
               double dRate = 1.0 - dFrameStd * dFrameStd;

               double dRatePlus = 1.0 - dFrameStdPlus * dFrameStdPlus;


               rect rect;
               graphics.GetFinalPlacement(rect);

               Space space;

               space.Slice(rect, rectClient, GetDirection(get_type()), dRate, dRatePlus);

               dcBack.BitBlt(
                  rect.left,
                  rect.top,
                  rect.width(),
                  rect.height(),
                  &dcBuffer,
                  rect.left,
                  rect.top,
                  SRCCOPY);
               recta.add(rect);
            }
            break;
         case TransitionEffectWipeTopLeft:
         case TransitionEffectWipeTopRight:
         case TransitionEffectWipeBottomLeft:
         case TransitionEffectWipeBottomRight:
            {
//               int finalX = 0;
//               int finalY = 0;
//               int finalW = cx;
//               int finalH = cy;

               const int iFrameCount = max(4, m_tool001.m_data.m_sliceframe.m_iFrameCount);
               const int iFrameMax = iFrameCount - 1;

               int iFrame = m_tool001.m_iStep - 1;
//               double dFrameStdMinus = 1.0 - ((double) (iFrame - 1)/ iFrameMax);
               double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);
               double dFrameStdPlus = 1.0 - ((double) (iFrame + 1) / iFrameMax);
//               double dRateMinus = 1.0 - dFrameStdMinus * dFrameStdMinus;
               double dRate = 1.0 - dFrameStd * dFrameStd;
               double dRatePlus = 1.0 - dFrameStdPlus * dFrameStdPlus;

               ::rect rectA;
               ::rect rectB;
               ::rect rectC;
//               ::rect & rectUpdate = m_tool001.m_rect;


               

               rect rect;
               graphics.GetFinalPlacement(rect);


               Space space;
               space.Slice(rectA, rectB, rectC, rect, GetDirection(get_type()), dRate, dRatePlus);


               dcBack.BitBlt(
                  rectA.left,
                  rectA.top,
                  rectA.width(),
                  rectA.height(),
                  &dcBuffer,
                  rectA.left,
                  rectA.top,
                  SRCCOPY);
               dcBack.BitBlt(
                  rectB.left,
                  rectB.top,
                  rectB.width(),
                  rectB.height(),
                  &dcBuffer,
                  rectB.left,
                  rectB.top,
                  SRCCOPY);
               dcBack.BitBlt(
                  rectC.left,
                  rectC.top,
                  rectC.width(),
                  rectC.height(),
                  &dcBuffer,
                  rectC.left,
                  rectC.top,
                  SRCCOPY);
               recta.add(rectA);
               recta.add(rectB);
               recta.add(rectC);
            }
            break;
         case TransitionEffectWipeCenter:
            {

               const int iFrameCount = max(4, m_tool001.m_data.m_sliceframe.m_iFrameCount);
               const int iFrameMax = iFrameCount - 1;

               int iFrame = m_tool001.m_iStep - 1;
               double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);
               double dRate = 1.0 - dFrameStd * dFrameStd;

               rect rect;
               graphics.GetFinalPlacement(rect);

               Space space;
               space.Scale(rect, dRate);


               dcBack.BitBlt(
                  rect.left,
                  rect.top,
                  rect.width(),
                  rect.height(),
                  &dcBuffer,
                  rect.left,
                  rect.top,
                  SRCCOPY);
               recta.add(rect);
            }
            break;
         case TransitionEffectWipeIn:
            {

               const int iFrameCount = max(4, m_tool001.m_data.m_sliceframe.m_iFrameCount);
               const int iFrameMax = iFrameCount - 1;

               int iFrame = m_tool001.m_iStep - 1;
               double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);
               double dFrameStdPlus = 1.0 - ((double) (iFrame + 1) / iFrameMax);
               double dRate = 1.0 - dFrameStd * dFrameStd;
               double dRatePlus = 1.0 - dFrameStdPlus * dFrameStdPlus;

               ::rect rectA;
               ::rect rectB;
               ::rect rectC;
               ::rect rectD;

               class rect rect;
               graphics.GetFinalPlacement(rect);

               Space space;
               space.WipeIn(
                  rectA,
                  rectB,
                  rectC,
                  rectD,
                  rect,
                  1.0 - dRatePlus,
                  1.0 - dRate);

               dcBack.BitBlt(
                  rectA.left,
                  rectA.top,
                  rectA.width(),
                  rectA.height(),
                  &dcBuffer,
                  rectA.left,
                  rectA.top,
                  SRCCOPY);
               dcBack.BitBlt(
                  rectB.left,
                  rectB.top,
                  rectB.width(),
                  rectB.height(),
                  &dcBuffer,
                  rectB.left,
                  rectB.top,
                  SRCCOPY);
               dcBack.BitBlt(
                  rectC.left,
                  rectC.top,
                  rectC.width(),
                  rectC.height(),
                  &dcBuffer,
                  rectC.left,
                  rectC.top,
                  SRCCOPY);
               dcBack.BitBlt(
                  rectD.left,
                  rectD.top,
                  rectD.width(),
                  rectD.height(),
                  &dcBuffer,
                  rectD.left,
                  rectD.top,
                  SRCCOPY);
               recta.add(rectA);
               recta.add(rectB);
               recta.add(rectC);
               recta.add(rectD);
            }
            break;
         case TransitionEffectWipeMidHorizontal:
            {

               const int iFrameCount = max(4, m_tool001.m_data.m_sliceframe.m_iFrameCount);
               const int iFrameMax = iFrameCount - 1;

               int iFrame = m_tool001.m_iStep - 1;
               double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);
               double dRate = dFrameStd * dFrameStd;

               rect rect;
               graphics.GetFinalPlacement(rect);

               double dy = (rect.height() / 2) * dRate;

               rect.top += dy;
               rect.bottom -= dy;

               dcBack.BitBlt(
                  rect.left,
                  rect.top,
                  rect.width(),
                  rect.height(),
                  &dcBuffer,
                  rect.left,
                  rect.top,
                  SRCCOPY);
               recta.add(rect);
            }
            break;
         case TransitionEffectWipeMidVertical:
            {

               const int iFrameCount = max(4, m_tool001.m_data.m_sliceframe.m_iFrameCount);
               const int iFrameMax = iFrameCount - 1;

               int iFrame = m_tool001.m_iStep - 1;
               double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);
               double dRate = dFrameStd * dFrameStd;

               rect rect;
               graphics.GetFinalPlacement(rect);

               double dx = (rect.width() / 2) * dRate;

               rect.left += dx;
               rect.right -= dx;

               dcBack.BitBlt(
                  rect.left,
                  rect.top,
                  rect.width(),
                  rect.height(),
                  &dcBuffer,
                  rect.left,
                  rect.top,
                  SRCCOPY);
               recta.add(rect);
            }
            break;
         case TransitionEffectScaleBottom:
         case TransitionEffectScaleTop:
         case TransitionEffectScaleRight:
         case TransitionEffectScaleLeft:
         case TransitionEffectScaleTopLeft:
         case TransitionEffectScaleTopRight:
         case TransitionEffectScaleBottomLeft:
         case TransitionEffectScaleBottomRight:
            {
//               int finalX = 0;
//               int finalY = 0;
//               int finalW = cx;
//               int finalH = cy;

               const int iFrameCount = max(4, m_tool001.m_data.m_sliceframe.m_iFrameCount);
               const int iFrameMax = iFrameCount - 1;

               int iFrame = m_tool001.m_iStep - 1;
               double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);

               double dRate = dFrameStd * dFrameStd * dFrameStd;

               rect rectBound;
               graphics.GetFinalPlacement(rectBound);


               class rect rect(rectBound);

               Space space;

               space.Deviate(rect, rectClient, GetDirection(get_type()), dRate);

               rect.intersect(rect, rectBound);

               dcBack.SetStretchBltMode(HALFTONE);
               dcBack.StretchBlt(
                  rect.left,
                  rect.top,
                  rect.width(),
                  rect.height(),
                  &dcBuffer,
                  rectBound.left,
                  rectBound.top,
                  rectBound.width(),
                  rectBound.height(),
                  SRCCOPY);

               dcBack.SelectClipRgn(NULL);

               recta.add(rect);
            }
            break;
         case TransitionEffectScaleCenter:
            {
//               int finalX = 0;
//               int finalY = 0;
//               int finalW = cx;
//               int finalH = cy;

               const int iFrameCount = max(4, m_tool001.m_data.m_sliceframe.m_iFrameCount);
               const int iFrameMax = iFrameCount - 1;

               int iFrame = m_tool001.m_iStep - 1;
               double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);

               double dRate = dFrameStd * dFrameStd;

               rect rect = rectClient;

               Space space;

               space.Scale(rect, dRate);



               dcBack.StretchBlt(
                  rectClient.left,
                  rectClient.top,
                  rectClient.width(),
                  rectClient.height(),
                  &dcBuffer,
                  rect.left,
                  rect.top,
                  rect.width(),
                  rect.height(),
                  SRCCOPY);
               dcBack.SelectClipRgn(NULL);
               recta.add(rectClient);
            }
            break;
         case TransitionEffectpixelate_BottomTop:
         case TransitionEffectpixelate_TopBottom:
         case TransitionEffectpixelate_LeftRight:
         case TransitionEffectpixelate_RightLeft:
            {


               rect rectBound;
               graphics.GetFinalPlacement(rectBound);

               

               int finalX = rectBound.left;
               int finalY = rectBound.top;
               int finalW = rectBound.width();
               int finalH = rectBound.height();


               rect rectDest;

               rectDest.left = 0;
               rectDest.top = 0;
               rectDest.right = finalW;
               rectDest.bottom = finalH;


               ::rect rectUpdate;

               int c1 = m_tool001.m_data.m_alphapixelate.m_c1;
               int iIndex = m_tool001.m_iStep - 1;
               int iSizeIndex = m_tool001.m_pointa[iIndex].x;
               int iSize = m_tool001.m_ia[iSizeIndex];
               int nextc = m_tool001.m_pointa[iIndex].y;
               int c;
               
               if(iIndex <= 0 || nextc == 0)
               {
                  c = 0;
               }
               else
               {
                  c = m_tool001.m_pointa[iIndex - 1].y;
               }


               m_dwDelay = 84;

               ::ca::dib * pdib = graphics.GetDib(7);

               if(iIndex >= 0)
               {
                  double dRateMinus = (double) c / (double) c1;
                  double dRate = (double) nextc / (double) c1;

                  if(dRateMinus < 0.0)
                     dRateMinus = 0.0;
                  else if(dRateMinus > 1.0)
                     dRateMinus = 1.0;

                  if(dRate < 0.0)
                     dRate = 0.0;
                  else if(dRate > 1.0)
                     dRate = 1.0;

                  if(dRateMinus > dRate)
                     dRateMinus = dRate;

                  double dAlpha;

                  {
                     double dBeginSpanTime = 0.30;
                     double dEndSpanTime = 0.05;
                     double dTime = (double) m_tool001.m_iStep / (double) m_tool001.m_iStepCount;
                     double dRate = 8.0;
                     double dMiddle = 0.87;
                     double dMiddleAlpha = 107.0;

                     if(dTime < dMiddle)
                     {
                        if(dTime < dBeginSpanTime)
                        {
                           dAlpha = 0.0;
                        }
                        else
                        {
                           dAlpha = dMiddleAlpha * ::pow(2.7182, ((dTime * (1.0 / (1.0 - (dBeginSpanTime + dEndSpanTime)))) - dMiddle) * dRate);
                        }
                     }
                     else
                     {
                        if(dTime > (1.0 - dEndSpanTime))
                        {
                           dAlpha = 255.0;
                        }
                        else
                        {
                           dAlpha = dMiddleAlpha + (255.0 - dMiddleAlpha) * (1.0 - ::pow(2.7182, (dMiddle - (dTime * (1.0 / (1.0 - (dBeginSpanTime + dEndSpanTime))))) * dRate));
                        }
                     }
                  }




                  if(nextc == 0)
                  {

                     m_tool001.m_data.m_alphapixelate.m_iSizeIndex = iSizeIndex;

/*                     ::ca::dib * pdib2 = graphics.GetDib(2);

                     ::ca::dib * pdib3 = graphics.GetDib(3);

                     ::ca::dib * pdib4 = graphics.GetDib(4);

                     ::ca::dib * pdib5 = graphics.GetDib(5);

                     ::ca::dib * pdib6 = graphics.GetDib(6);

                     pdib->create(xPixelMod, yPixelMod);

                     pdib2->create(xPixelMod, yPixelMod);

                     pdib3->create(finalW, finalH);

                     pdib4->create(finalW, finalH);

                     pdib5->create(xPixelMod, yPixelMod);

                     pdib6->create(xPixelMod, yPixelMod);

                     pdib5->get_graphics()->SetStretchBltMode(0);
                     pdib6->get_graphics()->SetStretchBltMode(0);
                     dcBuffer.SetStretchBltMode(0);
                     dcFrame1.SetStretchBltMode(0);

                     pdib5->get_graphics()->StretchBlt(
                        0, 0,
                        xPixelMod, yPixelMod,
                        &dcBuffer,
                        finalX, finalY,
                        finalW, finalH,
                        SRCCOPY);

                     pdib6->get_graphics()->StretchBlt(
                        0, 0,
                        xPixelMod, yPixelMod,
                        &dcFrame1,
                        finalX, finalY,
                        finalW, finalH,
                        SRCCOPY);


                     pdib3->get_graphics()->SetStretchBltMode(0);
                     pdib4->get_graphics()->SetStretchBltMode(0);
                     pdib5->get_graphics()->SetStretchBltMode(0);
                     pdib6->get_graphics()->SetStretchBltMode(0);

                     pdib3->get_graphics()->StretchBlt(
                        0, 0,
                        finalW, finalH,
                        pdib5->get_graphics(),
                        0, 0,
                        xPixelMod, yPixelMod,
                        SRCCOPY);

                     pdib4->get_graphics()->StretchBlt(
                        0, 0,
                        finalW, finalH,
                        pdib6->get_graphics(),
                        0, 0,
                        xPixelMod, yPixelMod,
                        SRCCOPY);

                     pdib->get_graphics()->SetStretchBltMode(0);
                     pdib2->get_graphics()->SetStretchBltMode(0);
                     pdib3->get_graphics()->SetStretchBltMode(0);
                     pdib4->get_graphics()->SetStretchBltMode(0);

                     pdib->get_graphics()->StretchBlt(
                        0, 0,
                        xPixelMod, yPixelMod,
                        pdib3->get_graphics(),
                        0, 0,
                        finalW, finalH,
                        SRCCOPY);

                     pdib2->get_graphics()->StretchBlt(
                        0, 0,
                        xPixelMod, yPixelMod,
                        pdib4->get_graphics(),
                        0, 0,
                        finalW, finalH,
                        SRCCOPY);*/

                     pdib->create(finalW, finalH);

                     pdib->get_graphics()->BitBlt(0, 0, finalW, finalH, &dcFrame1, 0, 0, SRCCOPY);

                     const int dSpan = 16;

                     if(iSizeIndex == m_tool001.m_ia.get_size() - 1)
                     {
                        dAlpha = 255.0;
                     }
                     else
                     {
                        if(dAlpha < dSpan)
                           dAlpha = 0.0;
                        else if(dAlpha > 255 - dSpan)
                           dAlpha = 255.0;
                        else
                        {
                           dAlpha = (dAlpha - dSpan) * 255.0 / (255.0 - dSpan * 2);
                        }
                     }

                     imaging.bitmap_blend(
                        pdib->get_graphics(),
                        null_point(),
                        size(finalW, finalH),
                        &dcBuffer,
                        point(finalX, finalY),
                        (byte)(dAlpha));

                     pdib->pixelate(iSize);
                  }

                  switch(m_etypea[m_iType])
                  {
                  case TransitionEffectpixelate_TopBottom:
                     {
                        m_tool001.GetSliceRect(finalW, finalH, rectDest, gcom::backview::TransitionEffect::Tool001::AlignTop, dRateMinus, dRate);
                     }
                     break;
                  case TransitionEffectpixelate_BottomTop:
                     {
                        m_tool001.GetSliceRect(finalW, finalH, rectDest, gcom::backview::TransitionEffect::Tool001::AlignBottom, dRateMinus, dRate);
                     }
                     break;
                  case TransitionEffectpixelate_LeftRight:
                     {
                        m_tool001.GetSliceRect(finalW, finalH, rectDest, gcom::backview::TransitionEffect::Tool001::AlignLeft, dRateMinus, dRate);
                     }
                     break;
                  case TransitionEffectpixelate_RightLeft:
                     {
                        m_tool001.GetSliceRect(finalW, finalH, rectDest, gcom::backview::TransitionEffect::Tool001::AlignRight, dRateMinus, dRate);
                     }
                     break;
                  }

                  rectDest.offset(rectBound.top_left());

                  rectDest.right +=1;
                  rectDest.bottom +=1;


                  dcBack.BitBlt(
                     rectDest.left,
                     rectDest.top,
                     min(rectDest.width(), finalW),
                     min(rectDest.height(), finalH),
                     pdib->get_graphics(),
                     rectDest.left - rectBound.left,
                     rectDest.top - rectBound.top,
                     SRCCOPY);

                  
                  recta.add(rectDest);

               }
            }
            break;
            case TransitionEffectRadialUnveil:
               {
//                  ::ca::dib * pdibT1 = graphics.GetDib(1977); // Radial Fill (White, Radius = 256 pixels)
                  ::ca::dib * pdibT2 = graphics.GetDib(1977); // Radial Fill (White, Radius = 256 pixels)

                  int iIndex = m_tool001.m_iStep - 1;
                  m_tool001.m_data.m_radialunveil.m_iRadius += m_tool001.m_data.m_radialunveil.m_iRadiusIncrement;

                  m_dwDelay = m_tool001.m_pointa[iIndex].y;

                  int r = m_tool001.m_pointa[iIndex].x;
                  int d = r * 2;



                  int xm = cx / 2;
                  int ym = cy / 2;
                  int x1 = max(0, xm - r);
                  int x2 = min(cx, xm + r);
                  int y1 = max(0, ym - r);
                  int y2 = min(cy, ym + r);


                  int xOff = 0;
                  int yOff = 0;
                  if(r > xm)
                     xOff = pdibT2->width() * (r - xm) / d;

                  if(r > ym)
                     yOff = pdibT2->height() * (r - ym) / d;

                  int wWindow = min(cx, d);
                  int hWindow = min(cy, d);

//                  double xrate = (double) wWindow / d;
//                  double yrate = (double) hWindow / d;

                  ::ca::dib * pdib1 = graphics.GetDib(_graphics::DibTemp1);
                  ::ca::dib * pdib2 = graphics.GetDib(_graphics::DibTemp2);
                  //::ca::dib * pdib3 = graphics.GetDib(_graphics::DibTemp3);

                  pdib1->create(wWindow, hWindow);
                  pdib2->create(wWindow, hWindow);
                  //pdib3->create(wWindow, hWindow);

                  //pdib1->get_graphics()->SetStretchBltMode(HALFTONE);
                  pdib1->get_graphics()->StretchBlt(
                     0,
                     0,
                     wWindow,
                     hWindow,
                     pdibT2->get_graphics(),
                     xOff, 
                     yOff,
                     pdibT2->width() - xOff * 2,
                     pdibT2->height() - yOff * 2,
                     SRCCOPY);

                  pdib2->get_graphics()->BitBlt(
                     0, 0, 
                     wWindow, hWindow,
                     &dcBuffer,
                     x1, y1,
                     SRCCOPY);


                  pdib2->channel_from(visual::rgba::channel_alpha,  pdib1);

                  /*drawdib.draw(
                     pdib1->get_graphics(),
                     0, 0,
                     wWindow, hWindow,
                     pdibT2,
                     xOff, yOff,
                     pdibT2->width() - 2 * xOff -1,
                     pdibT2->width() - 2 * yOff -1, 0);*/


                  /*dc2.BitBlt(0, 0, wWindow, hWindow, pdib1->get_graphics(), 0, 0, SRCCOPY);
                  dc2.BitBlt(0, 0, wWindow, hWindow, NULL, 0, 0, DSTINVERT);

                  dc3.FillSolidRect(0, 0, wWindow, hWindow, 0xff000000);
                  dc2.BitBlt(0, 0, wWindow, hWindow, &dc3, 0, 0, SRCAND);*/

                  /*StretchDIBits(
                     dc2,
                     0, 0,
                     wWindow, hWindow,
                     xOff, yOff,
                     pdibT2->width() - 2 * xOff,
                     pdibT2->height() - 2 * yOff,
                     pdibT2->m_pcolorref,
                     &pdibT2->m_Info,
                     DIB_RGB_COLORS,
                     SRCCOPY);*/

//                  ::ca::dib * pdibBuffer = graphics.GetDib(100);
                  /*StretchDIBits(
                     pdib1->get_graphics(),
                     0, 0,
                     wWindow, hWindow,
                     x1, y1,
                     wWindow, hWindow,
                     pdibBuffer->m_pcolorref,
                     &pdibBuffer->m_Info,
                     DIB_RGB_COLORS,
                     SRCPAINT);*/



                  //dcBack.BitBlt(x1, y1, wWindow, hWindow, &dcFrame1, x1, y1, SRCCOPY);
                  //dcBack.BitBlt(x1, y1, wWindow, hWindow, pdib1->get_graphics(),
                    // x1, y1, SRCCOPY);

                  //dcBack.BitBlt(x1, y1, wWindow, hWindow, pdib1->get_graphics(),
                    // x1, y1, SRCCOPY);

                  dcBack.set_alpha_mode(::ca::alpha_mode_set);

                  imaging.bitmap_blend(
                     &dcBack,
                     point(x1, y1),
                     size(wWindow, hWindow),
                     pdib2->get_graphics(),
                     null_point());


                  /*drawdib.draw(
                     dcBack,
                     x1, y1,
                     d, d,
                     pdib1,
                     0, 0,
                     d, d,
                     DDF_HALFTONE);*/
                  //dcBack.BitBlt(xm - r, ym - r, 2 * r, 2 * r, pdib1->get_graphics(), 0, 0, SRCCOPY);
                  rectUpdate.set(x1, y1, x2, y2);
                  recta.add(rectUpdate);
               }
               break;

         }

         single_lock sl(&graphics.m_mutex4Transfer, TRUE);
         ::ca::graphics & dcTransfer = graphics.GetTransferDC();
         ::ca::region_sp rgnTransferClip(get_app());
         rgnTransferClip->CreateRectRgnIndirect(graphics.m_rectFinalPlacement);
         //dcTransfer.SelectClipRgn(rgnTransferClip);

         for(int i = 0; i < recta.get_size(); i++)
         {
            ::rect rectTransfer = recta[i];

            dcTransfer.BitBlt(
               rectTransfer.left, rectTransfer.top,
               rectTransfer.width(), rectTransfer.height(),
               &dcBack,
               rectTransfer.left, rectTransfer.top,
               SRCCOPY);

         }
         dcTransfer.SelectClipRgn(NULL);
      }

      int TransitionEffect::CalcRepeatCount()
      {
         int iStepRepeatCount = m_tool001.m_iStepRepeatCount;
         switch(get_type())
         {
         case TransitionEffectpixelate_BottomTop:
         case TransitionEffectpixelate_TopBottom:
         case TransitionEffectpixelate_LeftRight:
         case TransitionEffectpixelate_RightLeft:
            {
               const int c1 = m_tool001.m_data.m_alphapixelate.m_c1;
//               const int c2 = m_tool001.m_data.m_alphapixelate.m_c2;

               int iIndex = m_tool001.m_iStep - 1;
               int iSizeIndex = m_tool001.m_pointa[iIndex].x;
               int iSize = m_tool001.m_ia[iSizeIndex];
               
               if(iSize < 64)
                  iSize = 64;

               ////iStepRepeatCount = 1c1 / (iSize * 8);
               //iStepRepeatCount = 1;
               int dwTotalTime = 1000;
               int iStepCount = c1 / iSize;
               if(c1 % iSize == 0)
                  iStepCount++;
               int iStartStep = m_tool001.m_pointa2[iIndex].y;
               int iStep = iIndex - iStartStep;
               iStepRepeatCount = iStepCount / 8;
               if(iStep > iStepCount - iStepCount / 4)
                  iStepRepeatCount = iStepCount - iStep - 1;
            }
            break;
         default:
            break;

         }
         iStepRepeatCount = max(1, iStepRepeatCount);
         return iStepRepeatCount;

      }

      bool TransitionEffect::IsActive()
      {
         return m_tool001.m_iStep > 0;
      }

      void TransitionEffect::OnNoPrecisionThousandMillisTimer()
      {
         if(m_pthreadRunStep == NULL)
            m_pthreadRunStep = CreateRunStepThread();
      }


      void TransitionEffect::OnTimer()
      {
         return;

         Main & main = HelperGetMain();

         DWORD dwTime = GetTickCount();
         if(dwTime - m_dwTransitionStepLastRunTime >= 5 + m_dwDelay
            && !m_bTransitionStepRunActive
            && (!m_bTransitionStepPostActive
            || dwTime - m_dwTransitionStepPostActive >= 2000))
         {
         /*            if(dwTime - m_dwTransitionStepLastRunTime >= 200)
         {
               TRACE("dwTime - m_dwTransitionStepLastRunTime >= 200 %d\n",  dwTime - m_dwTransitionStepLastRunTime);
               }
               if(!m_bTransitionStepRunActive)
               {
               TRACE("!m_bTransitionStepRunActive\n");
               }
               if(!m_bTransitionStepPostActive)
               {
               TRACE("!m_bTransitionStepPostActive\n");
               }
               if(dwTime - m_dwTransitionStepPostActive >= 2000)
               {
               TRACE("!m_bTransitionStepPostActive % d\n", dwTime - m_dwTransitionStepPostActive);
         }*/
            m_bTransitionStepPostActive = true;
            m_dwTransitionStepPostActive = GetTickCount();
            main.GetInterface().BackViewPostMessage(Interface::MessageBackViewDequeue, 18, 0);
         }
         else
         {
            ASSERT(TRUE);
         }

      }


      ::radix::thread * TransitionEffect::CreateRunStepThread()
      {
         return AfxBeginThread(get_app(), ThreadProcRunStep, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
      }

      void TransitionEffect::_Init()
      {
         Main & main = HelperGetMain();

         Graphics & graphics = main.GetGraphics();

         //ASSERT(graphics.GetBufferDC().get_os_data() != NULL);

         single_lock sl1Back(&graphics.m_mutex1Back, FALSE);

//         ::ca::graphics & dcBack = graphics.GetBackDC();
         ::ca::graphics & dcFrame1 = graphics.GetFrame1DC();
         ::ca::graphics & dcTransfer = graphics.GetTransferDC();

         Interface & iface = main.GetInterface();

         rect rectClient;
         iface.BackViewGetClientRect(rectClient);

         int cx = rectClient.width();
         int cy = rectClient.height();

         single_lock sl(&graphics.m_mutex4Transfer, TRUE);
         if(dcTransfer.get_os_data() != NULL)
         {
            dcFrame1.BitBlt(0, 0, cx, cy, &dcTransfer, 0, 0, SRCCOPY);
         }
         sl.unlock();
         //graphics.GetDib(_graphics::DibFrame1)->from(dcBack, (HBITMAP) *dcBack.GetCurrentBitmap());

         m_tool001.m_iStep = 1;
         m_tool001.m_iStepCount = -1;
         m_dwDelay = 200;

         m_tool001.Initialize(get_type(), cx, cy, *this);
      }

      UINT AFX_CDECL TransitionEffect::ThreadProcRunStep(LPVOID lpParameter)
      {
         TransitionEffect * peffect = (TransitionEffect *) lpParameter;
         srand((unsigned int) time(NULL));
         event event;
         MMRESULT mmr;
         int iResolution = 25;
         try
         {
            while(peffect->m_bRun)
            {
               while(!peffect->m_bActive && peffect->m_bRun)
               {
                  if(peffect->m_bDestroy)
                     break;
                  peffect->m_eventStartTransition.wait(seconds(0.5));
                  peffect->m_eventStartTransition.ResetEvent();
               }
               if(peffect->m_bDestroy)
                  break;
               if(!peffect->m_bRun)
                  break;

               peffect->_Init();
               if(peffect->m_bDestroy)
                  break;
               if(!peffect->m_bRun)
                  break;

               while(!peffect->TestEnd())
               {
                  if(peffect->m_bDestroy)
                     break;
               if(!peffect->m_bRun)
                  break;

                  mmr = timeSetEvent(
                     max(natural(iResolution), peffect->m_dwDelay),
                     iResolution,  // 5 ms resolution
                     (LPTIMECALLBACK) (HANDLE) event,
                     NULL,
                     TIME_ONESHOT | TIME_CALLBACK_EVENT_SET);

                  peffect->RunStepProcLevel2();

                  if(peffect->TestEnd())
                     break;
               if(!peffect->m_bRun)
                  break;

                  event.wait(seconds(3));
                  event.ResetEvent();
               }
               if(peffect->m_bDestroy)
                  break;
               if(!peffect->m_bRun)
                  break;
               peffect->_Final();
               peffect->StepNext();
            }
         }
         catch(...)
         {
            try
            {
               peffect->m_pthreadRunStep = NULL;
            }
            catch(...)
            {
               ASSERT(FALSE);
               return (UINT) -3;
            }
         }
         peffect->m_eventThreadExit.SetEvent();
         return 0;
      }

      ETransitionEffect TransitionEffect::get_type()
      {
         if(m_iType < 0
            || m_iType >= m_etypea.get_size())
            return TransitionEffectInvalid;
         return m_etypea[m_iType];
      }

      EAlign TransitionEffect::GetAlign(ETransitionEffect eeffect)
      {
         switch(eeffect)
         {
         case TransitionEffectFlyInBottomTop:
            return ::gcom::AlignTop;
         case TransitionEffectFlyInTopBottom:
            return ::gcom::AlignBottom;
         case TransitionEffectFlyInLeftRight:
            return ::gcom::AlignRight;
         case TransitionEffectFlyInRightLeft:
            return ::gcom::AlignLeft;
         case TransitionEffectFlyInLeftTop:
            return ::gcom::AlignTopRight;
         case TransitionEffectFlyInLeftBottom:
            return ::gcom::AlignBottomRight;
         case TransitionEffectFlyInRightBottom:
            return ::gcom::AlignBottomLeft;
         case TransitionEffectFlyInRightTop:
            return ::gcom::AlignTopLeft;
         default:
            return ::gcom::AlignNone;
         }

      }

      EDirection TransitionEffect::GetDirection(ETransitionEffect eeffect)
      {
         switch(eeffect)
         {
         case TransitionEffectFlyInBottomTop:
            return ::gcom::DirectionTop;
         case TransitionEffectFlyInTopBottom:
            return ::gcom::DirectionBottom;
         case TransitionEffectFlyInLeftRight:
            return ::gcom::DirectionRight;
         case TransitionEffectFlyInRightLeft:
            return ::gcom::DirectionLeft;
         case TransitionEffectFlyInLeftTop:
            return ::gcom::DirectionTopRight;
         case TransitionEffectFlyInLeftBottom:
            return ::gcom::DirectionBottomRight;
         case TransitionEffectFlyInRightBottom:
            return ::gcom::DirectionBottomLeft;
         case TransitionEffectFlyInRightTop:
            return ::gcom::DirectionTopLeft;
         case TransitionEffectWipeTop:
            return ::gcom::DirectionTop;
         case TransitionEffectWipeBottom:
            return ::gcom::DirectionBottom;
         case TransitionEffectWipeRight:
            return ::gcom::DirectionRight;
         case TransitionEffectWipeLeft:
            return ::gcom::DirectionLeft;
         case TransitionEffectWipeTopLeft:
            return ::gcom::DirectionTopLeft;
         case TransitionEffectWipeTopRight:
            return ::gcom::DirectionTopRight;
         case TransitionEffectWipeBottomLeft:
            return ::gcom::DirectionBottomLeft;
         case TransitionEffectWipeBottomRight:
            return ::gcom::DirectionBottomRight;
         case TransitionEffectScaleTop:
            return ::gcom::DirectionTop;
         case TransitionEffectScaleBottom:
            return ::gcom::DirectionBottom;
         case TransitionEffectScaleRight:
            return ::gcom::DirectionRight;
         case TransitionEffectScaleLeft:
            return ::gcom::DirectionLeft;
         case TransitionEffectScaleTopLeft:
            return ::gcom::DirectionTopLeft;
         case TransitionEffectScaleTopRight:
            return ::gcom::DirectionTopRight;
         case TransitionEffectScaleBottomLeft:
            return ::gcom::DirectionBottomLeft;
         case TransitionEffectScaleBottomRight:
            return ::gcom::DirectionBottomRight;
         default:
            return ::gcom::DirectionNone;
         }

      }

   } // namespace backview

} // namespace gcom





