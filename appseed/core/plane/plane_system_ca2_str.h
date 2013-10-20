#pragma once


   namespace str
   {


      inline CLASS_DECL_CORE string         from(const id & id)
      {

         return id;

      }

      inline CLASS_DECL_CORE string &       from(string & str, const id & id)
      {

         str = id;

         return str;

      }


      inline bool CLASS_DECL_CORE begins_eat(var & var, const char * lpcszPrefix)
      {

         return begins_eat(var.get_ref_string(), lpcszPrefix);

      }

      inline bool CLASS_DECL_CORE begins_eat_ci(var & var, const char * lpcszPrefix) // case insensitive
      {

         return begins_eat_ci(var.get_ref_string(), lpcszPrefix);

      }

      inline bool CLASS_DECL_CORE begins_eat_ci(var & var, const char * lpcszPrefix, const char * pszSeparator) // case insensitive
      {

         return begins_eat_ci(var.get_ref_string(), lpcszPrefix, pszSeparator);

      }

      inline bool CLASS_DECL_CORE begins_eat(property & property, const char * lpcszPrefix)
      {

         return begins_eat(property.m_var, lpcszPrefix);

      }

      inline bool CLASS_DECL_CORE begins_eat_ci(property & property, const char * lpcszPrefix) // case insensitive
      {

         return begins_eat_ci(property.m_var, lpcszPrefix);

      }

      inline bool CLASS_DECL_CORE begins_eat_ci(property & property, const char * lpcszPrefix, const char * pszSeparator) // case insensitive
      {

         return begins_eat_ci(property.m_var, lpcszPrefix, pszSeparator);

      }



   } // namespace str


   inline static string log_level_name(::core::log::e_level elevel)
   {
      switch(elevel)
      {
      case ::core::log::level_warning:
         return "warning";
      case ::core::log::level_error:
         return "error";
      case ::core::log::level_fatal:
         return "fatal";
      case ::core::log::level_info:
         return "info";
      default:
         return "undefined log error";
      }
   }






/*
   inline dump_context & dump_context::operator<<(const string & str)
   {
      *this << static_cast< const char * >( str );
      return *this;
   }

   */