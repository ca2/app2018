//#include "framework.h"



namespace file
{


   // FindSignature.cpp
   // from 7-zip on 2012-12-23, lunch time
   ////#include "framework.h"

   /*//#include "Common/Buffer.h"

   //#include "FindSignature.h"

   //#include "../../Common/StreamUtils.h"*/


   input_stream::input_stream()
   {

      m_gcount = 0;

   }




   input_stream::input_stream(stream_buffer * preader)
   {
      
      m_spbuffer = preader;      
      m_gcount = 0;

   }

   input_stream::input_stream(const input_stream & istream)
   {
      
      m_spbuffer = istream.m_spbuffer;      
      m_gcount = 0;

   }

   input_stream::~input_stream()
   {
      
      close();

   }


   ::primitive::memory_size input_stream::read(void * lpBuf, ::primitive::memory_size nCount)
   {
      
      return m_gcount = m_spbuffer->read(lpBuf, nCount);
      
   }

   void input_stream::full_read(void * lpBuf, ::primitive::memory_size nCount)
   {

      if(fail())
      {

         return;

      }


      if(!m_spbuffer->full_read(lpBuf,nCount))
      {

         setstate(failbit);

         return;

      }

      

      m_gcount = nCount;
      
   }


   void input_stream::full_fill(::primitive::memory_base & m)
   {
      
      full_read(m.get_data(), m.get_size());

   }

   void input_stream::read_arbitrary(int32_t & i)
   {


   }

   void input_stream::read_arbitrary(uint32_t & ui)
   {


   }

   void input_stream::read_arbitrary(int64_t & i)
   {


   }

   void input_stream::read_arbitrary(uint64_t & ui)
   {


   }


   void input_stream::read (bool & b)
   {
      UNREFERENCED_PARAMETER(b);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (char & ch)
   {
      UNREFERENCED_PARAMETER(ch);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (uchar & uch)
   {
      UNREFERENCED_PARAMETER(uch);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (wchar_t & wch)
   {
      UNREFERENCED_PARAMETER(wch);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (int16_t & sh)
   {
      UNREFERENCED_PARAMETER(sh);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (uint16_t & ui)
   {
      UNREFERENCED_PARAMETER(ui);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (int32_t & i)
   {
      UNREFERENCED_PARAMETER(i);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (uint32_t & ui)
   {
      UNREFERENCED_PARAMETER(ui);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (int64_t & i)
   {
      UNREFERENCED_PARAMETER(i);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (uint64_t & ui)
   {
      UNREFERENCED_PARAMETER(ui);
      ::exception::throw_interface_only(get_app());
   }
   
#ifdef APPLEOS
   
   void input_stream::read(unsigned long & ui)
   {
      
   }
   
#endif
   
   void input_stream::read (float & f)
   {
      UNREFERENCED_PARAMETER(f);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (double & d)
   {
      UNREFERENCED_PARAMETER(d);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (LPRECT lprect)
   {
      UNREFERENCED_PARAMETER(lprect);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (SIZE & size)
   {
      UNREFERENCED_PARAMETER(size);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (sp(type) info)
   {
      UNREFERENCED_PARAMETER(info);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (serializable & serializable)
   {
      UNREFERENCED_PARAMETER(serializable);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (id & id)
   {
      UNREFERENCED_PARAMETER(id);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (var & var)
   {
      UNREFERENCED_PARAMETER(var);
      ::exception::throw_interface_only(get_app());
   }


   void input_stream::read (string & str)
   {
      UNREFERENCED_PARAMETER(str);
      ::exception::throw_interface_only(get_app());
   }

   int input_stream::get()
   {
      unsigned char uch;

      if(read(&uch,1) == 1)
         return uch;

      return EOF;

   }


   int input_stream::peek()
   {
      unsigned char uch;

      if(read(&uch,1) == 1)
      {
         seek(-1,seek_current);
         return uch;
      }

      return EOF;

   }

   input_stream & input_stream::getline(char * sz,strsize n)
   {
      int c;
      while(n > 0)
      {
         c = get();
         if(c == EOF)
         {
            break;
         }
         else if(c == '\n')
         {
            c = get();
            if(c != '\r' && c != EOF)
               seek(-1,seek_current);
            break;
         }
         else if(c == '\r')
         {
            c = get();
            if(c != '\n' && c != EOF)
               seek(-1,seek_current);
            break;
         }
         *sz = (char)c;
         sz++;
         n--;
      }

      return *this;

   }


   input_stream & input_stream::operator = (const input_stream & istream)
   {

      m_spbuffer = istream.m_spbuffer;

      return *this;

   }


   bool input_stream::is_reader_null()
   {
      return m_spbuffer.is_null();
   }

   bool input_stream::is_reader_set()
   {
      return m_spbuffer.is_set();
   }


   void input_stream::read_to_hex(string & str, file_position dwStart, file_position dwEnd)
   {
      primitive::memory memory(get_app());
      if(dwStart == (file_position) -1)
      {
		   dwStart = tellg();
	   }
	   else
      {
         seek_from_begin(dwStart);
      }
      ::primitive::memory_position uiPos = 0;
      ::primitive::memory_size uiRead;
      memory.allocate(1024);

      strsize nCount;
      
      if (dwEnd == (file_position)-1)
      {

         nCount = ::numeric_info< strsize >::max();

      }
      else
      {

         nCount = (strsize)(dwEnd - dwStart);

      }
       
      while((uiRead = read(&memory.get_data()[uiPos], MIN(memory.get_size() - uiPos, (::primitive::memory_size) nCount))) > 0)
      {
         uiPos += uiRead;
         nCount -= uiRead;
         memory.allocate_add_up(1024 * 1024);
      }
      memory.allocate((::primitive::memory_size) uiPos);
      memory.to_hex(str);
   }



   
} // namespace file




