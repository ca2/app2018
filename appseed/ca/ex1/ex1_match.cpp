#include "framework.h"


namespace ex1
{

   namespace match
   {


      bool _and::matches(const var & var) const
      {
         return m_pitem1->matches(var) && m_pitem2->matches(var);
      }

      string::string(const char * psz) :
         m_str(psz)
      {
      }

      bool string::matches(const var & var) const
      {
         return var.get_string() == m_str;
      }

      ci_string::ci_string(const char * psz) :
         string(psz)
      {
      }

      bool ci_string::matches(const var & var) const
      {
         return var.get_string().CompareNoCase(m_str) == 0;
      }

      prefix::prefix(const char * psz) :
         string(psz)
      {
      }

      bool prefix::matches(const var & var) const
      {
         return gen::str::begins(var.get_string(), m_str);
      }

      ci_prefix::ci_prefix(const char * psz) :
         string(psz)
      {
      }

      bool ci_prefix::matches(const var & var) const
      {
         return gen::str::begins_ci(var.get_string(), m_str);
      }

      suffix::suffix(const char * psz) :
         string(psz)
      {
      }

      bool suffix::matches(const var & var) const
      {
         return gen::str::ends(var.get_string(), m_str);
      }

      ci_suffix::ci_suffix(const char * psz) :
         string(psz)
      {
      }

      bool ci_suffix::matches(const var & var) const
      {
         return gen::str::ends_ci(var.get_string(), m_str);
      }

      bool any::matches(const var & var) const
      {
         if(this->get_count() <= 0)
            return true;
         else
         {
            for(int i = 0; i < this->get_count(); i++)
            {
               if(ptr_at(i)->matches(var))
                  return true;
            }
            return false;
         }
      }


   } // namespace match


} // namespace ex1

