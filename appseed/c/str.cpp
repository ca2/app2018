#include "StdAfx.h"
#include "ca/x/x_defines.h"
#include "ca/x/x_tables.h"
#include "ca/x/x_charcategory_names.h"
#include "ca/x/x_charcategory.h"


void strcat_dup(char * dest, const char * cat)
{
   if(dest == NULL)
      return;
   if(cat == NULL)
      return;
   while(*dest != 0)
      dest++;
   while(*cat != 0)
      *dest++ = *cat++;
   *dest = '\0';
}


int stricmp_dup(const char * sz1, const char * sz2)
{
   if(sz1 == NULL)
   {
      if(sz2 == NULL)
         return 0;
      else
         return -1;
   }
   else if(sz2 == NULL)
   {
      return 1;
   }
   int iCmp = 0;
   while(*sz1 != '\0' && *sz2 != '\0')
   {
      iCmp = to_lower(*sz1) - to_lower(*sz2);
      if(iCmp != 0)
         return iCmp;
      sz1++;
      sz2++;
   }
   return iCmp;
}

int strcmp_dup(const char * sz1, const char * sz2)
{
   if(sz1 == NULL)
   {
      if(sz2 == NULL)
         return 0;
      else
         return -1;
   }
   else if(sz2 == NULL)
   {
      return 1;
   }
   int iCmp;
   while(*sz1 != '\0' && *sz2 != '\0')
   {
      iCmp = *sz1 - *sz2;
      if(iCmp != 0)
         return iCmp;
      sz1++;
      sz2++;
   }
   return *sz1 - *sz2;
}

int strnicmp_dup(const char * sz1, const char * sz2, int iLen)
{
   if(iLen <= 0)
      return 0; // equal
   if(sz1 == NULL)
   {
      if(sz2 == NULL)
         return 0;
      else
         return -1;
   }
   else if(sz2 == NULL)
   {
      return 1;
   }
   int iCmp = 0;
   while(iLen > 0 && *sz1 != '\0' && *sz2 != '\0')
   {
      iCmp = to_lower(*sz1) - to_lower(*sz2);
      if(iCmp != 0)
         return iCmp;
      sz1++;
      sz2++;
      iLen--;
   }
   return iCmp;
}

int strncmp_dup(const char * sz1, const char * sz2, int iLen)
{
   if(iLen <= 0)
      return 0; // equal
   if(sz1 == NULL)
   {
      if(sz2 == NULL)
         return 0;
      else
         return -1;
   }
   else if(sz2 == NULL)
   {
      return 1;
   }
   int iCmp = 0;
   while(iLen > 0 && *sz1 != '\0' && *sz2 != '\0')
   {
      iCmp = *sz1 - *sz2;
      if(iCmp != 0)
         return iCmp;
      sz1++;
      sz2++;
      iLen--;
   }
   return iCmp;
}

char * strrchr_dup(char * szStart, char ch)
{
   char * sz = szStart;
   while(*sz != '\0')
   {
      sz++;
   }
   while(sz != szStart)
   {
      sz--;
      if(*sz == ch)
         return sz;
   }
   return NULL;

}

char to_lower(int ch)
{
   if(ch >= 'A' && ch <= 'Z')
   {
      ch = ch - 'A' + 'a';
   }
   return ch;
}

char * to_lower(char * pchParam)
{
   char * pch = pchParam;
   while(*pch == '\0')
   {
      *pch = to_lower(*pch);
      pch++;
   }
   return pchParam;
}

char * strstr_dup(const char * src, const char * find)
{
   while(*src != '\0')
   {
      const char * srcParse = src;
      const char * findParse = find;
      while(*srcParse != '\0' && *findParse != '\0')
      {
         if(*srcParse != *findParse)
            break;
         srcParse++;
         findParse++;
      }
      if(*findParse == '\0')
         return (char *) src;
      src++;
   }
   return NULL;
}

char * stristr_dup(const char * src, const char * find)
{
   while(*src != '\0')
   {
      const char * srcParse = src;
      const char * findParse = find;
      while(*srcParse != '\0' && *findParse != '\0')
      {
         if(to_lower(*srcParse) != to_lower(*findParse))
            break;
         srcParse++;
         findParse++;
      }
      if(*findParse == '\0')
         return (char *) src;
      src++;
   }
   return NULL;
}


