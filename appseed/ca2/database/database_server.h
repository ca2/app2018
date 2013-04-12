#pragma once


namespace database
{

   class id;

   class CLASS_DECL_ca2 server :
      public client_array
   {
   public:


      bool                    m_bRemote;



      virtual bool data_server_load       (client * pclient, class id idSection, class id id, class id idIndex, ::ca::writable & obj, update_hint * phint = ::null());

      virtual bool data_server_load       (client * pclient, class id idSection, class id id, class id idIndex, var & var, update_hint * phint = ::null());
      virtual bool data_server_load       (client * pclient, class id idSection, class id id, class id idIndex, ::ca::byte_output_stream & ostream, update_hint * phint = ::null());
      virtual bool data_server_load       (client * pclient, class id idSection, class id id, class id idIndex, ::ca::byte_serializable & obj, update_hint * phint = ::null());
      virtual bool data_server_load       (client * pclient, class id idSection, class id id, class id idIndex, ::ca::plain_text_output_stream & ostream, update_hint * phint = ::null());
      virtual bool data_server_load       (client * pclient, class id idSection, class id id, class id idIndex, ::ca::plain_text_serializable & obj, update_hint * phint = ::null());
      
      
      


      virtual bool data_server_save       (client * pclient, class id idSection, class id id, class id idIndex, ::ca::readable & obj, update_hint * phint = ::null());

      virtual bool data_server_save       (client * pclient, class id idSection, class id id, class id idIndex, var & var, update_hint * phint = ::null());
      virtual bool data_server_save       (client * pclient, class id idSection, class id id, class id idIndex, ::ca::byte_input_stream & istream, update_hint * phint = ::null());
      virtual bool data_server_save       (client * pclient, class id idSection, class id id, class id idIndex, ::ca::byte_serializable & obj, update_hint * phint = ::null());
      virtual bool data_server_save       (client * pclient, class id idSection, class id id, class id idIndex, ::ca::plain_text_input_stream & istream, update_hint * phint = ::null());
      virtual bool data_server_save       (client * pclient, class id idSection, class id id, class id idIndex, ::ca::plain_text_serializable & obj, update_hint * phint = ::null());





      virtual bool var_load               (client * pclient, class id idSection, class id id, class id idIndex, ::ca::writable & obj, update_hint * phint = ::null());
      virtual bool var_save               (client * pclient, class id idSection, class id id, class id idIndex, ::ca::readable & obj, update_hint * phint = ::null());


      virtual bool data_pulse_change      (client * pclient, class id idSection, class id id, class id idIndex, update_hint * puh = ::null());

      virtual bool on_before_data_change  (client * pclient, class id idSection, class id id, class id idIndex, var & varNew, update_hint * puh = ::null());
      virtual bool on_after_data_change   (client * pclient, class id idSection, class id id, class id idIndex, update_hint * puh = ::null());

      virtual var data_load               (client * pclient, class id idSection, class id id, class id idIndex, update_hint * phint = ::null());
      virtual bool data_save              (client * pclient, class id idSection, class id id, class id idIndex, var var, update_hint * phint = ::null());
   };

} // namespace database