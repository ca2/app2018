#pragma once


namespace userstack
{


   class CLASS_DECL_sphere pane_view : 
      public ::userex::pane_tab_view,
      public FileManagerCallbackInterface,
      public ::userstack::form_callback
   {
   public:


      form_view *                               m_pformOptions;
      visual::dib_sp                            m_dibBk;
      int32_t                                       m_iNewArea;
      int32_t                                       m_iArea;
      int32_t                                       m_iDisplay;
      filemanager::SimpleFilePropertiesForm *   m_ppropform;
      ::fs::item_array                      m_itema;



      pane_view(::ca::application * papp);
      virtual ~pane_view();

      using ::userbase::tab_view::on_show_view;
      using ::userstack::form_callback::on_update;

      void rotate();
      
      void on_create_view(::user::view_creator_data * pcreatordata);
      void on_show_view();

      virtual void install_message_handling(::gen::message::dispatch * pinterface);

      void check_3click_dir(const char * psz);
      void check_menu_dir(const char * psz);
      void check_desktop_dir(const char * psz);

      void _001InitializeFormPreData(::user::form * pform);
      bool BaseOnControlEvent(::user::form * pview, ::user::control_event * pevent);
      
   /*   virtual void OnFileManagerOpenContextMenuFolder(
         ::filemanager::data * pdata, 
         ::fs::item & item);*/
      virtual void OnFileManagerOpenContextMenuFile(::filemanager::data * pdata, const ::fs::item_array & itema);
      virtual void OnFileManagerOpenContextMenu(::filemanager::data * pdata);

      void _001OnDraw(::ca::graphics * pdc);

      virtual void set_display(int32_t iDisplay);

      virtual void on_update(::view * pSender, LPARAM lHint, ::gen::object* pHint);
      virtual bool pre_create_window(CREATESTRUCT& cs);

   #ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
   #endif

      DECL_GEN_SIGNAL(_001OnMenuMessage)
      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnRButtonUp)
      DECL_GEN_SIGNAL(_001OnProperties)

   };


} // namespace userstack

