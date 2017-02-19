////////////////////////////////////////////////////////////////////////////////
//
// The official specification of the File Transfer Protocol (FTP) is the RFC 959.
// Most of the documentation are taken from this RFC.
// This is an implementation of an simple FTP client_socket. I have tried to implement
// platform independent. For the communication i used the classes transfer_socket,
// ::net::address, ... from David J. Kruglinski (Inside Visual C++). These classes are
// only small wrappers for the sockets-API.
// Further I used a smart pointer-implementation from Scott Meyers (Effective C++,
// More Effective C++, Effective STL).
// The implementation of the logon-sequence (with firewall support) was published
// in an article on Codeguru by Phil Anderson.
// The code for the parsing of the different FTP LIST responses is taken from
// D. J. Bernstein (http://cr.yp.to/ftpparse.html). I only wrapped the c-code in
// a class.
// I have tested the code on Windows and Linux (Kubuntu).
//
// Copyright (c) 2004-2012 Thomas Oswald
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
//
// History:
// v1.5 released 2007-06-01
//+# TODO
// v1.1 released 2005-12-04
//      - Bug in OpenPassiveDataConnection removed: SendCommand was called before data connection was established.
//      - Bugs in GetSingleResponseLine removed:
//         * Infinite loop if response line doesn't end with CRLF.
//         * Return value of std:string->find must be checked against npos.
//      - Now runs in unicode.
//      - Streams removed.
//      - Explicit detaching of observers are not necessary anymore.
//      - ExecuteDatachannelCommand now accepts an itransfer_notification object.
//        Through this concept there is no need to write the received files to a file.
//        For example the bytes can be written only in memory or an other tcp stream.
//      - Added an interface for the blocking socket (::sockets::transfer_socket).
//        Therefore it is possible to exchange the socket implementation, e.g. for
//        writing unit tests (by simulating an specific scenario of a FTP communication).
//      - Replaced the magic numbers concerning the reply codes by a class.
// v1.0 released 2004-10-25
////////////////////////////////////////////////////////////////////////////////


#include "framework.h"


namespace ftp
{




   //////////////////////////////////////////////////////////////////////
   // Construction/Destruction
   //////////////////////////////////////////////////////////////////////

   /// constructor
   /// @param[in] apSocket Instance of socket class which will be used for
   ///                     communication with the FTP server.
   ///                     client_socket class takes ownership of this instance.
   ///                     It will be deleted on destruction of this object.
   ///                     If this pointer is NULL, the transfer_socket implementation
   ///                     will be used.
   ///                     This gives the ability to set an other socket class.
   ///                     For example a socket class can be implemented which simulates
   ///                     a FTP server (for unit testing).
   /// @param[in] uiTimeout Timeout used for socket operation.
   /// @param[in] uiBufferSize Size of the buffer used for sending and receiving
   ///                         data via sockets. The size have an influence on
   ///                         the performance. Through empiric test i come to the
   ///                         conclusion that 2048 is a good size.
   /// @param[in] uiResponseWait Sleep time between receive calls to socket when getting
   ///                           the response. Sometimes the socket hangs if no wait time
   ///                           is set. Normally not wait time is necessary.
   client_socket::client_socket(::sockets::base_socket_handler & handler, 
      unsigned int uiTimeout/*=10*/,
      unsigned int uiBufferSize/*=2048*/, unsigned int uiResponseWait/*=0*/,
      const string& strRemoteDirectorySeparator/*=_T("/")*/) :
      ::object(handler.get_app()),
      ::sockets::base_socket(handler),
      ::sockets::socket(handler),
      ::sockets::stream_socket(handler),
      ::sockets::tcp_socket(handler),
      mc_uiTimeout(uiTimeout),
      mc_uiResponseWait(uiResponseWait),
      mc_strEolCharacterSequence(_T("\r\n")),
      mc_strRemoteDirectorySeparator(strRemoteDirectorySeparator),//+# documentation missing
      m_apFileListParser(canew(file_list_parser())),
      m_fTransferInProgress(false),
      m_fAbortTransfer(false),
      m_fResumeIfPossible(true)
   {

      m_econnectiontype = connection_type_plain;

      m_pmutex = new mutex(get_app());

      m_estate = state_initial;
      
      m_vBuffer.allocate(uiBufferSize);

   }


   /// destructor
   client_socket::~client_socket()
   {
      if (IsTransferringData())
         _abort();

      if (_is_connected())
         Logout();
   }

   /// Attach an observer to the client_socket. You can attach as many observers as you want.
   /// The client_socket send notifications (more precisely the virtual functions are called)
   /// to the observers.
   void client_socket::AttachObserver(client_socket::notification* pObserver)
   {
      ASSERT(pObserver);
      if (pObserver)
         m_setObserver.attach_observer(pObserver);
   }

   /// Detach an observer from the client_socket.
   void client_socket::DetachObserver(client_socket::notification* pObserver)
   {
      ASSERT(pObserver);
      if (pObserver)
         m_setObserver.detach_observer(pObserver);
   }

   /// Sets the file list parser which is used for parsing the results of the LIST command.
   void client_socket::SetFileListParser(sp(ifile_list_parser) apFileListParser)
   {
      m_apFileListParser = apFileListParser;
   }

   /// Returns a set of all observers currently attached to the client_socket.
   client_socket::observer_array& client_socket::GetObservers()
   {
      return m_setObserver;
   }

   /// Enables or disables resuming for file transfer operations.
   /// @param[in] fEnable True enables resuming, false disables it.
   void client_socket::SetResumeMode(bool fEnable/*=true*/)
   {
      m_fResumeIfPossible = fEnable;
   }

   /// Indicates if the resume mode is set.
   bool client_socket::IsResumeModeEnabled()
   {
      return m_fResumeIfPossible;
   }

   /// Opens the control channel to the FTP server.
   /// @param[in] strServerHost IP-::net::address or name of the server
   /// @param[in] iServerPort Port for channel. Usually this is port 21.
   bool client_socket::OpenControlChannel(const string& strServerHost, WINUSHORT ushServerPort/*=DEFAULT_FTP_PORT*/)
   {
      
      if (IsConnected() || _is_connected())
      {

         CloseControlChannel();

      }

      SetLineProtocol();

      try
      {
         
         if (!open(strServerHost, ushServerPort))
         {
            
            return false;

         }

      }
      catch (::sockets::transfer_socket_exception& blockingException)
      {
         ReportError(blockingException.GetErrorMessage(), __FILE__, __LINE__);
         SetCloseAndDelete();
         return false;
      }

      m_handler.add(this);

      return true;
   }

   /// Returns the connection state of the client_socket.
   bool client_socket::_is_connected()
   {
      return GetSocket() != INVALID_SOCKET;
   }

   /// Returns true if a download/upload is running, otherwise false.
   bool client_socket::IsTransferringData()
   {
      return m_fTransferInProgress;
   }

   /// Closes the control channel to the FTP server.
   void client_socket::CloseControlChannel()
   {
      try
      {
         SetCloseAndDelete();
         m_apCurrentRepresentation.release();
      }
      catch (::sockets::transfer_socket_exception& blockingException)
      {
         blockingException.GetErrorMessage();
         SetCloseAndDelete();
      }
   }

   /// Analyse the repy code of a FTP server-response.
   /// @param[in] Reply Reply of a FTP server.
   /// @retval FTP_OK    All runs perfect.
   /// @retval FTP_ERROR Something went wrong. An other response was expected.
   /// @retval NOT_OK    The command was not accepted.
   int client_socket::SimpleErrorCheck(const reply& Reply)
   {
      if (Reply.Code().IsNegativeReply())
         return FTP_NOTOK;
      else if (Reply.Code().IsPositiveCompletionReply())
         return FTP_OK;

      ASSERT(Reply.Code().IsPositiveReply());

      return FTP_ERROR;
   }

   long client_socket::cert_common_name_check(const char * common_name)
   {

      long l = ::sockets::tcp_socket::cert_common_name_check(common_name);

      if (l == X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT)
      {

         return X509_V_OK;

      }

      return l;

   }

