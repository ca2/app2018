#pragma once


CLASS_DECL_____ bool file_exists_dup(const char * path1);
#ifdef WINDOWS
CLASS_DECL_____ bool read_resource_as_file_dup(const char * pszFile, HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);
#endif
CLASS_DECL_____ const char * get_file_md5(const char * file);
CLASS_DECL_____ bool is_file_ok(const char * path1, const char * pszTemplate);
CLASS_DECL_____ bool file_put_contents_dup(const char * path, const char * contents, int len = -1);
CLASS_DECL_____ const char * file_get_contents_dup(const char * path);
CLASS_DECL_____ bool get_temp_file_name_dup(char * szRet, int iBufferSize, const char * pszName, const char * pszExtension);
CLASS_DECL_____ bool get_temp_file_name_template(char * szRet, int iBufferSize, const char * pszName, const char * pszExtension, const char * pszTemplate);
CLASS_DECL_____ uint64_t file_length_dup(const char * path);

CLASS_DECL_____ vsstring file_title_dup(const char * path);
CLASS_DECL_____ vsstring file_name_dup(const char * path);
CLASS_DECL_____ vsstring file_module_path_dup();


CLASS_DECL_____ bool file_ftd_dup(const char * pszDir, const char * pszFile);
#ifdef WINDOWS
CLASS_DECL_____ void file_read_n_number_dup(HANDLE hfile, ::md5::md5 * pctx, int & iNumber);
CLASS_DECL_____ void file_read_ex1_string_dup(HANDLE hfile, ::md5::md5 * pctx, vsstring & str);
#else
CLASS_DECL_____ void file_read_n_number_dup(FILE * hfile, ::md5::md5 * pctx, int & iNumber);
CLASS_DECL_____ void file_read_ex1_string_dup(FILE * hfile, ::md5::md5 * pctx, vsstring & str);
#endif


CLASS_DECL_____ bool PrintModules(vsstring & strImage, DWORD processID, const char * pszDll );
CLASS_DECL_____ void dll_processes(simple_uint_array & dwa, stra_dup & straProcesses, const char * pszDll);


CLASS_DECL_____ bool file_copy_dup(const char * pszDup, const char * pszSrc, bool bOverwrite = true);
