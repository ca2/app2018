//#include "framework.h"


namespace file
{


   buffered_file::buffered_file(::aura::application * papp, ::file::file_sp pfile, memory_size_t iBufferSize) :
      object(papp)
   {
      m_storage.allocate(iBufferSize);
      m_pfile              = pfile;
      m_uiBufferSize        = iBufferSize;
      m_uiPosition          = 0;
      m_uiBufLPos           = 0;
      m_uiBufUPos           = 0xffffffff;
      m_uiWriteLPos         = 0xffffffff;
      m_uiWriteUPos         = 0xffffffff;
      m_bDirty = false;
   }

   buffered_file::~buffered_file()
   {
      flush();
   }

   uint64_t buffered_file::GetBufferSize()
   {
      return m_uiBufferSize;
   }

   bool buffered_file::IsValid() const
   {
      return m_pfile.is_set();
   }

   /*int32_t buffered_file::remove_begin(void * lpBuf, UINT uiCount)
   {
      ASSERT(IsValid());
      if(uiCount > get_length())
         uiCount = m_storage.get_size();
      memcpy(lpBuf, ((LPBYTE)get_data()), uiCount);
      if(uiCount < this->get_size())
      {
         memmove(
            m_storage.get_data(),
            &((LPBYTE)m_storage.get_data())[uiCount],
            m_storage.get_size() - uiCount);
      }

      if(m_dwPosition <= uiCount)
         m_uiPosition = 0;
      else
         m_uiPosition -= uiCount;


      m_storage.allocate(m_uiPosition);

      return uiCount;
   }*/

   /*void buffered_file::load_string(string & str)
   {
   }*/

   file_position_t buffered_file::seek(file_offset_t lOff, ::file::e_seek nFrom)
   {
      uint64_t uiBegBufPosition = m_uiBufLPos;
      uint64_t uiEndBufPosition = m_uiBufUPos;
      uint64_t uiNewPos;
      if(nFrom == ::file::seek_begin)
      {
         if(lOff < 0)
            lOff = 0;
         uiNewPos = lOff;
      }
      else if(nFrom == ::file::seek_end)
      {
         uiNewPos = m_pfile->get_length() + lOff;
         //if (compare::gt(uiNewPos, m_pfile->get_length()))
           // uiNewPos = m_pfile->get_length();
      }
      else if(nFrom == ::file::seek_current)
      {
         int64_t iNewPos = m_uiPosition + lOff;
         if(iNewPos < 0)
            iNewPos = 0;
         else if(::compare::gt(iNewPos, m_pfile->get_length()))
            iNewPos = m_pfile->get_length();
         uiNewPos = iNewPos;
      }
      else
      {
         throw invalid_argument_exception(get_app(), "::file::buffered_file::seek invalid seek option");
      }

      if(uiNewPos >= uiBegBufPosition
         && uiNewPos <= uiEndBufPosition)
      {
         m_uiPosition = uiNewPos;
      }
      else
      {
         m_uiPosition = m_pfile->seek(uiNewPos, ::file::seek_begin);
         if(m_bDirty)
            flush();
         m_uiBufLPos     = 0;
         m_uiBufUPos     = 0xffffffff;
         m_uiWriteLPos   = 0xffffffff;
         m_uiWriteUPos   = 0xffffffff;
      }
      return m_uiPosition;
   }

   file_position_t buffered_file::get_position() const
   {
      return m_uiPosition;
   }

   file_size_t buffered_file::get_length() const
   {
      return m_pfile->get_length();
   }

   /*void buffered_file::Truncate(int32_t iPosition)
   {
      m_pfile->Truncate();
   }

   void buffered_file::clear()
   {
      m_pfile->clear();
   }*/

