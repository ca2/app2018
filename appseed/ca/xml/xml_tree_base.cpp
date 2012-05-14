#include "framework.h"

namespace xml
{

   tree_base::tree_base(::ca::application * papp) :
      ca(papp),
      m_varexchange(papp)
   {
      m_pschema = NULL;
   }

   tree_base::tree_base(::ca::application * papp, tree_schema * pschema) :
      ca(papp),
      m_varexchange(papp)
   {
      m_pschema = pschema;
   }

   tree_base::~tree_base()
   {

   }

   string tree_base::get_node_name(int iNameIndex)
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