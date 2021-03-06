#include "framework.h"


#if defined(WINDOWS)


string demangle(const char* name)
{

   return name;

}

#else





#include <cxxabi.h>

string demangle (const char* name)
{

   int status = -4;
   char* res = abi::__cxa_demangle(name, 0, 0, &status);
   string str;
   if (status == 0)
   {
      str = res;

   }
   else
   {
      str = name;
   }
   if (res != NULL)
   {
      free(res);
   }
   return str;
}

#endif


type::type(const id & id, const ::id & idFriendly)
{

   m_pfactoryitem = NULL;
   m_id              = id;
   m_idFriendly      = idFriendly;

}


type::type()
{

   m_pfactoryitem = NULL;

}

type::~type()
{

}


type::type(const type & info)
{

   m_id              = info.m_id;
   m_idFriendly      = info.m_idFriendly;
   m_pfactoryitem    = info.m_pfactoryitem;

}


type::type(const std_type_info & info)
{

   m_pfactoryitem = NULL;
#ifdef WINDOWS
   m_idFriendly      = info.name();
   m_id              = info.raw_name();
#elif defined(VSNORD)
   m_idFriendly	   = demangle(info.name());
   m_id			      = info.name();
#elif defined(APPLEOS)
   m_idFriendly      = demangle(info.name());
   m_id              = info.name();
#else
   m_idFriendly      = demangle(info.name());
   m_id              = info.name();
#endif

}


type::type(const string & strName, const string & strFriendlyName)
{

   m_pfactoryitem = NULL;
   m_id              = strName;
   m_idFriendly      = strFriendlyName;

}


type & type::operator = (const type & info)
{

   m_id              = info.m_id;
   m_idFriendly      = info.m_idFriendly;
   m_pfactoryitem    = info.m_pfactoryitem;

   return *this;

}


type & type::operator = (const std_type_info & info)
{

#ifdef WINDOWS
   m_idFriendly      = info.name();
   m_id              = info.raw_name();
#elif defined(VSNORD)
   m_idFriendly      = info.name();
   m_id              = info.name();
#elif defined(APPLEOS)
   m_idFriendly      = demangle(info.name());
   m_id              = info.name();
#else
   m_idFriendly      = demangle(info.name());
   m_id              = info.name();
#endif
//   m_pfactoryitem.release();

   return *this;

}


bool type::operator == (const type & info) const
{

   return m_id == info.m_id;

}


bool type::operator == (const std_type_info & info) const
{

#ifdef WINDOWS

   return m_id == info.raw_name();

#else

   return m_id == info.name();

#endif // WINDOWS

}


bool type::operator == (const string & strName) const
{

   return m_id == strName;

}


bool type::operator == (const id & id ) const
{

   return m_id == id ;

}


bool type::operator != (const type & info) const
{

   return m_id != info.m_id;

}


bool type::operator != (const std_type_info & info) const
{

#ifdef WINDOWS

   return m_id != info.raw_name();

#else

   return m_id != info.name();

#endif // WINDOWS

}


bool type::operator != (const string & strName) const
{

   return m_id != strName;

}


bool type::operator != (const id & id ) const
{

   return m_id != id ;

}


const char * type::name() const
{

   return m_id;

}

const char * type::friendly_name() const
{

   return m_idFriendly;

}


ptra * type::new_ptra()
{

   return new ptra();

}


CLASS_DECL_AURA bool operator == (const std_type_info & info1, const sp(type) info2)
{

#ifdef WINDOWS

   return strcmp(info1.raw_name(), info2->name()) == 0;

#else

   return strcmp(info1.name(), info2->name()) == 0;

#endif

}

CLASS_DECL_AURA bool operator != (const std_type_info & info1, const sp(type) info2)
{

   return !operator == (info1, info2);

}

