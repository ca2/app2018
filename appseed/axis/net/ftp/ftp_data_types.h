////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

#pragma once



namespace ftp
{
   
  


   // constants
   const TCHAR ANONYMOUS_USER[] = _T("anonymous");
   enum T_enConstants {
      DEFAULT_FTP_PORT = 21, ///< The default port that an FTP service listens to on a remote host
      FTP_ERROR  = -1,
      FTP_OK     =  0,
      FTP_NOTOK  =  1, };

   /// Data Structure
   class structure
   {
   public:
      enum e_type { type_file, type_record, type_page };

      structure(const structure& structure) :
         m_enStructure(structure.AsEnum()) {}

      bool operator==(const e_type& rhs) const { return m_enStructure==rhs; }
      bool operator!=(const e_type& rhs) const { return !operator==(rhs); }
      bool operator==(const structure& rhs) const { return m_enStructure==rhs.m_enStructure; }
      bool operator!=(const structure& rhs) const { return !operator==(rhs); }

      structure& operator=(const structure& rhs) { m_enStructure = rhs.AsEnum(); return *this; }

      e_type AsEnum() const { return m_enStructure; }
      string AsString() const;

      static const structure File()   { return type_file;   }
      static const structure Record() { return type_record; }
      static const structure Page()   { return type_page;   }

   private:
      structure(e_type enStructure) : m_enStructure(enStructure) {}
      e_type m_enStructure;
   };

   /// Transmission Modes
   class transfer_mode
   {
   public:
      enum e_transfer_mode { transfer_mode_stream, transfer_mode_block, transfer_mode_compressed };

      transfer_mode(e_transfer_mode enTransferMode) : m_etransfermode(enTransferMode) {}
      e_transfer_mode m_etransfermode;
      transfer_mode(const transfer_mode& transferMode) :
         m_etransfermode(transferMode.AsEnum()) {}

      bool operator==(const e_transfer_mode& rhs) const { return m_etransfermode==rhs; }
      bool operator!=(const e_transfer_mode& rhs) const { return !operator==(rhs); }
      bool operator==(const transfer_mode& rhs) const { return m_etransfermode==rhs.m_etransfermode; }
      bool operator!=(const transfer_mode& rhs) const { return !operator==(rhs); }

      transfer_mode& operator=(const transfer_mode& rhs) { m_etransfermode = rhs.AsEnum(); return *this; }

      e_transfer_mode AsEnum() const { return m_etransfermode; }
      string AsString() const;

      static const transfer_mode Stream()     { return transfer_mode_stream;     }
      static const transfer_mode Block()      { return transfer_mode_block;      }
      static const transfer_mode Compressed() { return transfer_mode_compressed; }

   };

   class firewall_type;
   typedef array<firewall_type> TFirewallTypeVector;

   /// Firewall Type
   class firewall_type
   {
   public:
      // don't change order of enumeration
      enum e_type {
         type_none, type_host_name, type_user_after_logon, type_proxy_open, type_transparent,
         type_user_with_no_logon, type_user_fire_id_at_remote_host, type_user_remote_id_at_remote_host_fire_id, 
         type_user_remote_id_at_fire_id_at_remote_host };

      firewall_type() : m_etype(type_none) {}
      firewall_type(const firewall_type& firewallType) :
         m_etype(firewallType.AsEnum()) {}

      bool operator==(const e_type& rhs) const { return m_etype==rhs; }
      bool operator!=(const e_type& rhs) const { return !operator==(rhs); }
      bool operator==(const firewall_type& rhs) const { return m_etype==rhs.m_etype; }
      bool operator!=(const firewall_type& rhs) const { return !operator==(rhs); }

      firewall_type& operator=(const firewall_type& rhs) { m_etype = rhs.AsEnum(); return *this; }

      e_type AsEnum() const { return m_etype; }

      string AsDisplayString() const;
      string AsStorageString() const;
      static void GetAllTypes(TFirewallTypeVector& vTypes);

