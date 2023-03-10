#pragma once


#define MESSAGE_OLE_DRAGENTER WM_APP + 324
#define MESSAGE_OLE_DRAGOVER WM_APP + 325
#define MESSAGE_OLE_DRAGLEAVE WM_APP + 326
#define MESSAGE_OLE_DRAGDROP WM_APP + 327

namespace message
{


   class CLASS_DECL_AURA create:
      public ::message::base
   {
   public:

      create(::aura::application * papp): ::message::base(papp) {}
      ::user::create_struct * m_lpcreatestruct;

      virtual void set_lresult(LRESULT lresult) override;
      using ::message::base::set;
      virtual void set(::user::primitive * pwnd, UINT uiMessage, WPARAM wparam, ::lparam lparam, LRESULT & lresult) override;
      virtual void error(const char * lpcszErrorMessage);
      virtual void failed(const char * lpcszErrorMessage);

   };


   class CLASS_DECL_AURA activate: public ::message::base
   {
   public:


      UINT  m_nState;
      ::user::interaction * m_pWndOther;
      bool  m_bMinimized;


      activate(::aura::application * papp);

      using ::message::base::set;

      virtual void set(::user::primitive * pwnd, UINT uiMessage, WPARAM wparam, ::lparam lparam, LRESULT & lresult) override;

   };

   class CLASS_DECL_AURA move: public ::message::base
   {
   public:


      move(::aura::application * papp): ::message::base(papp) {}
      point m_pt;
   };

   class CLASS_DECL_AURA size: public ::message::base
   {
   public:


      UINT     m_nType;
      ::size   m_size;

      size(::aura::application * papp): ::message::base(papp) {}

      using ::message::base::set;

      virtual void set(::user::primitive * pwnd, UINT uiMessage, WPARAM wparam, ::lparam lparam, LRESULT & lresult) override;
   };


   class CLASS_DECL_AURA scroll: public ::message::base
   {
   public:

      UINT              m_nSBCode;
      int32_t           m_nPos;
      ::user::primitive *  m_pScrollBar;

      scroll(::aura::application * papp): ::message::base(papp) {}

      using ::message::base::set;

      virtual void set(::user::primitive * pwnd, UINT uiMessage, WPARAM wparam, ::lparam lparam, LRESULT & lresult) override;

   };


   class CLASS_DECL_AURA mouse:
      public ::message::base,
      public ::user::mouse
   {
   public:



      mouse(::aura::application * papp);
      virtual ~mouse();

      using ::message::base::set;

      virtual void set(::user::primitive * pwnd, UINT uiMessage, WPARAM wparam, ::lparam lparam, LRESULT & lresult) override;
      static_function mouse * cast(::message::message * pobj)
      {
         return dynamic_cast < mouse * > (pobj);
      }

      virtual unsigned int get_message() override
      {
         return (unsigned int) m_id.i64();
      }

   };

   class CLASS_DECL_AURA mouse_wheel:
      public mouse
   {
   public:


      mouse_wheel(::aura::application * papp):
         mouse(papp)
      {
      }

      UINT     GetFlags();
      int16_t  GetDelta();
      point    GetPoint();
      using ::message::base::set;

      virtual void set(::user::primitive * pwnd, UINT uiMessage, WPARAM wparam, ::lparam lparam, LRESULT & lresult) override;
   };


   class CLASS_DECL_AURA mouse_activate: public ::message::base
   {
   public:


      mouse_activate(::aura::application * papp): ::message::base(papp) {}
      ::user::interaction * get_desktop_window();
      UINT GetHitTest();
      UINT get_message();
   };

   class CLASS_DECL_AURA context_menu: public ::message::base
   {
   public:


      context_menu(::aura::application * papp): ::message::base(papp) {}
      //::window_sp GetWindow();
      point GetPoint();
   };


   class CLASS_DECL_AURA set_cursor: public ::message::base
   {
   public:

      ::user::primitive * m_pWnd;
      UINT m_nHitTest;
      UINT m_message;

      set_cursor(::aura::application * papp): ::message::base(papp) {}

   };

   class CLASS_DECL_AURA show_window: public ::message::base
   {
   public:

      bool m_bShow;
      UINT  m_nStatus;

      show_window(::aura::application * papp): ::message::base(papp) {}
      using ::message::base::set;
      virtual void set(::user::primitive * pwnd, UINT uiMessage, WPARAM wparam, ::lparam lparam, LRESULT & lresult) override;

   };

