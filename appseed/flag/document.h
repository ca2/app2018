#pragma once

namespace flag
{

   class CLASS_DECL_CA2_FLAG document :
      public ::userbase::document
   {
   public:


      visual::dib_sp      m_spdib;


      document(::ca::application * papp);
	   virtual ~document();

      
      DECL_GEN_VSIGNAL(data_on_after_change)

	   virtual BOOL on_new_document();
   #ifdef _DEBUG
	   virtual void assert_valid() const;
	   virtual void dump(dump_context & dumpcontext) const;
   #endif

      bool on_open_document(var varFile);

   };

} // namespace flag