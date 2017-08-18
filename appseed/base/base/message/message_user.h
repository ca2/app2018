#pragma once


#define MESSAGE_OLE_DRAGENTER WM_APP + 324
#define MESSAGE_OLE_DRAGOVER WM_APP + 325
#define MESSAGE_OLE_DRAGLEAVE WM_APP + 326
#define MESSAGE_OLE_DRAGDROP WM_APP + 327

namespace message
{


   class CLASS_DECL_BASE create: public base
   {
   public:

      create(::aura::application * papp): ::message::base(papp) {}
      ::user::create_struct * m_lpcreatestruct;

      virtual void set_lresult(LRESULT lresult);
      using ::message::base::set;
      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);
      virtual void error(const char * lpcszErrorMessage);
      virtual void failed(const char * lpcszErrorMessage);
   };


   class CLASS_DECL_BASE activate: public base
   {
   public:


      UINT  m_nState;
      ::user::interaction * m_pWndOther;
      bool  m_bMinimized;


      activate(::aura::application * papp);
      using ::message::base::set;

      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);
   };

   class CLASS_DECL_BASE move: public base
   {
   public:


      move(::aura::application * papp): ::message::base(papp) {}
      point m_pt;
   };

   class CLASS_DECL_BASE size: public base
   {
   public:


      size(::aura::application * papp): ::message::base(papp) {}
      UINT     m_nType;
      ::size   m_size;
      using ::message::base::set;
      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);
   };


   class CLASS_DECL_BASE scroll: public base
   {
   public:


      scroll(::aura::application * papp): ::message::base(papp) {}
      UINT              m_nSBCode;
      int32_t           m_nPos;
      ::user::primitive *  m_pScrollBar;
      using ::message::base::set;
      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);
   };


   class CLASS_DECL_BASE mouse: public base,
      public ::user::mouse
   {
   public:



      mouse(::aura::application * papp);
      virtual ~mouse();
      using ::message::base::set;

      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);
      static_function mouse * cast(signal_details * pobj) {
         return (mouse *)pobj;
      }

      virtual unsigned int get_message()
      {
         return m_uiMessage;
      }

   };

   class CLASS_DECL_BASE mouse_wheel: public mouse
   {
   public:


      mouse_wheel(::aura::application * papp): mouse(papp) {}
      UINT     GetFlags();
      int16_t    GetDelta();
      point    GetPoint();
      using ::message::base::set;

      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);
   };


   class CLASS_DECL_BASE mouse_activate: public base
   {
   public:


      mouse_activate(::aura::application * papp): ::message::base(papp) {}
      ::user::interaction * GetDesktopWindow();
      UINT GetHitTest();
      UINT get_message();
   };

   class CLASS_DECL_BASE context_menu: public base
   {
   public:


      context_menu(::aura::application * papp): ::message::base(papp) {}
      //::window_sp GetWindow();
      point GetPoint();
   };


   class CLASS_DECL_BASE set_cursor: public base
   {
   public:


      set_cursor(::aura::application * papp): ::message::base(papp) {}
      ::user::primitive * m_pWnd;
      UINT m_nHitTest;
      UINT m_message;
   };

   class CLASS_DECL_BASE show_window: public base
   {
   public:


      show_window(::aura::application * papp): ::message::base(papp) {}
      bool m_bShow;
      UINT  m_nStatus;
      using ::message::base::set;
      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);
   };

   class CLASS_DECL_BASE on_draw: public base
   {
   public:


      on_draw(::aura::application * papp): ::message::base(papp) {}
      ::draw2d::graphics * m_pdc;
   };

   class CLASS_DECL_BASE erase_bkgnd: public base
   {
   public:


      ::draw2d::graphics * m_pdc;


      erase_bkgnd(::aura::application * papp);

      void set_result(bool bResult);

   };

   // WM_PAINT -> aura
   // WM_MOUSELEAVE -> aura

   class CLASS_DECL_BASE nchittest: public base
   {
   public:


      nchittest(::aura::application * papp): ::message::base(papp) {}
      point m_pt;
   };

   class CLASS_DECL_BASE key:
      public base,
      public ::user::key
   {
   public:


      /*#ifdef METROWIN

      Agile < ::Windows::UI::Core::CharacterReceivedEventArgs ^  > m_charrecv;
      Agile < ::Windows::UI::Core::CharacterReceivedEventArgs ^  > m_key;

      #endif*/


      key(::aura::application * papp);
      key(const key & key);

      using ::message::base::set;
      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);

      key & operator = (const key & key);

   };

   class CLASS_DECL_BASE nc_activate: public base
   {
   public:


      bool m_bActive;


      nc_activate(::aura::application * papp);

      using ::message::base::set;
      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);
   };

#ifdef WINDOWSEX

   class CLASS_DECL_BASE notify: public base
   {
   public:


      notify(::aura::application * papp): ::message::base(papp) {}
      LPNMHDR get_lpnmhdr();
      int32_t get_ctrl_id();
   };

#endif

   class CLASS_DECL_BASE update_cmd_ui: public base
   {
   public:


      update_cmd_ui(::aura::application * papp): ::message::base(papp) {}
      command_ui *    m_pcommandui;
   };

   class CLASS_DECL_BASE command: public base
   {
   public:


      command(::aura::application * papp): ::message::base(papp) {}
      UINT GetNotifyCode();
      UINT GetId();
      oswindow get_oswindow();
   };

   class CLASS_DECL_BASE ctl_color: public base
   {
   public:


      ctl_color(::aura::application * papp): ::message::base(papp) {}
      HBRUSH      m_hbrush;
      ::draw2d::graphics *       m_pdc;
      //::window_sp      m_pwnd;
      UINT        m_nCtlType;
   };

   class CLASS_DECL_BASE set_focus: public base
   {
   public:


      set_focus(::aura::application * papp): ::message::base(papp) {}
      using ::message::base::set;
      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);
   };



   class CLASS_DECL_BASE window_pos: public base
   {
   public:

      window_pos(::aura::application * papp): ::message::base(papp) {}
      WINDOWPOS * m_pwindowpos;
      using ::message::base::set;
      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);
   };



#ifdef WINDOWSEX


   class CLASS_DECL_BASE measure_item: public base
   {
   public:


      measure_item(::aura::application * papp): ::message::base(papp) {}
      int32_t m_i;
      LPMEASUREITEMSTRUCT m_lpmis;
   };

#endif

   class CLASS_DECL_BASE nc_calc_size: public base
   {
   public:


      nc_calc_size(::aura::application * papp): ::message::base(papp) {}
      NCCALCSIZE_PARAMS * m_pparams;
      bool GetCalcValidRects();
      using ::message::base::set;
      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);

   };


   class CLASS_DECL_BASE enable: public base
   {
   public:


      enable(::aura::application * papp): ::message::base(papp) {}
      bool get_enable();
   };




   class CLASS_DECL_BASE drag_and_drop: public base
   {
   public:

      drag_and_drop(::user::primitive * pwnd,UINT uiMessage):
         base(pwnd->get_app())
      {
         
         m_lresult = 0;
         
         set(pwnd,uiMessage,0,0,m_lresult);
         
      }

#ifdef WINDOWS

      IDataObject *  pDataObj;// [in] DragEnter, Drop
      POINTL         pt; // [in] DragEnter, DragOver, Drop
      DWORD          grfKeyState; // [in] DragEnter, DragOver, Drop
      DWORD          dwEffect; // [in][out] DragEnter, DragOver, Drop

#endif

   };

} // namespace message


