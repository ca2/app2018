#pragma once


namespace xml
{

   
   class CLASS_DECL_AXIS tree_base :
      virtual public ::object
   {
   public:


      var_exchange   m_varexchange;


      //The schema of the tree
      tree_schema * m_pschema;


      tree_base(sp(::axis::application) papp, tree_schema * pschema);
      tree_base(sp(::axis::application) papp);
      virtual ~tree_base();

      string get_node_name(int32_t iNameIndex);

      void set_schema(tree_schema * pschema);
      tree_schema & get_schema();

   
   };

   
} // namespace xml



