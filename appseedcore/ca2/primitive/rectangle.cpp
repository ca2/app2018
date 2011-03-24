#include "StdAfx.h"

void rect::DeflateBottomRightSizeByRate(double dRate)
{
    ASSERT(dRate != 0.0);
    double dx = width();
    double dy = height();
    double dxNew = dx / dRate;
    double dyNew = dy / dRate;
    dx = dx - dxNew;
    dy = dy - dyNew;
    right -= (long) dx;
    bottom -= (long) dy;
}

void rect::SetBottomRightSize(int iWidth, int iHeight)
{
    right = left + iWidth;
    bottom = top + iHeight;
}

void rect::ExtendOnCenter(LPCRECT lpcrect)
{
    int cx = width();
    int cy = height();

    ASSERT(cx != 0);
    ASSERT(cy != 0);
    
    double dx = lpcrect->right - lpcrect->left;
    double dy = lpcrect->bottom - lpcrect->top;
    double dr = max(dx / cx, dy / cy);
   
    int cw = (int) (cx * dr);
   int ch = (int) (cy * dr);
   left = (long) ((dx - cw) / 2.0);
   top = (long) ((dy - ch) / 2.0);
    right = left + cw;
    bottom = top + ch;
}


void rect::FitOnCenterOf(LPCRECT lpcrect)
{
    int cx = width();
    int cy = height();

    double dx = lpcrect->right - lpcrect->left;
    double dy = lpcrect->bottom - lpcrect->top;
    double dr = min(cx == 0 ? 1 : dx / cx, cy == 0 ? 1 : dy / cy);
   
    int cw = cx == 0 ? (int) dx : ((int) (cx * dr));
   int ch = cy == 0 ? (int) dy : ((int) (cy * dr));
   left = (long) ((lpcrect->left) + (dx - cw) / 2.0);
   top = (long) ((lpcrect->top) + (dy - ch) / 2.0);
    right = left + cw;
    bottom = top + ch;
}

void rect::ScaleRect(double dx, double dy, int ix, int iy)
{
   left    = (long) (((left    - ix) * dx) + ix);
   top     = (long) (((top     - iy) * dy) + iy);
   right   = (long) (((right   - ix) * dx) + ix);
   bottom  = (long) (((bottom  - iy) * dy) + iy);

}

void rect::ScaleHeightAspect(int iNewHeight, int iCenterX, int iCenterY)
{
   int iHeight = height();
   if(iHeight != 0)
   {
      double d = (double) iNewHeight / iHeight;
      ScaleRect(d, d, iCenterX, iCenterY);
   }
}

void rect::Align(int align, LPCRECT lpcrect)
{
   
   point pt(0, 0);
   
   if((align & gen::AlignH) == gen::AlignHorizontalCenter)
   {
      pt.x = lpcrect->left + (lpcrect->right - lpcrect->left) / 2 - width() / 2 - left;
   }
   else if((align & gen::AlignH) == gen::AlignLeft)
   {
      pt.x = lpcrect->left - left;
   }
   else if((align & gen::AlignH) == gen::AlignRight)
   {
      pt.x = lpcrect->right - right;
   }

   if((align & gen::AlignV) ==  gen::AlignVerticalCenter)
   {
      pt.y = lpcrect->top + (lpcrect->bottom - lpcrect->top) / 2 - height() / 2 - top;
   }
   else if((align & gen::AlignV) == gen::AlignTop)
   {
      pt.y = lpcrect->top - top;
   }
   else if((align & gen::AlignV) == gen::AlignBottom)
   {
      pt.y = lpcrect->bottom - bottom;
   }

   offset(pt);

}

void rect::ConstraintV5(LPCRECT lpcrect, const class size sizeMin)
{
   if(left < lpcrect->left)
      left = lpcrect->left;
   if(right > lpcrect->right)
      right = lpcrect->right;
   if(top < lpcrect->top)
      top = lpcrect->top;
   if(bottom > lpcrect->bottom)
      bottom = lpcrect->bottom;

   if(width() < sizeMin.cx)
      right = left + sizeMin.cx;
   if(height() < sizeMin.cy)
      bottom = top + sizeMin.cy;

}

