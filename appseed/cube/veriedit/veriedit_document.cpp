#include "StdAfx.h"

namespace veriedit
{

   document::document(::ca::application * papp) :
      ca(papp),
      ::ca::data_container(papp),
      ::document(papp),
      ::userbase::document(papp),
      m_file(papp),
      m_fileSave(papp)
   {
      m_pview = NULL;
      m_iBranch = 0;

      string str = "Thomas Borregaard Sørensen � minha Vida Eterna, meu Cora��o Eterno, Todo meu tesouro eterno, meu Universo eterno, meu tudo eterno!!";
      str += "Assinado Camilo Sasuke Tsumanuma.";
      str += "Visible or Note?";

      //command_signalid id;
      //gen::signalid * pid;


      /*id.m_id = System.id("edit_undo";
      pid = m_signalidaCommand.get(&id);
      id.m_id = "edit_redo";
      pid = m_signalidaCommand.get(&id);
      id.m_id = "file_save";
      pid = m_signalidaCommand.get(&id);*/
      connect_update_cmd_ui("file_save", &document::_001OnUpdateFileSave);
      connect_command("file_save", &document::_001OnFileSave);
      connect_update_cmd_ui("edit_cut", &document::_001OnUpdateEditCut);
      connect_command("edit_cut", &document::_001OnEditCut);
      connect_update_cmd_ui("edit_copy", &document::_001OnUpdateEditCopy);
      connect_command("edit_copy", &document::_001OnEditCopy);
      connect_update_cmd_ui("edit_paste", &document::_001OnUpdateEditPaste);
      connect_command("edit_paste", &document::_001OnEditPaste);
      connect_update_cmd_ui("file_save_as", &document::_001OnUpdateFileSaveAs);
      connect_command("file_save_as", &document::_001OnFileSaveAs);
      set_data(new user::plain_text_data(papp));
   }



   BOOL document::on_new_document()
   {
      ::ca::data::writing writing(get_data());
      if (!::userbase::document::on_new_document())
         return FALSE;

      set_path_name(System.file().time_square());
      if(m_file->IsOpened())
         m_file->close();
      if(!m_file->open(get_path_name(), ::ex1::file::type_binary | ::ex1::file::mode_read_write |
         ::ex1::file::shareDenyNone))
         return FALSE;

      update_all_views(NULL, 11);

      return TRUE;
   }

   document::~document()
   {
   }


    
   #ifdef _DEBUG
   void document::assert_valid() const
   {
      ::userbase::document::assert_valid();
   }

   void document::dump(dump_context & dumpcontext) const
   {
      ::userbase::document::dump(dumpcontext);
   }
   #endif //_DEBUG


   bool document::on_open_document(var varFile)
   {
      ::ca::data::writing writing(get_data());
      if(m_file->IsOpened())
         m_file->close();
      ::ex1::file_exception_sp fe(get_app());
      if(!m_file->open(varFile, ::ex1::file::type_binary | ::ex1::file::mode_read_write | ::ex1::file::shareDenyNone, &fe))
      {
         string strMessage;
         string strError;
         fe->GetErrorMessage(strError);
         strMessage.Format("Failed to open file. Error: %s", strError);
         System.simple_message_box(NULL, strMessage);
         return FALSE;
      }
      set_path_name(varFile);
      update_all_views(NULL, 11);
      return TRUE;
   }

   void document::reload()
   {
      ::ca::data::writing writing(get_data());
      if(m_file->IsOpened())
         m_file->close();
      if(!m_file->open(get_path_name(), ::ex1::file::type_binary | ::ex1::file::mode_read_write |
         ::ex1::file::shareDenyNone))
         return;
      update_all_views(NULL, 123);
   }


