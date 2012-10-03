#include "framework.h"



const char document_manager::gen_ShellOpenFmt[] = "%s\\shell\\open\\%s";
const char document_manager::gen_ShellPrintFmt[] = "%s\\shell\\print\\%s";
const char document_manager::gen_ShellPrintToFmt[] = "%s\\shell\\printto\\%s";
const char document_manager::gen_DefaultIconFmt[] = "%s\\DefaultIcon";
const char document_manager::gen_ShellNewFmt[] = "%s\\ShellNew";

#define DEFAULT_ICON_INDEX 0

const char document_manager::gen_IconIndexFmt[] = ",%d";
const char document_manager::gen_Command[] = "command";
const char document_manager::gen_OpenArg[] = _T(" \"%1\"");
const char document_manager::gen_PrintArg[] = _T(" /p \"%1\"");
const char document_manager::gen_PrintToArg[] = _T(" /pt \"%1\" \"%2\" \"%3\" \"%4\"");
const char document_manager::gen_DDEArg[] = " /dde";

const char document_manager::gen_DDEExec[] = "ddeexec";
const char document_manager::gen_DDEOpen[] = _T("[open(\"%1\")]");
const char document_manager::gen_DDEPrint[] = _T("[print(\"%1\")]");
const char document_manager::gen_DDEPrintTo[] = _T("[printto(\"%1\",\"%2\",\"%3\",\"%4\")]");

const char document_manager::gen_ShellNewValueName[] = "NullFile";
const char document_manager::gen_ShellNewValue[] = "";

// recursively remove a registry key if and only if it has no subkeys
/*
bool __delete_reg_key(const char * lpszKey)
{
   // copy the string
   LPTSTR lpszKeyCopy = _tcsdup(lpszKey);

   if(lpszKeyCopy == NULL)
      return FALSE;

   LPTSTR lpszLast = lpszKeyCopy + lstrlen(lpszKeyCopy);

   // work until the end of the string
   while (lpszLast != NULL)
   {
      *lpszLast = '\0';
      lpszLast = _tcsdec(lpszKeyCopy, lpszLast);

      // try to open that key
      HKEY hKey;
      if (::RegOpenKey(HKEY_CLASSES_ROOT, lpszKeyCopy, &hKey) != ERROR_SUCCESS)
         break;

      // enumerate the keys underneath
      char szScrap[_MAX_PATH+1];
      DWORD dwLen = _countof(szScrap);
      bool bItExists = FALSE;

      if (::RegEnumKey(hKey, 0, szScrap, dwLen) == ERROR_SUCCESS)
         bItExists = TRUE;
      ::RegCloseKey(hKey);

      // found one?  quit looping
      if (bItExists)
         break;

      // otherwise, delete and find the previous backwhack
      ::RegDeleteKey(HKEY_CLASSES_ROOT, lpszKeyCopy);
      lpszLast = _tcsrchr(lpszKeyCopy, '\\');
   }

   // release the string and return
   free(lpszKeyCopy);
   return TRUE;
}

__STATIC bool _API
__set_reg_key(const char * lpszKey, const char * lpszValue, const char * lpszValueName)
{
   if (lpszValueName == NULL)
   {
      if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
           lpszValue, lstrlen(lpszValue) * sizeof(char)) != ERROR_SUCCESS)
      {
//         TRACE(::radix::trace::category_AppMsg, 0, "Warning: registration database update failed for key '%s'.\n",
  //          lpszKey);
         return FALSE;
      }
      return TRUE;
   }
   else
   {
      HKEY hKey;

      if(::RegCreateKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) == ERROR_SUCCESS)
      {
         LONG lResult = ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ,
            (CONST BYTE*)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(char));

         if(::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
            return TRUE;
      }
      //TRACE(::radix::trace::category_AppMsg, 0, "Warning: registration database update failed for key '%s'.\n", lpszKey);
      return FALSE;
   }
}
*/
document_manager::document_manager()
{
}

