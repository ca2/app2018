#include "framework.h"


namespace filemanager
{


   a_view::a_view(sp(base_application) papp) :
      element(papp),
      ::user::split_layout(papp),

      ::user::split_view(papp),
      place_holder_container(papp)
   {
      }

   a_view::~a_view()
   {
   }

#ifdef DEBUG
   void a_view::assert_valid() const
   {
      ::user::split_view::assert_valid();
   }

   void a_view::dump(dump_context & dumpcontext) const
   {
      ::user::split_view::dump(dumpcontext);
   }

#endif //DEBUG


   void a_view::on_update(sp(::user::view) pSender, LPARAM lHint, object* phint)
   {
      data_interface::on_update(pSender, lHint, phint);
      ::user::split_view::on_update(pSender, lHint, phint);
      if (phint != NULL)
      {
         if (base < update_hint >::bases(phint))
         {
            update_hint * puh = (update_hint *)phint;
            if (GetFileManager() == puh->m_pmanager)
            {
               if (puh->is_type_of(update_hint::TypeInitialize))
               {
                  string str;
                  str.Format("::frame(%d,%d)", GetFileManager()->get_filemanager_data()->m_iTemplate, GetFileManager()->get_filemanager_data()->m_iDocument);
                  sp(frame) pframe = ((sp(::user::window)) GetParentFrame());
                  if (pframe != NULL)
                  {
                     pframe->m_dataid = str;
                  }
               }
               else if (puh->is_type_of(update_hint::TypePop))
               {
                  OnActivateFrame(WA_INACTIVE, ((GetParentFrame())));
                  GetParentFrame()->ActivateFrame(SW_SHOW);
                  OnActivateView(TRUE, this, this);
                  RedrawWindow();
               }
               else if (puh->is_type_of(update_hint::TypeCreateBars))
               {
                  sp(frame) pframe = (GetParentFrame());
                  if (pframe != NULL)
                  {
                     ASSERT(pframe != NULL);
                     ASSERT(base < ::frame > ::bases(pframe));
                     pframe->SetActiveView(this);
                     pframe->CreateBars();
                  }
                  sp(main_frame) pmainframe = (GetTopLevelFrame());
                  if (pmainframe != NULL)
                  {
                     pmainframe->SetActiveView(this);
                     pmainframe->CreateBars();
                  }
                  sp(::filemanager::child_frame) pchildframe = (GetParentFrame());
                  if (pchildframe != NULL)
                  {
                     ASSERT(pchildframe != NULL);
                     ASSERT(base < ::filemanager::child_frame > ::bases(pchildframe));

                     pchildframe->SetActiveView(this);
                     pchildframe->CreateBars();
                  }
               }
               else if (puh->is_type_of(update_hint::TypeSaveAsStart))
               {
                  if (!base < FileManagerSaveAsView >::bases(get_pane_window(0)))
                  {
                     //create_context cc;
                     //cc.m_usercreatecontext.m_pCurrentDoc = get_document();
                     //cc.m_usercreatecontext.m_typeinfoNewView =  System.type_info < FileManagerSaveAsView > ();
                     //cc.m_usercreatecontext.m_pCurrentFrame = this;

                     FileManagerSaveAsView * ptopview = create_view < FileManagerSaveAsView >();
                     if (ptopview == NULL)
                     {
                        System.simple_message_box(NULL, "Could not create folder tree ::user::view");
                     }
                     ptopview->m_pfilemanagerinterface = GetFileManager();
                     InsertPaneAt(0, ptopview, true);
                     string strName =
                        System.file().title_(GetFileManager()->get_filemanager_data()->m_pdocumentSave->get_path_name())
                        + " - " + System.datetime().international().get_gmt_date_time()
                        + "." + System.file().extension(GetFileManager()->get_filemanager_data()->m_pdocumentSave->get_path_name());
                     strName.replace(":", "-");
                     strName = System.dir().path(GetFileManager()->get_item().m_strPath, strName);
                     ptopview->_001SetText(strName);
                     GetFileManager()->get_filemanager_data()->m_pmanager->m_strTopic = strName;
                     set_position(0, 49);
                     set_position(1, 49 + 49);
                     layout();
                  }
               }
               else if (puh->is_type_of(update_hint::TypeSaveAsCancel))
               {
                  if (base < FileManagerSaveAsView >::bases(get_pane_window(0)))
                  {
                     RemovePaneAt(0);
                     set_position(0, 49);
                     layout();
                  }
               }
               else if (puh->is_type_of(update_hint::TypeSaveAsOK))
               {
                  ASSERT(GetFileManager()->get_filemanager_data()->m_pdocumentSave != NULL);

                  string strPath = puh->m_strPath;
                  if (strPath.is_empty())
                  {
                     string strTitle;
                     dynamic_cast <FileManagerSaveAsView *>(get_pane_window(0).m_p)->_001GetText(strTitle);
                     if (System.dir().name(strTitle).has_char() && GetFileManagerDoc()->get_fs_data()->is_dir(System.dir().name(strTitle)))
                     {
                        strPath = strTitle;
                     }
                     else if (GetFileManagerDoc()->get_fs_data()->is_dir(GetFileManager()->get_item().m_strPath))
                     {
                        strPath = System.dir().path(GetFileManager()->get_item().m_strPath, strTitle);
                     }
                     else if (strTitle.has_char())
                     {
                        strPath = System.dir().path(GetFileManager()->get_item().m_strPath, strTitle);
                     }
                     else
                     {
                        strPath = GetFileManager()->get_item().m_strPath;
                     }
                  }

                  bool bSave = !Application.dir().is(strPath);

                  if (bSave && GetFileManagerDoc()->get_fs_data()->file_exists(strPath))
                  {
                     if (System.simple_message_box(Session.get_view(), "Do you want to replace the existing file " + strPath + "?", MB_YESNO) == IDNO)
                     {
                        bSave = false;
                     }
                  }

                  update_hint uh;

                  if (bSave)
                  {
                     if (GetFileManager()->get_filemanager_data()->m_pdocumentSave->do_save(strPath))
                     {
                        uh.set_type(update_hint::TypeSaveAsSaved);
                        uh.m_strPath = strPath;
                     }
                     else
                     {
                        uh.set_type(update_hint::TypeSaveAsSaveFailed);
                     }
                  }
                  else
                  {
                     uh.set_type(update_hint::TypeSaveAsCancel);
                  }
                  get_document()->update_all_views(NULL, 0, &uh);

                  GetFileManager()->get_filemanager_data()->m_pdocumentSave = NULL;

                  if (base < FileManagerSaveAsView >::bases(get_pane_window(0)))
                  {
                     RemovePaneAt(0);
                     set_position(0, 49);
                     layout();
                  }
               }
            }
         }
      }

      ::filemanager::tab_view * ptabview = GetParentFrame()->GetTypedParent < ::filemanager::tab_view >();
      if (ptabview != NULL)
      {
         ptabview->on_update(this, lHint, phint);
      }

   }

