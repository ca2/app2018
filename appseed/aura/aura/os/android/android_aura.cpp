#include "framework.h"



//DWORD get_nanos()
//{
//   timeval ts;
//   gettimeofday(&ts,0);
//   return (ts.tv_sec * 1000 * 1000 * 1 + (ts.tv_usec / 1000)) % 0xffffffffu;
//
//}

thread_int_ptr < DWORD > g_dwLastError;

CLASS_DECL_AURA DWORD GetLastError()
{

   return g_dwLastError;

}

CLASS_DECL_AURA DWORD SetLastError(DWORD dw)
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







//void output_debug_string(const char * psz)
//{
//
//   output_debug_string(psz);
//
//}
//



















bool __node_aura_pre_init()
{

   //c_xstart();

   return true;

}

bool __node_pos_init()
{

   return true;

}

bool __node_pre_term()
{

   return true;

}


bool __node_pos_term()
{

   return true;

}


















































bool __node_aura_pos_init()
{


   return true;

}


bool __node_aura_pre_term()
{


   return true;

}

bool __node_aura_pos_term()
{

   return true;

}




































































































CLASS_DECL_AURA int __node_is_debugger_attached()
{
   return 0;
}




int get_processor_count()
{
   return 1;
}

