#include "framework.h"


namespace userex
{


   pane_tab_view::pane_tab_view(::aura::application * papp):
      object(papp),
      ::user::tab_view(papp),
      place_holder_container(papp)
   {

      m_pcolorview = NULL;

      m_pfilemanager = NULL;
      m_pfilemanagerTabbed = NULL;

      if (Application.m_pmainpane == NULL)
      {

         Application.m_pmainpane = this;

      }

      set_view_creator(this);

   }


   pane_tab_view::~pane_tab_view()
   {

   }


   ::user::interaction * pane_tab_view::get_view_uie()
   {

      return ::user::tab_view::get_view_uie();

   }


   id pane_tab_view::get_view_id()
   {

      return ::user::tab_view::get_view_id();

   }


   void pane_tab_view::GetTabClientRect(LPRECT lprect)
   {

      if(m_pviewdata == NULL || m_pviewdata->m_pwnd == NULL || m_pviewdata->m_iExtendOnParent == 0)
      {

         ::user::tab_view::GetTabClientRect(lprect);

      }
      else if(m_pviewdata->m_iExtendOnParent > 0)
      {

         int32_t i = m_pviewdata->m_iExtendOnParent;

         sp(::user::interaction) pui = m_pviewdata->m_pwnd;

         sp(::user::interaction) puiNext = m_pviewdata->m_pwnd;

         for(; i > 0; i--)
         {

            puiNext = pui->GetParent();

            if (puiNext == NULL || !puiNext->IsWindow())
            {

               break;

            }

            pui = puiNext;

         }

         pui->GetWindowRect(lprect);

         ScreenToClient(lprect);

      }
      else if(m_pviewdata->m_iExtendOnParent < 0)
      {

         sp(::user::interaction) pui = m_pviewdata->m_pwnd;

         sp(::user::interaction) puiNext = m_pviewdata->m_pwnd;

         user::interaction_spa wnda(get_app());

         while(true)
         {

            wnda.add(pui);

            puiNext = pui->GetParent();

            if (puiNext == NULL || (pui->m_pimpl) != NULL)
            {

               break;

            }

            pui = puiNext;

         }

         pui = wnda[wnda.get_size() + m_pviewdata->m_iExtendOnParent];

         pui->GetWindowRect(lprect);

         ScreenToClient(lprect);

      }

   }


   void pane_tab_view::install_message_routing(::message::sender * psender)
   {

      ::user::tab_view::install_message_routing(psender);

      IGUI_MSG_LINK(WM_CREATE, psender, this, &pane_tab_view::_001OnCreate);

   }


   void pane_tab_view::_001OnCreate(::message::message * pobj)
   {

      if (pobj->previous())
         return;


      sp(simple_frame_window) pframe = GetParent();

      if (pframe.is_set())
      {

         string strAppOptions = pframe->m_varFrame["app_options"]["resource"];

         if (strAppOptions.has_char())
         {

            string strTitle = pframe->m_varFrame["app_options"]["title"];

            add_tab(strTitle, "app_options");

         }

      }

   }


   void pane_tab_view::defer_hide_tab_with_id_that_contains_preserve_current(string strIdPart)
   {

      if (::str::begins_ci(m_pviewdata->m_id, strIdPart))
      {

         hide_tab_with_id_that_contains_preserve_current(strIdPart);

      }

   }


   void pane_tab_view::hide_tab_with_id_that_contains_preserve_current(string strIdPart)
   {

      for (auto & p : m_viewmap)
      {

         if (p.m_element2 == m_pviewdata || !p.m_element2->m_id.is_text())
         {

            continue;

         }

         if (::str::find_ci(p.m_element2->m_id.m_psz, strIdPart) >= 0)
         {

            p.m_element2->m_pholder->ShowWindow(SW_HIDE);

         }

      }

   }

   void pane_tab_view::on_show_view()
   {
      ::user::tab_view::on_show_view();
      ::user::view_creator::on_show_view();
      if(m_pviewdataOld != NULL && ::str::begins_ci(m_pviewdataOld->m_id, "file_manager"))
      {

         if(GetParentFrame()->ContinueModal())
         {

            GetParentFrame()->EndModalLoop("yes");

         }

         if(is_set_ref(filemanager_manager()))
         {

            filemanager_manager().get_filemanager_data()->m_pdocumentTopic = NULL;

         }

      }

      if (m_pviewdata != NULL)
      {

         defer_hide_tab_with_id_that_contains_preserve_current("file_manager");

         defer_hide_tab_with_id_that_contains_preserve_current("font_sel");

         defer_hide_tab_with_id_that_contains_preserve_current("color_sel");

      }

   }


