#pragma once


namespace userbase
{


   class CLASS_DECL_ca2 main_frame : 
      public simple_frame_window
   {
   public:


      bool           m_bPlacing;
      
      
      main_frame(sp(::ca::application) papp);
      virtual ~main_frame();

      virtual void install_message_handling(::ca::message::dispatch * pinterface);


   #ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
   #endif

      DECL_GEN_SIGNAL(_001OnCreate)

   };


} // namespace userbase


