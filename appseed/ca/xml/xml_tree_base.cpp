#include "framework.h"

namespace xml
{

   tree_base::tree_base(sp(::ca::application) papp) :
      ca(papp),
      m_varexchange(papp)
   {
      m_pschema = ::null();
   }

   tree_base::tree_base(sp(::ca::application) papp, tree_schema * pschema) :
      ca(papp),
      m_varexchange(papp)
   {
      m_pschema = pschema;
   }

   tree_base::~tree_base()
   {

   }

   string tree_base::get_node_name(int32_t iNameIndex)
   {
      return m_pschema->get_node_name(iNameIndex);
   }

   tree_schema & tree_base::get_schema()
   {
      return *m_pschema;
   }

   void tree_base::set_schema(tree_schema * pschema)
   {
      m_pschema = pschema;
   }

} // namespace xml