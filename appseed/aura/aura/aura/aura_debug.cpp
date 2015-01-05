﻿//#include "framework.h"


CLASS_DECL_AURA int32_t FUNCTION_DEBUGBOX(const char * pszMessage, const char * pszTitle, int32_t iFlags)
{

   return ::MessageBoxA(NULL,pszMessage,pszTitle,iFlags);

}


#ifdef DEBUG
void TRACELASTERROR()
{
   sp(::aura::application) m_pauraapp = get_thread_app();
   string strErrorMessage = ::get_system_error_message(::GetLastError());
   TRACE("Error Message :\n%s\n", strErrorMessage.c_str());
}
#endif


string FormatMessageFromSystem(uint32_t dwError)
{


   return ::get_system_error_message(dwError);


}
