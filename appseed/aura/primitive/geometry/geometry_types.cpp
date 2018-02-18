#include "framework.h"





int64_t MulDiv(int64_t nNumber, int64_t nNumerator, int64_t nDenominator)
{
   return muldiv64(nNumber, nNumerator, nDenominator);
}


bool null(LPRECT prectDest)
{

   prectDest->left      = 0;
   prectDest->top       = 0;
   prectDest->right     = 0;
   prectDest->bottom    = 0;
   return true;

}

bool x_intersect_rect(LPRECT prect,LPCRECT prect1,LPCRECT prect2)
{
   prect->left    = MAX(prect1->left,prect2->left);
   prect->right   = MIN(prect1->right,prect2->right);
   if(prect->right > prect->left)
   {
      return true;
   }
   else
   {
      return false;
   }
}


bool y_intersect_rect(LPRECT prect,LPCRECT prect1,LPCRECT prect2)
{
   prect->top     = MAX(prect1->top,prect2->top);
   prect->bottom  = MIN(prect1->bottom,prect2->bottom);
   if(prect->top < prect->bottom)
   {
      return true;
   }
   else
   {
      null(prect);
      return false;
   }
}

bool x_null_intersect_rect(LPRECT lprect,LPCRECT lpcrect1,LPCRECT lpcrect2)
{
   lprect->left    = MAX(lpcrect1->left,lpcrect2->left);
   lprect->right   = MIN(lpcrect1->right,lpcrect2->right);
   if(lprect->right >= lprect->left)
   {
      return true;
   }
   else
   {
      lprect->left = 0;
      lprect->right = 0;
      return false;
   }
}


bool y_null_intersect_rect(LPRECT lprect,LPCRECT lpcrect1,LPCRECT lpcrect2)
{
   lprect->top     = MAX(lpcrect1->top,lpcrect2->top);
   lprect->bottom  = MIN(lpcrect1->bottom,lpcrect2->bottom);
   if(lprect->top <= lprect->bottom)
   {
      return true;
   }
   else
   {
      lprect->top = 0;
      lprect->bottom= 0;
      return false;
   }

}


CLASS_DECL_AURA bool null_intersect_rect(LPRECT lprect,LPCRECT lpcrect1,LPCRECT lpcrect2)
{
   if(x_null_intersect_rect(lprect,lpcrect1,lpcrect2)
      && y_null_intersect_rect(lprect,lpcrect1,lpcrect2))
   {
      return true;
   }
   else
   {
      null(lprect);
      return false;
   }
}


bool x_left_null_intersect_rect(LPRECT lprect,LPCRECT lpcrect1,LPCRECT lpcrect2)
{
   lprect->left    = MAX(lpcrect1->left,lpcrect2->left);
   lprect->right   = MIN(lpcrect1->right,lpcrect2->right);
   if(lprect->right > lprect->left || (lprect->right == lprect->left && lpcrect1->left == lpcrect2->left))
   {
      return true;
   }
   else
   {
      lprect->left = 0;
      lprect->right = 0;
      return false;
   }
}


bool y_top_null_intersect_rect(LPRECT lprect,LPCRECT lpcrect1,LPCRECT lpcrect2)
{
   lprect->top     = MAX(lpcrect1->top,lpcrect2->top);
   lprect->bottom  = MIN(lpcrect1->bottom,lpcrect2->bottom);
   if(lprect->top < lprect->bottom || (lprect->top == lprect->bottom && lpcrect1->top == lpcrect2->top))
   {
      return true;
   }
   else
   {
      lprect->top = 0;
      lprect->bottom= 0;
      return false;
   }

}


CLASS_DECL_AURA bool top_left_null_intersect_rect(LPRECT lprect,LPCRECT lpcrect1,LPCRECT lpcrect2)
{
   if(x_left_null_intersect_rect(lprect,lpcrect1,lpcrect2)
      && y_top_null_intersect_rect(lprect,lpcrect1,lpcrect2))
   {
      return true;
   }
   else
   {
      null(lprect);
      return false;
   }
}

