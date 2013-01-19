#pragma once



CLASS_DECL_ca2 void DDX_Control(::user::data_exchange * pdx, id idControl, ::user::interaction & control);
CLASS_DECL_ca2 void DDX_Text(::user::data_exchange * pdx, id idControl, string & str);
CLASS_DECL_ca2 void DDV_MaxChars(::user::data_exchange * pdx, string & str, int32_t iMax);
CLASS_DECL_ca2 void DDX_Check(::user::data_exchange * pdx, id idControl, int32_t & iBool);
CLASS_DECL_ca2 void DDX_Check(::user::data_exchange * pdx, id idControl, bool & b);


class CLASS_DECL_ca2 dialog :
   virtual public form_view
{
public:


   form_document *            m_pdocument;
   simple_frame_window *      m_pframe;
   string                     m_strMatter;


   dialog(::ca::application * papp);
   dialog(const char * pszMatter, ::user::interaction * puiParent);
   virtual ~dialog();

   virtual void on_position_parent_frame();

   virtual void EndModalLoop(id nResult);

   virtual bool show(const char * pszMatter = NULL, gen::property_set * ppropertyset = NULL);

   virtual void on_show(const char * pszMatter, gen::property_set * ppropertyset);



   virtual void do_data_exchange(::user::data_exchange * pdx);


   virtual WINBOOL on_init_dialog();




   // MFC dialog compatibility : TODO, ca2fy, at least the member function names
   virtual void OnCancel();
   virtual void OnOK();
   //virtual bool update_data(bool bSave);
   // update_data true
   virtual void form_from_user();
   // update_data false
   virtual void form_to_user();
   //virtual bool OnInitDialog();
   // substituir, replace _001Initialize

};


