#include "framework.h"


form_document::form_document(::aura::application * papp) :
   element(papp),
   ::data::data_container_base(papp),
   ::user::document(papp)
{
}

void form_document::OnBeforeNavigate2(::html::data * pdata, var & varFile, uint32_t nFlags, const char * lpszTargetFrameName, byte_array& baPostedData, const char * lpszHeaders, bool* pbCancel)
{

   UNREFERENCED_PARAMETER(pdata);
   string strUrl(varFile);
   if(::str::begins_eat(strUrl, "ext://"))
   {
      Application.open_link(strUrl, lpszTargetFrameName);
/*         simple_shell_launcher launcher(NULL, "open", strUrl, "", "", SW_SHOWNORMAL);
         launcher.execute();*/

      *pbCancel = true;
      return;
   }
   if(::str::begins_eat(strUrl, "hist://"))
   {
      System.hist().hist(strUrl);
      *pbCancel = true;
      return;
   }

}


void form_document::form_document_set_property_set(const property_set & set)
{
   UNREFERENCED_PARAMETER(set);
}


property_set * form_document::form_document_get_property_set()
{

   return NULL;

}

property_set & form_document::form_document_property_set()
{

   return *form_document_get_property_set();

}