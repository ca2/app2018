#include "StdAfx.h"

rect_array::~rect_array()
{

}

void rect_array::offset(int cx, int cy)
{
   for(int i = 0; i < get_size(); i++)
   {
      rect & rect = element_at(i);
      rect.left   += cx;
      rect.top    += cy;
      rect.right  += cx;
      rect.bottom += cy;
   }
}

void rect_array::get_box(LPRECT lprect)
{
   rect rectUnion(0, 0, 0, 0);
   for(int i = 0; i < get_size(); i++)
   {
      rectUnion.unite(rectUnion, element_at(i));
   }

   *lprect = rectUnion;

}

void rect_array::intersect(LPCRECT lpcrect)
{
   for(int i = 0; i < get_size();)
   {
      element_at(i).intersect(element_at(i), lpcrect);
      if(element_at(i).is_null())
      {
         remove_at(i);
      }
      else
      {
         i++;
      }
   }

}

void rect_array::add(LPCRECT lpcrect)
{
   raw_array < rect >::add(rect(*lpcrect));
}
