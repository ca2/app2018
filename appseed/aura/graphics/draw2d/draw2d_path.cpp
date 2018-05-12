#include "framework.h"
//#include "draw2d.h"
//#include <math.h>


namespace draw2d
{


   static double g_dPi = atan(1.0) * 4.0;


   path::string_path::string_path()
   {
      m_x = 0;
      m_y = 0;
   }

   path::string_path::string_path(const string_path & stringpath)
   {

      operator = (stringpath);

   }

   path::string_path::~string_path()
   {
   }

   path::string_path & path::string_path::operator = (const string_path & stringpath)
   {

      if(this == &stringpath)
         return *this;

      m_x            = stringpath.m_x;
      m_y            = stringpath.m_y;
      m_strText      = stringpath.m_strText;
      m_spfont       = stringpath.m_spfont;

      return *this;

   }


   path::element::element()
   {

      m_etype = type_none;

   }

   path::element::element(const element & e)
   {

      operator = (e);

   }

   path::element::~element()
   {



   }

   path::element & path::element::operator = (const element & e)
   {

      if(this == &e)
         return *this;

      m_etype = e.m_etype;

      if(m_etype == type_string)
      {
         m_stringpath = e.m_stringpath;
      }
      else
      {
         u = e.u;
      }

      return *this;

   }


   path::path()
   {

      m_bUpdated     = false;
      m_bFill        = false;
      m_efillmode    = ::draw2d::fill_mode_winding;
      m_bHasPoint    = false;

   }

   path::path(::aura::application * papp) :
      ::object(papp)
   {

      m_bUpdated     = false;
      m_bFill        = false;
      m_efillmode    = ::draw2d::fill_mode_winding;
      m_bHasPoint    = false;

   }

   path::~path()
   {

   }


   bool path::has_current_point()
   {

      return m_bHasPoint;

   }

   pointd path::last_point()
   {

      if(!m_bHasPoint)
         _throw(::simple_exception(get_app(), "there is no point"));

      return m_pt;

   }



   bool path::add_arc(const RECT & rect, double dStart, double dAngle)
   {

      if(width(rect) <= 0 || height(rect) <= 0)
         return true;

      sp(element) e;

      e = canew(element);

      e->m_etype               = element::type_arc;
      e->u.m_arc.m_xCenter     = ((double) rect.right + (double) rect.left) / 2.0;
      e->u.m_arc.m_yCenter     = ((double) rect.bottom + (double) rect.top) / 2.0;
      e->u.m_arc.m_dRadiusX    = (double) rect.right - e->u.m_arc.m_xCenter;
      e->u.m_arc.m_dRadiusY    = (double) rect.bottom - e->u.m_arc.m_yCenter;
      e->u.m_arc.m_dAngle1     = dStart * g_dPi / 180.0;
      e->u.m_arc.m_dAngle2     = e->u.m_arc.m_dAngle1 + dAngle * g_dPi / 180.0;
      e->u.m_arc.m_dAngle      = dAngle * g_dPi / 180.0;
      e->u.m_arc.m_ptStart.x   = e->u.m_arc.m_xCenter + e->u.m_arc.m_dRadiusX * cos(e->u.m_arc.m_dAngle1);
      e->u.m_arc.m_ptStart.y   = e->u.m_arc.m_yCenter + e->u.m_arc.m_dRadiusY * sin(e->u.m_arc.m_dAngle1);
      e->u.m_arc.m_ptEnd.x     = e->u.m_arc.m_xCenter + e->u.m_arc.m_dRadiusX * cos(e->u.m_arc.m_dAngle2);
      e->u.m_arc.m_ptEnd.y     = e->u.m_arc.m_yCenter + e->u.m_arc.m_dRadiusY * sin(e->u.m_arc.m_dAngle2);

      m_elementa.add(e);

      m_bHasPoint = true;
      m_pt.x = (LONG)e->u.m_arc.m_ptEnd.x;
      m_pt.y = (LONG)e->u.m_arc.m_ptEnd.y;

      m_bUpdated = false;

      return true;

   }



