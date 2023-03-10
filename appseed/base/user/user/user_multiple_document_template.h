#pragma once


namespace user
{


   // MDI support (zero or more documents)
   class CLASS_DECL_BASE multiple_document_template :
      virtual public ::user::impact_system
   {
   public:


      spa(::user::document) m_docptra;          // open documents of this type
      UINT m_nUntitledCount;   // start at 0, for "Document1" title


      multiple_document_template(::aura::application * papp, const char * pszMatter, sp(type) pDocClass, sp(type) pFrameClass, sp(type) pViewClass);
      virtual ~multiple_document_template();


      virtual void load_template();
      virtual void add_document(::user::document * pDoc);
      virtual void remove_document(::user::document * pDoc);
      virtual ::count get_document_count() const;
      virtual ::user::document * get_document(index index) const;
      virtual void request_create(::create * pcreate);
      virtual void set_default_title(::user::document * pdocument);

      virtual void dump(dump_context &) const;
      virtual void assert_valid() const;

      virtual int64_t add_ref()
      {
         return ::user::impact_system::add_ref();
      }

      virtual int64_t dec_ref()
      {
         return ::user::impact_system::dec_ref();
      }

   };


} // namespace user






