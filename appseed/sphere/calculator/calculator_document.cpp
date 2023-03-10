#include "framework.h"


namespace calculator
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
   
   /////////////////////////////////////////////////////////////////////////////
   // document serialization

   /*void document::Serialize(CArchive& ar)
   {
      if (ar.IsStoring())
      {
         // TODO: add storing code here
      }
      else
      {
         // TODO: add loading code here
      }
   }
   */

   void document::data_on_after_change(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   bool document::on_open_document(var varFile)
   {


      string str = Application.file().as_string(varFile);
      update_all_views(NULL, 123);
      return TRUE;


   }


} // namespace calculator



