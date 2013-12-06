#include "framework.h"


CLASS_DECL_BASE int32_t FUNCTION_DEBUGBOX(const char * pszMessage, const char * pszTitle, int32_t iFlags)
{

   return MessageBox(NULL, pszMessage, pszTitle, iFlags);

}


#ifdef DEBUG
void TRACELASTERROR()
{
   sp(base_application) m_pbaseapp = get_thread_app();
   string strErrorMessage = ::get_system_error_message(::GetLastError());
   TRACE("Error Message :\n%s\n", strErrorMessage);
}
#endif


string FormatMessageFromSystem(uint32_t dwError)
{


   return ::get_system_error_message(dwError);


}
