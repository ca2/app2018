#include "framework.h"


namespace visual
{


   api::api(::ca::application * papp) :
      ca(papp)
   {

   }


   api::~api()
   {

   }


   void api::DrawBeziers(
      ::ca::graphics                     *pdc,
      LPPOINT             lppoints,
      int32_t                  iCount,
      double               dRateX,
      LPPOINT               lppointOffset)
   {

      rect clipRect;

      size viewportExt = pdc->GetViewportExt();
      point viewportOrg = pdc->GetViewportOrg();

      pdc->OffsetViewportOrg(
         lppointOffset->x,
         lppointOffset->y);

      pdc->ScaleViewportExt((int32_t) dRateX, 1, 1, 1);

      pdc->BeginPath();
      pdc->PolyBezier(lppoints, iCount);
      pdc->EndPath();
      pdc->StrokePath();

      pdc->SetWindowExt(viewportExt);
      pdc->SetViewportOrg(viewportOrg);

   }

   void api::DrawAndFillBeziers(
      ::ca::graphics                     *pdc,
      LPPOINT             lppoints,
      int32_t                  iCount,
      double               dRateX,
      LPPOINT               lppointOffset)
   {

      rect clipRect;

      size viewportExt = pdc->GetViewportExt();
      point viewportOrg = pdc->GetViewportOrg();

      pdc->OffsetViewportOrg(
         lppointOffset->x,
         lppointOffset->y);

      pdc->ScaleViewportExt((int32_t) dRateX, 1, 1, 1);

      pdc->BeginPath();
      pdc->PolyBezier(lppoints, iCount);
      pdc->EndPath();
      pdc->StrokeAndFillPath();

      pdc->SetWindowExt(viewportExt);
      pdc->SetViewportOrg(viewportOrg);

   }

   void api::DrawAndFillBeziers(
      ::ca::graphics                     *pdc,
      base_array<point_array, point_array &> *
                            lpglyph,
      double               dRateX,
      LPPOINT               lppointOffset)
   {

      rect clipRect;

      size viewportExt = pdc->GetViewportExt();
      point viewportOrg = pdc->GetViewportOrg();

      pdc->OffsetViewportOrg(
         lppointOffset->x,
         lppointOffset->y);

      pdc->ScaleViewportExt((int32_t)(dRateX * 1000.0), 1, 1, 1);

      for(int32_t i = 0; i < lpglyph->get_size(); i++)
      {
         pdc->PolyBezier(lpglyph->element_at(i).get_data(), (int32_t) lpglyph->element_at(i).get_size());
      }

      pdc->SetWindowExt(viewportExt);
      pdc->SetViewportOrg(viewportOrg);

   }

   void api::EmbossedTextOut(
      ::ca::graphics *             pdc,
      LPCRECT            lpcrect,
      double            dRateX,
      double            dHeight,
      const char *      psz,
      LPINT             lpiCharsPositions,
      int32_t               iCharsPositions,
      int32_t               iOffset)
   {
         rect clipRect;

   //      int32_t iOldMapMode = ::GetMapMode(pdc->m_hDC);
   //      point viewportOrg;
   //      ::ca::font * pfont = pdc->GetCurrentFont();
   //      ASSERT(pfont != NULL);
   //      VERIFY(::GetViewportOrgEx(pdc->m_hDC, &viewportOrg));

   //      VERIFY(::OffsetViewportOrgEx(
   //         pdc->m_hDC,
   //         lpcrect->left,
   //         lpcrect->top,
   //         NULL));
   //      VERIFY(::SetMapMode(pdc->m_hDC, MM_ANISOTROPIC));
   //      if(floatRateX == 0.0)
   //         floatRateX = 1.0;
   //      VERIFY(::ScaleViewportExtEx(pdc->m_hDC, floatRateX * 30000.0, 10000, 1, 1, NULL));

   //      VERIFY(::SelectObject(pdc->m_hDC, pfont->m_hObject));

         pdc->TextOut(lpcrect->left, lpcrect->top, psz);
         pdc->BeginPath();
         pdc->TextOut(lpcrect->left, lpcrect->top, psz);
         pdc->EndPath();
         pdc->StrokePath();


   //      VERIFY(::SetMapMode(pdc->m_hDC, iOldMapMode));
   //      VERIFY(::SetViewportOrgEx(
   //         pdc->m_hDC,
   //         viewportOrg.x,
   //         viewportOrg.y,
   //         NULL));
   }


