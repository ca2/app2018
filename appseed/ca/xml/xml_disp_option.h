#pragma once

namespace xml
{

   // display optional environment
   class CLASS_DECL_ca disp_option
   {
   public:
      bool newline;         // newline when new tag
      bool reference_value;   // do convert from entity to reference ( < -> &lt; )
      char value_quotation_mark;   // val="" (default value quotation mark "
      entities *   m_pentities;   // entity table for entity encode

      int tab_base;         // internal usage
      disp_option(::ca::application * papp);
   };

   //extern CLASS_DECL_ca disp_option g_optDefault;


} // namespace xml