#include "framework.h"


namespace file
{


   writer::writer()
   {
   }


   writer::~writer()
   {
   }


   void writer::write(const void *lpBuf, memory_size_t nCount)
   {

      memory_size_t memory_size;
      
      write(lpBuf, nCount, &memory_size);

   }

   void writer::write(const void *lpBuf, memory_size_t nCount, memory_size_t * dwWritten)
   {

      ::exception::throw_interface_only(get_app());

   }

   void writer::transfer_from_begin(reader & reader, memory_size_t uiBufSize)
   {

      reader.seek_to_begin();

      transfer_from(reader, uiBufSize);

   }

   void writer::transfer_from(reader & reader, memory_size_t uiBufSize)
   {


      if(reader.get_internal_data() != NULL && reader.get_internal_data_size() > reader.get_position())
      {

         write((byte *) reader.get_internal_data() + reader.get_position(), (memory_size_t) (reader.get_internal_data_size() - reader.get_position()));

         return;

      }


      memory_size_t uiRead;
      memory_size_t uiSize = 0;
      uiBufSize = MAX(32 * 1024, uiBufSize);

      memory buf;

      buf.allocate(uiBufSize);

      if(buf.get_data() == NULL)
         throw memory_exception(get_app());

      try
      {
         while(true)
         {
            uiRead = reader.read(buf.get_data(), buf.get_size());
            if(uiRead <= 0)
            {
               break;
            }
            write(buf.get_data(), uiRead);
            uiSize += uiRead;
         }
      }
      catch(...)
      {
      }

   }

   /*
   bool writer::is_writer_null()
   {
      return m_spwriter.is_null();
   }

   bool writer::is_writer_set()
   {
      return m_spwriter.is_set();
   }
   */
   void writer::close()
   {
      /*
      if(m_spwriter.is_set())
      {
         //m_spwriter->close();
         ::release(m_spwriter.m_p);
      }
      */
   }

   /*
   void writer::from_hex(const char * psz)
   {
      memory memory(get_app());
      memory.from_hex(psz);
      write(memory.get_data(), memory.get_size());
   }*/

   void writer::flush()
   {
   }

   /*
   void writer::write (char ch)
   {
      UNREFERENCED_PARAMETER(ch);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (uchar uch)
   {
      UNREFERENCED_PARAMETER(uch);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (int16_t sh)
   {
      UNREFERENCED_PARAMETER(sh);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (uint16_t ui)
   {
      UNREFERENCED_PARAMETER(ui);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (unichar wch)
   {
      UNREFERENCED_PARAMETER(wch);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (bool b)
   {
      UNREFERENCED_PARAMETER(b);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (int32_t i)
   {
      UNREFERENCED_PARAMETER(i);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (uint32_t ui)
   {
      UNREFERENCED_PARAMETER(ui);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (int64_t i)
   {
      UNREFERENCED_PARAMETER(i);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (uint64_t ui)
   {
      UNREFERENCED_PARAMETER(ui);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (float f)
   {
      UNREFERENCED_PARAMETER(f);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (double d)
   {
      UNREFERENCED_PARAMETER(d);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (const RECT & rect)
   {
      UNREFERENCED_PARAMETER(lpcrect);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (SIZE & size)
   {
      UNREFERENCED_PARAMETER(size);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (sp(type) info)
   {
      UNREFERENCED_PARAMETER(info);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (serializable & serializable)
   {
      UNREFERENCED_PARAMETER(serializable);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (const char * psz)
   {
      UNREFERENCED_PARAMETER(psz);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (const id & id)
   {
      UNREFERENCED_PARAMETER(id);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (const var & var)
   {
      UNREFERENCED_PARAMETER(var);
      ::exception::throw_interface_only(get_app());
   }

   void writer::write (const string & str)
   {
      UNREFERENCED_PARAMETER(str);
      ::exception::throw_interface_only(get_app());
   }
   */


   HRESULT write(writer * pwriter, const void * data, memory_size_t size)
   {
      HRESULT res = S_OK;
      try
      {
         pwriter->write(data, size);
      }
      catch(...)
      {
         res = E_FAIL;
      }
      RINOK(res);
      return res;
   }

} // namespace file