   /// Logs on to a FTP server.
   /// @param[in] logonInfo Structure with logon information.
   bool client_socket::Login(logon& logonInfo)
   {
      logonInfo.m_bFailedBecauseOfSecurityLevelCanUpgrade = false;
      m_LastLogonInfo = logonInfo;

      enum {
         LO = -2,      ///< Logged On
         ER = -1,      ///< Error
         NUMLOGIN = 9, ///< currently supports 9 different login sequences
      };

      int iLogonSeq[NUMLOGIN][18] = {
         // this array stores all of the logon sequences for the various firewalls
         // in blocks of 3 nums.
         // 1st num is command to send,
         // 2nd num is next point in logon sequence array if 200 series response
         //         is rec'd from server as the result of the command,
         // 3rd num is next point in logon sequence if 300 series rec'd
         { 0,LO,3,    1,LO, 6,   2,LO,ER }, // no firewall
         { 3, 6,3,    4, 6,ER,   5,ER, 9,   0,LO,12,   1,LO,15,   2,LO,ER }, // SITE hostname
         { 3, 6,3,    4, 6,ER,   6,LO, 9,   1,LO,12,   2,LO,ER }, // USER after logon
         { 7, 3,3,    0,LO, 6,   1,LO, 9,   2,LO,ER }, // proxy OPEN
         { 3, 6,3,    4, 6,ER,   0,LO, 9,   1,LO,12,   2,LO,ER }, // Transparent
         { 6,LO,3,    1,LO, 6,   2,LO,ER }, // USER with no logon
         { 8, 6,3,    4, 6,ER,   0,LO, 9,   1,LO,12,   2,LO,ER }, // USER fireID@remotehost
         { 9,ER,3,    1,LO, 6,   2,LO,ER }, // USER remoteID@remotehost fireID
         { 10,LO,3,   11,LO, 6,   2,LO,ER }  // USER remoteID@fireID@remotehost
      };

      // are we connecting directly to the host (logon type 0) or via a firewall? (logon type>0)
      string   strTemp;
      WINUSHORT    ushPort = 0;

      if (_is_connected())
         Logout();


      if (logonInfo.FwType() == firewall_type::None())
      {

         if (m_econnectiontype == connection_type_tls_implicit)
         {

            strTemp = logonInfo.Hostname();
            ushPort = 990;
            EnableSSL();
            m_strHost = strTemp;

         }
         else
         {

            strTemp = logonInfo.Hostname();
            ushPort = logonInfo.Hostport();

         }

      }
      else
      {
         strTemp = logonInfo.FwHost();
         ushPort = logonInfo.FwPort();
      }

      string strHostnamePort(logonInfo.Hostname());
      if (logonInfo.Hostport() != DEFAULT_FTP_PORT)
         strHostnamePort = logonInfo.Hostname() + ":" + ::str::from(logonInfo.Hostport()); // add port to hostname (only if port is not 21)


      if (!OpenControlChannel(strTemp, ushPort))
         return false;

      if (m_econnectiontype == connection_type_tls_implicit)
      {

         // SetCallOnConnect();

      }

      // get initial connect msg off server
      reply Reply;
      if (!GetResponse(Reply) || !Reply.Code().IsPositiveCompletionReply())
         return false;

      int iLogonPoint = 0;

      // go through appropriate logon procedure
      for (; ; )
      {
         // send command, get response
         reply Reply;
         switch (iLogonSeq[logonInfo.FwType().AsEnum()][iLogonPoint])
         {
            // state                 command           command argument                                                                              success     fail
         case 0:  if (SendCommand(command::USER(), { logonInfo.Username() }, Reply)) break; else return false;
         case 1:  if (SendCommand(command::PASS(), {logonInfo.Password() }, Reply)) break; else return false;
         case 2:  if (SendCommand(command::ACCT(), {logonInfo.Account() }, Reply)) break; else return false;
         case 3:  if (SendCommand(command::USER(), {logonInfo.FwUsername() }, Reply)) break; else return false;
         case 4:  if (SendCommand(command::PASS(), {logonInfo.FwPassword() }, Reply)) break; else return false;
         case 5:  if (SendCommand(command::SITE(), {strHostnamePort }, Reply)) break; else return false;
         case 6:  if (SendCommand(command::USER(), {logonInfo.Username() + "@" + strHostnamePort }, Reply)) break; else return false;
         case 7:  if (SendCommand(command::OPEN(), {strHostnamePort }, Reply)) break; else return false;
         case 8:  if (SendCommand(command::USER(), {logonInfo.FwUsername() + "@" + strHostnamePort }, Reply)) break; else return false;
         case 9:  if (SendCommand(command::USER(), {logonInfo.Username() + "@" + strHostnamePort + " " + logonInfo.FwUsername() }, Reply)) break; else return false;
         case 10: if (SendCommand(command::USER(), { logonInfo.Username() + "@" + logonInfo.FwUsername() + "@" + strHostnamePort }, Reply)) break; else return false;
         case 11: if (SendCommand(command::PASS(), { logonInfo.Password() + "@" + logonInfo.FwPassword() }, Reply)) break; else return false;
         }

         if (!Reply.Code().IsPositiveCompletionReply() && !Reply.Code().IsPositiveIntermediateReply())
         {
            
            if (atoi(Reply.Code().Value()) == 530)
            {

               if (m_econnectiontype == connection_type_plain && Reply.Value().find_ci("ftp over tls") >= 0)
               {

                  m_econnectiontype = connection_type_tls_implicit;

                  logonInfo.m_bFailedBecauseOfSecurityLevelCanUpgrade = true;

                  Sleep(1000);

               }

            }

            return false;

         }

         const unsigned int uiFirstDigitOfReplyCode = atoi_dup(Reply.Code().Value()) / 100;
         iLogonPoint = iLogonSeq[logonInfo.FwType().AsEnum()][iLogonPoint + uiFirstDigitOfReplyCode - 1]; //get next command from array
         switch (iLogonPoint)
         {
         case ER: // ER means somewhat has gone wrong
         {
            ReportError(_T("Logon failed."), __FILE__, __LINE__);
         }
         return false;
         case LO: // LO means we're fully logged on
            if (ChangeWorkingDirectory(mc_strRemoteDirectorySeparator) != FTP_OK)
               return false;
            return true;
         }
      }

      if (m_econnectiontype == connection_type_tls_implicit)
      {

         reply Reply;
         if (!SendCommand(command::PROT(), { "P" }, Reply))
         {

            return false;

         }

         if (!Reply.Code().IsPositiveCompletionReply() && !Reply.Code().IsPositiveIntermediateReply())
         {

            return false;

         }


      }
      //return false;
   }

   /// Rename a file on the FTP server.
   /// @remarks Can be used for moving the file to another directory.
   /// @param[in] strOldName Name of the file to rename.
   /// @param[in] strNewName The new name for the file.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::Rename(const string& strOldName, const string& strNewName)
   {
      reply Reply;
      if (!SendCommand(command::RNFR(), strOldName, Reply))
         return FTP_ERROR;

      if (Reply.Code().IsNegativeReply())
         return FTP_NOTOK;
      else if (!Reply.Code().IsPositiveIntermediateReply())
      {
         ASSERT(Reply.Code().IsPositiveCompletionReply() || Reply.Code().IsPositivePreliminaryReply());
         return FTP_ERROR;
      }

      if (!SendCommand(command::RNTO(), strNewName, Reply))
         return FTP_ERROR;

      return SimpleErrorCheck(Reply);
   }

   /// Moves a file within the FTP server.
   /// @param[in] strFullSourceFilePath Name of the file which should be moved.
   /// @param[in] strFullTargetFilePath The destination where the file should be moved to (file name must be also given).
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::Move(const string& strFullSourceFilePath, const string& strFullTargetFilePath)
   {
      return Rename(strFullSourceFilePath, strFullTargetFilePath);
   }

   /// Gets the directory listing of the FTP server. Sends the LIST command to
   /// the FTP server.
   /// @param[in] strPath Starting path for the list command.
   /// @param[out] vstrFileList Returns a simple list of the files and folders of the specified directory.
   /// @param[in] fPasv see documentation of client_socket::Passive
   bool client_socket::List(const string& strPath, stringa& vstrFileList, bool fPasv)
   {
      output_stream outputStream(mc_strEolCharacterSequence, command::LIST().AsString());
      if (!ExecuteDatachannelCommand(command::LIST(), strPath, representation(type::ASCII()), fPasv, 0, outputStream))
         return false;

      vstrFileList.clear();
      string strLine;
      outputStream.SetStartPosition();
      while (outputStream.GetNextLine(strLine))
         vstrFileList.push_back(strPath + strLine);

      return true;
   }

