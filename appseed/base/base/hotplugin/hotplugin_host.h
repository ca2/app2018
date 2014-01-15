#pragma once


namespace hotplugin
{


   class CLASS_DECL_BASE host :
      virtual public plugin
   {
   public:


      plugin *                      m_pplugin;
      primitive::memory             m_memory;
      double                        m_dProgressRate;
      bool                          m_bShowProgress;
      string                        m_strHostPluginLocation;


      bool                          m_bCa2InstallationReady;
      // this flag is only set if spaadmin is installing ca files while npca2 is *started*
      // it isn't set when npca2 is already running and spaadmin starts, may be only in the case above, when npca2 *restarts*.
      // this enables spaadmin to install ca files to ca folder, because npca2 would not use any ca shared libraries.
      bool                          m_bRunningSpaAdmin;


      mutex                         m_mutexSystem;

      ::draw2d::dib_sp              m_dib;


      host();
      virtual ~host();


      virtual void * get_system();
      //virtual void set_system(void * pvoidSystem);


      virtual void   redraw();

      // host should implement
      virtual bool         open_url(const char * psz);
      virtual bool         reload_plugin();

      // Host location is not the updated url - if there is a way to update the url and 
      // maintain the same plugin instance, what would lead to an out-of-date location url.
      // It is the location url of the page that hosts the plugin when the plugin was created.
      virtual string     get_host_location_url();

      virtual void         post_message(UINT uiMessage, WPARAM wparam, LPARAM lparam);
      virtual oswindow         get_host_window();
      virtual void         get_window_rect(LPRECT lprect);

      // client should implement
      virtual void   set_window_rect(LPCRECT lpcrect);

      // implemented here
      virtual void   set_memory(void * puchMemory, ::count c);
      virtual void   append_memory(void * puchMemory, ::count c);
      virtual ::count get_memory_length();
      virtual ::count read_memory(void * puchMemory, ::count c);
      virtual void   free_memory();
      //static  void   free_memory(byte ** ppuchMemory);


      virtual void set_ready();
      virtual void on_ready();


      virtual bool initialize();
      virtual bool finalize();


      virtual void start_plugin();

      virtual int32_t  start_ca2_system();

      virtual void on_paint(::draw2d::graphics * pgraphics, LPCRECT lprect);

      virtual int32_t starter_start(const char * pszCommandLine);

      static int32_t starter_start(const char * pszCommandLine, base_application * papp, plugin * pplugin = NULL);

      static int32_t starter_start_sync(const char * pszCommandLine, base_application * papp, plugin * pplugin = NULL);

      virtual void deferred_prodevian_redraw();


#ifdef WINDOWS
      virtual LRESULT message_handler(UINT uiMessage, WPARAM wparam, LPARAM lparam);
#else
      virtual int32_t message_handler(XEvent * pevent);
#endif
      virtual void start_ca2();

      virtual void set_progress_rate(double dRate);

      virtual double get_progress_rate();

      virtual void set_ca2_installation_ready(bool bReady = true);

      virtual bool is_ca2_installation_ready();

      virtual void set_status(const char * pszStatus);

      virtual void set_bitmap(::draw2d::graphics * pgraphics, LPCRECT lprect);

      virtual void paint_bitmap(::draw2d::graphics * pgraphics, LPCRECT lprect);

      virtual void blend_bitmap(::draw2d::graphics * pgraphics, LPCRECT lprect);


      virtual void translate_mouse_message(int * px, int * py);

      virtual bool show_window(bool bShow = true);
      virtual bool destroy_window();


   };


} // namespace hotplugin

