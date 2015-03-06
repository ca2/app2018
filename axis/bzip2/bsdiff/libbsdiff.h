#pragma once

#if defined(_M_IA64) || defined(_M_AMD64)
#define bs_offset long long
#else
#define bs_offset long
#endif





BZ_EXTERN int libbsdiff_diff(bs_offset * bsret,unsigned char *old,bs_offset oldsize,unsigned char *pnew,bs_offset newsize,unsigned char *patch,bs_offset patch_size);
BZ_EXTERN bs_offset libbsdiff_size_of_patched(unsigned char *patch);
//BZ_EXTERN int libbsdiff_patch(unsigned char *old,bs_offset oldsize,unsigned char *patch,bs_offset patch_size,unsigned char *pnew);


BZ_EXTERN int bsdiff(const char * oldfile,const char * newfile,const char * patchfile);
BZ_EXTERN int bspatch(const char * oldfile,const char * newfile,const char * patchfile);


#ifdef WIN32
#include <wchar.h>
typedef INT_PTR         int_ptr,* pint_ptr;
typedef UINT_PTR        uint_ptr,* puint_ptr;
typedef int_ptr         ssize_t;
#endif