bool rect::contains(LPCRECT lpcrect) const
{
   return lpcrect->left >= left
      && lpcrect->right <= right
      && lpcrect->top >= top
      && lpcrect->bottom <= bottom;
}

// Subtract minor from major and return the greatest box around.
void rect::SubtractRectMajor(LPCRECT lpcrectMajor, LPCRECT lpcrectMinor)
{
   rect rectIntersect;
   rect rectRet(*this);
   if(!rectIntersect.intersect(lpcrectMajor, lpcrectMinor))
   {
      rectRet = *lpcrectMajor;
   }
   else
   {
      if(rectIntersect == *lpcrectMajor)
      {
         rectRet.set(0, 0, 0, 0);
      }
      else
      {
         if(rectIntersect.left == lpcrectMajor->left
            && rectIntersect.right == lpcrectMajor->right)
         {
            if(rectIntersect.top == lpcrectMajor->top)
            {
               rectRet.set(
                  lpcrectMajor->left, 
                  rectIntersect.top,
                  lpcrectMajor->right,
                  lpcrectMajor->bottom);
            }
            else if(rectIntersect.bottom == lpcrectMajor->bottom)
            {
               rectRet.set(
                  lpcrectMajor->left, 
                  lpcrectMajor->top,
                  lpcrectMajor->right,
                  rectIntersect.bottom);
            }
            else
            {
               rectRet = *lpcrectMajor;
            }
         }
         else if(rectIntersect.top == lpcrectMajor->top
            && rectIntersect.bottom == lpcrectMajor->bottom)
         {
            if(rectIntersect.left == lpcrectMajor->left)
            {
               rectRet.set(
                  rectIntersect.left, 
                  lpcrectMajor->top,
                  lpcrectMajor->right,
                  lpcrectMajor->bottom);
            }
            else if(rectIntersect.right == lpcrectMajor->right)
            {
               rectRet.set(
                  lpcrectMajor->left, 
                  lpcrectMajor->top,
                  rectIntersect.right,
                  lpcrectMajor->bottom);
            }
            else
            {
               rectRet = *lpcrectMajor;
            }
         }
         else
         {
            rectRet = *lpcrectMajor;
         }
      }
   }
   *this = rectRet;
   return;

}

void rect::SubtractRectMinor(LPCRECT lpcrectMajor, LPCRECT lpcrectMinor)
{
   UNREFERENCED_PARAMETER(lpcrectMajor);
   UNREFERENCED_PARAMETER(lpcrectMinor);
   ASSERT(FALSE);
}







void rect64::DeflateBottomRightSizeByRate(double dRate)
{
    ASSERT(dRate != 0.0);
    double dx = (double) width();
    double dy = (double) height();
    double dxNew = dx / dRate;
    double dyNew = dy / dRate;
    dx = dx - dxNew;
    dy = dy - dyNew;
    right -= (long) dx;
    bottom -= (long) dy;
}

void rect64::SetBottomRightSize(__int64 iWidth, __int64 iHeight)
{
    right = left + iWidth;
    bottom = top + iHeight;
}

void rect64::ExtendOnCenter(const __rect64 *  lpcrect)
{
    __int64 cx = width();
    __int64 cy = height();

    ASSERT(cx != 0);
    ASSERT(cy != 0);
    
    double dx = (double) (lpcrect->right - lpcrect->left);
    double dy = (double) (lpcrect->bottom - lpcrect->top);
    double dr = max(dx / cx, dy / cy);
   
    __int64 cw = (__int64) (cx * dr);
   __int64 ch = (__int64) (cy * dr);
   left = (long) ((dx - cw) / 2.0);
   top = (long) ((dy - ch) / 2.0);
    right = left + cw;
    bottom = top + ch;
}


void rect64::FitOnCenterOf(const __rect64 *  lpcrect)
{
    __int64 cx = width();
    __int64 cy = height();

    double dx = (double) (lpcrect->right - lpcrect->left);
    double dy = (double) (lpcrect->bottom - lpcrect->top);
    double dr = min(cx == 0 ? 1 : dx / cx, cy == 0 ? 1 : dy / cy);
   
    __int64 cw = cx == 0 ? (__int64) dx : ((__int64) (cx * dr));
   __int64 ch = cy == 0 ? (__int64) dy : ((__int64) (cy * dr));
   left = (long) ((lpcrect->left) + (dx - cw) / 2.0);
   top = (long) ((lpcrect->top) + (dy - ch) / 2.0);
    right = left + cw;
    bottom = top + ch;
}

