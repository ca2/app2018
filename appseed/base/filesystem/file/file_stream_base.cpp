#include "framework.h"


namespace file
{

   
   stream_base::stream_base()
   {
      
      m_iostate = goodbit;

   }

   stream_base::stream_base(sp(stream_buffer) pbuffer) :
      m_spbuffer(pbuffer)
   {
      
      m_iostate = goodbit;

   }

   stream_base::stream_base(const stream_base & buffer)
   {
      
      m_iostate   = buffer.m_iostate;
      m_spbuffer  = buffer.m_spbuffer;

   }

   stream_base::~stream_base()
   {
   }


   string stream_base::GetFilePath() const
   {

      return m_spbuffer->GetFilePath();

   }

   void stream_base::close()
   {
      m_spbuffer.release();
   }

  
   file_position stream_base::seek(file_offset offset, e_seek eseek)
   { 
      
      return m_spbuffer->seek(offset, eseek);
   
   }


} // namespace file





