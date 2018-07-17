#include "framework.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>


int_bool file_exists_raw(const char * path1)
{

#ifdef WINDOWS

   uint32_t dwFileAttributes = GetFileAttributesA(path1);

   if (dwFileAttributes == INVALID_FILE_ATTRIBUTES || (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
      return FALSE;

   return TRUE;

#else


   // dedicaverse stat -> Sir And Arthur - Cesar Serenato

   struct stat st;

   if(stat(path1, &st))
      return false;

   if((st.st_mode & S_IFDIR))
      return false;

   return true;

#endif

}

void file_put_contents_raw(const char * path, const char * psz)
{

   FILE * f = fopen(path, "wb");

   if (f == NULL)
      return;

   ::count iSize = strlen(psz);

   ::count iRead = fwrite(psz, 1, iSize, f);

   fclose(f);

}

void file_add_contents_raw(const char * path, const char * psz)
{

   FILE * f = fopen(path, "ab");

   if (f == NULL)
      return;

   ::count iSize = strlen(psz);

   ::count iRead = fwrite(psz, 1, iSize, f);

   fclose(f);

}


void file_beg_contents_raw(const char * path, const char * psz)
{

   FILE * f = fopen(path, "rb+");

   long lLen;

   lLen = (long) ( strlen(psz));

   fseek(f, lLen, SEEK_END);

   long lEnd = ftell(f);

   long lSize = 1024 * 1024;

   char * buf = (char *) malloc(lSize);

   long lRemain = lEnd - lLen;

   while (lRemain > 0)
   {

      fseek(f, lEnd - lRemain - lLen, SEEK_SET);

      fread(buf, 1, MIN(lRemain, lSize), f);

      fseek(f, lEnd - lRemain, SEEK_SET);

      fwrite(buf, 1, MIN(lRemain, lSize), f);

      lRemain -= MIN(lRemain, lSize);

   }

   free(buf);

   fseek(f, 0, SEEK_SET);

   auto lRead = fwrite(psz, 1, lLen, f);

   if (lRead != lLen)
   {

      output_debug_string("file_beg_contents_raw: error writing to file.");

   }

   fclose(f);

}


uint64_t file_length_raw(const char * path)
{

#ifdef WINDOWS
   struct _stat st;
   ::_stat(path, &st);
   return st.st_size;

#else
   struct stat st;
   ::stat(path, &st);
   return st.st_size;

#endif

}




string file_line_dup(const char * path, index iLine)
{

   string str;

   FILE * file = ::fopen_dup(path, "r", _SH_DENYNO);

   if (file == NULL)
   {

      DWORD dw = ::get_last_error();

      return "";

   }

   int iChar;

   string strLine;

   int iLastChar = -1;

   while (iLine >= 0)
   {

      iChar = fgetc(file);

      if (iChar == EOF)
      {

         break;

      }

      if (iChar == '\r')
      {

         iLine--;

      }
      else if (iChar == '\n')
      {

         if (iLastChar != '\r')
         {

            iLine--;

         }

      }
      else if (iLine == 0)
      {

         str += (char)iChar;

      }

      iLastChar = iChar;

   }

   return str;

}


bool file_set_line_dup(const char * path, index iLine, const char * pszLine)
{

   if (iLine < 0)
   {

      return false;

   }

   string str;

   FILE * file = ::fopen_dup(path, "a+", _SH_DENYWR);

   if (file == NULL)
   {

      DWORD dw = ::get_last_error();

      return "";

   }

   int iChar;

   string strLine;

   int iLastChar = -1;

   index iPosStart = -1;

   index iPosEnd = -1;

   while (iLine >= 0)
   {

      iChar = fgetc(file);

      if (iChar == EOF)
      {

         break;

      }

      if (iChar == '\r')
      {

         iLine--;

      }
      else if (iChar == '\n')
      {

         if (iLastChar != '\r')
         {

            iLine--;

         }

      }
      else if (iLine == 0)
      {

         if (iPosStart <= 0)
         {

            iPosStart = ftell(file);

         }

      }

      iLastChar = iChar;

   }

   if (iLine > 0)
   {

      fwrite("\n", 1, iLine, file);

      fwrite(pszLine, 1, strlen(pszLine), file);

      fclose(file);

   }
   else
   {

      iPosEnd = ftell(file);

      ::file::path pathTime = path;

      pathTime += ".time";

      FILE * file2 = ::fopen_dup(pathTime, "w", _SH_DENYWR);

      if (iPosStart > 0)
      {

         memory m;

         fseek(file, 0, SEEK_SET);

         m.allocate(iPosStart);

         fread(m.get_data(), 1, iPosStart, file);

         fwrite(m.get_data(), 1, iPosStart, file2);

      }

      fwrite(pszLine, 1, strlen(pszLine), file2);

      index iEnd = fseek(file, 0, SEEK_END);

      if (iEnd - iPosEnd > 0)
      {

         memory m;

         fseek(file, (long) iPosEnd, SEEK_SET);

         m.allocate(iEnd - iPosEnd);

         fread(m.get_data(), 1, m.get_size(), file);

         fwrite(m.get_data(), 1, m.get_size(), file2);

      }

      fclose(file2);

      fclose(file);

#ifdef WINDOWS

      if (!file_copy_dup(path, pathTime.c_str(), true))
      {

         return false;

      }

      file_delete_dup(pathTime);

#else

      ::system("mv -f \"" + string(pathTime) + "\" \"" + string(path) + "\"");

#endif

   }

   return true;

}
