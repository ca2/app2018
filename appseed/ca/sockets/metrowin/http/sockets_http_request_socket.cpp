/** \file http_request_socket.cpp
 **   \date  2004-02-13
 **   \author grymse@alhem.net
**/
/*
Copyright (C) 2004-2007  Anders Hedstrom

This library is made available under the terms of the GNU GPL.

If you would like to use this library in a closed-source application,
a separate license agreement is available. For information about
the closed-source license agreement for the C++ sockets library,
please visit http://www.alhem.net/Sockets/license.html and/or
email license@alhem.net.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "framework.h"


namespace sockets
{


   http_request_socket::http_request_socket(socket_handler_base& h) :
      ::ca::ca(h.get_app()),
      socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_socket(h),
      http_tunnel(h),
      http_client_socket(h)
   {
   }


   http_request_socket::http_request_socket(socket_handler_base & h, const char * pszRequest, const string & url_in) :
      ::ca::ca(h.get_app()),
      socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_socket(h),
      http_tunnel(h),
      http_client_socket(h, url_in)
   {

      m_strRequest = pszRequest;

   }


   http_request_socket::http_request_socket(socket_handler_base & h, const char * pszRequest, const string & host, port_t port, const string & url_in) :
      ::ca::ca(h.get_app()),
      socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_socket(h),
      http_tunnel(h),
      http_client_socket(h, url_in)
   {

      m_strRequest = pszRequest;

      UNREFERENCED_PARAMETER(host);
      UNREFERENCED_PARAMETER(port);

   }


   http_request_socket::~http_request_socket()
   {
   }


   void http_request_socket::step()
   {
      m_request.attr("http_method") = m_strRequest;
      //outheader("Accept") = "text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,video/x-mng,image/png,image/jpeg,image/gif;q=0.2,*/*;q=0.1";
      //outheader("Accept-Language") = "en-us,en;q=0.5";
      //outheader("Accept-Encoding") = "gzip,deflate";
      //outheader("Accept-Charset") = "ISO-8859-1,utf-8;q=0.7,*;q=0.7";
      outheader("User-agent") = MyUseragent();

      if (GetUrlPort() != 80 && GetUrlPort() != 443)
         outheader("Host") = GetUrlHost() + ":" + ::ca::str::from(GetUrlPort());
      else
         outheader("Host") = GetUrlHost();
      SendRequest();
   }

} // namespace sockets

