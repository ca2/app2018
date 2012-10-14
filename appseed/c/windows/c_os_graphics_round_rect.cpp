#include "framework.h"
#undef new
#include <gdiplus.h>


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

using namespace Gdiplus;

graphics_round_rect::graphics_round_rect(void)
{

}

graphics_round_rect::~graphics_round_rect(void)
{

}

//=============================================================================
//
// GetRoundRectPath()
//
// Purpose:     Defines a Rounded Rectangle and places it in the GraphicsPath
//
// Parameters:  path.m_ppath		- [out] pointer to GraphicsPath that will recieve the
//									path data
//				r			- [in]	Rect that defines the round rectangle boundaries
//				dia			- [in]	diameter of the rounded corners (2*radius)
//
// Returns:     None
//
void graphics_round_rect::GetRoundRectPath(simple_path & path, const RECT & r, int dia)
{
   // diameter can't exceed width or height
   if(dia > width(r))	dia = width(r);
   if(dia > height(r))	dia = height(r);

   // define a corner
   Rect Corner(r.left, r.top, dia, dia);

   // begin path
   path.m_ppath->Reset();

   // top left
   path.m_ppath->AddArc(Corner, 180, 90);

   // tweak needed for radius of 10 (dia of 20)
   if(dia % 2 == 0)
   {
      Corner.Width += 1;
      Corner.Height += 1;
      //height(r) -=1; //height(r) -= 1;
   }

   // top right
   Corner.X += (width(r) - dia - 1);
   path.m_ppath->AddArc(Corner, 270, 90);

   // bottom right
   Corner.Y += (height(r) - dia - 1);
   path.m_ppath->AddArc(Corner,   0, 90);

   // bottom left
   Corner.X -= (width(r) - dia - 1);
   path.m_ppath->AddArc(Corner,  90, 90);

   // end path
   path.m_ppath->CloseFigure();
}

void graphics_round_rect::GetRoundTopLeft(simple_path & path, const RECT & r, int dia)
{
   // diameter can't exceed width or height
   if(dia > width(r))	dia = width(r);
   if(dia > height(r))	dia = height(r);

   // define a corner
   Rect Corner(r.left, r.top, dia, dia);

   // begin path
   path.m_ppath->Reset();


   // top left
//   path.m_ppath->AddArc(Corner, 180, 90);

   // tweak needed for radius of 10 (dia of 20)
   if(dia % 2 == 0)
   {
      Corner.Width += 1;
      Corner.Height += 1;
      //height(r) -=1; //height(r) -= 1;
   }

   // top right
   Corner.X += (width(r) - dia - 1);
   //path.m_ppath->AddArc(Corner, 270, 90);

   // bottom right
   Corner.Y += (height(r) - dia - 1);
   //path.m_ppath->AddArc(Corner,   0, 90);

   // bottom left
   Corner.X -= (width(r) - dia - 1);
   path.m_ppath->AddArc(Corner,  135, 45);

   path.m_ppath->AddLine(r.left, r.top + height(r) - dia / 2, r.left, r.top + dia / 2);


   Corner.Y -= (height(r) - dia - 1);
   path.m_ppath->AddArc(Corner, 180, 90);

   path.m_ppath->AddLine(r.left + dia / 2, r.top , r.left + width(r) - dia / 2, r.top);

   Corner.X += (width(r) - dia - 1);
   path.m_ppath->AddArc(Corner, 270, 45);


   // end path
   //path.m_ppath->CloseFigure();
}



void graphics_round_rect::GetRoundBottomRight(simple_path & path, const RECT & r, int dia)
{
   // diameter can't exceed width or height
   if(dia > width(r))	dia = width(r);
   if(dia > height(r))	dia = height(r);

   // define a corner
   Rect Corner(r.left, r.top, dia, dia);

   // begin path
   path.m_ppath->Reset();


   // top left
//   path.m_ppath->AddArc(Corner, 180, 90);

   // tweak needed for radius of 10 (dia of 20)
   if(dia % 2 == 0)
   {
      Corner.Width += 1;
      Corner.Height += 1;
      //height(r) -=1; //height(r) -= 1;
   }

   // top right
   Corner.X += (width(r) - dia - 1);
   path.m_ppath->AddArc(Corner, 315, 45);

   path.m_ppath->AddLine(r.left + width(r), r.top + dia / 2, r.left + width(r), r.top + height(r) - dia / 2);

   // bottom right
   Corner.Y += (height(r) - dia - 1);
   path.m_ppath->AddArc(Corner,   0, 90);

   path.m_ppath->AddLine(r.left + dia / 2, r.top + height(r) , r.left + width(r) - dia / 2, r.top + height(r));

   // bottom left
   Corner.X -= (width(r) - dia - 1);
   path.m_ppath->AddArc(Corner,  90, 45);

   


   //Corner.Y -= (height(r) - dia - 1);
   //path.m_ppath->AddArc(Corner, 180, 90);

   

   //Corner.X += (width(r) - dia - 1);
   //path.m_ppath->AddArc(Corner, 275, 45);


   // end path
   //path.m_ppath->CloseFigure();
}

