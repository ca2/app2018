// OutBuffer.h
// from 7-zip on 2012-12-23, dawn
#pragma once


/*#include "../IStream.h"
#include "../../Common/MyCom.h"
#include "../../Common/MyException.h"*/

namespace file
{

   class CLASS_DECL_CA2_FILE out_buffer_exception: 
      virtual public system_exception
   {
   public:
      out_buffer_exception(::aura::application * papp, HRESULT errorCode) : 
         ::object(papp),
         ::call_stack(papp),
         ::exception::base(papp),
         ::simple_exception(papp),
         system_exception(papp, errorCode)
      {
      }
   };

   class CLASS_DECL_CA2_FILE out_buffer :
      virtual public ::object
   {
   public:
      ::primitive::virtual_memory      m_memory;
      file_position_t                        _pos;
      file_position_t                        _limitPos;
      file_position_t                        _streamPos;
      writer *                         _stream;
      uint64_t                           _processedSize;
      byte *                           _buffer2;
      bool                             _overDict;

      HRESULT ErrorCode;

      out_buffer();
      ~out_buffer();

      bool create(memory_size_t bufferSize);
      void destroy();

      void SetMemStream(byte *buffer);
      void SetStream(writer *stream);
      void Init();
      HRESULT flush();
      void FlushWithCheck();
      void ReleaseStream();
      HRESULT FlushPart();

      void WriteByte(byte b);
      void WriteBytes(const void *data, size_t size);

      uint64_t GetProcessedSize() const;


   };


} // namespace file