bool copy(RECTD * prectDest, const RECTD * prectSrc)
{
   *prectDest = *prectSrc;
   return TRUE;
}

bool copy(RECTD * prectDest, const RECT * prectSrc)
{
   prectDest->left      = prectSrc->left;
   prectDest->top       = prectSrc->top;
   prectDest->right     = prectSrc->right;
   prectDest->bottom    = prectSrc->bottom;
   return TRUE;
}

bool copy(RECT * prectDest, const RECTD * prectSrc)
{
   if(!is32integer(prectSrc->left) || !is32integer(prectSrc->top) || !is32integer(prectSrc->right) || !is32integer(prectSrc->bottom))
      return FALSE;
   prectDest->left      = (LONG) prectSrc->left;
   prectDest->top       = (LONG) prectSrc->top;
   prectDest->right     = (LONG) prectSrc->right;
   prectDest->bottom    = (LONG) prectSrc->bottom;
   return TRUE;
}

bool copy(RECTD * prectDest, const RECT64 * prectSrc)
{
   if(!is_double(prectSrc->left) || !is_double(prectSrc->top) || !is_double(prectSrc->right) || !is_double(prectSrc->bottom))
      return FALSE;
   prectDest->left      = (double) prectSrc->left;
   prectDest->top       = (double) prectSrc->top;
   prectDest->right     = (double) prectSrc->right;
   prectDest->bottom    = (double) prectSrc->bottom;
   return TRUE;
}

bool copy(RECT64 * prectDest, const RECTD * prectSrc)
{
   prectDest->left      = (LONG) prectSrc->left;
   prectDest->top       = (LONG) prectSrc->top;
   prectDest->right     = (LONG) prectSrc->right;
   prectDest->bottom    = (LONG) prectSrc->bottom;
   return TRUE;
}
bool is_empty(const RECTD * prect)
{
   return
      prect->left   >= prect->right
      || prect->top >= prect->bottom;
}

bool contains(const RECTD * prect, POINTD pt)
{
   return
      pt.x >= prect->left && pt.x <= prect->right
      && pt.y >= prect->top && pt.y <= prect->bottom;
}
bool set_rect(RECTD * prectDest, double x1, double y1, double x2, double y2)
{
   prectDest->left      = x1;
   prectDest->top       = y1;
   prectDest->right     = x2;
   prectDest->bottom    = y2;
   return TRUE;
}
bool null(RECTD * prectDest)
{
   prectDest->left      = 0;
   prectDest->top       = 0;
   prectDest->right     = 0;
   prectDest->bottom    = 0;
   return true;
}
bool is_equal(const RECTD * prect1, const RECTD * prect2)
{

   return
      prect1->left      == prect2->left &&
      prect1->top       == prect2->top &&
      prect1->right     == prect2->right &&
      prect1->bottom    == prect2->bottom;

}


bool inflate(RECTD * prect, double x, double y)
{

   prect->left      -= x;
   prect->top       -= y;
   prect->right     += x;
   prect->bottom    += y;
   return true;

}


bool deflate(RECTD * prect, double x, double y)
{

   prect->left      += x;
   prect->top       += y;
   prect->right     -= x;
   prect->bottom    -= y;
   return true;

}


bool offset(RECTD * prect, double x, double y)
{

   prect->left      += x;
   prect->top       += y;
   prect->right     += x;
   prect->bottom    += y;
   return true;

}


bool x_intersect_rect(RECTD * prect, const RECTD * prect1, const RECTD * prect2)
{
   prect->left    = MAX(prect1->left   , prect2->left);
   prect->right   = MIN(prect1->right  , prect2->right);
   if(prect->right > prect->left)
   {
      return true;
   }
   else
   {
      return false;
   }
}