   void a_view::CreateViews()
   {
      SetPaneCount(2);

      SetSplitOrientation(orientation_horizontal);

      set_position(0, 24);



      ::path_view * ptopview = create_view < ::path_view >();
      if (ptopview == NULL)
      {
         System.simple_message_box(NULL, "Could not create folder tree ::user::view");
      }
      SetPane(0, ptopview, false);
      //ptopview->CreateViews();


      main_view * pmediaview = create_view < main_view >();

      if (pmediaview == NULL)
      {
         System.simple_message_box(NULL, "Could not create file list ::user::view");
      }
      SetPane(1, pmediaview, false);
      pmediaview->CreateViews();

   }



   main_view::main_view(sp(base_application) papp) :
      element(papp),
      ::user::split_layout(papp),

      ::user::split_view(papp),
      place_holder_container(papp)
   {
         m_ppropform = NULL;
      }

   main_view::~main_view()
   {
   }


   // BEGIN_MESSAGE_MAP(main_view, ::user::split_view)
   //{{__MSG_MAP(main_view)
   //}}__MSG_MAP
   // END_MESSAGE_MAP()

   /////////////////////////////////////////////////////////////////////////////
   // main_view diagnostics

#ifdef DEBUG
   void main_view::assert_valid() const
   {
      ::user::split_view::assert_valid();
   }

   void main_view::dump(dump_context & dumpcontext) const
   {
      ::user::split_view::dump(dumpcontext);
   }

#endif //DEBUG



