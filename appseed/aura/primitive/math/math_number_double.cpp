#include "framework.h"
#include "math_number_double.h"


namespace math
{


   void number_double::set_value_string(const char * psz)
   {

   }


   string number_double::get_value_string()
   {
      
      return "";

   }

   
   CLASS_DECL_AURA bool convert_to_double(double & d, string str)
   {

      str.replace(" ", "");
      
      str.replace("\t", "");

      str.replace("\n", "");

      str.replace("\r", "");

      bool bDouble = false;

      if (str.get_length() >= 1)
      {

         if (isdigit_dup(str[0]))
         {

            bDouble = true;

         }
         else if (str[0] == '-' || str[0] == '+')
         {

            if (str.get_length() >= 2)
            {

               if (isdigit_dup(str[1]))
               {

                  bDouble = true;

               }

            }

         }

      }

      if (!bDouble)
      {

         return false;

      }

      d = atof(str);

      return true;

   }


} // namespace math



