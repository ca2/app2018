#include "framework.h"



CLASS_DECL_c int32_t ___()
{

   MessageBox(::ca::null(), "___ library of ca", "___ library of ca", MB_ICONINFORMATION | MB_OK);

   return 0;

}


CLASS_DECL_c dword get_tick_count()
{

#ifdef METROWIN

   return (dword) (GetTickCount64() % 0x100000000ULL);

#else

   return (dword) GetTickCount();

#endif

}




