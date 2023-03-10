#include "framework.h"

#include <stdio.h>







FILE *fopen_dup(const char *path, const char *attrs, int iShare)
{

#if defined(WINDOWS)

   return _wfsopen(wstring(path), wstring(attrs), iShare);

#else

   return fopen(path, attrs);

#endif

}



int32_t fclose_dup(FILE *fp)
{


   return fclose(fp);
}

int32_t feof_dup(FILE *fp)
{


   return feof(fp);


}

int32_t fflush_dup(FILE * fp)
{

   return fflush(fp);

}

file_position_t fseek_dup(FILE *fp, file_offset_t offset, int32_t origin)
{

   return fseek(fp, (long) (offset), origin);

}

long ftell_dup(FILE *fp)
{


   return ftell(fp);
}

size_t fread_dup(void *buffer, size_t size, size_t count, FILE *str)
{


   return fread(buffer, size, count, str);

}

size_t fwrite_dup(const void *buffer, size_t size, size_t count, FILE *str)
{


   return fwrite(buffer, size, count, str);


}

char *fgets_dup(char *str, int32_t n, FILE *s)
{

   return fgets(str, n, s);


}



int32_t fgetc_dup(FILE *s)
{
   if (s == 0 || feof_dup(s))
      return EOF;

   uchar c;
   fread_dup(&c, 1, sizeof(uchar), s);

   return (int32_t)c;
}

int32_t ungetc_dup(int32_t c, FILE *s)
{
   if (s == 0)
      return EOF;

   fseek_dup(s, -1, SEEK_CUR);

   return (int32_t)c;
}


int32_t ferror_dup(FILE *fp)
{


   return ferror(fp);


}


uint64_t fsize_dup(FILE * fp)
{

   auto pos = ftell(fp);

   fseek(fp, 0, SEEK_END);

   auto len = ftell(fp);

   fseek(fp, (long) (pos), SEEK_SET);

   return len;

}


uint64_t flen_dup(FILE *str)
{


   return fsize_dup(str);


}



#if !defined(__APPLE__) && !defined(WINDOWS)

::file::path __node_full_file_path(file::path path)
{

   hstring str(PATH_MAX+1);

   return realpath(path, str);

}


#endif


int32_t file_touch_dup(const char * psz)
{

   FILE * pfile = ::fopen(psz, "a");

   if (pfile == NULL)
   {

      return FALSE;

   }

   ::fclose(pfile);

   return TRUE;

}



