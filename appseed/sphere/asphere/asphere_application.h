#pragma once



namespace asphere
{


   class CLASS_DECL_SPHERE application :
      virtual public ::core::application,
      virtual public ::file_watcher::listener
   {
   public:
      
      
      ::file_watcher::id                           m_watchid;
      bool                                         m_bContextTheme;
      colorertake5::ParserFactory *                m_pparserfactory;

      bool m_bSync;
      int64_t m_iSyncCount;
      stringa                                      m_straTheme;
      string                                       m_strTheme;
      ::file::path                                 m_pathTheme;
      string_map < COLORREF >                      m_mapBg;
      string_map < COLORREF >                      m_mapIconGlow;
      string_map < COLORREF >                      m_mapFg;
      string_map < COLORREF >                      m_mapMg;
      string_map < COLORREF >                      m_mapBack;
      string_map < COLORREF >                      m_mapText;
      COLORREF                                     m_crBg;
      COLORREF                                     m_crFg;
      COLORREF                                     m_crMg;
      COLORREF                                     m_crBack;
      COLORREF                                     m_crText;
      COLORREF                                     m_crIconGlow;
      
      string                                       m_strWeatherState;
      string                                       m_strDayNight;
      string                                       m_strDayNightTheme;


      application();
      virtual ~application();


      virtual bool init_instance() override;
      virtual bool init1() override;
      virtual bool init() override;

      
      void initialize_contextualized_theme();
      
      
      virtual string get_weather_state();
      virtual string get_day_night();
      virtual string get_day_night_theme();


      virtual void get_theme_text_color(COLORREF & crText, COLORREF & crBack);

      virtual void sync_with_stored_theme();
      virtual string get_current_weather();
      virtual string get_default_theme();
      virtual string stored_theme();
      virtual void set_context_theme(string strTheme);
      virtual void on_change_theme();
      virtual string get_theme();

      virtual ::file::path   get_theme_path();
      inline COLORREF get_theme_bg_color() { return m_crBg; }
      inline COLORREF get_theme_fg_color() { return m_crFg; }
      inline COLORREF get_theme_mg_color() { return m_crMg; }
      inline COLORREF get_theme_icon_glow_color() { return m_crIconGlow; }

      virtual void _001OnTheme(::message::message * pobj);

      virtual uint32_t guess_code_page(const string & str);

      ::colorertake5::ParserFactory                &  parser_factory();

      virtual void handle_file_action(::file_watcher::id watchid, const char * dir, const char * filename, ::file_watcher::e_action action) override;
      
      virtual void on_change_weather_state();

   };



} // namespace sphere


