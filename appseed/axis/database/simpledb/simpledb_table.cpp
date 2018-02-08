﻿#include "framework.h"


namespace simpledb
{


   table::table(base * pdatabase, const char * pszName) :
      object(pdatabase->get_app()),
      m_spfileMeta(pdatabase->get_app()),
      m_spfileFixed(pdatabase->get_app()),
      m_xmldocumentMeta(pdatabase->get_app())
   {


      m_strName = pszName;
      m_pdatabase = pdatabase;



      string strMetaPath;

      strMetaPath = System.dir().install() / "database" / m_pdatabase->getDatabase() / m_strName/ "meta.xml";

      if(!m_spfileMeta->open(strMetaPath, ::file::type_binary | ::file::mode_read_write | ::file::share_exclusive))
         _throw(simple_exception(get_app(), "integer_exception 1"));

      ::file::istream is(m_spfileMeta);

      if(!m_xmldocumentMeta.load(is))
         _throw(simple_exception(get_app(), "integer_exception 1"));

      sp(::xml::node) pfields = m_xmldocumentMeta.get_root()->get_child("fields");

      for(int32_t i = 0; i < pfields->get_children_count(); i++)
      {
         sp(::xml::node) pfield = pfields->child_at(i);
         if(pfield->get_name() != "field")
            continue;
         ::database::field_definition_item item;
         item = *pfield;
         m_fielddefinition.add(item);
      }

      string strFixedPath = m_xmldocumentMeta.get_root()->attr("fixed_path");
      if(strFixedPath.is_empty())
         strFixedPath = strMetaPath = System.dir().install() / "database" / m_pdatabase->getDatabase() /  m_strName/"fixed.txt";

      if(!m_spfileFixed->open(strMetaPath, ::file::mode_create | ::file::mode_no_truncate | ::file::type_binary | ::file::mode_read_write | ::file::share_exclusive |
                              ::file::defer_create_directory))
         _throw(simple_exception(get_app(), "integer_exception 1"));

   }


} // namespace simpledb


