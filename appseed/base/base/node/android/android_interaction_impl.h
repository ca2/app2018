#pragma once


namespace android
{


   CLASS_DECL_BASE LRESULT CALLBACK __send_message_hook(int32_t,WPARAM,LPARAM);
   //CLASS_DECL_BASE void _::ca2::StandardSubclass(oswindow);
   CLASS_DECL_BASE LRESULT CALLBACK __cbt_filter_hook(int32_t,WPARAM,LPARAM);
   CLASS_DECL_BASE LRESULT __call_window_procedure(::user::interaction * pWnd,oswindow hWnd,UINT nMsg,WPARAM wParam,LPARAM lParam);


   class CLASS_DECL_BASE interaction_impl:
      virtual public ::user::interaction_impl
   {
   public:

      rect64                              m_rectParentClient;
      spa(::user::interaction)         m_guieptraMouseHover;
      //::message_queue_listener *       m_pcallback;
      ::user::interaction_base *       m_pbasewnd;
      ::user::interaction *          m_pguiecapture;
      bool                             m_bExposing;
      int32_t                          m_iDepth;
      //UINT m_nFlags;      // see WF_ flags above

      // xxx      WNDPROC m_pfnSuper; // for subclassing of controls
      static const UINT m_nMsgDragList;



      interaction_impl();
      interaction_impl(::aura::application * papp);
      virtual ~interaction_impl();


      virtual void construct(oswindow hwnd);


      virtual void mouse_hover_add(::user::interaction * pinterface);
      virtual void mouse_hover_remove(::user::interaction * pinterface);

      //bool create_message_window(const char * pszName, ::message_queue_listener * pcallback = NULL);
      bool create_message_queue(const char * pszName);

      static const MESSAGE* PASCAL GetCurrentMessage();

      virtual void install_message_handling(::message::dispatch * pinterface);

      bool operator==(const interaction_impl& wnd) const;
      bool operator!=(const interaction_impl& wnd) const;

      DWORD GetStyle();
      DWORD GetExStyle();
      bool ModifyStyle(DWORD dwRemove,DWORD dwAdd,UINT nFlags = 0);
      bool ModifyStyleEx(DWORD dwRemove,DWORD dwAdd,UINT nFlags = 0);

      //virtual ::user::interaction * GetOwner();
      //virtual void set_owner(::user::interaction * pOwnerWnd);

      virtual oswindow get_handle() const;

      virtual bool _001OnCmdMsg(::aura::cmd_msg * pcmdmsg);

      virtual bool BaseOnControlEvent(::user::control_event * pevent);

      void _002OnDraw(::draw2d::graphics * pdc);

      DECL_GEN_SIGNAL(_001OnEraseBkgnd);
      DECL_GEN_SIGNAL(_001OnMove);
      DECL_GEN_SIGNAL(_001OnSize);
      DECL_GEN_SIGNAL(_001OnShowWindow);
      DECL_GEN_SIGNAL(_001OnProdevianSynch);


      virtual ::user::interaction * from_os_data(void * pdata);
      virtual void * get_os_data() const;


      // subclassing/unsubclassing functions
      virtual void pre_subclass_window();
      bool SubclassWindow(oswindow hWnd);
      //bool SubclassDlgItem(UINT nID, ::user::interaction * pParent);
      oswindow UnsubclassWindow();

      // handling of RT_DLGINIT resource (extension to RT_DIALOG)
      bool ExecuteDlgInit(const char * lpszResourceName);
      bool ExecuteDlgInit(LPVOID lpResource);

      // for child windows, views, panes etc
      virtual bool create_window(const char * lpszClassName,const char * lpszWindowName,DWORD dwStyle,const RECT & rect,::user::interaction * pParentWnd,id id,sp(::create) pContext = NULL);

      // advanced creation (allows access to extended styles)
      virtual bool create_window_ex(DWORD dwExStyle,const char * lpszClassName,const char * lpszWindowName,DWORD dwStyle,const RECT & rect,oswindow hWndParent,id id,LPVOID lpParam = NULL);


      virtual bool DestroyWindow();

      // special pre-creation and ::user::interaction rect adjustment hooks
      virtual bool pre_create_window(::user::create_struct& cs);

      // Advanced: virtual AdjustWindowRect
      enum AdjustType { adjustBorder = 0,adjustOutside = 1 };
      virtual void CalcWindowRect(LPRECT lpClientRect,UINT nAdjustType = adjustBorder);


      // get immediate child with given ID
      using ::user::interaction_impl::get_child_by_id;
      void get_child_by_id(id id,oswindow* phWnd) const;
      // as above, but returns oswindow
      using ::user::interaction_impl::GetDescendantWindow;
      ::user::interaction * GetDescendantWindow(id id);
      // like get_child_by_id but recursive
      void SendMessageToDescendants(UINT message,WPARAM wParam = 0,lparam lParam = 0,bool bDeep = TRUE,bool bOnlyPerm = FALSE);
      static ::user::interaction * PASCAL GetSafeOwner(::user::interaction * pParent = NULL,oswindow* pWndTop = NULL);

      virtual bool IsWindow();

#if(WINVER >= 0x0500)

      ::window_sp GetAncestor(UINT gaFlags) const;

#endif   // WINVER >= 0x0500

      LRESULT send_message(UINT message,WPARAM wParam = 0,lparam lParam = 0);
      bool post_message(UINT message,WPARAM wParam = 0,lparam lParam = 0);

      bool SendNotifyMessage(UINT message,WPARAM wParam,lparam lParam);
      bool SendChildNotifyLastMsg(LRESULT* pResult = NULL);

      bool DragDetect(POINT pt) const;





      // oswindow Text Functions
      void SetWindowText(const char * lpszString);
      strsize GetWindowText(LPTSTR lpszStringBuf,strsize nMaxCount);
      void GetWindowText(string & rString);
      strsize GetWindowTextLength();


      // oswindow size and position Functions
      virtual bool IsIconic();
      virtual bool IsZoomed();
      void MoveWindow(int32_t x,int32_t y,int32_t nWidth,int32_t nHeight,
         bool bRepaint = TRUE);
      void MoveWindow(LPCRECT lpRect,bool bRepaint = TRUE);
      int32_t SetWindowRgn(HRGN hRgn,bool bRedraw);
      int32_t GetWindowRgn(HRGN hRgn);

      //static const ::interaction_impl wndTop; // SetWindowPos's pWndInsertAfter
      //static const ::interaction_impl wndBottom; // SetWindowPos's pWndInsertAfter
      //static const ::interaction_impl wndTopMost; // SetWindowPos pWndInsertAfter
      //static const ::interaction_impl wndNoTopMost; // SetWindowPos pWndInsertAfter

      virtual bool SetWindowPos(int32_t z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags);
      virtual UINT ArrangeIconicWindows();
      virtual bool BringWindowToTop();
      using ::user::interaction_impl::GetWindowRect;
      virtual void GetWindowRect(RECT64 * lpRect);
      using ::user::interaction_impl::GetClientRect;
      virtual void GetClientRect(RECT64 * lpRect);

      void ClientToScreen(LPRECT lprect);
      void ClientToScreen(LPPOINT lppoint);
      void ClientToScreen(RECT64 * lprect);
      void ClientToScreen(POINT64 * lppoint);
      void ScreenToClient(LPRECT lprect);
      void ScreenToClient(LPPOINT lppoint);
      void ScreenToClient(RECT64 * lprect);
      void ScreenToClient(POINT64 * lppoint);

      virtual bool GetWindowPlacement(WINDOWPLACEMENT* lpwndpl);
      virtual bool SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl);

