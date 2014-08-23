#pragma once


namespace aura
{


   class CLASS_DECL_AURA application_interface:
      virtual public ::thread,
      virtual public ::aura::live_object,
      virtual public command_target_interface,
      virtual public request_interface,
      virtual public int_scalar_source
   {
   public:


      ::aura::system *                                m_paurasystem;
      ::aura::session *                               m_paurasession;
      :: axis :: application *                        m_paxissapp; // can be used only from axis and upper
      :: axis ::system *                              m_paurasystem; // can be used only from axis and upper
      :: axis ::session *                             m_paxissession; // can be used only from axis and upper
      :: base :: application *                        m_pbasesapp; // can be used only from base and upper
      ::base::system *                                m_pbasesystem; // can be used only from base and upper
      ::base::session *                               m_pbasesession; // can be used only from base and upper
      ::core::application *                           m_pcoreapp; // can be used only from core and upper
      ::core::system *                                m_pcoresystem; // can be used only from core and upper
      ::core::platform *                              m_pcoreplatform; // can be used only from core and upper


      application_interface();
      virtual ~application_interface();




   };


} // namespace aura
























