#include "framework.h"
#include <math.h>


#define d2d1_fax_options D2D1_FACTORY_OPTIONS // fax of merde
#define single_threaded D2D1_FACTORY_TYPE_SINGLE_THREADED // ???? muliple performance multi thread hidden option there exists cost uses?

struct PathTextDrawingContext
{
   ID2D1GeometrySink* psink;
};

class PathTextRenderer: public IDWriteTextRenderer
{
public:
   static void CreatePathTextRenderer(
      FLOAT pixelsPerDip,
      _Outptr_ PathTextRenderer **textRenderer
      );

   PathTextRenderer(
      FLOAT pixelsPerDip
      );

   STDMETHOD(DrawGlyphRun)(
      _In_opt_ void* clientDrawingContext,
      FLOAT baselineOriginX,
      FLOAT baselineOriginY,
      DWRITE_MEASURING_MODE measuringMode,
      _In_ DWRITE_GLYPH_RUN const* glyphRun,
      _In_ DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
      _In_opt_ IUnknown* clientDrawingEffect
      ) override;

   STDMETHOD(DrawUnderline)(
      _In_opt_ void* clientDrawingContext,
      FLOAT baselineOriginX,
      FLOAT baselineOriginY,
      _In_ DWRITE_UNDERLINE const* underline,
      _In_opt_ IUnknown* clientDrawingEffect
      ) override;

   STDMETHOD(DrawStrikethrough)(
      _In_opt_ void* clientDrawingContext,
      FLOAT baselineOriginX,
      FLOAT baselineOriginY,
      _In_ DWRITE_STRIKETHROUGH const* strikethrough,
      _In_opt_ IUnknown* clientDrawingEffect
      ) override;

   STDMETHOD(DrawInlineObject)(
      _In_opt_ void* clientDrawingContext,
      FLOAT originX,
      FLOAT originY,
      IDWriteInlineObject* inlineObject,
      BOOL isSideways,
      BOOL isRightToLeft,
      _In_opt_ IUnknown* clientDrawingEffect
      ) override;

   STDMETHOD(IsPixelSnappingDisabled)(
      _In_opt_ void* clientDrawingContext,
      _Out_ BOOL* isDisabled
      ) override;

   STDMETHOD(GetCurrentTransform)(
      _In_opt_ void* clientDrawingContext,
      _Out_ DWRITE_MATRIX* transform
      ) override;

   STDMETHOD(GetPixelsPerDip)(
      _In_opt_ void* clientDrawingContext,
      _Out_ FLOAT* pixelsPerDip
      ) override;

   STDMETHOD(QueryInterface)(
      REFIID riid,
      _Outptr_ void** object
      ) override;

   STDMETHOD_(ULONG,AddRef)() override;

   STDMETHOD_(ULONG,Release)() override;

private:
   FLOAT m_pixelsPerDip;   // Number of pixels per DIP.
   UINT m_ref;             // Reference count for AddRef and Release.
};

namespace draw2d_direct2d
{
   static double g_dPi = atan(1.0) * 4.0;


   graphics_path::graphics_path(::aura::application * papp) :
      ::element(papp)
   {

      m_bFigureEnded = true;

   }

   graphics_path::~graphics_path() 
   {


   }


   bool graphics_path::internal_begin_figure(bool bFill, ::draw2d::e_fill_mode efillmode)
   {

      if(m_psink != NULL)
      {
         
         internal_end_figure(false);


      }


      return true;


   }


   bool graphics_path::internal_end_figure(bool bClose)
   {

      if(m_psink == NULL || m_bFigureEnded)
         return true;

      if(bClose)
      {

         m_psink->EndFigure(D2D1_FIGURE_END_CLOSED);

      }
      else
      {

         m_psink->EndFigure(D2D1_FIGURE_END_OPEN);

      }

      m_bFigureEnded = true;

      return true;

   }

   bool graphics_path::internal_add_arc(const RECT & rect, double dStart, double dEnd)
   {

      D2D1_POINT_2F pt;

      D2D1_ARC_SEGMENT arcseg;

      internal_get_arc(pt, arcseg, rect, dStart, dEnd);

      if(!internal_prepare(pt))
         return false;

      m_psink->AddArc(arcseg);

      return true;

   }
   bool graphics_path::internal_add_rect(int x,int y, int cx, int cy)
   {



      internal_add_line(x,y);
      internal_add_line(x + cx,y);
      internal_add_line(x + cx,y + cy);
      internal_add_line(x,y + cy);
      internal_add_line(x,y);


      return true;

   }

