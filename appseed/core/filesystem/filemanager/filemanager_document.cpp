#include "framework.h"


namespace filemanager
{


   document::document(sp(base_application) papp) :
      element(papp),
      ::ca2::data_container_base(papp),
      ::userfs::document(papp)
   {

      command_signalid id;

      connect_update_cmd_ui("levelup", &document::_001OnUpdateLevelUp);
      connect_command("levelup", &document::_001OnLevelUp);
      connect_update_cmd_ui("add_location", &document::_001OnUpdateAddLocation);
      connect_command("add_location", &document::_001OnAddLocation);
      connect_update_cmd_ui("replace_text_in_file_system", &document::_001OnUpdateReplaceText);
      connect_command("replace_text_in_file_system", &document::_001OnReplaceText);
      connect_update_cmd_ui("edit_paste", &document::_001OnUpdateEditPaste);
      connect_command("edit_paste", &document::_001OnEditPaste);
      connect_update_cmd_ui("file_save", &document::_001OnUpdateFileSaveAs);
      connect_command("file_save", &document::_001OnFileSaveAs);
      connect_update_cmd_ui("cancel", &document::_001OnUpdateEditPaste);
      connect_command("cancel", &document::_001OnEditPaste);

   }

   document::~document()
   {
   }


   bool document::on_new_document()
   {

      if (!::user::document::on_new_document())
         return FALSE;



      m_fsset.m_spafsdata.remove_all();


      m_fsset.m_spafsdata.add(Application.fs());


      stringa stra;
      m_fsset.root_ones(stra);


      return TRUE;

   }

   #ifdef DEBUG
   void document::assert_valid() const
   {
      ::user::document::assert_valid();
   }

   void document::dump(dump_context & dumpcontext) const
   {
      ::user::document::dump(dumpcontext);
   }
   #endif //DEBUG

