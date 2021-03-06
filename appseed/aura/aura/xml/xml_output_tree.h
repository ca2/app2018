#pragma once


namespace xml
{


   class CLASS_DECL_AURA output_tree :
      public tree_base
   {
   public:

      // root xml node - document
      sp(::xml::document)     m_pdocument;
      // current xml node
      sp(node)                m_pnode;


      output_tree(::aura::application * papp, tree_schema * pschema);
      output_tree(::aura::application * papp);
      virtual ~output_tree();


      void set_attr(const char * lpcszName, const char * lpcszValue, bool bOptional = true);
      void set_attr(const char * lpcszName, int64_t iValue, bool bOptional = true);
      void set_bool_attr(const char * lpcszName, bool bValue, bool bOptional = true);
      void set_double_attr(const char * lpcszName, double dValue);
      void set_value(const char * lpcszValue);
      virtual node * export_node(exportable & exportable);
      virtual node * export_node(const char * lpcszName, exportable & exportable);
      virtual node * export_node(const char * lpcszName, var var);

      virtual void SetNodeName(const char * lpcszName);




   };


}   // namespace xml