      static const firewall_type None()                             { return type_none;                             }
      static const firewall_type SiteHostName()                     { return type_host_name;                     }
      static const firewall_type UserAfterLogon()                   { return type_user_after_logon;                   }
      static const firewall_type ProxyOpen()                        { return type_proxy_open;                        }
      static const firewall_type Transparent()                      { return type_transparent;                      }
      static const firewall_type UserWithNoLogon()                  { return type_user_with_no_logon;                  }
      static const firewall_type UserFireIDatRemotehost()           { return type_user_fire_id_at_remote_host;           }
      static const firewall_type UserRemoteIDatRemoteHostFireID()   { return type_user_remote_id_at_remote_host_fire_id;   }
      static const firewall_type UserRemoteIDatFireIDatRemoteHost() { return type_user_remote_id_at_fire_id_at_remote_host; }

      firewall_type(e_type enFirewallType) : m_etype(enFirewallType) {}
      e_type m_etype;
   };

   /// @brief Representation Type - 1st param (see representation)
   class type
   {
   public:
      enum e_type { tyASCII, tyEBCDIC, tyImage, tyLocalByte };

      type(const type& type) :
         m_etype(type.AsEnum()) {}

      bool operator==(const e_type& rhs) const { return m_etype==rhs; }
      bool operator!=(const e_type& rhs) const { return !operator==(rhs); }
      bool operator==(const type& rhs) const { return m_etype==rhs.m_etype; }
      bool operator!=(const type& rhs) const { return !operator==(rhs); }

      type& operator=(const type& rhs) { m_etype = rhs.AsEnum(); return *this; }

      e_type AsEnum() const { return m_etype; }
      string AsString() const;

      static const type ASCII()     { return tyASCII;     }
      static const type EBCDIC()    { return tyEBCDIC;    }
      static const type Image()     { return tyImage;     }
      static const type LocalByte() { return tyLocalByte; }

      type(e_type enType) : m_etype(enType) {}
      e_type m_etype;
   };

   /// @brief Representation Type - 2nd param (see representation)
   class format
   {
   public:
      enum e_format { format_invalid, format_non_print, format_telnet, format_carriage_control };

      format() : m_eformat(format_invalid) {}
      format(const format& format) :
         m_eformat(format.AsEnum()) {}

      bool operator==(const e_format& rhs) const { return m_eformat==rhs; }
      bool operator!=(const e_format& rhs) const { return !operator==(rhs); }
      bool operator==(const format& rhs) const { return m_eformat==rhs.m_eformat; }
      bool operator!=(const format& rhs) const { return !operator==(rhs); }

      format& operator=(const format& rhs) { m_eformat = rhs.AsEnum(); return *this; }

      e_format AsEnum() const { return m_eformat; }
      string AsString() const;
      bool IsValid() const { return m_eformat != format_invalid; }

      static const format NonPrint()        { return format_non_print;        }
      static const format TelnetFormat()    { return format_telnet;    }
      static const format CarriageControl() { return format_carriage_control; }

      format(e_format enTypeFormat) : m_eformat(enTypeFormat) {}
      e_format m_eformat;
   };

   /// Representation Type (see also type and format)
   class representation :
      virtual public ::object
   {
   public:
      representation(type Type) : m_Type(Type) {}
      representation(type Type, format Format) : m_Type(Type), m_Format(Format) {}

      bool operator==(const representation& rhs) const { return rhs.m_Type == m_Type && rhs.m_Format == m_Format; }
      bool operator!=(const representation& rhs) const { return !operator==(rhs); }
      representation& operator=(const representation& rhs)
      { 
         m_Type = rhs.m_Type;
         m_Format = rhs.m_Format;
         return *this;
      }

      const type&       Type()   const { return m_Type; }
      const format& Format() const { return m_Format; }

      type       m_Type;
      format m_Format;
   };

   //class args : public stringa
   //{
   //public:
   //   args() {}
   //   args(const string& strArgument) { push_back(strArgument); }
   //   args(const string& strFirstArgument, const string& strSecondArgument) { push_back(strFirstArgument); push_back(strSecondArgument); }
   //   args(const string& strFirstArgument, const string& strSecondArgument, const string& strThirdArgument) { push_back(strFirstArgument); push_back(strSecondArgument); push_back(strThirdArgument); }
   //};

