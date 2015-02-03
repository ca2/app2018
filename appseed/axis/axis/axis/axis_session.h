#pragma once


namespace userpresence
{


   class userpresence;


} // namespace userpresence


namespace axis
{


   class CLASS_DECL_AXIS session:
      virtual public ::aura::session,
      virtual public ::axis::application
   {
   public:

      

      ::userpresence::userpresence *                           m_puserpresence;
      sp(::ifs)                                                m_pifs;
      sp(::fs::remote_native)                                  m_prfs;
      sp(::fs::fs)                                             m_spfs;
      sp(class ::fs::data)                                     m_spfsdata;


      //::sockets::sockets *                                     m_psockets;
      bool                                                     m_bIfs;
      


      sp(::fontopus::fontopus)                                 m_pfontopus;
      sp(class ::fontopus::licensing)                          m_splicensing;

      //::user::schema_simple_impl                               m_schemasimple;




      session(::aura::application * papp);
      virtual ~session_parent;

      inline ::userpresence::userpresence & userpresence() { return *m_puserpresence; }

      //application_ptra & appptra();

      virtual bool is_session();


      void construct(::aura::application * papp, int iPhase);

      virtual bool process_initialize();

      virtual bool initialize1();

      virtual bool initialize2();

      virtual bool initialize_instance();

      virtual bool initialize();

      virtual bool finalize();

      virtual int32_t exit_instance();


      inline sp(class ::fs::data)               fs()           { return m_spfsdata; }
      //inline ::sockets::sockets &               sockets()      { return *m_psockets; }


      inline sp(::fontopus::fontopus)              fontopus()     { return m_pfontopus; }
      inline class ::fontopus::licensing &         licensing()      { return *m_splicensing; }


      virtual ::fontopus::fontopus * create_fontopus();


      virtual ::fontopus::user * safe_get_user();


      virtual ::fontopus::user * get_user();
      virtual ::fontopus::user * create_current_user();


      virtual bool is_licensed(const char * pszId,bool bInteractive = true);


      

      virtual bool get_auth(const string & pszForm,string & strUsername,string & strPassword);


      virtual bool  get_window_minimum_size(LPSIZE lpsize);


      //virtual void  get_cursor_pos(LPPOINT lppoint);


      virtual void on_request(sp(::create) pcreate);
      

      //virtual string matter_as_string(const char * pszMatter,const char * pszMatter2);
      //virtual string dir_matter(const char * pszMatter,const char * pszMatter2);

      //virtual bool is_inside_time_dir(const char * pszPath);
      //virtual bool file_is_read_only(const char * pszPath);


      virtual void defer_initialize_user_presence();


      virtual string get_cred(::aura::application * papp,const string & strRequestUrlParam,const RECT & rect,string & strUsername,string & strPassword,string strToken,string strTitle,bool bInteractive);

   };


} // namespace axis








//inline ::axis::session & Sess(::aura::application * papp);




























