#pragma once


namespace xml
{


   class CLASS_DECL_AURA department :
      public ::aura::department
   {
   public:


      entities       m_entities;
      disp_option *  m_poptionDefault;
      parse_info *   m_pparseinfoDefault;


      department(::aura::application * papp);
      virtual ~department();

      virtual bool init1();

      virtual bool init();



      string special_chars(const char * psz);


   };


} // namespace bas


