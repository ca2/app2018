#include "framework.h"


namespace windows
{


   file_find::file_find()
   {

      m_pFoundInfo = NULL;
      m_pNextInfo = NULL;
      m_hContext = NULL;
      m_chDirSeparator = '\\';

   }


   file_find::~file_find()
   {

      close();

   }


   void file_find::close()
   {

      //::aura::del(m_pFoundInfo);

      //::aura::del(m_pNextInfo);

      CloseContext();

      m_hContext = NULL;

   }


   void file_find::CloseContext()
   {

      if(m_hContext != NULL && m_hContext != INVALID_HANDLE_VALUE)
      {

         ::FindClose(m_hContext);

         m_hContext = NULL;

      }

   }


   bool file_find::FindFile(const char * pstrName /* = NULL */, DWORD dwUnused /* = 0 */)
   {

      UNUSED_ALWAYS(dwUnused);

      close();

      if (m_pNextInfo == NULL)
      {

         m_pNextInfo = &m_dataNext;

      }

      m_bGotLast = false;

      if (pstrName == NULL)
         pstrName = "*.*";

      ::str::international::MultiByteToUnicode(CP_UTF8, m_pNextInfo->cFileName, MAX_PATH, pstrName);

      wstring wstrName = ::str::international::utf8_to_unicode(pstrName);

      m_hContext = ::windows::shell::FindFirstFile(wstrName, m_pNextInfo);

      if (m_hContext == INVALID_HANDLE_VALUE)
      {

         DWORD dwTemp = ::get_last_error();

         close();

         ::set_last_error(dwTemp);

         return false;

      }

      wstring wstrRoot;

      unichar * pstrRoot = wstrRoot.alloc(_MAX_PATH);

      const unichar * pstr = ::windows::shell::FullPath(pstrRoot, wstrName, _MAX_PATH);

      // passed name isn't a valid path but was found by the API

      ASSERT(pstr != NULL);

      if (pstr == NULL)
      {

         m_strRoot.Empty();

         close();

         ::set_last_error(ERROR_INVALID_NAME);

         return false;
      }
      else
      {

         // find the last forward or backward whack

         unichar * pstrBack  = wcsrchr(pstrRoot, '\\');

         unichar * pstrFront = wcsrchr(pstrRoot, '/');

         if (pstrFront != NULL || pstrBack != NULL)
         {

            if (pstrFront == NULL)
               pstrFront = pstrRoot;

            if (pstrBack == NULL)
               pstrBack = pstrRoot;

            // from the start to the last whack is the root

            if (pstrFront >= pstrBack)
               *pstrFront = '\0';
            else
               *pstrBack = '\0';

         }

         wstrRoot.release_buffer();

      }

      m_strRoot = ::str::international::unicode_to_utf8(wstrRoot);

      return TRUE;

   }


   bool file_find::GetLastAccessTime(FILETIME* pTimeStamp) const
   {

      ASSERT(m_hContext != NULL);
      ASSERT(pTimeStamp != NULL);
      ASSERT_VALID(this);

      if (m_pFoundInfo != NULL && pTimeStamp != NULL)
      {
         *pTimeStamp = m_pFoundInfo->ftLastAccessTime;
         return TRUE;
      }
      else
         return false;

   }

   bool file_find::GetLastWriteTime(FILETIME* pTimeStamp) const
   {

      ASSERT(m_hContext != NULL);
      ASSERT(pTimeStamp != NULL);
      ASSERT_VALID(this);

      if (m_pFoundInfo != NULL && pTimeStamp != NULL)
      {
         *pTimeStamp = m_pFoundInfo->ftLastWriteTime;
         return TRUE;
      }
      else
         return false;

   }


   bool file_find::GetCreationTime(FILETIME* pTimeStamp) const
   {

      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      if (m_pFoundInfo != NULL && pTimeStamp != NULL)
      {
         *pTimeStamp = m_pFoundInfo->ftCreationTime;
         return TRUE;
      }
      else
         return false;

   }


