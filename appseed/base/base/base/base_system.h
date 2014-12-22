#pragma once



namespace base
{


   class CLASS_DECL_BASE system:
      virtual public ::axis::system,
      virtual public ::base::application
   {
   public:


      //::url::departament                           m_urldepartament;


      ::file::system_sp                            m_spfile;
      ::file::dir::system_sp                       m_spdir;
      ::http::system                               m_httpsystem;
      

      class ::fontopus::user_set                   m_userset;
      ::crypto::crypto_sp                          m_spcrypto;
      sp(::install::install)                       m_spinstall;



      ::visual::visual                             m_visual;



      system(::aura::application * papp);
      virtual ~system();






      virtual void construct(const char * pszAppId);

      virtual bool initialize_instance();

      virtual int32_t exit_instance();
      virtual bool finalize();

      //::url::departament                           & url()     { return m_urldepartament; }

      inline class ::http::system                  & http()    { return m_httpsystem; }
      inline ::file::system                        & file()    { return *m_spfile; }
      inline ::file::dir::system                   & dir()     { return *m_spdir; }
      ::fontopus::user_set                         & userset();

      class ::crypto::crypto                       & crypto();
      ::install::install                           & install() { return *m_spinstall; }

      inline class ::visual::visual                & visual()     { return m_visual       ; } 




      virtual bool process_initialize();

      virtual bool initialize2();


      virtual bool verb();



      virtual string dir_appmatter_locator(::aura::application * papp);

      virtual ::user::interaction * get_active_guie();
      virtual ::user::interaction * get_focus_guie();


      virtual bool is_system();


      virtual string crypto_md5_text(const string & str);

      virtual void install_progress_add_up(int iAddUp = 1);


   };


} // namespace base







void CLASS_DECL_BASE __start_system(::base::system * psystem);




