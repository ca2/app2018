//#include "framework.h"


namespace userex
{


   pane_tab_view::pane_tab_view(::aura::application * papp):
      object(papp),

      ::user::tab_view(papp),
      ::user::tab(papp),
      place_holder_container(papp)
   {


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
            if(puiNext == NULL || !puiNext->IsWindow())
               break;
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
            if(puiNext == NULL || (pui->m_pimpl) != NULL)
               break;
            pui = puiNext;
         }
         //         int32_t i = MAX(0, wnda.get_size() + m_pviewdata->m_iExtendOnParent);
         pui = wnda[wnda.get_size() + m_pviewdata->m_iExtendOnParent];
         pui->GetWindowRect(lprect);
         ScreenToClient(lprect);
      }
   }


   void pane_tab_view::on_show_view()
   {
      ::user::tab_view::on_show_view();
      ::user::view_creator::on_show_view();
      if(m_pviewdata != NULL && m_pviewdata->m_id != "file_manager" && m_pviewdataOld != NULL && m_pviewdataOld->m_id == "file_manager")
      {
         if(GetParentFrame()->ContinueModal(0))
         {
            GetParentFrame()->EndModalLoop("yes");
         }
         if(&filemanager_manager() != NULL)
         {
            filemanager_manager().get_filemanager_data()->m_pdocumentSave = NULL;
         }
      }

   }

   void pane_tab_view::on_new_view_creator_data(::user::view_creator_data * pcreatordata)
   {

      ::index iTab = get_tab_by_id(pcreatordata->m_id);

      if(iTab < 0)
         return;

      pane * ppane = (pane *)get_data()->m_panea.element_at(iTab);

      if(ppane == NULL)
         return;

      pcreatordata->m_pholder = get_new_place_holder(get_data()->m_rectTabClient);

      ppane->m_pholder = pcreatordata->m_pholder;

      if(ppane->m_pholder == NULL)
         return;

      pcreatordata->m_pviewdata = (void *)ppane;

   }


   bool pane_tab_view::on_hold(::user::interaction * pui,::user::place_holder * pholder)
   {
      if(!::user::place_holder_container::on_hold(pui,pholder))
         return false;
      ::user::tab::pane_array & panea = get_data()->m_panea;
      for(int32_t iTab = 0; iTab < panea.get_count(); iTab++)
      {
         if(panea[iTab]->m_pholder == pholder)
         {
            ::user::view_creator_data * pcreatordata = ensure_impact(panea[iTab]->m_id,get_data()->m_rectTabClient);
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


   void pane_tab_view::ensure_tab_by_id(id id)
   {

      ensure_impact(id,get_data()->m_rectTabClient);

   }


   void pane_tab_view::on_create_view(::user::view_creator_data * pcreatordata)
   {

      ::aura::library * plibrary = NULL;

      if(pcreatordata->m_id.is_text() && System.m_idmapCreateViewLibrary.Lookup(pcreatordata->m_id,plibrary) && plibrary != NULL)
      {


         //plibrary->on_create_view(pcreatordata);

      }

      else if(pcreatordata->m_id == "file_manager"
         || pcreatordata->m_id == "left_file"
         || pcreatordata->m_id == "right_file"
         || pcreatordata->m_id == "pick_file")
      {

         ::filemanager::data * pfilemanagerdata = oprop("data." + pcreatordata->m_id.str()).cast < ::filemanager::data >();

         if(pfilemanagerdata == NULL)
            pfilemanagerdata = new ::filemanager::data(get_app());




         pfilemanagerdata->m_id = pcreatordata->m_id;

         if(oprop("toolbar").is_set())
            pfilemanagerdata->m_strToolBar = oprop("toolbar");
         else
            pfilemanagerdata->m_strToolBar = "toolbar.xml";

         if(oprop("toolbar_save").is_set())
            pfilemanagerdata->m_strToolBarSave = oprop("toolbar_save");
         else
            pfilemanagerdata->m_strToolBarSave = "toolbar_save.xml";

         pfilemanagerdata->m_strDISection = Application.m_strAppName;

         sp(::filemanager::manager) pmanager = Session.filemanager().std().open_child(true,pfilemanagerdata->m_bTransparentBackground,pcreatordata->m_pholder,pfilemanagerdata,get_app()->m_pcoreapp);

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

         sp(::filemanager::manager) pmanager = Session.filemanager().std().open(cc);

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

   }


   ::filemanager::manager & pane_tab_view::filemanager_manager()
   {

      return  *m_pfilemanager.cast <::filemanager::manager>();

   }

   ::filemanager::manager & pane_tab_view::tabbed_filemanager_manager()
   {

      return  *m_pfilemanagerTabbed.cast <::filemanager::manager>();

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

      if(GetParentFrame()->ContinueModal(0) && &filemanager_manager() != NULL
         && filemanager_manager().get_filemanager_data()->m_pdocumentSave != NULL)
      {
         GetParentFrame()->EndModalLoop("yes");
      }

   }


} // namespace userex