   bool pane_tab_view::on_prepare_view_creator_data(::user::view_creator_data * pcreatordata)
   {

      pcreatordata->m_pholder = get_new_place_holder(get_data()->m_rectTabClient);

      if (pcreatordata->m_pholder == NULL)
      {

         return false;

      }

      return true;

   }


   bool pane_tab_view::on_after_create_view_creator_data(::user::view_creator_data * pcreatordata)
   {

      ::index iPane = id_pane(pcreatordata->m_id);

      if (iPane < 0)
      {

         keep_bool keepDisableSavingRestorableTabs(&m_bDisableSavingRestorableTabs, true, m_bDisableSavingRestorableTabs, true);

         if (!add_tab("", pcreatordata->m_id, true, false, pcreatordata->m_pholder))
         {

            return false;

         }

         iPane = id_pane(pcreatordata->m_id);

         if (iPane < 0)
         {

            return false;

         }

      }

      ::user::tab_pane * ppane = (::user::tab_pane *)get_data()->m_panea.element_at(iPane);

      if (ppane == NULL)
      {

         return false;

      }

      ppane->m_pholder = pcreatordata->m_pholder;

      if (ppane->m_pholder == NULL)
      {

         return false;

      }

      pcreatordata->m_pviewdata = (void *)ppane;

      if (matches_restorable_tab(pcreatordata->m_id, pcreatordata->m_pholder))
      {

         save_restorable_tabs();

      }

      return true;

   }


   bool pane_tab_view::on_hold(::user::interaction * pui,::user::place_holder * pholder)
   {

      if (!::user::place_holder_container::on_hold(pui, pholder))
      {

         return false;

      }

      ::user::tab_pane_array & panea = get_data()->m_panea;

      for(int32_t iTab = 0; iTab < panea.get_count(); iTab++)
      {

         if(panea[iTab]->m_pholder == pholder)
         {

            ::user::view_creator_data * pcreatordata = get_impact(panea[iTab]->m_id,get_data()->m_rectTabClient, false);

            if(pcreatordata != NULL)
            {

               if(pcreatordata->m_pwnd == NULL)
               {

                  pcreatordata->m_pwnd = pui;

               }

            }

            break;

         }

      }

      return true;

   }


   ::index pane_tab_view::create_tab_by_id(id id)
   {

      ::user::view_creator_data * pcreatordata = get_impact(id, get_data()->m_rectTabClient);

      if(pcreatordata == NULL)
      {

         return -1;

      }

      index iTab = id_tab(pcreatordata->m_id);

      if (iTab < 0)
      {

         return -1;

      }

      return iTab;

   }


