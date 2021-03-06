#include "framework.h"


namespace userstack
{


   document::document(::aura::application * papp) :
      ::object(papp),
      ::data::data_container_base(papp),
      ::user::controller(papp),
      ::user::document(papp)
   {

   }


   bool document::on_new_document()
   {

      if (!::user::document::on_new_document())
         return FALSE;
   
      update_all_views(NULL, 0);

      return TRUE;

   }


   document::~document()
   {

   }



   void document::assert_valid() const
   {

      ::user::document::assert_valid();

   }

   void document::dump(dump_context & dumpcontext) const
   {

      ::user::document::dump(dumpcontext);

   }


} // namespace userstack



