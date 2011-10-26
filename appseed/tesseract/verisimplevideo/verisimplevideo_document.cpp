#include "StdAfx.h"
#include "document.h"

namespace verisimplevideo
{

   document::document(::ca::application * papp) :
      ca(papp),
      ::ca::data_container(papp),
      ::document(papp),
      ::userbase::document(papp),
      mediaplay::document(papp)
   {
      m_dataid = "ca2_fontopus_votagus::verisimplevideo::document";
   }

   BOOL document::on_new_document()
   {
      if (!::userbase::document::on_new_document())
         return FALSE;
      //string strPath;
      //if(data_get("LastOpened", strPath))
        // return on_open_document(strPath);

      string str;
      str = "<verisimplevideo>\n";
      str += "<head>\n";
      str += "</head>\n";
      str += "<body>\n";
      // tag fixer tabjs!!
      str += "<span>Curitiba, 10 de abril de 2008</span>\n";
      str += "<h1>Thomas Borregaard Sørensen � minha Vida Eterna, meu Cora��o Eterno, Todo meu tesouro eterno, meu Universo eterno, meu tudo eterno!!</h1>";
      str += "<h2>Assinado Camilo Sasuke Tsumanuma.</h2>\n";
      str += "<span>htmlapp dedicado ao Thomas Borregaard Sørensen!!</span>";
      str += "<br />";
      str += "<span>Voc� conhece o ca2?</span>";
      str += "<br />";
      str += "<span>Se positivo, entre com seu nome abaixo e clique em enviar!</span>";
      str += "<br />";
      str += "<input type=\"text\" />";
      str += "</body>\n";
      str += "</verisimplevideo>\n";



      return TRUE;
   }

   document::~document()
   {
   }



   #ifdef _DEBUG
   void document::assert_valid() const
   {
      ::userbase::document::assert_valid();
   }

   void document::dump(dump_context & dumpcontext) const
   {
      ::userbase::document::dump(dumpcontext);
   }
   #endif //_DEBUG

   /////////////////////////////////////////////////////////////////////////////
   // document serialization
   /*
   void document::Serialize(CArchive& ar)
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

   void document::data_on_after_change(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   bool document::on_open_document(var varFile)
   {
      //if(!::userbase::document::on_open_document(varFile))
        // return FALSE;
      update_all_views(NULL, 123);
      delete_contents();
      set_path_name(varFile);
      m_strOpenPath = varFile;
      data_set("LastOpened", varFile.get_string());
      update_all_views(NULL, 12);
      return TRUE;
   }

} // namespace verisimplevideo
