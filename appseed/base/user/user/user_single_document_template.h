#pragma once


namespace user
{


   class CLASS_DECL_BASE single_document_template :
      virtual public ::user::impact_system
   {
   public:


      sp(::user::document) m_pdocument;


      single_document_template(::aura::application * papp, const char * pszMatter, sp(type) pDocClass, sp(type) pFrameClass, sp(type) pViewClass);
      virtual ~single_document_template();

      virtual void add_document(::user::document * pDoc);
      virtual void remove_document(::user::document * pDoc);
      virtual ::count get_document_count() const;
      virtual ::user::document * get_document(index index = 0) const;
      virtual void request_create(::create * pcreate);
      virtual void set_default_title(::user::document * pdocument);

      virtual void dump(dump_context &) const;
      virtual void assert_valid() const;


   };



} // namespace user






