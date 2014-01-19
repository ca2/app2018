#include "framework.h"





int64_t MulDiv(int64_t nNumber, int64_t nNumerator, int64_t nDenominator)
{
   return muldiv64(nNumber, nNumerator, nDenominator);
}

bool copy(tagRECTD * prectDest, const tagRECTD * prectSrc)
{
   *prectDest = *prectSrc;
   return TRUE;
}

bool copy(tagRECTD * prectDest, const RECT * prectSrc)
{
   prectDest->left      = prectSrc->left;
   prectDest->top       = prectSrc->top;
   prectDest->right     = prectSrc->right;
   prectDest->bottom    = prectSrc->bottom;
   return TRUE;
}

bool copy(RECT * prectDest, const tagRECTD * prectSrc)
{
   if(is64integer(prectSrc->left) || is64integer(prectSrc->top) || is64integer(prectSrc->right) || is64integer(prectSrc->bottom))
      return FALSE;
   prectDest->left      = (LONG) prectSrc->left;
   prectDest->top       = (LONG) prectSrc->top;
   prectDest->right     = (LONG) prectSrc->right;
   prectDest->bottom    = (LONG) prectSrc->bottom;
   return TRUE;
}

bool copy(tagRECTD * prectDest, const __rect64 * prectSrc)
{
   if(!is_double(prectSrc->left) || !is_double(prectSrc->top) || !is_double(prectSrc->right) || !is_double(prectSrc->bottom))
      return FALSE;
   prectDest->left      = (double) prectSrc->left;
   prectDest->top       = (double) prectSrc->top;
   prectDest->right     = (double) prectSrc->right;
   prectDest->bottom    = (double) prectSrc->bottom;
   return TRUE;
}

bool copy(__rect64 * prectDest, const tagRECTD * prectSrc)
{
   prectDest->left      = (LONG) prectSrc->left;
   prectDest->top       = (LONG) prectSrc->top;
   prectDest->right     = (LONG) prectSrc->right;
   prectDest->bottom    = (LONG) prectSrc->bottom;
   return TRUE;
}
bool is_empty(const tagRECTD * prect)
{
   return
      prect->left   >= prect->right
      && prect->top >= prect->bottom;
}

bool contains(const tagRECTD * prect, POINTD pt)
{
   return
      pt.x >= prect->left && pt.x <= prect->right
      && pt.y >= prect->top && pt.y <= prect->bottom;
}
bool set(tagRECTD * prectDest, double x1, double y1, double x2, double y2)
{
   prectDest->left      = x1;
   prectDest->top       = y1;
   prectDest->right     = x2;
   prectDest->bottom    = y2;
   return TRUE;
}
bool null(tagRECTD * prectDest)
{
   prectDest->left      = 0;
   prectDest->top       = 0;
   prectDest->right     = 0;
   prectDest->bottom    = 0;
   return true;
}
bool is_equal(const tagRECTD * prect1, const tagRECTD * prect2)
{

   return
      prect1->left      == prect2->left &&
      prect1->top       == prect2->top &&
      prect1->right     == prect2->right &&
      prect1->bottom    == prect2->bottom;

}


bool inflate(tagRECTD * prect, double x, double y)
{

   prect->left      -= x;
   prect->top       -= y;
   prect->right     += x;
   prect->bottom    += y;
   return true;

}


bool deflate(tagRECTD * prect, double x, double y)
{

   prect->left      += x;
   prect->top       += y;
   prect->right     -= x;
   prect->bottom    -= y;
   return true;

}


bool offset(tagRECTD * prect, double x, double y)
{

   prect->left      += x;
   prect->top       += y;
   prect->right     += x;
   prect->bottom    += y;
   return true;

}


bool x_intersect_rect(tagRECTD * prect, const tagRECTD * prect1, const tagRECTD * prect2)
{
   prect->left    = max(prect1->left   , prect2->left);
   prect->right   = min(prect1->right  , prect2->right);
   if(prect->right > prect->left)
   {
      return true;
   }
   else
   {
      return false;
   }
}


