#pragma once


namespace bergedge
{

   class CLASS_DECL_ca2 pane_view : 
      public ::userex::pane_tab_view,
      public FileManagerCallbackInterface,
      public ::bergedge::form_callback
   {
   public:


      sp(form_view)                             m_pformOptions;
      visual::dib_sp                            m_dibBk;
      int32_t                                   m_iNewArea;
      int32_t                                   m_iArea;
      int32_t                                   m_iDisplay;
      filemanager::SimpleFilePropertiesForm *   m_ppropform;
      ::fs::item_array                          m_itema;



      pane_view(sp(::ca2::application) papp);
      virtual ~pane_view();

      using ::user::tab_view::on_show_view;
      using ::bergedge::form_callback::on_update;

      void rotate();
      
      void on_create_view(::user::view_creator_data * pcreatordata);
      void on_show_view();

      virtual void install_message_handling(::ca2::message::dispatch * pinterface);

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

      void _001OnDraw(::ca2::graphics * pdc);

      virtual void set_display(int32_t iDisplay);

      virtual void on_update(sp(::user::view) pSender, LPARAM lHint, ::ca2::object* pHint);
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


} // namespace windesk