void rect64::ScaleRect(double dx, double dy, __int64 ix, __int64 iy)
{
   left    = (long) (((left    - ix) * dx) + ix);
   top     = (long) (((top     - iy) * dy) + iy);
   right   = (long) (((right   - ix) * dx) + ix);
   bottom  = (long) (((bottom  - iy) * dy) + iy);

}

void rect64::ScaleHeightAspect(__int64 iNewHeight, __int64 iCenterX, __int64 iCenterY)
{
   __int64 iHeight = height();
   if(iHeight != 0)
   {
      double d = (double) iNewHeight / iHeight;
      ScaleRect(d, d, iCenterX, iCenterY);
   }
}

void rect64::Align(__int64 align, const __rect64 *  lpcrect)
{
   
   point64 pt(0, 0);
   
   if((align & gen::AlignH) == gen::AlignHorizontalCenter)
   {
      pt.x = lpcrect->left + (lpcrect->right - lpcrect->left) / 2 - width() / 2 - left;
   }
   else if((align & gen::AlignH) == gen::AlignLeft)
   {
      pt.x = lpcrect->left - left;
   }
   else if((align & gen::AlignH) == gen::AlignRight)
   {
      pt.x = lpcrect->right - right;
   }

   if((align & gen::AlignV) ==  gen::AlignVerticalCenter)
   {
      pt.y = lpcrect->top + (lpcrect->bottom - lpcrect->top) / 2 - height() / 2 - top;
   }
   else if((align & gen::AlignV) == gen::AlignTop)
   {
      pt.y = lpcrect->top - top;
   }
   else if((align & gen::AlignV) == gen::AlignBottom)
   {
      pt.y = lpcrect->bottom - bottom;
   }

   offset(pt);

}

void rect64::ConstraintV5(const __rect64 *  lpcrect, const class size sizeMin)
{
   if(left < lpcrect->left)
      left = lpcrect->left;
   if(right > lpcrect->right)
      right = lpcrect->right;
   if(top < lpcrect->top)
      top = lpcrect->top;
   if(bottom > lpcrect->bottom)
      bottom = lpcrect->bottom;

   if(width() < sizeMin.cx)
      right = left + sizeMin.cx;
   if(height() < sizeMin.cy)
      bottom = top + sizeMin.cy;

}

bool rect64::contains(const __rect64 *  lpcrect) const
{
   return lpcrect->left >= left
      && lpcrect->right <= right
      && lpcrect->top >= top
      && lpcrect->bottom <= bottom;
}

// Subtract minor from major and return the greatest box around.
void rect64::SubtractRectMajor(const __rect64 *  lpcrectMajor, const __rect64 *  lpcrectMinor)
{
   rect64 rectIntersect;
   rect64 rectRet(*this);
   if(!rectIntersect.intersect(lpcrectMajor, lpcrectMinor))
   {
      rectRet = *lpcrectMajor;
   }
   else
   {
      if(rectIntersect == *lpcrectMajor)
      {
         rectRet.set(0, 0, 0, 0);
      }
      else
      {
         if(rectIntersect.left == lpcrectMajor->left
            && rectIntersect.right == lpcrectMajor->right)
         {
            if(rectIntersect.top == lpcrectMajor->top)
            {
               rectRet.set(
                  lpcrectMajor->left, 
                  rectIntersect.top,
                  lpcrectMajor->right,
                  lpcrectMajor->bottom);
            }
            else if(rectIntersect.bottom == lpcrectMajor->bottom)
            {
               rectRet.set(
                  lpcrectMajor->left, 
                  lpcrectMajor->top,
                  lpcrectMajor->right,
                  rectIntersect.bottom);
            }
            else
            {
               rectRet = *lpcrectMajor;
            }
         }
         else if(rectIntersect.top == lpcrectMajor->top
            && rectIntersect.bottom == lpcrectMajor->bottom)
         {
            if(rectIntersect.left == lpcrectMajor->left)
            {
               rectRet.set(
                  rectIntersect.left, 
                  lpcrectMajor->top,
                  lpcrectMajor->right,
                  lpcrectMajor->bottom);
            }
            else if(rectIntersect.right == lpcrectMajor->right)
            {
               rectRet.set(
                  lpcrectMajor->left, 
                  lpcrectMajor->top,
                  rectIntersect.right,
                  lpcrectMajor->bottom);
            }
            else
            {
               rectRet = *lpcrectMajor;
            }
         }
         else
         {
            rectRet = *lpcrectMajor;
         }
      }
   }
   *this = rectRet;
   return;

}

