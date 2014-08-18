#include "framework.h"


namespace database
{


   field_definition_item::field_definition_item()
   {
   }

   field_definition_item::field_definition_item(const field_definition_item & item)
   {
      operator = (item);
   }

   field_definition_item & field_definition_item::operator = (const field_definition_item & item)
   {
      if(this != &item)
      {
         m_strName   = item.m_strName;
         m_iSize     = item.m_iSize;
         m_etype     = item.m_etype;
      }
      return *this;
   }

   field_definition_item & field_definition_item::operator = (const xml::node & nodeConst)
   {
      xml::node & node = const_cast < xml::node & > (nodeConst);
      m_strName = node.attr("name");
      if(!node.get_attr("size", m_iSize))
         m_iSize = -1;
      string strType;
      if(node.get_attr("type", strType))
      {
         m_etype = field_str_to_type(strType);
      }
      else
      {
         m_etype = type_none;
      }
      return *this;
   }

   field_definition_item::e_type field_definition_item::field_str_to_type(const char * pszType)
   {
      string strType(pszType);
      strType.make_lower();
      if(strType == "text")
      {
         return field_definition_item::type_text;
      }
      return type_none;
   }


} // namespace database