#include "framework.h"


namespace user
{


   document_interface::document_interface() :
      ::ca::data_container_base(::null()),
      m_mutex(::null())
   {
      m_pdocumentemplate      = ::null();
      m_bModified             = FALSE;
      m_bAutoDelete           = TRUE;       // default to auto delete document_interface
      m_bEmbedded             = FALSE;        // default to file-based document_interface
      m_bNew                  = false;

      ASSERT(m_viewptra.is_empty());


   }

   document_interface::~document_interface()
   {
      // do not call delete_contents here !
#ifdef DEBUG
      if (is_modified())
         TRACE(::ca::trace::category_AppMsg, 0, "Warning: destroying an unsaved document_interface.\n");
#endif

      // there should be no views left!
      disconnect_views();
      ASSERT(m_viewptra.is_empty());

      if (m_pdocumentemplate != ::null())
         m_pdocumentemplate->remove_document(this);
      ASSERT(m_pdocumentemplate == ::null());     // must be detached

   }

   void document_interface::update_title()
   {

   }





   void document_interface::on_final_release()
   {
      on_close_document();  // may 'delete this'
   }

   void document_interface::disconnect_views()
   {
      single_lock sl(&m_mutex, true);
      for(index index = 0; index < m_viewptra.get_count(); index++)
      {
         ::view * pview = (::view *)m_viewptra[index];
         ASSERT_VALID(pview);
         ASSERT_KINDOF(::view, pview);
         pview->m_spdocument->release();
      }
      m_viewptra.remove_all();
   }

   void document_interface::on_alloc(::ca::applicationsp papp)
   {
      set_app(papp);
   }

   /////////////////////////////////////////////////////////////////////////////
   // document_interface attributes, general services



   void document_interface::set_title(const char * lpszTitle)
   {
      m_strTitle = lpszTitle;
      update_frame_counts();        // will cause name change in views
   }

   void document_interface::delete_contents()
   {
   }

   /////////////////////////////////////////////////////////////////////////////
   // Closing documents or views

   void document_interface::on_changed_view_list(single_lock * psl)
   {
      single_lock sl(&m_mutex, false);
      if(psl == ::null() || psl->m_psyncobject != &m_mutex)
         psl = &sl;
      psl->lock();
      // if no more views on the document_interface, delete ourself
      // not called if directly closing the document_interface or terminating the cast
      if (m_viewptra.is_empty() && m_bAutoDelete)
      {
         on_close_document(psl);
         return;
      }

      // update the frame counts as needed
      update_frame_counts(psl);
   }

   void document_interface::update_frame_counts(single_lock * psl)
      // assumes 1 doc per frame
   {
      single_lock sl(&m_mutex, false);
      if(psl == ::null() || psl->m_psyncobject != &m_mutex)
         psl = &sl;
      psl->lock();
      // walk all frames of views (mark and sweep approach)
      ::count count = get_view_count();
      index index;
      for(index = 0; index < count; index++)
      {
         ::view * pview = get_view(index);
         ASSERT_VALID(pview);
         // trans      ASSERT(::IsWindow(pview->get_handle()));
         if (pview->IsWindowVisible())   // Do not ::count invisible windows.
         {
            sp(frame_window) pFrame = pview->GetParentFrame();
            if (pFrame != ::null())
               pFrame->m_nWindow = -1;     // unknown
         }
      }

      // now do it again counting the unique ones
      int32_t nFrames = 0;
      count = get_view_count();
      for(index = 0; index < count; index++)
      {
         ::view * pview = get_view(index);
         ASSERT_VALID(pview);
         // trans      ASSERT(::IsWindow(pview->get_handle()));
         if (pview->IsWindowVisible())   // Do not ::count invisible windows.
         {
            sp(frame_window) pFrame = pview->GetParentFrame();
            if (pFrame != ::null() && pFrame->m_nWindow == -1)
            {
               ASSERT_VALID(pFrame);
               // not yet counted (give it a 1 based number)
               pFrame->m_nWindow = ++nFrames;
            }
         }
      }

      // lastly walk the frames and update titles (assume same order)
      // go through frames updating the appropriate one
      int32_t iFrame = 1;
      count = get_view_count();
      for(index = 0; index < count; index++)
      {
         ::view * pview = get_view(index);
         ASSERT_VALID(pview);
         // trans      ASSERT(::IsWindow(pview->get_handle()));
         if (pview->IsWindowVisible())   // Do not ::count invisible windows.
         {
            sp(frame_window) pFrame = pview->GetParentFrame();
            if (pFrame != ::null() && pFrame->m_nWindow == iFrame)
            {
               ASSERT_VALID(pFrame);
               if (nFrames == 1)
                  pFrame->m_nWindow = 0;      // the only one of its kind
               pFrame->post_simple_command(::user::interaction::simple_command_update_frame_title, TRUE);
               iFrame++;
            }
         }
      }
   }

