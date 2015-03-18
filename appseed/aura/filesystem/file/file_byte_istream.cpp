//#include "framework.h"


// return string length or -1 if UNICODE string is found in the archive
__STATIC UINT __read_string_length(::file::byte_istream ar)
{
   uint32_t nNewLen;

   // attempt BYTE length first
   byte bLen;
   ar >> bLen;

   if (bLen < 0xff)
      return bLen;

   // attempt WORD length
   uint16_t wLen;
   ar >> wLen;
   if (wLen == 0xfffe)
   {
      // UNICODE string prefix (length will follow)
      return (UINT)-1;
   }
   else if (wLen == 0xffff)
   {
      // read uint32_t of length
      ar >> nNewLen;
      return (UINT)nNewLen;
   }
   else
      return wLen;
}



namespace file
{


   int get_arbitrary_length(uint64_t ui);



   // FindSignature.cpp
   // from 7-zip on 2012-12-23, lunch time
   ////#include "framework.h"

   /*//#include "Common/Buffer.h"

   //#include "FindSignature.h"

   //#include "../../Common/StreamUtils.h"*/


   byte_istream::byte_istream()
   {
   }

   byte_istream::byte_istream(stream_buffer * preader) :
      istream(preader)
   {

   }

   byte_istream::byte_istream(const istream & is) :
      istream(is)
   {

   }

   byte_istream::~byte_istream()
   {

   }

   void byte_istream::read(char & ch)
   {

      if(m_spbuffer->read(&ch, sizeof(ch)) != sizeof(ch))
         throw io_exception(get_app(), "byte_istream::read");



   }


   void byte_istream::read(uchar & uch)
   {

      if(fail())
      {

         return;

      }

      if(m_spbuffer->read(&uch,sizeof(uch)) != sizeof(uch))
      {

         setstate(failbit);

      }



   }


   void byte_istream::read(int16_t & i)
   {

      read_arbitrary(&i, sizeof(i));



   }

   void byte_istream::read(uint16_t & ui)
   {
      read_arbitrary(&ui, sizeof(ui));

   }

   void byte_istream::read(wchar_t & wch)
   {
      read_arbitrary(&wch, sizeof(wch));

   }

   void byte_istream::read(bool & b)
   {
      m_spbuffer->read(&b, sizeof(b));

   }

   void byte_istream::read(int32_t & i)
   {
      read_arbitrary(&i, sizeof(i));

   }

   void byte_istream::read(uint32_t & ui)
   {
      read_arbitrary(&ui, sizeof(ui));

   }

   void byte_istream::read(int64_t & i)
   {
      read_arbitrary(&i, sizeof(i));

   }

   void byte_istream::read(uint64_t & ui)
   {
      read_arbitrary(&ui, sizeof(ui));

   }

   void byte_istream::read_arbitrary(int32_t & i)
   {

      read_arbitrary(&i, sizeof(i));

   }

   void byte_istream::read_arbitrary(uint32_t & ui)
   {

      read_arbitrary(&ui, sizeof(ui));

   }

   void byte_istream::read_arbitrary(int64_t & i)
   {

      read_arbitrary(&i, sizeof(i));

   }

   void byte_istream::read_arbitrary(uint64_t & ui)
   {

      read_arbitrary(&ui, sizeof(ui));

   }

   void byte_istream::read_arbitrary(void * p, ::count nMax)
   {

      if(fail())
         return;

      byte b;

      full_read(&b,sizeof(b));

      if(fail())
         return;

      if(b == 0)
      {

         memset(p, 0, nMax);

         return;

      }

      uint64_t uiRead = 0;

      int len = b & 0x3f;

      if(len > sizeof(uiRead) || len > nMax)
      {

         setstate(failbit);

         return;

      }

      full_read(&uiRead,len);

      if(fail())
         return;

      if(b & 0x40)
      {

         int64_t i = - ((int64_t) uiRead);

         memcpy(p, &i, nMax);

      }
      else
      {

         memcpy(p, &uiRead, nMax);

      }

   }


   void byte_istream::read(float & f)
   {

      full_read(&f, sizeof(f));

   }


   void byte_istream::read(double & d)
   {

      full_read(&d, sizeof(d));

   }