bool y_intersect_rect(tagRECTD * prect, const tagRECTD * prect1, const tagRECTD * prect2)
{
   prect->top     = max(prect1->top    , prect2->top);
   prect->bottom  = min(prect1->bottom , prect2->bottom);
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


bool intersect(tagRECTD * prect, const tagRECTD * prect1, const tagRECTD * prect2)
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

bool unite(tagRECTD * prect, const tagRECTD * prect1, const tagRECTD * prect2)
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
   prect->left    = min(prect1->left   , prect2->left);
   prect->right   = max(prect1->right  , prect2->right);
   prect->top     = min(prect1->top    , prect2->top);
   prect->bottom  = max(prect1->bottom , prect2->bottom);
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



CLASS_DECL_BASE double width(LPRECTD lpcrect)
{
   return lpcrect->right - lpcrect->left;
}

CLASS_DECL_BASE double height(LPCRECTD lpcrect)
{
   return lpcrect->bottom - lpcrect->top;
}
















bool copy(__rect64 * prectDest, const __rect64 * prectSrc)
{
   *prectDest = *prectSrc;
   return TRUE;
}

bool copy(__rect64 * prectDest, const RECT * prectSrc)
{
   prectDest->left      = prectSrc->left;
   prectDest->top       = prectSrc->top;
   prectDest->right     = prectSrc->right;
   prectDest->bottom    = prectSrc->bottom;
   return TRUE;
}

bool copy(RECT * prectDest, const __rect64 * prectSrc)
{
   if(!is32integer(prectSrc->left) || !is32integer(prectSrc->top) || !is32integer(prectSrc->right) || !is32integer(prectSrc->bottom))
      return FALSE;
   prectDest->left      = (LONG) prectSrc->left;
   prectDest->top       = (LONG) prectSrc->top;
   prectDest->right     = (LONG) prectSrc->right;
   prectDest->bottom    = (LONG) prectSrc->bottom;
   return TRUE;
}
bool is_empty(const __rect64 * prect)
{
   return
      prect->left   >= prect->right
      && prect->top >= prect->bottom;
}


bool contains(const __rect64 * prect, point64 pt)
{

   return
      pt.x >= prect->left && pt.x <= prect->right
      && pt.y >= prect->top && pt.y <= prect->bottom;

}

bool set(__rect64 * prectDest, int64_t x1, int64_t y1, int64_t x2, int64_t y2)
{

   prectDest->left      = x1;
   prectDest->top       = y1;
   prectDest->right     = x2;
   prectDest->bottom    = y2;
   return true;

}


bool null(__rect64 * prectDest)
{

   prectDest->left      = 0;
   prectDest->top       = 0;
   prectDest->right     = 0;
   prectDest->bottom    = 0;
   return true;

}

bool is_equal(const __rect64 * prect1, const __rect64 * prect2)
{
   return
      prect1->left      == prect2->left &&
      prect1->top       == prect2->top &&
      prect1->right     == prect2->right &&
      prect1->bottom    == prect2->bottom;
}

bool inflate(__rect64 * prect, int64_t x, int64_t y)
{

   prect->left      -= x;
   prect->top       -= y;
   prect->right     += x;
   prect->bottom    += y;
   return true;

}

bool deflate(__rect64 * prect, int64_t x, int64_t y)
{

   prect->left      += x;
   prect->top       += y;
   prect->right     -= x;
   prect->bottom    -= y;
   return true;

}


bool offset(__rect64 * prect, int64_t x, int64_t y)
{

   prect->left      += x;
   prect->top       += y;
   prect->right     += x;
   prect->bottom    += y;
   return true;

}

bool x_intersect_rect(__rect64 * prect, const __rect64 * prect1, const __rect64 * prect2)
{

   prect->left    = max(prect1->left   , prect2->left);
   prect->right   = min(prect1->right  , prect2->right);

   if(prect->right > prect->left)
   {
      return true;
   }
   else
   {
      return false;
   }

}


bool y_intersect_rect(__rect64 * prect, const __rect64 * prect1, const __rect64 * prect2)
{
   prect->top     = max(prect1->top    , prect2->top);
   prect->bottom  = min(prect1->bottom , prect2->bottom);
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


bool intersect(__rect64 * prect, const __rect64 * prect1, const __rect64 * prect2)
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


bool unite(__rect64 * prect, const __rect64 * prect1, const __rect64 * prect2)
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
   prect->left    = min(prect1->left   , prect2->left);
   prect->right   = max(prect1->right  , prect2->right);
   prect->top     = min(prect1->top    , prect2->top);
   prect->bottom  = max(prect1->bottom , prect2->bottom);
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







CLASS_DECL_BASE bool deflate(LPRECT prect, LPCRECT lpcrect)
{
   prect->left    += lpcrect->left;
   prect->right   -= lpcrect->right;
   prect->top     += lpcrect->top;
   prect->bottom  -= lpcrect->bottom;
   return true;
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
