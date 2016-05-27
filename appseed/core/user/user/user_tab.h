#pragma once


namespace user
{


   class tab_callback;

   class CLASS_DECL_CORE tab_pane:
      virtual public ::object
   {
   public:


      ::draw2d::brush_sp                  m_brushFill;
      ::draw2d::brush_sp                  m_brushFillSel;
      ::draw2d::brush_sp                  m_brushFillHover;
      id                                  m_id;
      istring                             m_istrTitleEx;
      ::visual::dib_sp                    m_dib;
      sp(place_holder)                    m_pholder;
      bool                                m_bVisible;
      bool                                m_bPermanent;
      size                                m_size;
      stringa                             m_straTitle;
      array<size>                         m_sizeaText;
      point                               m_pt;


      tab_pane(::aura::application * papp);
      tab_pane(const tab_pane & tab_pane);
      virtual ~tab_pane();


      tab_pane & operator = (const tab_pane & tab_pane);


      string get_title();

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

      enum e_message
      {
         message_start_tab_drag = WM_USER + 223 + 1984 + 77 + 51,
      };




      

      class CLASS_DECL_CORE data :
         virtual public ::data::data
      {
      public:

         size                                m_sizeSep;
         int32_t                             m_iHeightAddUp;
         ::draw2d::pen_sp                    m_pen;
         ::draw2d::font_sp                   m_font;
         ::draw2d::font_sp                   m_fontUnderline;
         ::draw2d::font_sp                   m_fontBigBold;
         ::draw2d::font_sp                   m_fontBold;
         ::draw2d::brush_sp                  m_brushTextHover;
         ::draw2d::brush_sp                  m_brushTextSel;
         ::draw2d::brush_sp                  m_brushText;
         ::draw2d::brush_sp                  m_brushCloseHover;
         ::draw2d::brush_sp                  m_brushCloseSel;
         ::draw2d::brush_sp                  m_brushClose;
         ::draw2d::pen_sp                    m_penBorder;
         ::draw2d::pen_sp                    m_penBorderSel;
         ::draw2d::pen_sp                    m_penBorderHover;
         bool                                m_bCreated;
         int32_t                              m_iTabHeight;
         int32_t                              m_iTabWidth;
         tab_pane_array                       m_panea;
         rect                             m_rectTab;
         rect                             m_rectTabClient;
         visual::graphics_extension       m_dcextension;
         index_array                      m_iaSel;
         rect                             m_rectBorder;
         rect                             m_rectMargin;
         rect                             m_rectTextMargin;
         ::index                          m_iHover;
         image_list                       m_imagelist;
         tab_callback *                   m_pcallback;
         ::index                          m_iDragTab;
         bool                             m_bDrag;
         bool                             m_bVertical;
         ::core::match::any                m_matchanyRestore;
         bool                             m_bEnableCloseAll;


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


      sp(data)                         m_spdata;
      ::visual::graphics_extension     m_dcextension;
      bool                             m_bRestoringTabs;
      bool                             m_bShowTabs;
      bool                             m_bHidingTabs;
      DWORD                            m_dwHidingTabs;
      bool                             m_bNoTabs;
      e_element                        m_eelement;
      sp(::create)         m_spcreatecontext;


      tab(::aura::application * papp);
            virtual ~tab();



      data * get_data();


      virtual void on_show_view();

      virtual void  _001SetVertical(bool bSet = true);
      virtual void _001SelectTab(::index iTab);
      virtual void _001CloseTab(::index iTab);
      virtual tab_pane * get_pane(::index iTab, bool bVisible = true);
      virtual ::user::interaction * get_tab_window(::index iTab, bool bVisible = true);
      virtual ::user::place_holder * get_tab_holder(::index iTab, bool bVisible = true);

      void _000OnMouse(::message::mouse * pmouse);

      virtual ::user::interaction * get_view_uie();

      virtual void _001SetTabCallback(tab_callback * pcallback);
      virtual bool _001IsAddTab(::index iTab);
      virtual void _001OnDropTab(::index iTab, e_position eposition);
      virtual bool set_cur_tab_by_id(id id, sp(::create) pcreatecontext = NULL);

      virtual id get_cur_tab_id();
      virtual id get_current_id();

      virtual void _001AddSel(::index iSel);
      virtual ::index _001GetSel();
      virtual void _001SetSel(::index iSel);

      virtual ::count _001GetPaneCount();

      virtual void _001OnTabClick(::index iTab);
      virtual void _001OnTabClose(::index iTab);
      virtual ::window_sp GetNotifyWnd();
      virtual index hit_test(point pt, e_element & eelement);
      virtual e_position DragHitTest(point pt);
      virtual void GetDragRect(LPRECT lprect, e_position eposition);
      virtual int_ptr GetTabCount();
      virtual bool get_element_rect(::index iTab, LPRECT lprect, e_element eelement);

      virtual void get_title(int iPane,stringa & stra);

      virtual void layout();
      virtual void layout_pane(index iPane);

      virtual void GetTabClientRect(LPRECT lprect);
      virtual void GetTabClientRect(RECT64 * lprect);

      virtual bool add_tab(const char * lpcsz, id idTab = id(), bool bVisible = true, bool bPermanent = false);
      virtual bool set_tab(const char * lpcsz, id idTab = id(), bool bVisible = true);
      virtual bool add_image_tab(const char * lpcsz, const char * pszImage, id idTab = id(), bool bVisible = true, bool bPermanent = false);
      virtual bool set_image_tab(const char * lpcsz, const char * pszImage, id idTab = id(), bool bVisible = true);
      virtual bool remove_tab_by_id(id idTab = id());
      virtual void remove_tab(::index iTab, bool bVisible = true);
      virtual bool show_tab_by_id(id idTab = id(), bool bShow = true);
      virtual bool show_tab(::index iTab, bool bShow = true);


      virtual bool set_title(::index iTab, const char * psz);
      virtual bool SetTitleById(id id, const char * psz);

      virtual ::index get_tab_by_id(id id);
      virtual id get_id_by_tab(::index iTab, bool bVisible = true);
      virtual tab_pane * get_pane_by_id(id id);
      virtual tab_pane * ensure_pane_by_id(id id);
      virtual void ensure_tab_by_id(id id);

      virtual void on_change_pane_count();

      virtual void get_text_id(stringa & stra);

      virtual void get_prefixed_ci_id(stringa & stra, const char * pszPrefix);
      virtual void get_suffixed_ci_id(stringa & stra, const char * pszSuffix);
      virtual void get_presuffixed_ci_id(stringa & stra, const char * pszPrefix, const char * pszSuffixed);

      virtual void get_begins_ci_eat_id(stringa & stra, const char * pszPrefix);
      virtual void get_ends_ci_eat_id(stringa & stra, const char * pszSuffix);
      virtual void get_begins_ends_ci_eat_id(stringa & stra, const char * pszPrefix, const char * pszSuffixed);

      virtual void install_message_handling(::message::dispatch * pinterface);
      virtual void _001ConnectParent(::message::dispatch * pinterface);


      virtual bool has_restore_tab();
      virtual void get_restore_tab(var_array & vara);
      virtual void open_tabs(const var_array & vara);



      void _001OnDraw(::draw2d::graphics * pgraphics);


      void _001OnDrawStandard(::draw2d::graphics * pgraphics);
      void _001OnDrawSchema01(::draw2d::graphics * pgraphics);



      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);
      DECL_GEN_SIGNAL(_001OnMouseMove);
      DECL_GEN_SIGNAL(_001OnMouseLeave);
      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnStartTabDrag);
      
      DECL_GEN_SIGNAL(_001OnAppLanguage);
      DECL_GEN_SIGNAL(_001OnSize);
      void _001OnTimer(::timer * ptimer);


      virtual bool defer_handle_full_screen_show_tabs(bool bLayout = true);


      virtual void restore_tabs();


      virtual void on_create_tabs();


   };


} // namespace user


