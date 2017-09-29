#include "framework.h"


html_document::html_document(::aura::application * papp) :
   ::object(papp),
   ::data::data_container_base(papp),
   ::user::controller(papp),
   ::user::document(papp)
{

}


bool html_document::on_new_document()
{

   ::database::client::initialize_data_client(&Application.dataserver());

   if (!::user::document::on_new_document())
      return FALSE;

   update_all_views(NULL, 123);


   return TRUE;

}

html_document::~html_document()
{
}



#ifdef DEBUG
void html_document::assert_valid() const
{
   ::user::document::assert_valid();
}

void html_document::dump(dump_context & dumpcontext) const
{
   ::user::document::dump(dumpcontext);
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

void html_document::data_on_after_change(::message::message * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
}


void html_document::form_document_set_property_set(const property_set & set)
{
   get_html_data()->m_propertyset = set;
}


property_set * html_document::form_document_get_property_set()
{

   return &get_html_data()->m_propertyset;

}

void html_document::OnBeforeNavigate2(::html::data * pdata, var & varFile, uint32_t nFlags, const char * lpszTargetFrameName, byte_array& baPostedData, const char * lpszHeaders, bool* pbCancel)
{
   UNREFERENCED_PARAMETER(pdata);
   UNREFERENCED_PARAMETER(varFile);
   UNREFERENCED_PARAMETER(nFlags);
   UNREFERENCED_PARAMETER(lpszTargetFrameName);
   UNREFERENCED_PARAMETER(baPostedData);
   UNREFERENCED_PARAMETER(lpszHeaders);
   UNREFERENCED_PARAMETER(pbCancel);


   if(get_html_data()->m_pform != NULL
      && get_html_data()->m_pform->m_pcallback != NULL
      && get_html_data()->m_pform->m_pcallback != dynamic_cast < ::user::form_callback * > (this))
   {
      get_html_data()->m_pform->m_pcallback->OnBeforeNavigate2(get_html_data(),varFile,nFlags,lpszTargetFrameName,baPostedData,lpszHeaders,pbCancel);
   }

}

bool html_document::on_open_document(var varFile)
{

   get_html_data()->m_pform = get_typed_view < html_form >();

   if(get_html_data()->m_pform == NULL)
   {
      
      return false;
      
   }

   get_html_data()->m_propset.merge(oprop("html_data_propset").propset());


   if(!get_html_data()->open_document(varFile))
   {
      
      return false;
      
   }

   set_path_name(get_html_data()->m_strPathName);
   /*   m_document.m_pauraapp = get_app();*/

   html_view_update_hint uh;
   uh.m_etype = html_view_update_hint::type_document_complete;
   uh.m_strUrl = varFile;
   update_all_views(NULL, 0, &uh);

   data_set(".local://LastOpenedFile", get_file_path());

   

   return TRUE;

}


void html_document::soft_reload()
{

   synch_lock lock(get_html_data()->m_pmutex);
   
   string str = get_html_data()->m_strSource;
   
   //if(m_propset["bReplaceEx1"])
   {
      get_html_data()->m_propertyset.replace_gen(str);
   }
   TRACE("%s", str);
   get_html_data()->load(str);
   html_view_update_hint uh;
   uh.m_etype = html_view_update_hint::type_document_complete;
   
   uh.m_strUrl = get_file_path();

   update_all_views(NULL, 0, &uh);

}


::html::data * html_document::get_html_data()
{

   ::html::data * pdata = m_spadata.get < ::html::data >();
   
   if (pdata != NULL)
   {

      return pdata;

   }

   pdata = canew(html::data(get_app()));

   set_data(this, pdata);
      
   pdata->m_pui = get_typed_view < html_form >();
   
   pdata->m_pcallback = this;
   
   pdata->m_propset["bReplaceEx1"] = true;

   return pdata;

}


void html_document::on_command_probe(::user::command * pcommand)
{

   if(pcommand->m_id == "viewindefaultbrowser")
   {
      
      pcommand->Enable();

      pcommand->m_bRet = true;

      return;

   }

}


void html_document::on_command(::user::command * pcommand)
{

   if(pcommand->m_id == "viewindefaultbrowser")
   {

      property_set propertyset;

      System.message_box("html_reader\\going_to_open_in_default_browser.xml", propertyset);

#ifndef METROWIN

      ::aura::shell_launcher launcher(NULL, "open", get_file_path(), NULL, get_file_path().folder(), SW_SHOWNORMAL);

      launcher.execute();

#endif

      pcommand->m_bRet = true;

      return;

   }

}


bool html_document::open_document(var varFile)
{

   return on_open_document(varFile) != FALSE;

}


bool html_document::load_html(const string & str)
{

   get_html_data()->load(str);

   return true;

}


sp(::user::interaction) html_document::get_frame()
{

   return get_html_data()->get_frame();

}




void html_document::form_document_set_view(::user::form * pview)
{

   get_html_data()->m_pform = dynamic_cast < html_form * > (pview);

}


void html_document::form_document_set_callback(form_callback * pcallback)
{

   get_html_data()->m_pform->m_pcallback = pcallback;

}







