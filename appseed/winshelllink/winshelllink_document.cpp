#include "StdAfx.h"
#include "winshelllink_document.h"


winshelllink_document::winshelllink_document(::ca::application * papp) :
   ca(papp),
   ::ca::data_container(papp),
   ::document(papp),
   ::userbase::document(papp)
{
}

BOOL winshelllink_document::on_new_document()
{
	if (!::userbase::document::on_new_document())
		return FALSE;


   update_all_views(NULL, 0);

   string str;
   str = "<winshelllink>\n";
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
   str += "</winshelllink>\n";

	return TRUE;
}

winshelllink_document::~winshelllink_document()
{
}



/////////////////////////////////////////////////////////////////////////////
// winshelllink_document diagnostics

#ifdef _DEBUG
void winshelllink_document::assert_valid() const
{
	::userbase::document::assert_valid();
}

void winshelllink_document::dump(dump_context & dumpcontext) const
{
	::userbase::document::dump(dumpcontext);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// winshelllink_document serialization

/*
void winshelllink_document::Serialize(CArchive& ar)
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

void winshelllink_document::data_on_after_change(gen::signal_object * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
}

bool winshelllink_document::on_open_document(var varUrl)
{
   string str = Application.file().as_string(varUrl);
   update_all_views(NULL, 123);
   return TRUE;
}



