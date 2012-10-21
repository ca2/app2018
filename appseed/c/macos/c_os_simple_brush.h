//
//  c_os_simple_brush.h
//  c
//
//  Created by Thomas Borregaard Sørensen on 9/30/12.
//
//
#pragma once


class CLASS_DECL_c simple_brush
{
public:
   
   enum e_style
   {
      style_stock,
      style_solid,
      style_linear_gradient,
      
   };
   
   
   int                  m_estyle;
   COLORREF             m_cr;
   int                  m_iStock;
   
   
   simple_brush();
   ~simple_brush();
   
   
   // aim to be all-platoform - but if there is no equivalent because no op : create empty method
   bool create_solid(simple_graphics & g, COLORREF cr);
   bool create_linear_gradient(simple_graphics & g, POINT p1, POINT p2, COLORREF cr1, COLORREF cr2);
   bool destroy();
   
   
   // platform-specific
   bool from_stock(int iId);
   
   
};
