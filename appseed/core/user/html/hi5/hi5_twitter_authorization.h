#pragma once


namespace hi5
{


   namespace twitter
   {


      class CLASS_DECL_CORE authorization :
         virtual public ::user::form_callback,
         virtual public ::user::view_creator,
         virtual public ::user::tab_callback,
         virtual public ::user::style
      {
      public:


         string                                    m_strPin;
         string                                    m_strAuthorizationUrl;
         int32_t                                   m_iView;
         sp(::user::single_document_template)      m_ptemplatePane;
         sp(::user::document)                      m_pdoc;
         sp(::user::tab_view)                      m_ptabview;
         sp(::html_document)                       m_pdocAuth;
         sp(::html_document)                       m_pdocMessage;
         sp(::user::form_view)                     m_pviewAuth;
         bool                                      m_bAuth;
         string                                    m_strForm;
         string                                    m_strLicense;
         bool                                      m_bLicense;
         bool                                      m_bInteractive;

         authorization(::aura::application * papp,const char * pszAuthorizationUrl, const char * pszForm, bool bAuth = false, bool bInteractive = true);
         virtual ~authorization();

         void ensure_main_document();
         void display_main_frame();
         void page1();
         void pageMessage(const char * pszMatter, property_set & set);

         string get_pin();

         void on_create_view(::user::view_creator_data * pcreatordata) override;
         virtual void on_show_view() override;


         virtual bool BaseOnControlEvent(::user::control_event * pevent) override;

         virtual bool style_translucency(::user::e_translucency & etranslucency, ::user::e_element);

      };


   } // namespace twitter


} // namespace hi5




