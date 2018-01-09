#include "framework.h"
//#include <time.h>



thread_int_ptr < DWORD > g_dwLastError;

CLASS_DECL_AURA DWORD get_last_error()
{

   return g_dwLastError;

}

CLASS_DECL_AURA DWORD set_last_error(DWORD dw)
{

   DWORD dwLastError = g_dwLastError;

   g_dwLastError = dw;

   return dwLastError;

}


CLASS_DECL_AURA bool _istlead(int32_t ch)
{

   return false;

}


void Sleep(DWORD dwMillis)
{
   timespec t;
   t.tv_sec = dwMillis / 1000;
   t.tv_nsec = (dwMillis % 1000) * 1000 * 1000;
   nanosleep(&t, NULL);
}







