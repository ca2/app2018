#pragma once


namespace user
{


   class CLASS_DECL_CORE menu_list_window :
      virtual public ::user::menu
   {
   public:

      enum EChildId
      {
         ChildIdClose = 0x00800000
      };

      UINT                                m_uiMessage;
      bool                                m_bAutoClose;
      bool                                m_bAutoDelete;
      ::user::front_end_schema::menu *    m_pschema;
      bool                                m_bOwnItem;
      menu_button                         m_buttonClose;
      int32_t                             m_iHeaderHeight;
      int32_t                             m_iItemHeight;
      size                                m_size;

      menu_list_window();
      menu_list_window(::aura::application * papp);
      menu_list_window(::aura::application * papp, sp(menu_item) pitem);
      virtual ~menu_list_window();

      void on_layout();

      virtual void install_message_handling(::message::dispatch * pinterface);

      virtual bool get_color(COLORREF & cr,e_color ecolor);

      virtual void destroy_menu();

      virtual bool TrackPopupMenu(sp(::user::interaction) pwndParent, sp(::user::interaction) pwndNotify);

      bool MenuFill(sp(::user::interaction) pwndFill, sp(::user::interaction) pwndNotify);

      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnDestroy);
      DECL_GEN_SIGNAL(_001OnClose);
      
      void _001OnTimer(::timer * ptimer);

      void _001OnDraw(::draw2d::graphics * pgraphics);

      bool BaseOnControlEvent(::user::control_event * pevent);


      bool _TrackPopupMenu(sp(::user::interaction) pwndParent, sp(::user::interaction) pwndNotify);
      void _CreateButtons(sp(menu_item) pitem);
      void _UpdateCmdUi(sp(menu_item) pitem);
      void _CalcSize(sp(menu_item) pitem, ::draw2d::graphics * pgraphics, int32_t & iMaxWidth, int32_t & iMaxHeight);
      void _LayoutButtons(sp(menu_item) pitem, int32_t iMaxWidth, LPRECT lprect, LPCRECT lpcrectBound);

   };


} // namespace user