void document_manager::UnregisterShellFileTypes()
{
   throw not_implemented(get_app());
/*   ASSERT(!m_templateptra.is_empty());  // must have some doc templates

   string strPathName, strTemp;

   __get_module_short_file_name(System.m_hInstance, strPathName);

   POSITION pos = m_templateptra.get_head_position();
   for (int nTemplateIndex = 1; pos != NULL; nTemplateIndex++)
   {
      document_template * ptemplate = (document_template *)m_templateptra.get_next(pos);

      string strFilterExt, strFileTypeId, strFileTypeName;
      if (ptemplate->GetDocString(strFileTypeId,
         document_template::regFileTypeId) && !strFileTypeId.is_empty())
      {
         // enough info to register it
         if (!ptemplate->GetDocString(strFileTypeName,
            document_template::regFileTypeName))
            strFileTypeName = strFileTypeId;    // use id name

         ASSERT(strFileTypeId.find(' ') == -1);  // no spaces allowed

         strTemp.Format(gen_DefaultIconFmt, (const char *)strFileTypeId);
         __delete_reg_key(strTemp);

         // If MDI System
         if (!ptemplate->GetDocString(strTemp, document_template::windowTitle) ||
            strTemp.is_empty())
         {
            // path\shell\open\ddeexec = [open("%1")]
            strTemp.Format(gen_ShellOpenFmt, (const char *)strFileTypeId,
               (const char *)gen_DDEExec);
            __delete_reg_key(strTemp);

            // path\shell\print\ddeexec = [print("%1")]
            strTemp.Format(gen_ShellPrintFmt, (const char *)strFileTypeId,
               (const char *)gen_DDEExec);
            __delete_reg_key(strTemp);

            // path\shell\printto\ddeexec = [printto("%1","%2","%3","%4")]
            strTemp.Format(gen_ShellPrintToFmt, (const char *)strFileTypeId,
               (const char *)gen_DDEExec);
            __delete_reg_key(strTemp);
         }

         // path\shell\open\command = path filename
         strTemp.Format(gen_ShellOpenFmt, (const char *)strFileTypeId,
            (const char *)gen_Command);
         __delete_reg_key(strTemp);

         // path\shell\print\command = path /p filename
         strTemp.Format(gen_ShellPrintFmt, (const char *)strFileTypeId,
            (const char *)gen_Command);
         __delete_reg_key(strTemp);

         // path\shell\printto\command = path /pt filename printer driver port
         strTemp.Format(gen_ShellPrintToFmt, (const char *)strFileTypeId,
            (const char *)gen_Command);
         __delete_reg_key(strTemp);

         ptemplate->GetDocString(strFilterExt, document_template::filterExt);
         if (!strFilterExt.is_empty())
         {
            ASSERT(strFilterExt[0] == '.');

            LONG lSize = _MAX_PATH * 2;
            LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
               strTemp.GetBuffer(lSize), &lSize);
            strTemp.ReleaseBuffer();

            if (lResult != ERROR_SUCCESS || strTemp.is_empty() ||
               strTemp == strFileTypeId)
            {
               strTemp.Format(gen_ShellNewFmt, (const char *)strFilterExt);
               __delete_reg_key(strTemp);

               // no association for that suffix
               __delete_reg_key(strFilterExt);
            }
         }
      }
   }*/
}


