#pragma once



namespace helloworld
{


   class CLASS_DECL_APP_HELLOWORLD application :
      virtual public ::hellobase::application,
      virtual public ::core::application
   {
   public:


      enum e_type
      {

         type_normal,
         type_mili,

      };

      e_type                                 m_etype;
      string                                 m_strHelloWorld;
      string                                 m_strHelloWorldDefault;
      string                                 m_strAlternateHelloWorld;
      string                                 m_strAlternateHelloWorldDefault;

      bool                                   m_bWorldChat;

      ::user::document *                     m_pdocMenu;
//      ::calculator::plain_edit_view *        m_prollfps;
      ::user::plain_edit_view *              m_prollfps;
      ::user::single_document_template *     m_ptemplateHelloWorldMain;
      ::user::single_document_template *     m_ptemplateHelloWorldView;
      ::user::single_document_template *     m_ptemplateHelloWorldSwitcher;


      //::user::split_view *                             m_ptopviewLast;
      //view *                                 m_pviewLast;

      //::user::split_view *                             m_ptopviewMain;
      //view *                                 m_pviewMain;
      //::user::button_view *                          m_ptoggleviewMain;
      //::user::plain_edit_view *              m_peditMain;

      //::user::split_view *                             m_ptopviewSwitcher;
      //view *                                 m_pviewSwitcher;
      //::user::button_view *                          m_ptoggleviewSwitcher;
      //::user::plain_edit_view *              m_peditSwitcher;

      //::user::split_view *                             m_ptopviewFont;
      //::user::font_list_view *               m_pfontlistview;
      //::user::button_view *                          m_ptoggleviewFontSel;
      //::user::plain_edit_view *              m_peditFontSel;

      mutex                                  m_mutexAiFont;
      int32_t                                m_iErrorAiFont;
      bool                                   m_bLoadAiFont;
      void *                                 m_faceAi; // FT_Face m_faceAi;

      application();
      virtual ~application();

      virtual string preferred_userschema() override;
      virtual bool init_instance() override;
      virtual int32_t  exit_application() override;

      virtual void on_request(::create * pcreate) override;

      virtual int64_t add_ref() override;
      virtual int64_t dec_ref() override;

   };


} // namespace helloworld