   /// Gets the directory listing of the FTP server. Sends the NLST command to
   /// the FTP server.
   /// @param[in] strPath Starting path for the list command.
   /// @param[out] vstrFileList Returns a simple list of the files and folders of the specified the directory.
   /// @param[in] fPasv see documentation of client_socket::Passive
   bool client_socket::NameList(const string& strPath, stringa& vstrFileList, bool fPasv)
   {
      output_stream outputStream(mc_strEolCharacterSequence, command::NLST().AsString());
      if (!ExecuteDatachannelCommand(command::NLST(), strPath, representation(type::ASCII()), fPasv, 0, outputStream))
         return false;

      vstrFileList.clear();
      string strLine;
      outputStream.SetStartPosition();
      while (outputStream.GetNextLine(strLine))
         vstrFileList.push_back(strPath + strLine);

      return true;
   }

   /// Gets the directory listing of the FTP server. Sends the LIST command to
   /// the FTP server.
   /// @param[in] strPath Starting path for the list command.
   /// @param[out] vFileList Returns a detailed list of the files and folders of the specified directory.
   ///                       vFileList contains file_status-Objects. These Objects provide a lot of
   ///                       information about the file/folder.
   /// @param[in] fPasv see documentation of client_socket::Passive
   bool client_socket::List(const string& strPath, file_status_ptra& vFileList, bool fPasv)
   {
      ASSERT(m_apFileListParser.is_set());

      output_stream outputStream(mc_strEolCharacterSequence, command::LIST().AsString());
      if (!ExecuteDatachannelCommand(command::LIST(), strPath, representation(type::ASCII()), fPasv, 0, outputStream))
         return false;

      vFileList.clear();
      string strLine;
      outputStream.SetStartPosition();
      while (outputStream.GetNextLine(strLine))
      {
         sp(file_status) spFtpFileStatus(canew(file_status(get_app())));
         if (m_apFileListParser->Parse(*spFtpFileStatus, strLine))
         {
            spFtpFileStatus->m_strPath = strPath;
            vFileList.push_back(spFtpFileStatus);
         }
      }

      return true;
   }

   /// Gets the directory listing of the FTP server. Sends the NLST command to
   /// the FTP server.
   /// @param[in] strPath Starting path for the list command.
   /// @param[out] vFileList Returns a simple list of the files and folders of the specified directory.
   ///                       vFileList contains file_status-Objects. Normally these Objects provide
   ///                       a lot of information about the file/folder. But the NLST-command provide
   ///                       only a simple list of the directory content (no specific information).
   /// @param[in] fPasv see documentation of client_socket::Passive
   bool client_socket::NameList(const string& strPath, file_status_ptra& vFileList, bool fPasv)
   {
      output_stream outputStream(mc_strEolCharacterSequence, command::NLST().AsString());
      if (!ExecuteDatachannelCommand(command::NLST(), strPath, representation(type::ASCII()), fPasv, 0, outputStream))
         return false;

      vFileList.clear();
      string strLine;
      outputStream.SetStartPosition();
      while (outputStream.GetNextLine(strLine))
      {
         sp(file_status) spFtpFileStatus(canew(file_status(get_app())));
         spFtpFileStatus->m_strPath = strPath;
         spFtpFileStatus->m_strName = strLine;
         vFileList.push_back(spFtpFileStatus);
      }

      return true;
   }

   /// Gets a file from the FTP server.
   /// Uses C functions for file access (very fast).
   /// @param[in] strRemoteFile Filename of the sourcefile on the FTP server.
   /// @param[in] strLocalFile Filename of the target file on the local computer.
   /// @param[in] repType Representation Type (see documentation of representation)
   /// @param[in] fPasv see documentation of client_socket::Passive
   bool client_socket::DownloadFile(const string& strRemoteFile, const string& strLocalFile, const representation& repType, bool fPasv)
   {
      ::ftp::file file(get_app());
      if (!file.Open(strLocalFile, (m_fResumeIfPossible ? ::file::mode_no_truncate : ::file::mode_create) | ::file::mode_write
      | ::file::type_binary | ::file::defer_create_directory))
      {
         ReportError(sys_error::GetErrorDescription(), __FILE__, __LINE__);
         return false;
      }
      file.Seek(0, ::file::seek_end);

      return DownloadFile(strRemoteFile, file, repType, fPasv);
   }

   /// Gets a file from the FTP server.
   /// Uses C functions for file access (very fast).
   /// @param[in] strRemoteFile Filename of the sourcefile on the FTP server.
   /// @param[in] Observer Object which receives the transfer notifications.
   /// @param[in] repType Representation Type (see documentation of representation)
   /// @param[in] fPasv see documentation of client_socket::Passive
   bool client_socket::DownloadFile(const string& strRemoteFile, itransfer_notification& Observer, const representation& repType, bool fPasv)
   {
      long lRemoteFileSize = 0;
      FileSize(strRemoteFile, lRemoteFileSize);

      for (auto * p : (observer_array &)m_setObserver)
         p->OnPreReceiveFile(strRemoteFile, Observer.GetLocalStreamName(), lRemoteFileSize);

      const bool fRet = ExecuteDatachannelCommand(command::RETR(), strRemoteFile, repType, fPasv,
         m_fResumeIfPossible ? Observer.GetLocalStreamSize() : 0, Observer);

      for (auto * p : (observer_array &)m_setObserver)
         p->OnPostReceiveFile(strRemoteFile, Observer.GetLocalStreamName(), lRemoteFileSize);

      return fRet;
   }

   /// Gets a file from the FTP server.
   /// The target file is on an other FTP server (FXP).
   /// NOTICE: The file is directly transferred from one server to the other server.
   /// @param[in] strSourceFile File which is on the source FTP server.
   /// @param[in] TargetFtpServer The FTP server where the downloaded file will be stored.
   /// @param[in] strTargetFile Filename of the target file on the target FTP server.
   /// @param[in] repType Representation Type (see documentation of representation)
   /// @param[in] fPasv see documentation of client_socket::Passive
   bool client_socket::DownloadFile(const string& strSourceFile, client_socket& TargetFtpServer,
      const string& strTargetFile, const representation& repType/*=representation(type::Image())*/,
      bool fPasv/*=true*/)
   {
      return TransferFile(*this, strSourceFile, TargetFtpServer, strTargetFile, repType, fPasv);
   }

   /// Puts a file on the FTP server.
   /// Uses C functions for file access (very fast).
   /// @param[in] strLocalFile Filename of the the local sourcefile which to put on the FTP server.
   /// @param[in] strRemoteFile Filename of the target file on the FTP server.
   /// @param[in] fStoreUnique if true, the FTP command STOU is used for saving
   ///                         else the FTP command STOR is used.
   /// @param[in] repType Representation Type (see documentation of representation)
   /// @param[in] fPasv see documentation of client_socket::Passive
   bool client_socket::UploadFile(const string& strLocalFile, const string& strRemoteFile, bool fStoreUnique, const representation& repType, bool fPasv)
   {
      ::ftp::file file(get_app());
      if (!file.Open(strLocalFile, ::file::mode_read | ::file::type_binary))
      {
         ReportError(sys_error::GetErrorDescription(), __FILE__, __LINE__);
         return false;
      }

      return UploadFile(file, strRemoteFile, fStoreUnique, repType, fPasv);
   }

   /// Puts a file on the FTP server.
   /// Uses C functions for file access (very fast).
   /// @param[in] Observer Object which receives the transfer notifications for upload requests.
   /// @param[in] strRemoteFile Filename of the target file on the FTP server.
   /// @param[in] fStoreUnique if true, the FTP command STOU is used for saving
   ///                         else the FTP command STOR is used.
   /// @param[in] repType Representation Type (see documentation of representation)
   /// @param[in] fPasv see documentation of client_socket::Passive
   bool client_socket::UploadFile(itransfer_notification& Observer, const string& strRemoteFile, bool fStoreUnique, const representation& repType, bool fPasv)
   {
      long lRemoteFileSize = 0;
      if (m_fResumeIfPossible)
         FileSize(strRemoteFile, lRemoteFileSize);

      command cmd(command::STOR());
      if (lRemoteFileSize > 0)
         cmd = command::APPE();
      else if (fStoreUnique)
         cmd = command::STOU();

      const long lLocalFileSize = Observer.GetLocalStreamSize();
      Observer.SetLocalStreamOffset(lRemoteFileSize);

      for (auto * p : (observer_array &)m_setObserver)
         p->OnPreSendFile(Observer.GetLocalStreamName(), strRemoteFile, lLocalFileSize);

      const bool fRet = ExecuteDatachannelCommand(cmd, strRemoteFile, repType, fPasv, 0, Observer);

      for (auto * p : (observer_array &)m_setObserver)
         p->OnPostSendFile(Observer.GetLocalStreamName(), strRemoteFile, lLocalFileSize);

      return fRet;
   }

