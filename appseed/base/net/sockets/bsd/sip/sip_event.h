#pragma once


namespace sockets
{


   class CLASS_DECL_BASE sip_event :
      virtual public base_object
   {
   public:
      enum e_event
      {
         event_invite, 
      };

      e_event     m_eevent;

      sip_event(sp(base_application) papp);
   };


} // namespace sockets



