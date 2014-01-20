#include "framework.h"


namespace net
{


   email_departament::email_departament(sp(base_application) papp) :
      element(papp),
      base_departament(papp)
   {

   }


   bool email_departament::utf8_mail(class ::net::email & email)
   {

      ::sockets::socket_handler handler(get_app());

      ::sockets::smtp_socket socket(handler);

      if(!socket.open("mail.ca2.cc", 25))
         return false;

      socket.m_estate = ::sockets::smtp_socket::state_initial;

      socket.m_email = email;

      uint32_t dwEnd = ::get_tick_count() + 1000 * 120;

      handler.add(&socket);

      while (true)
      {

         handler.select(8,0);

         if(::get_tick_count() > dwEnd)
            break;

         if(socket.m_estate == ::sockets::smtp_socket::state_end)
            break;

      }

      return socket.m_estate == ::sockets::smtp_socket::state_sent || socket.m_estate == ::sockets::smtp_socket::state_quit || socket.m_estate == ::sockets::smtp_socket::state_end;
   }


   bool email_departament::syntax_is_valid(const char * pszCandidate)
   {

      string strCandidate(pszCandidate);

      strCandidate.trim();

      index iFind = strCandidate.find("@");

      if(iFind <= 0)
         return false;

      ::count iLen = strCandidate.get_length();

      if(iFind == iLen - 1)
         return false;

      return true;

   }


} // namespace core