   /// Puts a file on the FTP server.
   /// The source file is on an other FTP server (FXP).
   /// NOTICE: The file is directly transferred from one server to the other server.
   /// @param[in] SourceFtpServer A FTP server from which the file is taken for upload action.
   /// @param[in] strSourceFile File which is on the source FTP server.
   /// @param[in] strTargetFile Filename of the target file on the FTP server.
   /// @param[in] repType Representation Type (see documentation of representation)
   /// @param[in] fPasv see documentation of client_socket::Passive
   bool client_socket::UploadFile(client_socket& SourceFtpServer, const string& strSourceFile,
      const string& strTargetFile, const representation& repType/*=representation(type::Image())*/,
      bool fPasv/*=true*/)
   {
      return TransferFile(SourceFtpServer, strSourceFile, *this, strTargetFile, repType, !fPasv);
   }

   /// Transfers a file from a FTP server to another FTP server.
   /// The source file is on an other FTP server (FXP).
   /// NOTICE: The file is directly transferred from one server to the other server.
   /// @param[in] SourceFtpServer A FTP server from which the file which is copied.
   /// @param[in] strSourceFile Name of the file which is on the source FTP server.
   /// @param[in] TargetFtpServer A FTP server to which the file is copied.
   /// @param[in] strTargetFile Name of the file on the target FTP server.
   /// @param[in] repType Representation Type (see documentation of representation)
   /// @param[in] fPasv see documentation of client_socket::Passive
   /*static*/ bool client_socket::TransferFile(client_socket& SourceFtpServer, const string& strSourceFile,
      client_socket& TargetFtpServer, const string& strTargetFile,
      const representation& repType/*=representation(type::Image())*/,
      bool fSourcePasv/*=false*/)
   {
      // transmit representation to server
      if (SourceFtpServer.RepresentationType(repType) != FTP_OK)
         return false;

      if (TargetFtpServer.RepresentationType(repType) != FTP_OK)
         return false;

      client_socket& PassiveServer = fSourcePasv ? SourceFtpServer : TargetFtpServer;
      client_socket& ActiveServer = fSourcePasv ? TargetFtpServer : SourceFtpServer;

      // set one FTP server in passive mode
      // the FTP server opens a port and tell us the socket (ip ::net::address + port)
      // this socket is used for opening the data connection
      WINULONG  ulIP = 0;
      WINUSHORT ushSock = 0;
      if (PassiveServer.Passive(ulIP, ushSock) != FTP_OK)
         return false;

      ::net::address csaPassiveServer(ulIP, ushSock);

      // transmit the socket (ip ::net::address + port) of the first FTP server to the
      // second server
      // the second FTP server establishes then the data connection to the first
      if (ActiveServer.DataPort(csaPassiveServer.get_display_number(), ushSock) != FTP_OK)
         return false;

      if (!SourceFtpServer.SendCommand(command::RETR(), strSourceFile))
         return false;

      reply ReplyTarget;
      if (!TargetFtpServer.SendCommand(command::STOR(), strTargetFile, ReplyTarget) ||
         !ReplyTarget.Code().IsPositivePreliminaryReply())
         return false;

      reply ReplySource;
      if (!SourceFtpServer.GetResponse(ReplySource) || !ReplySource.Code().IsPositivePreliminaryReply())
         return false;

      // get response from FTP servers
      if (!SourceFtpServer.GetResponse(ReplySource) || !ReplySource.Code().IsPositiveCompletionReply() ||
         !TargetFtpServer.GetResponse(ReplyTarget) || !ReplyTarget.Code().IsPositiveCompletionReply())
         return false;

      return true;
   }

   /// Executes a commando that result in a communication over the data port.
   /// @param[in] crDatachannelCmd Command to be executeted.
   /// @param[in] strPath Parameter for the command usually a path.
   /// @param[in] representation see documentation of client_socket::representation
   /// @param[in] fPasv see documentation of client_socket::Passive
   /// @param[in] dwByteOffset Server marker at which file transfer is to be restarted.
   /// @param[in] Observer Object for observing the execution of the command.
   bool client_socket::ExecuteDatachannelCommand(const command& crDatachannelCmd, const string& strPath, const representation& representation,
      bool fPasv, DWORD dwByteOffset, itransfer_notification& Observer)
   {
      if (!crDatachannelCmd.IsDatachannelCommand())
      {
         ASSERT(false);
         return false;
      }

      if (m_fTransferInProgress)
         return false;

      if (!_is_connected())
         return false;

      // transmit representation to server
      if (RepresentationType(representation) != FTP_OK)
         return false;

      bool fTransferOK = false;

      sp(::sockets::base_socket) pbasesocket;

      sp(::sockets::base_socket) pbasesocket2;

      if (fPasv)
      {

         sp(::sockets::transfer_socket) apSckDataConnection;

         if (crDatachannelCmd.IsDatachannelWriteCommand())
         {

            apSckDataConnection = new ::sockets::write_socket(m_handler);

         }
         else if (crDatachannelCmd.IsDatachannelReadCommand())
         {

            apSckDataConnection = new ::sockets::read_socket(m_handler);

         }
         else
         {

            return false;

         }

         pbasesocket = apSckDataConnection;

         if (!OpenPassiveDataConnection(*apSckDataConnection, crDatachannelCmd, strPath, dwByteOffset))
            return false;

         fTransferOK = TransferData(crDatachannelCmd, Observer, *apSckDataConnection);

         apSckDataConnection->SetCloseAndDelete();

      }
      else
      {
         
         sp(::sockets::listen_socket_base) apSckDataConnection;
         
         if (crDatachannelCmd.IsDatachannelWriteCommand())
         {

            apSckDataConnection = new ::sockets::listen_socket < ::sockets::write_socket >(m_handler);

         }
         else if (crDatachannelCmd.IsDatachannelReadCommand())
         {

            apSckDataConnection = new ::sockets::listen_socket < ::sockets::read_socket >(m_handler);

         }
         else
         {

            return false;

         }

         pbasesocket2 = apSckDataConnection;

         if (!OpenActiveDataConnection(*apSckDataConnection, crDatachannelCmd, strPath, dwByteOffset))
            return false;

         sp(::sockets::transfer_socket) psocket = apSckDataConnection->m_pbasesocket;

         pbasesocket = psocket;

         fTransferOK = TransferData(crDatachannelCmd, Observer, *psocket);

         if (psocket->GetSocket() != INVALID_SOCKET)
         {

            psocket->close();

         }

      }

      //DWORD dwStart = get_tick_count();

      //UINT nSeconds = 10;

      //if (pbasesocket.is_set() && !pbasesocket->IsDetached())
      //{

      //   try
      //   {

      //      while (pbasesocket->Handler().contains(pbasesocket) && get_tick_count() - dwStart < nSeconds * 1000)
      //      {

      //         pbasesocket->Handler().select(0, 100 * 1000);

      //      }

      //   }
      //   catch (...)
      //   {

      //   }

      //}

      //if (pbasesocket2.is_set() && !pbasesocket2->IsDetached())
      //{

      //   try
      //   {

      //      while (pbasesocket2->Handler().contains(pbasesocket2) && get_tick_count() - dwStart < nSeconds * 1000)
      //      {

      //         pbasesocket2->Handler().select(0, 100 * 1000);

      //      }

      //   }
      //   catch (...)
      //   {

      //   }

      //}

      // get response from FTP server
      reply Reply;
      if (!fTransferOK || !GetResponse(Reply) || !Reply.Code().IsPositiveCompletionReply())
         return false;

      return true;

   }


   /// Executes a commando that result in a communication over the data port.
   /// @param[in] crDatachannelCmd Command to be executeted.
   /// @param[in] Observer Object for observing the execution of the command.
   /// @param[in] sckDataConnection Socket which is used for sending/receiving data.
   bool client_socket::TransferData(const command& crDatachannelCmd, itransfer_notification& Observer, ::sockets::transfer_socket & sckDataConnection)
   {

      if (crDatachannelCmd.IsDatachannelWriteCommand())
      {
         if (!SendData(Observer, sckDataConnection))
            return false;
      }
      else if (crDatachannelCmd.IsDatachannelReadCommand())
      {
         if (!ReceiveData(Observer, sckDataConnection))
            return false;
      }
      else
      {
         ASSERT(false);
         return false;
      }
      return true;
   }

