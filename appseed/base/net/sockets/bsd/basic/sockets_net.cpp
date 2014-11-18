#include "framework.h" // from "base/net/net_sockets.h"




#if defined(LINUX) || defined(APPLEOS)
#include <unistd.h>
#if defined(APPLEOS)
#define	NI_MAXHOST	1025
#define	NI_MAXSERV	32
#endif
//#define __USE_MISC
//#include <ctype.h>
#endif


namespace sockets
{


   net::net(sp(::aura::application) papp) :
      element(papp)
//      m_mapCache(papp,""),
  //    m_mapReverseCache(papp,"sockets::net::m_mapReverseCache")
   {

      m_bInitialized = false;
      //m_mapCache.m_bAutoGudoSet = false;
      //m_mapReverseCache.m_bAutoGudoSet = false;

      //m_mapCache.remove_all();
      //m_mapReverseCache.remove_all();

   }

   net::~net()
   {

   }

   bool net::initialize()
   {

      if(m_bInitialized)
         return true;

      m_bInitialized = true;

      //if(System.m_bGudoNetCache)
      {

         //Application.gudo_get("sockets::net::m_mapCache",m_mapCache);

         //Application.gudo_get("sockets::net::m_mapReverseCache",m_mapReverseCache);

      }


      return true;

   }

   bool net::gudo_set()
   {

      if(!m_bInitialized)
         return false;

//      m_mapCache.gudo_set();

  //    m_mapReverseCache.gudo_set();

      return true;

   }


   bool net::finalize()
   {

      if(!m_bInitialized)
         return false;

      return true;

   }


   /*
   * Encode string per RFC1738 URL encoding rules
   * tnx rstaveley
   */
   string net::rfc1738_encode(const string & src)
   {
      static   char hex[] = "0123456789ABCDEF";
      string dst;
      for (int32_t i = 0; i < src.get_length(); i++)
      {
         if (isalnum((uchar) src[i]))
         {
            dst += src[i];
         }
         else
            if (src[i] == ' ')
            {
               dst += '+';
            }
            else
            {
               uchar ca = static_cast<uchar>(src[i]);
               dst += '%';
               dst += hex[ca / 16];
               dst += hex[ca % 16];
            }
      }
      return dst;
   } // rfc1738_encode


   /*
   * Decode string per RFC1738 URL encoding rules
   * tnx rstaveley
   */
   string net::rfc1738_decode(const string & src)
   {
      string dst;
      for (int32_t i = 0; i < src.get_length(); i++)
      {
         if (src[i] == '%' && isxdigit((uchar) (src[i + 1])) && isxdigit((uchar) (src[i + 2])))
         {
            char c1 = src[++i];
            char c2 = src[++i];
            c1 = c1 - 48 - ((c1 >= 'A') ? 7 : 0) - ((c1 >= 'a') ? 32 : 0);
            c2 = c2 - 48 - ((c2 >= 'A') ? 7 : 0) - ((c2 >= 'a') ? 32 : 0);
            dst += (char)(c1 * 16 + c2);
         }
         else
            if (src[i] == '+')
            {
               dst += ' ';
            }
            else
            {
               dst += src[i];
            }
      }
      return dst;
   } // rfc1738_decode


   bool net::isipv4(const string & str)
   {
      int32_t dots = 0;
      // %! ignore :port?
      for (int32_t i = 0; i < str.get_length(); i++)
      {
         if (str[i] == '.')
            dots++;
         else
            if (!isdigit((uchar) str[i]))
               return false;
      }
      if (dots != 3)
         return false;
      return true;
   }


