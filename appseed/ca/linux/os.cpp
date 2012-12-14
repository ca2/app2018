#include "framework.h"
#include <sys/times.h>

DWORD GetTickCount()
{
    tms tm;
    return times(&tm);
}


/*CLASS_DECL__ bool CopyRect(LPRECT prectDest, LPCRECT prectSrc)
{
      prectDest->left = prectSrc->left;
      prectDest->top = prectSrc->top;
      prectDest->right = prectSrc->right;
      prectDest->bottom = prectSrc->bottom;
   return true;
}

CLASS_DECL__ bool PtInRect(LPCRECT prect, POINT point)
{
   return point.x >= prect->left && point.x <= prect->right
   && point.y >= prect->top && point.y <= prect->bottom;

}

CLASS_DECL__ bool SetRect(LPRECT prect, long x1, long y1, long x2, long y2)
{

   prect->left = x1;
   prect->top = y1;

   prect->right = x2;
   prect->bottom = y2;
   return true;
}


CLASS_DECL__ bool SetRectEmpty(LPRECT prect)
{

   SetRect(prect, 0, 0, 0, 0);
   return true;
}


CLASS_DECL__ bool EqualRect(LPCRECT prect1, LPCRECT prect2)
{
      return prect1->left == prect2->left
         &&  prect1->top == prect2->top
         &&  prect1->right == prect2->right
         &&  prect1->bottom == prect2->bottom;
}


CLASS_DECL__ bool InflateRect(LPRECT prect, int x, int y)
{
      prect->left -= x;
      prect->top -= y;
      prect->right += x;
      prect->bottom += y;
      return true;
}

CLASS_DECL__ bool OffsetRect(LPRECT prect, int x, int y)
{
      prect->left += x;
      prect->top += y;
      prect->right += x;
      prect->bottom += y;
      return true;
}

CLASS_DECL__ bool x_intersect_rect(LPRECT prect, LPCRECT prect1, LPCRECT prect2)
{
   prect->left = max(prect1->left, prect2->left);
   prect->right = min(prect1->right, prect2->right);
   return prect->right >= prect->left;
}

CLASS_DECL__ bool y_intersect_rect(LPRECT prect, LPCRECT prect1, LPCRECT prect2)
{
   prect->top = max(prect1->top, prect2->top);
   prect->right = min(prect1->bottom, prect2->bottom);
   return prect->bottom >= prect->top;
}

CLASS_DECL__ bool IntersectRect(LPRECT prect, LPCRECT prect1, LPCRECT prect2)
{
   bool bXIntersects = x_intersect_rect(prect, prect1, prect2);
   bool bYIntersects = y_intersect_rect(prect, prect1, prect2);
   return bXIntersects && bYIntersects;
}


CLASS_DECL__ bool UnionRect(LPRECT prect, LPCRECT prect1, LPCRECT prect2)
{
   prect->left = min(prect1->left, prect2->left);
   prect->top = min(prect1->top, prect2->top);
   prect->right = max(prect1->right, prect2->right);
   prect->bottom = max(prect1->bottom, prect2->bottom);
   return ((prect->right - prect->left) > 0) && ((prect->bottom - prect->top) > 0);
}


CLASS_DECL__ bool SubtractRect(LPRECT prect, LPCRECT prect1, LPCRECT prect2)
{
   RECT rect;
   bool bXIntersects = x_intersect_rect(&rect, prect1, prect2);
   bool bYIntersects = y_intersect_rect(&rect, prect1, prect2);
   if(bXIntersects)
   {
      prect->left    = rect.left;
      prect->right   = rect.right;
   }
   if(bYIntersects)
   {
      prect->top    = rect.top;
      prect->bottom   = rect.bottom;
   }
   return bXIntersects || bYIntersects;
}*/




CLASS_DECL_ca errno_t fopen_s(FILE ** pFile, const char * pFileName, const char * mode)
{
   if(pFile == NULL)
   {
      return EINVAL;
   }

   *pFile = fopen(pFileName, mode);

   if(*pFile == NULL)
   {
      return errno;
   }

   return 0;

}