   /// Opens an active data connection.
   /// @param[out] sckDataConnection
   /// @param[in] crDatachannelCmd Command to be executeted.
   /// @param[in] strPath Parameter for the command usually a path.
   /// @param[in] dwByteOffset Server marker at which file transfer is to be restarted.
   bool client_socket::OpenActiveDataConnection(::sockets::socket & sckDataConnectionParam, const command& crDatachannelCmd, const string& strPath, DWORD dwByteOffset)
   {
      if (!crDatachannelCmd.IsDatachannelCommand())
      {
         ASSERT(false);
         return false;
      }


      ::sockets::listen_socket_base & sckDataConnection 
         = *(dynamic_cast < ::sockets::listen_socket_base * >(&sckDataConnectionParam));

      //ll.m_strCat = m_strCat;
      //ll.m_strCipherList = m_strCipherList;

      sckDataConnection.m_bDetach = true;
      //m_strIp = "127.0.0.1";
      //if (m_iPort == 443)
      //{
      //   ll.EnableSSL();
      //}
      // INADDR_ANY = ip ::net::address of localhost
      // second parameter "0" means that the WINSOCKAPI ask for a port
      string strIp = "127.0.0.1";
      int iPort = 0;
      if (sckDataConnection.Bind(0, "tcp", 1))
      {
         string strMessage;
         strMessage.Format("could not bind to address %s %d", strIp, iPort);
         TRACE(strMessage);
         //System.simple_message_box(NULL, strMessage);
         return false;
      }
      m_handler.add(&sckDataConnection);


      WINUSHORT ushLocalSock = 0;
      try
      {
         // INADDR_ANY = ip ::net::address of localhost
         // second parameter "0" means that the WINSOCKAPI ask for a port
         //::net::address csaAddressTemp(INADDR_ANY, 0);
         //apSckServer->create(SOCK_STREAM);
         //apSckServer->bind(csaAddressTemp);
         //apSckServer->get_socket_address(csaAddressTemp);
         //ushLocalSock = csaAddressTemp.Port();
         //apSckServer->listen();
      }
      catch (::sockets::transfer_socket_exception& blockingException)
      {
         ReportError(blockingException.GetErrorMessage(), __FILE__, __LINE__);
         sckDataConnection.SetCloseAndDelete();
         return false;
      }
      
      ::net::address csaAddressTemp(INADDR_ANY, 0);
      csaAddressTemp = sckDataConnection.get_socket_address();
      ushLocalSock = csaAddressTemp.get_service_number();


      // get own ip ::net::address
      ::net::address csaLocalAddress;
      csaLocalAddress = get_socket_address();

      // transmit the socket (ip ::net::address + port) to the server
      // the FTP server establishes then the data connection
      if (DataPort(csaLocalAddress.get_display_number(), ushLocalSock) != FTP_OK)
         return false;

      // if resuming is activated then set offset
      if (m_fResumeIfPossible &&
         (crDatachannelCmd == command::STOR() || crDatachannelCmd == command::RETR() || crDatachannelCmd == command::APPE()) &&
         (dwByteOffset != 0 && Restart(dwByteOffset) != FTP_OK))
         return false;

      // send FTP command RETR/STOR/NLST/LIST to the server
      reply Reply;
      if (!SendCommand(crDatachannelCmd, strPath, Reply) ||
         !Reply.Code().IsPositivePreliminaryReply())
         return false;

      while (sckDataConnection.m_pbasesocket == NULL)
      {

         m_handler.select(1, 0);

      }

      return sckDataConnection.m_pbasesocket != NULL;

   }

   /// Opens a passive data connection.
   /// @param[out] sckDataConnection
   /// @param[in] crDatachannelCmd Command to be executeted.
   /// @param[in] strPath Parameter for the command usually a path.
   /// @param[in] dwByteOffset Server marker at which file transfer is to be restarted.
   bool client_socket::OpenPassiveDataConnection(::sockets::socket & sckDataConnectionParam, const command& crDatachannelCmd, const string& strPath, DWORD dwByteOffset)
   {
      
      if (m_econnectiontype == connection_type_tls_implicit)
      {
         reply Reply;
         if (!SendCommand(command::PROT(), { "P" }, Reply))
            return false;

         if (!Reply.Code().IsPositiveCompletionReply())
            return false;


      }

      if (!crDatachannelCmd.IsDatachannelCommand())
      {
         ASSERT(false);
         return false;
      }
      ::sockets::transfer_socket & sckDataConnection
         = *(dynamic_cast < ::sockets::transfer_socket * >(&sckDataConnectionParam));

      WINULONG   ulRemoteHostIP = 0;
      WINUSHORT  ushServerSock = 0;

      if (m_econnectiontype == connection_type_tls_implicit)
      {
         
         sckDataConnection.EnableSSL();

      }

      // set passive mode
      // the FTP server opens a port and tell us the socket (ip ::net::address + port)
      // this socket is used for opening the data connection
      if (Passive(ulRemoteHostIP, ushServerSock) != FTP_OK)
         return false;

      // establish connection
      ::net::address sockAddrTemp(::net::ipv4(ulRemoteHostIP, ushServerSock));
      try
      {
         if (!sckDataConnection.open(sockAddrTemp))
         {
            return false;
         }
         
      }
      catch (::sockets::transfer_socket_exception& blockingException)
      {
         ReportError(blockingException.GetErrorMessage(), __FILE__, __LINE__);
         sckDataConnection.SetCloseAndDelete();
         return false;
      }

      m_handler.add(&sckDataConnection);

      // if resuming is activated then set offset
      if (m_fResumeIfPossible &&
         (crDatachannelCmd == command::STOR() || crDatachannelCmd == command::RETR() || crDatachannelCmd == command::APPE()) &&
         (dwByteOffset != 0 && Restart(dwByteOffset) != FTP_OK))
         return false;


      // send FTP command RETR/STOR/NLST/LIST to the server
      reply Reply;
      if (!SendCommand(crDatachannelCmd, strPath, Reply) ||
         !Reply.Code().IsPositivePreliminaryReply())
         return false;

      return true;
   }

   /// Sends data over a socket to the server.
   /// @param[in] Observer Object for observing the execution of the command.
   /// @param[in] sckDataConnection Socket which is used for the send action.
   bool client_socket::SendData(itransfer_notification& Observer, ::sockets::transfer_socket & sckDataConnection)
   {
      try
      {
         
         ((client_socket *) this)->m_fTransferInProgress = true;

         int iNumWrite = 0;

         size_t bytesRead = 0;

         Observer.OnPreBytesSend(m_vBuffer.get_data(), m_vBuffer.size(), bytesRead);

         DWORD dwStart = ::get_tick_count();

         while (true)
         {

            if (bytesRead <= 0)
            {

               break;

            }

            if (::get_tick_count() - dwStart > mc_uiTimeout * 1000)
            {

               break;

            }

            {

               synch_lock sl(sckDataConnection.m_pmutex);

               sckDataConnection.write(m_vBuffer.get_data(), static_cast<int>(m_vBuffer.size()));

            }


            for (auto * p : (observer_array &)m_setObserver)
            {

               p->OnBytesSent(m_vBuffer, iNumWrite);

            }

            Observer.OnPreBytesSend(m_vBuffer.get_data(), m_vBuffer.size(), bytesRead);

            dwStart = ::get_tick_count();

         }

         ((client_socket *) this)->m_fTransferInProgress = false;

         if (m_fAbortTransfer)
         {

            _abort();

            return false;

         }

      }
      catch (::sockets::transfer_socket_exception& blockingException)
      {

         ((client_socket *) this)->m_fTransferInProgress = false;

         ReportError(blockingException.GetErrorMessage(), __FILE__, __LINE__);

         sckDataConnection.SetCloseAndDelete();

         return false;

      }

      return true;

   }


   /// Receives data over a socket from the server.
   /// @param[in] Observer Object for observing the execution of the command.
   /// @param[in] sckDataConnection Socket which is used for receiving the data.
   bool client_socket::ReceiveData(itransfer_notification& Observer, ::sockets::transfer_socket& sckDataConnection)
   {

      try
      {
      
         ((client_socket *) this)->m_fTransferInProgress = true;

         for (auto * p : (observer_array &)m_setObserver)
         {

            p->OnBeginReceivingData();

         }

         int iNumRead;

         long lTotalBytes = 0;

         DWORD dwStart = ::get_tick_count();

         while (true)
         {

            if (m_fAbortTransfer)
            {

               break;

            }

            if (::get_tick_count() - dwStart > mc_uiTimeout * 1000)
            {

               break;

            }
            
            {

               synch_lock sl(m_pmutex);

               iNumRead = sckDataConnection.m_file.remove_begin(m_vBuffer.get_data(), static_cast<int>(m_vBuffer.size()));

               if (sckDataConnection.GetSocket() == INVALID_SOCKET && iNumRead <= 0)
               {

                  break;

               }

            }

            if (iNumRead > 0)
            {

               lTotalBytes += iNumRead;

               for (auto * p : (observer_array &)m_setObserver)
               {

                  p->OnBytesReceived(m_vBuffer, iNumRead);

               }

               Observer.OnBytesReceived(m_vBuffer, iNumRead);

            }
            else
            {

               Sleep(100);

            }

            dwStart = ::get_tick_count();

         }

         for (auto * p : (observer_array &)m_setObserver)
         {

            p->OnEndReceivingData(lTotalBytes);

         }

         ((client_socket *) this)->m_fTransferInProgress = false;

         if (m_fAbortTransfer)
         {

            _abort();

            return false;

         }

      }
      catch (::sockets::transfer_socket_exception& blockingException)
      {

         ((client_socket *) this)->m_fTransferInProgress = false;

         ReportError(blockingException.GetErrorMessage(), __FILE__, __LINE__);

         sckDataConnection.SetCloseAndDelete();

         return false;

      }

      return true;

   }