   bool path::varc(const POINT & pt, double h, double dAngle)
   {

      if (fabs(dAngle) <= 0.00001 || fabs(h) <= 0.001)
      {
         return false;
      }

      dAngle *= 3.1415 / 180.0;

      sp(element) e;

      e = canew(element);
      double t = 3.1415 / 2.0 - dAngle;
      double x = pt.x;
      double y = pt.y;
      e->u.m_arc.m_yCenter = pt.y + h/2.0;
      double b = pt.y - e->u.m_arc.m_yCenter / sin(t);
      double a = sqrt((b*b*x*x) / (y*y - b *b));

      e->u.m_arc.m_dRadiusX = a;
      e->u.m_arc.m_dRadiusY = b;
      e->m_etype = element::type_arc;
      e->u.m_arc.m_xCenter = pt.x - a * cos(t);
      e->u.m_arc.m_dAngle1 = t * g_dPi / 180.0;
      e->u.m_arc.m_dAngle2 = e->u.m_arc.m_dAngle1 / 2.0 - 90.0;
      e->u.m_arc.m_dAngle = (e->u.m_arc.m_dAngle2 - e->u.m_arc.m_dAngle1) * g_dPi / 180.0;

      e->u.m_arc.m_ptStart.x = e->u.m_arc.m_xCenter + e->u.m_arc.m_dRadiusX * cos(e->u.m_arc.m_dAngle1);
      e->u.m_arc.m_ptStart.y = e->u.m_arc.m_yCenter + e->u.m_arc.m_dRadiusY * sin(e->u.m_arc.m_dAngle1);
      e->u.m_arc.m_ptEnd.x = e->u.m_arc.m_xCenter + e->u.m_arc.m_dRadiusX * cos(e->u.m_arc.m_dAngle2);
      e->u.m_arc.m_ptEnd.y = e->u.m_arc.m_yCenter + e->u.m_arc.m_dRadiusY * sin(e->u.m_arc.m_dAngle2);

      m_elementa.add(e);

      m_bHasPoint = true;
      m_pt.x = (LONG)e->u.m_arc.m_ptEnd.x;
      m_pt.y = (LONG)e->u.m_arc.m_ptEnd.y;

      m_bUpdated = false;

      return true;

   }

   bool path::harc(const POINT & pt, double w, double dAngle)
   {


      if (fabs(dAngle) <= 0.00001 || fabs(w) <= 0.001)
      {
         return false;
      }

      dAngle *= g_dPi / 180.0;

      sp(element) e;

      e = canew(element);
      double t = dAngle + g_dPi / 2.0;
      e->u.m_arc.m_xCenter = pt.x + w / 2.0;
      double a = (pt.x - e->u.m_arc.m_xCenter) / cos(t);
      double x = pt.x - e->u.m_arc.m_xCenter;
      double b = 0;
      double y = b * b  *w / (4.0* tan(dAngle));
      y = 0;
      b = sqrt((a*a*y*y) / (a*a-x *x));

      e->u.m_arc.m_dRadiusX = a;
      e->u.m_arc.m_dRadiusY = b;
      e->m_etype = element::type_arc;
      e->u.m_arc.m_yCenter = pt.y - b * sin(t);
      e->u.m_arc.m_dAngle1 = t;
      e->u.m_arc.m_dAngle2 = fmod(g_dPi - e->u.m_arc.m_dAngle1, g_dPi);
      e->u.m_arc.m_dAngle = e->u.m_arc.m_dAngle2 - e->u.m_arc.m_dAngle1;

      e->u.m_arc.m_ptStart.x = e->u.m_arc.m_xCenter + e->u.m_arc.m_dRadiusX * cos(e->u.m_arc.m_dAngle1);
      e->u.m_arc.m_ptStart.y = e->u.m_arc.m_yCenter + e->u.m_arc.m_dRadiusY * sin(e->u.m_arc.m_dAngle1);
      e->u.m_arc.m_ptEnd.x = e->u.m_arc.m_xCenter + e->u.m_arc.m_dRadiusX * cos(e->u.m_arc.m_dAngle2);
      e->u.m_arc.m_ptEnd.y = e->u.m_arc.m_yCenter + e->u.m_arc.m_dRadiusY * sin(e->u.m_arc.m_dAngle2);

      m_elementa.add(e);

      m_bHasPoint = true;
      m_pt.x = (LONG)e->u.m_arc.m_ptEnd.x;
      m_pt.y = (LONG)e->u.m_arc.m_ptEnd.y;

      m_bUpdated = false;

      return true;

   }

