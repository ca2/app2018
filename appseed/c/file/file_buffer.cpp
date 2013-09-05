#include "framework.h"


namespace file
{


   buffer::buffer()
   {
   }



   buffer::~buffer()
   {
   }

   sp(::file::buffer) buffer::Duplicate() const
   {
      return NULL;
   }

   bool buffer::open(const char * lpszFileName, UINT nOpenFlags)
   {
      UNREFERENCED_PARAMETER(lpszFileName);
      UNREFERENCED_PARAMETER(nOpenFlags);
      return FALSE;
   }

   file_position buffer::seek(file_offset lOff, ::file::e_seek nFrom)
   {
      UNREFERENCED_PARAMETER(lOff);
      UNREFERENCED_PARAMETER(nFrom);
      return 0;
   }

   file_position buffer::get_position() const
   {
      return ((buffer *) this)->seek(0, ::file::seek_current);
   }

   void buffer::flush()
   {
   }

   void buffer::close()
   {
   }

   ::primitive::memory_size buffer::read(void *lpBuf, ::primitive::memory_size nCount)
   {
      return ::file::reader::read(lpBuf, nCount);
   }

   void buffer::write(const void * lpBuf, ::primitive::memory_size nCount)
   {
      ::file::writer::write(lpBuf, nCount);
   }


   void buffer::write(output_stream & ostream)
   {
      
      seek_to_begin();

      write_to(ostream.m_spwriter);

   }


   void buffer::read(input_stream & istream)
   {
   
      read_from(istream.m_spreader);

      seek_to_begin();

   }


   void buffer::Abort()
   {
   }

   void buffer::LockRange(file_position dwPos, file_size dwCount)
   {
      UNREFERENCED_PARAMETER(dwPos);
      UNREFERENCED_PARAMETER(dwCount);
   }

   void buffer::UnlockRange(file_position dwPos, file_size dwCount)
   {
      UNREFERENCED_PARAMETER(dwPos);
      UNREFERENCED_PARAMETER(dwCount);
   }

   void buffer::set_length(file_size dwNewLen)
   {
      UNREFERENCED_PARAMETER(dwNewLen);
   }

   file_size buffer::get_length() const
   {
      return 0;
   }

   // file does not support direct buffering (CMemFile does)
   uint64_t buffer::GetBufferPtr(UINT nCommand, uint64_t nCount, void ** ppBufStart, void ** ppBufMax)
   {
      UNREFERENCED_PARAMETER(nCommand);
      UNREFERENCED_PARAMETER(nCount);
      UNREFERENCED_PARAMETER(ppBufStart);
      UNREFERENCED_PARAMETER(ppBufMax);
      return 0;
   }

/*   void buffer::Rename(const char * lpszOldName, const char * lpszNewName)
   {
      UNREFERENCED_PARAMETER(lpszOldName);
      UNREFERENCED_PARAMETER(lpszNewName);
   }

   void buffer::remove(const char * lpszFileName)
   {
      UNREFERENCED_PARAMETER(lpszFileName);
   }*/

   void buffer::assert_valid() const
   {
   //   object::assert_valid();
      // we permit the descriptor m_hFile to be any value for derived classes
   }

   void buffer::dump(dump_context & dumpcontext) const
   {
      UNREFERENCED_PARAMETER(dumpcontext);
   //   object::dump(dumpcontext);

   //   dumpcontext << "with handle " << (UINT)m_hFile;
   //   dumpcontext << " and name \"" << m_wstrFileName << "\"";
   //   dumpcontext << "\n";
   }



   /////////////////////////////////////////////////////////////////////////////
   // file_exception helpers

/*   #ifdef DEBUG
   static const char * rgszFileExceptionCause[] =
   {
      "none",
      "generic",
      "fileNotFound",
      "badPath",
      "tooManyOpenFiles",
      "accessDenied",
      "invalidFile",
      "removeCurrentDir",
      "directoryFull",
      "badSeek",
      "hardIO",
      "sharingViolation",
      "lockViolation",
      "diskFull",
      "endOfFile",
   };
   static const char szUnknown[] = "unknown";
   #endif*/


   /*void vfxThrowFileException(int32_t cause, LONG lOsError,
   //   const char * lpszFileName /* == NULL */
   /*{
   #ifdef DEBUG
      const char * lpsz;
      if (cause >= 0 && cause < _countof(rgszFileExceptionCause))
         lpsz = rgszFileExceptionCause[cause];
      else
         lpsz = szUnknown;
      TRACE3("file exception: %hs, file %W, App error information = %ld.\n",
         lpsz, (lpszFileName == NULL) ? L"Unknown" : lpszFileName, lOsError);
   #endif
      THROW(new file_exception(cause, lOsError, lpszFileName));
   }*/