void document_manager::RegisterShellFileTypes(bool bCompat)
{
   UNREFERENCED_PARAMETER(bCompat);
   throw not_implemented(get_app());

/*   ASSERT(!m_templateptra.is_empty());  // must have some doc templates

   string strPathName, strTemp;

   __get_module_short_file_name(System.m_hInstance, strPathName);

   POSITION pos = m_templateptra.get_head_position();
   for (int nTemplateIndex = 1; pos != NULL; nTemplateIndex++)
   {
      document_template * ptemplate = (document_template *)m_templateptra.get_next(pos);

      string strOpenCommandLine = strPathName;
      string strPrintCommandLine = strPathName;
      string strPrintToCommandLine = strPathName;
      string strDefaultIconCommandLine = strPathName;

      if (bCompat)
      {
         string strIconIndex;
         HICON hIcon = ::ExtractIcon(System.m_hInstance, strPathName, nTemplateIndex);
         if (hIcon != NULL)
         {
            strIconIndex.Format(gen_IconIndexFmt, nTemplateIndex);
            DestroyIcon(hIcon);
         }
         else
         {
            strIconIndex.Format(gen_IconIndexFmt, DEFAULT_ICON_INDEX);
         }
         strDefaultIconCommandLine += strIconIndex;
      }

      string strFilterExt, strFileTypeId, strFileTypeName;
      if (ptemplate->GetDocString(strFileTypeId,
         document_template::regFileTypeId) && !strFileTypeId.is_empty())
      {
         // enough info to register it
         if (!ptemplate->GetDocString(strFileTypeName,
            document_template::regFileTypeName))
            strFileTypeName = strFileTypeId;    // use id name

         ASSERT(strFileTypeId.find(' ') == -1);  // no spaces allowed

         // first register the type ID of our server
         if (!__set_reg_key(strFileTypeId, strFileTypeName))
            continue;       // just skip it

         if (bCompat)
         {
            // path\DefaultIcon = path,1
            strTemp.Format(gen_DefaultIconFmt, (const char *)strFileTypeId);
            if (!__set_reg_key(strTemp, strDefaultIconCommandLine))
               continue;       // just skip it
         }

         // If MDI System
         if (!ptemplate->GetDocString(strTemp, document_template::windowTitle) ||
            strTemp.is_empty())
         {
            // path\shell\open\ddeexec = [open("%1")]
            strTemp.Format(gen_ShellOpenFmt, (const char *)strFileTypeId,
               (const char *)gen_DDEExec);
            if (!__set_reg_key(strTemp, gen_DDEOpen))
               continue;       // just skip it

            if (bCompat)
            {
               // path\shell\print\ddeexec = [print("%1")]
               strTemp.Format(gen_ShellPrintFmt, (const char *)strFileTypeId,
                  (const char *)gen_DDEExec);
               if (!__set_reg_key(strTemp, gen_DDEPrint))
                  continue;       // just skip it

               // path\shell\printto\ddeexec = [printto("%1","%2","%3","%4")]
               strTemp.Format(gen_ShellPrintToFmt, (const char *)strFileTypeId,
                  (const char *)gen_DDEExec);
               if (!__set_reg_key(strTemp, gen_DDEPrintTo))
                  continue;       // just skip it

               // path\shell\open\command = path /dde
               // path\shell\print\command = path /dde
               // path\shell\printto\command = path /dde
               strOpenCommandLine += gen_DDEArg;
               strPrintCommandLine += gen_DDEArg;
               strPrintToCommandLine += gen_DDEArg;
            }
            else
            {
               strOpenCommandLine += gen_OpenArg;
            }
         }
         else
         {
            // path\shell\open\command = path filename
            // path\shell\print\command = path /p filename
            // path\shell\printto\command = path /pt filename printer driver port
            strOpenCommandLine += gen_OpenArg;
            if (bCompat)
            {
               strPrintCommandLine += gen_PrintArg;
               strPrintToCommandLine += gen_PrintToArg;
            }
         }

         // path\shell\open\command = path filename
         strTemp.Format(gen_ShellOpenFmt, (const char *)strFileTypeId,
            (const char *)gen_Command);
         if (!__set_reg_key(strTemp, strOpenCommandLine))
            continue;       // just skip it

         if (bCompat)
         {
            // path\shell\print\command = path /p filename
            strTemp.Format(gen_ShellPrintFmt, (const char *)strFileTypeId,
               (const char *)gen_Command);
            if (!__set_reg_key(strTemp, strPrintCommandLine))
               continue;       // just skip it

            // path\shell\printto\command = path /pt filename printer driver port
            strTemp.Format(gen_ShellPrintToFmt, (const char *)strFileTypeId,
               (const char *)gen_Command);
            if (!__set_reg_key(strTemp, strPrintToCommandLine))
               continue;       // just skip it
         }

         ptemplate->GetDocString(strFilterExt, document_template::filterExt);
         if (!strFilterExt.is_empty())
         {
            ASSERT(strFilterExt[0] == '.');

            LONG lSize = _MAX_PATH * 2;
            LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
               strTemp.GetBuffer(lSize), &lSize);
            strTemp.ReleaseBuffer();

            if (lResult != ERROR_SUCCESS || strTemp.is_empty() ||
               strTemp == strFileTypeId)
            {
               // no association for that suffix
               if (!__set_reg_key(strFilterExt, strFileTypeId))
                  continue;

               if (bCompat)
               {
                  strTemp.Format(gen_ShellNewFmt, (const char *)strFilterExt);
                  ()__set_reg_key(strTemp, gen_ShellNewValue, gen_ShellNewValueName);
               }
            }
         }
      }
   }*/
}

