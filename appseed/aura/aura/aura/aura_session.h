#pragma once



#define session_parent session( )


namespace aura
{


   class CLASS_DECL_AURA session:
      virtual public ::aura::application,
      virtual public ::aura::session_interface
   {
   public:

      


      bool                                                     m_bMatterFromHttpCache;



      string_map < sp(::aura::application) >                   m_mapApplication;



      application_ptra                                         m_appptra;
      sp(::aura::savings)                                      m_psavings;


 





      session(::aura::application * papp);
      virtual ~session_parent;


      application_ptra & appptra();

      virtual bool is_session();


      void construct(::aura::application * papp, int iPhase);

      virtual bool process_initialize();

      virtual bool initialize1();

      virtual bool initialize2();

      virtual bool initialize_instance();

      virtual bool initialize();

      virtual bool finalize();

      virtual int32_t exit_instance();


      inline ::aura::savings &                  savings()      { return *m_psavings; }


      virtual sp(::aura::application) start_application(const char * pszType,const char * pszAppId,sp(::create) pcreatecontext);

      
      virtual COLORREF get_default_color(uint64_t ui);


      //virtual index initial_frame_position(LPRECT lprect,const RECT & rect, bool bMove);

      virtual bool  get_window_minimum_size(LPSIZE lpsize);


      virtual void frame_pre_translate_message(signal_details * pobj);


      //virtual ::aura::interaction * get_active_guie();
      //virtual ::aura::interaction * get_focus_guie();


      virtual bool is_licensed(const char * pszId,bool bInteractive = true);


   };


} // namespace aura





