   class command
   {
   public:
      enum e_command { cmdABOR, cmdACCT, cmdALLO, cmdAPPE, cmdCDUP, cmdCWD, cmdDELE, cmdHELP, cmdLIST, cmdMDTM, cmdMKD, cmdMODE, cmdNLST, cmdNOOP, cmdOPEN, cmdPASS, cmdPASV, cmdPORT, cmdPWD, cmdQUIT, cmdREIN, cmdREST, cmdRETR, cmdRMD, cmdRNFR, cmdRNTO, cmdSITE, cmdSIZE, cmdSMNT, cmdSTAT, cmdSTOR, cmdSTOU, cmdSTRU, cmdSYST, cmdTYPE, cmdUSER, };
      enum TSpecificationEnum { Unknown, RFC959, RFC3659, };
      enum e_type { DatachannelRead, DatachannelWrite, NonDatachannel, };

      class iextended_info : public iinterface
      {
      public:
         virtual ~iextended_info() {}
         virtual const string& GetServerString() const = 0;
         virtual const string& GetCompleteServerStringSyntax() const = 0;
         virtual UINT GetNumberOfParameters() const = 0;
         virtual UINT GetNumberOfOptionalParameters() const = 0;
         virtual TSpecificationEnum GetSpecification() const = 0;
         virtual e_type GetType() const = 0;
      };

      command(const command& datachannelCmd) :
         m_enCommand(datachannelCmd.AsEnum()) {}

      bool operator==(e_command rhs) const { return m_enCommand==rhs; }
      bool operator!=(e_command rhs) const { return !operator==(rhs); }

      bool operator==(const command& rhs) const { return m_enCommand==rhs.m_enCommand; }
      bool operator!=(const command& rhs) const { return !operator==(rhs); }

      command& operator=(const command& rhs)
      {
         m_enCommand = rhs.AsEnum();
         return *this;
      }

      e_command AsEnum() const { return m_enCommand; }
      string AsString() const;
      string AsString(const stringa & Arguments) const;
      const iextended_info& GetExtendedInfo() const;

      bool IsDatachannelReadCommand() const;
      bool IsDatachannelWriteCommand() const;
      bool IsDatachannelCommand() const;

      static const command ABOR() { return cmdABOR; }
      static const command ACCT() { return cmdACCT; }
      static const command ALLO() { return cmdALLO; }
      static const command APPE() { return cmdAPPE; }
      static const command CDUP() { return cmdCDUP; }
      static const command CWD()  { return cmdCWD;  }
      static const command DELE() { return cmdDELE; }
      static const command HELP() { return cmdHELP; }
      static const command LIST() { return cmdLIST; }
      static const command MDTM() { return cmdMDTM; }
      static const command MKD()  { return cmdMKD;  }
      static const command MODE() { return cmdMODE; }
      static const command NLST() { return cmdNLST; }
      static const command NOOP() { return cmdNOOP; }
      static const command OPEN() { return cmdOPEN; }
      static const command PASS() { return cmdPASS; }
      static const command PASV() { return cmdPASV; }
      static const command PORT() { return cmdPORT; }
      static const command PWD()  { return cmdPWD;  }
      static const command QUIT() { return cmdQUIT; }
      static const command REIN() { return cmdREIN; }
      static const command REST() { return cmdREST; }
      static const command RETR() { return cmdRETR; }
      static const command RMD()  { return cmdRMD;  }
      static const command RNFR() { return cmdRNFR; }
      static const command RNTO() { return cmdRNTO; }
      static const command SITE() { return cmdSITE; }
      static const command SIZE() { return cmdSIZE; }
      static const command SMNT() { return cmdSMNT; }
      static const command STAT() { return cmdSTAT; }
      static const command STOR() { return cmdSTOR; }
      static const command STOU() { return cmdSTOU; }
      static const command STRU() { return cmdSTRU; }
      static const command SYST() { return cmdSYST; }
      static const command TYPE() { return cmdTYPE; }
      static const command USER() { return cmdUSER; }

      class extended_info;
      class info2;
      command(e_command enDatachannelCmd) : m_enCommand(enDatachannelCmd) {}
      e_command m_enCommand;
   };

   /// @brief Structure for logon information.
   ///
   /// Holds all necessary parameters for logging on a ftp-server.
   /// Includes also the parameters which are needed for firewall logon.
   class logon
   {
   public:
      logon();
      logon(const string& strHostname, USHORT ushHostport=DEFAULT_FTP_PORT, const string& strUsername=ANONYMOUS_USER,
                 const string& strPassword=_T("anonymous@user.com"), const string& strAccount=_T(""));
      logon(const string& strHostname, USHORT ushHostport, const string& strUsername, const string& strPassword,
                 const string& strAccount, const string& strFwHostname, const string& strFwUsername, const string& strFwPassword,
                 USHORT ushFwPort, const firewall_type& crFwType);

