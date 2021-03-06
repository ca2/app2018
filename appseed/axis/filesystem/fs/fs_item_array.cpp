#include "framework.h"


namespace fs
{


   var item_array::get_var_file() const
   {

      if (this->get_count() <= 0)
      {

         return ::var(::var::type_empty);

      }
      else if (this->get_count() == 1)
      {

         if (this->element_at(0)->m_filepathEx.has_char())
         {

            return this->element_at(0)->m_filepathEx;

         }
         else
         {

            return this->element_at(0)->m_filepath;

         }

      }
      else
      {

         class var var;

         for(index i = 0; i < this->get_count(); i++)
         {

            if (this->element_at(i)->m_filepathEx.has_char())
            {

               var.stra().add(this->element_at(i)->m_filepathEx);

            }
            else
            {

               var.stra().add(this->element_at(i)->m_filepath);

            }

         }

         return var;

      }

   }


   var item_array::get_var_query() const
   {

      if (this->get_count() <= 0)
      {

         return ::var(::var::type_empty);

      }
      else if(this->get_count() == 1)
      {

         class var var;

         var["FileManagerItem_flags"] = canew(flags < e_flag >(this->element_at(0)->m_flags));

         return var;

      }
      else
      {

         class var var;

         for(index i = 0; i < this->get_count(); i++)
         {

            class var varPropSet;

            varPropSet["FileManagerItem_flags"] = canew(flags < e_flag > (this->element_at(i)->m_flags));

            var.vara().add(varPropSet);

         }

         return var;

      }

   }


} // namespace fs





