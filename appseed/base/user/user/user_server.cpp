#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


namespace user
{


   ::user::document * server::open_new_document(::aura::application * pappOnBehalfOf)
   {

      return open_document_file(get_app(), pappOnBehalfOf, var::type_null);

   }


   void server::on_request(::create * pcreate)
   {

      sp(::create) cc(canew(::create(Application.handler(), var(var::type_empty), true, NULL)));

      if (pcreate == NULL)
      {
         pcreate = cc;
      }

      request_create(pcreate);

//      return pcreate->m_spCommandLine->m_varQuery["document"].cast < ::user::document >();

   }


   ::user::document * server::open_document_file(::aura::application * pappOnBehalfOf, var varFile, bool bMakeVisible, ::user::interaction * puiParent)
   {

      sp(::create) cc(canew(::create(pappOnBehalfOf->handler(), varFile, bMakeVisible, puiParent)));

      request_create(cc);

      return ::user::get_document(cc);

   }


   ::user::document * server::create_subdocument(::user::view_creator_data * pcreatordata)
   {

      return pcreatordata->m_pdoc = open_document_file(
                                    pcreatordata->m_pholder->get_app(),
                                    var::type_null,
                                    true,
                                    pcreatordata->m_pholder);

   }


} // namespace user



