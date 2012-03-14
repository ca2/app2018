#include "StdAfx.h"

namespace user
{

   keyboard_layout_id::keyboard_layout_id()
   {
   }

   keyboard_layout_id::keyboard_layout_id(const keyboard_layout_id & id)
   {
      operator = (id);
   }

   keyboard_layout_id & keyboard_layout_id::operator = (const keyboard_layout_id & id)
   {
      if(this != &id)
      {
         m_strName   = id.m_strName;
         m_strPath   = id.m_strPath;
         m_hkla      = id.m_hkla;
      }
      return *this;
   }

   bool keyboard_layout_id::operator <= (const keyboard_layout_id & layout) const
   {
      int iCompare = m_strName.CompareNoCase(layout.m_strName);
      if(iCompare <= 0)
         return true;
      else if(iCompare == 0)
      {
         iCompare = m_strPath.CompareNoCase(layout.m_strPath);
         return iCompare <= 0;
      }
      else
         return false;
   }


   bool keyboard_layout_id::operator < (const keyboard_layout_id & layout) const
   {
      int iCompare = m_strName.CompareNoCase(layout.m_strName);
      if(iCompare < 0)
         return true;
      else if(iCompare == 0)
      {
         iCompare = m_strPath.CompareNoCase(layout.m_strPath);
         return iCompare < 0;
      }
      else
         return false;
   }

   bool keyboard_layout_id::operator == (const keyboard_layout_id & layout) const
   {
      int iCompare = m_strName.CompareNoCase(layout.m_strName);
      if(iCompare != 0)
         return false;
      iCompare = m_strPath.CompareNoCase(layout.m_strPath);
      if(iCompare != 0)
         return false;
      return true;
   }

   keyboard_layout::keyboard_layout(::ca::application * papp) :
      ca(papp)
   {
      m_setEscape.m_bKeyCaseInsensitive = false;
   }

      void keyboard_layout::process_escape(::xml::node * pnode, gen::property_set & set)
      {
         set.m_bKeyCaseInsensitive = false;
         for(int i = 0; i < pnode->get_children_count(); i++)
         {
            ::xml::node * pchild = pnode->child_at(i);
            if(pchild->get_name().CompareNoCase("item") == 0)
            {
               string str = pchild->attr("char");
               if(str.has_char())
               {
                  set[str] = pchild->attr("value");
               }
            }
            else if(pchild->get_name().CompareNoCase("escape") == 0)
            {
               process_escape(pchild, set[pnode->attr("value")].propset());
            }
         }
      }

   bool keyboard_layout::load(const char * pszPath)
   {
      int iMap;
      int iChar;
      int iKey;
      int iCode;
      string str = Application.file().as_string(pszPath);
      if(str.is_empty())
         return false;
      ::xml::document doc(get_app());
      if(!doc.load(str))
         return false;
      for(int i = 0; i < doc.get_root()->get_children_count(); i++)
      {
         ::xml::node * pnode = doc.get_root()->child_at(i);
         if(pnode->get_name().CompareNoCase("item") == 0)
         {
            string strKey = pnode->attr("key");
            string strCode = pnode->attr("code");
            string strChar = pnode->attr("char");
            string strValue = pnode->attr("value");
            string strEscape = pnode->attr("escape");
            iMap = 0;
            if(pnode->attr("shift").get_integer() == 1)
            {
               iMap |= 0x80000000;
            }
            if(strChar.has_char())
            {
               iChar = iMap | (int)(unsigned char)(char)(strChar[0]);
               if(strValue.has_char())
               {
                  m_mapChar[iChar] = strValue;
               }
               else
               {
                  m_mapChar[iChar] = "escape=" + strEscape;
               }
            }
            if(strKey.has_char())
            {
               iKey = iMap | (int)(atoi(strKey));
               if(strValue.has_char())
               {
                  m_mapKey[iKey] = strValue;
               }
               else
               {
                  m_mapKey[iKey] = "escape=" + strEscape;
               }
            }
            if(strCode.has_char())
            {
               iCode = iMap | (int)(atoi(strCode));
               if(strValue.has_char())
               {
                  m_mapCode[iCode] = strValue;
               }
               else
               {
                  m_mapCode[iCode] = "escape=" + strEscape;
               }
            }
         }
         else if(pnode->get_name().CompareNoCase("escape") == 0)
         {
            process_escape(pnode, m_setEscape[pnode->attr("value")].propset());
         }
      }

      return true;
   }

   string keyboard_layout::process_key(int iCode, int iKey, int iFlags)
   {
      UNREFERENCED_PARAMETER(iFlags);
      string str;
      if(!m_mapCode.Lookup(iCode, str))
      {
         if(!m_mapKey.Lookup(iKey, str))
         {
            if(!m_mapChar.Lookup(iKey, str))
            {
               str = (char) (iKey & 0xff);
            }
         }
      }
      if(gen::str::begins_eat(str, "escape="))
      {
         return process_escape(str);
      }
      else
      {
         return process_char(str);
      }
   }

   string keyboard_layout::process_char(const char * pszKey)
   {
      string strChar;
      if(m_strEscape.has_char())
      {
         stringa stra;
         stra.explode(";", m_strEscape);
         gen::property_set * pset = &m_setEscape;
         for(int i = 0; i < stra.get_size(); i++)
         {
            if(pset->has_property(stra[i]))
            {
               if((*pset)[stra[i]].get_value().get_type() == var::type_propset)
               {
                  pset = &(*pset)[stra[i]].propset();
               }
               else if(i == stra.get_upper_bound())
               {
                  m_strEscape.Empty();
                  return stra.implode("") + pszKey;
               }
            }
            else
            {
                m_strEscape.Empty();
               return stra.implode("") + pszKey;
            }
         }
         if(pset->has_property(pszKey))
         {
            m_strEscape.Empty();
            return (*pset)[pszKey];
         }
         else
         {
            m_strEscape.Empty();
            return stra.implode("") + pszKey;
         }
      }
      else
      {
         return pszKey;
      }
   }

   string keyboard_layout::process_escape(const char * pszEscape)
   {
      if(m_strEscape.has_char())
      {
         m_strEscape += ";";
         m_strEscape += pszEscape;
      }
      else
      {
         m_strEscape = pszEscape;
      }
      return "";
   }


} // namespace user
