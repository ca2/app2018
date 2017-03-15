#pragma once


namespace sockets
{


   class CLASS_DECL_AURA sip_server :
      public thread,
      public sip_axis
   {
   public:
      
      
      string   m_strIp;
      int32_t      m_iPort;


      sip_server(::aura::application * papp);
   

      virtual int32_t run();


   };


} // namespace sockets


