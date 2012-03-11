#include "StdAfx.h"


namespace xml
{

   var_exchange::var_exchange(::ca::application * papp) :
      ca(papp)
   {
      m_pvar = NULL;
   }

   var_exchange::~var_exchange()
   {

   }

   void var_exchange::xml_export(class output_tree & xmlot)
   {
      ASSERT(m_pvar != NULL);
      if(m_pvar == NULL)
         return;
      node * pnode = xmlot.m_pnode;
      pnode->add_attr("type", System.get_name(m_pvar->get_type()));
      pnode->set_value(m_pvar->get_string());
   }

   void var_exchange::xml_import(class input_tree & xmlit)
   {
      ASSERT(m_pvar != NULL);
      if(m_pvar == NULL)
         return;
      node * pnode = xmlit.m_pnode;
      if(pnode == NULL)
      {
         // property Node not found;
         //ASSERT(FALSE);
         return;
      }
      string str;
      pnode->get_attr("type", str);
      System.from_name(m_pvar->m_etype, str);
      m_pvar->set_string(pnode->get_value());
   
   }

} // namespace xml