   /////////////////////////////////////////////////////////////////////////////
   // document serialization
   /*
   void document::Serialize(CArchive& ar)
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
   /////////////////////////////////////////////////////////////////////////////
   // document commands

   void document::OnFileManagerBrowse()
   {

      {
         FileManagerViewUpdateHint uh;
         uh.set_type(FileManagerViewUpdateHint::TypeSynchronizePath);
         update_all_views(NULL, 0, &uh);
      }


      FileManagerInterface::OnFileManagerBrowse();

   }

   void document::OpenSelectionProperties()
   {
      {
         FileManagerViewUpdateHint uh;
         uh.set_type(FileManagerViewUpdateHint::TypeOpenSelectionProperties);
         update_all_views(NULL, 0, &uh);
      }

   }


   bool document::_001OnCommand(id id)
   {

      if(get_filemanager_data() != NULL
      && get_filemanager_data()->m_ptemplate != NULL)
      {
//         FileManagerTemplate * ptemplate = get_filemanager_data()->m_ptemplate;
         if(id == get_filemanager_data()->m_ptemplate->m_strLevelUp)
         {
            FileManagerOneLevelUp();
            return true;
         }
      }
      return ::user::document::_001OnCommand(id);
   }

   bool document::_001OnUpdateCmdUi(cmd_ui * pcmdui)
   {
      /*if(pcmdui->m_id == get_filemanager_data()->m_ptemplate->m_strLevelUp)
      {
      FileManagerOnUpdateLevelUp(pcmdui);
      return true;
      }*/
      return ::user::document::_001OnUpdateCmdUi(pcmdui);
   }


   /*bool document::_001OnCmdMsg(BaseCmdMsg * pcmdmsg)
   {
   if (nCode == CN_UPDATE_COMMAND_UI)
   {
   cmd_ui * pcmdui = (cmd_ui *) pExtra;
   if(nID == get_filemanager_data()->m_ptemplate->m_uiLevelUp)
   {
   FileManagerOnUpdateLevelUp(pcmdui);
   return TRUE;
   }
   }
   else
   {
   int32_t nMsg = HIWORD(nCode);
   nCode = LOWORD(nCode);

   // for backward compatibility HIWORD(nCode)==0 is WM_COMMAND
   if (nMsg == 0)
   nMsg = WM_COMMAND;

   if(nMsg == WM_COMMAND)
   {
   if(nID == get_filemanager_data()->m_ptemplate->m_uiLevelUp)
   {
   FileManagerOnLevelUp();
   return TRUE;
   }
   }
   }*/
   /*   return ::user::document::_001OnCmdMsg(pcmdmsg);
   }
   */

   void document::_001OnUpdateLevelUp(signal_details * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
         if(m_item->m_strPath.is_empty())
         {
            pcmdui->m_pcmdui->Enable(FALSE);
         }
         else
         {
            pcmdui->m_pcmdui->Enable(TRUE);
         }
         pobj->m_bRet = true;
   }

   void document::_001OnLevelUp(signal_details * pobj)
   {
      FileManagerOneLevelUp();
      pobj->m_bRet = true;
   }


   void document::_001OnUpdateAddLocation(signal_details * pobj)
   {

      SCAST_PTR(base_cmd_ui, pcmdui, pobj)

      pcmdui->m_pcmdui->Enable(TRUE);

      pobj->m_bRet = true;

   }


   void document::_001OnAddLocation(signal_details * pobj)
   {

      update_all_views(NULL, ::user::view::hint_add_location, NULL);

      pobj->m_bRet = true;

   }


   void document::_001OnUpdateReplaceText(signal_details * pobj)
   {

      SCAST_PTR(base_cmd_ui, pcmdui, pobj)

      pcmdui->m_pcmdui->Enable(TRUE);

      pobj->m_bRet = true;

   }


   void document::_001OnReplaceText(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      update_all_views(NULL, 89124593, NULL);
   }

   void document::_001OnUpdateEditPaste(signal_details * pobj)
   {
//      SCAST_PTR(base_cmd_ui, pcmdui, pobj)

//         pcmdui->m_pcmdui->Enable(System.m_strCopy.is_empty());
      pobj->m_bRet = true;
   }

   void document::_001OnEditPaste(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //System.file().paste(get_filemanager_data()->GetFileManager()->get_item().m_strPath, System.m_strCopy);
      //update_all_views(NULL, 123, NULL);
      //pobj->m_bRet = true;
   }

   void document::_001OnUpdateFileSaveAs(signal_details * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
         pcmdui->m_pcmdui->Enable(TRUE);
   }

   void document::_001OnFileSaveAs(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);

      if(get_filemanager_data()->is_saving())
      {
         FileManagerViewUpdateHint uh;
         uh.m_pmanager = this;
         uh.set_type(FileManagerViewUpdateHint::TypeSaveAsOK);
         update_all_views(NULL, 0, &uh);
      }

   }

   void document::Initialize(bool bMakeVisible)
   {
      string str;
      str.Format("document(%s)", get_filemanager_data()->m_strDISection);
      m_dataid = str;




      CreateViews();

      FileManagerViewUpdateHint uh;


      uh.m_pmanager = this;

      if(data_get("InitialBrowsePath", ::ca2::system::idEmpty, str))
      {

         if(str == "machinefs://")
         {

            id idMachine;

            #ifdef LINUX
               idMachine = "Linux";
            #elif defined(WINDOWSEX)
               idMachine = "Windows Desktop";
            #endif

            if(data_get("InitialBrowsePath", idMachine, str))
            {
               FileManagerBrowse(str);
            }
            else
            {
               FileManagerBrowse("");
            }
         }
         else
         {
            FileManagerBrowse(str);
         }
      }
      else
      {
         FileManagerBrowse("");
      }

      uh.set_type(FileManagerViewUpdateHint::TypeCreateBars);
      update_all_views(NULL, 0, &uh);

      if(bMakeVisible)
      {
         uh.set_type(FileManagerViewUpdateHint::TypePop);
         update_all_views(NULL, 0, &uh);
      }

   }

   void document::OpenFolder(sp(::fs::item) item)
   {
      FileManagerBrowse(item);
   }

   void document::CreateViews()
   {

      FileManagerViewUpdateHint uh;
      uh.m_pmanager = this;
      uh.set_type(FileManagerViewUpdateHint::TypeCreateViews);
      update_all_views(NULL, 0, &uh);

      uh.set_type(FileManagerViewUpdateHint::TypeInitialize);
      uh.m_uiId = get_filemanager_data()->m_iDocument;
      uh.m_pmanager = this;
      update_all_views(NULL, 0, &uh);

      uh.set_type(FileManagerViewUpdateHint::TypeSynchronizeLocations);
      uh.m_uiId = get_filemanager_data()->m_iDocument;
      uh.m_pmanager = this;
      update_all_views(NULL, 0, &uh);


   }

   void document::PopViews()
   {
      FileManagerViewUpdateHint uh;
      uh.m_uiId = get_filemanager_data()->m_iDocument;
      uh.m_pmanager = this;
      uh.set_type(FileManagerViewUpdateHint::TypeCreateBars);
      update_all_views(NULL, 0, &uh);

      uh.set_type(FileManagerViewUpdateHint::TypePop);

      update_all_views(NULL, 0, &uh);
   }

   bool document::HandleDefaultFileManagerItemCmdMsg(BaseCmdMsg * pcmdmsg, ::fs::item_array & itema)
   {
      if(pcmdmsg->m_etype == BaseCmdMsg::type_cmdui)
      {
         if(get_filemanager_data()->m_pcallback->GetFileManagerItemCallback(
            get_filemanager_data(), pcmdmsg->m_pcmdui->m_id, itema))
         {
            get_filemanager_data()->m_pcallback->OnFileManagerItemUpdate(
               get_filemanager_data(), pcmdmsg->m_pcmdui, itema);
            return TRUE;
         }
      }
      else
      {
         if(get_filemanager_data()->m_pcallback->GetFileManagerItemCallback(
            get_filemanager_data(), pcmdmsg->m_id, itema))
         {
            get_filemanager_data()->m_pcallback->OnFileManagerItemCommand(
               get_filemanager_data(), pcmdmsg->m_id, itema);
            return true;
         }
      }
      return false;
   }


   sp(file_manager_operation_document) document::get_operation_doc(bool bSwitch)
   {
      FileManagerTabView * ptabview = get_typed_view < FileManagerTabView > ();
      if(ptabview == NULL)
         return NULL;

      ASSERT(ptabview != NULL);
      if(ptabview != NULL)
      {
         if(bSwitch)
         {
            ptabview->set_cur_tab_by_id(200000);
         }
         else
         {
            ptabview->ensure(200000);
         }
         return 
            (ptabview->get(200000)->m_pdoc);
      }
      return NULL;
   }

   void document::GetActiveViewSelection(::fs::item_array & itema)
   {
      FileManagerViewUpdateHint uh;
      uh.set_type(FileManagerViewUpdateHint::TypeGetActiveViewSelection);
      update_all_views(NULL, 0, &uh);
      itema = uh.m_itemaSelected;
   }


   void document::FileManagerSaveAs(sp(::user::document_interface) pdocument)
   {
      FileManagerInterface::FileManagerSaveAs(pdocument);
      FileManagerViewUpdateHint uh;
      uh.m_pmanager = this;
      uh.set_type(FileManagerViewUpdateHint::TypeSaveAsStart);
      update_all_views(NULL, 0, &uh);
      uh.set_type(FileManagerViewUpdateHint::TypeCreateBars);
      update_all_views(NULL, 0, &uh);
   }




   ::filemanager::data * document::get_filemanager_data()
   {


      return m_spfilemanagerdata;


   }


   ::fs::data * document::get_fs_data()
   {


      return &m_fsset;


   }


   bool document::set_filemanager_data(::filemanager::data * pdata)
   {


      m_spfilemanagerdata = pdata;


      return true;


   }


} // namespace filemanager




