#pragma once


#if !defined(METROWIN)

CLASS_DECL_c int32_t call_async(
   const char * pszPath,
   const char * pszParam,
   const char * pszDir,
   int32_t iShow);

CLASS_DECL_c uint32_t call_sync(
   const char * pszPath,
   const char * pszParam,
   const char * pszDir,
   int32_t iShow,
   int32_t iRetry,
   int32_t iSleep,
   int32_t (* pfnOnRetry)(int32_t iTry, uint_ptr dwParam),
   uint_ptr dwParam);

#endif


#if !defined(METROWIN) && !defined(LINUX) && !defined(MACOS)

CLASS_DECL_c int32_t get_current_processor_index();

CLASS_DECL_c int32_t get_current_process_maximum_affinity();

CLASS_DECL_c int32_t get_current_process_affinity_order();

#endif




CLASS_DECL_c vsstring consume_param(const char * pszCommandLine, const char ** pszEndPtr);
CLASS_DECL_c vsstring get_command_line_param(const char * pszCommandLine, const char * pszParam, const char * pszIfParamValue, const char * pszReplaceParam);
CLASS_DECL_c vsstring get_command_line_param(const char * pszCommandLine, const char * pszParam);


CLASS_DECL_c bool os_initialize();
CLASS_DECL_c bool os_finalize();


CLASS_DECL_c bool main_initialize();
CLASS_DECL_c bool main_finalize();


