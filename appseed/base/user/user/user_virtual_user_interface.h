#pragma once


class CLASS_DECL_BASE virtual_user_interface :
   virtual public ::user::interaction
{
public:


   size                       m_size;
   string                     m_strWindowText;
   bool                       m_bEnabled;
   ::draw2d::font_sp              m_spfont;
   bool                       m_bCreate;
   sp(::user::interaction)    m_pparent;
   sp(::user::interaction)    m_pguieMessage;


   virtual_user_interface();
   virtual_user_interface(sp(base_application) papp);
   virtual ~virtual_user_interface();

   using ::user::interaction::message_handler;
   virtual void message_handler(signal_details * pobj);
   virtual ::draw2d::graphics * GetDC();
   sp(::user::interaction) set_parent(sp(::user::interaction) pguieParent);
   bool ShowWindow(int32_t nCmdShow);
   virtual bool ReleaseDC(::draw2d::graphics *);
   virtual sp(::user::interaction) get_parent();

   virtual void _001WindowMaximize();
   virtual void _001WindowRestore();
   virtual bool SetWindowPos(int32_t z, int32_t x, int32_t y, int32_t cx, int32_t cy, UINT nFlags);
   void SetWindowText(const char * psz);


   virtual sp(::user::interaction) GetDescendantWindow(id id);

   id SetDlgCtrlId(id id);
   id GetDlgCtrlId();


   // Advanced: virtual AdjustWindowRect
   enum AdjustType { adjustBorder = 0, adjustOutside = 1 };
   virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
   virtual sp(::user::frame_window) GetParentFrame();

   virtual sp(::user::interaction) SetFocus();

   void RepositionBars(UINT nIDFirst, UINT nIDLast, id nIDLeftOver, UINT nFlags, LPRECT lpRectParam, LPCRECT lpRectClient, bool bStretch);


   //virtual int32_t RunModalLoop(uint32_t dwFlags = 0, ::core::live_object * pliveobject = NULL);


   virtual bool DestroyWindow();

   virtual uint32_t GetStyle();
   virtual uint32_t GetExStyle();
   virtual LRESULT Default();

   virtual void SetFont(::draw2d::font* pFont, bool bRedraw = TRUE);
   virtual ::draw2d::font* GetFont();

   virtual LRESULT send_message(UINT uiMessage, WPARAM wparam = 0, lparam lparam = 0);

#ifdef LINUX

   virtual LRESULT send_message(XEvent * pevent);

#endif

   // as hosting ::user::window
   virtual void install_message_handling(::message::dispatch * pinterface);
   // as virtual ::user::window
   virtual void _002InstallMessageHandling(::message::dispatch * pinterface);


   using ::user::interaction::create_message_queue;
   bool create_message_queue();

   // timer Functions
   virtual uint_ptr SetTimer(uint_ptr nIDEvent, UINT nElapse,
      void (CALLBACK* lpfnTimer)(oswindow, UINT, uint_ptr, uint32_t));
   virtual bool KillTimer(uint_ptr nIDEvent);


   virtual bool IsWindow();
   virtual bool IsWindowEnabled();
   virtual bool IsWindowVisible();

   virtual void VirtualOnSize();
   using ::user::interaction::create;
   virtual bool create(sp(::user::interaction)pparent, id id);
   virtual bool create(const char * lpszClassName,
      const char * lpszWindowName, uint32_t dwStyle,
      const RECT& rect,
      sp(::user::interaction) pParentWnd, id id,
      sp(::create_context) pContext = NULL);
   virtual bool CreateEx(uint32_t dwExStyle, const char * lpszClassName,
      const char * lpszWindowName, uint32_t dwStyle,
      const RECT& rect,
      sp(::user::interaction) pParentWnd, id id,
      LPVOID lpParam = NULL);


   virtual sp(::user::frame_window) EnsureParentFrame();
   virtual sp(::user::interaction) GetTopLevelParent();
   virtual sp(::user::interaction) EnsureTopLevelParent();
   virtual sp(::user::frame_window) GetTopLevelFrame();

   using ::user::interaction::GetWindowText;
   strsize GetWindowText(LPTSTR lpszStringBuf, int32_t nMaxCount);
   void GetWindowText(string & str);




   DECL_GEN_SIGNAL(_001OnSize)
      DECL_GEN_SIGNAL(_001OnMove)
      DECL_GEN_SIGNAL(_001OnDestroy)
      DECL_GEN_SIGNAL(_001OnNcDestroy)

      void SendMessageToDescendants(UINT message, WPARAM wParam = 0, lparam lParam = 0, bool bDeep = true, bool bOnlyPerm = 0);

   virtual bool post_message(UINT uiMessage, WPARAM wparam, lparam lparam);

   virtual void set_view_port_org(::draw2d::graphics * pgraphics);


};



