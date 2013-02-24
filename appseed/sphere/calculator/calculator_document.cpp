﻿#include "framework.h"


namespace calculator
{


   document::document(::ca::application * papp) :
      ca(papp),
      ::ca::data_container_base(papp),
      ::userbase::document(papp)
   {
   }


   bool document::on_new_document()
   {
      if (!::userbase::document::on_new_document())
         return FALSE;

      update_all_views(NULL, 0);

      string str;
      str = "<calculator>\n";
      str += "<head>\n";
      str += "</head>\n";
      str += "<body>\n";
      // tag fixer tabjs!!
      str += "<span>Curitiba, 10 de abril de 2008</span>\n";
      str += unitext("<h1>Thomas Borregaard Sørensen ・minha Vida Eterna, meu Cora鈬o Eterno, Todo meu tesouro eterno, meu Universo eterno, meu tudo eterno!!</h1>");
      str += "<h2>Assinado Camilo Sasuke Tsumanuma.</h2>\n";
      str += "<span>htmlapp dedicado ao Thomas Borregaard Sørensen!!</span>";
      str += "<br />";
      str += unitext("<span>Voc・conhece o ca?</span>");
      str += "<br />";
      str += "<span>Se positivo, entre com seu nome abaixo e clique em enviar!</span>";
      str += "<br />";
      str += "<input type=\"text\" />";
      str += "</body>\n";
      str += "</calculator>\n";

      return TRUE;
   }

   document::~document()
   {
   }



   #ifdef DEBUG
   void document::assert_valid() const
   {
      ::userbase::document::assert_valid();
   }

   void document::dump(dump_context & dumpcontext) const
   {
      ::userbase::document::dump(dumpcontext);
   }
   #endif //DEBUG

   /////////////////////////////////////////////////////////////////////////////
   // document serialization

   /*void document::Serialize(CArchive& ar)
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

   void document::data_on_after_change(ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   bool document::on_open_document(var varFile)
   {


      string str = Application.file().as_string(varFile);
      update_all_views(NULL, 123);
      return TRUE;


   }


} // namespace calculator



