#pragma once


CLASS_DECL_ACE void call_sync(const char * path, const char * param);


CLASS_DECL_ACE bool process_modules(stringa & stra, uint32_t processID);

   
CLASS_DECL_ACE bool load_modules_diff(stringa & straOld, stringa & straNew, const char * pszExceptDir);


