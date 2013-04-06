#include "framework.h"



dialog::dialog(const char * pszMatter, sp(::user::interaction) puiParent) :
   ca(puiParent->get_app()),
   userbase::view(puiParent->get_app()),
   user::scroll_view(puiParent->get_app()),
   userbase::scroll_view(puiParent->get_app()),
   user::form(puiParent->get_app()),
   userbase::form_view(puiParent->get_app()),
   html_form(puiParent->get_app()),
   html_form_view(puiParent->get_app()),
   form_view(puiParent->get_app())
{
   m_strMatter = pszMatter;
   m_pdocument    = ::null();
   m_pframe       = ::null();
   m_ulFlags      &= ~::ca::ca::flag_auto_delete;
}


dialog::dialog(sp(::ca::application) papp) :
   ca(papp),
   userbase::view(papp),
   user::scroll_view(papp),
   userbase::scroll_view(papp),
   user::form(papp),
   userbase::form_view(papp),
   html_form(papp),
   html_form_view(papp),
   form_view(papp)
{
   m_pdocument    = ::null();
   m_pframe       = ::null();
   m_ulFlags      &= ~::ca::ca::flag_auto_delete;
}

dialog::~dialog()
{
   if(m_pdocument != ::null())
   {
      m_pdocument->on_close_document();
      m_pdocument = ::null();
   }
   if(m_pframe != ::null())
   {
//      m_pframe->DestroyWindow();
     // m_pframe = ::null();
   }
}


bool dialog::show(const char * pszMatter, ::ca::property_set  * ppropertyset)
{

   if(pszMatter != ::null() && *pszMatter != '\0')
   {
      m_strMatter = pszMatter;
   }

   ::ca::property_set set(get_app());

   set["hold"] = false;

   m_pdocument = Application.userex().create_form(this, ::null(), Bergedge.get_view(), set);
   if(m_pdocument == ::null())
   {
      string str;
      str.Format("Could not show dialog %s", pszMatter);
      TRACE(str);
      return false;
   }

   if(ppropertyset != ::null())
   {

      m_pdocument->get_html_data()->m_propertyset = *ppropertyset;

   }
   
   m_pdocument->on_open_document(Application.dir().matter(m_strMatter));
   
   m_pframe =  (m_pdocument->get_view()->GetParentFrame());
   m_pframe->m_bWindowFrame         = true;
   m_pframe->m_bblur_Background     = true;
   m_pframe->m_etranslucency        = ::user::interaction::TranslucencyPresent;

   on_position_parent_frame();

   on_show(m_strMatter, &m_pdocument->get_html_data()->m_propertyset);

   m_pframe->RunModalLoop();

   return true;

}

void dialog::on_show(const char * pszMatter, ::ca::property_set  * ppropertyset)
{
   
   UNREFERENCED_PARAMETER(pszMatter);
   
   UNREFERENCED_PARAMETER(ppropertyset);

}

void dialog::EndModalLoop(id nResult)
{

   m_pframe->EndModalLoop(nResult);

   m_pframe->ShowWindow(SW_HIDE);

}







void dialog::on_position_parent_frame()
{
   rect rectOpen;
   Bergedge.get_screen_rect(rectOpen);
   int32_t iWidth = rectOpen.width();
   int32_t iHeight = rectOpen.height();
   rectOpen.deflate(iWidth / 5, iHeight / 5);
   m_pframe->SetWindowPos(ZORDER_TOP, rectOpen.left,
      rectOpen.top,
      rectOpen.width(), rectOpen.height(), SWP_SHOWWINDOW);
   m_pframe->RedrawWindow();

}

void dialog::OnCancel()
{
}

void dialog::OnOK()
{
}

void dialog::form_from_user()
{
}

void dialog::form_to_user()
{
}


void dialog::do_data_exchange(::user::data_exchange * pdx)
{
}


WINBOOL dialog::on_init_dialog()
{

   return TRUE;

}



CLASS_DECL_ca2 void DDX_Control(::user::data_exchange * pdx, id idControl, ::user::interaction & control)
{
}

CLASS_DECL_ca2 void DDX_Text(::user::data_exchange * pdx, id idControl, string & str)
{
}

CLASS_DECL_ca2 void DDV_MaxChars(::user::data_exchange * pdx, string & str, int32_t iMax)
{
}

CLASS_DECL_ca2 void DDX_Check(::user::data_exchange * pdx, id idControl, int32_t & iBool)
{
}

CLASS_DECL_ca2 void DDX_Check(::user::data_exchange * pdx, id idControl, bool & b)
{
}