   bool net::isipv6(const string & str)
   {
      if(str.is_empty())
         return false;
      index qc = 0;
      index qd = 0;
      for (int32_t i = 0; i < str.get_length(); i++)
      {
         qc += (str[i] == ':') ? 1 : 0;
         qd += (str[i] == '.') ? 1 : 0;
      }
      if (qc < 2)
         return false;
      if (qc > 7)
      {
         return false;
      }
      if (qd && qd != 3)
      {
         return false;
      }
      ::str::parse pa(str,":.");
      string tmp = pa.getword();
      while (tmp.get_length())
      {
         if (tmp.get_length() > 4)
         {
            return false;
         }
         for (int32_t i = 0; i < tmp.get_length(); i++)
         {
            if (tmp[i] < '0' || (tmp[i] > '9' && tmp[i] < 'A') ||
               (tmp[i] > 'F' && tmp[i] < 'a') || tmp[i] > 'f')
            {
               return false;
            }
         }
         //
         tmp = pa.getword();
      }
      return true;
   }

   bool net::convert(in_addr & l, const string & str, int32_t ai_flags)
   {

      if(str.is_empty())
         return false;

//      uint32_t dwTimeProfile1 = get_tick_count();

      single_lock sl(&m_mutexCache, true);
      dns_cache_item item;
      if(m_mapCache.Lookup(str, item) && (!item.m_bTimeout || ((::get_tick_count() - item.m_dwLastChecked) < (11 *((84 + 77) * 1000)))))
      {
         l = item.m_ipaddr;
         //         uint32_t dwTimeProfile2 = get_tick_count();
         /*TRACE("Got from cache net::u2ip " + str + " : %d.%d.%d.%d (%d ms)",
         (uint32_t)((byte*)&pitem->m_ipaddr)[0],
         (uint32_t)((byte*)&pitem->m_ipaddr)[1],
         (uint32_t)((byte*)&pitem->m_ipaddr)[2],
         (uint32_t)((byte*)&pitem->m_ipaddr)[3],
         (dwTimeProfile2 - dwTimeProfile1));*/
         return item.r;
      }
      struct sockaddr_in sa;
      memset(&sa, 0, sizeof(sa));
      sa.sin_family = AF_INET;
#ifdef NO_GETADDRINFO
      if ((ai_flags & AI_NUMERICHOST) != 0 || isipv4(host))
      {
         ::str::parse pa((const char *)host, ".");
         union {
            struct {
               uchar b1;
               uchar b2;
               uchar b3;
               uchar b4;
            } a;
            ipaddr_t l;
         } u;
         u.a.b1 = static_cast<uchar>(pa.getvalue());
         u.a.b2 = static_cast<uchar>(pa.getvalue());
         u.a.b3 = static_cast<uchar>(pa.getvalue());
         u.a.b4 = static_cast<uchar>(pa.getvalue());
         memcpy(&sa.sin_addr, &u.l, sizeof(sa.sin_addr));
         return true;
      }
#ifndef LINUX
      struct hostent *he = gethostbyname( host );
      if (!he)
      {
         return false;
      }
      memcpy(&sa.sin_addr, he -> h_addr, sizeof(sa.sin_addr));
#else
      struct hostent he;
      struct hostent *result = NULL;
      int32_t myerrno = 0;
      char buf[2000];
      int32_t n = gethostbyname_r(host, &he, buf, sizeof(buf), &result, &myerrno);
      if (n || !result)
      {
         return false;
      }
      if (he.h_addr_list && he.h_addr_list[0])
         memcpy(&sa.sin_addr, he.h_addr, 4);
      else
         return false;
#endif
      return true;
#else
      struct addrinfo hints;
      memset(&hints, 0, sizeof(hints));
      // AI_NUMERICHOST
      // AI_CANONNAME
      // AI_PASSIVE - server
      // AI_ADDRCONFIG
      // AI_V4MAPPED
      // AI_ALL
      // AI_NUMERICSERV
      hints.ai_flags = ai_flags;
      hints.ai_family = AF_INET;
      hints.ai_socktype = 0;
      hints.ai_protocol = 0;
      struct addrinfo *res;
      if (net::isipv4(str))
         hints.ai_flags |= AI_NUMERICHOST;
      int32_t n = getaddrinfo(str, NULL, &hints, &res);
      if (n)
      {
         string error = "Error: ";
   #ifndef __CYGWIN__
         error += gai_strerror(n);
   #endif
         return false;
   #endif // NO_GETADDRINFO
      }
      item.r = true;
      ptr_array <  addrinfo > vec;
      addrinfo *ai = res;
      while (ai)
      {
         if (ai -> ai_addrlen == sizeof(sa))
            vec.add( ai );
         ai = ai -> ai_next;
      }
      if (!vec.get_count())
         return false;
      ai = vec[rand() % vec.get_count()];
      {
         memcpy(&sa, ai -> ai_addr, ai -> ai_addrlen);
      }
      freeaddrinfo(res);
      item.m_ipaddr = sa.sin_addr;
      item.m_dwLastChecked = ::get_tick_count();
      m_mapCache.set_at(str, item);

      //if(System.m_bGudoNetCache)
      {

         //Application.gudo_set("sockets::net::m_mapCache",m_mapCache);

      }

//      uint32_t dwTimeProfile2 = get_tick_count();
//      TRACE("DNS Lookup net::u2ip " + str + " : %d.%d.%d.%d (%d ms)",
  //       (uint32_t)((byte*)&pitem->m_ipaddr)[0],
    //     (uint32_t)((byte*)&pitem->m_ipaddr)[1],
      //   (uint32_t)((byte*)&pitem->m_ipaddr)[2],
        // (uint32_t)((byte*)&pitem->m_ipaddr)[3],
         //(dwTimeProfile2 - dwTimeProfile1));
      l = item.m_ipaddr;

      return item.r;
   }


