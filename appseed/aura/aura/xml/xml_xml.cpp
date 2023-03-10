#include "framework.h"



namespace xml
{


   department::department(::aura::application * papp) :
      object(papp),
      ::aura::department(papp)
   {

      m_poptionDefault     = NULL;
      m_pparseinfoDefault  = NULL;


   }


   department::~department()
   {

      ::aura::del(m_poptionDefault);

      ::aura::del(m_pparseinfoDefault);

   }


   bool department::init1()
   {

      if(Application.is_system())
      {
         System.factory().cloneable_large < edit_item > ();
      }

      if(!::aura::department::init1())
         return false;

      m_poptionDefault     = new disp_option(get_app());
      m_pparseinfoDefault  = new parse_info(get_app());

      m_entities.add_entity('&', "&amp;" );
      m_entities.add_entity('\"', "&quot;");
      m_entities.add_entity('\'', "&apos;");
      m_entities.add_entity('<', "&lt;");
      m_entities.add_entity('>', "&gt;");

      return true;

   }


   bool department::init()
   {

      if(!::aura::department::init())
         return false;

      return true;

   }


   string department::special_chars(const char * psz)
   {

      string str(psz);

      str.replace("&", "&amp;");
      str.replace("\"", "&quot;");      // quando ENT_NOQUOTES n縊 est・definida.
      str.replace("'", "&#039;");       // apenas quando ENT_QUOTES est・definida.
      str.replace("<", "&lt;");
      str.replace(">", "&gt;");

      return str;

   }


} //namespace xml



CLASS_DECL_AURA void xml_export(string & strXml, xml::exportable & exportable)
{

   ::xml::output_tree o(exportable.get_app());

   o.m_pdocument = canew(::xml::document(exportable.get_app()));

   o.m_pnode = o.m_pdocument->get_root();

   exportable.xml_export(o);

   strXml = o.m_pdocument->get_xml();

}


CLASS_DECL_AURA void xml_import(xml::importable & importable, string & strXml)
{

   ::xml::input_tree i(importable.get_app());

   i.m_pdocument = canew(xml::document(importable.get_app()));

   if (i.m_pdocument->load(strXml))
   {

      i.m_pnode = i.m_pdocument->get_root();

      importable.xml_import(i);

   }

}