void uitoa_dup(char * sz, unsigned int ui, int iBase)
{
   if(ui == 0)
   {
      sz[0] = '0';
      sz[1] = '\0';
      return;
   }
   int iIndex = 0;
   while(ui > 0)
   {
      int iDigit = (ui % iBase);
      char ch;
      if(iDigit <= 9)
      {
         ch = iDigit + '0';
      }
      else
      {
         ch = iDigit - 10 + 'a';
      }
      sz[iIndex] =  ch;
      ui = ui / iBase;
      iIndex++;
   }
   sz[iIndex] = '\0';
   str_reverse(sz);
}



void itoa_dup(char * sz, int i, int iBase)
{
   if(i == 0)
   {
      sz[0] = '0';
      sz[1] = '\0';
      return;
   }
   bool bNegative = false;
   if(i < 0)
   {
      i = -i;
      bNegative = true;
   }
   int iIndex = 0;
   while(i > 0)
   {
      int iDigit = (i % iBase);
      char ch;
      if(iDigit <= 9)
      {
         ch = iDigit + '0';
      }
      else
      {
         ch = iDigit - 10 + 'a';
      }
      sz[iIndex] =  ch;
      i = i / iBase;
      iIndex++;
   }
   if(bNegative)
   {
      sz[iIndex] = '-';
      iIndex++;
   }
   sz[iIndex] = '\0';
   str_reverse(sz);
}



/*void l64toa_dup(char * sz, __int64 i, __int64 iBase)
{
   if(i == 0)
   {
      sz[0] = '0';
      sz[1] = '\0';
      return;
   }
   bool bNegative = false;
   if(i < 0)
   {
      i = -i;
      bNegative = true;
   }
   __int64 iIndex = 0;
   while(i > 0)
   {
      __int64 iDigit = (i % iBase);
      char ch;
      if(iDigit <= 9)
      {
         ch = iDigit + '0';
      }
      else
      {
         ch = iDigit - 10 + 'a';
      }
      sz[iIndex] =  ch;
      i = i / iBase;
      iIndex++;
   }
   if(bNegative)
   {
      sz[iIndex] = '-';
      iIndex++;
   }
   sz[iIndex] = '\0';
   str_reverse(sz);
}

void i64toa_dup(char * sz, __int64 i)
{
  l64toa_dup(sz, i, 10);
}*/


void str_reverse(char * sz)
{
   int iLen = strlen_dup(sz);
   int iMid = iLen / 2;
   int iL = 0;
   int iR = iLen - 1;
   char ch;
   for(; iL < iMid; iL++, iR--)
   {
      ch = sz[iL];
      sz[iL] = sz[iR];
      sz[iR] = ch;
   }
}

void zero_pad(char * sz, int iPad)
{
   int iLen = strlen_dup(sz);
   int iZeroCount = iPad - iLen;
   if(iZeroCount > 0)
   {
      int iEnd = iLen - 1;
      int iFinalEnd = iEnd + iZeroCount;
      sz[iFinalEnd + 1] = '\0';
      for(; iEnd >= 0; iEnd--, iFinalEnd--)
      {
         sz[iFinalEnd] = sz[iEnd];
      }
      for(; iFinalEnd >= 0; iFinalEnd--)
      {
         sz[iFinalEnd] = '0';
      }
   }
}

char * str_begins_inc_dup(const char * sz1, const char * prefix)
{
   if(sz1 == NULL)
      return NULL;
   if(prefix == NULL)
      return (char *) sz1;
   while(*sz1 != '\0' && *prefix != '\0')
   {
      if(*sz1 != *prefix)
         return NULL;
      sz1++;
      prefix++;
   }
   if(*prefix == '\0')
      return (char *) sz1;
   else
      return NULL;
}


int str_begins_dup(const char * sz1, const char * prefix)
{
   if(sz1 == NULL)
      return FALSE;
   if(prefix == NULL)
      return TRUE;
   while(*sz1 != '\0' && *prefix != '\0')
   {
      if(*sz1 != *prefix)
         return FALSE;
      sz1++;
      prefix++;
   }
   return *prefix == '\0';
}

int str_begins_ci_dup(const char * sz1, const char * prefix)
{
   if(sz1 == NULL)
      return FALSE;
   if(prefix == NULL)
      return TRUE;
   while(*sz1 != '\0' && *prefix != '\0')
   {
      if(*sz1 != *prefix)
         return FALSE;
      sz1++;
      prefix++;
   }
   return *prefix == '\0';
}