bool y_intersect_rect(RECTD * prect, const RECTD * prect1, const RECTD * prect2)
{
   prect->top     = MAX(prect1->top    , prect2->top);
   prect->bottom  = MIN(prect1->bottom , prect2->bottom);
   if(prect->top < prect->bottom)
   {
      return true;
   }
   else
   {
      null(prect);
      return false;
   }
}




bool x_null_intersect_rect(RECTD * prect,const RECTD * prect1,const RECTD * prect2)
{
   prect->left    = MAX(prect1->left,prect2->left);
   prect->right   = MIN(prect1->right,prect2->right);
   if(prect->right >= prect->left)
   {
      return true;
   }
   else
   {
      return false;
   }
}


bool y_null_intersect_rect(RECTD * prect,const RECTD * prect1,const RECTD * prect2)
{
   prect->top     = MAX(prect1->top,prect2->top);
   prect->bottom  = MIN(prect1->bottom,prect2->bottom);
   if(prect->top <= prect->bottom)
   {
      return true;
   }
   else
   {
      null(prect);
      return false;
   }
}


bool intersect(RECTD * prect, const RECTD * prect1, const RECTD * prect2)
{
   if(x_intersect_rect(prect, prect1, prect2)
   && y_intersect_rect(prect, prect1, prect2))
   {
      return true;
   }
   else
   {
      null(prect);
      return false;
   }
}


bool unite(RECTD * prect, const RECTD * prect1, const RECTD * prect2)
{
   if(is_empty(prect1))
   {
      if(is_empty(prect2))
      {
         null(prect);
         return false;
      }
      else
      {
         *prect =* prect2;
         return true;
      }
   }
   else if(is_empty(prect2))
   {
      *prect = *prect1;
      return true;
   }
   prect->left    = MIN(prect1->left   , prect2->left);
   prect->right   = MAX(prect1->right  , prect2->right);
   prect->top     = MIN(prect1->top    , prect2->top);
   prect->bottom  = MAX(prect1->bottom , prect2->bottom);
   if(prect->right > prect->left &&
      prect->bottom > prect->top)
   {
      return true;
   }
   else
   {
      return false;
   }
}



CLASS_DECL_AURA double width(LPCRECTD lpcrect)
{
   return lpcrect->right - lpcrect->left;
}

CLASS_DECL_AURA double height(LPCRECTD lpcrect)
{
   return lpcrect->bottom - lpcrect->top;
}

CLASS_DECL_AURA double width(const RECTD & rect)
{
   return ::width(&rect);
}

CLASS_DECL_AURA double height(const RECTD & rect)
{
   return ::height(&rect);
}















bool copy(RECT64 * prectDest, const RECT64 * prectSrc)
{
   *prectDest = *prectSrc;
   return TRUE;
}

bool copy(RECT64 * prectDest, const RECT * prectSrc)
{
   prectDest->left      = prectSrc->left;
   prectDest->top       = prectSrc->top;
   prectDest->right     = prectSrc->right;
   prectDest->bottom    = prectSrc->bottom;
   return TRUE;
}

bool copy(RECT * prectDest, const RECT64 * prectSrc)
{
   if(!is32integer(prectSrc->left) || !is32integer(prectSrc->top) || !is32integer(prectSrc->right) || !is32integer(prectSrc->bottom))
      return FALSE;
   prectDest->left      = (LONG) prectSrc->left;
   prectDest->top       = (LONG) prectSrc->top;
   prectDest->right     = (LONG) prectSrc->right;
   prectDest->bottom    = (LONG) prectSrc->bottom;
   return TRUE;
}
bool is_empty(const RECT64 * prect)
{
   return
      prect->left   >= prect->right
      || prect->top >= prect->bottom;
}


bool contains(const RECT64 * prect, point64 pt)
{

   return
      pt.x >= prect->left && pt.x <= prect->right
      && pt.y >= prect->top && pt.y <= prect->bottom;

}

bool set_rect(RECT64 * prectDest, int64_t x1, int64_t y1, int64_t x2, int64_t y2)
{

   prectDest->left      = x1;
   prectDest->top       = y1;
   prectDest->right     = x2;
   prectDest->bottom    = y2;
   return true;

}


