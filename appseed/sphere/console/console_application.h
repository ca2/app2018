#pragma once


namespace console
{


   class CLASS_DECL_SPHERE application:
      virtual public ::userstack::application
   {
   public:


      sp(prompt) m_spprompt;


      application();
      virtual ~application();

      virtual bool initialize_application();


      inline prompt & console_prompt() { return *m_spprompt; }


      void pre_translate_message(::message::message * pobj);


      DECL_GEN_SIGNAL(_001OnImpact);


   };


} // namespace userstack