   memory_size_t buffered_file:: read(void *lpBufParam, memory_size_t nCount)
   {
      if(nCount == 0)
         return 0;
      memory_size_t uiRead = 0;
      memory_size_t uiReadNow = 0;
      while(uiRead < nCount)
      {
         if(m_uiPosition >= m_uiBufLPos && m_uiPosition <= m_uiBufUPos && m_uiBufUPos != 0xFFFFFFFF)
         {
            uiReadNow = MIN(nCount - uiRead, (memory_size_t) (m_uiBufUPos - m_uiPosition + 1));
            if(nCount == 1)
            {
               ((LPBYTE)lpBufParam)[uiRead] = m_storage.get_data()[m_uiPosition - m_uiBufLPos];
            }
            else
            {
               memcpy(&((LPBYTE)lpBufParam)[uiRead], &m_storage.get_data()[m_uiPosition - m_uiBufLPos], (size_t) uiReadNow);
            }
            m_uiPosition += uiReadNow;
            uiRead += uiReadNow;
         }
         if(uiRead < nCount)
         {
            if(!buffer())
               break;
         }
      }
      return uiRead;
   }

   bool buffered_file::buffer(memory_size_t uiGrow)
   {

      if(m_bDirty)
      {

         flush();

      }

      m_pfile->seek((file_offset_t) m_uiPosition, ::file::seek_begin);

      memory_size_t uiCopy;

      if (uiGrow > 0)
      {

         uiCopy = MIN(m_uiBufferSize, uiGrow);

      }
      else
      {

         uiCopy = m_uiBufferSize;

      }

      memory_size_t uiRead    = m_pfile->read(m_storage.get_data(), uiCopy);
      
      m_uiBufLPos             = m_uiPosition;
      
      m_uiBufUPos             = m_uiPosition + uiRead - 1;
      
      m_uiWriteLPos           = 0xffffffff;
      
      m_uiWriteUPos           = 0xffffffff;

      return uiRead > 0;

   }


   void buffered_file::write(const void * lpBuf, memory_size_t nCount)
   {
      memory_size_t uiWrite = 0;
      memory_size_t uiWriteNow = 0;
      while(uiWrite < nCount)
      {
         if(m_uiPosition >= m_uiBufLPos && m_uiPosition < (m_uiBufLPos + m_uiBufferSize))
         {
            m_bDirty = true;
            uiWriteNow = MIN(nCount - uiWrite, (memory_size_t) ((m_uiBufLPos + m_uiBufferSize) - m_uiPosition + 1));
            if(m_uiWriteLPos == 0xffffffff || m_uiWriteLPos > m_uiPosition)
               m_uiWriteLPos = m_uiPosition;
            if(m_uiWriteUPos == 0xffffffff || m_uiWriteUPos < (m_uiPosition + uiWriteNow - 1))
               m_uiWriteUPos = (m_uiPosition + uiWriteNow - 1);
            memcpy(&m_storage.get_data()[m_uiPosition - m_uiBufLPos], &((LPBYTE)lpBuf)[uiWrite], (size_t) uiWriteNow);
            m_uiPosition += uiWriteNow;
            uiWrite += uiWriteNow;
         }
         if(uiWrite < nCount)
         {
            buffer(nCount - uiWrite);
         }
      }
   }

   void buffered_file::flush()
   {
      if(m_bDirty)
      {
         m_pfile->seek((file_offset_t) m_uiWriteLPos, ::file::seek_begin);
         m_pfile->write(&m_storage.get_data()[m_uiWriteLPos - m_uiBufLPos], (memory_size_t) (m_uiWriteUPos - m_uiWriteLPos + 1));
         m_bDirty = false;
         m_uiWriteLPos = 0xffffffff;
         m_uiWriteUPos = 0xffffffff;
      }
   }


   void buffered_file::set_length(file_size_t dwNewLen)
   {
      m_pfile->set_length(dwNewLen);
      m_uiPosition = 0;
      m_uiBufLPos = 0;
      m_uiBufUPos = 0xffffffff;
      m_uiWriteLPos = 0xffffffff;
      m_uiWriteUPos = 0xffffffff;
      m_bDirty = false;
   }


} // namespace file



