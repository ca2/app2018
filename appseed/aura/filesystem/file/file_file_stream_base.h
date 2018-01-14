#pragma once


namespace file
{

   class CLASS_DECL_AURA file_stream_base:
      virtual public stream_base
   {
   public:


      memory_size_t      m_gcount;


      file_stream_base();
      virtual ~file_stream_base();


      virtual void open(const ::file::path & lpszFileName,uint32_t nOpenFlags,::aura::application * papp = ::get_app());

   };


} // namespace file