   bool document_interface::can_close_frame(sp(frame_window) pFrameArg)
      // permission to close all views using this frame
      //  (at least one of our views must be in this frame)
   {
      single_lock sl(&m_mutex, true);
      ASSERT_VALID(pFrameArg);
      UNUSED(pFrameArg);   // unused in release builds

      ::count count = get_view_count();
      for(index index = 0; index < count; index++)
      {
         ::view * pview = get_view(index);
         ASSERT_VALID(pview);
         sp(frame_window) pFrame = pview->GetParentFrame();
         // assume frameless views are ok to close
         if (pFrame != ::null())
         {
            // assumes 1 document_interface per frame
            ASSERT_VALID(pFrame);
            if (pFrame->m_nWindow > 0)
               return TRUE;        // more than one frame refering to us
         }
      }
      sl.unlock();
      // otherwise only one frame that we know about
      return save_modified();
   }

   void document_interface::pre_close_frame(sp(frame_window) /*pFrameArg*/)
   {
      // default does nothing
   }

   /////////////////////////////////////////////////////////////////////////////
   // File/Path commands

   void document_interface::set_path_name(var varFile, bool bAddToMRU)
   {
      UNREFERENCED_PARAMETER(bAddToMRU);
      string strPathName;
      if(varFile.get_type() == var::type_propset && varFile.propset()["url"].get_string().has_char())
      {
         strPathName = varFile.propset()["url"];
      }
      else if(varFile.ca < ::ca::file > () != ::null())
      {
         strPathName = System.datetime().international().get_gmt_date_time() + "." + get_document_template()->m_set["default_extension"];
      }
      else
      {
         strPathName = varFile;
      }
      // store the path fully qualified
      //char szFullPath[_MAX_PATH];
      //if ( lstrlen(lpszPathName) >= _MAX_PATH )
      //{
      //   ASSERT(FALSE);
      // ca API requires paths with length < _MAX_PATH
      // No other way to handle the error from a void function
      //   ::ca::ThrowFileException(::ca::file_exception::badPath);
      //}

      //   if( ::ca::FullPath(szFullPath, lpszPathName) == FALSE )
      //   {
      //      ASSERT(FALSE);
      // ca API requires paths with length < _MAX_PATH
      // No other way to handle the error from a void function
      //      ::ca::ThrowFileException(::ca::file_exception::badPath);
      //   }

      // store the path fully qualified
      string strFullPath;
      //      System.file_system().FullPath(strFullPath, strPathName);
      strFullPath = strPathName;
      m_strPathName = strFullPath;
      //!m_strPathName.is_empty());       // must be set to something
      m_bEmbedded = FALSE;

      // set the document_interface title based on path name
      string strTitle;
      strTitle = System.file().name_(strFullPath);
      set_title(strTitle);

      //string strPathName;
      //::ca::international::Utf8ToAcp(strPathName, m_wstrPathName);
      // add it to the file MRU list
      /* xxx if (bAddToMRU)
      guserbase::get(get_app())->AddToRecentFileList(lpszPathName);*/

      /*   ASSERT_VALID(this);
      m_bNew = false;


      m_strPathName = lpszPathName;
      ASSERT(!m_strPathName.is_empty());       // must be set to something
      m_bEmbedded = FALSE;
      ASSERT_VALID(this);

      // set the document_interface title based on path name
      string strTitle = System.file().title_(m_strPathName);
      set_title(strTitle);


      ASSERT_VALID(this);

      m_bNew = false;*/
   }

   /////////////////////////////////////////////////////////////////////////////
   // Standard file menu commands

   void document_interface::on_file_close()
   {
      if (!save_modified())
         return;

      // shut it down
      on_close_document();
      // this should destroy the document_interface
   }

   void document_interface::on_file_save()
   {
      do_file_save();
   }

