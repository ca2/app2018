#include "framework.h"

#include <crt_externs.h>
CLASS_DECL_BASE int32_t ca2_main();

//CLASS_DECL_BOOT extern fixed_alloc_array * g_pfixedallocaWstring;

//CLASS_DECL_BOOT fixed_alloc_array * new_wstring_manager();

void CLASS_DECL_BASE __cdecl _ca2_purecall()
{
   throw new simple_exception(get_app());
}


void __cdecl _null_se_translator(uint32_t uiCode, EXCEPTION_POINTERS * ppointers);





void __cdecl _null_se_translator(uint32_t uiCode, EXCEPTION_POINTERS * ppointers)
{
   UNREFERENCED_PARAMETER(uiCode);
   UNREFERENCED_PARAMETER(ppointers);
}






