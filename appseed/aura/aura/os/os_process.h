#pragma once


#if !defined(METROWIN)

CLASS_DECL_AURA int32_t call_async(
   const char * pszPath,
   const char * pszParam,
   const char * pszDir,
   int32_t iShow,
   bool bPrivileged);

CLASS_DECL_AURA uint32_t call_sync(
   const char * pszPath,
   const char * pszParam,
   const char * pszDir,
   int32_t iShow,
   int32_t iRetry,
   int32_t iSleep,
   int32_t (* pfnOnRetry)(int32_t iTry, uint_ptr dwParam),
   uint_ptr dwParam);

#endif


#if !defined(METROWIN) && !defined(LINUX) && !defined(APPLEOS)

CLASS_DECL_AURA int32_t get_current_processor_index();

CLASS_DECL_AURA int32_t get_current_process_maximum_affinity();

CLASS_DECL_AURA int32_t get_current_process_affinity_order();

#endif




CLASS_DECL_AURA string consume_param(const char * pszCommandLine, const char ** pszEndPtr);
CLASS_DECL_AURA string get_command_line_param(const char * pszCommandLine, const char * pszParam, const char * pszIfParamValue, const char * pszReplaceParam);
CLASS_DECL_AURA bool get_command_line_param(string & strValue, const char * pszCommandLine, const char * pszParam);
CLASS_DECL_AURA string get_command_line_param2(const char * pszCommandLine,const char * pszParam);



CLASS_DECL_AURA string process_platform_dir_name();
CLASS_DECL_AURA string process_version_dir_name();