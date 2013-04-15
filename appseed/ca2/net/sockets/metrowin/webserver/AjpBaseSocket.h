/**
 **   \file AjpBaseSocket.h
 **   \date  2007-10-05
 **   \author grymse@alhem.net
**/
/*
Copyright (C) 2007  Anders Hedstrom

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


   class CLASS_DECL_ca2 AjpBaseSocket : public tcp_socket
   {
   public:

      class Initializer
      {
      public:
         Initializer();
         virtual ~Initializer() {}

         ::collection::int_to_string Method;
         ::collection::int_to_string header;
         ::collection::int_to_string Attribute;
         ::collection::string_map < int > ResponseHeader;

      };

      AjpBaseSocket(socket_handler_base& h);

      void OnRawData(char *buf, size_t sz);

      virtual void OnHeader( short id, short len ) = 0;
      virtual void OnPacket( const char *buf, size_t sz ) = 0;

   protected:
      unsigned char get_byte(const char *buf, int& ptr);
      bool get_boolean(const char *buf, int& ptr);
      short get_integer(const char *buf, int& ptr);
      string get_string(const char *buf, int& ptr);

      void put_byte(char *buf, int& ptr, unsigned char zz);
      void put_boolean(char *buf, int& ptr, bool zz);
      void put_integer(char *buf, int& ptr, short zz);
      void put_string(char *buf, int& ptr, const char * psz);

   private:
      int m_state;
      int m_length;
      int m_ptr;
      char m_message[8192];
   };


} // namespace sockets