   /// Sends a command to the server.
   /// @param[in] Command Command to send.
   bool client_socket::SendCommand(const command& Command, const stringa & Arguments)
   {
      
      if (!_is_connected())
      {

         return false;

      }

      try
      {
         for (auto * p : (observer_array &)m_setObserver)
            p->OnSendCommand(Command, Arguments);
         const std::string strCommand = Command.AsString(Arguments) + "\r\n";
         //write(strCommand, static_cast<int>(strCommand.length()), mc_uiTimeout);
         write(strCommand, static_cast<int>(strCommand.length()));
      }
      catch (::sockets::transfer_socket_exception& blockingException)
      {
         ReportError(blockingException.GetErrorMessage(), __FILE__, __LINE__);
         const_cast<client_socket*>(this)->SetCloseAndDelete();
         return false;
      }
      Sleep(200);
      return true;
   }

   /// Sends a command to the server.
   /// @param[in]  Command Command to send.
   /// @param[out] Reply The Reply of the server to the sent command.
   bool client_socket::SendCommand(const command& Command, const stringa & Arguments, reply& Reply)
   {
      if (!SendCommand(Command, Arguments))
      {
         return false;
      }
      if(!GetResponse(Reply))
         return false;
      return true;
   }

   /// This function gets the server response.
   /// A server response can exists of more than one line. This function
   /// returns the full response (multiline).
   /// @param[out] Reply Reply of the server to a command.
   bool client_socket::GetResponse(reply& Reply)
   {
      string strResponse;
      if (!GetSingleResponseLine(strResponse))
         return false;

      if (strResponse.length() > 3 && strResponse[3] == _T('-'))
      {
         string strSingleLine(strResponse);
         const int iRetCode = atoi(strResponse);
         // handle multi-line server responses
         while (!(strSingleLine.length() > 3 &&
            strSingleLine[3] == _T(' ') &&
            atoi_dup(strSingleLine) == iRetCode))
         {
            if (!GetSingleResponseLine(strSingleLine))
               return false;
            strResponse += mc_strEolCharacterSequence + strSingleLine;
         }
      }

      bool fRet = Reply.Set(strResponse);

      for (auto * p : (observer_array &)m_setObserver)
         p->OnResponse(Reply);

      return fRet;
   }

   void client_socket::OnLine(const string & strLine)
   {

      single_lock sl(m_pmutex);

      m_qResponseBuffer.add_tail(strLine);

   }


   /// Reads a single response line from the server control channel.
   /// @param[out] strResponse Response of the server as string.
   bool client_socket::GetSingleResponseLine(string& strResponse)
   {
      
      if (!_is_connected())
         return false;

      try
      {

         DWORD dwStart = get_tick_count();

         // xxx yyy zzz
         //while (get_tick_count() - dwStart < 10 * 1000)
         while (get_tick_count() - dwStart < 100 * 1000)
         {

            {

               single_lock sl(m_pmutex);

               if (m_qResponseBuffer.has_elements())
               {

                  break;

               }

            }

            if (IsDetached())
            {

               Sleep(100);

            }
            else
            {

               m_handler.select(1,0);

               if (IsSSL())
               {

                  //Sleep(200);

               }

            }

            //// internal buffer is empty ==> get response from FTP server
            //int iNum = 0;
            //std::string strTemp;

            //do
            //{
            //   iNum = receive(m_vBuffer.get_data(), static_cast<int>(m_vBuffer.size()) - 1, mc_uiTimeout);
            //   if (mc_uiResponseWait != 0)
            //      Sleep(mc_uiResponseWait);
            //   ((memory &)m_vBuffer)[iNum] = '\0';
            //   strTemp += m_vBuffer.to_string();
            //} while (iNum == static_cast<int>(m_vBuffer.size()) - 1);

            //// each line in response is a separate entry in the internal buffer
            //while (strTemp.length())
            //{
            //   size_t iCRLF = strTemp.find('\n');
            //   if (iCRLF != std::string::npos)
            //   {
            //      m_qResponseBuffer.add_tail(strTemp.substr(0, iCRLF + 1));
            //      strTemp.erase(0, iCRLF + 1);
            //   }
            //   else
            //   {
            //      // this is not rfc standard; normally each command must end with CRLF
            //      // in this case it doesn't
            //      m_qResponseBuffer.add_tail(strTemp);
            //      strTemp.clear();
            //   }
            //}

            //if (m_qResponseBuffer.is_empty())
            //   return false;
         }

         if (m_qResponseBuffer.is_empty())
         {

            return false;

         }

         // get first response-line from buffer
         {

            single_lock sl(m_pmutex);

            strResponse = m_qResponseBuffer.remove_head();

         }

         // remove CrLf if exists (don't use mc_strEolCharacterSequence here)
         if (strResponse.length() > 1 && strResponse.substr(strResponse.length() - 2) == _T("\r\n"))
            strResponse.erase(strResponse.length() - 2, 2);
      }
      catch (::sockets::transfer_socket_exception& blockingException)
      {
         ReportError(blockingException.GetErrorMessage(), __FILE__, __LINE__);
         const_cast<client_socket*>(this)->SetCloseAndDelete();
         return false;
      }

      return true;
   }

