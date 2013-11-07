#include "framework.h"


#define SHELL_COMMAND_FIRST 0x1000
#define SHELL_COMMAND_LAST 0x2000


namespace filemanager
{


   ::filemanager::file_list::::filemanager::file_list(sp(base_application) papp) :
      element(papp),
      ::user::interaction(papp),
      ::user::form(papp),
      ::user::form_list(papp),
      ::filemanager::file_list(papp),
      ::user::scroll_view(papp),
      ::user::list(papp),
      ::userfs::list(papp)
   {

      m_bFileSize = false;
      m_bShow = false;
      m_dwLastFileSize = ::get_tick_count();

      m_pheaderctrl     = &m_headerctrl;
      m_pheaderctrl->SetBaseListCtrlInterface(this);

      connect_update_cmd_ui("edit_copy", &::filemanager::file_list::_001OnUpdateEditCopy);
      connect_command("edit_copy", &::filemanager::file_list::_001OnEditCopy);
      connect_update_cmd_ui("edit_paste", &::filemanager::file_list::_001OnUpdateEditPaste);
      connect_command("edit_paste", &::filemanager::file_list::_001OnEditPaste);
      connect_update_cmd_ui("trash_that_is_not_trash", &::filemanager::file_list::_001OnUpdateTrashThatIsNotTrash);
      connect_command("trash_that_is_not_trash", &::filemanager::file_list::_001OnTrashThatIsNotTrash);
      connect_update_cmd_ui("open_with", &::filemanager::file_list::_001OnUpdateOpenWith);
      connect_update_cmd_ui("spafy", &::filemanager::file_list::_001OnUpdateSpafy);
      connect_command("spafy", &::filemanager::file_list::_001OnSpafy);
      connect_update_cmd_ui("spafy2", &::filemanager::file_list::_001OnUpdateSpafy2);
      connect_command("spafy2", &::filemanager::file_list::_001OnSpafy2);
      connect_update_cmd_ui("file_rename", &::filemanager::file_list::_001OnUpdateFileRename);
      connect_command("file_rename", &::filemanager::file_list::_001OnFileRename);

   }

   ::filemanager::file_list::~::filemanager::file_list()
   {
   }