void rect64::SubtractRectMinor(const __rect64 *  lpcrectMajor, const __rect64 *  lpcrectMinor)
{
   UNREFERENCED_PARAMETER(lpcrectMajor);
   UNREFERENCED_PARAMETER(lpcrectMinor);
   ASSERT(FALSE);
}



























// rect
rect::rect() throw()
   { /* random filled */ }
rect::rect(__int64 l, __int64 t, __int64 r, __int64 b) throw()
   { left = (int) l; top = (int) t; right = (int) r; bottom = (int) b; }
rect::rect(const RECT& srcRect) throw()
   { ::CopyRect(this, &srcRect); }
rect::rect(const __rect64& srcRect) throw()
   { ::copy(this, &srcRect); }
rect::rect(LPCRECT lpSrcRect) throw()
   { ::CopyRect(this, lpSrcRect); }
rect::rect(POINT point, SIZE size) throw()
   { right = (left = point.x) + size.cx; bottom = (top = point.y) + size.cy; }
rect::rect(POINT topLeft, POINT bottomRight) throw()
   { left = topLeft.x; top = topLeft.y;
      right = bottomRight.x; bottom = bottomRight.y; }
int rect::width() const throw()
   { return right - left; }
int rect::height() const throw()
   { return bottom - top; }
class size rect::size() const throw()
   { return class size(right - left, bottom - top); }
point& rect::top_left() throw()
   { return *((point*)this); }
point& rect::bottom_right() throw()
   { return *((point*)this+1); }
const point& rect::top_left() const throw()
   { return *((point*)this); }
const point& rect::bottom_right() const throw()
   { return *((point*)this+1); }
void rect::swap_left_right() throw()
   { swap_left_right(LPRECT(this)); }
void WINAPI rect::swap_left_right(LPRECT lpRect) throw()
   { LONG temp = lpRect->left; lpRect->left = lpRect->right; lpRect->right = temp; }
rect::operator LPRECT() throw()
   { return this; }
rect::operator LPCRECT() const throw()
   { return this; }
bool rect::contains(POINT point) const throw()
   { return ::PtInRect(this, point) != FALSE; }
void rect::set(int x1, int y1, int x2, int y2) throw()
   { ::SetRect(this, x1, y1, x2, y2); }
void rect::set(POINT topLeft, POINT bottomRight) throw()
   { ::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y); }
void rect::null() throw()
   { ::SetRectEmpty(this); }
void rect::copy(LPCRECT lpSrcRect) throw()
   { ::CopyRect(this, lpSrcRect); }
bool rect::is_equal(LPCRECT lpRect) const throw()
   { return ::EqualRect(this, lpRect) != FALSE; }
void rect::inflate(int x, int y) throw()
   { ::InflateRect(this, x, y); }
void rect::inflate(SIZE size) throw()
   { ::InflateRect(this, size.cx, size.cy); }
void rect::deflate(int x, int y) throw()
   { ::InflateRect(this, -x, -y); }
void rect::deflate(SIZE size) throw()
   { ::InflateRect(this, -size.cx, -size.cy); }
void rect::offset(int x, int y) throw()
   { ::OffsetRect(this, x, y); }
void rect::offset(POINT point) throw()
   { ::OffsetRect(this, point.x, point.y); }
void rect::offset(SIZE size) throw()
   { ::OffsetRect(this, size.cx, size.cy); }
void rect::move_to_y(int y) throw()
   { bottom = height() + y; top = y; }
void rect::move_to_x(int x) throw()
   { right = width() + x; left = x; }
