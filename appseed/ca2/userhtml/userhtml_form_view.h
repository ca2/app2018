#pragma once

class CLASS_DECL_ca2 html_form_view :
   virtual public html_form
{
public:


   html::elemental * m_pelementalLButtonDown;

   
   html_form_view(sp(::ca::application) papp);
   virtual ~html_form_view();


#ifdef DEBUG
   virtual void assert_valid() const;
   virtual void dump(dump_context & dumpcontext) const;
#endif

   string GetLocationURL();
   bool Refresh2(int32_t i);
   bool Navigate(const char * pszUrl);

   virtual void on_document_complete(const char * pszUrl);

   virtual void install_message_handling(::ca::message::dispatch * pinterface);
   virtual bool pre_create_window(CREATESTRUCT& cs);
   virtual void OnDraw(::ca::graphics * pgraphics);      // overridden to draw this ::view
   virtual void on_update(sp(::view) pSender, LPARAM lHint, ::ca::object* pHint);


   DECL_GEN_SIGNAL(_001OnInitialUpdate)
   DECL_GEN_SIGNAL(_001OnSetFocus)
   DECL_GEN_SIGNAL(_001OnKillFocus)


};



