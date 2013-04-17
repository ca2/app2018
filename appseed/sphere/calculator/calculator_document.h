#pragma once


namespace calculator
{


   class CLASS_DECL_sphere document :
      public ::user::document
   {
   public:


      document(sp(::ca::application) papp);   
      virtual ~document();


      DECL_GEN_VSIGNAL(data_on_after_change);

      virtual bool on_new_document();

   
      
#ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
#endif

      bool on_open_document(var varFile);


   };


} //
