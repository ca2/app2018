//#include "framework.h"


namespace filemanager
{


   operation_document::operation_document(::aura::application * papp):
      object(papp),
      ::data::data_container_base(papp),
      ::user::document(papp),
      m_thread(papp)
   {

      }

   bool operation_document::on_new_document()
   {
      return TRUE;
   }


} // namespace filemanager



























