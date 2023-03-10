#pragma once


#include "aura/net/netserver/netserver.h"


namespace dynamic_source
{


   class CLASS_DECL_PROGRAMMING httpd_socket :
      virtual public ::netserver::socket
   {
   public:


      int32_t                                      m_iCa2FetchMode;

      string                                       m_strLocale;


      httpd_socket(::sockets::base_socket_handler&);
      ~httpd_socket();
   protected:
      httpd_socket(const httpd_socket& s);


   public:

      virtual void Reset();

   };


} // namespace netnode



