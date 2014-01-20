#pragma once


namespace sockets
{


   class base_socket_handler;
   class base_socket;


   class CLASS_DECL_BASE logger :
      virtual public ::object
   {
   public:


      virtual void log(base_socket_handler *, base_socket * , const string & strUser, int32_t err, const string & strSystem, ::core::log::e_level elevel = ::core::log::level_warning) = 0;


   };


} // namespace sockets




