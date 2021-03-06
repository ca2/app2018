#pragma once


class CLASS_DECL_CORE html_document :
   virtual public ::user::document
{
public:

   
   html_document(::aura::application * papp);
   virtual ~html_document();

   
   virtual void assert_valid() const override;
   virtual void dump(dump_context & dumpcontext) const override;


   virtual void OnBeforeNavigate2(::html::data * pdata, var & varFile, uint32_t nFlags, const char * lpszTargetFrameName, byte_array& baPostedData, const char * lpszHeaders, bool* pbCancel) override;
   
   virtual ::html::data * get_html_data();
   DECL_GEN_SIGNAL(data_on_after_change);

   virtual bool open_document(var varFile) override;

   virtual bool load_html(const string & str);

   virtual bool on_new_document() override;

   virtual void form_document_set_property_set(const property_set & set) override;
   virtual property_set * form_document_get_property_set() override;

   virtual void form_document_set_view(::user::form * pview);
   virtual void form_document_set_callback(form_callback * pcallback);


   bool on_open_document(var varFile) override;

   virtual void on_command_probe(::user::command * pcommand) override;
   virtual void on_command(::user::command * pcommand) override;

   virtual void soft_reload();

   virtual sp(::user::interaction) get_frame();





};



