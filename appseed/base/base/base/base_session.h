#pragma once


namespace base
{


   class CLASS_DECL_BASE session:
      virtual public ::axis::session,
      virtual public ::base::application
   {
   public:

      

      sp(::userpresence::userpresence)                         m_puserpresence;
      sp(::ifs)                                                m_pifs;
      sp(::fs::remote_native)                                  m_prfs;
      sp(::fs::fs)                                             m_spfs;
      sp(class ::fs::data)                                     m_spfsdata;


      sp(::fontopus::fontopus)                                 m_pfontopus;
      ::sockets::sockets *                                     m_psockets;
      bool                                                     m_bIfs;
      sp(class ::fontopus::licensing)                   m_splicensing;




      //::user::schema_simple_impl                               m_schemasimple;




      session(::aura::application * papp);
      virtual ~session_parent;

      inline ::userpresence::userpresence & userpresence() { return *m_puserpresence; }
      inline sp(::fontopus::fontopus)           fontopus()     { return m_pfontopus; }

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
      inline ::sockets::sockets &               sockets()      { return *m_psockets; }
      inline class ::fontopus::licensing &        licensing()      { return *m_splicensing; }

      //::user::str_context *                     str_context();








      
      virtual ::fontopus::fontopus * create_fontopus();


      virtual ::fontopus::user * safe_get_user();


      virtual ::fontopus::user * get_user();
      virtual ::fontopus::user * create_current_user();


      virtual bool is_licensed(const char * pszId,bool bInteractive = true);


      virtual bool get_auth(const string & pszForm,string & strUsername,string & strPassword);


      virtual bool  get_window_minimum_size(LPSIZE lpsize);


      //virtual void  get_cursor_pos(LPPOINT lppoint);


      virtual bool on_ui_mouse_message(::message::mouse * pmouse);


      virtual ::user::interaction * get_active_guie();
      virtual ::user::interaction * get_focus_guie();


      //virtual string matter_as_string(const char * pszMatter,const char * pszMatter2);
      //virtual string dir_matter(const char * pszMatter,const char * pszMatter2);

      //virtual bool is_inside_time_dir(const char * pszPath);
      //virtual bool file_is_read_only(const char * pszPath);

   };


} // namespace base








//inline ::base::session & Sess(::aura::application * papp);




























