﻿#include "framework.h"


html_document::html_document(::ca::application * papp) :
   ca(papp),
   ::userbase::document(papp)
{


}

bool html_document::on_new_document()
{

   ::database::client::initialize(get_app());

   if (!::userbase::document::on_new_document())
      return FALSE;

   update_all_views(NULL, 0);

/*
   str += "<h1>Thomas Borregaard Sørensen ・minha Vida Eterna, meu Cora鈬o Eterno, Todo meu tesouro eterno, meu Universo eterno, meu tudo eterno!!</h1>";
   str += "<h2>Assinado Camilo Sasuke Tsumanuma.</h2>\n";
   str += "<span>htmlapp dedicado ao Thomas Borregaard Sørensen!!</span>";
*/

   return TRUE;

}

html_document::~html_document()
{
}



#ifdef DEBUG
void html_document::assert_valid() const
{
   ::userbase::document::assert_valid();
}

void html_document::dump(dump_context & dumpcontext) const
{
   ::userbase::document::dump(dumpcontext);
}
#endif //DEBUG

/////////////////////////////////////////////////////////////////////////////
// html_document serialization

/*void html_document::Serialize(CArchive& ar)
{
   if (ar.IsStoring())
   {
      // TODO: add storing code here
   }
   else
   {
      // TODO: add loading code here
   }
}
*/

void html_document::data_on_after_change(gen::signal_object * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
}

void html_document::OnBeforeNavigate2(html::data * pdata, var & varFile, DWORD nFlags, const char * lpszTargetFrameName, byte_array& baPostedData, const char * lpszHeaders, bool* pbCancel)
{
   UNREFERENCED_PARAMETER(pdata);
   UNREFERENCED_PARAMETER(varFile);
   UNREFERENCED_PARAMETER(nFlags);
   UNREFERENCED_PARAMETER(lpszTargetFrameName);
   UNREFERENCED_PARAMETER(baPostedData);
   UNREFERENCED_PARAMETER(lpszHeaders);
   UNREFERENCED_PARAMETER(pbCancel);
}

bool html_document::on_open_document(var varFile)
{
   
   ::ca::data::writing writing(get_html_data());
   


   
   
   get_html_data()->m_pform = get_typed_view < html_form > ();


   

   
   if(get_html_data()->m_pform == NULL)
      return false;


   get_html_data()->m_propset.merge(oprop("html_data_propset").propset());


   if(!get_html_data()->open_document(varFile))
      return FALSE;
   
   set_path_name(get_html_data()->m_strPathName);
   /*   m_document.m_papp = get_app();*/
   
   html_view_update_hint uh;
   uh.m_etype = html_view_update_hint::type_document_complete;
   uh.m_strUrl = varFile;
   update_all_views(NULL, 0, &uh);


   data_set("LastOpenedFile", get_path_name());


   return TRUE;


}

void html_document::soft_reload()
{
   ::ca::data::writing writing(get_html_data());
   string str = get_html_data()->m_strSource;
   //if(m_propset["bReplaceEx1"])
   {
      get_html_data()->m_propertyset.replace_ex1(str);
   }
   TRACE0(str);
   get_html_data()->load(str);
   html_view_update_hint uh;
   uh.m_etype = html_view_update_hint::type_document_complete;
   uh.m_strUrl = get_path_name();
   update_all_views(NULL, 0, &uh);
}

::html::data * html_document::get_html_data()
{
   if(::document::get_data() == NULL)
   {
      set_data(get_typed_view < html_form > ()->get_html_data());
      get_html_data()->m_pcallback = this;
      get_html_data()->m_propset["bReplaceEx1"] = true;
   }
   return dynamic_cast < ::html::data * > (::document::get_data());
}

bool html_document::_001OnUpdateCmdUi(cmd_ui * pcmdui)
{
   if(pcmdui->m_id == "viewindefaultbrowser")
   {
      pcmdui->Enable();
      return true;
   }
   return false;
}
 
bool html_document::_001OnCommand(id id)
{
   
   if(id == "viewindefaultbrowser")
   {
      
      gen::property_set propertyset;
      
      System.message_box("html_reader\\going_to_open_in_default_browser.xml", propertyset);

      simple_shell_launcher launcher(NULL, "open", get_path_name(), NULL, System.dir().name(get_path_name()), SW_SHOWNORMAL);
         
      launcher.execute();
      
      return true;

   }

   return false;

}


bool html_document::open_document(var varFile)
{
   return on_open_document(varFile) != FALSE;
}