/*
__STATIC void _gen::AppendFilterSuffix(string & filter, OPENFILENAME& ofn,
   document_template * ptemplate, string* pstrDefaultExt)
{
   ENSURE_VALID(ptemplate);
   ASSERT_KINDOF(document_template, ptemplate);

   string strFilterExt, strFilterName;
   if (ptemplate->GetDocString(strFilterExt, document_template::filterExt) &&
      !strFilterExt.is_empty() &&
      ptemplate->GetDocString(strFilterName, document_template::filterName) &&
      !strFilterName.is_empty())
   {
      if (pstrDefaultExt != NULL)
         pstrDefaultExt->Empty();

      // add to filter
      filter += strFilterName;
      ASSERT(!filter.is_empty());  // must have a file type name
      filter += (char)'\0';  // next string please

      int iStart = 0;
      do
      {
         string strExtension = strFilterExt.Tokenize( _T( ";" ), iStart );

         if (iStart != -1)
         {
            // a file based document template - add to filter list

            // If you hit the following ASSERT, your document template
            // string is formatted incorrectly.  The section of your
            // document template string that specifies the allowable file
            // extensions should be formatted as follows:
            //    .<ext1>;.<ext2>;.<ext3>
            // Extensions may contain wildcards (e.g. '?', '*'), but must
            // begin with a '.' and be separated from one another by a ';'.
            // Example:
            //    .jpg;.jpeg
            ASSERT(strExtension[0] == '.');
            if ((pstrDefaultExt != NULL) && pstrDefaultExt->is_empty())
            {
               // set the default extension
               *pstrDefaultExt = strExtension.Mid( 1 );  // skip the '.'
               ofn.lpstrDefExt = const_cast< LPTSTR >((const char *)(*pstrDefaultExt));
               ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
            }

            filter += (char)'*';
            filter += strExtension;
            filter += (char)';';  // Always append a ';'.  The last ';' will get replaced with a '\0' later.
         }
      } while (iStart != -1);

      filter.set_at( filter.get_length()-1, '\0' );;  // replace the last ';' with a '\0'
      ofn.nMaxCustFilter++;
   }
}
*/

void document_manager::add_document_template(document_template * ptemplate)
{
   ASSERT_VALID(ptemplate);
   if(m_templateptra.add_unique(ptemplate))
   {
      ptemplate->load_template();
   }
}

count document_manager::get_template_count() const
{
   return m_templateptra.get_count();
}

document_template * document_manager::get_template(index index) const
{
   if(index < 0 || index >= m_templateptra.get_count())
      return NULL;
   return m_templateptra.element_at(index);
}

bool document_manager::save_all_modified()
{
   count count = m_templateptra.get_count();
   for(index index = 0; index < count; index++)
   {
      document_template * ptemplate = m_templateptra[index];
      ASSERT_KINDOF(document_template, ptemplate);
      if (!ptemplate->save_all_modified())
         return FALSE;
   }
   return TRUE;
}