void rect::move_to(int x, int y) throw()
   { move_to_x(x); move_to_y(y); }
void rect::move_to(POINT pt) throw()
   { move_to_x(pt.x); move_to_y(pt.y); }
bool rect::intersect(LPCRECT lpRect1, LPCRECT lpRect2) throw()
   { return ::IntersectRect(this, lpRect1, lpRect2) != FALSE;}
bool rect::unite(LPCRECT lpRect1, LPCRECT lpRect2) throw()
   { return ::UnionRect(this, lpRect1, lpRect2) != FALSE; }
void rect::operator=(const RECT& srcRect) throw()
   { ::CopyRect(this, &srcRect); }
bool rect::operator==(const RECT& rect) const throw()
   { return ::EqualRect(this, &rect) != FALSE; }
bool rect::operator!=(const RECT& rect) const throw()
   { return !::EqualRect(this, &rect) != FALSE; }
void rect::operator+=(POINT point) throw()
   { ::OffsetRect(this, point.x, point.y); }
void rect::operator+=(SIZE size) throw()
   { ::OffsetRect(this, size.cx, size.cy); }
void rect::operator+=(LPCRECT lpRect) throw()
   { inflate(lpRect); }
void rect::operator-=(POINT point) throw()
   { ::OffsetRect(this, -point.x, -point.y); }
void rect::operator-=(SIZE size) throw()
   { ::OffsetRect(this, -size.cx, -size.cy); }
void rect::operator-=(LPCRECT lpRect) throw()
   { deflate(lpRect); }
void rect::operator&=(const RECT& rect) throw()
   { ::IntersectRect(this, this, &rect); }
void rect::operator|=(const RECT& rect) throw()
   { ::UnionRect(this, this, &rect); }
rect rect::operator+(POINT pt) const throw()
   { rect rect(*this); ::OffsetRect(&rect, pt.x, pt.y); return rect; }
rect rect::operator-(POINT pt) const throw()
   { rect rect(*this); ::OffsetRect(&rect, -pt.x, -pt.y); return rect; }
rect rect::operator+(SIZE size) const throw()
   { rect rect(*this); ::OffsetRect(&rect, size.cx, size.cy); return rect; }
rect rect::operator-(SIZE size) const throw()
   { rect rect(*this); ::OffsetRect(&rect, -size.cx, -size.cy); return rect; }
rect rect::operator+(LPCRECT lpRect) const throw()
   { rect rect(this); rect.inflate(lpRect); return rect; }
rect rect::operator-(LPCRECT lpRect) const throw()
   { rect rect(this); rect.deflate(lpRect); return rect; }
rect rect::operator&(const RECT& rect2) const throw()
   { rect rect; ::IntersectRect(&rect, this, &rect2);
      return rect; }
rect rect::operator|(const RECT& rect2) const throw()
   { rect rect; ::UnionRect(&rect, this, &rect2);
      return rect; }
bool rect::subtract(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2) throw()
   { return ::SubtractRect(this, lpRectSrc1, lpRectSrc2) != FALSE; }

void rect::normalize() throw()
   {
      int nTemp;
      if (left > right)
      {
         nTemp = left;
         left = right;
         right = nTemp;
      }
      if (top > bottom)
      {
         nTemp = top;
         top = bottom;
         bottom = nTemp;
      }
   }

void rect::inflate(LPCRECT lpRect) throw()
   {
      left -= lpRect->left;      top -= lpRect->top;
      right += lpRect->right;      bottom += lpRect->bottom;
   }

void rect::inflate(int l, int t, int r, int b) throw()
   {
      left -= l;         top -= t;
      right += r;         bottom += b;
   }

void rect::deflate(LPCRECT lpRect) throw()
   {
      left += lpRect->left;   top += lpRect->top;
      right -= lpRect->right;   bottom -= lpRect->bottom;
   }

void rect::deflate(int l, int t, int r, int b) throw()
   {
      left += l;      top += t;
      right -= r;      bottom -= b;
   }

rect rect::MulDiv(int nMultiplier, int nDivisor) const throw()
   {
      return rect(
         ::MulDiv(left, nMultiplier, nDivisor),
         ::MulDiv(top, nMultiplier, nDivisor),
         ::MulDiv(right, nMultiplier, nDivisor),
         ::MulDiv(bottom, nMultiplier, nDivisor));
   }