   /* Error Codes */

   #define EPERM           1
   #define ENOENT          2
   #define ESRCH           3
   #define EINTR           4
   #define EIO             5
   #define ENXIO           6
   #define E2BIG           7
   #define ENOEXEC         8
   #define EBADF           9
   #define ECHILD          10
#ifndef MACOS
   #define EAGAIN          11
#endif
   #define ENOMEM          12
   #define EACCES          13
   #define EFAULT          14
   #define EBUSY           16
   #define EEXIST          17
   #define EXDEV           18
   #define ENODEV          19
   #define ENOTDIR         20
   #define EISDIR          21
   #define EINVAL          22
   #define ENFILE          23
   #define EMFILE          24
   #define ENOTTY          25
   #define EFBIG           27
   #define ENOSPC          28
   #define ESPIPE          29
   #define EROFS           30
   #define EMLINK          31
   #define EPIPE           32
   #define EDOM            33
   #define ERANGE          34

#if defined(WINDOWS)

   #define EDEADLK         36
   #define ENAMETOOLONG    38
   #define ENOLCK          39
   #define ENOSYS          40
   #define ENOTEMPTY       41
   #define EILSEQ          42

#endif

   /*
    * Support EDEADLOCK for compatibiity with older MS-C versions.
    */
   #define EDEADLOCK       EDEADLK


   /////////////////////////////////////////////////////////////////////////////
   // file name handlers

   string buffer::GetFileName() const
   {
      return "";
   }

   string buffer::GetFileTitle() const
   {
      return "";
   }

   string buffer::GetFilePath() const
   {
      return "";
   }






   // IMPLEMENT_DYNAMIC(file_exception, ::exception::base)

   /////////////////////////////////////////////////////////////////////////////


   /////////////////////////////////////////////////////////////////////////////
   // Help and other support

   // Strings in format ".....%1 .... %2 ...." etc.


   /////////////////////////////////////////////////////////////////////////////
   // file Status implementation

   bool buffer::GetStatus(file_status & rStatus) const
   {
      UNREFERENCED_PARAMETER(rStatus);
      return FALSE;
   }

   bool buffer::GetStatus(const char * lpszFileName, file_status & rStatus)
   {
      UNREFERENCED_PARAMETER(lpszFileName);
      UNREFERENCED_PARAMETER(rStatus);
      return FALSE;
   }


   void buffer::SetStatus(const char * lpszFileName, const file_status & status)
   {
      UNREFERENCED_PARAMETER(lpszFileName);
      UNREFERENCED_PARAMETER(status);
   }



   bool buffer::IsOpened()
   {
      return false;
   }

   string buffer::get_location() const
   {
      return GetFileName();
   }

   
   bool buffer::read(char * pch)
   {
      if(read(pch, 1) == 1)
      {
         return true;
      }
      else
      {
         return false;
      }
   }

   bool buffer::read(uchar * puch)
   {
      if(read(puch, 1) == 1)
      {
         return true;
      }
      else
      {
         return false;
      }
   }

   bool buffer::peek(char * pch)
   {
      if(read(pch, 1) == 1)
      {
         seek(-1, ::file::seek_current);
         return true;
      }
      else
      {
         return false;
      }
   }

   bool buffer::peek(uchar * puch)
   {
      if(read(puch, 1) == 1)
      {
         seek(-1, ::file::seek_current);
         return true;
      }
      else
      {
         return false;
      }
   }

   bool buffer::read(char & ch)
   {
      return read(&ch);
   }

   bool buffer::read(uchar & uch)
   {
      return read(&uch);
   }

   bool buffer::peek(char & ch)
   {
      return peek(&ch);
   }


   bool buffer::peek(uchar & uch)
   {
      return peek(&uch);
   }


   int buffer::sgetc()
   {

      char ch;

      if(!peek(&ch))
         return EOF;

      return ch;
      
   }

   int buffer::sbumpc()
   {

      char ch;

      if(read(&ch, 1) <= 0)
         return EOF;

      return ch;
      
   }

   bool buffer::read_string(string & str)
   {
      
      int i = sbumpc();

      if(i == EOF)
         return false;

      while(true)
      {

         if((char) i == '\n' || (char) i == '\r')
            break;
         
         str += (char) i;
      
         i = sbumpc();
      };

      int iNew = sbumpc();

      if(iNew == i || ((char) iNew != '\n' && (char) iNew != '\r'))
      {
         seek(-1, seek_current);
      }

      return true;

   }

   void buffer::SetFilePath(const char * lpszNewName)
   {
      UNREFERENCED_PARAMETER(lpszNewName);
   }


} // namespace file



