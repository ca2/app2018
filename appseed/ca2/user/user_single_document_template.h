#pragma once

class CLASS_DECL_ca single_document_template : 
   virtual public document_template
{
public:

   
   document * m_pdocument;


   single_document_template(::ax::application * papp, const char * pszMatter, ::ax::type_info pDocClass, ::ax::type_info pFrameClass, ::ax::type_info pViewClass);

   virtual ~single_document_template();
   virtual void add_document(document * pDoc);
   virtual void remove_document(document * pDoc);
   virtual count get_document_count() const;
   virtual document * get_document(index index = 0) const;
   virtual void request(::ax::create_context * pcreatecontext);
   virtual void set_default_title(document * pdocument);

#ifdef _DEBUG
   virtual void dump(dump_context&) const;
   virtual void assert_valid() const;
#endif //_DEBUG

};
