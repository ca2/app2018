﻿//#include "framework.h"


CLASS_DECL_ACE int32_t FUNCTION_DEBUGBOX(const char * pszMessage, const char * pszTitle, int32_t iFlags)
{

   return ::MessageBoxA(NULL,pszMessage,pszTitle,iFlags);

}


#ifdef DEBUG
void TRACELASTERROR()
{
   sp(::ace::application) m_paceapp = get_thread_app();
   string strErrorMessage = ::get_system_error_message(::GetLastError());
   TRACE("Error Message :\n%s\n", strErrorMessage.c_str());
}
#endif


string FormatMessageFromSystem(uint32_t dwError)
{


   return ::get_system_error_message(dwError);


}


extern "C"
void o_debug_string(const char * psz)
{
   output_debug_string(psz);
}