//=============================================================================
//
// DrawRoundRect()
//
// Purpose:     Draws a rounded rectangle with a solid pen
//
// Parameters:  pGraphics	- [in]	pointer to the Graphics device
//				r			- [in]	Rect that defines the round rectangle boundaries
//				color		- [in]	Color value for the brush
//				radius		- [in]  radius of the rounded corner
//				width		- [in]  width of the border
//
// Returns:     None
//
void graphics_round_rect::DrawRoundRect(simple_graphics & g, const RECT & r, COLORREF cr, int radius, int width)
{
   int dia	= 2*radius;

   // set to pixel mode
   int oldPageUnit = g.m_pgraphics->SetPageUnit(UnitPixel);

   // define the pen
   Pen pen(color, 1);
   pen.SetAlignment(PenAlignmentCenter);

   // get the corner path
   GraphicsPath path;

   // get path
   GetRoundRectPath(&path, r, dia);

   // draw the round rect
   g.m_pgraphics->DrawPath(&pen, &path);

   // if width > 1
   for(int i=1; i<width; i++)
   {
      dia++;
      // left stroke
      r.Inflate(-1, 0);

      // get the path
      GetRoundRectPath(&path, r, dia);

      // draw the round rect
      g.m_pgraphics->DrawPath(&pen, &path);

      // up stroke
      r.Inflate(0, -1);

      // get the path
      GetRoundRectPath(&path, r, dia);

      // draw the round rect
      g.m_pgraphics->DrawPath(&pen, &path);
   }

   // restore page unit
   g.m_pgraphics->SetPageUnit((Unit)oldPageUnit);
}

void graphics_round_rect::DrawTopLeft(simple_graphics & g, const RECT & r, COLORREF cr, int radius, int width)
{
   int dia	= 2*radius;

   // set to pixel mode
   int oldPageUnit = g.m_pgraphics->SetPageUnit(UnitPixel);

   // define the pen
   Pen pen(color, 1);
   pen.SetAlignment(PenAlignmentCenter);

   // get the corner path
   GraphicsPath path;

   // get path
   GetRoundTopLeft(&path, r, dia);

   // draw the round rect
   g.m_pgraphics->DrawPath(&pen, &path);

   // if width > 1
   for(int i=1; i<width; i++)
   {
      dia++;
      // left stroke
      r.Inflate(-1, 0);

      // get the path
      GetRoundTopLeft(&path, r, dia);

      // draw the round rect
      g.m_pgraphics->DrawPath(&pen, &path);

      // up stroke
      r.Inflate(0, -1);

      // get the path
      GetRoundTopLeft(&path, r, dia);

      // draw the round rect
      g.m_pgraphics->DrawPath(&pen, &path);
   }

   // restore page unit
   g.m_pgraphics->SetPageUnit((Unit)oldPageUnit);
}

void graphics_round_rect::DrawBottomRight(simple_graphics & g, const RECT & r, COLORREF cr, int radius, int width)
{
   int dia	= 2*radius;

   // set to pixel mode
   int oldPageUnit = g.m_pgraphics->SetPageUnit(UnitPixel);

   // define the pen
   Pen pen(color, 1);
   pen.SetAlignment(PenAlignmentCenter);

   // get the corner path
   GraphicsPath path;

   // get path
   GetRoundBottomRight(&path, r, dia);

   // draw the round rect
   g.m_pgraphics->DrawPath(&pen, &path);

   // if width > 1
   for(int i=1; i<width; i++)
   {
      dia++;
      // left stroke
      r.Inflate(-1, 0);

      // get the path
      GetRoundBottomRight(&path, r, dia);

      // draw the round rect
      g.m_pgraphics->DrawPath(&pen, &path);

      // up stroke
      r.Inflate(0, -1);

      // get the path
      GetRoundBottomRight(&path, r, dia);

      // draw the round rect
      g.m_pgraphics->DrawPath(&pen, &path);
   }

   // restore page unit
   g.m_pgraphics->SetPageUnit((Unit)oldPageUnit);
}

//=============================================================================
//
// FillRoundRect()
//
// Purpose:     Fills a rounded rectangle with a solid brush.  Draws the border
//				first then fills in the rectangle.
//
// Parameters:  pGraphics	- [in]	pointer to the Graphics device
//				r			- [in]	Rect that defines the round rectangle boundaries
//				color		- [in]	Color value for the brush
//				radius		- [in]  radius of the rounded corner
//
// Returns:     None
//
void graphics_round_rect::FillRoundRect(simple_graphics & g, const RECT & r, COLORREF cr, int radius)
{
   SolidBrush sbr(color);
   FillRoundRect(pGraphics, &sbr, r, color, radius);
}

//=============================================================================
//
// FillRoundRect()
//
// Purpose:     Fills a rounded rectangle with a solid brush.  Draws the border
//				first then fills in the rectangle.
//
// Parameters:  pGraphics	- [in]	pointer to the Graphics device
//				pBrush		- [in]  pointer to a Brush
//				r			- [in]	Rect that defines the round rectangle boundaries
//				color		- [in]	Color value for the border (needed in case the
//									brush is a type other than solid)
//				radius		- [in]  radius of the rounded corner
//
// Returns:     None
//
void graphics_round_rect::FillRoundRect(simple_graphics & g, Brush* pBrush, const RECT & r, Color border, int radius)
{
   int dia	= 2*radius;

   // set to pixel mode
   int oldPageUnit = g.m_pgraphics->SetPageUnit(UnitPixel);

   // define the pen
   Pen pen(border, 1);
   pen.SetAlignment(PenAlignmentCenter);

   // get the corner path
   GraphicsPath path;

   // get path
   GetRoundRectPath(&path, r, dia);

   // fill
   g.m_pgraphics->FillPath(pBrush, &path);

   // draw the border last so it will be on top in case the color is different
   g.m_pgraphics->DrawPath(&pen, &path);

   // restore page unit
   g.m_pgraphics->SetPageUnit((Unit)oldPageUnit);
}