   void main_view::on_update(sp(::user::view) pSender, LPARAM lHint, object* phint)
   {
      data_interface::on_update(pSender, lHint, phint);
      ::user::split_view::on_update(pSender, lHint, phint);
      if (phint != NULL)
      {
         if (base < update_hint >::bases(phint))
         {
            update_hint * puh = (update_hint *)phint;
            if (GetFileManager() == puh->m_pmanager)
            {
               if (puh->is_type_of(update_hint::TypeInitialize))
               {
                  string str;
                  str.Format("::frame(%d,%d)", GetFileManager()->get_filemanager_data()->m_iTemplate, GetFileManager()->get_filemanager_data()->m_iDocument);
                  sp(::frame) pframe = ((sp(::user::window)) GetParentFrame());
                  if (pframe != NULL)
                  {
                     pframe->m_dataid = str;
                  }
               }
               else if (puh->is_type_of(update_hint::TypeCreateViews)
                  && get_pane_count() == 0)
               {
                  CreateViews();
               }
               else if (puh->is_type_of(update_hint::TypeOpenSelectionProperties))
               {
                  OpenSelectionProperties();
               }
               else if (puh->is_type_of(update_hint::TypePop))
               {
                  OnActivateFrame(WA_INACTIVE, ((GetParentFrame())));
                  GetParentFrame()->ActivateFrame(SW_SHOW);
                  OnActivateView(TRUE, this, this);
                  RedrawWindow();
                  sp(::frame) pframe = ((sp(::user::window)) GetParentFrame());
                  if (pframe != NULL)
                  {
                     //xxx               pframe->WindowDataLoadWindowRect();
                     //xxx          pframe->WindowDataEnableSaveWindowRect(true);
                  }
               }
               else if (puh->is_type_of(update_hint::TypeCreateBars))
               {
                  sp(::frame) pframe = ((sp(::user::window)) GetParentFrame());
                  if (pframe != NULL)
                  {
                     ASSERT(pframe != NULL);
                     ASSERT(base < ::frame > ::bases(pframe));

                     pframe->CreateBars();
                  }
                  sp(main_frame) pmainframe = (GetTopLevelFrame());
                  if (pmainframe != NULL)
                  {
                     pmainframe->CreateBars();
                  }
                  sp(::filemanager::child_frame) pchildframe = ((sp(::user::window)) GetParentFrame());
                  if (pchildframe != NULL)
                  {
                     ASSERT(pchildframe != NULL);
                     ASSERT(base < ::filemanager::child_frame > ::bases(pchildframe));


                     pchildframe->CreateBars();
                  }
               }
            }
         }
      }


   }

   void main_view::CreateViews()
   {
      SetPaneCount(2);

      SetSplitOrientation(orientation_vertical);

      set_position_rate(0, 0.3);



      ::filemanager::left_view * pleftview = create_view < ::filemanager::left_view >();

      if (pleftview == NULL)
      {
         System.simple_message_box(NULL, "Could not create folder tree ::user::view");
      }
      SetPane(0, pleftview, false);
      pleftview->CreateViews();

      m_pfilelist = create_view < ::filemanager::file_list >();

      if (m_pfilelist == NULL)
      {
         System.simple_message_box(NULL, "Could not create file list ::user::view");
      }
      SetPane(1, m_pfilelist, false);

      m_ppreview = create_view < ::filemanager::preview >();
      m_ppreview->ShowWindow(SW_HIDE);
   }


   void main_view::OpenSelectionProperties()
   {
      ::fs::item_array itema;
      m_pfilelist->GetSelected(itema);
      if (m_ppropform == NULL)
      {
         m_ppropform = new ::filemanager::file_properties_form(get_app());
      }
      sp(::user::interaction) puie = m_ppropform->open(this, itema);
      if (puie == NULL)
         return;
      SetPane(1, puie, false);
      layout();
   }

   bool main_view::_001OnCommand(id id)
   {
      //  int32_t iPos = -1;
      if (id == "change_view")
      {
         if (m_ppreview->IsWindowVisible())
         {
            SetPane(1, m_pfilelist, false);
            layout();
            m_ppreview->ShowWindow(SW_HIDE);
         }
         else
         {
            SetPane(1, m_ppreview, false);
            layout();
            m_pfilelist->ShowWindow(SW_HIDE);
         }
         return true;
      }
      return false;
   }



} // namespace filemanager



