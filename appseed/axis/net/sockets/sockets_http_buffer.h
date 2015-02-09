#pragma once


namespace sockets
{


   class CLASS_DECL_AXIS http_buffer :
      virtual public ::file::transfer_buffer
   {
   public:


      http_buffer(::aura::application * papp, mutex * pmutex = NULL);
      http_buffer(::aura::application * papp, ::file::memory_buffer * pmemoryfileIn);
      virtual ~http_buffer();


      virtual fesp open(const char * lpszFileName, UINT nOpenFlags);


      DECL_GEN_SIGNAL(on_http_request_response);


   };


} // namespace sockets