   class CLASS_DECL_AURA kill_focus : public ::message::base
   {
   public:

      oswindow       m_oswindowNew;

      kill_focus(::aura::application * papp) : ::message::base(papp) {}
      using ::message::base::set;
      virtual void set(::user::primitive * pwnd, UINT uiMessage, WPARAM wparam, ::lparam lparam, LRESULT & lresult) override;

   };

   class CLASS_DECL_AURA on_draw: public ::message::base
   {
   public:

      ::draw2d::graphics * m_pdc;

      on_draw(::aura::application * papp): ::message::base(papp) {}

   };

   class CLASS_DECL_AURA erase_bkgnd: public ::message::base
   {
   public:


      ::draw2d::graphics * m_pdc;


      erase_bkgnd(::aura::application * papp);

      void set_result(bool bResult);

   };

   // WM_PAINT -> aura
   // WM_MOUSELEAVE -> aura

   class CLASS_DECL_AURA nchittest: public ::message::base
   {
   public:


      nchittest(::aura::application * papp): ::message::base(papp) {}
      point m_pt;
   };

   class CLASS_DECL_AURA key:
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
      virtual void set(::user::primitive * pwnd, UINT uiMessage, WPARAM wparam, ::lparam lparam, LRESULT & lresult) override;

      key & operator = (const key & key);

   };

   class CLASS_DECL_AURA nc_activate: public ::message::base
   {
   public:


      bool m_bActive;


      nc_activate(::aura::application * papp);

      using ::message::base::set;
      virtual void set(::user::primitive * pwnd, UINT uiMessage, WPARAM wparam, ::lparam lparam, LRESULT & lresult) override;
   };


   class CLASS_DECL_AURA notify: public ::message::base
   {
   public:


      notify(::aura::application * papp): ::message::base(papp) {}

#ifdef WINDOWSEX
      LPNMHDR get_lpnmhdr();
#endif

      int32_t get_ctrl_id();

   };


   class CLASS_DECL_AURA command: public ::message::base
   {
   public:


      command(::aura::application * papp): ::message::base(papp) {}
      UINT GetNotifyCode();
      UINT GetId();
      oswindow get_oswindow();
   };

   class CLASS_DECL_AURA ctl_color: public ::message::base
   {
   public:


      ctl_color(::aura::application * papp): ::message::base(papp) {}
      HBRUSH      m_hbrush;
      ::draw2d::graphics *       m_pdc;
      //::window_sp      m_pwnd;
      UINT        m_nCtlType;
   };

   class CLASS_DECL_AURA set_focus: public ::message::base
   {
   public:


      set_focus(::aura::application * papp): ::message::base(papp) {}
      using ::message::base::set;
      virtual void set(::user::primitive * pwnd, UINT uiMessage, WPARAM wparam, ::lparam lparam, LRESULT & lresult) override;
   };



   class CLASS_DECL_AURA window_pos: public ::message::base
   {
   public:

      window_pos(::aura::application * papp): ::message::base(papp) {}
      WINDOWPOS * m_pwindowpos;
      using ::message::base::set;
      virtual void set(::user::primitive * pwnd, UINT uiMessage, WPARAM wparam, ::lparam lparam, LRESULT & lresult) override;
   };



#ifdef WINDOWSEX


   class CLASS_DECL_AURA measure_item: public ::message::base
   {
   public:


      measure_item(::aura::application * papp): ::message::base(papp) {}
      int32_t m_i;
      LPMEASUREITEMSTRUCT m_lpmis;
   };

#endif

   class CLASS_DECL_AURA nc_calc_size: public ::message::base
   {
   public:


      nc_calc_size(::aura::application * papp): ::message::base(papp) {}
      NCCALCSIZE_PARAMS * m_pparams;
      bool GetCalcValidRects();
      using ::message::base::set;
      virtual void set(::user::primitive * pwnd, UINT uiMessage, WPARAM wparam, ::lparam lparam, LRESULT & lresult) override;

   };


   class CLASS_DECL_AURA enable: public ::message::base
   {
   public:


      enable(::aura::application * papp): ::message::base(papp) {}
      bool get_enable();
   };




   class CLASS_DECL_AURA drag_and_drop: public ::message::base
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


