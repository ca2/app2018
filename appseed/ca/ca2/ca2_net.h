// http://www.alhem.net/
/** from C++ Sockets Library
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
#pragma once


// 1.8.6: define FD_SETSIZE to something bigger than 64 if there are a lot of
// simultaneous connections (must be done before including winsock.h)
//#define FD_SETSIZE 1024

// windows 2000 with ipv6 preview installed:
//    http://msdn.microsoft.com/downloads/sdks/platform/tpipv6.asp
// see the FAQ on how to install
/*#define WIN32_LEAN_AND_MEAN
#include <ws2tcpip.h>
#if _MSC_VER < 1200
#ifndef __CYGWIN__
#ifdef ENABLE_IPV6
#include <tpipv6.h>  // For IPv6 Tech Preview.
#endif
#endif
#endif // _MSC_VER < 1200
*/

typedef unsigned long ipaddr_t;
typedef unsigned short port_t;
#ifdef WINDOWS
typedef int socklen_t;
#endif

#include "ca2_sockets_address.h"

namespace ca2
{
// http://www.alhem.net/
/** from C++ Sockets Library \file Utility.h
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

   class CLASS_DECL_ca net :
      public ::radix::object
   {
   private:
      string m_host; ///< local hostname
      ipaddr_t m_ip; ///< local ip address
      string m_addr; ///< local ip address in string format
      string m_local_addr6; ///< local ip address in string format
      struct in6_addr m_local_ip6; ///< local ipv6 address
      bool m_local_resolved; ///< ResolveLocal has been called if true
   public:


      class CLASS_DECL_ca dns_cache_item
      {
      public:
         
         ipaddr_t       m_ipaddr;
         DWORD          m_dwLastChecked;
         bool           r;

      };

      mutex                                           m_mutexCache;
      ::collection::string_map < dns_cache_item * >   m_mapCache;

      /*
      * Encode string per RFC1738 URL encoding rules
      * tnx rstaveley
      */
      string rfc1738_encode(const string & src);

      /*
      * Decode string per RFC1738 URL encoding rules
      * tnx rstaveley
      */
      string rfc1738_decode(const string & src);

      bool isipv4(const string & str);

      bool isipv6(const string & str);

      bool u2ip(const string & str, ipaddr_t& l, int ai_flags = 0);
      bool u2ip(const string &, struct sockaddr_in& sa, int ai_flags = 0);

      /** Hostname to ip resolution ipv6, not asynchronous. */
      bool u2ip(const string & str, struct in6_addr& l, int ai_flags = 0);
      bool u2ip(const string &, struct sockaddr_in6& sa, int ai_flags = 0);

      void l2ip(const ipaddr_t ip, string & str);

      void l2ip(const in_addr& ip, string & str);

      void l2ip(const struct in6_addr& ip, string & str,bool mixed = false);

      int in6_addr_compare(in6_addr a,in6_addr b);

      void ResolveLocal();

      const string & GetLocalHostname();

      ipaddr_t GetLocalIP();

      const string & GetLocalAddress();

      const struct in6_addr& GetLocalIP6();

      const string & GetLocalAddress6();

      string Sa2String(sockaddr * psa);

      sockets::address_sp CreateAddress(sockaddr * psa, socklen_t sa_len);

      bool reverse(sockaddr * psa, socklen_t sa_len, string & hostname, int flags = 0);

      bool reverse(sockaddr * psa, socklen_t sa_len, string & hostname, string & service, int flags = 0);

      bool u2service(const string & name, int& service, int ai_flags);

   };

} // namespace ca2
