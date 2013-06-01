#pragma once


namespace fontopus
{

   
   class contact_list;


   class CLASS_DECL_ca2 user :
      public ::ca::object
   {
   public:

         
      ::http::cookies *                      m_phttpcookies;

      string                                 m_strLogin;
      string                                 m_strName;
      string                                 m_strPathPrefix;
      string                                 m_strPath;
      string                                 m_strDataPath;
      string                                 m_strAppDataPath;
      string_to_string_map                   m_sessionidmap;
      string                                 m_strFunUserId;


      string                                 m_strFontopusServerSessId;
      string                                 m_strRequestingServer;

      sp(contact_list)                         m_pcontactlist;

      sp(::userpresence::presence)             m_ppresence;

      sp(::ifs)                                  m_pifs;


      user(sp(::ca::application) papp);
      virtual ~user();

      virtual void start_veiev_synch();
         

      DECL_GEN_SIGNAL(veiev_synch);
      DECL_GEN_VSIGNAL(on_end_veiev_synch);


      // if interactive flag is true, the function may interact with the user to authenticate her/him,
      // otherwise it fails to get sessid if it hasn't been previously cached during System instance lifetime.
      string get_sessid(const char * pszRequestingParty = NULL, bool bInteractive = true);
      void   set_sessid(const char * pszSessid, const char * pszRequestingParty);

      string get_ca2_server(const char * pszPrefix);


      virtual bool initialize();


      virtual bool create_ifs();


   };

} // namespace fontopus


#define AppUser(pcaapp) (*planeApp(pcaapp).m_pfontopus->get_user())
#define ApplicationUser (AppUser(m_papp))