   void pane_tab_view::on_create_view(::user::view_creator_data * pcreatordata)
   {

      ::aura::library * plibrary = NULL;

      if(pcreatordata->m_id.is_text() && System.m_idmapCreateViewLibrary.Lookup(pcreatordata->m_id,plibrary) && plibrary != NULL)
      {


         //plibrary->on_create_view(pcreatordata);

      }
      else if (pcreatordata->m_id == "fontopus")
      {

         sp(::fontopus::view) pview = create_view < ::fontopus::view >();

         if (pview.is_set())
         {

            pcreatordata->m_pdoc = get_document();

            pcreatordata->m_pwnd = pview;

            pcreatordata->m_iExtendOnParent = 0;

         }

      }
      else if (::str::begins_ci(pcreatordata->m_id, "font_sel"))
      {

         Session.will_use_view_hint("font_sel");

         auto pdoc = Session.userex()->m_mapimpactsystem["font_sel"]->open_document_file(get_app(), ::var::type_null, false, pcreatordata->m_pholder);

         m_pfontview = pdoc->get_typed_view < font_view >();

         pdoc->m_pviewTopic->m_puiViewNotify = this;

         pcreatordata->m_pwnd = pdoc->m_pviewTopic;

      }
      else if (::str::begins_ci(pcreatordata->m_id, "color_sel"))
      {

         Session.will_use_view_hint("color_sel");

         auto pdoc = Session.userex()->m_mapimpactsystem["color_sel"]->open_document_file(get_app(), ::var::type_null, false, pcreatordata->m_pholder);

         m_pcolorview = pdoc->get_typed_view < color_view >();

         pdoc->m_pviewTopic->m_puiViewNotify = this;

         pcreatordata->m_pwnd = pdoc->m_pviewTopic;

      }
      else if(::str::begins_ci(pcreatordata->m_id, "file_manager")
              || pcreatordata->m_id == "left_file"
              || pcreatordata->m_id == "right_file"
              || pcreatordata->m_id == "pick_file")
      {

         ::filemanager::data * pfilemanagerdata = oprop("data." + pcreatordata->m_id.str()).cast < ::filemanager::data >();

         if(pfilemanagerdata == NULL)
            pfilemanagerdata = canew(::filemanager::data(get_app()));

         if (oprop("filemanager_icon_size").i32() > 0)
         {

            pfilemanagerdata->m_iIconSize = oprop("filemanager_icon_size").i32();

         }


         pfilemanagerdata->m_id = pcreatordata->m_id;

         if (m_psetObject != NULL
               && m_psetObject->has_property("filemanager_toolbar")
               && m_psetObject->operator[]("filemanager_toolbar").m_element2.m_etype == ::var::type_propset)
         {

            property_set & set = m_psetObject->operator[]("filemanager_toolbar").m_element2.propset();

            if (set[::filemanager::manager::mode_normal].is_set())
               pfilemanagerdata->m_setToolbar[::filemanager::manager::mode_normal] = set[::filemanager::manager::mode_normal];
            else
               pfilemanagerdata->m_setToolbar[::filemanager::manager::mode_normal] = "filemanager_toolbar.xml";

            if (set[::filemanager::manager::mode_saving].is_set())
               pfilemanagerdata->m_setToolbar[::filemanager::manager::mode_saving] = set[::filemanager::manager::mode_saving];
            else
               pfilemanagerdata->m_setToolbar[::filemanager::manager::mode_saving] = "filemanager_saving_toolbar.xml";

            if (set[::filemanager::manager::mode_import].is_set())
               pfilemanagerdata->m_setToolbar[::filemanager::manager::mode_import] = set[::filemanager::manager::mode_import];
            else
               pfilemanagerdata->m_setToolbar[::filemanager::manager::mode_import] = "filemanager_import_toolbar.xml";

            if (set[::filemanager::manager::mode_export].is_set())
               pfilemanagerdata->m_setToolbar[::filemanager::manager::mode_export] = set[::filemanager::manager::mode_export];
            else
               pfilemanagerdata->m_setToolbar[::filemanager::manager::mode_export] = "filemanager_export_toolbar.xml";


         }
         else
         {

            pfilemanagerdata->m_setToolbar[::filemanager::manager::mode_normal] = "filemanager_toolbar.xml";
            pfilemanagerdata->m_setToolbar[::filemanager::manager::mode_saving] = "filemanager_saving_toolbar.xml";
            pfilemanagerdata->m_setToolbar[::filemanager::manager::mode_import] = "filemanager_import_toolbar.xml";
            pfilemanagerdata->m_setToolbar[::filemanager::manager::mode_export] = "filemanager_export_toolbar.xml";

         }

         sp(::filemanager::manager) pmanager = Session.filemanager()->open_child(true,pfilemanagerdata->m_bTransparentBackground,pcreatordata->m_pholder,pfilemanagerdata,&App(get_app()));

         if(pmanager != NULL)
         {

            m_pfilemanager = pmanager;

            sp(::user::impact) pview = pmanager->get_view();

            if(pview != NULL)
            {

               sp(::user::frame_window) pframe = (sp(::user::frame_window)) pview->GetParentFrame();

               if(pframe != NULL)
               {

                  pcreatordata->m_pdoc = pmanager;

               }

            }

         }

      }
      else if(pcreatordata->m_id == "tabbed_file_manager")
      {

         sp(::create) cc(allocer());
         cc->m_bTransparentBackground     = true;
         cc->m_bMakeVisible               = true;
         cc->m_puiParent                  = pcreatordata->m_pholder;

         sp(::filemanager::manager) pmanager = Session.filemanager()->open(get_app(), -1, cc);

         if(pmanager != NULL)
         {

            m_pfilemanagerTabbed = pmanager;

            sp(::user::impact) pview = pmanager->get_view();

            if(pview != NULL)
            {

               sp(::user::frame_window) pframe = (sp(::user::frame_window)) pview->GetParentFrame();

               if(pframe != NULL)
               {

                  pcreatordata->m_pdoc = pmanager;

                  pcreatordata->m_pwnd = pframe;

               }

            }

         }

      }
      else if (pcreatordata->m_id == "app_options")
      {

         create_app_options(pcreatordata);

      }
      else if (pcreatordata->m_id.is_text())
      {

         if (::str::begins_ci(pcreatordata->m_id.m_psz, "form_"))
         {

            sp(html_document) pdocument = Application.create_child_form(this, pcreatordata->m_pholder);

            if (pdocument.is_set())
            {

               m_mapForm[pcreatordata->m_id] = pdocument;

               pdocument->m_id = string("document.") + string(pcreatordata->m_id);

               ::user::impact * pview = pdocument->get_view(0);

               pcreatordata->m_pwnd = pview->GetParentFrame();

               prepare_form(pcreatordata->m_id, pdocument);

            }

         }

      }

   }


