//#include "framework.h"


namespace filemanager
{


   view::view(::aura::application * papp) :
      object(papp),
      ::user::split_layout(papp),
      ::user::split_view(papp),
      place_holder_container(papp)
   {
   }

   view::~view()
   {
   }

   void view::assert_valid() const
   {
      ::user::split_view::assert_valid();
   }

   void view::dump(dump_context & dumpcontext) const
   {
      ::user::split_view::dump(dumpcontext);
   }



   void view::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
   {
      ::filemanager::impact::on_update(pSender, lHint, phint);
      ::user::split_view::on_update(pSender, lHint, phint);
      if (phint != NULL)
      {
         if (base_class < update_hint >::bases(phint))
         {
            update_hint * puh = (update_hint *)phint;
            if (get_filemanager_manager() == puh->m_pmanager)
            {
               if (puh->is_type_of(update_hint::TypeInitialize))
               {
                  string str;
                  str.Format("::frame(%d,%d)", get_filemanager_data()->m_iTemplate, get_filemanager_data()->m_iDocument);
                  sp(frame) pframe = ((::window_sp) GetParentFrame());
                  if (pframe != NULL)
                  {
                     pframe->m_dataid = str;
                  }
                  m_puserschema = get_filemanager_data();

               }
               else if (puh->is_type_of(update_hint::TypePop))
               {

                  OnActivateFrame(WA_INACTIVE, ((GetParentFrame())));

                  sp(::user::frame_window) spframewindow = GetParentFrame();

                  if (spframewindow.is_set())
                  {

                     spframewindow->ActivateFrame(SW_SHOW);

                  }

                  OnActivateView(TRUE, this, this);

                  RedrawWindow();

               }
               else if(puh->is_type_of(update_hint::TypeCreateBars))
               {

                  //sp(simple_frame_window) pframe = (GetParentFrame());

                  //if(pframe != NULL)
                  {

                    // pframe->create_bars();

                  }

               }
               else if(puh->is_type_of(update_hint::TypeTopicStart))
               {
                  if (get_filemanager_manager()->m_emode != manager::mode_import
                     && !base_class < ::filemanager::save_as_view >::bases(get_pane_window(0)))
                  {
                     //create_context cc;
                     //cc.m_usercreatecontext.m_pCurrentDoc = get_document();
                     //cc.m_usercreatecontext.m_typeinfoNewView =  System.type_info < ::filemanager::save_as_view > ();
                     //cc.m_usercreatecontext.m_pCurrentFrame = this;

                     ::filemanager::save_as_view * ptopview = create_view < ::filemanager::save_as_view >();
                     if (ptopview == NULL)
                     {
                        System.simple_message_box(NULL, "Could not create folder tree ::user::impact");
                     }
                     ptopview->m_pmanager = get_filemanager_manager();
                     InsertPaneAt(0, ptopview, true);
                     ::file::path path = get_filemanager_data()->m_pdocumentTopic->get_file_path();
                     string strName = path.title() + " - " + System.datetime().international().get_gmt_date_time() + "." + path.ext();
                     strName.replace(":", "-");
                     strName = get_filemanager_item().m_filepath / strName;
                     ptopview->_001SetText(strName, puh->m_actioncontext);
                     get_filemanager_data()->m_pmanager->m_strTopic = strName;
                     set_position(0, 49);
                     set_position(1, 49 + 49);
                     on_layout();
                  }
                  sp(simple_frame_window) pframe = (GetParentFrame());

                  if(pframe != NULL)
                  {

                      pframe->create_bars();

                  }

               }
               else if (puh->is_type_of(update_hint::TypeTopicCancel))
               {
                  if (base_class < ::filemanager::save_as_view >::bases(get_pane_window(0)))
                  {
                     RemovePaneAt(0);
                     set_position(0, 49);
                     on_layout();
                  }
               }
               else if (puh->is_type_of(update_hint::TypeTopicOK))
               {


                  ASSERT(get_filemanager_data()->m_pdocumentTopic != NULL);

                  string strPath = puh->m_filepath;

                  if (strPath.is_empty())
                  {
                     
                     ::file::path strTitle;

                     dynamic_cast <::filemanager::save_as_view *>(get_pane_window(0).m_p)->_001GetText(strTitle);

                     if (strTitle.folder().has_char() && get_filemanager_manager()->get_fs_data()->is_dir(strTitle.folder()))
                     {

                        strPath = strTitle;

                     }
                     else if (get_filemanager_manager()->get_fs_data()->is_dir(get_filemanager_item().m_filepath))
                     {

                        strPath = get_filemanager_item().m_filepath / strTitle;

                     }
                     else if (strTitle.has_char())
                     {

                        strPath = get_filemanager_item().m_filepath / strTitle;

                     }
                     else
                     {

                        strPath = get_filemanager_item().m_filepath;

                     }

                  }

                  if (get_filemanager_manager()->m_emode == manager::mode_import)
                  {

                     get_filemanager_data()->m_pdocumentTopic->open_document(strPath);

                  }
                  else
                  {

                     bool bSave = !Application.dir().is(strPath);

                     if (bSave && get_filemanager_manager()->get_fs_data()->file_exists(strPath))
                     {
                        if (System.simple_message_box(Session.get_view(), "Do you want to replace the existing file " + strPath + "?", MB_YESNO) == IDNO)
                        {
                           bSave = false;
                        }
                     }

                     update_hint uh;

                     if (bSave)
                     {
                        if (get_filemanager_data()->m_pdocumentTopic->do_save(strPath))
                        {
                           uh.set_type(update_hint::TypeTopicSaved);
                           uh.m_filepath = strPath;
                        }
                        else
                        {
                           uh.set_type(update_hint::TypeTopicSaveFailed);
                        }
                     }
                     else
                     {
                        uh.set_type(update_hint::TypeTopicCancel);
                     }
                     get_document()->update_all_views(NULL, 0, &uh);

                     get_filemanager_data()->m_pdocumentTopic = NULL;

                     if (base_class < ::filemanager::save_as_view >::bases(get_pane_window(0)))
                     {
                        RemovePaneAt(0);
                        set_position(0, 49);
                        on_layout();
                     }

                  }
               }
            }
         }
      }

      tab_view * ptabview = GetParentFrame()->GetTypedParent < tab_view >();
      if (ptabview != NULL)
      {
         ptabview->on_update(this, lHint, phint);
      }

   }

   void view::on_create_views()
   {

      if (get_pane_count() > 0)
         return;

      bool bPathView = !Application.command_thread()->m_varTopicQuery.has_property("no_path_view");

      SetPaneCount(bPathView ? 2 : 1);

      SetSplitOrientation(orientation_horizontal);

      if (bPathView)
      {

         set_position(0, 24);

         initialize_split_layout();

         path_view * ppathview = create_pane_view < path_view >(0);

         if (ppathview == NULL)
         {

            System.simple_message_box(NULL, "Could not create filemanager path view");

         }

      }
      else
      {

         initialize_split_layout();

      }


      main_view * pmainview = create_pane_view < main_view >(bPathView ? 1 : 0);

      if (pmainview == NULL)
      {

         System.simple_message_box(NULL, "Could not create file list ::user::impact");

      }

      //SetPane(, pmainview, false);

      //pmainview->create_views();

   }


} // namespace filemanager