bool null(RECT64 * prectDest)
{

   prectDest->left      = 0;
   prectDest->top       = 0;
   prectDest->right     = 0;
   prectDest->bottom    = 0;
   return true;

}

bool is_equal(const RECT64 * prect1, const RECT64 * prect2)
{
   return
      prect1->left      == prect2->left &&
      prect1->top       == prect2->top &&
      prect1->right     == prect2->right &&
      prect1->bottom    == prect2->bottom;
}

bool inflate(RECT64 * prect, int64_t x, int64_t y)
{

   prect->left      -= x;
   prect->top       -= y;
   prect->right     += x;
   prect->bottom    += y;
   return true;

}

bool deflate(RECT64 * prect, int64_t x, int64_t y)
{

   prect->left      += x;
   prect->top       += y;
   prect->right     -= x;
   prect->bottom    -= y;
   return true;

}


bool offset(RECT64 * prect, int64_t x, int64_t y)
{

   prect->left      += x;
   prect->top       += y;
   prect->right     += x;
   prect->bottom    += y;
   return true;

}


bool resize(RECT64 * prect,int64_t cx,int64_t cy)
{
   prect->right     = prect->left + cx;
   prect->bottom    = prect->top + cy;
   return true;

}


bool reverse_size(RECT64 * prect,int64_t cx,int64_t cy)
{

   prect->left      = prect->right  - cx;
   prect->top       = prect->bottom - cy;
   return true;

}

bool x_intersect_rect(RECT64 * prect, const RECT64 * prect1, const RECT64 * prect2)
{

   prect->left    = MAX(prect1->left   , prect2->left);
   prect->right   = MIN(prect1->right  , prect2->right);

   if(prect->right > prect->left)
   {
      return true;
   }
   else
   {
      return false;
   }

}


bool y_intersect_rect(RECT64 * prect, const RECT64 * prect1, const RECT64 * prect2)
{
   prect->top     = MAX(prect1->top    , prect2->top);
   prect->bottom  = MIN(prect1->bottom , prect2->bottom);
   if(prect->top < prect->bottom)
   {
      return true;
   }
   else
   {
      null(prect);
      return false;
   }

}


bool intersect(RECT64 * prect, const RECT64 * prect1, const RECT64 * prect2)
{
   if(x_intersect_rect(prect, prect1, prect2)
   && y_intersect_rect(prect, prect1, prect2))
   {
      return true;
   }
   else
   {
      null(prect);
      return false;
   }

}


bool unite(RECT64 * prect, const RECT64 * prect1, const RECT64 * prect2)
{
   if(is_empty(prect1))
   {
      if(is_empty(prect2))
      {
         null(prect);
         return false;
      }
      else
      {
         *prect =* prect2;
         return TRUE;
      }
   }
   else if(is_empty(prect2))
   {
      *prect = *prect1;
      return true;
   }
   prect->left    = MIN(prect1->left   , prect2->left);
   prect->right   = MAX(prect1->right  , prect2->right);
   prect->top     = MIN(prect1->top    , prect2->top);
   prect->bottom  = MAX(prect1->bottom , prect2->bottom);
   if(prect->right > prect->left &&
      prect->bottom > prect->top)
   {
      return true;
   }
   else
   {
      return false;
   }
}







CLASS_DECL_AURA bool deflate(LPRECT prect, LPCRECT lpcrect)
{
   prect->left    += lpcrect->left;
   prect->right   -= lpcrect->right;
   prect->top     += lpcrect->top;
   prect->bottom  -= lpcrect->bottom;
   return true;
}

CLASS_DECL_AURA bool deflate(LPRECT prect,const RECT & rect)
{
   return deflate(prect, &rect);
}


