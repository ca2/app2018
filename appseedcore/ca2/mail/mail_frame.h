#pragma once

class CLASS_DECL_ca mail_frame : public simple_frame_window
{
	
public:
   mail_frame(::ca::application * papp);

   string                    m_strAnimatedStatusBarText;
   int                        m_iAnimateStep;
   image_list                  m_imagelist;
   bool m_bTimerOn;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void ShowControlBars(bool bShow);
	void AnimateStatusBar();
	void SetAnimatedStatusBarText(const char * lpcsz);
	virtual ~mail_frame();
#ifdef _DEBUG
	virtual void assert_valid() const;
	virtual void dump(dump_context & dumpcontext) const;
#endif



//   simple_menu_bar     m_menubar;
//   SimpleDialogBar   m_dialogbar;
//xxx	SimpleReBar       m_wndReBar;
//	simple_status_bar   m_statusbar;
//   simple_toolbar     m_toolbar;
//   simple_toolbar     m_toolbarView;


   //bool              m_bFullScreen;

// Generated message map functions
protected:


	//{{AFX_MSG(mail_frame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
};

