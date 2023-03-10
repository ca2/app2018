#include "framework.h"
#include "math_value.h"
#include "math_number.h"

#if defined(LINUX)
#include <ctype.h>
#endif

#include "aura/exception/exception_not_implemented.h"


namespace math
{


   void value::set_value_string(const char * psz)
   {
      m_pimpl->set_value_string(psz);
   }

   string value::get_value_string(e_get_value egetvalue)
   {
      return m_pimpl->get_value_string(egetvalue);;
   }

   value_impl::value_impl()
   {


   }

   value_impl::~value_impl()
   {


   }


   void value_impl::set_value_string(const char * psz)
   {
      m_str = psz;
   }

   string value_impl::get_value_string(e_get_value egetvalue)
   {
      return m_str;
   }

   void value::parse_numeric_formula(const char * psz)
   {
//      int32_t iOpen               = 0;
      //    bool bLeft              = false;
      //  bool bNumber            = false;
      m_pimpl                 = new number();
//      const char * pszNumber  = NULL;
      //    value_impl * pimpl      = m_pimpl;

      string strNumber;
//      binary_operation * pbinopNew;
      pointer_array < number * > stackptra;

      _throw(not_implemented(get_app()));

      /*

      while(*psz != '\0')
      {
         if(isdigit(*psz))
         {
            if(!bNumber)
            {
               pszNumber = psz;
               bNumber = true;
            }
         }
         else if(!isdigit(*psz))
         {
            if(bNumber)
            {
               string str(pszNumber, psz - pszNumber - 1);
               strNumber = str;
               if(dynamic_cast < binary_operation * > (pimpl) != NULL)
               {
                  dynamic_cast < binary_operation * > (pimpl)->m_pvalue2 = new number();
                  dynamic_cast < binary_operation * > (pimpl)->m_pvalue2->set_value_string(strNumber);
                  dynamic_cast < binary_operation * > (pimpl)->m_pvalue2->m_pbinop = dynamic_cast < binary_operation * > (pimpl);
                  pimpl = dynamic_cast < binary_operation * > (pimpl)->m_pvalue2;

               }
               else(dynamic_cast < number * > (pimpl) != NULL)
               {
                  dynamic_cast < number * > (pimpl)->set_value_string(strNumber);
               }
            }
            if(*psz == '(')
            {
               stackptra.Push(pimpl);
            }
            else if(*psz == ')')
            {
               pimpl = stackptra.Pop();
            }
            else if(*psz == '+')
            {
               ASSERT(dynamic_cast < number * > (pimpl) != NULL);
               pbinopNew = new binary_operation;
               pbinopNew->m_etype = binary_operation::type_addition;
               pbinopNew->m_pvalue1 = dynamic_cast < number * > (pimpl);
            }
            else if(*psz == '-')
            {
               ASSERT(dynamic_cast < number * > (pimpl) != NULL);
               pbinopNew = new binary_operation;
               pbinopNew->m_etype = binary_operation::type_subtraction;
               pbinopNew->m_pvalue1 = dynamic_cast < number * > (pimpl);
            }
            else if(*psz == '*')
            {
               ASSERT(dynamic_cast < number * > (pimpl) != NULL);
               pbinopNew = new binary_operation;
               pbinopNew->m_etype = binary_operation::type_multiplication;
               pbinopNew->m_pvalue1 = dynamic_cast < number * > (pimpl);
            }
            else if(*psz == '/')
            {
               ASSERT(dynamic_cast < number * > (pimpl) != NULL);
               pbinopNew = new binary_operation;
               pbinopNew->m_etype = binary_operation::type_division;
               pbinopNew->m_pvalue1 = dynamic_cast < number * > (pimpl);
            }

         }
      }*/

   }


} // namespace math




