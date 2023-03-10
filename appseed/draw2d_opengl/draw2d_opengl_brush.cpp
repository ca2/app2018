#include "framework.h"

#undef new

namespace draw2d_opengl
{


   brush::brush(::aura::application * papp) :
      ::object(papp)
   { 

//      m_pbrush = NULL;

   }

   brush::~brush()
   { 
      //::aura::del(m_pbrush);
   }

   //bool brush::create_solid(COLORREF crColor)
   //{

   //   if(m_pbrush != NULL)
   //   {
   //      delete m_pbrush;
   //   }

   //   m_pbrush = new plusplus::SolidBrush(plusplus::Color(argb_get_a_value(crColor), argb_get_r_value(crColor), argb_get_g_value(crColor), argb_get_b_value(crColor)));

   //   return m_pbrush != NULL;
   //      
   //}

   // bool brush::CreateHatchBrush(int32_t nIndex, COLORREF crColor)
   // { 
   //    //return attach(::CreateHatchBrush(nIndex, crColor));
   //    return FALSE;
   // }
//    bool brush::CreateBrushIndirect(const LOGBRUSH* lpLogBrush)
  //  {
       //return attach(::CreateBrushIndirect(lpLogBrush)); 
    //   return FALSE;
    //}
    //bool brush::CreatePatternBrush(::draw2d::bitmap* pBitmap)
    //{ 
    //   //return attach(::CreatePatternBrush((HBITMAP)pBitmap->get_os_data()));
    //   return FALSE;
    //}
    //bool brush::CreateDIBPatternBrush(const void * lpPackedDIB, UINT nUsage)
    //{ 
    //   //return attach(::CreateDIBPatternBrushPt(lpPackedDIB, nUsage));
    //   return FALSE;
    //}
    //bool brush::CreateSysColorBrush(int32_t nIndex)
    //{ 
    //   //return attach(::GetSysColorBrush(nIndex));
    //   return FALSE;

    //}
/*    int32_t brush::GetLogBrush(LOGBRUSH* pLogBrush)
    { 
       //ASSERT(get_os_data() != NULL);
       //return ::GetObject(get_os_data(), sizeof(LOGBRUSH), pLogBrush); 
       return FALSE;
    }*/


//   void brush::construct(COLORREF crColor)
//   {
////      if (!attach(::create_solid(crColor)))
//  //       _throw(resource_exception());
//   }
//
//   void brush::construct(int32_t nIndex, COLORREF crColor)
//   {
//    //  if (!attach(::CreateHatchBrush(nIndex, crColor)))
//      //   _throw(resource_exception());
//   }
//
//   void brush::construct(::draw2d::bitmap* pBitmap)
//   {
//      //ASSERT_VALID(pBitmap);
//
//      //if (!attach(::CreatePatternBrush((HBITMAP)pBitmap->get_os_data())))
//        // _throw(resource_exception());
//   }
//
//   bool brush::CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage)
//   {
//      //ASSERT(hPackedDIB != NULL);
//      //const void * lpPackedDIB = ::GlobalLock(hPackedDIB);
//      //ASSERT(lpPackedDIB != NULL);
//      //bool bResult = attach(::CreateDIBPatternBrushPt(lpPackedDIB, nUsage));
//      //::GlobalUnlock(hPackedDIB);
//      //return bResult;
//      return FALSE;
//   }


   void brush::dump(dump_context & dumpcontext) const
   {
      ::draw2d::object::dump(dumpcontext);

      //if (get_os_data() == NULL)
        // return;

/*      LOGBRUSH lb;
      VERIFY(GetObject(sizeof(lb), &lb));
      dumpcontext << "lb.lbStyle = " << lb.lbStyle;
      dumpcontext << "\nlb.lbHatch = " << lb.lbHatch;
      dumpcontext << "\nlb.lbColor = " << (void *)(uint_ptr)lb.lbColor;

      dumpcontext << "\n";
      */
   }



   //void * brush::get_os_data() const
   //{
   //   
   //   //if(m_pbrush == NULL || !m_bUpdated)
   //   //{
   //   //   ::aura::del(((brush *) this)->m_pbrush);
   //   //   if(m_etype == type_solid)
   //   //   {
   //   //      try
   //   //      {
   //   //         ((brush *) this)->m_pbrush = new plusplus::SolidBrush(plusplus::Color(argb_get_a_value(m_cr), argb_get_r_value(m_cr), argb_get_g_value(m_cr), argb_get_b_value(m_cr)));
   //   //      }
   //   //      catch(...)
   //   //      {
   //   //      }
   //   //   }
   //   //   else if(m_etype == type_linear_gradient_point_color)
   //   //   {
   //   //      try
   //   //      {
   //   //         ((brush *) this)->m_pbrush = new plusplus::LinearGradientBrush(
   //   //            plusplus::PointF((plusplus::REAL) m_pt1.x,(plusplus::REAL) m_pt1.y),
   //   //            plusplus::PointF((plusplus::REAL) m_pt2.x,(plusplus::REAL) m_pt2.y),
   //   //            plusplus::Color(argb_get_a_value(m_cr1), argb_get_r_value(m_cr1), argb_get_g_value(m_cr1), argb_get_b_value(m_cr1)),
   //   //            plusplus::Color(argb_get_a_value(m_cr2), argb_get_r_value(m_cr2), argb_get_g_value(m_cr2), argb_get_b_value(m_cr2)));
   //   //      }
   //   //      catch(...)
   //   //      {
   //   //      }
   //   //   }
   //   //   else if(m_etype == type_radial_gradient_color)
   //   //   {
   //   //      try
   //   //      {

   //   //         plusplus::GraphicsPath * gp = new plusplus::GraphicsPath();
   //   //         gp->AddEllipse((plusplus::REAL) (m_pt.x - m_size.cx / 2),(plusplus::REAL)(m_pt.y - m_size.cy / 2),(plusplus::REAL) (m_size.cx),(plusplus::REAL) (m_size.cy));

   //   //         plusplus::PathGradientBrush * pgb = new plusplus::PathGradientBrush(gp);

   //   //         plusplus::Color c1(argb_get_a_value(m_cr1),argb_get_r_value(m_cr1),argb_get_g_value(m_cr1),argb_get_b_value(m_cr1));
   //   //         plusplus::Color c2(argb_get_a_value(m_cr2),argb_get_r_value(m_cr2),argb_get_g_value(m_cr2),argb_get_b_value(m_cr2));

   //   //            INT c = 1;

   //   //            pgb->SetCenterPoint(plusplus::PointF((plusplus::REAL) m_pt.x,(plusplus::REAL) m_pt.y));
   //   //         pgb->SetCenterColor(c1);
   //   //         pgb->SetSurroundColors(&c2,&c);

   //   //         ((brush *) this)->m_pbrush = pgb;
   //   //      }
   //   //      catch(...)
   //   //      {
   //   //      }
   //   //   }
   //   //   else
   //   //   {
   //   //      try
   //   //      {
   //   //         ((brush *) this)->m_pbrush = new plusplus::SolidBrush(plusplus::Color(argb_get_a_value(m_cr), argb_get_r_value(m_cr), argb_get_g_value(m_cr), argb_get_b_value(m_cr)));
   //   //      }
   //   //      catch(...)
   //   //      {
   //   //      }
   //   //   }
   //   //}

   //   //if(m_pbrush != NULL)
   //   //{
   //   //   ((brush *) this)->m_bUpdated = true;
   //   //}

   //   //return (void *) (plusplus::Brush *) m_pbrush;

   //   return NULL;

   //}

} // namespace draw2d_opengl


