#pragma once


namespace str
{


   inline CLASS_DECL_AURA string         from(const id & id)
   {

      return id;

   }

   inline CLASS_DECL_AURA string &       from(string & str, const id & id)
   {

      str = id;

      return str;

   }


   inline bool begins_eat(var & var, const string & strPrefix)
   {

      return begins_eat(var.get_ref_string(),strPrefix);

   }

   inline bool begins_eat_ci(var & var, const char * lpcszPrefix) // case insensitive
   {

      return begins_eat_ci(var.get_ref_string(), lpcszPrefix);

   }

   inline bool begins_eat_ci(var & var, const char * lpcszPrefix, const char * pszSeparator) // case insensitive
   {

      return begins_eat_ci(var.get_ref_string(), lpcszPrefix, pszSeparator);

   }

   inline bool begins_eat(property & property, const string & strPrefix)
   {

      return begins_eat(property.m_element2,strPrefix);

   }

   inline bool begins_eat_ci(property & property, const char * lpcszPrefix) // case insensitive
   {

      return begins_eat_ci(property.m_element2, lpcszPrefix);

   }

   inline bool begins_eat_ci(property & property, const char * lpcszPrefix, const char * pszSeparator) // case insensitive
   {

      return begins_eat_ci(property.m_element2, lpcszPrefix, pszSeparator);

   }



} // namespace str


#ifdef DEBUG


inline static string log_level_name(::aura::log::e_level elevel)
{
   switch (elevel)
   {
   case ::aura::log::level_warning:
      return "warning";
   case ::aura::log::level_error:
      return "error";
   case ::aura::log::level_fatal:
      return "fatal";
   case ::aura::log::level_info:
      return "info";
   default:
      return "undefined log error";
   }
}

#endif




/*
inline dump_context & dump_context::operator<<(const string & str)
{
*this << static_cast< const char * >( str );
return *this;
}

*/


