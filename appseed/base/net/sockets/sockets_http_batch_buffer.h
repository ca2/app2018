#pragma once


namespace sockets
{


   class CLASS_DECL_BASE http_batch_buffer : 
      virtual public ::file::binary_buffer
   {
   public:

      UINT                 m_nOpenFlags;
      string               m_strPath;



      http_batch_buffer(sp(::aura::application) papp);

      virtual bool open(const char * lpszFileName, UINT nOpenFlags);
      virtual void close();
      virtual void flush();

      virtual void get_file_data() = 0;
      virtual void set_file_data() = 0;


   };

} // namespace sockets