   bool graphics_path::internal_add_string(::draw2d_direct2d::graphics * pdc, int32_t x,int32_t y,const string & strText,::draw2d::font_sp spfont)
   {

      HRESULT hr;

      if(m_psink == NULL)
      {
         hr = m_ppath->Open(
            &m_psink
            );
      }


      wstring szOutline(strText);

      IDWriteFactory * pfactory = TlsGetWriteFactory();

      Microsoft::WRL::ComPtr<IDWriteTextLayout>                       textLayout;


      pfactory->CreateTextLayout(szOutline,szOutline.length(),(IDWriteTextFormat *)spfont.cast <font>()->get_os_font(pdc),1024 * 1024,1024 * 1024,&textLayout);


      Microsoft::WRL::ComPtr<PathTextRenderer>                       textRenderer;

      PathTextRenderer::CreatePathTextRenderer(System.m_dpi, &textRenderer);

      PathTextDrawingContext context;
      context.psink = m_psink.Get();

      textLayout->Draw(&context, textRenderer.Get(), 0, 0);


      return true;

   }

   bool graphics_path::internal_add_line(int x1, int y1, int x2, int y2)
   {

      bool bOk1 = add_line(x1, y1);

      bool bOk2 = add_line(x2, y2);

      return bOk1 && bOk2;

   }


   bool graphics_path::internal_add_line(int x, int y)
   {

      D2D1_POINT_2F pt;

      pt.x = (FLOAT) x;
      pt.y = (FLOAT) y;

      return internal_prepare(pt);
   }


   bool graphics_path::internal_add_move(int x, int y)
   {

      internal_end_figure(false);

      D2D1_POINT_2F pt;

      pt.x = (FLOAT) x;
      pt.y = (FLOAT) y;

      return internal_prepare(pt);

   }

   bool graphics_path::internal_prepare(D2D1_POINT_2F pt)
   {

      if(m_psink == NULL)
      {

         m_ppath->Open(&m_psink);

      }

      if(m_bFigureEnded)
      {

         m_psink->BeginFigure(pt, m_bFill ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW);

         if(m_efillmode == ::draw2d::fill_mode_winding)
         {

            m_psink->SetFillMode(D2D1_FILL_MODE_WINDING);

         }
         else if(m_efillmode == ::draw2d::fill_mode_alternate)
         {

            m_psink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);

         }

         m_bFigureEnded = false;

      }
      else
      {

         m_psink->AddLine(pt);

      }

