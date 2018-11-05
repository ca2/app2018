#pragma once


namespace file
{


   class size_count_writer:
      virtual public writer
   {
   public:
      file_sp     _stream;
      file_size_t         _size;

      void SetStream(writer * stream)
      {
         _stream = stream;
      }

      void Init()
      {
         _size = 0;
      }

      file_size_t get_size() const
      {
         return _size;
      }

      void write(const void *data, memory_size_t size, memory_size_t * processedSize);

   };


} // namespace file