   void ::filemanager::file_list::install_message_handling(::message::dispatch * pinterface)
   {
      ::user::view::install_message_handling(pinterface);
      ::filemanager::file_list::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_RBUTTONUP, pinterface, this, &::filemanager::file_list::_001OnContextMenu);
      IGUI_WIN_MSG_LINK(WM_TIMER, pinterface, this, &::filemanager::file_list::_001OnTimer);
      connect_command_range(SHELL_COMMAND_FIRST, SHELL_COMMAND_LAST, &::filemanager::file_list::_001OnShellCommand);
      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &::filemanager::file_list::_001OnShowWindow);

   }





   #ifdef DEBUG
   void ::filemanager::file_list::assert_valid() const
   {
      ::user::view::assert_valid();
   }

   void ::filemanager::file_list::dump(dump_context & dumpcontext) const
   {
      ::user::view::dump(dumpcontext);
   }
   #endif //DEBUG


   void ::filemanager::file_list::on_update(sp(::user::view) pSender, LPARAM lHint, object* phint)
   {

      ::filemanager::data_interface::on_update(pSender, lHint, phint);

      if(m_bStatic && lHint == hint_add_location)
      {

         stringa stra;

         GetFileManager()->data_get(GetFileManager()->get_filemanager_data()->m_pschema->m_dataidStatic, ::core::system::idEmpty, stra);

         string strPath = GetFileManager()->get_item().m_strPath;

         strPath.trim();

         if(strPath.has_char() && GetFileManager()->get_fs_data()->is_dir(strPath))
         {

            if(stra.add_unique(strPath) >= 0)
            {

               GetFileManager()->data_set(GetFileManager()->get_filemanager_data()->m_pschema->m_dataidStatic, ::core::system::idEmpty, stra);

               add_item(GetFileManager()->get_item().m_strPath, System.file().name_(GetFileManager()->get_item().m_strPath));
            
               _001OnUpdateItemCount();

            }

         }

      }
      else if(lHint == 123)
      {
         _017UpdateList();
      }
      else if(lHint == 1234)
      {
         _001UpdateColumns();
         _017UpdateList();
      }
      else if(lHint == 123458)
      {
         _001SetView(ViewList);
      }
      else if(lHint == 1234511)
      {
         _001SetView(ViewReport);
      }
      else if(lHint == 1234525)
      {
         _001SetView(ViewIcon);
      }
      if(phint != NULL)
      {
         if(base < filemanager::update_hint >::bases(phint))
         {
            filemanager::update_hint * puh = (filemanager::update_hint *) phint;
            if(puh->is_type_of(filemanager::update_hint::TypeInitialize))
            {
               m_pbaseapp = get_app()->m_pplaneapp;
               db_server * pcentral = dynamic_cast < db_server * > (&System.m_simpledb.db());
               if(pcentral == NULL)
                  return;
               //DBFileSystemSizeSet * pset = pcentral->m_pfilesystemsizeset;
               SetTimer(5432185, 230, NULL);
               //::user::window::SetTimer(5432184, 23, NULL);
   // dbbreak            m_pserverNext = simpledb::get(get_app())->GetDataServer();
   //            AddClient(this);
     //          SetDataInterface(&m_datainterface);
       //        AddClient(&m_datainterface);
               string str;
               str.Format("::filemanager::file_list(%s)", GetFileManager()->get_filemanager_data()->m_strDISection);
               if(GetFileManager()->get_filemanager_data()->m_bPassBk)
               {
                  ::user::list::m_bBackgroundBypass = true;
               }
               else if(GetFileManager()->get_filemanager_data()->m_bTransparentBackground)
               {
                  ::user::list::m_etranslucency = ::user::list::TranslucencyPresent;
               }
               m_dataid = str;
               _001UpdateColumns();

            }
            else if(!m_bStatic && puh->is_type_of(filemanager::update_hint::TypeSynchronizePath))
            {
               if(GetFileManager()->get_filemanager_data()->m_pholderFileList != NULL)
               {
                  if(GetFileManager()->get_filemanager_data()->m_pholderFileList->m_uiptraHold.get_size() > 0)
                  {
                     GetFileManager()->get_filemanager_data()->m_pholderFileList->m_uiptraHold[0].ShowWindow(SW_HIDE);
                  }
                  GetFileManager()->get_filemanager_data()->m_pholderFileList->hold(this);
                  GetFileManager()->get_filemanager_data()->m_pholderFileList->layout();
               }
               _017PreSynchronize();
               _017Synchronize();
               data_get_DisplayToStrict();
               _001OnUpdateItemCount();
               /*string str;
               if(data_get("sort-" + GetFileManager()->get_item().m_strPath, ::core::system::idEmpty, str))
               {
                  stringa stra;
                  stra.add_tokens(str, ";", true);
                  if(stra.get_size() == m_iaDisplayToStrict.get_size())
                  {
                     for(int32_t i = 0; i < m_iaDisplayToStrict.get_size(); i++)
                     {
                        m_iaDisplayToStrict.set(i, atoi(stra[i]));
                     }
                  }
               }*/
            }
            else if(m_bStatic && puh->is_type_of(filemanager::update_hint::TypeSynchronizeLocations))
            {
               _017UpdateList();
            }
            else if(puh->is_type_of(filemanager::update_hint::TypeFilter))
            {
               if(puh->m_wstrFilter.is_empty())
               {
                  FilterClose();
               }
               else
               {
                  FilterBegin();
                  Filter1(puh->m_wstrFilter);
//                  FilterApply();
               }
            }
            else if(puh->is_type_of(filemanager::update_hint::TypeGetActiveViewSelection))
            {
               if(GetParentFrame()->GetActiveView() ==  (this))
               {
                  GetSelected(puh->m_itemaSelected);
               }
            }
         }
         else if(base < form_update_hint > :: bases(phint))
         {
            form_update_hint * puh = dynamic_cast<form_update_hint * > (phint);
            if(puh->m_etype == form_update_hint::type_after_browse)
            {
               if(puh->m_strForm == "filemanager\\replace_name_in_file_system.xhtml")
               {
                  html::elemental * pelemental = dynamic_cast < html::elemental * > (puh->m_pformview->get_html_data()->get_element_by_name("encontrar"));
                  html::impl::input_text * pinput = dynamic_cast < html::impl::input_text * > (pelemental->m_pimpl);
                  sp(text_interface) ptext =  (pinput->m_pedit);
                  range range;
                  _001GetSelection(range);
                  if(range.get_item_count() > 0)
                  {
                     ptext->_001SetText(get_fs_list_data()->m_itema.get_item(range.ItemAt(0).get_lower_bound()).m_strName);
                  }
               }
            }
            file_manager_form_update_hint * pmanageruh = dynamic_cast<file_manager_form_update_hint * > (phint);
            if(pmanageruh != NULL)
            {
               if(!pmanageruh->m_strFind.is_empty())
               {
                  Application.file().replace(m_strPath, pmanageruh->m_strFind, pmanageruh->m_strReplace);
                  _017UpdateList();
               }
            }
         }

      }
   }

   void ::filemanager::file_list::_001OnClick(UINT nFlags, point point)
   {
      UNREFERENCED_PARAMETER(nFlags);
      index iItem;
      index iSubItem;
      if(_001HitTest_(point, iItem, iSubItem))
      {
         if(iSubItem == m_iNameSubItem || (m_eview == ViewList && iSubItem == 0))
         {
           _017OpenSelected(true);
         }
      }
   }

   void ::filemanager::file_list::_001OnRightClick(UINT nFlags, point point)
   {
      UNREFERENCED_PARAMETER(nFlags);
      index iItem;
      index iSubItem;
      if(_001HitTest_(point, iItem, iSubItem))
      {
         _017OpenContextMenuSelected();
      }
      else
      {
         _017OpenContextMenu();
      }
   }

   /*bool ::filemanager::file_list::OnSetData(
      const ::database::id & key,
      int32_t iLine,
      int32_t iColumn,
      var & var, ::database::update_hint * puh)
   {
      if(key.get_value() == _vms::FILE_MANAGER_ID_FILE_NAME)
      {
         ASSERT(var.get_type() == var::type_string || var.is_empty());
         string str;
         str = var.m_str;
         RenameFile(iLine, str);
         return true;
      }
      return data_server_interface::OnSetData(key, iLine, iColumn, var, puh);
   }

   bool ::filemanager::file_list::get_data(
      const ::database::id & key,
      int32_t iLine,
      int32_t iColumn,
      var & var)
   {
      if(key.get_value() == _vms::FILE_MANAGER_ID_FILE_NAME)
      {
         string str;
         str = m_itema.get_item(iLine).m_strName;
         var.set_type(var::type_string);
         var.m_str = str;
         return true;
      }
      return data_server_interface::OnSetData(key, iLine, iColumn, var);
   }*/

   void ::filemanager::file_list::RenameFile(int32_t iLine, string &wstrNameNew)
   {

      string str = get_fs_list_data()->m_itema.get_item(iLine).m_strPath;

      strsize iFind = str.reverse_find(L'\\');

      string wstrNew = str.Left(iFind + 1) + wstrNameNew;

      System.file().path().rename(wstrNew, str, get_app());

      _017UpdateList();

   }

   void ::filemanager::file_list::_001OnContextMenu(signal_details * pobj)
   {
      //SCAST_PTR(::message::context_menu, pcontextmenu, pobj)
      SCAST_PTR(::message::mouse, pcontextmenu, pobj)
      index iItem;
//      HRESULT hr;
      point point = pcontextmenu->m_pt;
      class point ptClient = point;
      ::user::list::ScreenToClient(&ptClient);
        if(_001HitTest_(ptClient, iItem))
      {
         ::user::menu menu(get_app());
         if(get_fs_list_data()->m_itema.get_item(iItem).IsFolder())
         {
            _017OpenContextMenuFolder(new ::fs::item(get_fs_list_data()->m_itema.get_item(iItem)));
            /*if (menu.LoadXmlMenu(GetFileManager()->get_filemanager_data()->m_pschema->m_strFolderPopup))
            {
               ::user::menu menuPopup(get_app(), menu.GetSubMenu(0));
               //SimpleMenu* pPopup = (SimpleMenu *) menu.GetSubMenu(0);
               //ASSERT(pPopup != NULL);
               sp(::user::frame_window) pframe = ( (GetParentFrame()));
               pframe->SetActiveView(this);
               menuPopup.set_app(get_app());
               menuPopup.TrackPopupMenu(
                  point.x, point.y,
                  pframe);
            }*/
         }
         else if (menu.LoadXmlMenu(GetFileManager()->get_filemanager_data()->m_pschema->m_strFilePopup))
         {
            ::user::menu menuPopup(get_app(), menu.GetSubMenu(0));
            //SimpleMenu* pPopup = (SimpleMenu *) menu.GetSubMenu(0);
            //ASSERT(pPopup != NULL);
            sp(::user::frame_window) pframe = (GetTopLevelFrame());

            pframe->SetActiveView(this);

            //IContextMenu * pcontextmenu;
            //int32_t iInsertIndex = menu.FindMenuItemPos(GetFileManager()->get_filemanager_data()->m_pschema->m_uiFilePopupSubstId);
   /*         if(iInsertIndex >= 0)
            {
               pPopup->DeleteMenu(iInsertIndex, MF_BYPOSITION);
               hr = m_spshellfolder->GetUIObjectOf(
                  NULL,
                  1,
                  (LPCITEMIDLIST *) &m_itema.get_item(iItem).m_lpiidlRelative,
                  IID_IContextMenu,
                  NULL,
                  (void **) &m_contextmenu.m_pcontextmenu);


               if(SUCCEEDED(hr))
               {
                  hr = m_contextmenu.m_pcontextmenu->QueryContextMenu(
                        pPopup->GetSafeHmenu(),
                        iInsertIndex,
                        SHELL_COMMAND_FIRST,
                        SHELL_COMMAND_LAST,
                        CMF_NORMAL);

               }
            }*/

            menuPopup.TrackPopupMenu(0, point.x, point.y, pframe);
         }
      }
      else
      {
         ::user::menu menu(get_app());
         if (menu.LoadXmlMenu(GetFileManager()->get_filemanager_data()->m_pschema->m_strPopup))
         {
            ::user::menu menuPopup(get_app(), menu.GetSubMenu(0));
            //ASSERT(pPopup != NULL);
            //sp(::user::frame_window) pframe = GetTopLevelFrame();



   /*         hr = m_spshellfolderParent->GetUIObjectOf(
               NULL,
               1,
               (LPCITEMIDLIST *) &m_lpiidlRelative,
               IID_IContextMenu,
               NULL,
               (void **) &m_contextmenu.m_pcontextmenu);


            if(SUCCEEDED(hr))
            {
               hr = m_contextmenu.m_pcontextmenu->QueryContextMenu(
                     pPopup->GetSafeHmenu(),
                     0,
                     SHELL_COMMAND_FIRST,
                     SHELL_COMMAND_LAST,
                     CMF_NORMAL);

            }*/


            sp(::user::frame_window) pframe = (GetTopLevelFrame());

            pframe->SetActiveView(this);

            menuPopup.TrackPopupMenu(0, point.x, point.y, pframe);

         }
      }
   }

   bool ::filemanager::file_list::pre_create_window(CREATESTRUCT& cs)
   {

      cs.style |= WS_CLIPCHILDREN;

      return ::user::view::pre_create_window(cs);
   }

   UINT c_cdecl ::filemanager::file_list::ThreadProcFileSize(LPVOID lpparam)
   {
      file_size * psize = (file_size *) lpparam;
      db_server * pcentral = dynamic_cast < db_server * > (&App(psize->m_pview->m_pbaseapp).simpledb().db());
      if(pcentral == NULL)
         return 0;
      DBFileSystemSizeSet * pset = pcentral->m_pfilesystemsizeset;
      bool bPendingSize;
      int64_t i64Size;
      while(true)
      {
         bPendingSize = false;
         pset->get_fs_size(i64Size, psize->m_str, bPendingSize);
         if(!bPendingSize)
            break;
      }
      //psize->m_pview->_001RedrawWindow();
      delete psize;
      //::filemanager::file_list * pview = (::filemanager::file_list *) lpparam;
      //pview->FileSize();
      return 0;
   }

   void ::filemanager::file_list::FileSize()
   {
      if(m_bFileSize)
         return;
      m_bFileSize = true;

      db_server * pcentral = dynamic_cast < db_server * > (&System.m_simpledb.db());
      if(pcentral == NULL)
         return;
      DBFileSystemSizeSet * pset = pcentral->m_pfilesystemsizeset;

      int32_t i;
      while(true)
      {
         i = 0;
         while(i < get_fs_list_data()->m_itema.get_count() || IsWindowVisible())
         {
            int64_t i64Size;
            bool bPendingSize;
            single_lock lock(m_pthread->m_pthread);
            if(!lock.lock(millis(1984)))
               return;
            if(i >= get_fs_list_data()->m_itema.get_count())
               i = 0;
            bPendingSize = false;
            try
            {
               pset->get_fs_size(i64Size, get_fs_list_data()->m_itema.get_item(i).m_strPath, bPendingSize);
            }
            catch(...)
            {
            }
            lock.unlock();
            i++;
            Sleep(23);
         }
         Sleep(84);
      }
      m_bFileSize = false;
   }


   void ::filemanager::file_list::_001OnTimer(signal_details * pobj)
   {
      SCAST_PTR(::message::timer, ptimer, pobj)
      if(ptimer->m_nIDEvent == 198477)
      {
         if(GetFileManager()->get_filemanager_data()->m_bSetBergedgeTopicFile)
         {
            stringa stra;
            GetSelectedFilePath(stra);
            if(stra.get_count() <= 0)
            {
               Session.m_varTopicFile.unset();
            }
            else if(stra.get_count() == 1)
            {
               Session.m_varTopicFile = stra[0];
            }
            else
            {
               Session.m_varTopicFile = stra;
            }

         }
         else
         {
            KillTimer(198477);
         }
      }
      else if(ptimer->m_nIDEvent == 1234567)
      {
         ASSERT(FALSE);
         m_iAnimate += 2;
         if(m_iAnimate >= 11)
         {
            m_iAnimate = 0;
            KillTimer(ptimer->m_nIDEvent);

         }
         RedrawWindow();
      }
      else if(ptimer->m_nIDEvent == 123)
      {
         RedrawWindow();
         KillTimer(123);
      }
      else if(ptimer->m_nIDEvent == 5432176)
      {
         _001CreateImageListStep();
      }
      else if(ptimer->m_nIDEvent == 5432185)
      {
/*         if(m_bPendingSize)
         {
            _001RedrawWindow();
         }
         if(::get_tick_count() - m_dwLastFileSize > 840)
         {
            m_dwLastFileSize = ::get_tick_count();
            sp(::user::interaction) pwnd = GetParentFrame();
//            bool b = pwnd->IsWindowVisible();
            if(pwnd->IsWindowVisible() && m_bFileSize)
            {
               m_bShow = true;
               file_size_add_request(false);
            }
         }*/
      }
      else if(ptimer->m_nIDEvent == 5432184)
      {
      }
   }

   void ::filemanager::file_list::StartAnimation()
   {
      //SetTimer(1234567, 50, NULL);
   }

   bool ::filemanager::file_list::_001OnCmdMsg(BaseCmdMsg * pcmdmsg)
   {
      ::fs::item_array itema;
      GetSelected(itema);
      if(GetFileManagerDoc()->HandleDefaultFileManagerItemCmdMsg(pcmdmsg, itema))
         return TRUE;
      return ::user::view::_001OnCmdMsg(pcmdmsg);
   }

   void ::filemanager::file_list::_001OnShellCommand(signal_details * pobj)
   {
      SCAST_PTR(::message::command, pcommand, pobj)
      m_contextmenu.OnCommand(pcommand->GetId());
   }

   void ::filemanager::file_list::_001OnFileManagerItemCommand(signal_details * pobj)
   {
      SCAST_PTR(BaseCommand, pcommand, pobj)
      ::fs::item_array itema;
      index iItemRange, iItem;
      range range;
      _001GetSelection(range);
      for(iItemRange = 0;
          iItemRange < range.get_item_count();
          iItemRange++)
      {
         item_range itemrange = range.ItemAt(iItemRange);
         for(iItem = itemrange.get_lower_bound() ; iItem <= itemrange.get_upper_bound(); iItem ++)
         {
            itema.add(new  ::fs::item  (get_fs_list_data()->m_itema.get_item(iItem)));
         }
      }
      GetFileManager()->get_filemanager_data()->OnFileManagerItemCommand(
         pcommand->m_id,
         itema);
   }

   void ::filemanager::file_list::_001OnFileManagerItemUpdate(signal_details * pobj)
   {
      SCAST_PTR(::message::update_cmd_ui, pupdatecmdui, pobj)
      ::fs::item_array itema;
      index iItemRange, iItem;
      range range;
      _001GetSelection(range);
      for(iItemRange = 0;
          iItemRange < range.get_item_count();
          iItemRange++)
      {
         item_range itemrange = range.ItemAt(iItemRange);
         for(iItem = itemrange.get_lower_bound() ; iItem <= itemrange.get_upper_bound(); iItem ++)
         {
            itema.add(new  ::fs::item  (get_fs_list_data()->m_itema.get_item(iItem)));
         }
      }
      GetFileManager()->get_filemanager_data()->OnFileManagerItemUpdate(
         pupdatecmdui->m_pcmdui,
         itema);
      pobj->m_bRet = true;
   }

   void ::filemanager::file_list::_017OpenContextMenuFolder(sp(::fs::item)  item)
   {
      
      stringa straCommand;
      
      stringa straCommandTitle;
      
      GetFileManager()->get_filemanager_data()->OnFileManagerOpenContextMenuFolder(item, straCommand, straCommandTitle);
      
      if(straCommand.get_size() > 0)
      {
         
         ::user::menu menu(get_app());
         
         point ptCursor;
         
         Session.get_cursor_pos(&ptCursor);
      
         if(menu.create_menu(straCommand, straCommandTitle))
         {

            menu.TrackPopupMenu(0, ptCursor.x, ptCursor.y, GetParentFrame());

         }

      }

   }

   void ::filemanager::file_list::_017OpenContextMenuFile(const ::fs::item_array & itema)
   {
      GetFileManager()->get_filemanager_data()->OnFileManagerOpenContextMenuFile(itema);
   }

   void ::filemanager::file_list::_017OpenContextMenu()
   {
      GetFileManager()->get_filemanager_data()->OnFileManagerOpenContextMenu();
   }

   void ::filemanager::file_list::_017OpenFolder(sp(::fs::item) item)
   {
      GetFileManager()->FileManagerBrowse(item);
   }

   void ::filemanager::file_list::_017OpenFile(const ::fs::item_array &itema)
   {
      GetFileManager()->get_filemanager_data()->OnFileManagerOpenFile(itema);
   }

   void ::filemanager::file_list::_001OnFileRename(signal_details * pobj)
   {
      ::filemanager::file_list::_001OnFileRename(pobj);
   }

   void ::filemanager::file_list::_001OnUpdateFileRename(signal_details * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
      pcmdui->m_pcmdui->Enable(_001GetSelectedItemCount() == 1);
      pobj->m_bRet = true;
   }


   void ::filemanager::file_list::_001OnUpdateEditCopy(signal_details * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
      range range;
      _001GetSelection(range);
      pcmdui->m_pcmdui->Enable(range.get_item_count() > 0);
      pobj->m_bRet = true;
   }

   void ::filemanager::file_list::_001OnEditCopy(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      ::fs::item_array itema;
      GetSelected(itema);
      stringa stra;

      for(int32_t i = 0; i < itema.get_size(); i++)
      {
         stra.add(itema[i].m_strPath);
      }
      /*
      string str;
      if(itema.get_size() > 0)
      {
         str = itema[0].m_strPath;
      }
      for(int32_t i = 1; i < itema.get_size(); i++)
      {
         str += "|" + itema[i]->m_strPath;
      }
      System.m_strCopy = str;
   */


      System.copydesk().set_filea(stra);

   }

   void ::filemanager::file_list::_001OnUpdateEditPaste(signal_details * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
      pcmdui->m_pcmdui->Enable(System.copydesk().get_file_count() > 0);
      pobj->m_bRet = true;
   }


   void ::filemanager::file_list::_001OnEditPaste(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      stringa stra;

      System.copydesk().get_filea(stra);
      string strDir;
      strDir = GetFileManagerItem().m_strPath;

      GetFileManagerDoc()->get_operation_doc(true)->m_thread.queue_copy(stra, strDir, NULL, true);
      GetFileManagerDoc()->get_operation_doc(true)->m_thread.kick();


     /* for(int32_t i = 0; i < stra.get_size(); i++)
      {
         ::CopyFileW(
            L"\\\\?\\" + ::str::international::utf8_to_unicode(stra[i]),
            L"\\\\?\\" + ::str::international::utf8_to_unicode(System.dir().path(strDir, System.file().title(stra[i]))), TRUE);
      }*/
      //get_document()->update_all_views(NULL, 123);
   }

   void ::filemanager::file_list::_001OnUpdateTrashThatIsNotTrash(signal_details * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
      range range;
      _001GetSelection(range);
      pcmdui->m_pcmdui->Enable(range.get_item_count() > 0);
      pobj->m_bRet = true;
   }

   void ::filemanager::file_list::_001OnTrashThatIsNotTrash(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      ::fs::item_array itema;
      GetSelected(itema);
      stringa stra;
      for(int32_t i = 0; i < itema.get_size(); i++)
      {
         stra.add(itema[i].m_strPath);
      }
      Application.file().trash_that_is_not_trash(stra);
      _017UpdateList();
   }

   void ::filemanager::file_list::_001OnUpdateOpenWith(signal_details * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)

         ::user::menu_button_cmd_ui * pcmdui1 = dynamic_cast < ::user::menu_button_cmd_ui * > (pcmdui->m_pcmdui);
         if(pcmdui1 != NULL)
         {
            ::user::menu_item_ptra * pitema = pcmdui1->m_pitema;

            ::user::menu_base * pbase = pitema->element_at(pcmdui->m_pcmdui->m_iIndex)->m_pbase;
            pitema->remove_at(pcmdui->m_pcmdui->m_iIndex);


            index iStartIndex = pcmdui->m_pcmdui->m_iIndex;
            index iIndex = iStartIndex;


            ::fs::item_array itema;
            GetSelected(itema);

            string strPath = itema[0].m_strPath;

            string strExt = System.file().extension(strPath);

            stringa stra;
            System.os().file_extension_get_open_with_list_keys(stra, strExt);

            m_straOpenWith = stra;
            ::count iCount = stra.get_size();

            sp(::user::menu_item) pmenuitem(new ::user::menu_item(get_app()));
            string str;
            for(int32_t i = 0; i < iCount; i++)
            {
               pmenuitem->m_id = "open with" + stra[i];
               pmenuitem->m_button._001SetButtonText(stra[i]);
               pmenuitem->m_iLevel = pitema->m_pitemContainer != NULL ? pitema->m_pitemContainer->m_iLevel + 1 : 0;
               pmenuitem->m_pbase = pbase;
               pitema->insert_at(iIndex, pmenuitem);
               iIndex++;
            }
            pcmdui->m_pcmdui->m_iIndex = iStartIndex;

            pcmdui->m_pcmdui->m_iCount = iIndex;


            pbase->layout();

         }
      pobj->m_bRet = true;
   }


   bool ::filemanager::file_list::_001OnUpdateCmdUi(cmd_ui * pcmdui)
   {
      int32_t iPos = -1;
      for(int32_t i = 0; i < m_straOpenWith.get_size(); i++)
      {
         string strId = "open with" + m_straOpenWith[i];
         if(pcmdui->m_id == strId)
         {
            iPos = i;
            break;
         }
      }
      if(iPos >= 0)
      {
         pcmdui->Enable(TRUE);
         return true;
      }
      else
      {
         return ::user::view::_001OnUpdateCmdUi(pcmdui);
      }
   }

   bool ::filemanager::file_list::_001OnCommand(id id)
   {
      if(id == "1000")
      {
   //      _017OpenSelected(true);
         return true;
      }
      int32_t iPos = -1;
      for(int32_t i = 0; i < m_straOpenWith.get_size(); i++)
      {
         string strId = "open with" + m_straOpenWith[i];
         if(id == strId)
         {
            iPos = i;
            break;
         }
      }
      if(iPos >= 0)
      {
         ::fs::item_array itema;
         GetSelected(itema);
         string strPath = itema[0].m_strPath;

#ifdef WINDOWSEX

         simple_shell_launcher launcher(NULL, "open", m_straOpenWith[iPos], strPath, System.dir().name(strPath), SW_SHOW);

         launcher.execute();

#else

         throw todo(get_app());

#endif

         return true;

      }
      else
      {
         return ::user::view::_001OnCommand(id);
      }

   }

   void ::filemanager::file_list::_001OnUpdateSpafy(signal_details * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
      range range;
      _001GetSelection(range);
      pcmdui->m_pcmdui->Enable(range.get_item_count() > 0);
      pobj->m_bRet = true;
   }

   void ::filemanager::file_list::_001OnSpafy(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      ::fs::item_array itema;
      GetSelected(itema);
      stringa stra;
      stringa straSub;

      string strFileList;
      string strFileCheck;
      for(int32_t i = 0; i < itema.get_size(); i++)
      {
         if(Application.dir().is(itema[i].m_strPath)
            && System.file().name_(itema[i].m_strPath) != ".svn")
         {
            Application.dir().rls(itema[i].m_strPath, &straSub);
            for(int32_t j = 0; j < straSub.get_size(); j++)
            {
               if(!Application.dir().is(straSub[j])
                && straSub[j].find(".svn") < 0)
               {
                  strFileList += straSub[j] + "\n";
                  strFileCheck += straSub[j] + ",";
                  strFileCheck += System.file().length(straSub[j]).get_string() + ",";
                  strFileCheck += System.file().md5(straSub[j]) +"\n";
               }
            }
         }
         else
         {
            strFileList += itema[i].m_strPath + "\n";
            strFileCheck += itema[i].m_strPath + ",";
            strFileCheck += System.file().length(itema[i].m_strPath).get_string() + ",";
            strFileCheck += System.file().md5(itema[i].m_strPath) +"\n";
         }
      }

      ::datetime::time time = ::datetime::time::get_current_time();

      string strTime;
      strTime.Format("%04d-%02d-%02d %02d-%02d",
         time.GetYear(),
         time.GetMonth(),
         time.GetDay(),
         time.GetHour(),
         time.GetMinute());
      string strBase = System.dir().path(GetFileManagerItem().m_strPath, "spafy_");
      string strList = strBase + "list_" + strTime + ".txt";
      string strCheck = strBase + "check_" + strTime + ".txt";


      Application.file().put_contents(strList, strFileList);
      Application.file().put_contents(strCheck, strFileCheck);

   }

   void ::filemanager::file_list::_001OnUpdateSpafy2(signal_details * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
      pcmdui->m_pcmdui->Enable(TRUE);
      pobj->m_bRet = true;
   }

   void ::filemanager::file_list::_001OnSpafy2(signal_details * pobj)
   {
      ::userfs::list_data * pdata = get_fs_list_data();
      UNREFERENCED_PARAMETER(pobj);
      stringa stra;
      stringa straSub;

      string strFileList;
      string strFileCheck;
      for(int32_t i = 0; i < pdata->m_itema.get_count(); i++)
      {
         if(::userfs::list::get_document()->set().is_dir(pdata->m_itema.get_item(i).m_strPath)
            && ::userfs::list::get_document()->set().file_name(pdata->m_itema.get_item(i).m_strPath) != ".svn")
         {
            Application.dir().rls(pdata->m_itema.get_item(i).m_strPath, &straSub);
            for(int32_t j = 0; j < straSub.get_size(); j++)
            {
             string strExtension = System.file().extension(straSub[j]);

               if(!Application.dir().is(straSub[j])
         && (strExtension == "exe" || strExtension == "dll" || strExtension == "dll.manifest"
             || strExtension == "exe.manifest"))
               {
                  strFileList += straSub[j] + "\n";
                  strFileCheck += straSub[j] + ",";
                  strFileCheck += System.file().length(straSub[j]).get_string() + ",";
                  strFileCheck += System.file().md5(straSub[j]) +"\n";
               }
            }
         }
         else
         {
             string strExtension = System.file().extension(pdata->m_itema.get_item(i).m_strPath);
         if(strExtension == "exe" || strExtension == "dll" || strExtension == "dll.manifest"
             || strExtension == "exe.manifest")
         {

            strFileList += pdata->m_itema.get_item(i).m_strPath + "\n";
            strFileCheck += pdata->m_itema.get_item(i).m_strPath + ",";
            strFileCheck += System.file().length(pdata->m_itema.get_item(i).m_strPath).get_string() + ",";
            strFileCheck += System.file().md5(pdata->m_itema.get_item(i).m_strPath) +"\n";
         }
         }
      }

      ::datetime::time time = ::datetime::time::get_current_time();

      string strTime;
      strTime.Format("%04d-%02d-%02d %02d-%02d",
         time.GetYear(),
         time.GetMonth(),
         time.GetDay(),
         time.GetHour(),
         time.GetMinute());
      string strBase = System.dir().path(GetFileManagerItem().m_strPath, "spafy_");
      string strList = strBase + "list_" + strTime + ".txt";
      string strCheck = strBase + "check_" + strTime + ".txt";


      Application.file().put_contents(strList, strFileList);
      Application.file().put_contents(strCheck, strFileCheck);

   }


   void ::filemanager::file_list::_001OnAfterSort()
   {
      data_set_DisplayToStrict();
   }

   void ::filemanager::file_list::schedule_file_size(const char * psz)
   {
      UNREFERENCED_PARAMETER(psz);
      if(!IsWindowVisible())
         return;
   //   file_size * psize = new file_size;
     // psize->m_pview = this;
      //psize->m_str = psz;
      //__begin_thread(ThreadProc4, psize, THREAD_PRIORITY_IDLE);
   }

   void ::filemanager::file_list::_001OnShowWindow(signal_details * pobj)
   {
      SCAST_PTR(::message::show_window, pshowwindow, pobj);

      if(!pshowwindow->m_bShow)
      {
   //      System.simple_message_box("hide");
      }

   }

   id ::filemanager::file_list::data_get_current_list_layout_id()
   {
      return GetFileManager()->get_item().m_strPath;
   }

   ::filemanager::file_list::::filemanager::file_list(sp(base_application) papp) :
      element(papp),
      ::user::interaction(papp),
      ::user::form(papp),
      ::user::form_list(papp),
      ::user::scroll_view(papp),
      ::user::list(papp),
      ::userfs::list(papp),
      m_gdibuffer(papp),
      m_mutex(papp)
   {
         m_iAnimate = 0;
         m_bRestartCreateImageList = false;
         m_bStatic = false;
         m_bPendingSize = false;
         m_pcreateimagelistthread = NULL;
      }

   ::filemanager::file_list::~::filemanager::file_list()
   {

   }




   void ::filemanager::file_list::_017Browse(const char * lpcsz)
   {
      _001ClearSelection();

      m_scrollinfo.m_ptScroll = m_scrollinfo.m_rectMargin.top_left();

      m_strPath = lpcsz;

      if (GetFileManagerItem().m_strPath.is_empty())
      {
         m_strPath.Empty();
         _017UpdateList(m_strPath);
      }
      else
      {
         string str(lpcsz);
         /*if(str.Right(4) == ".zip")
         {
         _017UpdateZipList(str, lpszExtra);
         goto zipDone;
         }*/
         _017UpdateList(str);
      }
      //   zipDone:;
   }

   void ::filemanager::file_list::_017UpdateList(const char * lpcsz)
   {
      /*      UNREFERENCED_PARAMETER(lpcsz);
      stringa straStrictOrder;
      data_get(
      data_get_current_sort_id(),
      string(data_get_current_list_layout_id()) + ".straStrictOrder",
      straStrictOrder);
      index_biunique iaDisplayToStrict;
      icon_layout iconlayout;
      data_get(data_get_current_sort_id(), data_get_current_list_layout_id(),
      iconlayout);
      iaDisplayToStrict = iconlayout.m_iaDisplayToStrict;
      index_biunique iaDisplayToStrictNew;

      m_straFileSize.remove_all();
      ::fs::list_item item;
      get_fs_list_data()->m_itema.clear(NULL);
      _001OnUpdateItemCount();*/
      /*


      string strParent = lpcsz;
      //      ULONG ulRet;
      //      STRRET strretDisplayName;
      //      HRESULT hr;
      uint32_t dwTimeIn;
      uint32_t dwTimeOut;

      dwTimeIn = get_tick_count();

      int32_t iMaxSize;
      iMaxSize = 1000;

      int32_t iSize;
      iSize = 0;

      m_itema.SetItemCount(iMaxSize);

      m_straStrictOrder.remove_all();

      stringa straPath;
      stringa straTitle;
      if(strlen(lpcsz) == 0)
      {
      System.dir().root_ones(straPath);
      straTitle = straPath;
      }
      else
      {
      System.dir().ls(lpcsz, &straPath, & straTitle);
      }

      for(int32_t i = 0; i < straPath.get_size(); i++)
      {
      item.m_flags.unsignalize_all();
      if(Application.dir().is(straPath[i]))
      {
      item.m_flags.signalize(filemanager::FlagFolder);
      }
      else
      {
      }
      item.m_iImage = -1;
      item.m_strPath = straPath[i];
      item.m_strName = straTitle[i];
      m_straStrictOrder.add(straPath[i]);

      m_itema.SetItemAt(iSize, item);
      iSize++;
      if(iSize >= iMaxSize)
      {
      iMaxSize += 1000;
      m_itema.SetItemCount(iMaxSize);
      }
      }
      m_itema.SetItemCount(iSize);



      //iaDisplayToStrict

      dwTimeOut = get_tick_count();

      TRACE("timeIn%d\n", dwTimeIn);
      TRACE("timeOut%d\n", dwTimeIn);
      TRACE("timeDelta%d\n", dwTimeOut - dwTimeIn);


      _001OnUpdateItemCount();

      if(m_eview == ViewIcon)
      {
      // primeiro, todos System arquivos que foram removidos
      // ou seja, que existem no array antigo,
      // mas n�o existe no novo.
      for(index strictOld = 0; strictOld < straStrictOrder.get_count(); strictOld++)
      {
      string str = straStrictOrder[strictOld];
      index find = m_straStrictOrder.find_first(str);
      if(find < 0)
      {
      iaDisplayToStrictNew.remove_b(strictOld);
      }
      }*/
      // segundo, reordena conforme a
      // ordem que a listagem de arquivos fornecida pelo
      // sistema operacional pode ser fornecida.
      /*       for(index strictNew = 0; strictNew < m_straStrictOrder.get_count(); strictNew++)
      {
      string str = m_straStrictOrder[strictNew];
      index strictOld = straStrictOrder.find_first(str);
      if(strictOld >= 0)
      {
      index iDisplay = iaDisplayToStrict.get_a(strictOld);
      iaDisplayToStrictNew.set(iDisplay, strictNew);
      }
      }
      // terceiro, adiciona System novos arquivos nos primeiros espa�os
      // vazios
      for(index strictNew = 0; strictNew < m_straStrictOrder.get_count(); strictNew++)
      {
      string str = m_straStrictOrder[strictNew];
      index strictOld = straStrictOrder.find_first(str);
      if(strictOld < 0)
      {
      iaDisplayToStrictNew.add_b_in_first_free_a(strictNew);
      }
      }
      m_iconlayout.m_iaDisplayToStrict = iaDisplayToStrictNew;
      }
      else
      {
      m_itema.Arrange(ArrangeByName);
      }

      _001CreateImageList();

      _001RedrawWindow();

      file_size_add_request(true);
      for(int32_t i = 0; i < m_itema.get_item_count(); i++)
      {
      pset->m_table.add_request(m_itema.get_item(i).m_strPath);
      }*/
      /*  if(m_eview == ViewIcon)
      {
      data_set(
      data_get_current_sort_id(),
      string(data_get_current_list_layout_id()) + ".straStrictOrder",
      m_straStrictOrder);
      m_iconlayout.m_iaDisplayToStrict = iaDisplayToStrictNew;
      data_set_DisplayToStrict();
      } */

      return ::userfs::list::_017UpdateList(lpcsz);


   }


   void ::filemanager::file_list::_017UpdateZipList(const char * lpcsz)
   {
      /*      ::fs::list_item item;

      get_fs_list_data()->m_itema.clear(NULL, NULL);
      _001OnUpdateItemCount();

      string szPath(lpcsz);
      string wstrExtra(lpszExtra);
      string wstrExtraPath;
      string wstrItemExtra;

      int32_t iFind;
      ::file::binary_buffer_sp spfile(allocer());

      //spfile->open(szPath, ::file::mode_read | ::file::type_binary);

      array < ::file::memory_buffer, ::file::memory_buffer & > filea;
      _vmszipFile zipfile;

      zipfile.m_pfile = &file;

      unzFile pf = _vmszipApi::unzipOpen(&zipfile);

      array < ::file::memory_buffer, ::file::memory_buffer & > filea;
      int32_t iStart = 0;
      int32_t iFind;
      while((iFind  = wstrExtra.find(L".zip:", iStart)) >= 0)
      {
      filea.add(::file::memory_buffer());
      pf->dump(filea.last_element(), wstrExtra.Mid(iStart + 5, iFind - iStart + 5));
      iStart = iFind + 1;
      }*/


      /*    stringa wstraItem;

      string str;

      str = szPath;
      str += L":" + wstrExtra;
      str = str.Mid(0, str.reverse_find(".zip:") + 4);

      zip::Util(get_app()).ls(str, false, &wstraItem);

      string wstrFolder;
      stringa wstraFolder;
      string wstrItem;
      for(int32_t i = 0; i < wstraItem.get_size(); i++)
      {
      wstrItem = wstraItem[i];



      wstrExtraPath = wstrItem;

      // ignore this file if its not in the Extra sub folder
      if(wstrExtraPath.Left(wstrExtra.get_length()) != wstrExtra ||
      wstrExtraPath == wstrExtra)
      continue;

      wstrItemExtra = wstrExtraPath.Mid(wstrExtra.get_length());

      item.m_flags.unsignalize_all();

      iFind = wstrItemExtra.find("/");
      if(iFind > 0)
      {
      wstrFolder = wstrItemExtra.Left(iFind);
      if(wstraFolder.contains(wstrFolder))
      continue;
      wstraFolder.add(wstrFolder);
      item.m_flags.signalize(filemanager::FlagFolder);
      item.m_strPath    = szPath;
      item.m_iImage     = -1;
      item.m_strName    = wstrFolder;
      item.m_strExtra   = wstrExtra + wstrFolder + "/";
      }
      else
      {
      item.m_strPath    = szPath;
      item.m_iImage     = -1;
      item.m_strName    = wstrItemExtra;
      item.m_strExtra   = wstrExtraPath;
      if(!item.m_strExtra.Right(4).CompareNoCase(".zip"))
      {
      item.m_strExtra += ":";
      }
      }

      item.m_flags.signalize(FlagInZip);
      m_itema.add_item(item);
      }
      _001OnUpdateItemCount();
      _001CreateImageList();*/


      return ::userfs::list::_017UpdateZipList(lpcsz);

   }

   void ::filemanager::file_list::_001CreateImageList()
   {

      icon_key iconkey;
      icon icon;

#ifdef WINDOWSEX
      for (POSITION pos = m_iconmap.get_start_position();
         pos != NULL;
         m_iconmap.get_next_assoc(pos, iconkey, icon))
      {
         DestroyIcon(icon.m_hicon);
      }
#endif

      m_iCreateImageListStep = 0;
      m_bCreateImageList = true;
      if (m_pcreateimagelistthread == NULL)
      {
         m_pcreateimagelistthread = new create_image_list_thread(get_app());
         m_pcreateimagelistthread->m_plist = this;
         m_pcreateimagelistthread->begin();
      }
   }

   ::filemanager::file_list::create_image_list_thread::create_image_list_thread(sp(base_application) papp) :
      element(papp),
      thread(papp)
   {
   }

   int32_t ::filemanager::file_list::create_image_list_thread::run()
   {
      int32_t iStepSetCount = 84;
      int32_t iStepSetSleep = 23;
      while (get_run())
      {
         int32_t i = iStepSetCount;
         while (i > 0 && get_run())
         {
            if (!m_plist->_001CreateImageListStep())
               goto endloop;
            i--;
         }
         m_plist->post_message(MessageMainPost, MessageMainPostCreateImageListItemStepSetRedraw);
         Sleep(iStepSetSleep);
      }
   endloop:
      m_plist->post_message(MessageMainPost, MessageMainPostCreateImageListItemRedraw);
      synch_lock lock(m_plist->m_pthread->m_pthread);
      m_plist->m_pcreateimagelistthread = NULL;
      return 0;

   }

   bool ::filemanager::file_list::_001CreateImageListStep()
   {

      single_lock sl(&m_mutex, true);

      if (m_iCreateImageListStep < 0 || m_iCreateImageListStep >= get_fs_list_data()->m_itema.get_count())
      {
         if (m_bRestartCreateImageList)
         {
            m_bRestartCreateImageList = false;
            m_iCreateImageListStep = 0;
         }
         else
         {
            return false;
         }
      }
      if (m_iCreateImageListStep < 0
         || m_iCreateImageListStep >= get_fs_list_data()->m_itema.get_count())
      {
         return false;
      }

      ::user::list_column * pcolumn = m_columna._001GetBySubItem(m_iNameSubItem);

      if (pcolumn != NULL && pcolumn->m_iSubItem == m_iNameSubItem)
      {
         ::userfs::list_item & item = get_fs_list_data()->m_itema.get_item((int32_t)m_iCreateImageListStep);

         ///IShellFolder * lpsf = m_pshellfolder;
         item.m_iImage = System.user()->shellimageset().GetImage(
            _GetWnd()->GetTopLevelParent()->get_handle(),
            item.m_strPath,
            NULL,
            _shell::IconNormal,
            get_document()->set().is_dir(item.m_strPath));

         m_iCreateImageListStep++;

      }
      else
      {
         ::user::list::_001CreateImageList(pcolumn);
      }

      return true;
   }

   void ::filemanager::file_list::_001InsertColumns()
   {
      class user::control::descriptor control;


      if (GetFileManager()->get_filemanager_data()->m_bIconView)
      {
         m_eview = ViewIcon;
      }

      ::user::list_column column;

      int32_t iCount = 0;

      filemanager::file_list_callback * pcallback =
         GetFileManager()->get_filemanager_data()->m_pschema->m_pfilelistcallback;

      if (pcallback != NULL)
      {
         iCount = pcallback->GetActionButtonCount();
      }

      index i;
      for (i = 0; i < iCount; i++)
      {
         control.m_bTransparent = true;
         control.set_type(user::control::type_button);
         control.m_typeinfo = System.type_info < BaseButtonControl >();
         control.m_id = 1000 + i;
         control.add_function(user::control::function_action);
         index iControl = _001AddControl(control);

         column.m_iWidth = 18;
         column.m_iSubItem = i;
         column.m_iControl = iControl;
         column.m_bCustomDraw = true;
         column.m_bEditOnSecondClick = true;
         column.m_pil = pcallback->GetActionButtonImageList(i);
         _001AddColumn(column);
      }


      if (GetFileManager()->get_filemanager_data()->m_bListSelection)
      {
         column.m_iWidth = GetFileManager()->get_filemanager_data()->m_iIconSize;
         column.m_iSubItem = i;
         //column.m_bIcon                = true;
         column.m_sizeIcon.cx = GetFileManager()->get_filemanager_data()->m_iIconSize;
         column.m_sizeIcon.cy = GetFileManager()->get_filemanager_data()->m_iIconSize;
         column.m_iControl = -1;
         column.m_datakey = "FILE_MANAGER_ID_FILE_NAME";
         column.m_bEditOnSecondClick = false;
         if (GetFileManager()->get_filemanager_data()->m_iIconSize >= 48)
         {
            column.m_pil = System.user()->shellimageset().GetImageList48();
         }
         else
         {
            column.m_pil = System.user()->shellimageset().GetImageList16();
         }
         _001AddColumn(column);
         m_iSelectionSubItem = i;

         i++;
      }
      else
      {
         m_iSelectionSubItem = -1;
      }

      control.set_type(user::control::type_edit_plain_text);
      control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
      //pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
      control.set_data_type(user::control::DataTypeString);
      control.add_function(user::control::function_vms_data_edit);
      //control.m_typeinfo = System.type_info < simple_edit_plain_text > ();
      control.m_typeinfo = sp(type)();
      control.m_iSubItem = i;
      control.m_id = 1000 + i;
      index iControl = _001AddControl(control);



      column.m_iSubItem = i;
      m_iNameSubItem = i;
      if (GetFileManager()->get_filemanager_data()->m_bListText)
      {
         m_iNameSubItemText = i;
         column.m_iWidth = 500;
      }
      else
      {
         m_iNameSubItemText = -1;
         column.m_iWidth = GetFileManager()->get_filemanager_data()->m_iIconSize;
      }
      get_fs_list_data()->m_iNameSubItemText = m_iNameSubItemText;
      //column.m_bIcon                = true;
      column.m_sizeIcon.cx = GetFileManager()->get_filemanager_data()->m_iIconSize;
      column.m_sizeIcon.cy = GetFileManager()->get_filemanager_data()->m_iIconSize;
      column.m_iControl = iControl;
      column.m_datakey = "FILE_MANAGER_ID_FILE_NAME";
      column.m_bEditOnSecondClick = true;
      if (GetFileManager()->get_filemanager_data()->m_iIconSize >= 48)
      {
         column.m_pilHover = System.user()->shellimageset().GetImageList48Hover();
         column.m_pil = System.user()->shellimageset().GetImageList48();
      }
      else
      {
         column.m_pil = System.user()->shellimageset().GetImageList16();
      }
      _001AddColumn(column);

      i++;

      if (GetFileManager()->get_filemanager_data()->m_bFileSize)
      {
         // file/directory size
         column.m_iWidth = 100;
         column.m_iSubItem = i;
         m_iSizeSubItem = i;
         column.m_sizeIcon.cx = 0;
         column.m_sizeIcon.cy = 0;
         column.m_pilHover = NULL;
         column.m_pil = NULL;
         _001AddColumn(column);
      }

   }


   void ::filemanager::file_list::_001GetItemText(::user::list_item * pitem)
   {

      if (m_bStatic)
         return ::user::list::_001GetItemText(pitem);

      return ::userfs::list::_001GetItemText(pitem);

      /*UNREFERENCED_PARAMETER(iListItem);
      if(iSubItem == m_iNameSubItemText)
      {
      str = get_fs_lim_itema.get_item(iItem).m_strName;
      return true;
      }
      else if(iSubItem == m_iSizeSubItem)
      {
      bool bPendingSize = false;

      bool bGet;
      bGet = System.get_fs_size(str, m_itema.get_item(iItem).m_strPath, bPendingSize);
      if(bGet)
      {
      m_straFileSize.set_at_grow(iItem, str);
      }
      else
      {
      if(iItem < m_straFileSize.get_size())
      {
      str = m_straFileSize[iItem];
      }
      }
      if(bPendingSize)
      {
      schedule_file_size(m_itema.get_item(iItem).m_strPath);
      m_bPendingSize = true;
      }
      return true;
      }
      //else if(m_bStatic)
      //{
      // return ::user::list::_001GetItemText(str, iItem, iSubItem, iListItem);
      //}
      else
      return false;*/
   }

   void ::filemanager::file_list::_001GetItemImage(::user::list_item * pitem)
   {
      return ::userfs::list::_001GetItemImage(pitem);
      /*      if(iSubItem == m_iSelectionSubItem)
      {
      if(m_rangeSelection.has_item(iItem))
      {
      return 1;
      }
      else
      {
      return 0;
      }
      }
      else if(iSubItem == m_iNameSubItem)
      {
      return m_itema.get_item(iItem).m_iImage;
      }
      else
      return ::user::list::_001GetItemImage(iItem, iSubItem, iListItem);*/

   }

   /*UINT c_cdecl ::filemanager::file_list::_017ThreadProcCreateImageList(LPVOID lpParameter)
   {
   try
   {
   ::SetThreadPriority(
   ::get_current_thread(),
   THREAD_PRIORITY_ABOVE_NORMAL);

   ::filemanager::file_list * plist =
   (::filemanager::file_list *) lpParameter;
   plist->m_bCreateImageList = true;
   ::user::list_column & column = plist->m_columna.GetBySubItem(1);
   //      if(column.m_pil->GetSafeHandle() != NULL)
   //         column.m_pil->DeleteImageList();
   plist->_001CreateImageList(column);
   return 0;
   }
   catch(...)
   {
   return 1;
   }
   }*/


   bool ::filemanager::file_list::TwiHasTranslucency()
   {
      return ::user::list::TwiHasTranslucency() && !m_bCreateImageListRedraw;
   }


   void ::filemanager::file_list::GetSelectedFilePath(stringa & array)
   {
      range range;

      _001GetSelection(range);

      string str;
      //      HRESULT hr;
      for (index i = 0; i < range.get_item_count(); i++)
      {
         item_range & itemrange = range.ItemAt(i);
         for (index iItem = itemrange.get_lower_bound(); iItem <= itemrange.get_upper_bound(); iItem++)
         {
            index iStrict;
            if (m_eview == ViewIcon)
            {
               iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
            }
            else
            {
               iStrict = m_listlayout.m_iaDisplayToStrict[iItem];
            }
            ::userfs::list_item & item = get_fs_list_data()->m_itema.get_item(iStrict);
            if (!item.IsFolder())
            {
               array.add(item.m_strPath);
            }
         }
      }

   }


   void ::filemanager::file_list::_017UpdateList()
   {

      if (m_bStatic)
      {

         ::userfs::list_item item(get_app());

         stringa stra;

         GetFileManager()->data_get(GetFileManager()->get_filemanager_data()->m_pschema->m_dataidStatic, ::core::system::idEmpty, stra);

         for (int32_t i = 0; i < stra.get_size(); i++)
         {

            item.m_flags.unsignalize_all();

            if (Application.dir().is(stra[i]))
            {
               item.m_flags.signalize(::fs::FlagFolder);
            }
            else
            {
            }

            string strPath = stra[i];
            string strName = System.file().title_(strPath);


            item.m_iImage = -1;
            item.m_strPath = strPath;
            item.m_strName = strName;

            get_fs_list_data()->m_itema.add_item(item);

         }

         _001OnUpdateItemCount();

         return;

      }

      if (GetFileManager()->get_filemanager_data()->m_bSetBergedgeTopicFile)
      {
         SetTimer(198477, 230, NULL);
      }


      ::userfs::list_item folder(get_app());

      //      HRESULT hr;
      string strPath = GetFileManagerItem().m_strPath;
      //      LPMALLOC lpmalloc = NULL;
      //      IShellFolder * lpsfDesktop;


      _017UpdateList(strPath);



   }

   /*void ::filemanager::file_list::_017OneLevelUp()
   {
   if(m_lpiidlAbsolute == NULL)
   return;

   single_lock slBrowse(&m_csBrowse, TRUE);
   LPMALLOC lpmalloc = NULL;
   IShellFolder * lpsfDesktop;
   HRESULT hr;

   hr = SHGetMalloc(&lpmalloc);

   hr = SHGetDesktopFolder(&lpsfDesktop);

   LPITEMIDLIST lpiidl = _shell::_017ItemIDListGetFolderParent(m_lpiidlAbsolute);

   _017Browse(lpiidl);

   lpsfDesktop->Release();
   lpmalloc->Release();

   }*/

   void ::filemanager::file_list::_001OnMainPostMessage(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj)
         switch (pbase->m_wparam)
      {
         case MessageMainPostCreateImageListItemStepSetRedraw:
         {
                                                                uint32_t dwNow = get_tick_count();
                                                                if (dwNow - m_dwLastRedraw > 284)
                                                                {
                                                                   m_dwLastRedraw = dwNow;
                                                                   _001RedrawWindow();
                                                                }
         }
            break;
         case MessageMainPostCreateImageListItemRedraw:
         {
                                                         _001RedrawWindow();
         }
            break;
      }
      pbase->set_lresult(0);
      pbase->m_bRet = true;
   }

   ::fs::item & ::filemanager::file_list::GetFileManagerItem()
   {
      return GetFileManager()->get_item();
   }

   void ::filemanager::file_list::_017Synchronize()
   {

      if (m_bStatic)
      {

         _017UpdateList();

         return;

      }

      ::data::lock lock(m_pdata);

      _001HideEditingControls();

      _017Browse(GetFileManagerItem().m_strPath);

   }

   void ::filemanager::file_list::install_message_handling(::message::dispatch *pinterface)
   {
      ::user::form_list::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(MessageMainPost, pinterface, this, &::filemanager::file_list::_001OnMainPostMessage);
      IGUI_WIN_MSG_LINK(WM_HSCROLL, pinterface, this, &::filemanager::file_list::_001OnHScroll);
      IGUI_WIN_MSG_LINK(WM_VSCROLL, pinterface, this, &::filemanager::file_list::_001OnVScroll);
      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &::filemanager::file_list::_001OnShowWindow);

   }

   void ::filemanager::file_list::_001OnDraw(::draw2d::graphics *pdc)
   {
      if (m_iAnimate <= 0)
      {
         // Normal Drawing
         ::user::form_list::_001OnDraw(pdc);
      }
      else
      {
         // Animation Drawing
         rect rectClipBox;
         pdc->GetClipBox(rectClipBox);
         class imaging & imaging = System.visual().imaging();
         //      ::user::list::_001DrawBackground(pdc, rectClipBox);
         imaging.color_blend(
            pdc,
            rectClipBox,
            RGB(255, 255, 255),
            127);

         imaging.bitmap_blend(
            pdc,
            null_point(),
            rectClipBox.size(),
            m_gdibuffer.GetBuffer(),
            null_point(),
            (byte)(255 - m_iAnimate * 25));
      }

   }

   void ::filemanager::file_list::StartAnimation()
   {
      m_iAnimate = 1;
   }

   void ::filemanager::file_list::TakeAnimationSnapshot()
   {
      m_iAnimate = 1;
      ::user::list::_001OnDraw(m_gdibuffer.GetBuffer());
   }

   void ::filemanager::file_list::_017PreSynchronize()
   {
      //TakeAnimationSnapshot();
   }





   void ::filemanager::file_list::_017OpenSelected(bool bOpenFile)
   {
      ::fs::item_array itema;
      index iItemRange, iItem;
      range range;
      _001GetSelection(range);
      for (iItemRange = 0;
         iItemRange < range.get_item_count();
         iItemRange++)
      {
         item_range itemrange = range.ItemAt(iItemRange);
         for (iItem = itemrange.get_lower_bound();
            iItem <= itemrange.get_upper_bound();
            iItem++)
         {
            if (iItem < 0)
               continue;
            if (iItem >= _001GetItemCount())
               continue;
            index iStrict;
            if (m_eview == ViewIcon)
            {
               iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
            }
            else
            {
               iStrict = m_listlayout.m_iaDisplayToStrict[iItem];
            }
            ::userfs::list_item & item = get_fs_list_data()->m_itema.get_item(iStrict);
            if (item.IsFolder())
            {
               _017OpenFolder(new ::fs::item(item));
               break;
            }
            else
            {
               itema.add(new  ::fs::item(item));
            }
         }
      }
      if (bOpenFile && itema.get_size() > 0)
      {
         _017OpenFile(itema);
      }
      _001ClearSelection();
   }

   void ::filemanager::file_list::_017OpenContextMenuSelected()
   {
      ::fs::item_array itema;
      index iItemRange, iItem;
      range range;
      _001GetSelection(range);
      for (iItemRange = 0;
         iItemRange < range.get_item_count();
         iItemRange++)
      {
         item_range itemrange = range.ItemAt(iItemRange);
         for (iItem = itemrange.get_lower_bound();
            iItem <= itemrange.get_upper_bound();
            iItem++)
         {
            if (iItem < 0)
               continue;
            if (iItem >= get_fs_list_data()->m_itema.get_count())
               continue;
            index iStrict;
            if (m_eview == ViewIcon)
            {
               iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
            }
            else
            {
               iStrict = m_listlayout.m_iaDisplayToStrict[iItem];
            }
            if (get_fs_list_data()->m_itema.get_item(iStrict).IsFolder())
            {
               _017OpenContextMenuFolder(new  ::fs::item(get_fs_list_data()->m_itema.get_item(iStrict)));
               break;
            }
            else
            {
               itema.add(new ::fs::item(get_fs_list_data()->m_itema.get_item(iStrict)));
            }
         }
      }
      if (itema.get_size() > 0)
      {
         _017OpenContextMenuFile(itema);
      }
      else
      {
         _017OpenContextMenu();
      }
      _001ClearSelection();
   }

   void ::filemanager::file_list::_017OpenContextMenuFolder(sp(::fs::item) item)
   {
      UNREFERENCED_PARAMETER(item);
   }

   void ::filemanager::file_list::_017OpenContextMenuFile(const ::fs::item_array &itema)
   {
      UNREFERENCED_PARAMETER(itema);
   }

   void ::filemanager::file_list::_017OpenContextMenu()
   {
   }

   void ::filemanager::file_list::_017OpenFolder(sp(::fs::item) item)
   {
      UNREFERENCED_PARAMETER(item);
      ASSERT(FALSE);
   }

   void ::filemanager::file_list::_017OpenFile(const ::fs::item_array &itema)
   {
      UNREFERENCED_PARAMETER(itema);
      ASSERT(FALSE);
   }

   void ::filemanager::file_list::_001OnInitializeForm(sp(::user::control) pcontrol)
   {
      ASSERT(pcontrol != NULL);
      if (pcontrol == NULL)
         return;

      filemanager::file_list_callback * pcallback =
         GetFileManager()->get_filemanager_data()->m_pschema->m_pfilelistcallback;

      sp(BaseButtonControl) pbutton = (pcontrol);
      if (pcallback != NULL && pbutton != NULL)
      {
         pcallback->InitializeActionButton(((int32_t)pcontrol->descriptor().m_id) - 1000, pbutton);
      }
   }

   void ::filemanager::file_list::_001OnButtonAction(
      sp(::user::control) pcontrol)
   {
      filemanager::file_list_callback * pcallback =
         GetFileManager()->get_filemanager_data()->m_pschema->m_pfilelistcallback;

      if (pcallback != NULL)
      {
         ::fs::item item;
         index iItem = pcontrol->GetEditItem();
         index iStrict;
         if (m_eview == ViewIcon)
         {
            iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
         }
         else
         {
            iStrict = m_listlayout.m_iaDisplayToStrict[iItem];
         }
         pcallback->OnButtonAction((int32_t)pcontrol->descriptor().m_id - 1000, new ::fs::item(get_fs_list_data()->m_itema.get_item(iStrict)));
      }
   }

   void ::filemanager::file_list::GetSelected(::fs::item_array &itema)
   {
      index iItemRange, iItem;
      range range;
      _001GetSelection(range);
      index_array iaItem;
      for (iItemRange = 0;
         iItemRange < range.get_item_count();
         iItemRange++)
      {
         item_range itemrange = range.ItemAt(iItemRange);
         for (iItem = max(0, itemrange.get_lower_bound());
            iItem <= itemrange.get_upper_bound();
            iItem++)
         {

            if (iItem < get_fs_list_data()->m_itema.get_count() && !iaItem.contains(iItem))
            {

               iaItem.add(iItem);

               index iStrict;

               if (m_eview == ViewIcon)
               {

                  iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);

               }
               else
               {

                  if (iItem >= m_listlayout.m_iaDisplayToStrict.get_count())
                     continue;

                  iStrict = m_listlayout.m_iaDisplayToStrict[iItem];

               }

               sp(::fs::item) spitem(new ::fs::item);

               spitem->m_strPath = get_fs_list_data()->m_itema.get_item(iStrict).m_strPath;

               spitem->m_flags = get_fs_list_data()->m_itema.get_item(iStrict).m_flags;

               itema.add(spitem);

            }
         }
      }
   }

   void ::filemanager::file_list::_001OnVScroll(signal_details * pobj)
   {
      SCAST_PTR(::message::scroll, pscroll, pobj)
         m_iCreateImageListStep = pscroll->m_nPos;
      m_bRestartCreateImageList = true;
      pobj->m_bRet = false;
   }

   void ::filemanager::file_list::_001OnHScroll(signal_details * pobj)
   {
      pobj->m_bRet = false;
   }

   ::count ::filemanager::file_list::_001GetItemCount()
   {
      return get_fs_list_data()->m_itema.get_count();
   }


   bool ::filemanager::file_list::add_item(const char * pszPath, const char * pszTitle)
   {

      ::userfs::list_item item(get_app());

      item.m_strPath = pszPath;

      item.m_strName = pszTitle;

      if (Application.dir().is(pszPath))
      {

         item.m_flags.signalize(::fs::FlagFolder);

      }

      get_fs_list_data()->m_itema.add_item(item);

      _001OnUpdateItemCount();

      return true;

   }


   sp(image_list) ::filemanager::file_list::GetActionButtonImageList(index i)
   {
      if (i == 0)
      {
         return System.user()->shellimageset().GetImageList16();
      }
      return NULL;
   }

   void ::filemanager::file_list::_001OnFileRename(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      sp(::user::control) pcontrol = _001GetControlBySubItem(m_iNameSubItem);
      range range;
      _001GetSelection(range);
      if (range.get_item_count() == 1 && range.ItemAt(0).get_lower_bound() == range.ItemAt(0).get_upper_bound())
      {
         _001PlaceControl(pcontrol);
      }
   }

   void ::filemanager::file_list::_001OnUpdateFileRename(signal_details * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
         range range;
      _001GetSelection(range);
      pcmdui->m_pcmdui->Enable(
         range.get_item_count() == 1
         && range.ItemAt(0).get_lower_bound() == range.ItemAt(0).get_upper_bound());
      pobj->m_bRet = true;
   }

   void ::filemanager::file_list::_001OnShowWindow(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //      SCAST_PTR(::message::show_window, pshow, pobj)

      db_server * pcentral = dynamic_cast < db_server * > (&System.m_simpledb.db());
      if (pcentral == NULL)
         return;
      //DBFileSystemSizeSet * pset = pcentral->m_pfilesystemsizeset;
      /*if(pshow->m_bShow)
      {
      for(int32_t i = 0; i < m_itema.get_item_count(); i++)
      {
      pset->m_table.add_request(m_itema.get_item(i).m_strPath);
      }
      }
      else
      {
      for(int32_t i = 0; i < m_itema.get_item_count(); i++)
      {
      pset->m_table.remove_request(m_itema.get_item(i).m_strPath);
      }
      }*/
   }

   void ::filemanager::file_list::file_size_add_request(bool bClear)
   {
      UNREFERENCED_PARAMETER(bClear);
      db_server * pcentral = dynamic_cast < db_server * > (&System.m_simpledb.db());
      if (pcentral == NULL)
         return;
      DBFileSystemSizeSet * pset = pcentral->m_pfilesystemsizeset;
      single_lock sl(pset->m_table.m_pmutex, TRUE);

      //      int64_t iSize;
      //      bool bPending;
      for (int32_t i = 0; i < get_fs_list_data()->m_itema.get_count(); i++)
      {
         //pset->get_cache_fs_size(iSize, m_itema.get_item(i).m_strPath, bPending);
      }
   }


   void ::filemanager::file_list::_001InitializeFormPreData()
   {
      ::filemanager::list_data * pdata = GetFileManager()->get_filemanager_data();
      pdata->m_pcallback->OnFileManagerInitializeFormPreData(pdata, GetDlgCtrlId(), this);
   }


   bool ::filemanager::file_list::query_drop(index iDisplayDrop, index iDisplayDrag)
   {
      if (iDisplayDrag < 0)
         return false;
      if (iDisplayDrop < 0)
         return false;
      if (m_iItemDrag != m_iItemDrop)
      {
         index strict;
         if (m_eview == ViewIcon)
            strict = m_iconlayout.m_iaDisplayToStrict[iDisplayDrop];
         else
            strict = m_listlayout.m_iaDisplayToStrict[iDisplayDrop];
         if (strict <= -1)
            return true; // can drop in is_empty place
         else if (strict >= _001GetItemCount())
            return true; // can drop if destination is invalid
         else
         {
            // can drop if destination is folder
            return get_fs_list_data()->m_itema.get_item(strict).IsFolder();
         }
      }
      return false;
   }


   bool ::filemanager::file_list::do_drop(index iDisplayDrop, index iDisplayDrag)
   {
      index strict;
      index strictDrag;
      if (m_eview == ViewIcon)
      {
         strict = m_iconlayout.m_iaDisplayToStrict[iDisplayDrop];
         strictDrag = m_iconlayout.m_iaDisplayToStrict[iDisplayDrag];
      }
      else
      {
         strict = m_listlayout.m_iaDisplayToStrict[iDisplayDrop];
         strictDrag = m_listlayout.m_iaDisplayToStrict[iDisplayDrag];
      }
      if (strict >= 0 && get_fs_list_data()->m_itema.get_item(strict).IsFolder())
      {
         string strPath = get_fs_list_data()->m_itema.get_item(strictDrag).m_strPath;
         string strName = System.file().name_(strPath);
         System.file().move(
            System.dir().path(get_fs_list_data()->m_itema.get_item(strict).m_strPath, strName),
            strPath);
         _017Synchronize();
      }
      else
      {
         ::user::list::do_drop(iDisplayDrop, iDisplayDrag);
      }
      return true;
   }

   COLORREF ::filemanager::file_list::get_background_color()
   {
      if (GetFileManager() != NULL && GetFileManager()->get_filemanager_data()->is_saving())
      {
         return RGB(255, 177, 84);
      }
      else
      {
         return RGB(200, 255, 255);
      }
   }

} // namespace filemanager