   bool file_find::GetLastAccessTime(::datetime::time& refTime) const
   {

      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      if (m_pFoundInfo != NULL)
      {
         refTime = ::datetime::time(m_pFoundInfo->ftLastAccessTime);
         return TRUE;
      }
      else
         return false;

   }


   bool file_find::GetLastWriteTime(::datetime::time& refTime) const
   {

      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      if (m_pFoundInfo != NULL)
      {
         refTime = ::datetime::time(m_pFoundInfo->ftLastWriteTime);
         return TRUE;
      }
      else
         return false;

   }

   bool file_find::GetCreationTime(::datetime::time& refTime) const
   {
      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      if (m_pFoundInfo != NULL)
      {
         refTime = ::datetime::time(((LPWIN32_FIND_DATAW) m_pFoundInfo)->ftCreationTime);
         return TRUE;
      }
      else
         return false;
   }


   bool file_find::IsDots() const
   {

      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      // return TRUE if the file name is "." or ".." and
      // the file is a directory

      bool bResult = false;

      if (m_pFoundInfo != NULL && IsDirectory())
      {

         LPWIN32_FIND_DATAW pFindData = m_pFoundInfo;

         if (pFindData->cFileName[0] == '.')
         {

            if (pFindData->cFileName[1] == '\0' ||
                  (pFindData->cFileName[1] == '.' &&
                   pFindData->cFileName[2] == '\0'))
            {

               bResult = TRUE;

            }

         }

      }

      return bResult;

   }


   bool file_find::FindNextFile()
   {

      //ASSERT(m_hContext != NULL);

      if (m_hContext == NULL)
         return false;

      if (m_pFoundInfo == NULL)
      {

         m_pFoundInfo = &m_dataFound;

      }

      //ASSERT_VALID(this);

      WIN32_FIND_DATAW * pTemp = m_pFoundInfo;

      m_pFoundInfo = m_pNextInfo;

      m_pNextInfo = pTemp;

      return ::FindNextFileW(m_hContext, m_pNextInfo);

   }


   string file_find::GetFileURL() const
   {

      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      string strResult(L"file://");

      strResult += GetFilePath();

      return strResult;

   }


   string file_find::GetRoot() const
   {

      ASSERT(m_hContext != NULL);

      ASSERT_VALID(this);

      return m_strRoot;

   }


   ::file::path file_find::GetFilePath() const
   {

      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      string strResult = GetRoot();

      if (strResult[strResult.get_length()-1] != '\\' &&
            strResult[strResult.get_length()-1] != '/')
         strResult += m_chDirSeparator;

      strResult += GetFileName();

      return strResult;

   }


   string file_find::GetFileTitle() const
   {

      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      string strFullName = GetFileName();

      string strResult;

      _splitpath(strFullName, NULL, NULL, strResult.GetBuffer(_MAX_PATH), NULL);

      strResult.ReleaseBuffer();

      return strResult;

   }

   ::file::path file_find::GetFileName() const
   {

      //ASSERT(m_hContext != NULL);
      //ASSERT_VALID(this);

      if (m_pFoundInfo == NULL)
      {

         return ::file::path();

      }

      strsize iLen = wcslen(m_pFoundInfo->cFileName);

      strsize i = iLen - 1;

      while (i >= 0 && m_pFoundInfo->cFileName[i] == '\\')
      {

         i--;

      }

      if (i < 0)
      {

         i = 0;

      }

      return ::file::path(m_pFoundInfo->cFileName, i + 1, ::file::path_file, IsDirectory() ? 1 : 0, false, get_length());

   }




   void file_find::dump(dump_context & dumpcontext) const
   {

      object::dump(dumpcontext);

      dumpcontext << "\nm_hContext = " << m_hContext;

   }


   void file_find::assert_valid() const
   {

      // if you trip the ASSERT in the else side, you've called
      // a get() function without having done at least one
      // FindNext() call

      if (m_hContext == NULL)
         ASSERT(m_pFoundInfo == NULL && m_pNextInfo == NULL);
      else
         ASSERT(m_pFoundInfo != NULL && m_pNextInfo != NULL);

   }

} // namespace windows




