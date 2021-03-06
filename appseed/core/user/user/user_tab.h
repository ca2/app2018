#pragma once


namespace user
{


   class tab_callback;


   class CLASS_DECL_CORE tab_pane:
      virtual public ::object
   {
   protected:


      istring                             m_istrTitleEx3;


   public:


      tab *                               m_ptab;
      ::draw2d::brush_sp                  m_brushFill;
      ::draw2d::brush_sp                  m_brushFillSel;
      ::draw2d::brush_sp                  m_brushFillHover;
      id                                  m_id;
      ::visual::dib_sp                    m_dib;
      sp(place_holder)                    m_pholder;
      bool                                m_bTabPaneVisible;
      bool                                m_bPermanent;
      size                                m_size;
      stringa                             m_straTitle;
      array<size>                         m_sizeaText;
      point                               m_pt;


      tab_pane(tab * ptab);
      tab_pane(const tab_pane & tab_pane);
      virtual ~tab_pane();


      tab_pane & operator = (const tab_pane & tab_pane);


      string get_title();
      void set_title(const char * psz);


      virtual void do_split_layout(::visual::graphics_extension & dc,::draw2d::graphics * pgraphics);


   };


   class CLASS_DECL_CORE tab_pane_array:
      public spa(tab_pane)
   {
   public:


      tab_pane_array(::aura::application * papp);
      virtual ~tab_pane_array();


      virtual tab_pane * get_by_id(id id);
      ::count get_visible_count();


   };

