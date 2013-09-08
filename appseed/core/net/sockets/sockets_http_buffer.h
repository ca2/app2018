#pragma once


namespace sockets
{


   class CLASS_DECL_ca2 http_buffer : 
      virtual public ::file::transfer_stream
   {
   public:

         
      http_buffer(sp(base_application) papp, mutex * pmutex = NULL);
      http_buffer(sp(base_application) papp, ::file::memory_buffer * pmemoryfileIn);
      virtual ~http_buffer();


      virtual bool open(const char * lpszFileName, UINT nOpenFlags);

         
      DECL_GEN_SIGNAL(on_http_request_response);


   };


} // namespace sockets




