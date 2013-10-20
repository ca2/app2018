#pragma once


namespace user
{


   // MDI support (zero or more documents)
   class CLASS_DECL_CORE multiple_document_template :
      virtual public document_template
   {
   public:


      spa(::user::document_interface) m_docptra;          // open documents of this type
      UINT m_nUntitledCount;   // start at 0, for "Document1" title


      multiple_document_template(sp(base_application) papp, const char * pszMatter, sp(type) pDocClass, sp(type) pFrameClass, sp(type) pViewClass);
      virtual ~multiple_document_template();


      virtual void load_template();
      virtual void add_document(sp(::user::document_interface) pDoc);
      virtual void remove_document(sp(::user::document_interface) pDoc);
      virtual ::count get_document_count() const;
      virtual sp(::user::document_interface) get_document(index index) const;
      virtual void request_create(sp(::create_context) pcreatecontext);
      virtual void set_default_title(sp(::user::document_interface) pdocument);

      virtual void dump(dump_context &) const;
      virtual void assert_valid() const;


      


   };


} // namespace user