__int64 rect::area()
   { return width() * height(); }








































































// rect64
rect64::rect64() throw()
   { /* random filled */ }
rect64::rect64(__int64 l, __int64 t, __int64 r, __int64 b) throw()
   { left = l; top = t; right = r; bottom = b; }
rect64::rect64(const __rect64 & srcRect) throw()
   { ::copy(this, &srcRect); }
rect64::rect64(const RECT & srcRect) throw()
   { ::copy(this, &srcRect); }
rect64::rect64(const __rect64 * lpSrcRect) throw()
   { ::copy(this, lpSrcRect); }
rect64::rect64(const RECT * lpSrcRect) throw()
   { ::copy(this, lpSrcRect); }
rect64::rect64(__point64 point64, __size64 size64) throw()
   { right = (left = point64.x) + size64.cx; bottom = (top = point64.y) + size64.cy; }
rect64::rect64(__point64 topLeft, __point64 bottomRight) throw()
   { left = topLeft.x; top = topLeft.y;
      right = bottomRight.x; bottom = bottomRight.y; }
__int64 rect64::width() const throw()
   { return right - left; }
__int64 rect64::height() const throw()
   { return bottom - top; }
size64 rect64::size() const throw()
   { return size64(right - left, bottom - top); }
point64& rect64::top_left() throw()
   { return *((point64*)this); }
point64& rect64::bottom_right() throw()
   { return *((point64*)this+1); }
const point64& rect64::top_left() const throw()
   { return *((point64*)this); }
const point64& rect64::bottom_right() const throw()
   { return *((point64*)this+1); }
void rect64::swap_left_right() throw()
   { swap_left_right((__rect64 *)(this)); }
void WINAPI rect64::swap_left_right(__rect64 * lpRect) throw()
   { __int64 temp = lpRect->left; lpRect->left = lpRect->right; lpRect->right = temp; }
rect64::operator __rect64 *() throw()
   { return this; }
rect64::operator const __rect64 *() const throw()
   { return this; }
rect64::operator rect() const
{
   return class rect(*this);
}
bool rect64::contains(__point64 point) const throw()
   { return ::contains(this, point); }
void rect64::set(__int64 x1, __int64 y1, __int64 x2, __int64 y2) throw()
   { ::set(this, x1, y1, x2, y2); }
void rect64::set(__point64 topLeft, __point64 bottomRight) throw()
   { ::set(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y); }
void rect64::null() throw()
   { ::null(this); }
void rect64::copy(const __rect64 * lpSrcRect) throw()
   { ::copy(this, lpSrcRect); }
bool rect64::is_equal(const __rect64 * lpRect) const throw()
   { return ::is_equal(this, lpRect); }
void rect64::inflate(__int64 x, __int64 y) throw()
   { ::inflate(this, x, y); }
void rect64::inflate(__size64 size64) throw()
   { ::inflate(this, size64.cx, size64.cy); }
void rect64::deflate(__int64 x, __int64 y) throw()
   { ::inflate(this, -x, -y); }
void rect64::deflate(__size64 size64) throw()
   { ::inflate(this, -size64.cx, -size64.cy); }
void rect64::offset(__int64 x, __int64 y) throw()
   { ::offset(this, x, y); }
void rect64::offset(__point64 point64) throw()
   { ::offset(this, point64.x, point64.y); }
void rect64::offset(__size64 size64) throw()
   { ::offset(this, size64.cx, size64.cy); }
void rect64::move_to_y(__int64 y) throw()
   { bottom = height() + y; top = y; }
void rect64::move_to_x(__int64 x) throw()
   { right = width() + x; left = x; }
void rect64::move_to(__int64 x, __int64 y) throw()
   { move_to_x(x); move_to_y(y); }
void rect64::move_to(__point64 pt) throw()
   { move_to_x(pt.x); move_to_y(pt.y); }
bool rect64::intersect(const __rect64 * lpRect1, const __rect64 * lpRect2) throw()
   { return ::intersect(this, lpRect1, lpRect2);}
bool rect64::unite(const __rect64 * lpRect1, const __rect64 * lpRect2) throw()
   { return ::unite(this, lpRect1, lpRect2); }
