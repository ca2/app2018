// InStreamWithCRC.h
// from 7-zip on dawn of 13/01/2001 - Thursday

namespace libcompress
{

   class input_stream_with_crc:
      public ::file::input_stream
   {
   public:

      sp(::file::input_stream)  _stream;
      uint64_t         _size;
      uint32_t         _crc;

      ::primitive::memory_size read(void *data, ::primitive::memory_size size);
      file_position seek(file_offset offset, ::file::e_seek seekOrigin);
      void SetStream(::file::input_stream *stream) { _stream = stream;  }
      void Init()
      {
         _size = 0;
         // _wasFinished = false;
         _crc = CRC_INIT_VAL;
      }
      void ReleaseStream() { ::release(_stream.m_p); }
      uint32_t GetCRC() const { return CRC_GET_DIGEST(_crc); }
      uint64_t GetSize() const { return _size; }
      // bool WasFinished() const { return _wasFinished; }
   };

} // namespace libcompress
