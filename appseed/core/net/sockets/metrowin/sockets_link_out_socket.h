//OutSocket.h
/*
Copyright (C) 2004  Anders Hedstrom

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

#pragma once


namespace sockets
{


   class link_in_socket;

   class CLASS_DECL_ca2 link_out_socket : public tcp_socket
   {
   public:

      link_out_socket(base_socket_handler & h);
      virtual ~link_out_socket();

      virtual void OnRead( char *buf, size_t n );
      void link_write(void * p, size_t size);


      void server_to_link_out(httpd_socket * psocket);

      link_in_socket * m_in;
      socket * m_out;
   };



} // namespace sockets


