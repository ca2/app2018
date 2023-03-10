#include "framework.h"


namespace draw2d_direct2d
{

   
   region::region(::aura::application * papp) :
      ::object(papp)
   { 
      
      m_bUpdated = false;
      
   }

   region::~region()
   {
      
      destroy();
      
   }


   int region::GetRegionData(LPRGNDATA lpRgnData, int nDataSize) const
   { 
   
      _throw(todo(get_app()));

      //   ASSERT(get_os_data() != NULL); return (int)::GetRegionData((HRGN)get_os_data(), nDataSize, lpRgnData); 
   
   }
   void region::SetRectRgn(int x1, int y1, int x2, int y2)
   { 
   
      _throw(todo(get_app()));

      //ASSERT(get_os_data() != NULL); ::SetRectRgn((HRGN)get_os_data(), x1, y1, x2, y2); 
   }
   
   void region::SetRectRgn(LPCRECT lpRect)
   { 
   
      _throw(todo(get_app()));

   //   ::SetRectRgn((HRGN)get_os_data(), lpRect->left, lpRect->top, lpRect->right, lpRect->bottom); 
   }

   int region::CombineRgn(const ::draw2d::region* pRgn1, const ::draw2d::region* pRgn2, int nCombineMode)
   { 
   
      _throw(todo(get_app()));

      //ASSERT(get_os_data() != NULL); return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgn1->get_os_data(),
   //(HRGN)pRgn2->get_os_data(), nCombineMode); 
   }
   int region::CopyRgn(const ::draw2d::region* pRgnSrc)
   { 
   
      _throw(todo(get_app()));

      //ASSERT(get_os_data() != NULL); return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgnSrc->get_os_data(), NULL, RGN_COPY); 
   }

   bool region::EqualRgn(const ::draw2d::region* pRgn) const
   {
   
      _throw(todo(get_app()));

      
   //   ASSERT(get_os_data() != NULL); 
      
     // return ::EqualRgn((HRGN)get_os_data(), (HRGN)pRgn->get_os_data()) != FALSE;

   }

   int region::OffsetRgn(int x, int y)
   {
   
      _throw(todo(get_app()));

      
      //ASSERT(get_os_data() != NULL); return ::OffsetRgn((HRGN)get_os_data(), x, y); 
   }
   int region::OffsetRgn(POINT point)
   {
   
      _throw(todo(get_app()));

      
      //ASSERT(get_os_data() != NULL); return ::OffsetRgn((HRGN)get_os_data(), point.x, point.y); 
   }
   int region::GetRgnBox(LPRECT lpRect) const
   {
   
      _throw(todo(get_app()));

      
      //ASSERT(get_os_data() != NULL); return ::GetRgnBox((HRGN)get_os_data(), lpRect); 
   }
   
   bool region::contains(int x, int y) const
   { 
   
      ((region *) this)->defer_update();

      BOOL b = FALSE;

      HRESULT hr = m_pgeometry->FillContainsPoint(D2D1::Point2F((FLOAT) x, (FLOAT) y), NULL, &b);

      if(FAILED(hr))
         return false;

      return b != FALSE;

      //Gdiplus::PointF pointf((Gdiplus::REAL) x, (Gdiplus::REAL) y);
      //
      //ASSERT(get_os_data() != NULL); //return ::PtInRegion((HRGN)get_os_data(), x, y); 

      //return m_pgeometry->IsVisible(pointf)  != FALSE;
   
   }

   bool region::contains(POINT point) const
   { 

      return contains(point.x, point.y);


      //_throw(todo(get_app()));

      

      //return ::PtInRegion((HRGN)get_os_data(), point.x, point.y); 
      //Gdiplus::PointF pointf((Gdiplus::REAL) point.x, (Gdiplus::REAL) point.y);
      //
      //ASSERT(get_os_data() != NULL); //return ::PtInRegion((HRGN)get_os_data(), x, y); 

      //return m_pgeometry->IsVisible(pointf)  != FALSE;

   }

   bool region::RectInRegion(LPCRECT lpRect) const
   { 
   
      _throw(todo(get_app()));

      
      //ASSERT(get_os_data() != NULL); return ::RectInRegion((HRGN)get_os_data(), lpRect); 

      //Gdiplus::RectF rectf((Gdiplus::REAL) lpRect->left, (Gdiplus::REAL) lpRect->top, (Gdiplus::REAL) (lpRect->right - lpRect->left), (Gdiplus::REAL) (lpRect->bottom - lpRect->top));
      //
      //ASSERT(get_os_data() != NULL); //return ::PtInRegion((HRGN)get_os_data(), x, y); 

      //return m_pgeometry->IsVisible(rectf)  != FALSE;

   }

   void * region::get_os_data() const
   {
   
      ((region *) this)->defer_update();

      return (ID2D1Geometry *) m_pgeometry.Get();

   }



   void region::defer_update()
   {
      
      if(m_pgeometry == NULL || !m_bUpdated)
      {

         if(m_pgeometry != NULL)
         {


            m_pgeometry = nullptr;

         }

         m_pgeometry = get();

      }

   }

