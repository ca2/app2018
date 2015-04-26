//#include "framework.h"
//#include "windows.h"


//#include <errno.h>
//#include <io.h>
//#include <fcntl.h>


namespace windows
{


   stdio_file::stdio_file(::aura::application * papp) :
      object(papp),
      ::windows::file(papp)
   {

      m_pStream = NULL;

   }


   stdio_file::~stdio_file()
   {

      if (m_pStream != NULL)
         close();

   }


   cres stdio_file::open(const ::file::path & lpszFileName, UINT nOpenFlags)
   {

      ASSERT(lpszFileName != NULL);

      ASSERT(__is_valid_string(lpszFileName));

      if(nOpenFlags  & ::file::defer_create_directory)
      {
         ::dir::mk(::dir::name(lpszFileName));
      }

      m_pStream = NULL;
      if(!::windows::file::open(lpszFileName,(nOpenFlags & ~::file::type_text)))
      {

         return failure;

      }

      ASSERT(m_hFile != hFileNull);

      char szMode[4]; // C-runtime open string
      int32_t nMode = 0;

      // determine read/write mode depending on ::file::binary_buffer_sp mode
      if (nOpenFlags & ::file::mode_create)
      {
         if (nOpenFlags & ::file::mode_no_truncate)
            szMode[nMode++] = 'a';
         else
            szMode[nMode++] = 'w';
      }
      else if (nOpenFlags & ::file::mode_write)
         szMode[nMode++] = 'a';
      else
         szMode[nMode++] = 'r';

      // add '+' if necessary (when read/write modes mismatched)
      if (szMode[0] == 'r' && (nOpenFlags & ::file::mode_read_write) ||
         szMode[0] != 'r' && !(nOpenFlags & ::file::mode_write))
      {
         // ::file::seek_current szMode mismatched, need to add '+' to fix
         szMode[nMode++] = '+';
      }

      // will be inverted if not necessary
      int32_t nFlags = _O_RDONLY|_O_TEXT;
      if (nOpenFlags & (::file::mode_write|::file::mode_read_write))
         nFlags ^= _O_RDONLY;

      if (nOpenFlags & ::file::type_binary)
         szMode[nMode++] = 'b', nFlags ^= _O_TEXT;
      else
         szMode[nMode++] = 't';
      szMode[nMode++] = '\0';

      // open a C-runtime low-level file handle
      int32_t nHandle = _open_osfhandle(m_hFile, nFlags);

      // open a C-runtime stream from that handle
      if (nHandle != -1)
         m_pStream = _fdopen(nHandle, szMode);

      if (m_pStream == NULL)
      {
         // an error somewhere along the way...
         //      if (pException != NULL)
         //    {
         //         pException->m_lOsError = _doserrno;
         //         pException->m_cause = ::file::exception::OsErrorToException(_doserrno);
         //  }

         ::windows::file::Abort(); // close m_hFile

         return failure;

      }

      return ::no_exception;
   }

   ::primitive::memory_size stdio_file::read(void * lpBuf, ::primitive::memory_size nCount)
   {
      ASSERT_VALID(this);
      ASSERT(m_pStream != NULL);

      if (nCount == 0)
         return 0;   // avoid Win32 "null-read"

      ASSERT(__is_valid_address(lpBuf, nCount));

      primitive::memory_size nRead = 0;

      if ((nRead = fread(lpBuf, sizeof(BYTE), nCount, m_pStream)) == 0 && !feof(m_pStream))
         throw_file_exception(get_app(), ::file::exception::type_generic, _doserrno, m_strFileName);
      if (ferror(m_pStream))
      {
         clearerr(m_pStream);
         throw_file_exception(get_app(), ::file::exception::type_generic, _doserrno, m_strFileName);
      }
      return nRead;
   }

   void stdio_file::write(const void * lpBuf, ::primitive::memory_size nCount)
   {
      ASSERT_VALID(this);
      ASSERT(m_pStream != NULL);
      ASSERT(__is_valid_address(lpBuf, nCount, FALSE));

      if (fwrite(lpBuf, sizeof(BYTE), nCount, m_pStream) != nCount)
         throw_file_exception(get_app(), ::file::exception::type_generic, _doserrno, m_strFileName);
   }

   void stdio_file::write_string(const char * lpsz)
   {
      ASSERT(lpsz != NULL);
      ASSERT(m_pStream != NULL);

      if (_fputts(lpsz, m_pStream) == _TEOF)
         throw_file_exception(get_app(), ::file::exception::diskFull, _doserrno, m_strFileName);
   }

   LPTSTR stdio_file::read_string(LPTSTR lpsz, UINT nMax)
   {
      ASSERT(lpsz != NULL);
      ASSERT(__is_valid_address(lpsz, nMax));
      ASSERT(m_pStream != NULL);

      LPTSTR lpszResult = _fgetts(lpsz, nMax, m_pStream);
      if (lpszResult == NULL && !feof(m_pStream))
      {
         clearerr(m_pStream);
         throw_file_exception(get_app(), ::file::exception::type_generic, _doserrno, m_strFileName);
      }
      return lpszResult;
   }

