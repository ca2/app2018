#include "framework.h"


namespace draw2d_cairo
{


   region::region(sp(base_application) papp) :
      element(papp)
   {

      m_pdc = NULL;

      m_psurface = NULL;

   }


   region::~region()
   {

   }



/*   region::operator HRGN() const
   {
      return (HRGN)(this == NULL ? NULL : get_os_data());
   }
   region* PASCAL region::from_handle(sp(base_application) papp, HRGN hRgn)
   {
      return dynamic_cast < region * > (::win::object::from_handle(papp, hRgn));
   }
   int_bool region::CreateRectRgn(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   { return Attach(::CreateRectRgn(x1, y1, x2, y2)); }
   int_bool region::CreateRectRgnIndirect(LPCRECT lpRect)
   { return Attach(::CreateRectRgnIndirect(lpRect)); }
   int_bool region::CreateEllipticRgn(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   { return Attach(::CreateEllipticRgn(x1, y1, x2, y2)); }
   int_bool region::CreateEllipticRgnIndirect(LPCRECT lpRect)
   { return Attach(::CreateEllipticRgnIndirect(lpRect)); }
   int_bool region::CreatePolygonRgn(LPPOINT lpPoints, int32_t nCount, int32_t nMode)
   { return Attach(::CreatePolygonRgn(lpPoints, nCount, nMode)); }
   int_bool region::CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts, int32_t nCount, int32_t nPolyFillMode)
   { return Attach(::CreatePolyPolygonRgn(lpPoints, lpPolyCounts, nCount, nPolyFillMode)); }
   int_bool region::CreateRoundRectRgn(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3)
   { return Attach(::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3)); }
   int_bool region::CreateFromPath(::draw2d::graphics * pgraphics)
   { ASSERT(pgraphics != NULL); return Attach(::PathToRegion((dynamic_cast<::win::graphics * >(pgraphics))->get_handle1())); }
   int_bool region::CreateFromData(const XFORM* lpXForm, int32_t nCount, const RGNDATA* pRgnData)
   { return Attach(::ExtCreateRegion(lpXForm, nCount, pRgnData)); }
   int32_t region::GetRegionData(LPRGNDATA lpRgnData, int32_t nDataSize) const
   { ASSERT(get_os_data() != NULL); return (int32_t)::GetRegionData((HRGN)get_os_data(), nDataSize, lpRgnData); }
   void region::SetRectRgn(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   { ASSERT(get_os_data() != NULL); ::SetRectRgn((HRGN)get_os_data(), x1, y1, x2, y2); }

   void region::SetRectRgn(LPCRECT lpRect)
   {
      ::SetRectRgn((HRGN)get_os_data(), lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
   }

   int32_t region::CombineRgn(const ::draw2d::region* pRgn1, const ::draw2d::region* pRgn2, int32_t nCombineMode)
   { ASSERT(get_os_data() != NULL); return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgn1->get_os_data(),
   (HRGN)pRgn2->get_os_data(), nCombineMode); }
   int32_t region::CopyRgn(const ::draw2d::region* pRgnSrc)
   { ASSERT(get_os_data() != NULL); return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgnSrc->get_os_data(), NULL, RGN_COPY); }
   int_bool region::EqualRgn(const ::draw2d::region* pRgn) const
   { ASSERT(get_os_data() != NULL); return ::EqualRgn((HRGN)get_os_data(), (HRGN)pRgn->get_os_data()); }
   int32_t region::OffsetRgn(int32_t x, int32_t y)
   { ASSERT(get_os_data() != NULL); return ::OffsetRgn((HRGN)get_os_data(), x, y); }
   int32_t region::OffsetRgn(POINT point)
   { ASSERT(get_os_data() != NULL); return ::OffsetRgn((HRGN)get_os_data(), point.x, point.y); }
   int32_t region::GetRgnBox(LPRECT lpRect) const
   { ASSERT(get_os_data() != NULL); return ::GetRgnBox((HRGN)get_os_data(), lpRect); }
   int_bool region::PtInRegion(int32_t x, int32_t y) const
   { ASSERT(get_os_data() != NULL); return ::PtInRegion((HRGN)get_os_data(), x, y); }
   int_bool region::PtInRegion(POINT point) const
   { ASSERT(get_os_data() != NULL); return ::PtInRegion((HRGN)get_os_data(), point.x, point.y); }
   int_bool region::RectInRegion(LPCRECT lpRect) const
   { ASSERT(get_os_data() != NULL); return ::RectInRegion((HRGN)get_os_data(), lpRect); }*/