void document_manager::close_all_documents(bool bEndSession)
{
   count count = m_templateptra.get_count();
   for(index index = 0; index < count; index++)
   {
      document_template * ptemplate = m_templateptra[index];
      ASSERT_KINDOF(document_template, ptemplate);
      ptemplate->close_all_documents(bEndSession);
   }
}

bool document_manager::do_prompt_file_name(var & varFile, UINT nIDSTitle, DWORD lFlags, bool bOpenFileDialog, document_template * ptemplate, ::document * pdocument)
{
   return System.do_prompt_file_name(varFile, nIDSTitle, lFlags, bOpenFileDialog, ptemplate, pdocument);
}

count document_manager::get_document_count()
{
   // count all documents
   count nCount = 0;
   count count = m_templateptra.get_count();
   for(index index = 0; index < count; index++)
   {
      document_template * ptemplate = m_templateptra[index];
      nCount += ptemplate->get_document_count();
   }
   return nCount;
}

#ifdef WINDOWS

bool document_manager::OnDDECommand(__in LPTSTR lpszCommand)
{
   UNREFERENCED_PARAMETER(lpszCommand);
   /*string strCommand = lpszCommand;
   document * pDoc = NULL;

   // open format is "[open("%s")]" - no whitespace allowed, one per line
   // print format is "[print("%s")]" - no whitespace allowed, one per line
   // print to format is "[printto("%s","%s","%s","%s")]" - no whitespace allowed, one per line
   gen::command & cmdInfo = System.command();
   command.m_nShellCommand = gen::command_line::FileDDE;

   if (strCommand.Left(7) == _T("[open(\""))
   {
      cmdInfo.m_nShellCommand = gen::command_line::FileOpen;
      strCommand = strCommand.Right(strCommand.get_length() - 7);
   }
   else if (strCommand.Left(8) == _T("[print(\""))
   {
      cmdInfo.m_nShellCommand = gen::command_line::FilePrint;
      strCommand = strCommand.Right(strCommand.get_length() - 8);
   }
   else if (strCommand.Left(10) == _T("[printto(\""))
   {
      cmdInfo.m_nShellCommand = gen::command_line::FilePrintTo;\
      strCommand = strCommand.Right(strCommand.get_length() - 10);
   }
   else
      return FALSE; // not a command we handle

   int i = strCommand.find('"');
   if (i == -1)
      return FALSE; // illegally terminated

   cmdInfo.m_varFile = strCommand.Left(i);
   strCommand = strCommand.Right(strCommand.get_length() - i);

   //gen::command_line* pOldInfo = NULL;
   bool bRetVal = TRUE;

   // // If we were started up for DDE retrieve the Show state
//   System.command_line() = cmdInfo;

   if (cmdInfo.m_nShellCommand == gen::command_line::FileOpen)
   {
      // show the application ::ca::window
      ::user::interaction* pMainWnd = System.GetMainWnd();
      int nCmdShow = System.m_nCmdShow;
      if (nCmdShow == -1 || nCmdShow == SW_SHOWNORMAL)
      {
/* trans         if (pMainWnd->IsIconic())
            nCmdShow = SW_RESTORE;
         else
            nCmdShow = SW_SHOW; */
      //}
      /*pMainWnd->ShowWindow(nCmdShow);
/* trans      if (nCmdShow != SW_MINIMIZE)
         pMainWnd->SetForegroundWindow(); */

      // then open the document
      /*System.open_document_file(cmdInfo.m_varFile);


      // next time, show the ::ca::window as default
      System.m_nCmdShow = -1;
      goto RestoreAndReturn;
   }

   if (cmdInfo.m_nShellCommand == gen::command_line::FilePrintTo)
   {
      if (strCommand.Left(3) != _T("\",\""))
      {
         bRetVal = FALSE;
         goto RestoreAndReturn;
      }
      else
      {
         strCommand = strCommand.Right(strCommand.get_length() - 3);
         i = strCommand.find('"');
         if (i == -1)
         {
            bRetVal = FALSE;
            goto RestoreAndReturn;
         }
         else
         {
            cmdInfo.m_strPrinterName = strCommand.Left(i);
            strCommand = strCommand.Right(strCommand.get_length() - i);
         }
      }

      if (strCommand.Left(3) != _T("\",\""))
      {
         bRetVal = FALSE;
         goto RestoreAndReturn;
      }
      else
      {
         strCommand = strCommand.Right(strCommand.get_length() - 3);
         i = strCommand.find('"');
         if (i == -1)
         {
            bRetVal = FALSE;
            goto RestoreAndReturn;
         }
         else
         {
            cmdInfo.m_strDriverName = strCommand.Left(i);
            strCommand = strCommand.Right(strCommand.get_length() - i);
         }
      }

      if (strCommand.Left(3) != _T("\",\""))
      {
         bRetVal = FALSE;
         goto RestoreAndReturn;
      }
      else
      {
         strCommand = strCommand.Right(strCommand.get_length() - 3);
         i = strCommand.find('"');
         if (i == -1)
         {
            bRetVal = FALSE;
            goto RestoreAndReturn;
         }
         else
         {
            cmdInfo.m_strPortName = strCommand.Left(i);
            strCommand = strCommand.Right(strCommand.get_length() - i);
         }
      }
   }

   // get document count before opening it
   int nOldCount; nOldCount = get_document_count();

   // open the document, then print it.
   pDoc = System.open_document_file(cmdInfo.m_varFile);
   //System.m_pCmdInfo = &cmdInfo;
   System.GetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_PRINT_DIRECT);
   //System.m_pCmdInfo = NULL;

   // close the document if it wasn't open previously (based on doc count)
   if (get_document_count() > nOldCount)
      pDoc->on_close_document();


RestoreAndReturn:
   //System.m_pCmdInfo = pOldInfo;
   return bRetVal;
}

void document_manager::_001OnFileNew()
{
   if (m_templateptra.is_empty())
   {
      TRACE(::radix::trace::category_AppMsg, 0, "Error: no document templates registered with application.\n");
      // linux System.simple_message_box(__IDP_FAILED_TO_CREATE_DOC);
      System.simple_message_box(NULL, "Failed to create document");
      return;
   }

   document_template * ptemplate = (document_template *)m_templateptra.first_element();
   if (m_templateptra.get_count() > 1)
   {
      // more than one document template to choose from
      // bring up dialog prompting ::fontopus::user
         return;     // none - cancel operation
   }

   ASSERT(ptemplate != NULL);
   ASSERT_KINDOF(document_template, ptemplate);

   ptemplate->open_document_file(NULL, TRUE, System.m_puiInitialPlaceHolderContainer);
      // if returns NULL, the ::fontopus::user has already been alerted*/
   return TRUE;
}

