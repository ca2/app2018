#include "framework.h"

























// size
size::size() throw()
   { /* random filled */ }
size::size(__int64 initCX, __int64 initCY) throw()
   { cx = (int) initCX; cy = (int) initCY; }
size::size(SIZE initSize) throw()
   { *(SIZE*)this = initSize; }
size::size(const SIZE * pinitSize) throw()
   { *(SIZE*)this = *pinitSize; }
size::size(__size64  initSize) throw()
   { cx = (int) initSize.cx; cy = (int) initSize.cy; }
size::size(const __size64 *  pinitSize) throw()
   { cx = (int) pinitSize->cx; cy = (int) pinitSize->cy; }
size::size(POINT initPt) throw()
   { *(POINT*)this = initPt; }
size::size(DWORD dwSize) throw()
   {
      cx = (short)LOWORD(dwSize);
      cy = (short)HIWORD(dwSize);
   }

size::operator SIZE *() throw()
   { return this; }
size::operator const SIZE *() const throw()
   { return this; }

bool size::operator==(SIZE size) const throw()
   { return (cx == size.cx && cy == size.cy); }
bool size::operator!=(SIZE size) const throw()
   { return (cx != size.cx || cy != size.cy); }
void size::operator+=(SIZE size) throw()
   { cx += size.cx; cy += size.cy; }
void size::operator-=(SIZE size) throw()
   { cx -= size.cx; cy -= size.cy; }
void size::set_size(int CX, int CY) throw()
   { cx = CX; cy = CY; }
class size size::operator+(SIZE size) const throw()
   {
      class size sizeRet;
      sizeRet.cx = cx + size.cx;
      sizeRet.cy = cy + size.cy;
      return sizeRet;
   }
size size::operator-(SIZE size) const throw()
   {
      class size sizeRet;
      sizeRet.cx = cx - size.cx;
      sizeRet.cy = cy - size.cy;
      return sizeRet;
   }
size size::operator-() const throw()
   { return size(-cx, -cy); }
point size::operator+(POINT point) const throw()
   {
      class point pointRet;
      pointRet.x = cx + point.x;
      pointRet.y = cy + point.y;
      return pointRet;
   }
point size::operator-(POINT point) const throw()
   {
      class point pointRet;
      pointRet.x = cx - point.x;
      pointRet.y = cy - point.y;
      return pointRet;
   }
rect size::operator+(LPCRECT lpRect) const throw()
   { return rect(lpRect) + *this; }
rect size::operator-(LPCRECT lpRect) const throw()
   { return rect(lpRect) - *this; }

__int64 size::area()
   { return cx * cy; }









































// size64
size64::size64() throw()
   { /* random filled */ }
size64::size64(__int64 initCX, __int64 initCY) throw()
   { cx = initCX; cy = initCY; }
size64::size64(__size64 initSize) throw()
   { *(__size64*)this = initSize; }
size64::size64(const __size64 * pinitSize) throw()
   { *(__size64*)this = *pinitSize; }
size64::size64(__point64 initPt) throw()
   { *(__point64*)this = initPt; }
size64::size64(uint64_t dwSize) throw()
   {
      cx = (int)LODWORD(dwSize);
      cy = (int)HIDWORD(dwSize);
   }

size64::operator __size64 *() throw()
   { return this; }
size64::operator const __size64 *() const throw()
   { return this; }


BOOL size64::operator==(__size64 size64) const throw()
   { return (cx == size64.cx && cy == size64.cy); }
BOOL size64::operator!=(__size64 size64) const throw()
   { return (cx != size64.cx || cy != size64.cy); }
void size64::operator+=(__size64 size64) throw()
   { cx += size64.cx; cy += size64.cy; }
void size64::operator-=(__size64 size64) throw()
   { cx -= size64.cx; cy -= size64.cy; }
void size64::set_size(__int64 CX, __int64 CY) throw()
   { cx = CX; cy = CY; }


size64 size64::operator+(__size64 size) const throw()
   { return size64(cx + size.cx, cy + size.cy); }
size64 size64::operator-(__size64 size) const throw()
   { return size64(cx - size.cx, cy - size.cy); }
size64 size64::operator-() const throw()
   { return size64(-cx, -cy); }
point64 size64::operator+(__point64 point) const throw()
   { return point64(cx + point.x, cy + point.y); }
point64 size64::operator-(__point64 point) const throw()
   { return point64(cx - point.x, cy - point.y); }
rect64 size64::operator+(const __rect64 * lpRect) const throw()
   { return rect64(lpRect) + *this; }
rect64 size64::operator-(const __rect64 * lpRect) const throw()
   { return rect64(lpRect) - *this; }

__int64 size64::area()
   { return cx * cy; }





























































// sized
sized::sized() throw()
   { /* random filled */ }
sized::sized(double initCX, double initCY) throw()
   { cx = (int) initCX; cy = (int) initCY; }
sized::sized(SIZED initSize) throw()
   { *(SIZED*)this = initSize; }
sized::sized(const SIZED * pinitSize) throw()
   { *(SIZED*)this = *pinitSize; }
sized::sized(__size64  initSize) throw()
   { cx = (int) initSize.cx; cy = (int) initSize.cy; }
sized::sized(const __size64 *  pinitSize) throw()
   { cx = (int) pinitSize->cx; cy = (int) pinitSize->cy; }
sized::sized(POINT initPt) throw()
   { *(POINT*)this = initPt; }
sized::sized(DWORD dwSize) throw()
   {
      cx = (short)LOWORD(dwSize);
      cy = (short)HIWORD(dwSize);
   }

sized::operator SIZED *() throw()
   { return this; }
sized::operator const SIZED *() const throw()
   { return this; }

bool sized::operator==(SIZED sized) const throw()
   { return (cx == sized.cx && cy == sized.cy); }
bool sized::operator!=(SIZED sized) const throw()
   { return (cx != sized.cx || cy != sized.cy); }
void sized::operator+=(SIZED sized) throw()
   { cx += sized.cx; cy += sized.cy; }
void sized::operator-=(SIZED sized) throw()
   { cx -= sized.cx; cy -= sized.cy; }
void sized::set_size(int CX, int CY) throw()
   { cx = CX; cy = CY; }
class sized sized::operator+(SIZED sized) const throw()
   {
      class sized sizeRet;
      sizeRet.cx = cx + sized.cx;
      sizeRet.cy = cy + sized.cy;
      return sizeRet;
   }
sized sized::operator-(SIZED sized) const throw()
   {
      class sized sizeRet;
      sizeRet.cx = cx - sized.cx;
      sizeRet.cy = cy - sized.cy;
      return sizeRet;
   }
sized sized::operator-() const throw()
   { return sized(-cx, -cy); }
pointd sized::operator+(POINTD pointd) const throw()
   {
      class pointd pointRet;
      pointRet.x = cx + pointd.x;
      pointRet.y = cy + pointd.y;
      return pointRet;
   }
pointd sized::operator-(POINTD pointd) const throw()
   {
      class pointd pointRet;
      pointRet.x = cx - pointd.x;
      pointRet.y = cy - pointd.y;
      return pointRet;
   }
rectd sized::operator+(LPCRECTD lpRect) const throw()
   { return rectd(lpRect) + *this; }
rectd sized::operator-(LPCRECTD lpRect) const throw()
   { return rectd(lpRect) - *this; }

double sized::area()
   { return cx * cy; }
