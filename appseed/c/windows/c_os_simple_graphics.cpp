#include "framework.h"
#undef new
#include <gdiplus.h>


simple_graphics::simple_graphics()
{

   m_pgraphics    = NULL;

   m_hwnd         = NULL;

   m_iType        = 0;

   m_pbitmap      = NULL;

   m_pbrush       = NULL;

   m_pfont        = NULL;

   m_ppen         = NULL;

   m_hdc          = NULL;

}


simple_graphics::~simple_graphics()
{

   if(m_pgraphics != NULL && m_iType != 0)
   {

      destroy();

   }

}

bool simple_graphics::create(HDC hdc)
{

   if(m_iType != 0)
      destroy();
   
   m_pgraphics = new Gdiplus::Graphics(hdc);

   if(m_pgraphics == NULL)
      return false;
   
   m_iType = 1;
   
   return true;

}

bool simple_graphics::create_from_bitmap(simple_bitmap & b)
{

   if(m_iType != 0)
      destroy();
   
   m_pgraphics = new ::Gdiplus::Graphics(b.m_pbitmap);

   if(m_pgraphics == NULL)
      return false;

   m_iType = 3;
   
   return true;

}

bool simple_graphics::from_entire_window(HWND hwnd)
{

   if(m_iType != 0)
      destroy();

   m_hdc = ::GetWindowDC(hwnd);

   if(m_hdc == NULL)
      return false;

   m_pgraphics = new Gdiplus::Graphics(m_hdc);

   if(m_pgraphics == NULL)
   {
      ::ReleaseDC(hwnd, m_hdc);
      m_hdc = NULL;
      return false;
   }

   m_hwnd = hwnd;

   m_iType = 2;
   
   return true;

}


bool simple_graphics::from_window(HWND hwnd)
{

   if(m_iType != 0)
      destroy();

   m_hdc = ::GetDC(hwnd);

   if(m_hdc == NULL)
      return false;

   m_pgraphics = new Gdiplus::Graphics(m_hdc);

   if(m_pgraphics == NULL)
   {
      ::ReleaseDC(hwnd, m_hdc);
      m_hdc = NULL;
      return false;
   }

   m_hwnd = hwnd;

   m_iType = 2;
   
   return true;

}

bool simple_graphics::from_window_paint(HWND hwnd, LPRECT lprectPaint)
{


   if(m_iType != 0)
      destroy();

   m_hdc = BeginPaint(hwnd, &m_ps);

   if(m_hdc == NULL)
      return false;

   m_pgraphics = new Gdiplus::Graphics(m_hdc);

   if(m_pgraphics == NULL)
   {
      ::EndPaint(hwnd, &m_ps);
      memset(&m_ps, 0, sizeof(m_ps));
      m_hdc = NULL;
      return false;
   }

   m_hwnd = hwnd;

   m_iType = 4;

   if(lprectPaint != NULL)
   {
    
      *lprectPaint = m_ps.rcPaint;

   }

   
   return true;


}

bool simple_graphics::reference_os_data(Gdiplus::Graphics * pgraphics)
{
   
   m_iType = 5;

   m_pgraphics = pgraphics;

   return true;

}


bool simple_graphics::create()
{

   simple_graphics g;

   g.from_window(NULL);
   
   return create(g.m_hdc);
   
}

bool simple_graphics::create_from_screen()
{
   
   return from_window(NULL);
   
}

bool simple_graphics::select(simple_font & font)
{
   
   m_pfont = &font;

   return true;

}

bool simple_graphics::select(simple_brush & brush)
{
   
   m_pbrush = &brush;

   return true;

}

bool simple_graphics::select(simple_pen & pen)
{
   
   m_ppen = &pen;

   return true;

}