   bool net::convert(struct in6_addr& l, const string & str, int32_t ai_flags)
   {


      return from_string(&l, str) != FALSE;


   }


   bool net::convert(string & str, const in_addr & ip)
   {

      struct sockaddr_in sa;

      memset(&sa, 0, sizeof(sa));

      sa.sin_family  = AF_INET;

      sa.sin_addr    = ip;

      return reverse( (struct sockaddr *)&sa, sizeof(sa), str, NI_NUMERICHOST);

   }


   bool net::convert(string & str, const struct in6_addr& ip, bool mixed)
   {


      char slask[100]; // l2ip temporary
      *slask = 0;
      uint32_t prev = 0;
      bool skipped = false;
      bool ok_to_skip = true;
      if (mixed)
      {
         uint16_t x;
         uint16_t addr16[8];
         memcpy(addr16, &ip, sizeof(addr16));
         for (index i = 0; i < 6; i++)
         {
            x = ntohs(addr16[i]);
            if (*slask && (x || !ok_to_skip || prev))
               strcat(slask,":");
            if (x || !ok_to_skip)
            {
               sprintf(slask + strlen(slask),"%x", x);
               if (x && skipped)
                  ok_to_skip = false;
            }
            else
            {
               skipped = true;
            }
            prev = x;
         }
         x = ntohs(addr16[6]);
         sprintf(slask + strlen(slask),":%u.%u",x / 256,x & 255);
         x = ntohs(addr16[7]);
         sprintf(slask + strlen(slask),".%u.%u",x / 256,x & 255);
      }
      else
      {
         struct sockaddr_in6 sa;
         memset(&sa, 0, sizeof(sa));
         sa.sin6_family = AF_INET6;
         sa.sin6_addr = ip;
         return reverse( (struct sockaddr *)&sa, sizeof(sa), str, NI_NUMERICHOST);
      }
      str = slask;
      return true;
   }


   int32_t net::in6_addr_compare(in6_addr a,in6_addr b)
   {
      for (index i = 0; i < 16; i++)
      {
         if (a.s6_addr[i] < b.s6_addr[i])
            return -1;
         if (a.s6_addr[i] > b.s6_addr[i])
            return 1;
      }
      return 0;
   }