   bool region::is_simple_positive_region()
   {

      switch(m_etype)
      {
      case type_none:
         return true;
      case type_rect:
         return true;
      case type_oval:
         return true;
      case type_polygon:
         return true;
      case type_poly_polygon:
         return false;
      case type_combine:
         return false;
      default:
         throw not_implemented(get_app());
      }

      return true;

   }


   bool region::mask(cairo_t * pdc)
   {
synch_lock ml(&user_mutex());
      if(m_pdc != NULL)
      {

         cairo_destroy(m_pdc);

         m_pdc = NULL;

      }

      if(m_psurface != NULL)
      {

         cairo_surface_destroy(m_psurface);

         m_psurface = NULL;

      }


      m_rectBoundingBoxInternal = rect(0, 0, 0, 0);

      max_bounding_box(m_rectBoundingBoxInternal);

      m_psurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_rectBoundingBoxInternal.width(), m_rectBoundingBoxInternal.height());

      if(m_psurface == NULL)
      {

         return false;

      }

      m_pdc = cairo_create(m_psurface);

      int x = m_rectBoundingBoxInternal.left;

      int y = m_rectBoundingBoxInternal.top;

      cairo_translate(m_pdc, -x, -y);

      cairo_set_source_rgba(m_pdc, 0.0, 0.0, 0.0, 0.0);


      cairo_rectangle(m_pdc, 0, 0, m_rectBoundingBoxInternal.width(), m_rectBoundingBoxInternal.height());

      cairo_set_operator(m_pdc, CAIRO_OPERATOR_SOURCE);

      cairo_fill(m_pdc);

      cairo_set_source_rgba(m_pdc, 0.0, 0.0, 0.0, 1.0);

      switch(m_etype)
      {
      case type_none:
         break;
      case type_rect:
         mask_rect(m_pdc);
         break;
      case type_oval:
         mask_oval(m_pdc);
         break;
      case type_polygon:
         mask_polygon(m_pdc);
         break;
      case type_poly_polygon:
         mask_polygon(m_pdc);
         break;
      case type_combine:
         mask_combine(m_pdc);
         break;
      default:
         throw not_implemented(get_app());
      }

      cairo_mask_surface(pdc, m_psurface, m_rectBoundingBoxInternal.left, m_rectBoundingBoxInternal.top);