   void document_interface::on_file_save_as()
   {

      if(!do_save(var(var::type_empty)))
      {

         TRACE(::ca::trace::category_AppMsg, 0, "Warning: File save-as failed.\n");

      }


   }

   bool document_interface::do_file_save()
   {

      if(is_new_document() || System.file_is_read_only(m_strPathName))
      {

         // we do not have read-write access or the file does not (now) exist

         if(!do_save(var(var::type_empty)))
         {

            TRACE(::ca::trace::category_AppMsg, 0, "Warning: File save with new name failed.\n");

            return FALSE;

         }

      }
      else
      {

         if(!do_save(m_strPathName))
         {

            TRACE(::ca::trace::category_AppMsg, 0, "Warning: File save failed.\n");

            return FALSE;

         }

      }

      return TRUE;

   }

   bool document_interface::do_save(var varFile, bool bReplace)
      // Save the document_interface data to a file
      // lpszPathName = path name where to save document_interface file
      // if lpszPathName is ::null() then the ::fontopus::user will be prompted (SaveAs)
      // note: lpszPathName can be different than 'm_strPathName'
      // if 'bReplace' is TRUE will change file name if successful (SaveAs)
      // if 'bReplace' is FALSE will not change path name (SaveCopyAs)
   {

      var newName = varFile;

      if(newName.is_empty() || is_new_document())
      {
         document_template * ptemplate = get_document_template();
         ASSERT(ptemplate != ::null());

         newName = m_strPathName;
         if (bReplace && (newName.is_empty() || is_new_document()))
         {
            newName = m_strTitle;
            // check for dubious filename
            strsize iBad = newName.get_string().FindOneOf(":/\\");
            if (iBad != -1)
               newName = newName.get_string().Left(iBad);

            // append the default suffix if there is one
            string strExt;
            if (ptemplate->GetDocString(strExt, document_template::filterExt) &&
               !strExt.is_empty())
            {
               ASSERT(strExt[0] == '.');
               strsize iStart = 0;
               newName += strExt.Tokenize(";", iStart);
            }
         }

         if (!System.do_prompt_file_name(newName, 0 /*bReplace ? __IDS_SAVEFILE : __IDS_SAVEFILECOPY*/, 0 /*OFN_HIDEREADONLY | OFN_PATHMUSTEXIST */, FALSE, ptemplate, this))
            return FALSE;       // don't even attempt to save

      }

      wait_cursor wait(get_app());

      if(!on_save_document(newName))
      {
         if(varFile.is_empty())
         {
            // be sure to delete the file
            try
            {
               System.file().del(newName);
            }
            catch(base_exception * pe)
            {
               TRACE(::ca::trace::category_AppMsg, 0, "Warning: failed to delete file after failed SaveAs.\n");
               pe->Delete();
            }

         }
         return FALSE;
      }

      // reset the title and change the document_interface name
      if (bReplace)
         set_path_name(newName);

      return TRUE;        // success
   }

   bool document_interface::save_modified()
   {
      if (!is_modified())
         return TRUE;        // ok to continue

      // get name/title of document_interface
      string name;
      if (m_strPathName.is_empty())
      {
         name = m_strTitle;
         if(name.is_empty())
         {
            name = System.load_string("Untitled");
         }
      }
      else
      {
         // get name based on file title of path name
         name = System.file().name_(m_strPathName);
      }

      string prompt;
      prompt = System.load_string("MessageBoxChangedFileAskToSave");
      prompt.replace("%1", name);
      switch (Application.simple_message_box(::null(), prompt, MB_YESNOCANCEL))
      {
      case IDCANCEL:
         return FALSE;       // don't continue

      case IDYES:
         // If so, either Save or Update, as appropriate
         if (!do_file_save())
            return FALSE;       // don't continue
         break;

      case IDNO:
         // If not saving changes, revert the document_interface
         break;

      default:
         ASSERT(FALSE);
         break;
      }
      return TRUE;    // keep going
   }

   HMENU document_interface::GetDefaultMenu()
   {
      return ::null();    // just use original default
   }

   HACCEL document_interface::GetDefaultAccelerator()
   {
      return ::null();    // just use original default
   }

