#pragma once


namespace hotplugin
{


   class CLASS_DECL_BASE composer
   {
   public:


      enum e_status
      {

         status_start_system,
         status_start_host,
         status_init_host,
         status_start_window,
         status_ok,

      };


      bool                          m_bFocus;
      bool                          m_bActive;
      bool                          m_bSendActivationState;
      bool                          m_bActivationStateSent;


      e_status                               m_estatus;
      bool                                   m_bSystemOk;
      bool                                   m_bInit;
      bool                                   m_bHostOk;
      bool                                   m_bTryInitHost;
      bool                                   m_bHostInit;
      bool                                   m_bTryInitWindow;
      bool                                   m_bWindowOk;
      bool                                   m_bOk;
      rect                                   m_rect;
      bool                                   m_bResponsive;
      string                                 m_strEntryHallText;


      ::hotplugin::host *                    m_pbasehost;


      bool                                   m_bOpenUrl;
      string                                 m_strOpenUrl;
      string                                 m_strOpenTarget;

      string                                 m_strPluginUrl;
      string                                 m_strPluginHeaders;


      string                                 m_strHostPluginLocation;

      bool                                   m_bWrite;


      
      composer();
      virtual ~composer();


      virtual ::hotplugin::host * create_host(sp(::base::system) psystem) = 0;

      virtual void hotplugin_composer_on_timer();

      virtual bool on_composer_init_window();

      virtual void deferred_prodevian_redraw();

      // if composer on paint returns (returns true), it has painted something meaningful : no other painting is needed or even desired (finally when system, and host are ok, 
      // if host returns in a fashion-timed way the response for bitmap, it draw this bitmap, and not the default waiting [hall] screen painted by this composer).

#ifdef WINDOWS

      virtual bool windows_on_paint(HDC hdc);

#endif


      virtual bool      open_link(const string & strLink,const string & pszTarget);

      virtual bool      _open_link(const string & strLink,const string & pszTarget);

      virtual bool is_active();
   
   };


} // namespace hotplugin
