#pragma once


#ifdef DEBUG


namespace sockets
{


   class base_socket_handler;
   class base_socket;


   class CLASS_DECL_AURA logger :
      virtual public ::object
   {
      public:


         virtual void log(base_socket_handler *, base_socket *, const string & strUser, int32_t err, const string & strSystem, ::aura::log::e_level elevel = ::aura::log::level_warning) = 0;


   };


} // namespace sockets


#endif