#endif

void document_manager::on_file_open()
{
   // prompt the ::fontopus::user (with all document templates)

   ::ca::create_context_sp createcontext(get_app());

   if (!do_prompt_file_name(createcontext->m_spCommandLine->m_varFile, 0 /*__IDS_OPENFILE */, 0 /*OFN_HIDEREADONLY | OFN_FILEMUSTEXIST*/, TRUE, NULL, NULL))
      return; // open cancelled

   System.open_document_file(createcontext);
      // if returns NULL, the ::fontopus::user has already been alerted
}


void document_manager::assert_valid() const
{
   ::radix::object::assert_valid();

   count count = m_templateptra.get_count();
   for(index index = 0; index < count; index++)
   {
      document_template * ptemplate = (document_template *) m_templateptra[index];
      ASSERT_VALID(ptemplate);
   }
}

void document_manager::dump(dump_context & dumpcontext) const
{
   ::radix::object::dump(dumpcontext);

   if (dumpcontext.GetDepth() != 0)
   {
      dumpcontext << "\nm_templateList[] = {";
      count count = m_templateptra.get_count();
      for(index index = 0; index < count; index++)
      {
         document_template * ptemplate = m_templateptra[index];
         dumpcontext << "\ntemplate " << ptemplate;
      }
      dumpcontext << "}";
   }

   dumpcontext << "\n";
}