   void net::ResolveLocal()
   {

      char h[256];

      // get local hostname and translate into ip-address
      *h = 0;
      gethostname(h,255);
      {
         if(convert(m_ip, h))
         {
            convert(m_addr, m_ip);
         }
      }
#ifdef ENABLE_IPV6
#ifdef IPPROTO_IPV6
      memset(&m_local_ip6, 0, sizeof(m_local_ip6));
      {
         if (convert(m_local_ip6, h))
         {
            convert(m_local_addr6, m_local_ip6);
         }
      }
#endif
#endif
      m_host = h;
      m_local_resolved = true;

   }


   const string & net::GetLocalHostname()
   {
      if (!m_local_resolved)
      {
         ResolveLocal();
      }
      return m_host;
   }


   in_addr net::GetLocalIP()
   {
      if (!m_local_resolved)
      {
         ResolveLocal();
      }
      return m_ip;
   }


   const string & net::GetLocalAddress()
   {
      if (!m_local_resolved)
      {
         ResolveLocal();
      }
      return m_addr;
   }


   const struct in6_addr& net::GetLocalIP6()
   {
      if (!m_local_resolved)
      {
         ResolveLocal();
      }
      return m_local_ip6;
   }


   const string & net::GetLocalAddress6()
   {
      if (!m_local_resolved)
      {
         ResolveLocal();
      }
      return m_local_addr6;
   }

   string net::Sa2String(struct sockaddr *sa)
   {
      if (sa -> sa_family == AF_INET6)
      {
         struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *)sa;
         string tmp;
         convert(tmp, sa6 -> sin6_addr);
         return tmp + ":" + ::str::from(ntohs(sa6 -> sin6_port));
      }
      if (sa -> sa_family == AF_INET)
      {
         struct sockaddr_in *sa4 = (struct sockaddr_in *)sa;
         string tmp;
         convert(tmp, sa4 -> sin_addr);
         return tmp + ":" + ::str::from(ntohs(sa4 -> sin_port));
      }
      return "";
   }


   /*
   ::net::address net::CreateAddress(struct sockaddr *sa,socklen_t sa_len)
   {
      retur
      switch (sa -> sa_family)
      {
      case AF_INET:
         if (sa_len == sizeof(struct sockaddr_in))
         {
            struct sockaddr_in *p = (struct sockaddr_in *)sa;
            ::sockets::address_sp addr;
            addr(new ::sockets::ipv4_address(get_app(), *p));
            return addr;
         }
         break;
      case AF_INET6:
         if (sa_len == sizeof(struct sockaddr_in6))
         {
            struct sockaddr_in6 *p = (struct sockaddr_in6 *)sa;
            ::sockets::address_sp addr;
            addr(new ::sockets::ipv6_address(get_app(), *p));
            return addr;
         }
         break;
      }
      return ::sockets::address_sp();
   }
   */