   ::filemanager::manager & pane_tab_view::filemanager_manager()
   {

      return  *(m_pfilemanager == NULL ? NULL : dynamic_cast < ::filemanager::manager * > (m_pfilemanager));

   }

   ::filemanager::manager & pane_tab_view::tabbed_filemanager_manager()
   {

      return  *(m_pfilemanagerTabbed == NULL ? NULL : dynamic_cast < ::filemanager::manager * > (m_pfilemanagerTabbed));

   }


   void pane_tab_view::FileManagerSaveAs(::user::document * pdocument)
   {

      set_cur_tab_by_id("file_manager");
      filemanager_manager().FileManagerSaveAs(pdocument);

   }


   void pane_tab_view::TabbedFileManagerSaveAs(::user::document * pdocument)
   {

      set_cur_tab_by_id("tabbed_file_manager");
      tabbed_filemanager_manager().FileManagerSaveAs(pdocument);

   }


   void pane_tab_view::_001OnTabClose(int32_t iTab)
   {

      ::user::tab::_001OnTabClose(iTab);

      if(GetParentFrame()->ContinueModal() && is_set_ref(filemanager_manager())
            && filemanager_manager().get_filemanager_data()->m_pdocumentTopic!= NULL)
      {

         GetParentFrame()->EndModalLoop("yes");

      }

   }


   string pane_tab_view::get_cred(const string & strRequestUrl, const RECT & rect, string & strUsername, string & strPassword, string strToken, string strTitle, bool bInteractive)
   {

      set_cur_tab_by_id("fontopus");

      sp(::fontopus::view) pview = get_view();

      if (pview.is_null())
      {

         return "failed";

      }

      return pview->get_cred(strRequestUrl, rect, strUsername, strPassword, strToken, strTitle, bInteractive);

   }


   void pane_tab_view::on_update(::user::impact * pSender, LPARAM lHint, ::object* pHint)
   {

      ::user::tab_view::on_update(pSender, lHint, pHint);

   }


   void pane_tab_view::_001OnNcDraw(::draw2d::graphics * pgraphics)
   {


      ::user::tab_view::_001OnNcDraw(pgraphics);


   }

   void pane_tab_view::on_control_event(::user::control_event * pevent)
   {

      if (pevent->m_eevent == ::user::event_context_menu_close)
      {

         if (m_pviewdataOld != NULL)
         {

            set_cur_tab_by_id(m_pviewdataOld->m_id);

            pevent->m_bRet = true;

            return;

         }

      }

      ::user::tab_view::on_control_event(pevent);

   }


   bool pane_tab_view::create_app_options(::user::view_creator_data * pcreatordata)
   {

      m_pdocAppOptions = Application.create_child_form(this, pcreatordata->m_pholder);

      sp(html_document) pdoc = m_pdocAppOptions;

      if (pdoc.is_set())
      {

         pdoc->get_html_data()->m_propertyset["app_options_title"] = get_pane_by_id(pcreatordata->m_id)->get_title();

      }

      string strAppOptions = "matter://options.html";

      sp(simple_frame_window) pframe = GetParent();

      if (pframe.is_set())
      {

         strAppOptions = pframe->m_varFrame["app_options"]["resource"];

      }

      m_pdocAppOptions->open_document(strAppOptions);

      return true;

   }


   void pane_tab_view::prepare_form(id id, ::html_document * pdocument)
   {

      Application.prepare_form(id, pdocument);

   }


   void pane_tab_view::_001OnRemoveTab(::user::tab_pane * ptab)
   {

      ::user::tab_view::_001OnRemoveTab(ptab);

      m_viewmap.remove_key(ptab->m_id);

   }

} // namespace userex



