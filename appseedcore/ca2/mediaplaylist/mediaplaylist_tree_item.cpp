#include "StdAfx.h"


namespace mediaplaylist
{


   string tree_item::get_text(ex1::tree * ptree)
   {
      UNREFERENCED_PARAMETER(ptree);
      return m_strName;
   }

   index tree_item::get_image(ex1::tree * ptree)
   {
      if(ptree->is_selected(this))
      {
         return -1;
      }
      else
      {
         return -1;
      }

   }



} // namespace mediaplaylist