   void document_interface::report_save_load_exception(const char * lpszPathName, base_exception* e, bool bSaving, const char * nIDPDefault)
   {

      try
      {

         UNREFERENCED_PARAMETER(bSaving);
         UNREFERENCED_PARAMETER(nIDPDefault);
         //   UINT nIDP = nIDPDefault;
         //   UINT nHelpContext = nIDPDefault;
         string prompt;

         if (e != ::null())
         {
            if (base < user_exception >::bases(e))
               return; // already reported

            /*if (base < CArchiveException >::bases(e))
            {
            switch (((CArchiveException*)e)->m_cause)
            {
            case CArchiveException::badSchema:
            case CArchiveException::badClass:
            case CArchiveException::badIndex:
            case CArchiveException::endOfFile:
            nIDP = __IDP_FAILED_INVALID_FORMAT;
            break;
            default:
            break;
            }
            }
            else*/ if (base < ::ca::file_exception >::bases(e))
            {
               ::ca::file_exception * pfe = dynamic_cast < ::ca::file_exception * > (e);
               // throw not_implemented(get_app());
               TRACE(::ca::trace::category_AppMsg, 0, "Reporting file I/O exception on Save/Load with lOsError = $%lX.\n",
                  pfe->m_lOsError);


               if (pfe->m_strFileName.is_empty())
                  pfe->m_strFileName = lpszPathName;

               if (!pfe->get_error_message(prompt))
               {
                  /*               switch (((::ca::file_exception_sp*)e)->m_cause)
                  {
                  case ::ca::file_exception::fileNotFound:
                  case ::ca::file_exception::badPath:
                  nIDP = __IDP_FAILED_INVALID_PATH;
                  break;
                  case ::ca::file_exception::diskFull:
                  nIDP = __IDP_FAILED_DISK_FULL;
                  break;
                  case ::ca::file_exception::accessDenied:
                  nIDP = bSaving ? __IDP_FAILED_ACCESS_WRITE :
                  __IDP_FAILED_ACCESS_READ;
                  break;

                  case ::ca::file_exception::badSeek:
                  case ::ca::file_exception::generic:
                  case ::ca::file_exception::tooManyOpenFiles:
                  case ::ca::file_exception::invalidFile:
                  case ::ca::file_exception::hardIO:
                  case ::ca::file_exception::directoryFull:
                  break;

                  default:
                  break;*/
                  //     }
               }
               prompt.ReleaseBuffer();
            }
         }

         if (prompt.is_empty())
         {
            string strTitle = System.file().title_(lpszPathName);
            //throw not_implemented(get_app());
            /*
            ::ca::FormatString1(prompt, nIDP, strTitle);*/
         }

         //System.simple_message_box(prompt, MB_ICONEXCLAMATION, nHelpContext);
         System.simple_message_box(::null(), prompt, MB_ICONEXCLAMATION);

      }
      catch(...)
      {

      }

   }


   bool document_interface::on_new_document()
   {
#ifdef DEBUG
      if(is_modified())
         TRACE(::ca::trace::category_AppMsg, 0, "Warning: on_new_document replaces an unsaved document_interface.\n");
#endif

      delete_contents();
      m_strPathName.Empty();      // no path name yet
      set_modified_flag(FALSE);     // make clean
      m_bNew = true;
      return true;
   }

   bool document_interface::on_open_document(var varFile)
   {
#ifdef DEBUG
      if (is_modified())
         TRACE(::ca::trace::category_AppMsg, 0, "Warning: on_open_document replaces an unsaved document_interface.\n");
#endif

      ::ca::byte_stream spfile;

      try
      {

         spfile = Application.file().get_byte_stream(varFile, ::ca::file::mode_read | ::ca::file::shareDenyWrite | ::ca::file::type_binary);

         /*if(::ca::str::begins_ci(varFile, "uifs://"))
         {
         spfile = ifs(get_app(), "").get_file(varFile, ::ca::file::mode_read | ::ca::file::shareDenyWrite | ::ca::file::type_binary, &fe);
         }
         else
         {
         spfile = System.fs()->get_file(varFile, ::ca::file::mode_read | ::ca::file::shareDenyWrite | ::ca::file::type_binary, &fe);
         }*/
      }
      catch(base_exception & e)
      {
         report_save_load_exception(varFile, &e, FALSE, "__IDP_FAILED_TO_OPEN_DOC");
         return FALSE;
      }

      delete_contents();
      set_modified_flag();  // dirty during de-serialize

      try
      {
         wait_cursor wait(get_app());
         read(spfile);     // load me
         spfile.close();
      }
      catch(base_exception & e)
      {
         spfile.close();
         delete_contents();   // remove failed contents

         try
         {
            report_save_load_exception(varFile, &e, FALSE, "__IDP_FAILED_TO_OPEN_DOC");
         }
         catch(...)
         {
         }
         return FALSE;
      }

      set_modified_flag(FALSE);     // start off with unmodified

      return TRUE;
   }

