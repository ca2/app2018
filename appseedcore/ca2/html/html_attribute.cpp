#include "StdAfx.h"


namespace html
{
   void attribute::set_name(const char * pszName)
   {
      m_strName = pszName;
   }

   string attribute::get_name()
   {
      return m_strName;
   }

   void attribute::set_value(const char * pszValue)
   {
      m_strValue = pszValue;
   }

   string attribute::get_value()
   {
      return m_strValue;
   }



   // attribute_array

   attribute * attribute_array::get(const char * pszName)
   {
      for(int i = 0; i < get_size(); i++)
      {
         if(element_at(i).get_name().CompareNoCase(pszName) == 0)
            return &element_at(i);
      }
      return NULL;
   }
   
   string attribute_array::get_value(const char * pszName)
   {
      attribute * pattr = get(pszName);
      if(pattr == NULL)
         return "";
      else
         return pattr->get_value();
   }


} // namespace html