int str_ends_ci_dup(const char * psz, const char * pszSuffix)
{
   int iLen = strlen_dup(psz);
   int iLenSuffix = strlen_dup(pszSuffix);
   if(iLen < iLenSuffix)
      return false;
   return stricmp_dup(&psz[iLen - iLenSuffix], pszSuffix) == 0;
}

















#define NULCHAR     '\0'
#define TABCHAR     '\t'
#define SLASHCHAR   '\\'
#define SPACECHAR   ' '
#define DQUOTECHAR  '\"'


void __cdecl parse_cmdline(char *cmdstart, char **argv, char *args, int * numargs, int * numchars)
{
        char *p;
        char c;
        int inquote;                    /* 1 = inside quotes */
        int copychar;                   /* 1 = copy char to *args */
        unsigned numslash;              /* num of backslashes seen */

        *numchars = 0;
        *numargs = 1;                   /* the program name at least */

        /* first scan the program name, copy it, and count the bytes */
        p = cmdstart;
        if (argv)
            *argv++ = args;

#ifdef WILDCARD
        /* To handle later wild card expansion, we prefix each entry by
        it's first character before quote handling.  This is done
        so _[w]cwild() knows whether to expand an entry or not. */
        if (args)
            *args++ = *p;
        ++*numchars;

#endif  /* WILDCARD */

        /* A quoted program name is handled here. The handling is much
           simpler than for other arguments. Basically, whatever lies
           between the leading double-quote and next one, or a terminal null
           character is simply accepted. Fancier handling is not required
           because the program name must be a legal NTFS/HPFS file name.
           Note that the double-quote characters are not copied, nor do they
           contribute to numchars. */
        if ( *p == DQUOTECHAR ) {
            /* scan from just past the first double-quote through the next
               double-quote, or up to a null, whichever comes first */
            while ( (*(++p) != DQUOTECHAR) && (*p != NULCHAR) ) {

/*#ifdef _MBCS
                if (_ismbblead(*p)) {
                    ++*numchars;
                    if ( args )
                        *args++ = *p++;
                }
#endif  /* _MBCS */
                ++*numchars;
                if ( args )
                    *args++ = *p;
            }
            /* append the terminating null */
            ++*numchars;
            if ( args )
                *args++ = NULCHAR;

            /* if we stopped on a double-quote (usual case), skip over it */
            if ( *p == DQUOTECHAR )
                p++;
        }
        else {
            /* Not a quoted program name */
            do {
                ++*numchars;
                if (args)
                    *args++ = *p;

                c = (char) *p++;
/*#ifdef _MBCS
                if (_ismbblead(c)) {
                    ++*numchars;
                    if (args)
                        *args++ = *p;   /* copy 2nd byte too */
    //                p++;  /* skip over trail byte */
  //              }
//#endif  /* _MBCS */

            } while ( c != SPACECHAR && c != NULCHAR && c != TABCHAR );

            if ( c == NULCHAR ) {
                p--;
            } else {
                if (args)
                    *(args-1) = NULCHAR;
            }
        }

        inquote = 0;

        /* loop on each argument */
        for(;;) {

            if ( *p ) {
                while (*p == SPACECHAR || *p == TABCHAR)
                    ++p;
            }

            if (*p == NULCHAR)
                break;              /* end of args */

            /* scan an argument */
            if (argv)
                *argv++ = args;     /* store ptr to arg */
            ++*numargs;

#ifdef WILDCARD
        /* To handle later wild card expansion, we prefix each entry by
        it's first character before quote handling.  This is done
        so _[w]cwild() knows whether to expand an entry or not. */
        if (args)
            *args++ = *p;
        ++*numchars;

#endif  /* WILDCARD */

        /* loop through scanning one argument */
        for (;;) {
            copychar = 1;
            /* Rules: 2N backslashes + " ==> N backslashes and begin/end quote
               2N+1 backslashes + " ==> N backslashes + literal "
               N backslashes ==> N backslashes */
            numslash = 0;
            while (*p == SLASHCHAR) {
                /* count number of backslashes for use below */
                ++p;
                ++numslash;
            }
            if (*p == DQUOTECHAR) {
                /* if 2N backslashes before, start/end quote, otherwise
                    copy literally */
                if (numslash % 2 == 0) {
                    if (inquote) {
                        if (p[1] == DQUOTECHAR)
                            p++;    /* Double quote inside quoted string */
                        else        /* skip first quote char and copy second */
                            copychar = 0;
                    } else
                        copychar = 0;       /* don't copy quote */

                    inquote = !inquote;
                }
                numslash /= 2;          /* divide numslash by two */
            }

            /* copy slashes */
            while (numslash--) {
                if (args)
                    *args++ = SLASHCHAR;
                ++*numchars;
            }

            /* if at end of arg, break loop */
            if (*p == NULCHAR || (!inquote && (*p == SPACECHAR || *p == TABCHAR)))
                break;

            /* copy character into argument */
/*#ifdef _MBCS
            if (copychar) {
                if (args) {
                    if (_ismbblead(*p)) {
                        *args++ = *p++;
                        ++*numchars;
                    }
                    *args++ = *p;
                } else {
                    if (_ismbblead(*p)) {
                        ++p;
                        ++*numchars;
                    }
                }
                ++*numchars;
            }
            ++p;
#else  /* _MBCS */
            if (copychar) {
                if (args)
                    *args++ = *p;
                ++*numchars;
            }
            ++p;
//#endif  /* _MBCS */
            }

            /* null-terminate the argument */

            if (args)
                *args++ = NULCHAR;          /* terminate string */
            ++*numchars;
        }

        /* We put one last argument in -- a null ptr */
        if (argv)
            *argv++ = NULL;
        ++*numargs;
}