void rect64::operator=(const __rect64& srcRect) throw()
   { ::copy(this, &srcRect); }
bool rect64::operator==(const __rect64& rect64) const throw()
   { return ::is_equal(this, &rect64); }
bool rect64::operator!=(const __rect64& rect64) const throw()
   { return !::is_equal(this, &rect64); }
void rect64::operator+=(__point64 point64) throw()
   { ::offset(this, point64.x, point64.y); }
void rect64::operator+=(__size64 size64) throw()
   { ::offset(this, size64.cx, size64.cy); }
void rect64::operator+=(const __rect64 * lpRect) throw()
   { inflate(lpRect); }
void rect64::operator-=(__point64 point64) throw()
   { ::offset(this, -point64.x, -point64.y); }
void rect64::operator-=(__size64 size64) throw()
   { ::offset(this, -size64.cx, -size64.cy); }
void rect64::operator-=(const __rect64 * lpRect) throw()
   { deflate(lpRect); }
void rect64::operator&=(const __rect64& rect64) throw()
   { ::intersect(this, this, &rect64); }
void rect64::operator|=(const __rect64& rect64) throw()
   { ::unite(this, this, &rect64); }
rect64 rect64::operator+(__point64 pt) const throw()
   { rect64 rect64(*this); ::offset(&rect64, pt.x, pt.y); return rect64; }
rect64 rect64::operator-(__point64 pt) const throw()
   { rect64 rect64(*this); ::offset(&rect64, -pt.x, -pt.y); return rect64; }
rect64 rect64::operator+(__size64 size64) const throw()
   { rect64 rect64(*this); ::offset(&rect64, size64.cx, size64.cy); return rect64; }
rect64 rect64::operator-(__size64 size64) const throw()
   { rect64 rect64(*this); ::offset(&rect64, -size64.cx, -size64.cy); return rect64; }
rect64 rect64::operator+(const __rect64 * lpRect) const throw()
   { rect64 rect64(this); rect64.inflate(lpRect); return rect64; }
rect64 rect64::operator-(const __rect64 * lpRect) const throw()
   { rect64 rect64(this); rect64.deflate(lpRect); return rect64; }
rect64 rect64::operator&(const __rect64& rect2) const throw()
   { rect64 rect64; ::intersect(&rect64, this, &rect2);
      return rect64; }
rect64 rect64::operator|(const __rect64& rect2) const throw()
   { rect64 rect64; ::unite(&rect64, this, &rect2);
      return rect64; }
//BOOL rect64::subtract(const __rect64 * lpRectSrc1, const __rect64 * lpRectSrc2) throw()
//   { return ::subtract(this, lpRectSrc1, lpRectSrc2); }

void rect64::normalize() throw()
   {
      __int64 nTemp;
      if (left > right)
      {
         nTemp = left;
         left = right;
         right = nTemp;
      }
      if (top > bottom)
      {
         nTemp = top;
         top = bottom;
         bottom = nTemp;
      }
   }

void rect64::inflate(const __rect64 * lpRect) throw()
   {
      left -= lpRect->left;      top -= lpRect->top;
      right += lpRect->right;      bottom += lpRect->bottom;
   }

void rect64::inflate(__int64 l, __int64 t, __int64 r, __int64 b) throw()
   {
      left -= l;         top -= t;
      right += r;         bottom += b;
   }

void rect64::deflate(const __rect64 * lpRect) throw()
   {
      left += lpRect->left;   top += lpRect->top;
      right -= lpRect->right;   bottom -= lpRect->bottom;
   }

void rect64::deflate(__int64 l, __int64 t, __int64 r, __int64 b) throw()
   {
      left += l;      top += t;
      right -= r;      bottom -= b;
   }

rect64 rect64::MulDiv(__int64 nMultiplier, __int64 nDivisor) const throw()
   {
      return rect64(
         ::MulDiv(left, nMultiplier, nDivisor),
         ::MulDiv(top, nMultiplier, nDivisor),
         ::MulDiv(right, nMultiplier, nDivisor),
         ::MulDiv(bottom, nMultiplier, nDivisor));
   }

__int64 rect64::area()
   { return width() * height(); }