      return true;

   }

   bool region::mask_rect(cairo_t * pdc)
   {
synch_lock ml(&user_mutex());
      cairo_rectangle(pdc, m_x1, m_y1, m_x2, m_y2);

      cairo_fill(pdc);

      return true;

   }

   bool region::mask_oval(cairo_t * pdc)
   {
synch_lock ml(&user_mutex());
      double centerx    = (m_x2 + m_x1) / 2.0;
      double centery    = (m_y2 + m_y1) / 2.0;

      double radiusx    = abs(m_x2 - m_x1) / 2.0;
      double radiusy    = abs(m_y2 - m_y1) / 2.0;

      if(radiusx == 0.0 || radiusy == 0.0)
         return false;

      cairo_keep k(pdc);

      cairo_translate(pdc, centerx, centery);

      cairo_scale(pdc, radiusx, radiusy);

      cairo_arc(pdc, 0.0, 0.0, 1.0, 0.0, 2.0 * 3.1415);

      cairo_fill(pdc);

      return true;

   }

   bool region::mask_polygon(cairo_t * pdc)
   {
synch_lock ml(&user_mutex());
      if(m_nCount <= 0)
         return true;


      cairo_move_to(pdc, m_lppoints[0].x, m_lppoints[0].y);

      for(int32_t i = 1; i < m_nCount; i++)
      {

         cairo_line_to(pdc, m_lppoints[i].x, m_lppoints[i].y);

      }
      cairo_fill(pdc);

      return true;

   }

   bool region::mask_poly_polygon(cairo_t * pdc)
   {
synch_lock ml(&user_mutex());
      int32_t n = 0;

      for(int32_t i = 0; i < m_nCount; i++)
      {
         int32_t jCount = m_lppolycounts[i];
         if(jCount > 0)
         {
            cairo_move_to(pdc, m_lppoints[n].x, m_lppoints[n].y);
            n++;
            for(int32_t j = 1; i < jCount; j++)
            {
               cairo_line_to(pdc, m_lppoints[n].x, m_lppoints[n].y);
               n++;
            }
         }

      }
      cairo_fill(pdc);

      return true;

   }

   bool region::mask_combine(cairo_t * pdc)
   {
synch_lock ml(&user_mutex());
      cairo_push_group(pdc);

      dynamic_cast < ::draw2d_cairo::region * >(m_pregion1)->mask(pdc);

      cairo_pop_group_to_source(pdc);

      cairo_paint(pdc);

      cairo_push_group(pdc);

      dynamic_cast < ::draw2d_cairo::region * >(m_pregion2)->mask(pdc);

      cairo_pop_group_to_source(pdc);

      if(m_ecombine == ::draw2d::region::combine_add)
      {
         cairo_set_operator(pdc, CAIRO_OPERATOR_SOURCE);
      }
      else if(m_ecombine == ::draw2d::region::combine_exclude)
      {
         cairo_set_operator(pdc, CAIRO_OPERATOR_CLEAR);
      }
      else if(m_ecombine == ::draw2d::region::combine_intersect)
      {
         cairo_set_operator(pdc, CAIRO_OPERATOR_IN);
      }
      else
      {
         cairo_set_operator(pdc, CAIRO_OPERATOR_SOURCE);
      }

      cairo_paint(m_pdc);

      return true;

   }

   bool region::clip(cairo_t * pdc)
   {
synch_lock ml(&user_mutex());
      if(m_pdc != NULL)
      {

         cairo_destroy(m_pdc);

         m_pdc = NULL;

      }

      if(m_psurface != NULL)
      {

         cairo_surface_destroy(m_psurface);

         m_psurface = NULL;

      }

      switch(m_etype)
      {
      case type_none:
         break;
      case type_rect:
         clip_rect(pdc);
         break;
      case type_oval:
         clip_oval(pdc);
         break;
      case type_polygon:
         clip_polygon(pdc);
         break;
      default:
         throw not_implemented(get_app());
      }

      return true;

   }

   bool region::clip_rect(cairo_t * pdc)
   {
synch_lock ml(&user_mutex());
      cairo_rectangle(pdc, m_x1, m_y1, m_x2, m_y2);

      cairo_clip(pdc);

      return true;

   }

   bool region::clip_oval(cairo_t * pdc)
   {
synch_lock ml(&user_mutex());
      double centerx    = (m_x2 + m_x1) / 2.0;
      double centery    = (m_y2 + m_y1) / 2.0;

      double radiusx    = abs(m_x2 - m_x1) / 2.0;
      double radiusy    = abs(m_y2 - m_y1) / 2.0;

      if(radiusx == 0.0 || radiusy == 0.0)
         return false;

      cairo_translate(pdc, centerx, centery);

      cairo_scale(pdc, radiusx, radiusy);

      cairo_arc(pdc, 0.0, 0.0, 1.0, 0.0, 2.0 * 3.1415);

      cairo_clip(pdc);

      cairo_scale(pdc, 1.0 / radiusx, 1.0 / radiusy);

      cairo_translate(pdc, -centerx, -centery);


      return true;

   }

   bool region::clip_polygon(cairo_t * pdc)
   {
synch_lock ml(&user_mutex());
      if(m_nCount <= 0)
         return true;


      cairo_move_to(pdc, m_lppoints[0].x, m_lppoints[0].y);

      for(int32_t i = 1; i < m_nCount; i++)
      {

         cairo_line_to(pdc, m_lppoints[i].x, m_lppoints[i].y);

      }
      cairo_clip(pdc);

      return true;

   }

   void * region::get_os_data() const
   {

      if(m_bUpdated)
         return (void *) this;

      ((region *) this)->m_bUpdated = true;

      return (void *) this;

   }

} // namespace draw2d_cairo



