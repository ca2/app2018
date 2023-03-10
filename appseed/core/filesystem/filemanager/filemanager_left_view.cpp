#include "framework.h"


namespace filemanager
{


   left_view::left_view(::aura::application * papp) :
      object(papp),
      ::user::split_layout(papp),
      ::user::split_view(papp),
      place_holder_container(papp)
   {

   }


   left_view::~left_view()
   {

   }


   void left_view::install_message_routing(::message::sender * psender)
   {

      ::filemanager::impact::install_message_routing(psender);
      ::user::split_view::install_message_routing(psender);

   }




   void left_view::assert_valid() const
   {
      ::user::split_view::assert_valid();
   }

   void left_view::dump(dump_context & dumpcontext) const
   {
      ::user::split_view::dump(dumpcontext);
   }



   void left_view::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
   {

      ::filemanager::impact::on_update(pSender, lHint, phint);

      ::user::split_view::on_update(pSender, lHint, phint);

      if (phint != NULL)
      {
         if (base_class < update_hint > ::bases(phint))
         {
            update_hint * puh = (update_hint *) phint;
            if (get_filemanager_manager() == puh->m_pmanager)
            {
               if (puh->is_type_of(update_hint::TypeInitialize))
               {
                  string str;
                  str.Format("frame(%d,%d)", get_filemanager_data()->m_iTemplate, get_filemanager_data()->m_iDocument);
                  sp(::user::frame_window) pframe = GetParentFrame();
                  if (pframe != NULL)
                  {
                     pframe->set_data_key_modifier(str);
                  }

               }
            }
         }
      }


   }

   void left_view::on_create_views()
   {

      //SetPaneCount(2);

      SetPaneCount(1);

      SetSplitOrientation(orientation_horizontal);

      initialize_split_layout();

      /*

      set_position_rate(0, 0.30);



      file_list * plist = create_view < file_list >();

      if (plist == NULL)
      {
         System.simple_message_box(NULL, "Could not create folder tree ::user::impact");
      }

      plist->m_bStatic = true;

      plist->m_scrolldata.m_rectMargin.null();

      SetPane(0, plist, false);

      */

      m_pusertree = create_pane_view < ::user::show < ::user::tree > > (0);

      if(m_pusertree == NULL)
      {
         System.simple_message_box(NULL, "Could not create file list ::user::impact");
      }

      m_ptree = canew(tree(get_app()));

      m_pusertree->merge(m_ptree, true);

      //SetPane(1, pusertree, false);

      //SetPane(0,m_pusertree,false);


   }


} // namespace filemanager