      // Coordinate Mapping Functions
      //virtual void MapWindowPoints(::user::interaction * pwndTo,LPPOINT lpPoint,UINT nCount);
      //virtual void MapWindowPoints(::user::interaction * pwndTo,LPRECT lpRect);

      // Update/Painting Functions
  //    virtual ::draw2d::graphics * GetDC();
//      virtual ::draw2d::graphics * GetWindowDC();
      virtual bool ReleaseDC(::draw2d::graphics * pgraphics);
      virtual void Print(::draw2d::graphics * pgraphics,DWORD dwFlags) const;
      virtual void PrintClient(::draw2d::graphics * pgraphics,DWORD dwFlags) const;

      virtual void UpdateWindow();
      virtual void SetRedraw(bool bRedraw = TRUE);
      virtual bool GetUpdateRect(LPRECT lpRect,bool bErase = FALSE);
      virtual int32_t GetUpdateRgn(::draw2d::region* pRgn,bool bErase = FALSE);
      virtual void Invalidate(bool bErase = TRUE);
      virtual void InvalidateRect(LPCRECT lpRect,bool bErase = TRUE);
      virtual void InvalidateRgn(::draw2d::region* pRgn,bool bErase = TRUE);
      virtual void ValidateRect(LPCRECT lpRect);
      virtual void ValidateRgn(::draw2d::region* pRgn);
      virtual bool ShowWindow(int32_t nCmdShow);
      virtual void _001WindowMaximize();
      virtual void _001WindowRestore();
      virtual bool IsWindowVisible();
      virtual void ShowOwnedPopups(bool bShow = TRUE);

