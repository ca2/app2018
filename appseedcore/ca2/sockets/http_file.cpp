#include "StdAfx.h"

namespace http
{

   bool file::read_string(string & str)
   {
      DWORD_PTR nRead;
      char ch;
      bool bR = false;
      while((nRead = read(&ch, sizeof(ch))) > 0)
      {
         str += ch;
         if(bR)
         {
            if(ch == '\n')
            {
               break;
            }
            else
            {
               seek(-1, ::ex1::seek_current);
            }
         }
         else if(ch == '\r')
         {
            bR = true;
         }
         else if(ch == '\n')
         {
            break;
         }
      }
      return str.has_char();
   }

   inline file & file::operator << (ex1::file & file)
      {
         DWORD_PTR nRead;
         primitive::memory mem;
         if(!mem.allocate(1024 * 1024))
         {
            AfxThrowMemoryException();
         }
         while((nRead = file.read(mem.GetAllocation(), mem.GetStorageSize())) > 0)
         {
            write(mem.GetAllocation(), nRead);
         }
         return *this;
      }

} // namespace http