CLASS_DECL_c char * strchr_dup(const char * sz, char ch)
{
   if(sz == NULL)
      return NULL;
   while(true)
   {
      if(*sz == ch)
         return (char *) sz;
      if(*sz == '\0')
         return NULL;
      sz++;
   }
}


int isdigit_dup(int ch)
{
   return ch >= '0' && ch <= '9';
}

int isalpha_dup(int ch)
{
   return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

int isspace_dup(int ch)
{
   return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\t';
}



CLASS_DECL_c char * strpbrk_dup(char * sz, const char * szFind)
{
   char * pszRet = NULL;
   while(*szFind != '\0')
   {
      char * psz = strchr_dup(sz, *szFind);
      if(psz != NULL && (pszRet == NULL || psz < pszRet))
      {
         pszRet = psz;
      }
      szFind++;
   }
   return pszRet;
}




vsstring str_replace_dup(const char * psz, const char * pszFind, const char * pszReplace)
{
   vsstring str(psz);
   str.replace(pszFind, pszReplace);
   return str;
}


CLASS_DECL_c vsstring itoa_dup(int i)
{
   char sz[256];
   itoa_dup(sz, i, 10);
   return vsstring(sz);
}

/*CLASS_DECL_c vsstring i64toa_dup(__int64 i)
{
   char sz[512];
   i64toa_dup(sz, i);
   return vsstring(sz);
}*/

#ifdef _WINDOWS

vsstring read_resource_as_string_dup(HINSTANCE hinst, UINT nID, LPCTSTR lpcszType)
{

   HRSRC hrsrc = ::FindResource(hinst, MAKEINTRESOURCE(nID), lpcszType);

   if(hrsrc == NULL)
		return "";

   HGLOBAL hres = ::LoadResource(hinst, hrsrc);

   if(hres == NULL)
		return "";

   DWORD dwResSize = ::SizeofResource(hinst, hrsrc);

   char * psz = NULL;

	if(hres != NULL)
   {

      UINT FAR* lpnRes = (UINT FAR*)::LockResource(hres);

      psz = (char *) ca2_alloc(dwResSize + 1);
      memcpy_dup(psz, lpnRes, dwResSize);
      psz[dwResSize] = '\0';
      ::FreeResource(hres);
	}
	vsstring vss;
   vss.attach(psz);
   return vss;

}


#endif


CLASS_DECL_c char * strtok_r_dup(char *s1, const char *s2, char **lasts)
{
  char *ret;

  if (s1 == NULL)
    s1 = *lasts;
  while(*s1 && strchr_dup(s2, *s1))
    ++s1;
  if(*s1 == '\0')
    return NULL;
  ret = s1;
  while(*s1 && !strchr_dup(s2, *s1))
    ++s1;
  if(*s1)
    *s1++ = '\0';
  *lasts = s1;
  return ret;
}




