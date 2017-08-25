#pragma once


namespace console
{


   class CLASS_DECL_SPHERE prompt_frame : 
      virtual public simple_frame_window,
      virtual public ::user::message_queue_listener
   {
   public:

      
      bool                       m_bAutoHideOnOutClick;
      int32_t                 m_iAnimateStep;
      sp(image_list)          m_pimagelist;
      bool                    m_bTimerHide;
      bool                    m_bTimerOn;
      e_position              m_eposition;
      bool                    m_bMoveDrag;
      bool                    m_bLControl;

      prompt_frame(::aura::application * papp);
      virtual ~prompt_frame();

      virtual void install_message_routing(::message::sender * pinterface);

	   void ShowControlBars(bool bShow);


      bool show_mini();
	
   #ifdef DEBUG
	   virtual void assert_valid() const;
	   virtual void dump(dump_context & dumpcontext) const;
   #endif

      bool m_bHoverMouse;
      uint32_t m_dwLastHover;

      void OnHoverAction(bool bShowWinActionArea);
   
      void ToFront();

      virtual bool on_simple_command(::user::command * pcommand);
      virtual bool on_simple_command_probe(::user::command * pcommand);


      virtual bool get_translucency(::user::e_translucency & etranslucency);


	   DECL_GEN_SIGNAL(_001OnCreate);
	   void _001OnTimer(::timer * ptimer);

      DECL_GEN_SIGNAL(_001OnClose);
      DECL_GEN_SIGNAL(_001OnMove);
      DECL_GEN_SIGNAL(_001OnShowWindow);
      DECL_GEN_SIGNAL(_001OnApp2000);


      virtual void message_queue_message_handler(::message::message * pobj);

      sp(::user::wndfrm::frame::frame) 
         create_frame_schema();
   };


} // namespace prompt


