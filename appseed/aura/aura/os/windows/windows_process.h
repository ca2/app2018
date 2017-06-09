#pragma once


CLASS_DECL_AURA void call_sync(const char * path, const char * param);

CLASS_DECL_AURA bool process_modules(stringa & stra, uint32_t processID);

CLASS_DECL_AURA bool load_modules_diff(stringa & straOld, stringa & straNew, const char * pszExceptDir);

CLASS_DECL_AURA bool are_dlls_in_use(const stringa & straDll);

CLASS_DECL_AURA bool are_dlls_in_use(uint32_t processid, const stringa & straDll);