   class CLASS_DECL_CORE tab :
      virtual public control,
      virtual public place_holder_container
   {
   public:

      enum e_state
      {

         state_initial,
         state_close_button_down,
         state_other_tab_button_down,
         state_drag_commanded,
         state_drag_started

      };

      enum e_message
      {

         message_start_tab_drag = WM_USER + 223 + 1984 + 77 + 51,

      };

      enum e_timer
      {

         timer_drag_start = WM_USER + 1000,

      };




      class CLASS_DECL_CORE data :
         virtual public ::data::data
      {
      public:

         size                             m_sizeSep;
         int32_t                          m_iHeightAddUp;
         ::draw2d::pen_sp                 m_pen;
         ::draw2d::brush_sp               m_brushTextHover;
         ::draw2d::brush_sp               m_brushTextSel;
         ::draw2d::brush_sp               m_brushText;
         ::draw2d::brush_sp               m_brushCloseHover;
         ::draw2d::brush_sp               m_brushCloseSel;
         ::draw2d::brush_sp               m_brushClose;
         ::draw2d::pen_sp                 m_penBorder;
         ::draw2d::pen_sp                 m_penBorderSel;
         ::draw2d::pen_sp                 m_penBorderHover;
         bool                             m_bCreated;
         int32_t                          m_iTabHeight;
         int32_t                          m_iTabWidth;
         tab_pane_array                   m_panea;
         rect                             m_rectTab;
         rect                             m_rectTabClient;
         visual::graphics_extension       m_dcextension;
         id_array                         m_idaSel;
         rect                             m_rectBorder;
         rect                             m_rectMargin;
         rect                             m_rectTextMargin;
         ::index                          m_iHover;
         image_list                       m_imagelist;
         tab_callback *                   m_pcallback;
         ::index                          m_iClickTab;
         bool                             m_bDrag;
         bool                             m_bVertical;
         ::core::match::any               m_matchanyRestore;
         bool                             m_bEnableCloseAll;
         bool                             m_bNoClient;
         size                             m_sizeTabTotal;


         data(::aura::application * papp);
         virtual ~data();

         virtual tab_pane * get_pane_by_id(id id);

         ::count get_visible_tab_count();

      };


      class CLASS_DECL_CORE remove_tab_exception
      {
      public:

         string m_strHint;

         remove_tab_exception(const char * pszHint)
         {
            m_strHint = pszHint;
         }

      };

      e_state                          m_estate;
      sp(data)                         m_spdata;
      ::visual::graphics_extension     m_dcextension;
      bool                             m_bDisableSavingRestorableTabs;
      bool                             m_bShowTabs;
      bool                             m_bHidingTabs;
      DWORD                            m_dwHidingTabs;
      bool                             m_bNoTabs;
      e_element                        m_eelementClick;
      sp(::create)                     m_spcreate;
      ::count                          m_iRestoredTabCount;
      bool                             m_bDrawTabAtBackground;


      tab();
      virtual ~tab();


      virtual void install_message_routing(::message::sender * pinterface) override;


      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);
      DECL_GEN_SIGNAL(_001OnMouseMove);
      DECL_GEN_SIGNAL(_001OnMouseLeave);
      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnShowWindow);
      DECL_GEN_SIGNAL(_001OnSize);
      DECL_GEN_SIGNAL(_001OnStartTabDrag);
      DECL_GEN_SIGNAL(_001OnAppLanguage);


      data * get_data();


      virtual void on_show_view();
      virtual void on_stage_view();

      virtual void  _001SetVertical(bool bSet = true);
      virtual void _001SelectTab(::index iTab);
      virtual void _001CloseTab(::index iTab);


      virtual ::user::interaction * tab_window(::index iTab);
      virtual ::user::place_holder * tab_holder(::index iTab);

      virtual ::user::interaction * pane_window(::index iTab);
      virtual ::user::place_holder * pane_holder(::index iTab);

      virtual void _000OnMouse(::message::mouse * pmouse) override;

      virtual ::user::interaction * get_view_uie();

      virtual void _001SetTabCallback(tab_callback * pcallback);
      virtual bool _001IsAddTab(::index iTab);
      virtual void _001OnDropTab(::index iTab, e_position eposition);
      virtual bool set_cur_tab_by_id(id id, ::create * pcreate = NULL);

      virtual id get_cur_tab_id();
      virtual id get_current_id();

      virtual void _001OnRemoveTab(tab_pane * ptab);

      virtual ::index _001GetSel();
      virtual void _001SetSel(::index iSel);

      virtual ::count _001GetPaneCount();

      virtual void _001OnTabClick(::index iTab);
      virtual void _001OnTabClose(::index iTab);
      virtual ::window_sp GetNotifyWnd();
      virtual index hit_test(point pt, e_element & eelement) override;
      virtual e_position DragHitTest(point pt);
      virtual void GetDragRect(LPRECT lprect, e_position eposition);
      virtual bool get_element_rect(::index iTab, LPRECT lprect, e_element eelement);

      virtual void get_title(int iPane,stringa & stra);

      virtual void on_layout() override;
      virtual void layout_pane(index iPane, bool bDisplay = false);

      virtual void GetTabClientRect(LPRECT lprect);
      virtual void GetTabClientRect(RECT64 * lprect);

      virtual bool add_tab(const char * lpcsz, id idTab = id(), bool bVisible = true, bool bPermanent = false, ::user::place_holder * pholder = NULL);
      virtual bool set_tab(const char * lpcsz, id idTab = id(), bool bVisible = true);
      virtual bool add_image_tab(const char * lpcsz, const char * pszImage, id idTab = id(), bool bVisible = true, bool bPermanent = false);
      virtual bool set_image_tab(const char * lpcsz, const char * pszImage, id idTab = id(), bool bVisible = true);
      virtual bool remove_tab_by_id(id idTab = id());
      virtual void remove_tab(::index iTab, bool bVisible = true);
      virtual void remove_all_tabs();
      virtual bool show_tab_by_id(id idTab = id(), bool bShow = true);
      virtual bool show_pane(::index iPane, bool bShow = true);
      virtual bool hide_tab(::index iTab);


      virtual bool set_title(::index iTab, const char * psz);
      virtual bool SetTitleById(id id, const char * psz);

      virtual ::count get_tab_count();
      virtual ::count get_pane_count();

      virtual index find_child_pane(::user::interaction * pui);
      virtual void defer_remove_child_pane(::user::interaction * pui);

      virtual void on_remove_child(::user::interaction * pui) override;
      virtual void on_remove_place_holder_child(::user::interaction * pui) override;

      virtual void on_hide_child(::user::interaction * pui) override;
      virtual void on_hide_place_holder_child(::user::interaction * pui) override;

      virtual ::index id_tab(id id);
      virtual id tab_id(::index iTab);
      virtual ::index id_pane(id id);
      virtual id pane_id(::index iPane);

      virtual ::index tab_pane(index iTab);
      virtual ::index pane_tab(index iPane);


      virtual ::user::tab_pane * get_pane(::index iPane);
      virtual ::user::tab_pane * get_tab(::index iTab);


      virtual ::user::tab_pane * get_pane_by_id(id id);
      virtual ::user::tab_pane * create_pane_by_id(id id);
      virtual ::index create_tab_by_id(id id);

      virtual void on_change_pane_count(::array < ::user::tab_pane * > array = ::array < ::user::tab_pane * >());

      virtual void get_text_id(stringa & stra);

      virtual void get_prefixed_ci_id(stringa & stra, const char * pszPrefix);
      virtual void get_suffixed_ci_id(stringa & stra, const char * pszSuffix);
      virtual void get_presuffixed_ci_id(stringa & stra, const char * pszPrefix, const char * pszSuffixed);

      virtual void get_begins_ci_eat_id(stringa & stra, const char * pszPrefix);
      virtual void get_ends_ci_eat_id(stringa & stra, const char * pszSuffix);
      virtual void get_begins_ends_ci_eat_id(stringa & stra, const char * pszPrefix, const char * pszSuffixed);

      virtual void _001ConnectParent(::message::sender * pinterface);


      virtual bool has_restore_tab();
      virtual void get_restore_tab(var_array & vara);
      virtual bool matches_restorable_tab(const var & varId, ::user::place_holder * pholder = NULL);
      virtual ::count open_tabs(const var_array & vara);
      virtual ::count restore_tabs();
      virtual void save_restorable_tabs();
      virtual void on_create_tabs();


      virtual void _001OnDraw(::draw2d::graphics * pgraphics) override;


      virtual void _001OnDrawStandard(::draw2d::graphics * pgraphics);
      virtual void _001OnDrawSchema01(::draw2d::graphics * pgraphics);

      virtual void _001OnTimer(::timer * ptimer) override;

      virtual bool defer_handle_auto_hide_tabs(bool bLayout = true);


   };


} // namespace user