   void byte_istream::read(LPRECT lprect)
   {

      full_read(&lprect->left,     sizeof(lprect->left));
      full_read(&lprect->top,      sizeof(lprect->top));
      full_read(&lprect->right,    sizeof(lprect->right));
      full_read(&lprect->bottom,   sizeof(lprect->bottom));

   }

   void byte_istream::read(SIZE & size)
   {

      full_read(&size.cx,     sizeof(size.cx));
      full_read(&size.cy,      sizeof(size.cy));

   }


   void byte_istream::read(sp(type) info)
   {

      ::primitive::memory m;


      {
         int32_t iLen;
         full_read(&iLen, sizeof(iLen));
         m.allocate(iLen + 1);
         full_fill(m);
         m.get_data()[iLen] = '\0';
         info->m_id = (const char *) m.get_data();
      }
      {
         int32_t iLen;
         full_read(&iLen, sizeof(iLen));
         m.allocate(iLen + 1);
         full_fill(m);
         m.get_data()[iLen] = '\0';
         info->m_idFriendly = (const char *) m.get_data();
      }

   }


   void byte_istream::read(serializable & serializable)
   {
      serializable.read(*this);

   }

   void byte_istream::read(id & id)
   {

      bool bNull;

      read(bNull);

      if(bNull)
         id.clear();
      else
      {
         string str;

         *this >> str;

         id = str;
      }



   }


   void byte_istream::read(var & var)
   {

      var.read(*this);

   }

   void byte_istream::read(property & property)
   {

      property.read(*this);

   }


   string byte_istream::get_location() const
   {
      return "<unknown byte_istream location>";
   }


   /*file_position byte_istream::seek(file_offset offset, e_seek seekOrigin)
   {
   return m_spbuffer->seek(offset, seekOrigin);
   }*/


   void byte_istream::full_load(string & str)
   {

      if(m_spbuffer.is_null())
         return;

      sp(seekable) spseekable = m_spbuffer;

      if(spseekable.is_set())
      {

         spseekable->seek_to_end();

         ::file_position uiLength = spseekable->get_position();

         spseekable->seek_to_begin();

         ::strsize uiCount = ::numeric_info< strsize >::natural(uiLength);

         ::file_position uiPos = 0;

         LPSTR lpstr = str.GetBufferSetLength((strsize) (uiLength + 1));

         while(uiCount > 0)
         {

            ::primitive::memory_size uiRead =  m_spbuffer->read(&lpstr[uiPos], (::primitive::memory_size) uiCount);

            uiCount -= uiRead;

            uiPos+=uiRead;

         }

         str.ReleaseBuffer((strsize) uiLength);

      }
      else
      {

         ::file::string_buffer strbuffer;

         ::strsize uiPos = 0;

         ::primitive::memory_size uiRead;

         do
         {

            try
            {

               strbuffer.alloc_up(1024);

            }
            catch(...)
            {

               break;

            }

            uiRead =  m_spbuffer->read((void *) &((const char *) strbuffer)[uiPos], (::primitive::memory_size) (strbuffer.get_allocation_size() - uiPos));

            uiPos += uiRead;

         } while(uiRead > 0);

         strbuffer.m_iSize = uiPos;

         str = strbuffer.str();

      }

   }


   void byte_istream::read (string & string)
   {
      int32_t nConvert = 0;   // if we get UNICODE, convert

      UINT nNewLen = __read_string_length(*this);
      if (nNewLen == (UINT)-1)
      {
         nConvert = 1 - nConvert;
         nNewLen = __read_string_length(*this);
         ASSERT(nNewLen != -1);
      }

      char * lpBuf;
      // set length of string to new length
      UINT nByteLen = nNewLen;
      nByteLen += nByteLen * nConvert;    // bytes to read
      if (nNewLen == 0)
         lpBuf = string.GetBufferSetLength(0);
      else
         lpBuf = string.GetBufferSetLength((int32_t)nByteLen+nConvert);

      // read in the characters
      if (nNewLen != 0)
      {
         ASSERT(nByteLen != 0);

         // read new data
         if (m_spbuffer->read(lpBuf, nByteLen) != nByteLen)
         {
            //   ::core::ThrowArchiveException(CArchiveException::endOfFile);
         }

         // convert the data if as necessary
         if (nConvert != 0)
         {
            lpBuf[nNewLen] = '\0';    // must be NUL terminated
            string.ReleaseBuffer();   // don't delete the old data
         }
      }

   }




} // namespace file