bool simple_graphics::destroy()
{
   
   bool bOk = true;

   if(m_pgraphics != NULL && m_iType != 5)
   {
      
      try
      {
         
         delete m_pgraphics;

      }
      catch(...)
      {

         bOk = false;

      }
      
   }

   m_pgraphics = NULL;


   if(m_iType == 2)
   {

      try
      {

         if(!::ReleaseDC(m_hwnd, m_hdc))
         {
         
            bOk = false;

         }

      }
      catch(...)
      {

         bOk = false;

      }

   }
   else if(m_iType == 4)
   {

      try
      {

         if(!EndPaint(m_hwnd, &m_ps))
         {

            bOk = false;
         }

      }
      catch(...)
      {

         bOk = false;

      }

   }


   m_hwnd   = NULL;

   m_hdc    = NULL;

   m_iType  = 0;

   return bOk;

}


POINT simple_graphics::get_offset()
{

   Gdiplus::Matrix m;

   m_pgraphics->GetTransform(&m);

   Gdiplus::PointF origin(0, 0);

   m.TransformPoints(&origin);

   return point_coord((int64_t) origin.X, (int64_t) origin.Y);
}



bool simple_graphics::set_offset(int x, int y)
{
   
   Gdiplus::Matrix m;

   m.Translate((Gdiplus::REAL) x, (Gdiplus::REAL) y);

   return m_pgraphics->SetTransform(&m) == Gdiplus::Ok;

}

bool simple_graphics::offset(int x, int y)
{

   POINT pt = get_offset();

   ::offset(pt, x, y);

   return set_offset(pt.x, pt.y);

}


bool simple_graphics::bit_blt(int x, int y, int cx, int cy, simple_graphics & gSrc, int x1, int y1, DWORD rop)
{
   
   return m_pgraphics->DrawImage(gSrc.m_pbitmap->m_pbitmap, x, y, x1, y1, cx, cy, Gdiplus::UnitPixel) != FALSE;

}

bool simple_graphics::blend_bitmap_data(int x, int y, int cx, int cy, COLORREF * pdata)
{
   
   try
   {
   
      Gdiplus::Bitmap b(cx, cy, cx *4 , PixelFormat32bppARGB, (BYTE *) pdata);

      set_alpha_mode(::ca::alpha_mode_blend);

      m_pgraphics->SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);

      m_pgraphics->DrawImage(&b, x, y, 0, 0, cx, cy, Gdiplus::UnitPixel);

      m_pgraphics->Flush(Gdiplus::FlushIntentionSync);

      return true;

   }
   catch(...)
   {

   }

   return false;

}

SIZE simple_graphics::get_text_extent(const char * psz, int iLen)
{

   if(iLen < 0)
      iLen = strlen(psz);

   vsstring str(psz, iLen);

   wstring wstr(str);

   Gdiplus::RectF box;

   Gdiplus::PointF origin(0, 0);

   Gdiplus::StringFormat strFormat(Gdiplus::StringFormat::GenericTypographic());

   strFormat.SetFormatFlags(strFormat.GetFormatFlags() 
                           | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                           | Gdiplus::StringFormatFlagsLineLimit | Gdiplus::StringFormatFlagsNoWrap);

   m_pgraphics->MeasureString(wstr, (int) wstr.get_length(), m_pfont->m_pfont, origin, &strFormat,  &box);

   SIZE size;

   size.cx = box.Width;
   size.cy = box.Height;

   return size;

}


bool simple_graphics::draw_line(int x1, int y1, int x2, int y2, simple_pen & pen)
{

   return m_pgraphics->DrawLine(pen.m_ppen, Gdiplus::Point(x1, y1), Gdiplus::Point(x2, y2)) == Gdiplus::Ok;

}


bool simple_graphics::rectangle(LPCRECT lpcrect)
{
   
   bool bOk1 = fill_rect(*lpcrect);

   bool bOk2 = draw_rect(*lpcrect);

   return bOk1 && bOk2;

}

bool simple_graphics::draw_rect(LPCRECT lpcrect, simple_pen & pen)
{

   if(pen.m_ppen == NULL)
      return true;
   
   return m_pgraphics->DrawRectangle(pen.m_ppen, lpcrect->left, lpcrect->top, width(lpcrect), height(lpcrect)) == Gdiplus::Ok;

}


