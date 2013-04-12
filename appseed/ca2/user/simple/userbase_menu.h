#pragma once

namespace userbase
{

   class front_end;
   class menu_item;

   class CLASS_DECL_ca2 menu : 
      public ::user::interaction,
      public menu_base
   {
   public:


      enum e_message
      {
         MessageDestroyWindow = WM_USER + 128
      };

      enum EChildId
      {
         ChildIdClose = 0x00800000
      };


      menu(sp(::ca::application) papp);
      menu(sp(::ca::application) papp, menu_item * pitem);
      virtual ~menu();


      menu_item * GetSubMenu(int32_t i);

      void layout();

      virtual bool TrackPopupMenu(int32_t iFlags, int32_t x, int32_t y, sp(::user::interaction) oswindowParent);

      DECL_GEN_SIGNAL(_001OnLButtonDown)

      void _001OnDraw(::ca::graphics *pdc);
      DECL_GEN_SIGNAL(OnMessageDestroyWindow)


      virtual bool BaseOnControlEvent(::user::control_event * pevent);

      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnDestroy)
      DECL_GEN_SIGNAL(_001OnTimer)
      DECL_GEN_SIGNAL(_001OnIdleUpdateCmdUI)
      DECL_GEN_SIGNAL(_001OnNcActivate)
      DECL_GEN_SIGNAL(_001OnNcCalcSize)
      DECL_GEN_SIGNAL(_001OnEnable)


      bool _TrackPopupMenu(int32_t iFlags, int32_t x, int32_t y, sp(::user::interaction) oswindowParent, menu * pmenuParent);
      sp(::user::interaction)     m_oswindowParent;
      menu *        m_pmenuParent;
      HMENU                m_hmenu;
      menu *        _m_pmenu;
      bool                 m_bOwnItem;
      size                 m_size;
      int32_t                  m_iItemHeight;
      bool                 m_bAutoDelete;
      int32_t                  m_iHeaderHeight;
      ::userbase::button    m_buttonClose;
      id                   m_idSubMenu;
      int32_t                  m_iHoverSubMenu;
      uint32_t                m_dwOut;
      menu *        m_psubmenu;
      id                   m_idTimerMenu;
      point                m_ptTrack;

      /*DECL_GEN_SIGNAL(OnMouseProc)*/
      bool pre_create_window(CREATESTRUCT& cs);
      virtual void PostNcDestroy();
      ::user::front_end_schema::menu  * m_pschema;
      virtual void install_message_handling(::ca::message::dispatch * pinterface);

      DECL_GEN_SIGNAL(_001OnShowWindow)
   };

} // namespace userbase