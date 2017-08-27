#include "framework.h"


namespace userex
{


   split_view::split_view(::aura::application * papp) :
      object(papp),
      ::user::split_layout(papp),
      
      ::user::split_view(papp),
      place_holder_container(papp)
   {
      m_pviewcontainer = this;
   }
 
   split_view::~split_view()
   {
   }


   bool split_view::on_new_view_creator_data(::user::view_creator_data * pcreatordata)
   {

      ::index iPane = get_pane_by_id(pcreatordata->m_id);

      if (iPane < 0)
      {

         return false;

      }

      Pane * ppane = (Pane *) m_panea.element_at(iPane);

      pcreatordata->m_pholder = get_new_place_holder(ppane->m_rect);

      ppane->m_pholder = pcreatordata->m_pholder;

      if (ppane->m_pholder == NULL)
      {

         return false;

      }

      pcreatordata->m_pviewdata = (void *) ppane;

      return true;

   }

   bool split_view::create_pane(int32_t iPane, bool bFixedSize, ::id id)
   {

      ASSERT(iPane >= 0);

      ASSERT(iPane < get_pane_count());

      Pane & pane = m_panea(iPane);

      pane.m_bFixedSize = bFixedSize;

      pane.m_id = id;

      ::rect rectClient;

      pane.m_pholder->GetClientRect(rectClient);

      ::user::view_creator_data * pdata = ::user::view_creator::create_impact(id, rectClient);

      if (pdata == NULL)
      {

         return false;

      }

      return true;

   }


   void split_view::on_command_probe(::user::command * pcommand)
   {

   }


   void split_view::on_command(::user::command * pcommand)
   {

   }


} // namespace userex