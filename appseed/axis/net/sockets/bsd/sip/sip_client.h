#pragma once


namespace sockets
{


   class CLASS_DECL_AXIS sip_client :
      virtual public ::object
   {
   public:

      sip_axis_client_socket *   m_psocket;

      void invite(const char * pszTo, const char * pszFrom);

   };


} // namespace sockets