      void SetHost(const string& strHostname, USHORT ushHostport=DEFAULT_FTP_PORT, const string& strUsername=ANONYMOUS_USER, 
                   const string& strPassword=_T("anonymous@user.com"), const string& strAccount=_T(""));

      void SetFirewall(const string& strFwHostname, const string& strFwUsername, const string& strFwPassword,
                       USHORT ushFwPort, const firewall_type& crFwType);

      void DisableFirewall() { m_FwType = firewall_type::None(); }

      string&       Hostname()    { return m_strHostname;    }
      USHORT               Hostport()   const  { return m_ushHostport;    }
      string&       Username()    { return m_strUsername;    }
      string&       Password()    { return m_strPassword;    }
      const string&       Account()    const  { return m_strAccount;     }
      const string&       FwHost()     const  { return m_strFwHostname;  }
      const string&       FwUsername() const  { return m_strFwUsername;  }
      const string&       FwPassword() const  { return m_strFwPassword;  }
      USHORT               FwPort()     const  { return m_ushFwPort;      }
      const firewall_type& FwType()     const  { return m_FwType;         }
   
   private:
      string        m_strHostname;   ///< name or ip-address of the ftp-server
      USHORT         m_ushHostport;   ///< port of the ftp-server
      string        m_strUsername;   ///< username for ftp-server
      string        m_strPassword;   ///< password for ftp-server
      string        m_strAccount;    ///< account mostly needed on ftp-servers running on unix/linux
      string        m_strFwHostname; ///< name or ip-address of the firewall
      string        m_strFwUsername; ///< username for firewall
      string        m_strFwPassword; ///< password for firewall
      USHORT         m_ushFwPort;     ///< port of the firewall
      firewall_type  m_FwType;        ///< type of firewall
   };

   /// Holds a response of a ftp-server.
   class reply
   {
      string m_strResponse;

      /// Holds the reply code.
      class code
      {
         TCHAR m_szCode[4];
      public:
         code()
         {
            ZERO(m_szCode);
         }
         LPCTSTR Value() const { return m_szCode; }
         bool Set(const string& strCode)
         {
            if( strCode.length()!=3 ||
                strCode[0]<_T('1') || strCode[0]>_T('5') ||
                strCode[1]<_T('0') || strCode[1]>_T('5') )
            {
               ZERO(m_szCode);
               return false;
            }
            strcpy(m_szCode, strCode);
            return true;
         }

         bool IsPositiveReply() const { return IsPositivePreliminaryReply() || IsPositiveCompletionReply() || IsPositiveIntermediateReply(); }
         bool IsNegativeReply() const { return IsTransientNegativeCompletionReply() || IsPermanentNegativeCompletionReply(); }

         bool IsPositivePreliminaryReply() const         { return m_szCode[0] == _T('1'); }
         bool IsPositiveCompletionReply() const          { return m_szCode[0] == _T('2'); }
         bool IsPositiveIntermediateReply() const        { return m_szCode[0] == _T('3'); }
         bool IsTransientNegativeCompletionReply() const { return m_szCode[0] == _T('4'); }
         bool IsPermanentNegativeCompletionReply() const { return m_szCode[0] == _T('5'); }

         bool IsRefferingToSyntax() const                      { return m_szCode[1] == _T('0'); }
         bool IsRefferingToInformation() const                 { return m_szCode[1] == _T('1'); }
         bool IsRefferingToConnections() const                 { return m_szCode[1] == _T('2'); }
         bool IsRefferingToAuthenticationAndAccounting() const { return m_szCode[1] == _T('3'); }
         bool IsRefferingToUnspecified() const                 { return m_szCode[1] == _T('4'); }
         bool IsRefferingToFileSystem() const                  { return m_szCode[1] == _T('5'); }
      } m_Code;
   public:
      bool Set(const string& strResponse)
      {
         m_strResponse = strResponse;
         if( m_strResponse.length()>2 )
            return m_Code.Set(m_strResponse.substr(0, 3));
         return false;
      }
      const string& Value() const { return m_strResponse; }
      const code& Code() const { return m_Code; }
   };



} // namespace ftp