bool polygon_contains(LPPOINT lppt, LPPOINT lpptPolygon, int iCount)
{

   int i, j = iCount - 1;
   int x = lppt->x;
   int y = lppt->y;
   bool oddNodes = false;

   for (i = 0; i < iCount; i++)
   {
      if (((lpptPolygon[i].y < y && lpptPolygon[j].y >= y) || (lpptPolygon[j].y < y && lpptPolygon[i].y >= y)) && (lpptPolygon[i].x <= x || lpptPolygon[j].x <= x))
      {
         oddNodes ^= (lpptPolygon[i].x + (y - lpptPolygon[i].y) / (lpptPolygon[j].y - lpptPolygon[i].y) * (lpptPolygon[j].x - lpptPolygon[i].x) < x);
      }
      j = i;
   }

   return oddNodes;

}


bool polygon_contains(LPPOINT64 lppt,LPPOINT64 lpptPolygon,int iCount)
{

   int i,j = iCount - 1;
   int64_t x = lppt->x;
   int64_t y = lppt->y;
   bool oddNodes = false;

   for(i = 0; i < iCount; i++)
   {
      if(((lpptPolygon[i].y < y && lpptPolygon[j].y >= y) || (lpptPolygon[j].y < y && lpptPolygon[i].y >= y)) && (lpptPolygon[i].x <= x || lpptPolygon[j].x <= x))
      {
         oddNodes ^= (lpptPolygon[i].x + (y - lpptPolygon[i].y) / (lpptPolygon[j].y - lpptPolygon[i].y) * (lpptPolygon[j].x - lpptPolygon[i].x) < x);
      }
      j = i;
   }

   return oddNodes;

}

bool polygon_contains(LPPOINTD lppt,LPPOINTD lpptPolygon,int iCount)
{

   int i,j = iCount - 1;
   double x = lppt->x;
   double y = lppt->y;
   bool oddNodes = false;

   for(i = 0; i < iCount; i++)
   {
      if(((lpptPolygon[i].y < y && lpptPolygon[j].y >= y) || (lpptPolygon[j].y < y && lpptPolygon[i].y >= y)) && (lpptPolygon[i].x <= x || lpptPolygon[j].x <= x))
      {
         oddNodes ^= (lpptPolygon[i].x + (y - lpptPolygon[i].y) / (lpptPolygon[j].y - lpptPolygon[i].y) * (lpptPolygon[j].x - lpptPolygon[i].x) < x);
      }
      j = i;
   }

   return oddNodes;

}

CLASS_DECL_AURA bool copy(POINT64 * lpptDst,const POINT * lpptSrc)
{
   
   lpptDst->x = lpptSrc->x;
   
   lpptDst->y = lpptSrc->y;

   return true;

}


CLASS_DECL_AURA bool copy(LPPOINT lpptDst,const POINT64 * lpptSrc)
{

   lpptDst->x = (LONG)lpptSrc->x;

   lpptDst->y = (LONG)lpptSrc->y;

   return true;


}



#ifdef APPLEOS




void copy(CGRect & rectDst, const RECT & rectSrc)
{
   rectDst.origin.x = rectSrc.left;
   rectDst.origin.y = rectSrc.top;
   rectDst.size.width = width(rectSrc);
   rectDst.size.height = height(rectSrc);

}


void copy(LPRECT lprectDst, const CGRect & rectSrc)
{
   lprectDst->left = rectSrc.origin.x;
   lprectDst->top = rectSrc.origin.y;
   lprectDst->right = rectSrc.origin.x + rectSrc.size.width;
   lprectDst->bottom = rectSrc.origin.y + rectSrc.size.height;
}

void copy(CGRect & rect, LPCRECT lpcrect)
{
   
   rect.origin.x = lpcrect->left;
   rect.origin.y = lpcrect->top;
   rect.size.width = width(lpcrect);
   rect.size.height = height(lpcrect);
   
   
}

#endif


CLASS_DECL_AURA int_bool IsRectEmpty(const RECT & rect)
{

   return IsRectEmpty((LPCRECT)&rect);

}
