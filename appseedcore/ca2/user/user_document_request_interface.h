#pragma once


namespace user
{

   class CLASS_DECL_ca document_request_interface :
      virtual public ::radix::object
   {
   public:

     
      virtual ::document * open_document_file(::ca::create_context * pcreatecontext);
      virtual ::document * open_document_file(var varFile = gen::g_newconst, bool bMakeVisible = true, ::user::interaction * puiParent = NULL);


   };

} // namespace user


