﻿#pragma once


namespace user
{


   class CLASS_DECL_AURA message_queue_listener:
      virtual public ::object
   {
   public:

      virtual void message_queue_message_handler(::message::message * pobj) { UNREFERENCED_PARAMETER(pobj); };

   };


} // namespace user









