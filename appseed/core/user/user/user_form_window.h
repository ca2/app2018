#pragma once


namespace user
{


   class CLASS_DECL_CORE form_window :
      virtual public ::user::form_control
   {
   public:


      //bool                          m_bOnEditUpdate;
      //bool                          m_bOnLanguageChange;
      //UINT                          m_uiId;
      //control_descriptor_set       m_controldescriptorset;
      //bool                          m_bInitialized;
      //::user::form_callback *       m_pcallback;


      form_window();
      virtual ~form_window();


      //ref_array < CTransparentStatic > m_staticpa;

      virtual string get_path();
      virtual bool open_document(var varFile);


      using ::user::control::_001IsPointInside;
      virtual bool _001IsPointInside(sp(control) pcontrol, point64 pt);
      virtual void control_get_window_rect(sp(control) pcontrol, LPRECT lprect);
      virtual void control_get_client_rect(sp(control) pcontrol, LPRECT lprect);


      virtual bool _001Initialize();

      virtual void _017OnAppLanguage();
      virtual void _001OnButtonAction(sp(control) pcontrol);
      virtual void _001OnInitializeForm(sp(control) pcontrol);
      void _001UpdateFunctionStatic();
      virtual bool _001OnBeforeSave(sp(control) pcontrol);
      virtual void _001RemoveControls();
      virtual bool _001Validate(sp(control) pcontrol, var & var);
      virtual bool _001SaveEdit(sp(control) pcontrol);
      virtual void on_command(::user::command * pcommand);
      DECL_GEN_SIGNAL(_001OnNotify);
      DECL_GEN_SIGNAL(_001OnMessageNotify);
      virtual void _001GetSelection(::database::key & key, ::database::selection & selection);
      virtual void Update(bool bSave);
      virtual bool _001SetData(id uiId, bool bData);
      virtual bool _001GetData(id uiId, bool & bData);
      virtual int_ptr _001AddControl(class control_descriptor & descriptor);
      virtual void _001FillCombo(sp(control) pcontrol);

      virtual void WfiOnClose();

      DECL_GEN_SIGNAL(_000OnPosCreate);
      DECL_GEN_SIGNAL(_001OnCreate);

      virtual void install_message_routing(::message::sender *pinterface);
      virtual void _001InitializeFormPreData();
      virtual void _001OnUpdate(sp(::user::impact) pviewSender, LPARAM lhint, object* phint);
      virtual void _001Update(sp(control) pcontrol);
      virtual void _001UpdateCheckBox(sp(control) pcontrol);
      virtual void _001UpdateComboBox(sp(control) pcontrol);
      virtual void _001UpdateEdit(sp(control) pcontrol);
      virtual void _001UpdateSimpleList(sp(control) pcontrol);

      virtual void _001UpdateDbFlags(sp(control) pcontrol);
      virtual void _001UpdateDbFlagsCheckBox(sp(control) pcontrol);

      DECL_GEN_SIGNAL(_001OnAppLanguage);

      bool OnCommand(WPARAM wparam, LPARAM lparam);
      bool OnCommandButton(sp(control) pcontrol, UINT uiNotificationCode, LPARAM lparam);
      bool OnCommandCheckBox(sp(control) pcontrol, UINT uiNotificationCode, LPARAM lparam);
      bool OnCommandComboBox(sp(control) pcontrol, UINT uiNotificationCode, LPARAM lparam);
      bool OnCommandEdit(sp(control) pcontrol, UINT uiNotificationCode, LPARAM lparam);


      DECL_GEN_SIGNAL(data_on_after_change);


      virtual bool create_control(class control_descriptor * pdescriptor, index iIndex);
      virtual bool normalize_control_descriptor_typeinfo(class ::user::control_descriptor * pdescriptor);

      bool operator == (const control & control) const;

      void on_control_event(::user::control_event * pevent);
      void _001SetControlFactory();


      virtual void OnBeforeNavigate2(var & varFile, uint32_t nFlags, const char * lpszTargetFrameName, byte_array& baPostedData, const char * lpszHeaders, bool* pbCancel);


   };


} // namespace user










