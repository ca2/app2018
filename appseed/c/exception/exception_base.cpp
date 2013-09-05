#include "framework.h"


namespace exception
{


void base::operator delete(void * pbData)
{
   object::operator delete(pbData);
}

void base::operator delete(void * pbData,   const char * /* lpszFileName */, int32_t /* nLine */)
{
   operator delete(pbData);
}

base::base(sp(base_application) papp, uint32_t uiSkip) :
   element(papp),
   ::call_stack(papp, uiSkip)
{
   // most exceptions are deleted when not needed
   m_ulFlags |= flag_auto_clean;
   m_ulFlags &= ~flag_ready_for_delete;
}

base::base(sp(base_application) papp, bool bAutoDelete, uint32_t uiSkip) :
   element(papp),
   ::call_stack(papp, uiSkip)
{
   // for exceptions which are not auto-delete (usually)
   if(bAutoDelete)
   {
      m_ulFlags |= flag_auto_clean;
   }
   else
   {
      m_ulFlags &= ~flag_auto_clean;
   }
   m_ulFlags &= ~flag_ready_for_delete;
}

base::~base()
{
}

void base::Delete()
{
   // delete exception if it is auto-deleting
   if(is_set_ca_flag(flag_auto_clean))
   {
      m_ulFlags |= flag_ready_for_delete;
      delete this;
   }
}


bool base::get_error_message(string & str, PUINT pnHelpContext)
{

   if (pnHelpContext != NULL)
      *pnHelpContext = 0;

   str.Empty();

   return false;

}

string base::get_message(PUINT pnHelpContext)
{

   string str;

   if(!get_error_message(str, pnHelpContext))
      return "";

   return str;

}





int32_t base::ReportError(UINT nType /* = MB_OK */, const char * pszMessageId /* = NULL */)
{
   string   strErrorMessage;
   int32_t     nDisposition;
   UINT    nHelpContext;

   // nHelpContext should come with the message "<helpcontext id=\"123\" />"

   if (get_error_message(strErrorMessage, &nHelpContext))
      nDisposition = System.simple_message_box(strErrorMessage, nType);
   else
   {
      if (pszMessageId == 0)
         //pszMessageId = __IDP_NO_ERROR_AVAILABLE;
         pszMessageId = "Error message not available";
      nDisposition = System.simple_message_box( pszMessageId, nType);
   }
   return nDisposition;
}


} // namespace exception



