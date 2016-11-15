#include "framework.h"
#include <math.h>


namespace draw2d_cairo
{


   region::region(::aura::application * papp) :
      ::object(papp)
   {

      m_pdc = NULL;

      m_psurface = NULL;

   }


   region::region(const region & region) :
      ::draw2d::region(region)
   {

      m_pdc = NULL;

      m_psurface = NULL;

   }


   region::~region()
   {

   }


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

         ::exception::throw_not_implemented(get_app());

      }

      return true;

   }


   bool region::mask(cairo_t * pgraphics)
   {

      synch_lock ml(cairo_mutex());

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

      cairo_set_antialias(m_pdc, CAIRO_ANTIALIAS_BEST);

      int x = m_rectBoundingBoxInternal.left;

      int y = m_rectBoundingBoxInternal.top;

      cairo_translate(m_pdc, -x, -y);

      cairo_push_group(m_pdc);

      _mask(m_pdc);

      cairo_pop_group_to_source(m_pdc);

      cairo_rectangle(m_pdc, 0, 0, m_rectBoundingBoxInternal.width(), m_rectBoundingBoxInternal.height());

      cairo_fill(m_pdc);

      cairo_mask_surface(pgraphics, m_psurface, m_rectBoundingBoxInternal.left, m_rectBoundingBoxInternal.top);

      cairo_destroy(m_pdc);

      m_pdc = NULL;

      cairo_surface_destroy(m_psurface);

      m_psurface = NULL;


      return true;

   }


   bool region::_mask(cairo_t * pgraphics)
   {

      switch(m_etype)
      {
      case type_none:

         break;

      case type_rect:

         mask_rect(pgraphics);

         cairo_set_source_rgba(pgraphics, 0.0, 0.0, 0.0, 1.0);

         cairo_fill(pgraphics);

         break;

      case type_oval:

         mask_oval(pgraphics);

         cairo_set_source_rgba(pgraphics, 0.0, 0.0, 0.0, 1.0);

         cairo_fill(pgraphics);

         break;

      case type_polygon:

         mask_polygon(pgraphics);

         cairo_set_source_rgba(pgraphics, 0.0, 0.0, 0.0, 1.0);

         cairo_fill(pgraphics);

         break;

      case type_poly_polygon:

         mask_polygon(pgraphics);

         cairo_set_source_rgba(pgraphics, 0.0, 0.0, 0.0, 1.0);

         cairo_fill(pgraphics);

         break;

      case type_combine:

         mask_combine(pgraphics);

         cairo_set_operator(pgraphics, CAIRO_OPERATOR_SOURCE);

         cairo_paint(pgraphics);

         break;

      default:

         ::exception::throw_not_implemented(get_app());

      }

      return true;

   }


   bool region::mask_rect(cairo_t * pgraphics)
   {

      synch_lock ml(cairo_mutex());

      cairo_rectangle(pgraphics, m_x1, m_y1, m_x2, m_y2);

      return true;

   }


   bool region::mask_oval(cairo_t * pgraphics)
   {

      synch_lock ml(cairo_mutex());

      double centerx    = (m_x2 + m_x1) / 2.0;

      double centery    = (m_y2 + m_y1) / 2.0;

      double radiusx    = fabs(m_x2 - m_x1) / 2.0;

      double radiusy    = fabs(m_y2 - m_y1) / 2.0;

      if(radiusx == 0.0 || radiusy == 0.0)
      {

         return false;

      }

      cairo_keep k(pgraphics);

      cairo_translate(pgraphics, centerx, centery);

      cairo_scale(pgraphics, radiusx, radiusy);

      cairo_arc(pgraphics, 0.0, 0.0, 1.0, 0.0, 2.0 * 3.1415);

      return true;

   }


   bool region::mask_polygon(cairo_t * pgraphics)
   {

      synch_lock ml(cairo_mutex());

      if(m_nCount <= 0)
      {

         return true;

      }

      cairo_move_to(pgraphics, m_lppoints[0].x, m_lppoints[0].y);

      for(int32_t i = 1; i < m_nCount; i++)
      {

         cairo_line_to(pgraphics, m_lppoints[i].x, m_lppoints[i].y);

      }

      return true;

   }


   bool region::mask_poly_polygon(cairo_t * pgraphics)
   {

      synch_lock ml(cairo_mutex());

      int32_t n = 0;

      for(int32_t i = 0; i < m_nCount; i++)
      {

         int32_t jCount = m_lppolycounts[i];

         if(jCount > 0)
         {

            cairo_move_to(pgraphics, m_lppoints[n].x, m_lppoints[n].y);

            n++;

            for(int32_t j = 1; i < jCount; j++)
            {

               cairo_line_to(pgraphics, m_lppoints[n].x, m_lppoints[n].y);

               n++;

            }

         }

      }

      return true;

   }


   bool region::mask_combine(cairo_t * pgraphics)
   {

      synch_lock ml(cairo_mutex());

      cairo_push_group(pgraphics);

      m_pregion1.cast < ::draw2d_cairo::region >()->_mask(pgraphics);

      if(m_ecombine == ::draw2d::region::combine_add)
      {

         cairo_set_operator(pgraphics, CAIRO_OPERATOR_SOURCE);

      }
      else if(m_ecombine == ::draw2d::region::combine_exclude)
      {

         cairo_set_operator(pgraphics, CAIRO_OPERATOR_CLEAR);

      }
      else if(m_ecombine == ::draw2d::region::combine_intersect)
      {

         cairo_set_operator(pgraphics, CAIRO_OPERATOR_IN);

      }
      else
      {

         cairo_set_operator(pgraphics, CAIRO_OPERATOR_SOURCE);

      }

      m_pregion2.cast < ::draw2d_cairo::region >()->_mask(pgraphics);

      cairo_pop_group_to_source(pgraphics);

      return true;

   }


   bool region::clip(cairo_t * pgraphics)
   {

      synch_lock ml(cairo_mutex());

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

         clip_rect(pgraphics);

         break;

      case type_oval:

         clip_oval(pgraphics);

         break;

      case type_polygon:

         clip_polygon(pgraphics);

         break;

      default:

         ::exception::throw_not_implemented(get_app());

      }

      return true;

   }


   bool region::clip_rect(cairo_t * pgraphics)
   {

      synch_lock ml(cairo_mutex());

      cairo_rectangle(pgraphics, m_x1, m_y1, m_x2, m_y2);

      cairo_clip(pgraphics);

      return true;

   }


   bool region::clip_oval(cairo_t * pgraphics)
   {

      synch_lock ml(cairo_mutex());

      double centerx    = (m_x2 + m_x1) / 2.0;

      double centery    = (m_y2 + m_y1) / 2.0;

      double radiusx    = fabs(m_x2 - m_x1) / 2.0;

      double radiusy    = fabs(m_y2 - m_y1) / 2.0;

      if(radiusx == 0.0 || radiusy == 0.0)
      {

         return false;

      }

      cairo_translate(pgraphics, centerx, centery);

      cairo_scale(pgraphics, radiusx, radiusy);

      cairo_arc(pgraphics, 0.0, 0.0, 1.0, 0.0, 2.0 * 3.1415);

      cairo_clip(pgraphics);

      cairo_scale(pgraphics, 1.0 / radiusx, 1.0 / radiusy);

      cairo_translate(pgraphics, -centerx, -centery);

      return true;

   }


   bool region::clip_polygon(cairo_t * pgraphics)
   {

      synch_lock ml(cairo_mutex());

      if(m_nCount <= 0)
      {

         return true;

      }

      cairo_move_to(pgraphics, m_lppoints[0].x, m_lppoints[0].y);

      for(int32_t i = 1; i < m_nCount; i++)
      {

         cairo_line_to(pgraphics, m_lppoints[i].x, m_lppoints[i].y);

      }

      cairo_clip(pgraphics);

      return true;

   }


   void * region::get_os_data() const
   {

      if(m_bUpdated)
      {

         return (void *) this;

      }

      ((region *) this)->m_bUpdated = true;

      return (void *) this;

   }


} // namespace draw2d_cairo