   bool document_interface::on_save_document(var varFile)
   {

      ::ca::byte_stream spfile;

      try
      {

         spfile = Application.file().get_byte_stream(varFile, ::ca::file::defer_create_directory | ::ca::file::mode_create | ::ca::file::mode_write | ::ca::file::shareExclusive);

      }
      catch(base_exception & e)
      {

         report_save_load_exception(varFile, &e, TRUE, "__IDP_INVALID_FILENAME");

         return false;

      }


      try
      {

         wait_cursor wait(get_app());

         if(varFile["xmledit"].ca < ::primitive::memory_file > () != ::null())
         {

         }
         else
         {

            write(spfile);

         }

         spfile.close();

      }
      catch(base_exception & e)
      {

         report_save_load_exception(varFile, &e, TRUE, "__IDP_FAILED_TO_SAVE_DOC");

         return false;

      }


      set_modified_flag(FALSE);     // back to unmodified

      return true;        // success
   }

   void document_interface::on_close_document(single_lock * psl)
      // must close all views now (no prompting) - usually destroys this
   {
      single_lock sl(&m_mutex, false);
      if(psl == ::null() || psl->m_psyncobject != &m_mutex)
         psl = &sl;
      psl->lock();
      // destroy all frames viewing this document_interface
      // the last destroy may destroy us
      bool bAutoDelete = m_bAutoDelete;
      m_bAutoDelete = FALSE;  // don't destroy document_interface while closing views
      for(index index = 0; index < m_viewptra.get_count(); index++)
      {
         // get frame attached to the ::view
         ::view * pview = m_viewptra[index];
         ASSERT_VALID(pview);
         sp(frame_window) pFrame = pview->GetParentFrame();

         if(pFrame != ::null())
         {
            // and close it
            pre_close_frame(pFrame);
            pFrame->DestroyWindow();
            // will destroy the ::view as well
         }
      }
      m_viewptra.remove_all();
      m_bAutoDelete = bAutoDelete;
      psl->unlock();

      // clean up contents of document_interface before destroying the document_interface itself
      delete_contents();

      release();
   }

   void document_interface::on_idle()
   {
      // default does nothing
   }

   /////////////////////////////////////////////////////////////////////////////
   // ::view operations

   void document_interface::add_view(::view * pview)
   {
      single_lock sl(&m_mutex, true);
      ASSERT_VALID(pview);
      ASSERT(pview->::view::get_document() == ::null()); // must not be already attached
      if(m_viewptra.add_unique(pview))
      {
         pview->m_spdocument = this;
         on_changed_view_list();    // must be the last thing done to the document_interface
      }
   }

   void document_interface::remove_view(::view * pview)
   {
      single_lock sl(&m_mutex, true);
      ASSERT_VALID(pview);
      ASSERT(pview->::view::get_document() == this); // must be attached to us
      if(m_viewptra.remove(pview) > 0)
      {
         pview->m_spdocument = ::ca::null();
         on_changed_view_list();    // must be the last thing done to the document_interface
      }
   }

   ::count document_interface::get_view_count() const
   {
      return m_viewptra.get_count();
   }

   ::view * document_interface::get_view(index index) const
   {
      single_lock sl(&((document_interface *) this)->m_mutex, true);
      if(index < 0 || index >= m_viewptra.get_count())
         return ::null();
      ::view * pview = (::view *) m_viewptra[index];
      ASSERT_KINDOF(::view, pview);
      return pview;
   }

   void document_interface::update_all_views(::view * pSender, LPARAM lHint, ::ca::object* pHint)
      // walk through all views
   {
      ASSERT(pSender == ::null() || !m_viewptra.is_empty());
      // must have views if sent by one of them

      ::count count = get_view_count();
      for(index index = 0; index < count; index++)
      {
         ::view * pview = get_view(index);
         ASSERT_VALID(pview);
         if (pview != pSender)
            pview->on_update(pSender, lHint, pHint);
      }
   }

