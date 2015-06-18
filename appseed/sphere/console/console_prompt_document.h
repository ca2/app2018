#pragma once


namespace console
{


   class CLASS_DECL_SPHERE prompt_document :
      public ::user::document
   {
   public:


      prompt_document(::aura::application * papp);
      virtual ~prompt_document();

   
      DECL_GEN_SIGNAL(data_on_after_change);


	   virtual bool on_new_document();

   #ifdef DEBUG
	   virtual void assert_valid() const;
	   virtual void dump(dump_context & dumpcontext) const;
   #endif


      bool on_open_document(var varFile);


   };


} // namespace console