   bool path::add_arc(const RECTD & rect,double dStart,double dAngle)
   {

      if(width(rect) <= 0 || height(rect) <= 0)
         return true;

      sp(element) e;

      e = canew(element);

      e->m_etype               = element::type_arc;
      e->u.m_arc.m_xCenter     = (rect.right + rect.left) / 2.0;
      e->u.m_arc.m_yCenter     = (rect.bottom + rect.top) / 2.0;
      e->u.m_arc.m_dRadiusX    = fabs(rect.right - rect.left) / 2.0;
      e->u.m_arc.m_dRadiusY    = fabs(rect.bottom - rect.top) / 2.0;
      e->u.m_arc.m_dAngle1     = dStart * g_dPi / 180.0;
      e->u.m_arc.m_dAngle      = dAngle * g_dPi / 180.0;
      e->u.m_arc.m_dAngle2     = e->u.m_arc.m_dAngle1 + e->u.m_arc.m_dAngle;
      e->u.m_arc.m_ptStart.x   = e->u.m_arc.m_xCenter + e->u.m_arc.m_dRadiusX * cos(e->u.m_arc.m_dAngle1);
      e->u.m_arc.m_ptStart.y   = e->u.m_arc.m_yCenter + e->u.m_arc.m_dRadiusY * sin(e->u.m_arc.m_dAngle1);
      e->u.m_arc.m_ptEnd.x     = e->u.m_arc.m_xCenter + e->u.m_arc.m_dRadiusX * cos(e->u.m_arc.m_dAngle2);
      e->u.m_arc.m_ptEnd.y     = e->u.m_arc.m_yCenter + e->u.m_arc.m_dRadiusY * sin(e->u.m_arc.m_dAngle2);

      m_elementa.add(e);

      m_bHasPoint = true;
      m_pt.x = e->u.m_arc.m_ptEnd.x;
      m_pt.y = e->u.m_arc.m_ptEnd.y;

      m_bUpdated = false;

      return true;

   }


   bool path::add_move(int32_t x,int32_t y)
   {

      return add_move((double)x,(double)y);

   }


   bool path::add_move(double x, double y)
   {

      sp(element) e;

      e = canew(element);

      e->m_etype               = element::type_move;
      e->u.m_move.m_x          = x;
      e->u.m_move.m_y          = y;

      m_elementa.add(e);

      m_bHasPoint = true;
      m_pt.x = x;
      m_pt.y = y;

      m_bUpdated = false;

      return true;

   }


   bool path::add_line(double x,double y)
   {

      if(!has_current_point())
      {
         return add_move(x,y);
      }

      sp(element) e;

      e = canew(element);

      e->m_etype               = element::type_line;
      e->u.m_line.m_x          = x;
      e->u.m_line.m_y          = y;

      m_elementa.add(e);

      m_bHasPoint = true;
      m_pt.x = x;
      m_pt.y = y;

      m_bUpdated = false;

      return true;

   }


   bool path::add_line(int32_t x, int32_t y)
   {

      return add_line((double)x,(double)y);

   }


   bool path::add_line(double x,double y,double x2,double y2)
   {

      bool bOk1 = add_line(x,y);

      bool bOk2 = add_line(x2,y2);

      return bOk1 && bOk2;

   }


   bool path::add_line(int32_t x,int32_t y,int32_t x2,int32_t y2)
   {

      bool bOk1 = add_line(x, y);

      bool bOk2 = add_line(x2, y2);

      return bOk1 && bOk2;

   }


   bool path::begin_figure(bool bFill, ::draw2d::e_fill_mode efillmode)
   {

      m_bFill = bFill;

      m_efillmode = efillmode;

      m_bHasPoint = false;

      m_bUpdated = false;

      return true;

   }

   bool path::end_figure(bool bClose)
   {


      sp(element) e;

      e = canew(element);

      e->m_etype               = element::type_end;

      e->u.m_end.m_bClose      = bClose;

      m_elementa.add(e);

      m_bUpdated = false;

      return true;

   }



   bool path::add_string(int32_t x,int32_t y,const string & strText,::draw2d::font_sp spfont)
   {

      sp(element) e;

      e = canew(element);

      e->m_etype                     = element::type_string;
      e->m_stringpath.m_x            = x;
      e->m_stringpath.m_y            = y;
      e->m_stringpath.m_strText      = strText;
      e->m_stringpath.m_spfont       = spfont;

      m_elementa.add(e);

      return true;

   }