   ID2D1Geometry * region::get()
   {
      
      switch(m_etype)
      {
      case type_none:
         {
            
            ID2D1PathGeometry * ppathgeometry = NULL;

            HRESULT hr = get_d2d1_factory1()->CreatePathGeometry(&ppathgeometry);

            if(FAILED(hr))
               return NULL;

            return ppathgeometry;

         }
      case type_rect:
         return get_rect();
      case type_oval:
         return get_oval();
      case type_polygon:
         return get_polygon();
      case type_poly_polygon:
         return get_polygon();
      case type_combine:
         return get_combine();
      default:
         ::exception::throw_not_implemented(get_app());
      }

      return NULL;

   }

   ID2D1Geometry * region::get_rect()
   {

      ID2D1RectangleGeometry * pgeometry = NULL;

      get_d2d1_factory1()->CreateRectangleGeometry(D2D1::RectF((FLOAT)m_x1, (FLOAT)m_y1, (FLOAT)m_x2, (FLOAT)m_y2), &pgeometry);

      return pgeometry;
         
   }

   ID2D1Geometry * region::get_oval()
   {
      
      D2D1_ELLIPSE ellipse;
      
      ellipse.point.x = (float)(m_x2 + m_x1) / 2.f;
      ellipse.point.y = (float)(m_y2 + m_y1) / 2.f;
      ellipse.radiusX = (float)(m_x2 - m_x1) / 2.f;
      ellipse.radiusY = (float)(m_y2 - m_y1) / 2.f;

      ID2D1EllipseGeometry * pgeometry = NULL;

      get_d2d1_factory1()->CreateEllipseGeometry(ellipse, &pgeometry);

      return pgeometry;

   }

   ID2D1Geometry * region::get_polygon()
   {

      ::draw2d::path_sp path(allocer());

      /*point_array pa;

      for(int i = 0; i < m_nCount; i++)
      {
         pa.add(Gdiplus::PointF((Gdiplus::REAL) m_lppoints[i].x, (Gdiplus::REAL) m_lppoints[i].y));
      }*/

      /*
      if(m_efillmode == ::draw2d::fill_mode_alternate)
      {
         path.SetFillMode(Gdiplus::FillModeAlternate);
      }
      else
      {
         path.SetFillMode(Gdiplus::FillModeWinding);
      }
      */

      path->begin_figure(true, m_efillmode);
      path->add_lines(m_lppoints, m_nCount);
      path->end_figure(true);

      return (ID2D1PathGeometry *) path->detach();

   }

   ID2D1Geometry * region::get_poly_polygon()
   {

      ::draw2d::path_sp path(allocer());

      pointd_array pa;

      /*if(m_efillmode == ::draw2d::fill_mode_alternate)
      {
         path.SetFillMode(Gdiplus::FillModeAlternate);
      }
      else
      {
         path.SetFillMode(Gdiplus::FillModeWinding);
      }*/

      int n = 0;

      for(int i = 0; i < m_nCount; i++)
      {
         int jCount = m_lppolycounts[i];
         pa.remove_all();
         for(int j = 0; j < jCount; j++)
         {
            pa.add(pointd(m_lppoints[n].x, m_lppoints[n].y));
            n++;
         }
         path->begin_figure(true, m_efillmode);
         path->add_lines(pa.get_data(), (int) pa.get_count());
         path->end_figure(true);
      }

      return (ID2D1PathGeometry *) path->detach();

   }

   ID2D1Geometry * region::get_combine()
   {

      Microsoft::WRL::ComPtr <ID2D1PathGeometry> ppathgeometry ;

      HRESULT hr = get_d2d1_factory1()->CreatePathGeometry(&ppathgeometry);

      Microsoft::WRL::ComPtr < ID2D1GeometrySink > psink ;

      if (FAILED(hr))
      {

         return NULL;

      }

      hr = ppathgeometry->Open(&psink);

      if(FAILED(hr))
      {
         
         return NULL;

      }

      ID2D1Geometry * pgeometry1 = (ID2D1Geometry *) m_pregion1->get_os_data();

      ID2D1Geometry * pgeometry2 = (ID2D1Geometry *) m_pregion2->get_os_data();

      if(m_ecombine == combine_add)
      {

         hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_UNION, NULL, NULL, psink.Get());

      }
      else if(m_ecombine == combine_exclude)
      {

         hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_EXCLUDE, NULL, NULL, psink.Get());

      }
      else if(m_ecombine == combine_intersect)
      {

         hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_INTERSECT, NULL, NULL, psink.Get());

      }
      else
      {

         hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_UNION, NULL, NULL, psink.Get());

      }

      if(FAILED(hr))
      {

         return NULL;

      }


      hr = psink->Close();

      if(FAILED(hr))
      {

         return NULL;

      }

      return ppathgeometry.Detach();

   }


   bool region::destroy()
   {

      m_pgeometry = nullptr;

      return true;

   }


} // namespace draw2d_direct2d