      return true;

   }

   bool graphics_path::internal_get_arc(D2D1_POINT_2F & pt, D2D1_ARC_SEGMENT & arcseg, const RECT & rect, double dStart, double dEnd)
   {

      double pi = g_dPi;

      D2D1_POINT_2F ptCenter;

      ptCenter.x = ((double) rect.left + (double) rect.right) / 2.0f;
      ptCenter.y = ((double) rect.top + (double) rect.bottom) / 2.0f;

      double rx = (double) rect.right    - ptCenter.x;
      double ry = (double) rect.bottom   - ptCenter.y;

      pt.x = ptCenter.x + cos(dStart * pi / 180.0) * rx;
      pt.y = ptCenter.y - sin(dStart * pi / 180.0) * ry;

      arcseg.point.x = ptCenter.x + cos((dEnd) * pi / 180.0) * rx;
      arcseg.point.y = ptCenter.y - sin((dEnd) * pi / 180.0) * ry;

      if(dEnd - dStart > 0)
      {
         arcseg.sweepDirection = D2D1_SWEEP_DIRECTION_CLOCKWISE;
      }
      else
      {
         arcseg.sweepDirection = D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;
      }

      if(abs(dEnd - dStart) < 180.0)
      {
         arcseg.arcSize = D2D1_ARC_SIZE_SMALL;
      }
      else
      {
         arcseg.arcSize = D2D1_ARC_SIZE_LARGE;
      }

      arcseg.rotationAngle = abs(dEnd-dStart);

      arcseg.size.width    = rx;

      arcseg.size.height   = ry;

      return true;

   }


   void * graphics_path::get_os_data() const
   {
      defer_update();

      return (ID2D1PathGeometry *) m_ppath.Get();

   }

   ID2D1PathGeometry * graphics_path::get_os_path(::draw2d_direct2d::graphics * pdc)
   {


      m_ppath = nullptr;

      HRESULT hr = ::GetD2D1Factory1()->CreatePathGeometry(&m_ppath);

      ::draw2d_direct2d::throw_if_failed(hr);

      m_psink  = nullptr;

      for(int32_t i = 0; i < m_elementa.get_count(); i++)
      {

         set(pdc,m_elementa(i));

      }

      if(m_psink != NULL)
      {
         m_psink->Close();
      }
      else
      {
         m_ppath = nullptr;
      }

      m_psink = nullptr;


      return (ID2D1PathGeometry *)m_ppath.Get();

   }

   void * graphics_path::detach()
   {

      ID2D1PathGeometry * pgeometry = (ID2D1PathGeometry *) get_os_data();

      return m_ppath.Detach();

   }

   bool graphics_path::destroy()
   {

      if(m_psink != NULL)
      {

         m_psink = nullptr;


      }

      if(m_ppath != NULL)
      {

         m_ppath = nullptr;

      }

      return true;
   
   }

   bool graphics_path::create()
   {


      return true;
      

   }

   bool graphics_path::set(::draw2d_direct2d::graphics * pdc, const ::draw2d::path::element & e)
   {

      switch(e.m_etype)
      {
      case ::draw2d::path::element::type_move:
         set(e.u.m_move);
         break;
      case ::draw2d::path::element::type_arc:
         set(e.u.m_arc);
         break;
      case ::draw2d::path::element::type_line:
         set(e.u.m_line);
         break;
      case ::draw2d::path::element::type_rect:
         set(e.u.m_rect);
         break;
      case ::draw2d::path::element::type_string:
         set(pdc,e.m_stringpath);
         break;
      case ::draw2d::path::element::type_end:
         internal_end_figure(e.u.m_end.m_bClose);
         break;
      default:
         throw "unexpected simple os graphics element type";
      }

      return false;

   }


   bool graphics_path::set(const ::draw2d::path::arc & arc)
   {
      
      ::rect rect;

      rect.left      = (LONG) (arc.m_xCenter - arc.m_dRadiusX);
      rect.right     = (LONG) (arc.m_xCenter + arc.m_dRadiusX);
      rect.top       = (LONG) (arc.m_yCenter - arc.m_dRadiusY);
      rect.bottom    = (LONG) (arc.m_yCenter + arc.m_dRadiusY);

      return internal_add_arc(rect,arc.m_dAngle1 * 180.0 / g_dPi,arc.m_dAngle2 * 180.0 / g_dPi);

   }


   bool graphics_path::set(const ::draw2d::path::move & move)
   {

      return internal_add_move((int) move.m_x, (int) move.m_y);

   }


   bool graphics_path::set(const ::draw2d::path::line & line)
   {

      return internal_add_line((int) line.m_x, (int) line.m_y);

   }

   bool graphics_path::set(const ::draw2d::path::rect & r)
   {

      return internal_add_rect((int)r.m_x,(int)r.m_y,(int)r.m_cx,(int)r.m_cy);

   }

   bool graphics_path::set(::draw2d_direct2d::graphics * pdc,const ::draw2d::path::string_path & path)
   {
      return true; // done at pdc
      //return internal_add_string(pdc, (int) path.m_x,(int)path.m_y, path.m_strText, path.m_spfont);

   }


} // namespace draw2d_direct2d






//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

using namespace Microsoft::WRL;

// An identity matrix for use by IDWritePixelSnapping::GetCurrentTransform.
const DWRITE_MATRIX identityTransform =
{
   1,0,
   0,1,
   0,0
};

//
// Public creation method for PathTextRenderer. This method
// creates a new PathTextRenderer and calls its AddRef method
// in good COM style.
//
void PathTextRenderer::CreatePathTextRenderer(
   FLOAT pixelsPerDip,
   _Outptr_ PathTextRenderer **textRenderer
   )
{
   *textRenderer = nullptr;

   PathTextRenderer *newRenderer = new PathTextRenderer(pixelsPerDip);
   newRenderer->AddRef();
   *textRenderer = newRenderer;
   newRenderer = nullptr;
}

