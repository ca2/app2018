#pragma once


namespace xml
{


   class CLASS_DECL_BASE departament :
      public base_departament
   {
   public:

      
      entities       m_entities;
      disp_option *  m_poptionDefault;
      parse_info *   m_pparseinfoDefault;


      departament(base_application * papp);
      virtual ~departament();

      virtual bool initialize1();

      virtual bool initialize();



      string special_chars(const char * psz);


   };


} // namespace core


