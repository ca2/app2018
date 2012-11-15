#pragma once
namespace bsd
{

namespace sockets
{

   class sip_socket;

   class CLASS_DECL_ca sip_callback :
      virtual public base_object
   {
   public:

      void on_sip_event(sip_socket * psocket, sip_event * pevent);
   };

} // namespace sockets

} // namespace bsd