bool simple_graphics::fill_rect(LPCRECT lpcrect, simple_brush & brush)
{
   
   if(brush.m_pbrush == NULL)
      return true;
   
   return m_pgraphics->FillRectangle(brush.m_pbrush, lpcrect->left, lpcrect->top, width(lpcrect), height(lpcrect)) == Gdiplus::Ok;

}

bool simple_graphics::set_text_color(COLORREF cr)
{
   
   return ::SetTextColor(m_hdc, cr) != FALSE;

}

bool simple_graphics::set_alpha_mode(::ca::e_alpha_mode emode)
{

   switch(emode)
   {
   case ::ca::alpha_mode_blend:

      return m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver) == Gdiplus::Ok;

   case ::ca::alpha_mode_set:

      return m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceCopy) == Gdiplus::Ok;

   };

   return false;

}

bool simple_graphics::replace_clip(simple_path & path)
{

   return m_pgraphics->SetClip(path.m_ppath, Gdiplus::CombineModeReplace) == Gdiplus::Ok;

}

bool simple_graphics::exclude_clip(simple_path & path)
{

   return m_pgraphics->SetClip(path.m_ppath, Gdiplus::CombineModeExclude) == Gdiplus::Ok;

}

bool simple_graphics::replace_clip(const RECT & r)
{

   Gdiplus::Rect rect;

   rect.X = r.left;
   rect.Y = r.top;
   rect.Width = width(r);
   rect.Height = height(r);

   return m_pgraphics->SetClip(rect, Gdiplus::CombineModeReplace) == Gdiplus::Ok;

}

bool simple_graphics::alpha_blend(int x, int y, int cx, int cy, simple_graphics & gSrc, int x1, int y1, int cx1, int cy1, BLENDFUNCTION bf)
{

   return ::AlphaBlend(m_hdc, x, y, cx, cy, gSrc.m_hdc, x1, y1, cx1, cy1, bf) != FALSE;

}

void simple_graphics::fill_solid_rect(LPCRECT lpRect, COLORREF clr)
{
   m_pgraphics->FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color(GetAValue(clr), GetRValue(clr), GetGValue(clr), GetBValue(clr))), lpRect->left, lpRect->top, width(lpRect), height(lpRect));
}

bool simple_graphics::text_out(int x, int y, const char * pszUtf8, int iSize)
{
   
   wstring wstr(pszUtf8);

   Gdiplus::StringFormat strFormat(Gdiplus::StringFormat::GenericTypographic());

   strFormat.SetFormatFlags(strFormat.GetFormatFlags() 
                           | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                           | Gdiplus::StringFormatFlagsLineLimit | Gdiplus::StringFormatFlagsNoWrap);

   return m_pgraphics->DrawString(wstr, wstr.get_length(), m_pfont->m_pfont, Gdiplus::PointF(x, y), &strFormat, m_pbrush->m_pbrush) == Gdiplus::Ok;

}


bool simple_graphics::fill_polygon(POINT * p, int iCount, ::ca::e_fill_mode)
{

   if(m_pbrush == NULL)
      return true;

   Gdiplus::Point * ppa = new Gdiplus::Point[iCount];

   for(int i = 0; i < iCount; i++)
   {
      ppa[i].X = p[i].x;
      ppa[i].Y = p[i].y;
   }

   bool bOk = m_pgraphics->FillPolygon(m_pbrush->m_pbrush, ppa, 4, Gdiplus::FillModeWinding) == Gdiplus::Ok;

   delete ppa;

   return bOk;


}



bool simple_graphics::draw_path(simple_path & path, simple_pen & pen)
{

   return m_pgraphics->DrawPath(pen.m_ppen, path.m_ppath) == Gdiplus::Ok;

}

bool simple_graphics::fill_path(simple_path & path, simple_brush & brush)
{
   
   return m_pgraphics->FillPath(brush.m_pbrush, path.m_ppath) == Gdiplus::Ok;

}