   /// Executes the FTP command CDUP (change to parent directory).
   /// This command is a special case of client_socket::ChangeWorkingDirectory
   /// (CWD), and is  included to simplify the implementation of programs for
   /// transferring directory trees between operating systems having different
   /// syntaxes for naming the parent directory.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::ChangeToParentDirectory()
   {
      reply Reply;
      if (!SendCommand(command::CDUP(), {}, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command QUIT.
   /// This command terminates a USER and if file transfer is not in progress,
   /// the server closes the control connection. If file transfer is in progress,
   /// the connection will remain open for result response and the server will
   /// then close it.
   /// If the user-process is transferring files for several USERs but does not
   /// wish to close and then reopen connections for each, then the REIN command
   /// should be used instead of QUIT.
   /// An unexpected close on the control connection will cause the server to take
   /// the effective action of an abort (ABOR) and a logout.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::Logout()
   {
      reply Reply;
      if (!SendCommand(command::QUIT(), {}, Reply))
         return FTP_ERROR;

      CloseControlChannel();

      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command PASV. Set the passive mode.
   /// This command requests the server-DTP (data transfer process) on a data to
   /// "listen"  port (which is not its default data port) and to wait for a
   /// connection rather than initiate one upon receipt of a transfer command.
   /// The response to this command includes the host and port ::net::address this
   /// server is listening on.
   /// @param[out] ulIpAddress IP ::net::address the server is listening on.
   /// @param[out] ushPort Port the server is listening on.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::Passive(WINULONG& ulIpAddress, WINUSHORT& ushPort)
   {
      reply Reply;
      if (!SendCommand(command::PASV(), {}, Reply))
         return FTP_ERROR;

      if (Reply.Code().IsPositiveCompletionReply())
      {
         if (!GetIpAddressFromResponse(Reply.Value(), ulIpAddress, ushPort))
            return FTP_ERROR;
      }

      return SimpleErrorCheck(Reply);
   }

   /// Parses a response string and extracts the ip ::net::address and port information.
   /// @param[in]  strResponse The response string of a FTP server which holds
   ///                         the ip ::net::address and port information.
   /// @param[out] ulIpAddress Buffer for the ip ::net::address.
   /// @param[out] ushPort     Buffer for the port information.
   /// @retval true  Everything went ok.
   /// @retval false An error occurred (invalid format).
   bool client_socket::GetIpAddressFromResponse(const string& strResponse, WINULONG& ulIpAddress, WINUSHORT& ushPort)
   {
      // parsing of ip-::net::address and port implemented with a finite state machine
      // ...(192,168,1,1,3,44)...
      enum T_enState { state0, state1, state2, state3, state4 } enState = state0;

      string strIpAddress, strPort;
      WINUSHORT ushTempPort = 0;
      WINULONG  ulTempIpAddress = 0;
      int iCommaCnt = 4;
      for (strsize i = 0; i < strResponse.get_length(); i++)
      {
         char it = strResponse[i];
         switch (enState)
         {
         case state0:
            if (it == _T('('))
               enState = state1;
            break;
         case state1:
            if (it == _T(','))
            {
               if (--iCommaCnt == 0)
               {
                  enState = state2;
                  ulTempIpAddress += atoi(strIpAddress);
               }
               else
               {
                  ulTempIpAddress += atoi(strIpAddress) << 8 * iCommaCnt;
                  strIpAddress.clear();
               }
            }
            else
            {
               if (!isdigit_dup(it))
                  return false;
               strIpAddress += it;
            }
            break;
         case state2:
            if (it == _T(','))
            {
               ushTempPort = static_cast<WINUSHORT>(atoi(strPort) << 8);
               strPort.clear();
               enState = state3;
            }
            else
            {
               if (!isdigit_dup(it))
                  return false;
               strPort += it;
            }
            break;
         case state3:
            if (it == _T(')'))
            {
               // compiler warning if using +=operator
               ushTempPort = ushTempPort + static_cast<WINUSHORT>(atoi(strPort));
               enState = state4;
            }
            else
            {
               if (!isdigit_dup(it))
                  return false;
               strPort += it;
            }
            break;
         case state4:
            break; // some compilers complain if not all enumeration values are listet
         }
      }

      if (enState == state4)
      {
         ulIpAddress = ulTempIpAddress;
         ushPort = ushTempPort;
      }

      return enState == state4;
   }

   /// Executes the FTP command ABOR.
   /// This command tells the server to abort the previous FTP service command
   /// and any associated transfer of data.  The abort command may require
   /// "special action", as discussed in the Section on FTP Commands, to force
   /// recognition by the server. No action is to be taken if the previous
   /// command has been completed (including data transfer). The control
   /// connection is not to be closed by the server, but the data connection
   /// must be closed.
   /// There are two cases for the server upon receipt of this command:<BR>
   /// (1) the FTP service command was already completed, or <BR>
   /// (2) the FTP service command is still in progress.<BR>
   /// In the first case, the server closes the data connection (if it is open)
   /// and responds with a 226 reply, indicating that the abort command was
   /// successfully processed.
   /// In the second case, the server aborts the FTP service in progress and
   /// closes the data connection, returning a 426 reply to indicate that the
   /// service request terminated abnormally. The server then sends a 226 reply,
   /// indicating that the abort command was successfully processed.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::_abort()
   {
      if (m_fTransferInProgress)
      {
         m_fAbortTransfer = true;
         return FTP_OK;
      }

      m_fAbortTransfer = false;
      reply Reply;
      if (!SendCommand(command::ABOR(), {}, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command PWD (PRINT WORKING DIRECTORY)
   /// This command causes the name of the current working directory
   /// to be returned in the reply.
   int client_socket::PrintWorkingDirectory()
   {
      reply Reply;
      if (!SendCommand(command::PWD(), {}, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command SYST (SYSTEM)
   /// This command is used to find out the type of operating system at the server.
   /// The reply shall have as its first word one of the system names listed in the
   /// current version of the Assigned Numbers document [Reynolds, Joyce, and
   /// Jon Postel, "Assigned Numbers", RFC 943, ISI, April 1985.].
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::system()
   {
      reply Reply;
      if (!SendCommand(command::SYST(), {}, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command NOOP
   /// This command does not affect any parameters or previously entered commands.
   /// It specifies no action other than that the server send an FTP_OK reply.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::Noop()
   {
      reply Reply;
      if (!SendCommand(command::NOOP(), {}, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command PORT (DATA PORT)
   /// The argument is a HOST-PORT specification for the data port to be used in data
   /// connection. There are defaults for both the user and server data ports, and
   /// under normal circumstances this command and its reply are not needed.  If
   /// this command is used, the argument is the concatenation of a 32-bit internet
   /// host ::net::address and a 16-bit TCP port ::net::address.
   /// @param[in] strHostIP IP-::net::address like xxx.xxx.xxx.xxx
   /// @param[in] uiPort 16-bit TCP port ::net::address.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::DataPort(const string& strHostIP, WINUSHORT ushPort)
   {
      string strPortArguments;
      // convert the port number to 2 bytes + add to the local IP
      strPortArguments = strHostIP + "," + ::str::from (ushPort >> 8) + "," + ::str::from (ushPort & 0xFF);

      strPortArguments.replace(".", ",");

      reply Reply;
      if (!SendCommand(command::PORT(), strPortArguments, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command TYPE (REPRESENTATION TYPE)
   /// Caches the representation state if successful.
   /// see Documentation of nsFTP::representation
   /// @param[in] representation see Documentation of nsFTP::representation
   /// @param[in] iSize Indicates Bytesize for type LocalByte.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::RepresentationType(const representation& representation, DWORD dwSize)
   {
      // check representation
      if (m_apCurrentRepresentation.is_set() && representation == *m_apCurrentRepresentation)
         return FTP_OK;

      const int iRet = _RepresentationType(representation, dwSize);

      if (iRet == FTP_OK)
      {
         if (m_apCurrentRepresentation.is_null())
            m_apCurrentRepresentation = canew(::ftp::representation(representation));
         else
            *m_apCurrentRepresentation = representation;
      }
      else
         m_apCurrentRepresentation.release();

      return iRet;
   }

   /// Executes the FTP command TYPE (REPRESENTATION TYPE)
   /// see Documentation of nsFTP::representation
   /// @param[in] representation see Documentation of nsFTP::representation
   /// @param[in] dwSize Indicates Bytesize for type LocalByte.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::_RepresentationType(const representation& representation, DWORD dwSize)
   {
      stringa Arguments(representation.Type().AsString());

      switch (representation.Type().AsEnum())
      {
      case type::tyLocalByte:
         Arguments.add(::str::from((uint32_t) dwSize));
         break;
      case type::tyASCII:
      case type::tyEBCDIC:
      case type::tyImage:
         if (representation.Format().IsValid())
            Arguments.push_back(representation.Format().AsString());
      }

      reply Reply;
      if (!SendCommand(command::TYPE(), Arguments, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command CWD (CHANGE WORKING DIRECTORY)
   /// This command allows the user to work with a different directory or dataset
   /// for file storage or retrieval without altering his login or accounting
   /// information. Transfer parameters are similarly unchanged.
   /// @param[in] strDirectory Pathname specifying a directory or other system
   ///                         dependent file group designator.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::ChangeWorkingDirectory(const string& strDirectory)
   {
      ASSERT(!strDirectory.empty());
      reply Reply;
      if (!SendCommand(command::CWD(), strDirectory, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command MKD (MAKE DIRECTORY)
   /// This command causes the directory specified in the pathname to be created
   /// as a directory (if the pathname is absolute) or as a subdirectory of the
   /// current working directory (if the pathname is relative).
   /// @pararm[in] strDirectory Pathname specifying a directory to be created.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::MakeDirectory(const string& strDirectory)
   {
      ASSERT(!strDirectory.empty());
      reply Reply;
      if (!SendCommand(command::MKD(), strDirectory, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command SITE (SITE PARAMETERS)
   /// This command is used by the server to provide services specific to his
   /// system that are essential to file transfer but not sufficiently universal
   /// to be included as commands in the protocol.  The nature of these services
   /// and the specification of their syntax can be stated in a reply to the HELP
   /// SITE command.
   /// @param[in] strCmd Command to be executed.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::SiteParameters(const string& strCmd)
   {
      ASSERT(!strCmd.empty());
      reply Reply;
      if (!SendCommand(command::SITE(), strCmd, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command HELP
   /// This command shall cause the server to send helpful information regarding
   /// its implementation status over the control connection to the user.
   /// The command may take an argument (e.g., any command name) and return more
   /// specific information as a response.  The reply is type 211 or 214.
   /// It is suggested that HELP be allowed before entering a USER command. The
   /// server may use this reply to specify site-dependent parameters, e.g., in
   /// response to HELP SITE.
   /// @param[in] strTopic Topic of the requested help.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::Help(const string& strTopic)
   {
      reply Reply;
      if (!SendCommand(command::HELP(), strTopic, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command DELE (DELETE)
   /// This command causes the file specified in the pathname to be deleted at the
   /// server site.  If an extra level of protection is desired (such as the query,
   /// "Do you really wish to delete?"), it should be provided by the user-FTP process.
   /// @param[in] strFile Pathname of the file to delete.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::Delete(const string& strFile)
   {
      ASSERT(!strFile.empty());
      reply Reply;
      if (!SendCommand(command::DELE(), strFile, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command RMD (REMOVE DIRECTORY)
   /// This command causes the directory specified in the pathname to be removed
   /// as a directory (if the pathname is absolute) or as a subdirectory of the
   /// current working directory (if the pathname is relative).
   /// @param[in] strDirectory Pathname of the directory to delete.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::RemoveDirectory(const string& strDirectory)
   {
      ASSERT(!strDirectory.empty());
      reply Reply;
      if (!SendCommand(command::RMD(), strDirectory, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command STRU (FILE STRUCTURE)
   /// see documentation of nsFTP::structure
   /// The default structure is File.
   /// @param[in] crStructure see Documentation of nsFTP::structure
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::FileStructure(const structure& crStructure)
   {
      reply Reply;
      if (!SendCommand(command::STRU(), crStructure.AsString(), Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command MODE (TRANSFER MODE)
   /// see documentation of nsFTP::transfer_mode
   /// The default transfer mode is Stream.
   /// @param[in] crTransferMode see Documentation of nsFTP::transfer_mode
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::TransferMode(const transfer_mode& crTransferMode)
   {
      reply Reply;
      if (!SendCommand(command::MODE(), crTransferMode.AsString(), Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command STAT (STATUS)
   /// This command shall cause a status response to be sent over the control
   /// connection in the form of a reply. The command may be sent during a file
   /// transfer (along with the Telnet IP and Synch signals--see the Section on
   /// FTP Commands) in which case the server will respond with the status of the
   /// operation in progress, or it may be sent between file transfers. In the
   /// latter case, the command may have an argument field.
   /// @param[in] strPath If the argument is a pathname, the command is analogous
   ///                    to the "list" command except that data shall be transferred
   ///                    over the control connection. If a partial pathname is
   ///                    given, the server may respond with a list of file names or
   ///                    attributes associated with that specification. If no argument
   ///                    is given, the server should return general status information
   ///                    about the server FTP process. This should include current
   ///                    values of all transfer parameters and the status of connections.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::Status(const string& strPath)
   {
      reply Reply;
      if (!SendCommand(command::STAT(), strPath, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command ALLO (ALLOCATE)
   /// This command may be required by some servers to reserve sufficient storage
   /// to accommodate the new file to be transferred.
   /// @param[in] iReserveBytes The argument shall be a decimal integer representing
   ///                          the number of bytes (using the logical byte size) of
   ///                          storage to be reserved for the file. For files sent
   ///                          with record or page structure a maximum record or page
   ///                          size (in logical bytes) might also be necessary; this
   ///                          is indicated by a decimal integer in a second argument
   ///                          field of the command.
   /// @pararm[in] piMaxPageOrRecordSize This second argument is optional. This command
   ///                          shall be followed by a STORe or APPEnd command.
   ///                          The ALLO command should be treated as a NOOP (no operation)
   ///                          by those servers which do not require that the maximum
   ///                          size of the file be declared beforehand, and those servers
   ///                          interested in only the maximum record or page size should
   ///                          accept a dummy value in the first argument and ignore it.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::Allocate(int iReserveBytes, const int* piMaxPageOrRecordSize/*=NULL*/)
   {
      stringa Arguments(::str::from(iReserveBytes));
      if (piMaxPageOrRecordSize != NULL)
      {
         Arguments.push_back(_T("R"));
         Arguments.push_back(::str::from(*piMaxPageOrRecordSize));
      }

      reply Reply;
      if (!SendCommand(command::ALLO(), Arguments, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command SMNT ()
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::StructureMount(const string& strPath)
   {
      reply Reply;
      if (!SendCommand(command::SMNT(), strPath, Reply))
         return FTP_ERROR;
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command (STRUCTURE MOUNT)
   /// This command allows the user to mount a different file system data structure
   /// without altering his login or accounting information. Transfer parameters
   /// are similarly unchanged.  The argument is a pathname specifying a directory
   /// or other system dependent file group designator.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::Reinitialize()
   {
      reply Reply;
      if (!SendCommand(command::REIN(), {}, Reply))
         return FTP_ERROR;

      if (Reply.Code().IsPositiveCompletionReply())
         return FTP_OK;
      else if (Reply.Code().IsPositivePreliminaryReply())
      {
         if (!GetResponse(Reply) || !Reply.Code().IsPositiveCompletionReply())
            return FTP_ERROR;
      }
      else if (Reply.Code().IsNegativeReply())
         return FTP_NOTOK;

      ASSERT(Reply.Code().IsPositiveIntermediateReply());
      return FTP_ERROR;
   }

   /// Executes the FTP command REST (RESTART)
   /// This command does not cause file transfer but skips over the file to the
   /// specified data checkpoint. This command shall be immediately followed
   /// by the appropriate FTP service command which shall cause file transfer
   /// to resume.
   /// @param[in] dwPosition Represents the server marker at which file transfer
   ///                       is to be restarted.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::Restart(DWORD dwPosition)
   {
      reply Reply;
      if (!SendCommand(command::REST(), stringa(::str::from((uint32_t) dwPosition)), Reply))
         return FTP_ERROR;

      if (Reply.Code().IsPositiveIntermediateReply())
         return FTP_OK;
      else if (Reply.Code().IsNegativeReply())
         return FTP_NOTOK;

      ASSERT(Reply.Code().IsPositiveReply());

      return FTP_ERROR;
   }

   /// Executes the FTP command SIZE
   /// Return size of file.
   /// SIZE is not specified in RFC 959.
   /// @param[in] Pathname of a file.
   /// @param[out] Size of the file specified in pathname.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::FileSize(const string& strPath, long& lSize)
   {
      reply Reply;
      if (!SendCommand(command::SIZE(), strPath, Reply))
         return FTP_ERROR;
      lSize = atoi(Reply.Value().substr(4));
      return SimpleErrorCheck(Reply);
   }

   /// Executes the FTP command MDTM
   /// Show last modification time of file.
   /// MDTM is not specified in RFC 959.
   /// @param[in] strPath Pathname of a file.
   /// @param[out] strModificationTime Modification time of the file specified in pathname.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::FileModificationTime(const string& strPath, string& strModificationTime)
   {
      strModificationTime.erase();

      reply Reply;
      if (!SendCommand(command::MDTM(), strPath, Reply))
         return FTP_ERROR;

      if (Reply.Value().length() >= 18)
      {
         string strTemp(Reply.Value().substr(4));
         size_t iPos = strTemp.find(_T('.'));
         if (iPos != string::npos)
            strTemp = strTemp.substr(0, iPos);
         if (strTemp.length() == 14)
            strModificationTime = strTemp;
      }

      if (strModificationTime.empty())
         return FTP_ERROR;

      return SimpleErrorCheck(Reply);
   }

   /// Show last modification time of file.
   /// @param[in] strPath Pathname of a file.
   /// @param[out] tmModificationTime Modification time of the file specified in pathname.
   /// @return see return values of client_socket::SimpleErrorCheck
   int client_socket::FileModificationTime(const string& strPath, tm& tmModificationTime)
   {
      string strTemp;
      const int iRet = FileModificationTime(strPath, strTemp);

      memset(&tmModificationTime, 0, sizeof(tmModificationTime));
      if (iRet == FTP_OK)
      {
         tmModificationTime.tm_year = atoi(strTemp.substr(0, 4));
         tmModificationTime.tm_mon = atoi(strTemp.substr(4, 2));
         tmModificationTime.tm_mday = atoi(strTemp.substr(6, 2));
         tmModificationTime.tm_hour = atoi(strTemp.substr(8, 2));
         tmModificationTime.tm_min = atoi(strTemp.substr(10, 2));
         tmModificationTime.tm_sec = atoi(strTemp.substr(12));
      }
      return iRet;
   }

   /// Notifies all observers that an error occurred.
   /// @param[in] strErrorMsg Error message which is reported to all observers.
   /// @param[in] Name of the sourcecode file where the error occurred.
   /// @param[in] Line number in th sourcecode file where the error occurred.
   void client_socket::ReportError(const string& strErrorMsg, const string& strFile, DWORD dwLineNr)
   {
      for (auto * p : (observer_array &)m_setObserver)
         p->OnInternalError(strErrorMsg, strFile, dwLineNr);
   }


} // namespace ftp





