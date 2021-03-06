#pragma once


namespace database
{


   class server;
   class update_hint;


   class CLASS_DECL_AXIS change_event :
      public ::message::message
   {
   public:


      server *          m_pserver;
      client *          m_pclient;
      key               m_datakey;
      update_hint *     m_puh;
      bool              m_bOk;
      var *             m_pvar;

      change_event();
      change_event(var & var);


      bool data_get(::object & obj);


   };


} // namespace event