void document_manager::request(::ca::create_context * pcreatecontext)
{

   if(pcreatecontext->m_spCommandLine->m_varFile.is_empty())
   {
      throw invalid_argument_exception(get_app());
   }

   // find the highest confidence
   count count = m_templateptra.get_count();
   document_template::Confidence bestMatch = document_template::noAttempt;
   document_template * pBestTemplate = NULL;
   ::user::document_interface * pOpenDocument = NULL;

   /*char szPath[_MAX_PATH];
   ASSERT(lstrlen(varFileName) < _countof(szPath));
   char szTemp[_MAX_PATH];
   if (lpszFileName[0] == '\"')
      ++lpszFileName;
   ::gen::tcsncpy_s(szTemp, _countof(szTemp), varFileName, _TRUNCATE);
   LPTSTR lpszLast = _tcsrchr(szTemp, '\"');
   if (lpszLast != NULL)
      *lpszLast = 0;*/

   //if( gen::FullPath(szPath, szTemp) == FALSE )
   //{
   //   ASSERT(FALSE);
   //   return NULL; // We won't open the file. ca2 API requires paths with
                   // length < _MAX_PATH
   //}

/*   char szLinkName[_MAX_PATH];
   if (gen::ResolveShortcut(System.GetMainWnd(), szPath, szLinkName, _MAX_PATH))
      ::gen::tcscpy_s(szPath, _countof(szPath), szLinkName);
*/

   for(index index = 0; index < count; index++)
   {
      document_template * ptemplate = m_templateptra[index];
      ASSERT_KINDOF(document_template, ptemplate);

      document_template::Confidence match;
      ASSERT(pOpenDocument == NULL);
      match = ptemplate->MatchDocType(pcreatecontext->m_spCommandLine->m_varFile, pOpenDocument);
      if (match > bestMatch)
      {
         bestMatch = match;
         pBestTemplate = ptemplate;
      }
      if (match == document_template::yesAlreadyOpen)
         break;      // stop here
   }

   if (pOpenDocument != NULL)
   {
      ::view * pview = pOpenDocument->get_view(0); // get first one
      if(pview != NULL)
      {
         ASSERT_VALID(pview);
         frame_window* pFrame = pview->GetParentFrame();

         if (pFrame == NULL)
            TRACE(::radix::trace::category_AppMsg, 0, "Error: Can not find a frame for document to activate.\n");
         else
         {
            pFrame->ActivateFrame();

            if (pFrame->GetParent() != NULL)
            {
               frame_window* pAppFrame;
               if (pFrame != (pAppFrame = dynamic_cast < frame_window * > (System.GetMainWnd())))
               {
                  ASSERT_KINDOF(frame_window, pAppFrame);
                  pAppFrame->ActivateFrame();
               }
            }
         }
      }
      else
         TRACE(::radix::trace::category_AppMsg, 0, "Error: Can not find a ::view for document to activate.\n");

      pcreatecontext->m_spCommandLine->m_varQuery["document"] = pOpenDocument;
   }

   if (pBestTemplate == NULL)
   {
      // linux System.simple_message_box(__IDP_FAILED_TO_OPEN_DOC);
      System.simple_message_box(NULL, "failed to open document");
      return;
   }

   pBestTemplate->request(pcreatecontext);
}

count document_manager::get_open_document_count()
{
   count nOpen = 0;
   count count = m_templateptra.get_count();
   for(index index = 0; index < count; index++)
   {
      document_template * ptemplate = m_templateptra[index];
      nOpen += ptemplate->get_document_count();
   }
   return nOpen;
}


document_manager::~document_manager()
{
   // for cleanup - delete all document templates
   count count = m_templateptra.get_count();
   for(index index = 0; index < count; index++)
   {
      document_template * ptemplate = m_templateptra[index];
      if (ptemplate->m_bAutoDelete)
      {
         delete ptemplate;
      }
   }
   m_templateptra.remove_all();
}




void document_manager::_001OnFileNew()
{
}
