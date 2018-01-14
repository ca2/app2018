#pragma once


namespace user
{


   class CLASS_DECL_BASE form_window:
      virtual public ::user::control
   {
   public:


      // if this object is a form_view class instance, this member should be set to the form_view class instance
      // that superseeds this object (based on this object).
      // form_view is later defined in core library.
      form_view *                m_pformview;
      string                     m_strPath;
      bool                       m_bOnEditUpdate;
      bool                       m_bOnLanguageChange;
      UINT                       m_uiId;
      control_descriptor_set    m_controldescriptorset;
      bool                       m_bInitialized;
      form_callback *            m_pcallback;


      form_window();
      virtual ~form_window();


      //ref_array < CTransparentStatic > m_staticpa;

      virtual string get_path();
      virtual bool open_document(var varFile);
      sp(type) controltype_to_typeinfo(::user::e_control_type e_type);
   


      using ::user::control::_001IsPointInside;
      virtual bool _001IsPointInside(sp(control) pcontrol,point64 pt);
      virtual void control_get_window_rect(sp(control) pcontrol,LPRECT lprect);
      virtual void control_get_client_rect(sp(control) pcontrol,LPRECT lprect);


      virtual bool _001Initialize();

      virtual void _017OnAppLanguage();
      virtual void _001OnButtonAction(sp(control) pcontrol);
      virtual void _001OnInitializeForm(sp(control) pcontrol);
      void _001UpdateFunctionStatic();
      virtual bool _001OnBeforeSave(sp(control) pcontrol);
      virtual void _001RemoveControls();
      bool _001Validate(sp(control) pcontrol,var & var);
      bool _001SaveEdit(sp(control) pcontrol);
      virtual void on_command(::user::command * pcommand) override;
      DECL_GEN_SIGNAL(_001OnNotify);
      DECL_GEN_SIGNAL(_001OnMessageNotify);
      virtual void _001GetSelection(::database::id & key,::database::selection & selection);
      void Update(bool bSave);
      bool _001SetData(id uiId,bool bData);
      bool _001GetData(id uiId,bool & bData);
      int_ptr _001AddControl(class control_descriptor & descriptor);
      void _001FillCombo(sp(control) pcontrol);

      virtual void WfiOnClose();

      DECL_GEN_SIGNAL(_000OnPosCreate);
      DECL_GEN_SIGNAL(_001OnCreate);

      virtual void install_message_routing(::message::sender *pinterface) override;
      virtual void _001InitializeFormPreData();
      virtual void _001OnUpdate(sp(::user::impact) pviewSender,LPARAM lhint,object* phint);
      virtual void _001Update(sp(control) pcontrol);
      virtual void _001UpdateCheckBox(sp(control) pcontrol);
      virtual void _001UpdateComboBox(sp(control) pcontrol);
      virtual void _001UpdateEdit(sp(control) pcontrol);
      virtual void _001UpdateSimpleList(sp(control) pcontrol);

      virtual void _001UpdateDbFlags(sp(control) pcontrol);
      virtual void _001UpdateDbFlagsCheckBox(sp(control) pcontrol);

      DECL_GEN_SIGNAL(_001OnAppLanguage);

      bool OnCommand(WPARAM wparam,LPARAM lparam);
      bool OnCommandButton(sp(control) pcontrol,UINT uiNotificationCode,LPARAM lparam);
      bool OnCommandCheckBox(sp(control) pcontrol,UINT uiNotificationCode,LPARAM lparam);
      bool OnCommandComboBox(sp(control) pcontrol,UINT uiNotificationCode,LPARAM lparam);
      bool OnCommandEdit(sp(control) pcontrol,UINT uiNotificationCode,LPARAM lparam);


      DECL_GEN_SIGNAL(data_on_after_change);


      virtual bool create_control(class control_descriptor * pdescriptor, index iItem);
      virtual bool normalize_control_descriptor_typeinfo(class ::user::control_descriptor * pdescriptor);

      bool operator == (const control & control) const;

      bool BaseOnControlEvent(::user::control_event * pevent) override;
      void _001SetControlFactory();


      virtual bool _001OnControlSetFocus(::user::interaction * pui);
      virtual bool _001OnControlKillFocus(::user::interaction * pui);


      virtual void OnBeforeNavigate2(var & varFile,uint32_t nFlags,const char * lpszTargetFrameName,byte_array& baPostedData,const char * lpszHeaders,bool* pbCancel);
      //        ::user::form_window(::aura::application * papp);
      //    virtual ~::user::form_window();

      //virtual void install_message_routing(::message::sender * pinterface);

      //      DECL_GEN_SIGNAL(_001OnCreate);


      //      ::user::form_window(::aura::application * papp);

      //void on_update(::user::impact * pSender,LPARAM lHint,object* phint);
      //      virtual bool BaseOnControlEvent(::user::control_event * pevent);
      //      DECL_GEN_SIGNAL(_001OnCreate);
      void _001OnTimer(::timer * ptimer) override;
      DECL_GEN_SIGNAL(_001OnUser123);
      //      virtual void install_message_routing(::message::sender * pinterface);


      

//      virtual bool _001IsPointInside(sp(control) pcontrol,point64 pt);

  //    virtual void control_get_window_rect(sp(control) pcontrol,LPRECT lprect);
    //  virtual void control_get_client_rect(sp(control) pcontrol,LPRECT lprect);


      //virtual bool open_document(var varFile);


   };


} // namespace user


