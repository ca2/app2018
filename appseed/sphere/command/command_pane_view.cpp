#include "framework.h"


namespace prompt
{


   pane_view::pane_view(::aura::application * papp) :
      ::object(papp),
      ::user::tab_view(papp),
      ::userex::pane_tab_view(papp),
      place_holder_container(papp)
   {


   }


   pane_view::~pane_view()
   {

   }



   void pane_view::assert_valid() const
   {
      ::user::impact::assert_valid();
   }

   void pane_view::dump(dump_context & dumpcontext) const
   {
      ::user::impact::dump(dumpcontext);
   }


   /////////////////////////////////////////////////////////////////////////////
   // pane_view message handlers

   void pane_view::_001OnCreate(::message::message * pobj)
   {
//      SCAST_PTR(::message::create, pcreate, pobj);
      if(pobj->previous())
         return;

      add_tab("3-action-launch", prompt::pane_view_three_action_launch);
      add_tab("menu", prompt::pane_view_context_menu);
      add_tab("primary verbing", prompt::PaneViewPrimaryCommand);
      add_tab("options", prompt::pane_view_configuration);
      add_tab("File Manager", "file_manager");

      set_cur_tab_by_id(prompt::PaneViewPrimaryCommand);

   }

   void pane_view::on_update(::user::impact * pSender, LPARAM lHint, ::object* pHint)
   {
      ::user::tab_view::on_update(pSender, lHint, pHint);
      if(lHint == 543218)
      {
         set_cur_tab_by_id(prompt::PaneViewPrimaryCommand);
      }
      if(pHint != NULL)
      {
         if(base_class <pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint *) pHint;
            if(puh->is_type_of(pane_view_update_hint::type_get_view))
            {
            }
            else if(puh->is_type_of(pane_view_update_hint::type_set_view))
            {
               set_cur_tab_by_id(puh->m_eview);
            }
         }
      }
      if(pHint != NULL)
      {
         if(base_class <pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint * ) pHint;
            if(puh->is_type_of(pane_view_update_hint::TypeOnShowKaraoke))
            {
               ( (GetTopLevelFrame()))->SetActiveView(this);
            }
            else if(puh->is_type_of(pane_view_update_hint::TypeOnShowView))
            {

            }
         }
      }


   }

   void pane_view::on_show_view()
   {
      ::userex::pane_tab_view::on_show_view();
      sp(frame) pframe = GetTypedParent < frame > ();

      if(get_view_id() == "file_manager")
      {
         pframe->m_bAutoHideOnOutClick = false;
         pframe->ShowWindow(SW_MAXIMIZE);
      }
      else if(get_view_id() == prompt::pane_view_context_menu)
      {
         sp(::filemanager::manager) pdoc =  (get_view_uie());
         pdoc->FileManagerBrowse(System.dir().appdata()/ "command/menu", ::action::source::system_default());
      }
      else
      {
         pframe->m_bAutoHideOnOutClick = true;
         //pframe->OnHoverAction(false);
      }
   }


   void pane_view::_001OnSize(::message::message * pobj)
   {
      pobj->previous();

   }


   bool pane_view::pre_create_window(::user::create_struct& cs)
   {
      cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

      return ::user::impact::pre_create_window(cs);
   }


   void pane_view::on_create_view(::user::view_creator_data * pcreatordata)
   {
      switch(pcreatordata->m_id)
      {
      case pane_view_context_menu:
      {
         sp(::filemanager::manager) pdoc = Session.filemanager()->open_child_list(false, true);
         if(pdoc != NULL)
         {
            pdoc->get_filemanager_data()->m_iIconSize = 16;
            pdoc->get_filemanager_data()->m_bListText = true;
            pdoc->get_filemanager_data()->m_bListSelection = false;
            pdoc->get_filemanager_template()->m_strFilePopup = "filemanager\\file_popup.xml";
            pdoc->get_filemanager_data()->m_datakey = "winactionarea_menu";
            pdoc->get_filemanager_data()->m_pcallback = this;
            pdoc->Initialize(true);
            pdoc->update_all_views(NULL, 1234);
            pdoc->update_all_views(NULL, 123458);
            sp(::user::impact) pview = pdoc->get_view();
            pdoc->FileManagerBrowse(System.dir().appdata()/ "command\\menu", ::action::source::system_default());
            if(pview != NULL)
            {
               sp(::user::frame_window) pframe =  (pview->GetParentFrame());
               if(pframe != NULL)
               {
#ifdef WINDOWSEX
                  pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
#endif
                  pframe->SetParent(this);
                  pcreatordata->m_pdoc = pdoc;
                  pcreatordata->m_pwnd = pframe;
               }
            }
         }
      }
      break;
      case PaneViewPrimaryCommand:
      {
         sp(::user::impact) pview = create_view < primary_view > ();
         if(pview != NULL)
         {
            pcreatordata->m_pdoc = get_document();
            pcreatordata->m_pwnd = pview;
            pcreatordata->m_iExtendOnParent = 0;
         }
      }
      break;
//      case "file_manager":
//      {
//         sp(::filemanager::manager) pdoc = Session.filemanager()->open_child(false, true);
//         if(pdoc != NULL)
//         {
//            pdoc->get_filemanager_data()->m_datakey = "winactionarea_filemanager";
//            pdoc->Initialize(true);
//            pdoc->update_all_views(NULL, 1234);
//            pdoc->update_all_views(NULL, 123458);
//            sp(::user::impact) pview = pdoc->get_view();
//            if(pview != NULL)
//            {
//               sp(::user::frame_window) pframe =  (pview->GetParentFrame());
//               if(pframe != NULL)
//               {
//#ifdef WINDOWSEX
//                  pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
//#endif
//                  pframe->SetParent(this);
//                  pcreatordata->m_pdoc = pdoc;
//                  pcreatordata->m_pwnd = pframe;
//
//
//               }
//            }
//         }
//      }
//      break;
      case pane_view_three_action_launch:
      {
         sp(::filemanager::manager) pdoc = Session.filemanager()->open_child_list(false, true);
         if(pdoc != NULL)
         {
            pdoc->get_filemanager_data()->m_iIconSize = 48;
            pdoc->get_filemanager_data()->m_bListText = false;
            pdoc->get_filemanager_data()->m_bListSelection = false;
            pdoc->get_filemanager_data()->m_pcallback = this;
            pdoc->get_filemanager_data()->m_datakey = "winactionarea_3-action-launch";
            pdoc->Initialize(true);
            pdoc->update_all_views(NULL, 1234);
            pdoc->update_all_views(NULL, 123458);
            sp(::user::impact) pview = pdoc->get_view();

            pdoc->FileManagerBrowse(System.dir().appdata()/ "command\\3-action-launch", ::action::source::system_default());

            if(pview != NULL)
            {
               sp(::user::frame_window) pframe =  (pview->GetParentFrame());
               if(pframe != NULL)
               {
#ifdef WINDOWSEX
                  pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
#endif
                  pframe->SetParent(this);
                  pcreatordata->m_pdoc = pdoc;
                  pcreatordata->m_pwnd = pframe;


               }
            }
         }
      }
      break;
      case pane_view_configuration:
      {
         sp(::user::document) pdoc = Application.create_form(this, this);
         if(pdoc == NULL)
            return;
         ::user::view_creator_data * pcreatordata = new ::user::view_creator_data;
         sp(::user::impact) pview = pdoc->get_typed_view < ::user::impact > ();
         ::user::form_update_hint uh;
         uh.m_actioncontext = ::action::source::system_default();
         uh.m_etype = ::user::form_update_hint::type_browse;
         uh.m_strForm = "filemanager\\replace_name_in_file_system.xhtml";
         pdoc->update_all_views(NULL, 0, &uh);

         uh.m_etype = ::user::form_update_hint::type_get_form_view;
         pdoc->update_all_views(NULL, 0, &uh);

         uh.m_etype = ::user::form_update_hint::type_after_browse;
         pdoc->update_all_views(NULL, 0, &uh);


         pcreatordata->m_pwnd = (pview->GetParentFrame());
//         sp(form_child_frame) pframe = (pcreatordata->m_pwnd);
         //pframe->m_iTabId = iId;
         pcreatordata->m_pdoc = pdoc;
      }
      break;
      default:
         ASSERT(FALSE);
         break;
      }

      pane_tab_view::on_create_view(pcreatordata);

   }


   void pane_view::_001OnMenuMessage(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      set_cur_tab_by_id(m_pviewdataOld->m_id);

   }


   void pane_view::install_message_routing(::message::sender * pinterface)
   {

      ::user::impact::install_message_routing(pinterface);

      IGUI_MSG_LINK(WM_CREATE, pinterface, this, &pane_view::_001OnCreate);
      IGUI_MSG_LINK(WM_SIZE, pinterface, this, &pane_view::_001OnSize);
      IGUI_MSG_LINK(WM_USER + 1122, this, this, &pane_view::_001OnMenuMessage);

   }


   void pane_view::rotate()
   {

      id idNew;

      if (get_view_id() == "file_manager")
      {

         idNew = prompt::pane_view_context_menu;

      }
      else
      {

         switch (get_view_id())
         {
         case prompt::pane_view_context_menu:
            idNew = prompt::PaneViewPrimaryCommand;
            break;
         case prompt::PaneViewPrimaryCommand:
            idNew = "file_manager";
            break;
         default:
            idNew = prompt::PaneViewPrimaryCommand;
         }

      }

      set_cur_tab_by_id(idNew);

   }


   void pane_view::on_control_event(::user::control_event * pevent)
   {

      ::prompt::form_callback::on_control_event(pevent);

      if(pevent->m_bRet)
      {

         return;

      }

      ::userex::pane_tab_view::on_control_event(pevent);

   }


   void pane_view::OnFileManagerOpenFile(::filemanager::data * pdata, ::fs::item_array & itema)
   {

      UNREFERENCED_PARAMETER(pdata);

      if(itema.get_size() > 0)
      {

         int_ptr i = 0;

#ifdef WINDOWSEX

         i = (int_ptr) ::ShellExecuteW(
             GetTopLevel()->get_handle(),
             NULL,
             ::str::international::utf8_to_unicode("\"" + itema[0]->m_filepath + "\""),
             NULL,
             ::str::international::utf8_to_unicode("\"" +  itema[0]->m_filepath.folder() + "\""),
             SW_SHOWNORMAL);

#else

         //_throw(todo(get_app()));

#endif

         string str;

         str.Format("%d", i);

         //Application.simple_message_box(str);

         if(i == ERROR_FILE_NOT_FOUND)
         {

         }

      }

      GetParentFrame()->ShowWindow(SW_HIDE);

   }


} // namespace prompt


