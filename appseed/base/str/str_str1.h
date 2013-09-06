#pragma once


typedef int_ptr strsize;

BEGIN_EXTERN_C

CLASS_DECL_c void strcat_dup(char * dest, const char * cat);
CLASS_DECL_c void strcpy_dup(char * dest, const char * cat);
CLASS_DECL_c void strncpy_dup(char * dest, const char * cat, strsize iLen);
CLASS_DECL_c count strlen_dup(const char * cat);
CLASS_DECL_c char * strdup_dup(const char * src);
CLASS_DECL_c char * strndup_dup(const char * src, strsize iLen);
CLASS_DECL_c char * strstr_dup(const char * src, const char * find);
CLASS_DECL_c char * stristr_dup(const char * src, const char * find);
CLASS_DECL_c char * strnstr_dup(const char * src, const char * find, strsize iLen);
CLASS_DECL_c char * strnistr_dup(const char * src, const char * find, strsize iLen);
CLASS_DECL_c int32_t stricmp_dup(const char * sz1, const char * sz2);
CLASS_DECL_c int32_t strcmp_dup(const char * sz1, const char * sz2);
CLASS_DECL_c int32_t strnicmp_dup(const char * sz1, const char * sz2, strsize iLen);
CLASS_DECL_c int32_t strncmp_dup(const char * sz1, const char * sz2, strsize iLen);
CLASS_DECL_c char * str_begins_inc_dup(const char * sz1, const char * prefix);
CLASS_DECL_c int32_t str_begins_dup(const char * sz1, const char * prefix);
CLASS_DECL_c int32_t str_ends_dup(const char * sz1, const char * suffix);
CLASS_DECL_c int32_t str_begins_ci_dup(const char * sz1, const char * prefix);
CLASS_DECL_c char * strrchr_dup(char * sz, char ch);


CLASS_DECL_c void uitoa_dup(char * sz, uint64_t i, int32_t iBase);
CLASS_DECL_c void itoa_dup(char * sz, int64_t i, int32_t iBase);


//CLASS_DECL_c void l64toa_dup(char * sz, int64_t i, int64_t iBase);
//CLASS_DECL_c void i64toa_dup(char * sz, int64_t i);
CLASS_DECL_c void str_reverse(char * sz);
CLASS_DECL_c void zero_pad(char * sz, count iPad);
CLASS_DECL_c int32_t str_ends_ci_dup(const char * psz, const char * pszSuffix);
CLASS_DECL_c char * strchr_dup(const char * sz, char ch);
CLASS_DECL_c char * strpbrk_dup(char * sz, const char * szFind);

CLASS_DECL_c char char_to_lower(int32_t ch);
CLASS_DECL_c char * to_lower(char * pch);
CLASS_DECL_c int32_t isdigit_dup(int32_t ch);
CLASS_DECL_c int32_t isxdigit_dup(int32_t ch);
CLASS_DECL_c int32_t isalpha_dup(int32_t ch);
CLASS_DECL_c int32_t isspace_dup(int32_t ch);

#if defined(LINUX)

CLASS_DECL_c char * strlwr(char * psz);
CLASS_DECL_c char * strupr(char * psz);

#endif

CLASS_DECL_c void wcscat_dup(wchar_t * dest, const wchar_t * cat);
CLASS_DECL_c void wcscpy_dup(wchar_t * dest, const wchar_t * src);
CLASS_DECL_c void wcsncpy_dup(wchar_t * dest, const wchar_t * src, strsize n);
CLASS_DECL_c int32_t iswalnum_dup(int32_t wch);
CLASS_DECL_c count wcslen_dup(const wchar_t * pwsz);
CLASS_DECL_c wchar_t * wcschr_dup(const wchar_t * sz, wchar_t ch);
CLASS_DECL_c int32_t wcscmp_dup(const wchar_t * sz1, const wchar_t * sz2);
CLASS_DECL_c int32_t wcsicmp_dup(const wchar_t * sz1, const wchar_t * sz2);


CLASS_DECL_c void uitow_dup(wchar_t * sz, uint64_t i, int32_t iBase);
CLASS_DECL_c void itow_dup(wchar_t * sz, int64_t i, int32_t iBase);


//CLASS_DECL_c void l64toa_dup(char * sz, int64_t i, int64_t iBase);
//CLASS_DECL_c void i64toa_dup(char * sz, int64_t i);
CLASS_DECL_c void wcs_reverse(wchar_t * sz);
CLASS_DECL_c void w_zero_pad(char * sz, strsize iPad);


CLASS_DECL_c wchar_t w_to_lower(int32_t wch);
CLASS_DECL_c wchar_t w_to_upper(int32_t wch);

CLASS_DECL_c errno_t wcslwr_s_dup(wchar_t * sz, strsize size);
CLASS_DECL_c errno_t wcsupr_s_dup(wchar_t * sz, strsize size);

CLASS_DECL_c int32_t iswalpha_dup(int32_t ch);
CLASS_DECL_c int32_t iswdigit_dup(int32_t ch);
CLASS_DECL_c int32_t iswspace_dup(int32_t ch);


CLASS_DECL_c void __cdecl wparse_cmdline(WCHAR *cmdstart, WCHAR **argv, WCHAR *args, int32_t * numargs, int32_t * numchars);
CLASS_DECL_c void __cdecl parse_cmdline(char *cmdstart, char **argv, char *args, int32_t * numargs, int32_t * numchars);

CLASS_DECL_c char * strtok_r_dup(char *s1, const char *s2, char **lasts);

CLASS_DECL_c count utf16_len(const char * psz);
CLASS_DECL_c void utf8_to_16(wchar_t * pwsz, const char * psz);
//CLASS_DECL_c wchar_t * ::str::international::utf8_to_unicode(const char * psz);
//CLASS_DECL_c char * ::str::international::unicode_to_utf8(const wchar_t * psz);




#if defined(LINUX)

#define _strdup strdup

#endif



CLASS_DECL_c size_t safe_strlen(const char * psz);




CLASS_DECL_c int_bool matches_wildcard_criteria(const char * pszCriteria, const char * pszValue);
CLASS_DECL_c int_bool matches_wildcard_criteria_ci(const char * pszCriteria, const char * pszValue);


END_EXTERN_C