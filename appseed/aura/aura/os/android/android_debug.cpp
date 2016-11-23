

#include <android/log.h>


//static string * m_pstrOutputDebugStringA = NULL;
mutex * g_pmutexOutputDebugStringA = NULL;

VOID WINAPI OutputDebugStringA(LPCSTR lpOutputString)
{

   synch_lock sl(g_pmutexOutputDebugStringA);

   //if (m_pstrOutputDebugStringA == NULL)
   //{

   //   m_pstrOutputDebugStringA = new string();

   //}

   //m_pstrOutputDebugStringA->operator +=(lpOutputString);

   //// very lazy implementation

   //stringa stra;

   //stra.add_lines(*m_pstrOutputDebugStringA);

   //if (stra.get_count() > 0)
   //{

   //   *m_pstrOutputDebugStringA = stra.pop();

   //   if (stra.get_count() > 0)
   //   {

   //      for (auto str : stra)
   //      {

            //__android_log_print(ANDROID_LOG_INFO, "OutputDebugString", str);

            __android_log_print(ANDROID_LOG_INFO,"OutputDebugString",lpOutputString);

   //      }

   //   }

   //}


}


CLASS_DECL_AURA void simple_debug_print(const char * psz)
{
   __android_log_print(ANDROID_LOG_INFO, "simple_debug_string", psz);
}



VOID WINAPI OutputDebugStringW(LPCWSTR lpOutputString)
{


   OutputDebugStringA(string(lpOutputString));


}




DWORD
WINAPI
FormatMessage(
   DWORD dwFlags,
   LPCVOID lpSource,
   DWORD dwMessageId,
   DWORD dwLanguageId,
   LPSTR lpBuffer,
   DWORD nSize,
   va_list *Arguments
   )
{


   return 0;


}