   void api::EmbossedTextOut(
      ::ca::graphics *          pdc,
      const char *   psz,
      int32_t            iLeft,
      int32_t            iTop,
      int32_t            iWidth,
      COLORREF       crText,
      COLORREF       crOutline,
      int32_t            iLen)
   {
         rect clipRect;

   //      int32_t iOldMapMode = ::GetMapMode(pdc->m_hDC);
   //      point viewportOrg;
   //      ::ca::font * pfont = pdc->GetCurrentFont();
   //      ASSERT(pfont != NULL);
   //      VERIFY(::GetViewportOrgEx(pdc->m_hDC, &viewportOrg));

   //      VERIFY(::OffsetViewportOrgEx(
   //         pdc->m_hDC,
   //         lpcrect->left,
   //         lpcrect->top,
   //         NULL));
   //      VERIFY(::SetMapMode(pdc->m_hDC, MM_ANISOTROPIC));
   //      if(floatRateX == 0.0)
   //         floatRateX = 1.0;
   //      VERIFY(::ScaleViewportExtEx(pdc->m_hDC, floatRateX * 30000.0, 10000, 1, 1, NULL));

   //      VERIFY(::SelectObject(pdc->m_hDC, pfont->m_hObject));

         string str;
         str = gen::international::utf8_to_unicode(psz);

#ifdef WINDOWSEX
         ::SetTextColor((HDC)pdc->get_os_data(), crOutline);
         ::TextOutU((HDC)pdc->get_os_data(), iLeft - iWidth, iTop, str, iLen);
         ::TextOutU((HDC)pdc->get_os_data(), iLeft, iTop - iWidth, str, iLen);
         ::TextOutU((HDC)pdc->get_os_data(), iLeft + iWidth, iTop, str, iLen);
         ::TextOutU((HDC)pdc->get_os_data(), iLeft, iTop + iWidth, str, iLen);

         ::SetTextColor((HDC)pdc->get_os_data(), crText);
         ::TextOutU((HDC)pdc->get_os_data(), iLeft, iTop, str, iLen);
#endif

         //pdc->TextOut(lpcrect->left, lpcrect->top, str);
   //      pdc->BeginPath();
   //      ::TextOutW((HDC)pdc->get_os_data(), lpcrect->left, lpcrect->top, lpcsz, iLen);
         //pdc->TextOut(lpcrect->left, lpcrect->top, str);
   //      pdc->EndPath();
   //      pdc->StrokePath();


   //      VERIFY(::SetMapMode(pdc->m_hDC, iOldMapMode));
   //      VERIFY(::SetViewportOrgEx(
   //         pdc->m_hDC,
   //         viewportOrg.x,
   //         viewportOrg.y,
   //         NULL));
   }

   void api::SimpleTextOut(
      ::ca::graphics *          pdc,
      LPCRECT        lpcrect,
      double         dRateX,
      double         dHeight,
      const char *   psz,
      LPINT          lpiCharsPositions,
      int32_t            iCharsPositions,
      int32_t            iOffset)
   {
      UNREFERENCED_PARAMETER(dRateX);
      UNREFERENCED_PARAMETER(dHeight);
      UNREFERENCED_PARAMETER(iOffset);
      UNREFERENCED_PARAMETER(iCharsPositions);
      UNREFERENCED_PARAMETER(lpiCharsPositions);

      pdc->TextOut(lpcrect->left, lpcrect->top, psz);
      return;

      string str;
      str = gen::international::utf8_to_unicode(psz);
      ::TextOutU((HDC)pdc->get_os_data(), lpcrect->left, lpcrect->top, str, (int32_t) str.get_length());
   }


   void api::EmbossedTextOut(
      ::ca::graphics *          pdc,
      LPCRECT        lpcrect,
      double         dHeight,
      double         dRateX,
      const char *   psz)
   {
         pdc->TextOut(lpcrect->left, lpcrect->top, psz);
         pdc->BeginPath();
         pdc->TextOut(lpcrect->left, lpcrect->top, psz);
         pdc->EndPath();
         pdc->StrokePath();

   }

   bool api::open()
   {
      return true;
   }

   bool api::close()
   {
      return true;
   }

} // namespace visual