/*   bool net::convert(in_addr & sa, const string & host, int32_t ai_flags)
   {

      memset(&sa, 0, sizeof(sa));
      sa.sin_family = AF_INET;
#ifdef NO_GETADDRINFO
      if ((ai_flags & AI_NUMERICHOST) != 0 || isipv4(host))
      {
         ::str::parse pa((const char *)host, ".");
         union {
            struct {
               uchar b1;
               uchar b2;
               uchar b3;
               uchar b4;
            } a;
            ipaddr_t l;
         } u;
         u.a.b1 = static_cast<uchar>(pa.getvalue());
         u.a.b2 = static_cast<uchar>(pa.getvalue());
         u.a.b3 = static_cast<uchar>(pa.getvalue());
         u.a.b4 = static_cast<uchar>(pa.getvalue());
         memcpy(&sa.sin_addr, &u.l, sizeof(sa.sin_addr));
         return true;
      }
#ifndef LINUX
      struct hostent *he = gethostbyname( host );
      if (!he)
      {
         return false;
      }
      memcpy(&sa.sin_addr, he -> h_addr, sizeof(sa.sin_addr));
#else
      struct hostent he;
      struct hostent *result = NULL;
      int32_t myerrno = 0;
      char buf[2000];
      int32_t n = gethostbyname_r(host, &he, buf, sizeof(buf), &result, &myerrno);
      if (n || !result)
      {
         return false;
      }
      if (he.h_addr_list && he.h_addr_list[0])
         memcpy(&sa.sin_addr, he.h_addr, 4);
      else
         return false;
#endif
      return true;
#else
      struct addrinfo hints;
      memset(&hints, 0, sizeof(hints));
      // AI_NUMERICHOST
      // AI_CANONNAME
      // AI_PASSIVE - server
      // AI_ADDRCONFIG
      // AI_V4MAPPED
      // AI_ALL
      // AI_NUMERICSERV
      hints.ai_flags = ai_flags;
      hints.ai_family = AF_INET;
      hints.ai_socktype = 0;
      hints.ai_protocol = 0;
      struct addrinfo *res;
      if (net::isipv4(host))
         hints.ai_flags |= AI_NUMERICHOST;
      int32_t n = getaddrinfo(host, NULL, &hints, &res);
      if (!n)
      {
         ptr_array <  addrinfo > vec;
         addrinfo *ai = res;
         while (ai)
         {
            if (ai -> ai_addrlen == sizeof(sa))
               vec.add( ai );
            ai = ai -> ai_next;
         }
         if (!vec.get_count())
            return false;
         ai = vec[System.math().rnd() % vec.get_count()];
         {
            memcpy(&sa, ai -> ai_addr, ai -> ai_addrlen);
         }
         freeaddrinfo(res);
         return true;
      }
      string error = "Error: ";
#ifndef __CYGWIN__
      error += gai_strerror(n);
#endif
      return false;
#endif // NO_GETADDRINFO
   }*/


   /*
   bool net::convert(struct in6_addr & sa, const string & host, int32_t ai_flags)
   {

      memset(&sa, 0, sizeof(sa));
      sa.sin6_family = AF_INET6;
#ifdef NO_GETADDRINFO
      if ((ai_flags & AI_NUMERICHOST) != 0 || isipv6(host))
      {
         //         list<string> vec;
         index x = 0;
         for (index i = 0; i <= host.get_length(); i++)
         {
            if (i == host.get_length() || host[i] == ':')
            {
               string s = host.Mid(x, i - x);
               //
               if (strstr(s,".")) // x.x.x.x
               {
                  Parse pa(s,".");
                  char slask[100]; // u2ip temporary hgenstring conversion
                  unsigned long b0 = static_cast<unsigned long>(pa.getvalue());
                  unsigned long b1 = static_cast<unsigned long>(pa.getvalue());
                  unsigned long b2 = static_cast<unsigned long>(pa.getvalue());
                  unsigned long b3 = static_cast<unsigned long>(pa.getvalue());
                  sprintf(slask,"%lx",b0 * 256 + b1);
                  vec.push_back(slask);
                  sprintf(slask,"%lx",b2 * 256 + b3);
                  vec.push_back(slask);
               }
               else
               {
                  vec.push_back(s);
               }
               //
               x = i + 1;
            }
         }
         index sz = vec.get_length(); // number of byte pairs
         index i = 0; // index in in6_addr.in6_u.u6_addr16[] ( 0 .. 7 )
         uint16_t addr16[8];
         for (list<string>::iterator it = vec.begin(); it != vec.end(); it++)
         {
            string bytepair = *it;
            if (bytepair.get_length())
            {
               addr16[i++] = htons(net::hgenunsigned(bytepair));
            }
            else
            {
               addr16[i++] = 0;
               while (sz++ < 8)
               {
                  addr16[i++] = 0;
               }
            }
         }
         memcpy(&sa.sin6_addr, addr16, sizeof(addr16));
         return true;
      }
#ifdef SOLARIS
      int32_t errnum = 0;
      struct hostent *he = getipnodebyname( host, AF_INET6, 0, &errnum );
#else
      struct hostent *he = gethostbyname2( host, AF_INET6 );
#endif
      if (!he)
      {
         return false;
      }
      memcpy(&sa.sin6_addr,he -> h_addr_list[0],he -> h_length);
#ifdef SOLARIS
      free(he);
#endif
      return true;
#else
      struct addrinfo hints;
      memset(&hints, 0, sizeof(hints));
      hints.ai_flags = ai_flags;
      hints.ai_family = AF_INET6;
      hints.ai_socktype = 0;
      hints.ai_protocol = 0;
      struct addrinfo *res;
      if (net::isipv6(host))
         hints.ai_flags |= AI_NUMERICHOST;
      int32_t n = getaddrinfo(host, NULL, &hints, &res);
      if (!n)
      {
         ptr_array <  addrinfo > vec;
         struct addrinfo *ai = res;
         while (ai)
         {
            if (ai -> ai_addrlen == sizeof(sa))
               vec.add( ai );
            ai = ai -> ai_next;
         }
         if (!vec.get_count())
            return false;
         ai = vec[System.math().rnd() % vec.get_count()];
         {
            memcpy(&sa, ai -> ai_addr, ai -> ai_addrlen);
         }
         freeaddrinfo(res);
         return true;
      }
      string error = "Error: ";
#ifndef __CYGWIN__
      error += gai_strerror(n);
#endif
      return false;
#endif // NO_GETADDRINFO

   }
   */


   bool net::reverse(string & number, const string & hostname, int32_t flags)
   {

      ::net::address address(hostname);

      number = address.get_display_number();

      return true;

   }


   bool net::reverse(struct sockaddr *sa, socklen_t sa_len, string & hostname, int32_t flags)
   {
      string service;
      return reverse(sa, sa_len, hostname, service, flags);
   }


   bool net::reverse(struct sockaddr *sa, socklen_t sa_len, string & hostname, string & service, int32_t flags)
   {

      single_lock sl(&m_mutexCache,true);
      reverse_cache_item item;
      string strIpString = to_vsstring(sa);
      if(m_mapReverseCache.Lookup(strIpString,item) && (!item.m_bTimeout || ((::get_tick_count() - item.m_dwLastChecked) < (11 * ((84 + 77) * 1000)))))
      {
         hostname = item.m_strReverse;
         return item.r;
      }
      else
      {
         item.m_ipaddr = *((in_addr*)sa->sa_data);
      }


      hostname = "";
      service = "";
#ifdef NO_GETADDRINFO
      switch (sa -> sa_family)
      {
      case AF_INET:
         if (flags & NI_NUMERICHOST)
         {
            union {
               struct {
                  uchar b1;
                  uchar b2;
                  uchar b3;
                  uchar b4;
               } a;
               ipaddr_t l;
            } u;
            struct sockaddr_in *sa_in = (struct sockaddr_in *)sa;
            memcpy(&u.l, &sa_in -> sin_addr, sizeof(u.l));
            char tmp[100];
            sprintf(tmp, "%u.%u.%u.%u", u.a.b1, u.a.b2, u.a.b3, u.a.b4);
            hostname = tmp;
            return true;
         }
         else
         {
            struct sockaddr_in *sa_in = (struct sockaddr_in *)sa;
            struct hostent *h = gethostbyaddr( (const char *)&sa_in -> sin_addr, sizeof(sa_in -> sin_addr), AF_INET);
            if (h)
            {
               hostname = h -> h_name;
               return true;
            }
         }
         break;
#ifdef ENABLE_IPV6
      case AF_INET6:
         if (flags & NI_NUMERICHOST)
         {
            char slask[100]; // l2ip temporary
            *slask = 0;
            uint32_t prev = 0;
            bool skipped = false;
            bool ok_to_skip = true;
            {
               uint16_t addr16[8];
               struct sockaddr_in6 *sa_in6 = (struct sockaddr_in6 *)sa;
               memcpy(addr16, &sa_in6 -> sin6_addr, sizeof(addr16));
               for (index i = 0; i < 8; i++)
               {
                  uint16_t x = ntohs(addr16[i]);
                  if (*slask && (x || !ok_to_skip || prev))
                     strcat(slask,":");
                  if (x || !ok_to_skip)
                  {
                     sprintf(slask + strlen(slask),"%x", x);
                     if (x && skipped)
                        ok_to_skip = false;
                  }
                  else
                  {
                     skipped = true;
                  }
                  prev = x;
               }
            }
            if (!*slask)
               strcpy(slask, "::");
            hostname = slask;
            return true;
         }
         else
         {
            // %! TODO: ipv6 reverse lookup
            struct sockaddr_in6 *sa_in = (struct sockaddr_in6 *)sa;
            struct hostent *h = gethostbyaddr( (const char *)&sa_in -> sin6_addr, sizeof(sa_in -> sin6_addr), AF_INET6);
            if (h)
            {
               hostname = h -> h_name;
               return true;
            }
         }
         break;
#endif
      }
      return false;
#else
      char host[NI_MAXHOST];
      char serv[NI_MAXSERV];
      // NI_NOFQDN
      // NI_NUMERICHOST
      // NI_NAMEREQD
      // NI_NUMERICSERV
      // NI_DGRAM
      int32_t n = getnameinfo(sa, sa_len, host, sizeof(host), serv, sizeof(serv), flags);
      if (n)
      {
         // EAI_AGAIN
         // EAI_BADFLAGS
         // EAI_FAIL
         // EAI_FAMILY
         // EAI_MEMORY
         // EAI_NONAME
         // EAI_OVERFLOW
         // EAI_SYSTEM
         return false;
      }
      hostname = host;
      service = serv;
      item.m_strReverse = hostname;
      item.m_dwLastChecked = ::get_tick_count();
      m_mapReverseCache.set_at(strIpString,item);

      return true;
#endif // NO_GETADDRINFO
   }

   string net::reverse_name(const ::net::address & address)
   {

      string strHostname;

      reverse((sockaddr *) &address.u.m_sa,sizeof(address.u.m_sa), strHostname, 0);

      return strHostname;

   }


   string net::reverse_name(const string & strAddress)
   {

      ::net::address address(strAddress);

      return reverse_name(address);

   }



   bool net::u2service(const string & name, int32_t& service, int32_t ai_flags)
   {

#ifdef NO_GETADDRINFO
      // %!
      return false;
#else
      struct addrinfo hints;
      service = 0;
      memset(&hints, 0, sizeof(hints));
      // AI_NUMERICHOST
      // AI_CANONNAME
      // AI_PASSIVE - server
      // AI_ADDRCONFIG
      // AI_V4MAPPED
      // AI_ALL
      // AI_NUMERICSERV
      hints.ai_flags = ai_flags;
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = 0;
      hints.ai_protocol = 0;
      struct addrinfo *res;
      int32_t n = getaddrinfo(NULL, name, &hints, &res);
      if (!n)
      {
         service = res -> ai_protocol;
         freeaddrinfo(res);
         return true;
      }
      return false;
#endif // NO_GETADDRINFO

   }


   int32_t net::service_port(const string & str, int32_t flags)
   {

      if(::str::is_simple_natural(str))
         return ::str::to_int(str);

      if(str.CompareNoCase("http"))
      {
         return 80;
      }
      else if(str.CompareNoCase("https"))
      {
         return 443;
      }
      else
      {

         int32_t service = 0;

         if(!u2service(str, service, 0))
            return 0;

         return service;

      }

   }


   string  net::service_name(int32_t iPort, int32_t flags)
   {

      switch(iPort)
      {
      case 80:
         return "http";
      case 443:
         return "https";
      default:
         return "";
      }

   }

   string net::canonical_name(const ::net::address & address)
   {

      string str;

      if(address.is_ipv4())
      {
         ::sockets::net::convert(str, address.u.m_addr.sin_addr);
      }
      else if(address.is_ipv6())
      {
         ::sockets::net::convert(str, address.u.m_addr6.sin6_addr);
      }

      return str;

   }


   string net::service_name(const ::net::address & address)
   {

      return service_name(address.get_service_number());

   }


   net::dns_cache_item::dns_cache_item()
   {

      ZERO(m_ipaddr);
      m_dwLastChecked = 0;
      r = false;
      m_bTimeout = true;
   }


   net::dns_cache_item::dns_cache_item(const dns_cache_item & item)
   {

      this->operator = (item);

   }


   void net::dns_cache_item::write(::file::output_stream & ostream) const
   {

      ostream.write(&m_ipaddr, sizeof(m_ipaddr));
      ostream.write(&m_dwLastChecked,sizeof(m_dwLastChecked));
      ostream.write(&r,sizeof(r));
      ostream.write(&m_bTimeout,sizeof(m_bTimeout));

   }


   void net::dns_cache_item::read(::file::input_stream & istream)
   {

      istream.read(&m_ipaddr,sizeof(m_ipaddr));
      istream.read(&m_dwLastChecked,sizeof(m_dwLastChecked));
      istream.read(&r,sizeof(r));
      istream.read(&m_bTimeout,sizeof(m_bTimeout));

   }


   net::dns_cache_item & net::dns_cache_item::operator = (const dns_cache_item & item)
   {

      if(this==&item)
         return *this;

      memcpy(&m_ipaddr,&item.m_ipaddr,sizeof(m_ipaddr));
      m_dwLastChecked = item.m_dwLastChecked;
      r = item.r;
      m_bTimeout = item.m_bTimeout;

      return *this;

   }


   net::reverse_cache_item::reverse_cache_item()
   {

      ZERO(m_ipaddr);
      m_dwLastChecked = 0;
      r = false;
      m_bTimeout = true;

   }


   net::reverse_cache_item::reverse_cache_item(const reverse_cache_item & item)
   {

      this->operator = (item);

   }

   void net::reverse_cache_item::write(::file::output_stream & ostream) const
   {

      ostream.write(&m_ipaddr,sizeof(m_ipaddr));
      ostream.write(m_strReverse);
      ostream.write(&m_dwLastChecked,sizeof(m_dwLastChecked));
      ostream.write(&r,sizeof(r));
      ostream.write(&m_bTimeout,sizeof(m_bTimeout));

   }


   void net::reverse_cache_item::read(::file::input_stream & istream)
   {

      istream.read(&m_ipaddr,sizeof(m_ipaddr));
      istream.read(m_strReverse);
      istream.read(&m_dwLastChecked,sizeof(m_dwLastChecked));
      istream.read(&r,sizeof(r));
      istream.read(&m_bTimeout,sizeof(m_bTimeout));
   }


   net::reverse_cache_item & net::reverse_cache_item::operator = (const reverse_cache_item & item)
   {

      if(this==&item)
         return *this;

      memcpy(&m_ipaddr, &item.m_ipaddr, sizeof(m_ipaddr));
      m_dwLastChecked = item.m_dwLastChecked;
      m_strReverse = item.m_strReverse;
      r = item.r;
      m_bTimeout = item.m_bTimeout;

      return *this;

   }


} // namespace sockets





namespace net
{


   int32_t family_len(int32_t family)
   {

      if(family == AF_INET)
         return sizeof(sockaddr_in);
      else if(family == AF_INET6)
         return sizeof(sockaddr_in6);
      else
         return 0;

   }


} // namespace net














