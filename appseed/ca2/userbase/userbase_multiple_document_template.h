#pragma once

namespace userbase
{

   class CLASS_DECL_ca2 multiple_document_template :
      virtual public document_template,
      virtual public ::multiple_document_template
   {
   public:
      multiple_document_template(::ca::application * papp, const char * pszMatter, ::ca::type_info pDocClass, ::ca::type_info pFrameClass, ::ca::type_info pViewClass);
      virtual ~multiple_document_template();

      void reload_template();


   #ifdef _DEBUG
      virtual void dump(dump_context&) const;
      virtual void assert_valid() const;
   #endif


   };

} // namespace userbase