      virtual ::draw2d::graphics * GetDCEx(::draw2d::region* prgnClip,DWORD flags);
      virtual bool LockWindowUpdate();
      virtual void UnlockWindowUpdate();
      virtual bool RedrawWindow(LPCRECT lpRectUpdate = NULL,
         ::draw2d::region* prgnUpdate = NULL,
         UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
      // xxx      virtual bool EnableScrollBar(int32_t nSBFlags, UINT nArrowFlags = ESB_ENABLE_BOTH);

      virtual    void set_view_port_org(::draw2d::graphics * pgraphics);


      virtual bool DrawAnimatedRects(int32_t idAni,CONST RECT *lprcFrom,CONST RECT *lprcTo);
      virtual bool DrawCaption(::draw2d::graphics * pgraphics,LPCRECT lprc,UINT uFlags);

#if(WINVER >= 0x0500)

      virtual bool AnimateWindow(DWORD dwTime,DWORD dwFlags);

#endif   // WINVER >= 0x0500

#if(_WIN32_WINNT >= 0x0501)

      virtual bool PrintWindow(::draw2d::graphics * pgraphics,UINT nFlags) const;

#endif   // _WIN32_WINNT >= 0x0501

      // Layered oswindow

#if(_WIN32_WINNT >= 0x0500)

      virtual bool SetLayeredWindowAttributes(COLORREF crKey,BYTE bAlpha,DWORD dwFlags);
      virtual bool UpdateLayeredWindow(::draw2d::graphics * pDCDst,POINT *pptDst,SIZE *psize,
         ::draw2d::graphics * pDCSrc,POINT *pptSrc,COLORREF crKey,BLENDFUNCTION *pblend,DWORD dwFlags);

#endif   // _WIN32_WINNT >= 0x0500

#if(_WIN32_WINNT >= 0x0501)

      virtual bool GetLayeredWindowAttributes(COLORREF *pcrKey,BYTE *pbAlpha,DWORD *pdwFlags) const;

#endif   // _WIN32_WINNT >= 0x0501


      // Timer Functions
      virtual uint_ptr SetTimer(uint_ptr nIDEvent,UINT nElapse,
         void (CALLBACK* lpfnTimer)(oswindow,UINT,uint_ptr,DWORD));
      virtual bool KillTimer(uint_ptr nIDEvent);

      // oswindow State Functions
      virtual bool IsWindowEnabled();
      virtual bool EnableWindow(bool bEnable = TRUE);

      // the active ::user::interaction applies only to top-level (frame windows)
      virtual ::user::interaction * GetActiveWindow();
      virtual ::user::interaction * SetActiveWindow();

      // the foreground ::user::interaction applies only to top-level windows (frame windows)
      virtual bool SetForegroundWindow();
      static ::user::interaction * PASCAL GetForegroundWindow();

      virtual id SetDlgCtrlId(id id);
      virtual id GetDlgCtrlId();



      // capture and focus apply to all windows
      virtual ::user::interaction * GetCapture();
      virtual ::user::interaction * SetCapture(::user::interaction * pinterface = NULL);
      virtual ::user::interaction * ReleaseCapture();
      static ::user::interaction * PASCAL GetFocus();
      ::user::interaction * SetFocus();

      static ::user::interaction * PASCAL GetDesktopWindow();

      // Obsolete and non-portable APIs - not recommended for new code
      virtual void CloseWindow();
      virtual bool OpenIcon();

      // Dialog-Box Item Functions
      // (NOTE: Dialog-Box Items/Controls are not necessarily in dialog boxes!)
      virtual void CheckDlgButton(int32_t nIDButton,UINT nCheck);
      virtual void CheckRadioButton(int32_t nIDFirstButton,int32_t nIDLastButton,
         int32_t nIDCheckButton);
      virtual int32_t GetCheckedRadioButton(int32_t nIDFirstButton,int32_t nIDLastButton);
      virtual int32_t DlgDirList(LPTSTR lpPathSpec,int32_t nIDListBox,
         int32_t nIDStaticPath,UINT nFileType);
      virtual int32_t DlgDirListComboBox(LPTSTR lpPathSpec,int32_t nIDComboBox,
         int32_t nIDStaticPath,UINT nFileType);
      virtual bool DlgDirSelect(LPTSTR lpString,int32_t nSize,int32_t nIDListBox);
      virtual bool DlgDirSelectComboBox(LPTSTR lpString,int32_t nSize,int32_t nIDComboBox);

      //virtual UINT GetChildByIdInt(int32_t nID, bool * lpTrans = NULL, bool bSigned = TRUE) const;
      //virtual int32_t GetChildByIdText(int32_t nID, LPTSTR lpStr, int32_t nMaxCount) const;
      //virtual int32_t GetChildByIdText(int32_t nID, string & rString) const;
      virtual ::user::interaction * GetNextDlgGroupItem(::user::interaction * pWndCtl,bool bPrevious = FALSE) const;
      virtual ::user::interaction * GetNextDlgTabItem(::user::interaction * pWndCtl,bool bPrevious = FALSE) const;
      virtual UINT IsDlgButtonChecked(int32_t nIDButton) const;
      virtual LRESULT SendDlgItemMessage(int32_t nID,UINT message,WPARAM wParam = 0,LPARAM lParam = 0);
      virtual void SetDlgItemInt(int32_t nID,UINT nValue,bool bSigned = TRUE);
      virtual void SetDlgItemText(int32_t nID,const char * lpszString);

      // Scrolling Functions
      virtual int32_t GetScrollPos(int32_t nBar) const;
      virtual void GetScrollRange(int32_t nBar,LPINT lpMinPos,LPINT lpMaxPos) const;
      virtual void ScrollWindow(int32_t xAmount,int32_t yAmount,
         LPCRECT lpRect = NULL,
         LPCRECT lpClipRect = NULL);
      virtual int32_t SetScrollPos(int32_t nBar,int32_t nPos,bool bRedraw = TRUE);
      virtual void SetScrollRange(int32_t nBar,int32_t nMinPos,int32_t nMaxPos,
         bool bRedraw = TRUE);
      virtual void ShowScrollBar(UINT nBar,bool bShow = TRUE);
      virtual void EnableScrollBarCtrl(int32_t nBar,bool bEnable = TRUE);

      virtual int32_t ScrollWindowEx(int32_t dx,int32_t dy,
         LPCRECT lpRectScroll,LPCRECT lpRectClip,
         ::draw2d::region* prgnUpdate,LPRECT lpRectUpdate,UINT flags);
      virtual int32_t GetScrollLimit(int32_t nBar);

      virtual ::user::interaction * ChildWindowFromPoint(POINT point);
      virtual ::user::interaction * ChildWindowFromPoint(POINT point,UINT nFlags);
      static ::user::interaction * PASCAL FindWindow(const char * lpszClassName,const char * lpszWindowName);
      static ::user::interaction * FindWindowEx(oswindow hwndParent,oswindow hwndChildAfter,const char * lpszClass,const char * lpszWindow);

      virtual ::user::interaction * GetNextWindow(UINT nFlag = GW_HWNDNEXT);
      virtual ::user::interaction * GetTopWindow();

      virtual ::user::interaction * GetWindow(UINT nCmd);
      virtual ::user::interaction * GetLastActivePopup();

      virtual bool IsChild(::user::interaction *  pWnd);
      virtual ::user::interaction * GetParent() const;
      virtual ::user::interaction * SetParent(::user::interaction * pWndNewParent);
      static ::user::interaction * PASCAL oswindowFromPoint(POINT point);

      // Alert Functions

      bool FlashWindow(bool bInvert);

      virtual int32_t message_box(const char * lpszText,const char * lpszcaption = NULL,UINT nType = MB_OK);


#if(WINVER >= 0x0500)

      virtual bool FlashWindowEx(DWORD dwFlags,UINT  uCount,DWORD dwTimeout);

#endif   // WINVER >= 0x0500

      // Clipboard Functions
      virtual bool ChangeClipboardChain(oswindow hWndNext);
      virtual oswindow  SetClipboardViewer();
      virtual bool OpenClipboard();
      static ::user::interaction * PASCAL GetClipboardOwner();
      static ::user::interaction * PASCAL GetClipboardViewer();
      static ::user::interaction * PASCAL GetOpenClipboardWindow();

      // caret Functions
      virtual void Createcaret(::draw2d::bitmap* pBitmap);
      virtual void CreateSolidcaret(int32_t nWidth,int32_t nHeight);
      virtual void CreateGraycaret(int32_t nWidth,int32_t nHeight);
      static point PASCAL GetcaretPos();
      static void PASCAL SetcaretPos(POINT point);
      virtual void Hidecaret();
      virtual void Showcaret();

      // Shell Interaction Functions
      virtual void DragAcceptFiles(bool bAccept = TRUE);


      // Context Help Functions
      virtual bool SetWindowContextHelpId(uint32_t dwContextHelpId);
      virtual uint32_t GetWindowContextHelpId() const;

      // Dialog Data support
      //virtual bool update_data(bool bSaveAndValidate = TRUE);
      // data wnd must be same type as this

      // Help Command Handlers
      void OnHelp();          // F1 (uses current context)
      void OnHelpIndex();     // ID_HELP_INDEX
      void OnHelpFinder();    // ID_HELP_FINDER, ID_DEFAULT_HELP
      void OnHelpUsing();     // ID_HELP_USING
      // xxx virtual void WinHelp(dword_ptr dwData, UINT nCmd = HELP_CONTEXT);
      //virtual void HtmlHelp(dword_ptr dwData, UINT nCmd = 0x000F);
      // xxx virtual void WinHelpInternal(dword_ptr dwData, UINT nCmd = HELP_CONTEXT);

      // layout and other functions
      /*   void RepositionBars(const char * pszPrefix, const char * pszIdLeftOver,
      UINT nFlag = reposDefault, LPRECT lpRectParam = NULL,
      LPCRECT lpRectClient = NULL, bool bStretch = TRUE);*/

      //void RepositionBars(UINT nIDFirst, UINT nIDLast, id nIDLeftOver,
      // UINT nFlags = reposDefault, LPRECT lpRectParam = NULL,
      //LPCRECT lpRectClient = NULL, bool bStretch = TRUE);


      // dialog support
      void UpdateDialogControls(command_target* pTarget,bool bDisableIfNoHndler);
      void CenterWindow(::user::interaction * pAlternateOwner = NULL);
      //virtual id   RunModalLoop(DWORD dwFlags = 0, ::aura::live_object * pliveobject = NULL);
      //virtual bool ContinueModal(int32_t iLevel);
      //virtual void EndModalLoop(id nResult);
      //virtual void EndAllModalLoops(id nResult);

      // oswindow-Management message handler member functions
      virtual bool OnCommand(WPARAM wParam,LPARAM lParam);
      virtual bool OnNotify(WPARAM wParam,LPARAM lParam,LRESULT* pResult);

      void OnActivate(UINT nState,::user::interaction * pWndOther,bool bMinimized);
      void OnActivateApp(bool bActive,DWORD dwThreadID);
      LRESULT OnActivateTopLevel(WPARAM,LPARAM);
      void OncancelMode();
      void OnChildActivate();
      void OnClose();
      void OnContextMenu(::user::interaction * pWnd,point pos);
      bool OnCopyData(::user::interaction * pWnd,COPYDATASTRUCT* pCopyDataStruct);
      DECL_GEN_SIGNAL(_001OnCreate);


      HBRUSH OnCtlColor(::draw2d::graphics * pgraphics,::user::interaction * pWnd,UINT nCtlColor);

      DECL_GEN_SIGNAL(_001OnDestroy);
      void OnEnable(bool bEnable);
      void OnEndSession(bool bEnding);
      void OnEnterIdle(UINT nWhy,::user::interaction * pWho);
      bool OnEraseBkgnd(::draw2d::graphics * pgraphics);
      void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
      //xxx bool OnHelpInfo(HELPINFO* lpHelpInfo);
      void OnIconEraseBkgnd(::draw2d::graphics * pgraphics);
      void OnKillFocus(::user::interaction * pNewWnd);
      LRESULT OnMenuChar(UINT nChar,UINT nFlags,::aura::menu* pMenu);
      void OnMenuSelect(UINT nItemID,UINT nFlags,HMENU hSysMenu);
      void OnMove(int32_t x,int32_t y);
      DECL_GEN_SIGNAL(_001OnPaint);
      DECL_GEN_SIGNAL(_001OnPrint);
      DECL_GEN_SIGNAL(_001OncaptureChanged);
      void OnParentNotify(UINT message,LPARAM lParam);
      HCURSOR OnQueryDragIcon();
      bool OnQueryEndSession();
      bool OnQueryNewPalette();
      bool OnQueryOpen();
      void OnSetFocus(::user::interaction * pOldWnd);
      void OnShowWindow(bool bShow,UINT nStatus);
      void OnSize(UINT nType,int32_t cx,int32_t cy);
      void OnTcard(UINT idAction,DWORD dwActionData);
      void OnWindowPosChanging(WINDOWPOS* lpwndpos);
      void OnWindowPosChanged(WINDOWPOS* lpwndpos);

      void OnChangeUIState(UINT nAction,UINT nUIElement);
      void OnUpdateUIState(UINT nAction,UINT nUIElement);
      UINT OnQueryUIState();

      // Nonclient-Area message handler member functions
      bool OnNcActivate(bool bActive);
      void OnNcCalcSize(bool bCalcValidRects,NCCALCSIZE_PARAMS* lpncsp);
      bool OnNcCreate(::user::create_struct * lpCreateStruct);
      DECL_GEN_SIGNAL(_001OnNcDestroy);
      LRESULT OnNcHitTest(point point);
      void OnNcLButtonDblClk(UINT nHitTest,point point);
      void OnNcLButtonDown(UINT nHitTest,point point);
      void OnNcLButtonUp(UINT nHitTest,point point);
      void OnNcMButtonDblClk(UINT nHitTest,point point);
      void OnNcMButtonDown(UINT nHitTest,point point);
      void OnNcMButtonUp(UINT nHitTest,point point);
      void OnNcMouseMove(UINT nHitTest,point point);
      void OnNcPaint();
      void OnNcRButtonDblClk(UINT nHitTest,point point);
      void OnNcRButtonDown(UINT nHitTest,point point);
      void OnNcRButtonUp(UINT nHitTest,point point);

      // System message handler member functions
      void OnDropFiles(HDROP hDropInfo);
      void OnPaletteIsChanging(::user::interaction * pRealizeWnd);
      void OnSysChar(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnSysCommand(UINT nID,LPARAM lParam);
      void OnSysDeadChar(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnSysKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnSysKeyUp(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnCompacting(UINT nCpuTime);
      void OnDevModeChange(LPTSTR lpDeviceName);
      void OnFontChange();
      void OnPaletteChanged(::user::interaction * pFocusWnd);
      void OnSpoolerStatus(UINT nStatus,UINT nJobs);
      void OnSysColorChange();
      void OnTimeChange();
      void OnSettingChange(UINT uFlags,const char * lpszSection);
      void OnWinIniChange(const char * lpszSection);

      // Input message handler member functions
      void OnChar(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnDeadChar(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnKeyUp(UINT nChar,UINT nRepCnt,UINT nFlags);
      void OnLButtonDblClk(UINT nFlags,point point);
      void OnLButtonDown(UINT nFlags,point point);
      void OnLButtonUp(UINT nFlags,point point);
      void OnMButtonDblClk(UINT nFlags,point point);
      void OnMButtonDown(UINT nFlags,point point);
      void OnMButtonUp(UINT nFlags,point point);
      int32_t OnMouseActivate(::user::interaction * pDesktopWnd,UINT nHitTest,UINT message);
      void OnMouseMove(UINT nFlags,point point);
      bool OnMouseWheel(UINT nFlags,short zDelta,point pt);
      LRESULT OnRegisteredMouseWheel(WPARAM wParam,LPARAM lParam);
      void OnRButtonDblClk(UINT nFlags,point point);
      void OnRButtonDown(UINT nFlags,point point);
      void OnRButtonUp(UINT nFlags,point point);
      DECL_GEN_SIGNAL(_001OnSetCursor);
      void OnTimer(uint_ptr nIDEvent);

      // Initialization message handler member functions
      void OnInitMenu(::aura::menu* pMenu);
      void OnInitMenuPopup(::aura::menu* pPopupMenu,UINT nIndex,bool bSysMenu);

      // Clipboard message handler member functions
      void OnAskCbFormatName(UINT nMaxCount,LPTSTR lpszString);
      void OnChangeCbChain(oswindow hWndRemove,oswindow hWndAfter);
      void OnDestroyClipboard();
      void OnDrawClipboard();
      void OnHScrollClipboard(::user::interaction * pClipAppWnd,UINT nSBCode,UINT nPos);
      void OnPaintClipboard(::user::interaction * pClipAppWnd,HGLOBAL hPaintStruct);
      void OnRenderAllFormats();
      void OnRenderFormat(UINT nFormat);
      void OnSizeClipboard(::user::interaction * pClipAppWnd,HGLOBAL hRect);
      void OnVScrollClipboard(::user::interaction * pClipAppWnd,UINT nSBCode,UINT nPos);

      // control message handler member functions
      //xxx      int32_t OnCompareItem(int32_t nIDCtl, LPCOMPAREITEMSTRUCT lpCompareItemStruct);
      //xxx      void OnDeleteItem(int32_t nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);
      //xxx      void OnDrawItem(int32_t nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
      UINT OnGetDlgCode();
      //xxx      void OnMeasureItem(int32_t nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

      // MDI message handler member functions
      void OnMDIActivate(bool bActivate,::user::interaction * pActivateWnd,::user::interaction * pDeactivateWnd);

      // menu loop notification messages
      void OnEnterMenuLoop(bool bIsTrackPopupMenu);
      void OnExitMenuLoop(bool bIsTrackPopupMenu);

      // Win4 messages
      //xxx      void OnStyleChanged(int32_t nStyleType, LPSTYLESTRUCT lpStyleStruct);
      //xxx      void OnStyleChanging(int32_t nStyleType, LPSTYLESTRUCT lpStyleStruct);
      void OnSizing(UINT nSide,LPRECT lpRect);
      void OnMoving(UINT nSide,LPRECT lpRect);
      void OncaptureChanged(::user::interaction * pWnd);
      //bool OnDeviceChange(UINT nEventType,dword_ptr dwData);

      // Overridables and other helpers (for implementation of derived classes)
      // for deriving from a standard control
      // xxx      virtual WNDPROC* GetSuperWndProcaddr();

      // for dialog data exchange and validation
      //      virtual void do_data_exchange(CDataExchange* pDX);

      // for modality
      virtual void BeginModalState();
      virtual void EndModalState();

      // for translating oswindows messages in main message pump
      virtual void pre_translate_message(::signal_details * pobj);


      // for processing oswindows messages
      virtual void message_handler(::signal_details * pobj);
      //virtual bool OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

      // for handling default processing
      LRESULT Default();
      virtual LRESULT DefWindowProc(UINT message,WPARAM wParam,LPARAM lParam);

      // for custom cleanup after WM_NCDESTROY
      virtual void PostNcDestroy();

      // for notifications from parent
      virtual bool OnChildNotify(UINT message,WPARAM wParam,LPARAM lParam,LRESULT* pResult);
      // return TRUE if parent should not process this message
      bool ReflectChildNotify(UINT message,WPARAM wParam,LPARAM lParam,LRESULT* pResult);
      static bool PASCAL ReflectLastMsg(oswindow hWndChild,LRESULT* pResult = NULL);

      virtual bool CheckAutoCenter();
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
      static bool PASCAL GrayCtlColor(HDC hDC,oswindow hWnd,UINT nCtlColor,
         HBRUSH hbrGray,COLORREF clrText);


      // helper routines for implementation
      bool HandleFloatingSysCommand(UINT nID,LPARAM lParam);
      bool IsTopParentActive();
      void ActivateTopParent();
      static ::user::interaction * PASCAL GetDescendantWindow(::user::interaction * hWnd,id id);
      static void PASCAL SendMessageToDescendants(void*  hWnd,UINT message,WPARAM wParam,lparam lParam,bool bDeep,bool bOnlyPerm);
      virtual void on_final_release();
      static bool PASCAL ModifyStyle(oswindow hWnd,DWORD dwRemove,DWORD dwAdd,UINT nFlags);
      static bool PASCAL ModifyStyleEx(oswindow hWnd,DWORD dwRemove,DWORD dwAdd,UINT nFlags);
      static void PASCAL _FilterToolTipMessage(MESSAGE* pMsg,::user::interaction * pWnd);
      bool _EnableToolTips(bool bEnable,UINT nFlag);
      static oswindow PASCAL GetSafeOwner_(oswindow hWnd,oswindow* pWndTop);
      void PrepareForHelp();

      friend class frame_window;

      // for creating dialogs and dialog-like windows
      bool CreateDlg(const char * lpszTemplateName,::user::interaction * pParentWnd);
      //bool CreateDlgIndirect(LPCDLGTEMPLATE lpDialogTemplate, ::user::interaction * pParentWnd,
      // HINSTANCE hInst);


      // implementation of message dispatch/hooking
      CLASS_DECL_BASE friend LRESULT CALLBACK __send_message_hook(int32_t,WPARAM,LPARAM);
      //CLASS_DECL_BASE friend void _::ca2::StandardSubclass(oswindow);
      CLASS_DECL_BASE friend LRESULT CALLBACK __cbt_filter_hook(int32_t,WPARAM,LPARAM);
      CLASS_DECL_BASE friend LRESULT __call_window_procedure(::user::interaction * pWnd,oswindow hWnd,UINT nMsg,WPARAM wParam,LPARAM lParam);

      // standard message implementation
      LRESULT OnNTCtlColor(WPARAM wParam,LPARAM lParam);
      LRESULT OnDisplayChange(WPARAM,LPARAM);
      LRESULT OnDragList(WPARAM,LPARAM);

      static bool CALLBACK GetAppsEnumWindowsProc(oswindow hwnd,LPARAM lParam);


      static void get_app_wnda(user::oswindow_array & wnda);

      virtual void _001DeferPaintLayeredWindowBackground(HDC hdc);

      virtual LONG GetWindowLong(int32_t nIndex);
      virtual LONG SetWindowLong(int32_t nIndex,LONG lValue);

      virtual void _001BaseWndInterfaceMap();


      void _001UpdateScreen();
      void _001Expose();


      void _001OnTriggerMouseInside();


   };


} // namespace android