   BOOL document::on_save_document(const char * lpszPathName)
   {
      ::ca::data::writing writing(get_data());
      string strTemp;
      strTemp = System.file().time_square(System.file().title_(lpszPathName), System.file().extension(lpszPathName));
      if(!System.file().exists(strTemp))
         return FALSE;
      if(!m_fileSave->open(strTemp, ::ex1::file::type_binary | ::ex1::file::mode_read_write))
      {
         ::DeleteFile(strTemp);
         return FALSE;
      }
      update_all_views(NULL, 12345);
      if(m_file->IsOpened())
      {
         m_file->close();
      }
      if(m_fileSave->IsOpened())
      {
         m_fileSave->close();
      }
      try
      {
         System.file().copy(lpszPathName, strTemp, FALSE);
      }
      catch(...)
      {
         ::DeleteFile(strTemp);
         return FALSE;
      }
      ::DeleteFile(strTemp);
      if(m_fileSave->IsOpened())
      {
         m_fileSave->close();
      }
      reload();
      return TRUE;
   }

   void document::_001OnUpdateFileSave(gen::signal_object * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
      pcmdui->m_pcmdui->Enable(TRUE);
   }

   void document::_001OnUpdateFileSaveAs(gen::signal_object * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
      pcmdui->m_pcmdui->Enable(FALSE);
   }

   void document::_001OnUpdateFileNew(gen::signal_object * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
      pcmdui->m_pcmdui->Enable(TRUE);
   }

   void document::_001OnUpdateFilePut(gen::signal_object * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
      pcmdui->m_pcmdui->Enable(TRUE);
   }


   void document::_001OnUpdateEditCut(gen::signal_object * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
      pcmdui->m_pcmdui->Enable(TRUE);
   }

   void document::_001OnUpdateEditCopy(gen::signal_object * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
      pcmdui->m_pcmdui->Enable(TRUE);
   }

   void document::_001OnUpdateEditPaste(gen::signal_object * pobj)
   {
      SCAST_PTR(base_cmd_ui, pcmdui, pobj)
      pcmdui->m_pcmdui->Enable(TRUE);
   }

   void document::_001OnFileSave(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      ::userbase::document::on_file_save();
   }

   void document::_001OnFileSaveAs(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      Application.::veriedit::application::m_ppaneview->set_cur_tab_by_id(veriedit::PaneViewFileManager);
      Application.::veriedit::application::m_ppaneview->m_pfilemanagerdoc->FileManagerSaveAs(this);
   }

   void document::_001OnFileNew(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      ::userbase::document::on_new_document();
   }

   void document::_001OnEditCut(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void document::_001OnEditCopy(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void document::_001OnEditPaste(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void document::_001OnFilePut(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      ::userbase::document::on_file_save();
      Put();
   }

   void document::Put()
   {
      string strVotagusFolder;
      System.dir().votagus(strVotagusFolder);
      string strCommon;
      string strRemote;
      strCommon = System.dir().path(strVotagusFolder, "seed\\ca2\\fontopus\\net\\ds\\common");
      if(strCommon.CompareNoCase(get_path_name().Mid(0, strCommon.get_length())) == 0)
      {
         strCommon = get_path_name().Mid(strCommon.get_length());
         strCommon.replace("\\", "/");
         strRemote = "seed/ca2/fontopus/net/ds/common" + strCommon;
         FtpPut(get_path_name(), strRemote);
      }
   }

   void document::FtpPut(const char * lpcszLocal, const char * lpcszRemote)
   {
      UNREFERENCED_PARAMETER(lpcszLocal);
      UNREFERENCED_PARAMETER(lpcszRemote);
      throw not_implemented_exception();
/*      CInternetSession sess("MyProgram/1.0");

      CFtpConnection* pConnect = NULL;

       try
       {
           // Request a connection to ftp.microsoft.com. Default
           // parameters mean that we'll try with username = ANONYMOUS
           // and password set to the machine name @ domain name
           pConnect = sess.GetFtpConnection("votagus.net", "votagus", "1p.t5mL.1..");

           pConnect->PutFile(lpcszLocal, lpcszRemote);

       }
       catch (CInternetException* pEx)
       {
           string str;
           pEx->GetErrorMessage(str);
           printf_s("ERROR!  %s\n", str);
           pEx->Delete();
        }

       // if the connection is open, close it
       if (pConnect != NULL) 
       {
           pConnect->close();
           delete pConnect;
       }
*/
   }


   edit_view * document::get_edit_view()
   {
      return get_typed_view < edit_view > ();
   }

} // namespace veriedit