PathTextRenderer::PathTextRenderer(FLOAT pixelsPerDip):
m_pixelsPerDip(pixelsPerDip),
m_ref(0)
{
}

//
// Draws a given glyph run along the geometry specified
// in the given clientDrawingEffect.
//
// This method calculates the horizontal displacement
// of each glyph cluster in the run, then calculates the
// tangent vector of the geometry at each of those distances.
// It then renders the glyph cluster using the offset and angle
// defined by that tangent, thereby placing each cluster on
// the path and also rotated to the path.
//
HRESULT PathTextRenderer::DrawGlyphRun(
   _In_opt_ void* clientDrawingContext,
   FLOAT baselineOriginX,
   FLOAT baselineOriginY,
   DWRITE_MEASURING_MODE measuringMode,
   _In_ DWRITE_GLYPH_RUN const* glyphRun,
   _In_ DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
   _In_opt_ IUnknown* clientDrawingEffect
   )
{
   if(clientDrawingContext == nullptr)
   {
      return S_OK;
   }

   PathTextDrawingContext* dc = static_cast<PathTextDrawingContext*>(clientDrawingContext);

   HRESULT hr = glyphRun->fontFace->GetGlyphRunOutline(
      glyphRun->fontEmSize,
      glyphRun->glyphIndices,
      glyphRun->glyphAdvances,
      glyphRun->glyphOffsets,
      glyphRun->glyphCount,
      glyphRun->isSideways,
      glyphRun->bidiLevel % 2,
      dc->psink
      );

   return hr;
}

HRESULT PathTextRenderer::DrawUnderline(
   _In_opt_ void* clientDrawingContext,
   FLOAT baselineOriginX,
   FLOAT baselineOriginY,
   _In_ DWRITE_UNDERLINE const* underline,
   _In_opt_ IUnknown* clientDrawingEffect
   )
{
   // We don't use underline in this application.
   return E_NOTIMPL;
}

HRESULT PathTextRenderer::DrawStrikethrough(
   _In_opt_ void* clientDrawingContext,
   FLOAT baselineOriginX,
   FLOAT baselineOriginY,
   _In_ DWRITE_STRIKETHROUGH const* strikethrough,
   _In_opt_ IUnknown* clientDrawingEffect
   )
{
   // We don't use strikethrough in this application.
   return E_NOTIMPL;
}

HRESULT PathTextRenderer::DrawInlineObject(
   _In_opt_ void* clientDrawingContext,
   FLOAT originX,
   FLOAT originY,
   IDWriteInlineObject* inlineObject,
   BOOL isSideways,
   BOOL isRightToLeft,
   _In_opt_ IUnknown* clientDrawingEffect
   )
{
   // We don't use inline objects in this application.
   return E_NOTIMPL;
}

//
// IDWritePixelSnapping methods
//
HRESULT PathTextRenderer::IsPixelSnappingDisabled(
   _In_opt_ void* clientDrawingContext,
   _Out_ BOOL* isDisabled
   )
{
   *isDisabled = FALSE;
   return S_OK;
}

HRESULT PathTextRenderer::GetCurrentTransform(
   _In_opt_ void* clientDrawingContext,
   _Out_ DWRITE_MATRIX* transform
   )
{
   *transform = identityTransform;
   return S_OK;
}

HRESULT PathTextRenderer::GetPixelsPerDip(
   _In_opt_ void* clientDrawingContext,
   _Out_ FLOAT* pixelsPerDip
   )
{
   *pixelsPerDip = m_pixelsPerDip;
   return S_OK;
}

//
// IUnknown methods
//
// These use a basic, non-thread-safe implementation of the
// standard reference-counting logic.
//
HRESULT PathTextRenderer::QueryInterface(
   REFIID riid,
   _Outptr_ void** object
   )
{
   *object = nullptr;
   return E_NOTIMPL;
}

ULONG PathTextRenderer::AddRef()
{
   m_ref++;

   return m_ref;
}

ULONG PathTextRenderer::Release()
{
   m_ref--;

   if(m_ref == 0)
   {
      delete this;
      return 0;
   }

   return m_ref;
}

