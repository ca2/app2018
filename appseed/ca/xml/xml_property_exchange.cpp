#include "framework.h"

namespace xml
{

   property_set_exchange::property_set_exchange(::ca::applicationsp papp) :
      ca(papp)
   {
      m_ppropertyset = ::null();
   }

   property_set_exchange::~property_set_exchange()
   {

   }


   void property_set_exchange::xml_export(class output_tree & xmlot)
   {
      ASSERT(m_ppropertyset != ::null());
      if(m_ppropertyset == ::null())
         return;
      var_exchange varexchange(get_app());
      for(int32_t i = 0; i < m_ppropertyset->m_propertya.get_size(); i++)
      {
         ::ca::property & property = m_ppropertyset->m_propertya[i];
         varexchange.m_pvar = &property.get_value();
         xmlot.export_node(property.name(), varexchange);
      }
   }

   void property_set_exchange::xml_import(class input_tree & xmlit)
   {
      ASSERT(m_ppropertyset != ::null());
      if(m_ppropertyset == ::null())
         return;
      var_exchange varexchange(get_app());
      for(int32_t i = 0; i < m_ppropertyset->m_propertya.get_size(); i++)
      {
         ::ca::property & property = m_ppropertyset->m_propertya[i];
         varexchange.m_pvar = &property.get_value();
         xmlit.import_node(property.name(), varexchange);
      }
   }

} // namespace xml
