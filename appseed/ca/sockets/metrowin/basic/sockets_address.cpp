#include "framework.h"


namespace sockets
{


   address::address(::ca::application * papp, const char * pszAddress, const char * pszServiceName)
   {

      m_hostname = ref new ::Windows::Networking::HostName(rtstr(pszAddress));

      m_strServiceName = pszServiceName;

   }


   address::address(::ca::application * papp, const char * pszAddress, int iPort)
   {

      m_hostname = ref new ::Windows::Networking::HostName(rtstr(pszAddress));

      m_strServiceName = gen::str::from(iPort);

   }

   
   address::address(const address & address)
   {

      operator = (address);

   }


   address::~address()
   {

   }


   address & address::operator = (const address & address)
   {

      m_hostname        = address.m_hostname;
      m_strServiceName  = address.m_strServiceName;

      return *this;

   }


   bool address::operator == (const address & address) const
   {

      return m_hostname->IsEqual(address.m_hostname) && get_service_number() == address.get_service_number();

   }


   string address::get_display_number() const
   {

      return gen::international::unicode_to_utf8(m_hostname->DisplayName->Begin());

   }


   string address::get_canonical_name() const
   {

      return gen::international::unicode_to_utf8(m_hostname->CanonicalName->Begin());

   }


   string address::get_service_name() const
   {

      if(gen::str::is_simple_natural(m_strServiceName))
         return service_number_to_name(gen::str::to_int(m_strServiceName));
      else
         return m_strServiceName;

   }


   int address::get_service_number() const
   {

      if(gen::str::is_simple_natural(m_strServiceName))
         return service_name_to_number(m_strServiceName);
      else
         return gen::str::to_int(m_strServiceName);

   }
   

   int address::service_name_to_number(const char * psz) const
   {

      string str(psz);

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
         return 0;
      }

   }


   string  address::service_number_to_name(int iPort) const
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

   
   bool address::is_ipv4() const
   {

      return System.net().isipv4(get_display_number());

   }

   bool address::is_ipv6() const
   {

      return System.net().isipv6(get_display_number());

   }


   bool address::is_in_net(const address & addr, const address & addrMask) const
   {

      if(is_ipv4() && addr.is_ipv4() && addrMask.is_ipv4())
      {
         
         in_addr a1;
         in_addr a2;
         in_addr aM;

         System.net().convert(a1, get_display_number());
         System.net().convert(a2, addr.get_display_number());
         System.net().convert(aM, addrMask.get_display_number());

         return (a1.S_un.S_addr & aM.S_un.S_addr) ==  (a2.S_un.S_addr & aM.S_un.S_addr);

      }
      else if(is_ipv6() && addr.is_ipv6() && addrMask.is_ipv6())
      {
         
         in6_addr a1;
         in6_addr a2;
         in6_addr aM;

         System.net().convert(a1, get_display_number());
         System.net().convert(a2, addr.get_display_number());
         System.net().convert(aM, addrMask.get_display_number());

         memand_dup(&a1, &a1, &aM, sizeof(a1));
         memand_dup(&a2, &a2, &aM, sizeof(a2));

         return memcmp(&a1, &a2, sizeof(aM)) == 0;

      }
      else
      {
         return false;
      }

   }


} // namespace sockets




