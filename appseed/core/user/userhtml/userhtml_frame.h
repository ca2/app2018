#pragma once



class CLASS_DECL_CORE html_frame : public simple_frame_window
{
   
public: // create from serialization only
   html_frame(sp(::axis::application) papp);
   // // DECLARE_DYNCREATE(html_frame)

// Attributes
public:
   //BaseMenuMessageHandler        m_menuhook;
   string                     m_strAnimatedStatusBarText;
   int32_t                        m_iAnimateStep;
   sp(image_list)                m_pimagelist;
   bool m_bTimerOn;

   virtual bool pre_create_window(::user::create_struct& cs);
   virtual bool _001OnCmdMsg(::aura::cmd_msg * pcmdmsg); 
   void ShowControlBars(bool bShow);
   void AnimateStatusBar();
   void SetAnimatedStatusBarText(const char * lpcsz);
   virtual ~html_frame();
#ifdef DEBUG
   virtual void assert_valid() const;
   virtual void dump(dump_context & dumpcontext) const;
#endif



   bool m_bHoverMouse;
   uint32_t m_dwLastHover;

   void OnHoverAction();


//   simple_menu_bar     m_menubar;
//   SimpleDialogBar   m_dialogbar;
//xxx   SimpleReBar       m_wndReBar;
//   simple_status_bar   m_statusbar;
//   simple_toolbar     m_toolbar;
//   simple_toolbar     m_toolbarView;


   //bool              m_bFullScreen;

// Generated message map functions
protected:


   DECL_GEN_SIGNAL(_001OnCreate);
};