   bool stdio_file::read_string(string & rString)
   {
      ASSERT_VALID(this);

      //rString = &afxWchNil;    // is_empty string without deallocating
      rString.Empty();
      const int32_t nMaxSize = 128;
      char * lpsz = rString.GetBuffer(nMaxSize);
      char * lpszResult;
      ::primitive::memory_size nLen = 0;
      for (;;)
      {
         lpszResult = fgets(lpsz, nMaxSize+1, m_pStream);
         rString.ReleaseBuffer();

         // handle error/eof case
         if (lpszResult == NULL && !feof(m_pStream))
         {
            clearerr(m_pStream);
            throw_file_exception(get_app(), ::file::exception::type_generic, _doserrno,
               m_strFileName);
         }

         // if string is read completely or EOF
         if (lpszResult == NULL ||
            (nLen = lstrlen(lpsz)) < nMaxSize ||
            lpsz[nLen-1] == '\n')
            break;

         nLen = rString.get_length();
         lpsz = rString.GetBuffer(nMaxSize + nLen) + nLen;
      }

      // remov '\n' from ::file::seek_end of string if present
      lpsz = rString.GetBuffer(0);
      nLen = rString.get_length();
      if (nLen != 0 && lpsz[nLen-1] == '\n')
         rString.GetBufferSetLength(nLen-1);

      return lpszResult != NULL;
   }

   /*void stdio_file::write_string(const char * lpsz)
   {
   ASSERT(lpsz != NULL);
   ASSERT(m_pStream != NULL);

   if (fputws(lpsz, m_pStream) == _TEOF)
   throw_file_exception(get_app(), ::file::exception::diskFull, _doserrno, m_strFileName);
   }*/

   /*wchar_t * stdio_file::read_string(wchar_t * lpsz, UINT nMax)
   {
   ASSERT(lpsz != NULL);
   ASSERT(__is_valid_address(lpsz, nMax));
   ASSERT(m_pStream != NULL);

   wchar_t * lpszResult = fgetws(lpsz, nMax, m_pStream);
   if (lpszResult == NULL && !feof(m_pStream))
   {
   clearerr(m_pStream);
   throw_file_exception(get_app(), ::file::exception::generic, _doserrno, m_strFileName);
   }
   return lpszResult;
   }*/

   file_position stdio_file::seek(file_offset lOff, ::file::e_seek nFrom)
   {
      ASSERT_VALID(this);
      ASSERT(nFrom == ::file::seek_begin || nFrom == ::file::seek_end || nFrom == ::file::seek_current);
      ASSERT(m_pStream != NULL);

      if (fseek(m_pStream, (long) lOff, nFrom) != 0)
         throw_file_exception(get_app(), ::file::exception::badSeek, _doserrno,
         m_strFileName);

      long pos = ftell(m_pStream);
      return pos;
   }

   file_position stdio_file::get_position() const
   {
      ASSERT_VALID(this);
      ASSERT(m_pStream != NULL);

      long pos = ftell(m_pStream);
      if (pos == -1)
         throw_file_exception(get_app(), ::file::exception::invalidFile, _doserrno,
         m_strFileName);
      return pos;
   }

   void stdio_file::Flush()
   {
      ASSERT_VALID(this);

      if (m_pStream != NULL && fflush(m_pStream) != 0)
         throw_file_exception(get_app(), ::file::exception::diskFull, _doserrno,
         m_strFileName);
   }

   void stdio_file::close()
   {
      ASSERT_VALID(this);
      ASSERT(m_pStream != NULL);

      int32_t nErr = 0;

      if (m_pStream != NULL)
         nErr = fclose(m_pStream);

      m_hFile = (UINT) hFileNull;
      m_pStream = NULL;

      if (nErr != 0)
         throw_file_exception(get_app(), ::file::exception::diskFull, _doserrno,
         m_strFileName);
   }


   void stdio_file::Abort()
   {

      ASSERT_VALID(this);

      if (m_pStream != NULL)
         fclose(m_pStream);  // close but ignore errors

      m_hFile = (UINT) hFileNull;

      m_pStream = NULL;

   }


   ::file::buffer_sp  stdio_file::Duplicate() const
   {

      ASSERT_VALID(this);

      ASSERT(m_pStream != NULL);

      throw not_supported_exception(get_app());
      return NULL;
   }

   void stdio_file::LockRange(file_position /* dwPos */, file_size /* dwCount */)
   {
      ASSERT_VALID(this);
      ASSERT(m_pStream != NULL);

      throw not_supported_exception(get_app());
   }

   void stdio_file::UnlockRange(file_position /* dwPos */, file_size /* dwCount */)
   {
      ASSERT_VALID(this);
      ASSERT(m_pStream != NULL);

      throw not_supported_exception(get_app());
   }


   void stdio_file::dump(dump_context & dumpcontext) const
   {
      ::windows::file::dump(dumpcontext);

      dumpcontext << "m_pStream = " << (void *)m_pStream;
      dumpcontext << "\n";
   }




   file_size stdio_file::get_length() const
   {
      ASSERT_VALID(this);

      LONG nCurrent;
      LONG nLength;
      LONG nResult;

      nCurrent = ftell(m_pStream);
      if (nCurrent == -1)
         throw_file_exception(get_app(), ::file::exception::invalidFile, _doserrno,
         m_strFileName);

      nResult = fseek(m_pStream, 0, SEEK_END);
      if (nResult != 0)
         throw_file_exception(get_app(), ::file::exception::badSeek, _doserrno,
         m_strFileName);

      nLength = ftell(m_pStream);
      if (nLength == -1)
         throw_file_exception(get_app(), ::file::exception::invalidFile, _doserrno,
         m_strFileName);
      nResult = fseek(m_pStream, nCurrent, SEEK_SET);
      if (nResult != 0)
         throw_file_exception(get_app(), ::file::exception::badSeek, _doserrno,
         m_strFileName);

      return nLength;
   }


} // namespace windows



