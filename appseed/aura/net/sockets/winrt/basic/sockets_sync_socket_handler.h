#pragma once


namespace sockets
{


   class CLASS_DECL_AXIS sync_socket_handler :
      virtual public ::object,
      public socket::callback
   {
      public:


         socket *                      m_psocket;
         ::memory_file         m_file;
         socket_handler                m_handler;
         int                           m_iDefaultTimeout;
#ifdef DEBUG
         sync_socket_handler(::axis::application * papp, logger *log = NULL);
#else
         sync_socket_handler(::axis::application * papp);
#endif
         virtual ~sync_socket_handler();



         virtual void OnRawData(socket * psocket, void * pdata, size_t len);

         void handle(socket * psocket);

         virtual void write(string & str);
         virtual void write(void * pdata, memory_size_t len);
         virtual memory_size_t read(void * pdata, memory_size_t len);
         virtual string read_line();
         virtual string read_string();


         virtual void read_payload_v1(string & strPayload, int timeout = -1);
         virtual void write_payload_v1(const char * szPayload, int timeout = -1);

   };


} // namespace sockets






