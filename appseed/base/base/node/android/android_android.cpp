#include "framework.h"



void CLASS_DECL_BASE __abort()
{
   //__android_term();
   abort();
}





#ifdef DEBUG
bool CLASS_DECL_BASE __check_memory()
// check all of memory (look for memory tromps)
{
   //return _CrtCheckMemory() != FALSE;
   return true;
}
#endif























