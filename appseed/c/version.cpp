#include "StdAfx.h"


CLASS_DECL_c int _ca_is_basis()
{
#if CA2_PLATFORM_VERSION == CA2_BASIS
   return TRUE;
#else
   return FALSE;
#endif
}

CLASS_DECL_c int _ca_is_stage()
{
#if CA2_PLATFORM_VERSION == CA2_STAGE
   return TRUE;
#else
   return FALSE;
#endif
}



