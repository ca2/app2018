#pragma once




namespace base
{


   class CLASS_DECL_BASE session:
      virtual public ::aura::session,
      virtual public :: base ::application,
      virtual public ::user::schema
   {
   public:



      ::user::interaction *                                    m_puiMouseMoveCapture;
      ::user::interaction *                                    m_puiLastLButtonDown;

      bool                                                     m_bMatterFromHttpCache;

      bool                                                     m_bSystemSynchronizedCursor;
      point                                                    m_ptCursor;
      bool                                                     m_bSystemSynchronizedScreen;
      rect_array                                               m_rectaMonitor;
      rect_array                                               m_rectaWkspace;
      index                                                    m_iMainMonitor;
      index                                                    m_iMainWkspace;
      sp(::fontopus::fontopus)                                 m_pfontopus;
      sp(class ::fontopus::licensing)                          m_splicensing;




      bool                                                     m_bDrawCursor;


      ::visual::e_cursor                                       m_ecursor;
      ::visual::e_cursor                                       m_ecursorDefault;
      ::base::copydesk *                                       m_pcopydesk;


      ::user::interaction *                                   m_puiFocus;
      ::aura::str_context *                                   m_puserstrcontext;
      map < ::user::e_key,::user::e_key,bool,bool > *         m_pmapKeyPressed;
      ::user::user *                                          m_puser;




      ::user::schema_simple_impl *                            m_pschemasimple;




      session(::aura::application * papp);
      virtual ~session_parent;


      virtual bool is_session();


      void construct(::aura::application * papp, int iPhase);

      virtual bool process_initialize();

      virtual bool initialize1();

      virtual bool initialize2();

      virtual bool initialize_instance();

      virtual bool initialize();

      virtual bool finalize();

      virtual int32_t exit_instance();


      ::base::copydesk &                           copydesk();
      inline ::user::user *                        user()         { return m_puser; }

      ::aura::str_context *                        str_context();
      inline sp(::fontopus::fontopus)              fontopus()     { return m_pfontopus; }
      inline class ::fontopus::licensing &         licensing()      { return *m_splicensing; }


      virtual ::fontopus::fontopus * create_fontopus();


      virtual ::fontopus::user * safe_get_user();


      virtual ::fontopus::user * get_user();
      virtual ::fontopus::user * create_current_user();


      virtual bool is_licensed(const char * pszId,bool bInteractive = true);


      virtual bool get_auth(const string & pszForm,string & strUsername,string & strPassword);


      virtual bool on_create_frame_window();

      virtual void set_cursor(::visual::e_cursor ecursor);
      virtual void set_default_cursor(::visual::e_cursor ecursor);


      virtual COLORREF get_default_color(uint64_t ui);

      virtual bool is_key_pressed(::user::e_key ekey);

      virtual void set_key_pressed(::user::e_key ekey,bool bPressed);


      virtual ::visual::cursor * get_cursor();
      virtual ::visual::cursor * get_default_cursor();



      virtual index get_main_monitor(LPRECT lprect = NULL);
      virtual bool set_main_monitor(index iMonitor);
      virtual ::count get_monitor_count();
      virtual bool  get_monitor_rect(index iMonitor,LPRECT lprect);

      virtual index get_ui_wkspace(::user::interaction * pui);
      virtual index get_main_wkspace(LPRECT lprect = NULL);
      virtual bool set_main_wkspace(index iWkspace);
      virtual ::count get_wkspace_count();
      virtual bool  get_wkspace_rect(index iWkspace,LPRECT lprect);

      virtual bool wkspace_to_monitor(LPRECT lprect,index iMonitor,index iWkspace);
      virtual bool monitor_to_wkspace(LPRECT lprect,index iWkspace,index iMonitor);

      virtual bool wkspace_to_monitor(LPRECT lprect);
      virtual bool monitor_to_wkspace(LPRECT lprect);

      virtual ::count get_desk_monitor_count();
      virtual bool  get_desk_monitor_rect(index iMonitor,LPRECT lprect);

      virtual index initial_frame_position(LPRECT lprect,const RECT & rect, bool bMove, ::user::interaction * pui);

      virtual void  get_monitor(rect_array & rectaMonitor, rect_array & rectaIntersect, const RECT & rect);

      virtual index get_zoneing(LPRECT lprect, const RECT & rect,::user::EAppearance eappearance);
      virtual index get_best_zoneing(::user::EAppearance * peappearance, LPRECT lprect,const RECT & rect);
      virtual index get_best_monitor(LPRECT lprect,const RECT & rect);
      virtual index get_best_wkspace(LPRECT lprect,const RECT & rect);
      virtual index get_good_restore(LPRECT lprect,const RECT & rect, ::user::interaction * pui);
      virtual index get_good_iconify(LPRECT lprect,const RECT & rect);

      virtual index get_good_move(LPRECT lprect,const RECT & rect, ::user::interaction * pui);

      virtual bool  get_window_minimum_size(LPSIZE lpsize);


      virtual void  get_cursor_pos(LPPOINT lppoint);







      virtual bool on_ui_mouse_message(::message::mouse * pmouse);
      virtual void frame_pre_translate_message(signal_details * pobj);


      virtual ::user::interaction * get_active_guie();
      virtual ::user::interaction * get_focus_guie();



      virtual bool is_licensed(const char * pszId,bool bInteractive = true);



   };


} // namespace base





