   bool path::add_rect(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {

      sp(element) e;

      e = canew(element);

      e->m_etype               = element::type_rect;

      e->u.m_rect.m_x = x1;
      e->u.m_rect.m_y = y1;
      e->u.m_rect.m_cx = x2-x1;
      e->u.m_rect.m_cy = y2-y1;

      m_elementa.add(e);

      m_bUpdated = false;

      return true;

   }


   bool path::add_rect(LPCRECT lpcrect)
   {

      return add_rect(lpcrect->left,lpcrect->top,lpcrect->right,lpcrect->bottom);

   }


   bool path::add_rect(const RECT & rect)
   {

      return add_rect(&rect);

   }

   bool path::add_line(point p1, point p2)
   {

      return add_line(p1.x, p1.y, p2.x, p2.y);

   }


   bool path::add_line(pointd p)
   {

      return add_line(p.x,p.y);

   }


   bool path::add_lines(const POINT * lppoint, ::count nCount)
   {

      bool bOk = true;

      for(int32_t i = 0; i < nCount; i++)
      {

         if(!add_line(lppoint[i].x, lppoint[i].y))
            bOk = false;
      }

      return bOk;

   }


   bool path::add_lines(const POINTD * lppoint, ::count nCount)
   {

      bool bOk = true;

      for(int32_t i = 0; i < nCount; i++)
      {

         if(!add_line(lppoint[i]))
            bOk = false;
      }

      return bOk;

   }

   bool path::add_move(pointd p)
   {

      return add_move(p.x,p.y);

   }

   void * path::detach()
   {

      ::exception::throw_interface_only(get_app());

      return NULL;

   }


   void path::get_bounding_rect(LPRECT lprect)
   {

      lprect->right     = 0x80000000;
      lprect->left      = 0x7fffffff;
      lprect->bottom    = 0x80000000;
      lprect->top       = 0x7fffffff;


      for(int i = 0; i < m_elementa.get_count(); i++)
      {

         get_bounding_rect(lprect, m_elementa(i));

      }

   }

   void path::get_bounding_rect(LPRECT lprect, element & e)
   {

      switch(e.m_etype)
      {
      case ::draw2d::path::element::type_arc:
         get_bounding_rect(lprect, e.u.m_arc);
         break;
      case ::draw2d::path::element::type_line:
         get_bounding_rect(lprect,e.u.m_line);
         break;
      case ::draw2d::path::element::type_move:
         get_bounding_rect(lprect, e.u.m_move);
         break;
      case ::draw2d::path::element::type_rect:
         get_bounding_rect(lprect,e.u.m_rect);
         break;
      case ::draw2d::path::element::type_string:
         get_bounding_rect(lprect,e.m_stringpath);
         break;
      case ::draw2d::path::element::type_end:
      {

      }
      break;
      default:
         _throw(::simple_exception(get_app(), "unexpected simple os graphics element type"));
      }




   }

   void path::get_bounding_rect(LPRECT lprect,rect & r)
   {

      lprect->left = (LONG) r.m_x;
      lprect->top = (LONG)r.m_y;
      lprect->right= (LONG)(r.m_x + r.m_cx);
      lprect->bottom = (LONG)(r.m_y + r.m_cy);

   }

   void path::get_bounding_rect(LPRECT lprect, arc & a)
   {

      ::rect r;

      r.left         = (LONG) (a.m_xCenter - a.m_dRadiusX);

      r.right        = (LONG) (a.m_xCenter + a.m_dRadiusX);

      r.top          = (LONG) (a.m_yCenter - a.m_dRadiusY);

      r.bottom       = (LONG) (a.m_yCenter + a.m_dRadiusY);

      POINT pt1;

      POINT pt2;

      lprect->left   = (LONG) MIN(a.m_xCenter, lprect->left);

      lprect->top    = (LONG) MIN(a.m_yCenter, lprect->top);

      lprect->right  = (LONG) MAX(a.m_xCenter, lprect->right);

      lprect->bottom = (LONG) MAX(a.m_yCenter, lprect->bottom);

//      double e = a.m_dRadiusY / a.m_dRadiusX;

      double f1 = a.m_dAngle1;

      double f2 = a.m_dAngle2;

      pt1.x          = (LONG) (a.m_xCenter + a.m_dRadiusX * cos(f1));

      pt1.y          = (LONG) (a.m_yCenter - a.m_dRadiusY * sin(f1));

      pt2.x          = (LONG) (a.m_xCenter + a.m_dRadiusX * cos(f2));

      pt2.y          = (LONG) (a.m_yCenter - a.m_dRadiusY * sin(f2));

      lprect->left   = (LONG) MIN(pt1.x, lprect->left);

      lprect->top    = (LONG) MIN(pt1.y, lprect->top);

      lprect->right  = (LONG) MAX(pt1.x, lprect->right);

      lprect->bottom = (LONG) MAX(pt1.y, lprect->bottom);

      lprect->left   = (LONG) MIN(pt2.x, lprect->left);

      lprect->top    = (LONG) MIN(pt2.y, lprect->top);

      lprect->right  = (LONG) MAX(pt2.x, lprect->right);

      lprect->bottom = (LONG) MAX(pt2.y, lprect->bottom);

      if((f1 >= 0 && f2 <= f1) || (f1 <= 0 && f2 >= 0))
      {

         lprect->left   = (LONG) MIN(r.right, lprect->left);

         lprect->right  = (LONG) MAX(r.right, lprect->right);

      }

      f1 -= System.math().GetPi() / 2.0;

      f2 -= System.math().GetPi() / 2.0;

      if((f1 >= 0 && f2 <= f1) || (f1 <= 0 && f2 >= 0))
      {

         lprect->top    = (LONG) MIN(r.top, lprect->top);

         lprect->bottom = (LONG) MAX(r.top, lprect->bottom);

      }

      f1 -= System.math().GetPi() / 2.0;

      f2 -= System.math().GetPi() / 2.0;

      if((f1 >= 0 && f2 <= f1) || (f1 <= 0 && f2 >= 0))
      {

         lprect->left   = (LONG) MIN(r.left, lprect->left);

         lprect->right  = (LONG) MAX(r.left, lprect->right);

      }

      f1 -= System.math().GetPi() / 2.0;

      f2 -= System.math().GetPi() / 2.0;

      if((f1 >= 0 && f2 <= f1) || (f1 <= 0 && f2 >= 0))
      {

         lprect->top    = (LONG) MIN(r.bottom, lprect->top);

         lprect->bottom = (LONG) MAX(r.bottom, lprect->bottom);

      }

   }

   void path::get_bounding_rect(LPRECT lprect, move & m)
   {

      lprect->left   = (LONG) MIN(m.m_x, lprect->left);

      lprect->top    = (LONG) MIN(m.m_y, lprect->top);

      lprect->right  = (LONG) MAX(m.m_x, lprect->right);

      lprect->bottom = (LONG) MAX(m.m_y, lprect->bottom);

   }


   void path::get_bounding_rect(LPRECT lprect, line & l)
   {

      lprect->left   = (LONG) MIN(l.m_x, lprect->left);

      lprect->top    = (LONG) MIN(l.m_y, lprect->top);

      lprect->right  = (LONG) MAX(l.m_x, lprect->right);

      lprect->bottom = (LONG) MAX(l.m_y, lprect->bottom);

   }

   void path::get_bounding_rect(LPRECT lprect,string_path & p)
   {


   }



   //
   // CRoundRect.h : Version 1.0 - see article at CodeProject.com
   //
   // Author:  Darren Sessions
   //
   //
   // Description:
   //     CRoundRect Draws or Fills rounded rectangles for GDI+.  It was implemented
   //	   to overcome the asymmetric issues associated with GDI+ round rectangles
   //
   // History
   //     Version 1.0 - 2008 June 24
   //     - Initial public release
   //
   // License:
   //     This software is released under the Code Project Open License (CPOL),
   //     which may be found here:  http://www.codeproject.com/info/eula.aspx
   //     You are free to use this software in any way you like, except that you
   //     may not sell this source code.
   //
   //     This software is provided "as is" with no expressed or implied warranty.
   //     I accept no liability for any damage or loss of business that this
   //     software may cause.
   //
   ///////////////////////////////////////////////////////////////////////////////

   //=============================================================================
   //
   // get_round_rect()
   //
   // Purpose:     Defines a Rounded rectangle and places it in the GraphicsPath
   //
   // Parameters:  path.m_ppath		- [out] pointer to GraphicsPath that will recieve the
   //									path data
   //				r			- [in]	Rect that defines the round rectangle boundaries
   //				dia			- [in]	diameter of the rounded corners (2*radius)
   //
   // Returns:     None
   //
   bool path::add_round_rect(const RECT & r, int32_t dia)
   {

      // diameter can't exceed width or height
      if (dia > width(r))	dia = (int32_t)width(r);
      if (dia > height(r))	dia = (int32_t)height(r);

      RECT Corner(rect_dim(r.left, r.top, dia, dia));
      add_arc(Corner, 180, 90);

      // top right
      Corner.left += (width(r) - dia - 1);
      Corner.right = Corner.left + dia;
      add_arc(Corner, 270, 90);

      // bottom right
      Corner.top += (height(r) - dia - 1);
      Corner.bottom = Corner.top + dia;
      add_arc(Corner, 0, 90);

      // bottom left
      Corner.left -= (width(r) - dia - 1);
      Corner.right = Corner.left + dia;
      add_arc(Corner, 90, 90);

      return true;

   }


   bool path::add_round_top_left(const RECT & r, int32_t dia)
   {
      // diameter can't exceed width or height
      if (dia > width(r))	dia = width(r);
      if (dia > height(r))	dia = height(r);

      // define a corner
      RECT Corner(rect_dim(r.left, r.top, dia, dia));

      // begin path
      //path.m_ppath->Reset();


      // top left
      //   path.m_ppath->AddArc(Corner, 180, 90);

      // tweak needed for radius of 10 (dia of 20)
      int32_t dia2 = dia;
      if (dia % 2 == 0)
      {
         dia2++;
         //Corner.Height += 1;
         //height(r) -=1; //height(r) -= 1;
      }

      // top right
      Corner.left += (width(r) - dia - 1);
      //path.m_ppath->AddArc(Corner, 270, 90);

      // bottom right
      Corner.top += (height(r) - dia - 1);
      //path.m_ppath->AddArc(Corner,   0, 90);

      // bottom left
      Corner.left -= (width(r) - dia - 1);
      Corner.right = Corner.left + dia2;
      add_arc(Corner, 135, 45);

      add_line(r.left, r.top + height(r) - dia / 2, r.left, r.top + dia / 2);


      Corner.top -= (height(r) - dia - 1);
      Corner.bottom = Corner.top + dia2;
      add_arc(Corner, 180, 90);

      add_line(r.left + dia / 2, r.top, r.left + width(r) - dia / 2, r.top);

      Corner.left += (width(r) - dia - 1);
      Corner.right = Corner.left + dia2;
      add_arc(Corner, 270, 45);


      // end path
      //path.m_ppath->CloseFigure();

      return true;

   }



   bool path::add_round_bottom_right(const RECT & r, int32_t dia)
   {
      // diameter can't exceed width or height
      if (dia > width(r))	dia = width(r);
      if (dia > height(r))	dia = height(r);

      // define a corner
      RECT Corner(rect_dim(r.left, r.top, dia, dia));

      // begin path
      //path.m_ppath->Reset();


      // top left
      //   path.m_ppath->AddArc(Corner, 180, 90);

      // tweak needed for radius of 10 (dia of 20)
      int32_t dia2 = dia;
      if (dia % 2 == 0)
      {
         dia2++;
         //Corner.Height += 1;
         //height(r) -=1; //height(r) -= 1;
      }

      // top right
      Corner.left += (width(r) - dia - 1);
      Corner.right = Corner.left + dia2;
      add_arc(Corner, 315, 45);

      add_line(r.left + width(r), r.top + dia / 2, r.left + width(r), r.top + height(r) - dia / 2);

      // bottom right
      Corner.top += (height(r) - dia - 1);
      Corner.bottom = Corner.top + dia2;
      add_arc(Corner, 0, 90);

      add_line(r.left + dia / 2, r.top + height(r), r.left + width(r) - dia / 2, r.top + height(r));

      // bottom left
      Corner.left -= (width(r) - dia - 1);
      Corner.right = Corner.left + dia2;
      add_arc(Corner, 90, 45);




      //Corner.Y -= (height(r) - dia - 1);
      //path.m_ppath->AddArc(Corner, 180, 90);



      //Corner.X += (width(r) - dia - 1);
      //path.m_ppath->AddArc(Corner, 275, 45);


      // end path
      //path.m_ppath->CloseFigure();

      return true;

   }

   bool path::add_arc_label(LPCRECT lpcrect)
   {

      ::rectd r;
      ::rectd r2;

      {

         r.left = lpcrect->left;
         r.top = lpcrect->top;
         r.right = lpcrect->left + height(lpcrect);
         r.bottom = lpcrect->bottom;

         add_arc(r,90,180);

      }

      {

         r2.left = lpcrect->left + height(lpcrect) / 2.0;
         r2.top = lpcrect->top;
         r2.right = lpcrect->right - height(lpcrect) / 2.0;
         r2.bottom = lpcrect->bottom;

         add_line(r2.right, r2.top);

      }

      {

         r.right = lpcrect->right;
         r.top = lpcrect->top;
         r.left = lpcrect->right - height(lpcrect);
         r.bottom = lpcrect->bottom;

         add_arc(r,-90,180);

      }

      {

         add_line(r2.left,r2.bottom);

      }

      return true;

   }


} // namespace draw2d






