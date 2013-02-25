﻿#include "framework.h"


namespace platform
{


   document::document(::ca::application * papp) :
      ca(papp),
      ::ca::data_container_base(papp),
      form_document(papp)
   {
      
      m_pbergedgedocument = NULL;

   }

   bool document::on_new_document()
   {
      if (!::userbase::document::on_new_document())
         return FALSE;

      update_all_views(NULL, 0);


      return TRUE;
   }

   document::~document()
   {
   }



   #ifdef DEBUG
   void document::assert_valid() const
   {
      ::userbase::document::assert_valid();
   }

   void document::dump(dump_context & dumpcontext) const
   {
      ::userbase::document::dump(dumpcontext);
   }
   #endif //DEBUG

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

   void document::data_on_after_change(::ca::signal_object * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

   }

   view * document::get_platform_view()
   {

      return get_typed_view < view > ();

   }

   pane_view * document::get_platform_pane_view()
   {

      return get_typed_view < pane_view > ();

   }

   frame * document::get_platform_frame()
   {
      
      pane_view * pview = get_platform_pane_view();
      
      if(pview == NULL)
         return NULL;
      
      frame * pplatformframe = pview->GetTypedParent < frame >();
      
      return pplatformframe;


   }


   ::bergedge::bergedge * document::get_bergedge()
   {

      return m_pbergedgedocument->get_bergedge();

   }


} // namespace platform