   void document_interface::send_update(::view * pSender, LPARAM lHint, ::ca::object* pHint)
      // walk through all views
   {
      ASSERT(pSender == ::null() || !m_viewptra.is_empty());
      // must have views if sent by one of them

      update * pupdate;
      ::count count = get_view_count();
      for(index index = 0; index < count; index++)
      {
         ::view * pview = get_view(index);

         pupdate = new update;
         pupdate->m_pSender = pSender;
         pupdate->m_lHint = lHint;
         pupdate->m_pHint = pHint;
         if (pview != pSender)
            pview->send_message(WM_VIEW, 0, (LPARAM) pupdate);
      }
   }

   void document_interface::send_initial_update()
      // walk through all views and call OnInitialUpdate
   {
      ::count count = get_view_count();
      for(index index = 0; index < count; index++)
      {
         ::view * pview = get_view(index);
         ASSERT_VALID(pview);
         pview->_001OnInitialUpdate(::null());
      }
   }

   /////////////////////////////////////////////////////////////////////////////
   // command routing

   bool document_interface::_001OnCmdMsg(BaseCmdMsg * pcmdmsg)
   {
      if (command_target::_001OnCmdMsg(pcmdmsg))
         return TRUE;

      // otherwise check template
      if (m_pdocumentemplate != ::null() &&
         m_pdocumentemplate->_001OnCmdMsg(pcmdmsg))
         return TRUE;

      return FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // document_interface diagnostics

   void document_interface::dump(dump_context & dumpcontext) const
   {
      ::ca::object::dump(dumpcontext);

      dumpcontext << "m_strTitle = " << m_strTitle;
      dumpcontext << "\nm_strPathName = " << m_strPathName;
      dumpcontext << "\nm_bModified = " << m_bModified;
      dumpcontext << "\nm_pDocTemplate = " << (void *)m_pdocumentemplate;

      if (dumpcontext.GetDepth() > 0)
      {
         ::count count = get_view_count();
         for(index index = 0; index < count; index++)
         {
            ::view * pview = get_view(index);
            dumpcontext << "\nwith ::view " << (void *)pview;
         }
      }

      dumpcontext << "\n";
   }

   void document_interface::assert_valid() const
   {
      ::ca::object::assert_valid();

      ::count count = get_view_count();
      for(index index = 0; index < count; index++)
      {
         ::view * pview = get_view(index);
         ASSERT_VALID(pview);
      }
   }


   bool document_interface::is_new_document()
   {
      return m_bNew;
   }

   void document_interface::write(::ca::byte_output_stream & ostream)
   {
      UNREFERENCED_PARAMETER(ostream);
   }

   void document_interface::read(::ca::byte_input_stream & istream)
   {
      UNREFERENCED_PARAMETER(istream);
   }


   ::view * document_interface::get_view(const ::ca::type_info & info, index indexFind)
   {
      single_lock sl(&m_mutex, true);
      ::count countView = get_view_count();
      ::count countFind = 0;
      ::view * pview;
      for(index index = 0; index < countView; index++)
      {
         pview = get_view(index);
         if(info == typeid(*pview))
         {
            if(indexFind == countFind)
               return pview;
            else
               countFind++;
         }
      }
      return ::null();
   }


   void document_interface::show_all_frames(UINT nCmdShow)
   {
      ::count count = get_view_count();
      for(index index = 0; index < count; index++)
      {
         ::view * pview = get_view(index);
         pview->GetParentFrame()->ShowWindow(nCmdShow);
      }
   }


   // document_interface
   const string & document_interface::get_title() const
   { ASSERT(this != ::null()); return m_strTitle; }
   const string & document_interface::get_path_name() const
   { ASSERT(this != ::null()); return m_strPathName; }
   document_template * document_interface::get_document_template() const
   { ASSERT(this != ::null()); return m_pdocumentemplate; }
   bool document_interface::is_modified()
   { ASSERT(this != ::null()); return m_bModified; }
   void document_interface::set_modified_flag(bool bModified)
   { ASSERT(this != ::null()); m_bModified = bModified; }
   void document_interface::set_new(bool bNew)
   {
      ASSERT(this != ::null());
      m_bNew = bNew;
   }



} // namespace user
