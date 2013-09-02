#pragma once


inline CLASS_DECL_ca LONG width(LPCRECT lpcrect)
{
   return lpcrect->right - lpcrect->left;
}

inline CLASS_DECL_ca LONG height(LPCRECT lpcrect)
{
   return lpcrect->bottom - lpcrect->top;
}

inline CLASS_DECL_ca LONG width(const RECT & rect)
{
   return width(&rect);
}

inline CLASS_DECL_ca LONG height(const RECT & rect)
{
   return height(&rect);
}

inline CLASS_DECL_ca LONG area(LPCRECT lpcrect)
{
   return width(lpcrect) * height(lpcrect);
}

inline CLASS_DECL_ca LONG area(const RECT & rect)
{
   return width(rect) * height(rect);
}


inline CLASS_DECL_ca RECT & inflate_rect(RECT & rect, LONG dleft, LONG dtop, LONG dright, LONG dbottom)
{
   rect.left   -= dleft;
   rect.right  += dtop;
   rect.top    -= dright;
   rect.top    += dbottom;
   return rect;
}

inline CLASS_DECL_ca LPRECT inflate_rect(LPRECT lprect, LONG dleft, LONG dtop, LONG dright, LONG dbottom)
{
   return &inflate_rect(*lprect, dleft, dtop, dright, dbottom);
}

inline CLASS_DECL_ca RECT & deflate_rect(RECT & rect, LONG dleft, LONG dtop, LONG dright, LONG dbottom)
{
   rect.left   += dleft;
   rect.right  -= dtop;
   rect.top    += dright;
   rect.top    -= dbottom;
   return rect;
}

inline CLASS_DECL_ca LPRECT deflate_rect(LPRECT lprect, LONG dleft, LONG dtop, LONG dright, LONG dbottom)
{
   return &deflate_rect(*lprect, dleft, dtop, dright, dbottom);
}

inline CLASS_DECL_ca RECT & inflate_rect(RECT & rect, LONG dx, LONG dy)
{
   return inflate_rect(rect, dx, dy, dx, dy);
}

inline CLASS_DECL_ca LPRECT inflate_rect(LPRECT lprect, LONG dx, LONG dy)
{
   return &inflate_rect(*lprect, dx, dy);
}

inline CLASS_DECL_ca RECT & deflate_rect(RECT & rect, LONG dx, LONG dy)
{
   return deflate_rect(rect, dx, dy, dx, dy);
}

inline CLASS_DECL_ca LPRECT deflate_rect(LPRECT lprect, LONG dx, LONG dy)
{
   return &deflate_rect(*lprect, dx, dy);
}

inline CLASS_DECL_ca RECT & inflate_rect(RECT & rect, LONG d)
{
   return inflate_rect(rect, d, d);
}

inline CLASS_DECL_ca LPRECT inflate_rect(LPRECT lprect, LONG d)
{
   return &inflate_rect(*lprect, d);
}

inline CLASS_DECL_ca RECT & deflate_rect(RECT & rect, LONG d)
{
   return deflate_rect(rect, d, d);
}

inline CLASS_DECL_ca LPRECT deflate_rect(LPRECT lprect, LONG d)
{
   return &deflate_rect(*lprect, d);
}

inline CLASS_DECL_ca RECT & rect_by_dimension(RECT & rect, LONG x, LONG y, LONG cx, LONG cy)
{
   rect.left      = x;
   rect.top       = y;
   rect.right     = x + cx;
   rect.bottom    = y + cy;
   return rect;
}

inline CLASS_DECL_ca RECT & rect_dim(RECT & rect, LONG x, LONG y, LONG cx, LONG cy)
{
   return rect_by_dimension(rect, x, y, cx, cy);
}

inline CLASS_DECL_ca LPRECT rect_by_dimension(LPRECT lprect, LONG x, LONG y, LONG cx, LONG cy)
{
   return &rect_by_dimension(*lprect, x, y, cx, cy);
}

inline CLASS_DECL_ca LPRECT rect_dim(LPRECT lprect, LONG x, LONG y, LONG cx, LONG cy)
{
   return rect_by_dimension(lprect, x, y, cx, cy);
}

inline CLASS_DECL_ca RECT rect_by_dimension(LONG x, LONG y, LONG cx, LONG cy)
{
   RECT r;
   return rect_by_dimension(r, x, y, cx, cy);
}

inline CLASS_DECL_ca RECT rect_dim(LONG x, LONG y, LONG cx, LONG cy)
{
   return rect_by_dimension(x, y, cx, cy);
}


inline CLASS_DECL_ca POINT & point_by_coordinate(POINT & p, LONG x, LONG y)
{
   p.x = x;
   p.y = y;
   return p;
}

inline CLASS_DECL_ca POINT & point_coord(POINT & p, LONG x, LONG y)
{
   return point_by_coordinate(p, x, y);
}


inline CLASS_DECL_ca LPPOINT point_by_coordinate(LPPOINT lppt, LONG x, LONG y)
{
   return &point_by_coordinate(*lppt, x, y);
}

inline CLASS_DECL_ca LPPOINT point_coord(LPPOINT lppt, LONG x, LONG y)
{
   return point_by_coordinate(lppt, x, y);
}

inline CLASS_DECL_ca POINT point_by_coordinate(LONG x, LONG y)
{
   POINT p;
   return point_by_coordinate(p, x, y);
}

inline CLASS_DECL_ca POINT point_coord(LONG x, LONG y)
{
   return point_by_coordinate(x, y);
}

inline CLASS_DECL_ca POINT & offset(POINT & pt, LONG x, LONG y)
{
   pt.x += x;
   pt.y += y;
   return pt;
}

CLASS_DECL_ca bool polygon_contains(LPPOINT lppt, LPPOINT lpptPolygon, int iCount);
