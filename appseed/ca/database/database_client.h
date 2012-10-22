#pragma once




namespace database
{


   class client_array;


   class CLASS_DECL_ca client :
      virtual public ::radix::object
   {
   public:


      friend class client_array;
      friend class server;


      client();
      virtual ~client();


      virtual bool set_server(server * pserver);


      void initialize(::ca::application * papp);
      void initialize(server * pserver);

      DECL_GEN_VSIGNAL(data_on_before_change)
      DECL_GEN_VSIGNAL(data_on_after_change)

      virtual bool data_set(class id, bool b, update_hint * phint = NULL);
      virtual bool data_set(class id, const char * lpsz, update_hint * puh = NULL);
      virtual bool data_set(class id, int i, update_hint * puh = NULL);
      virtual bool data_set(class id, int64_t i, update_hint * puh = NULL);

      virtual bool data_get(class id, bool & b);
      virtual bool data_get(class id, string & str);
      virtual bool data_get(class id, int & i);
      virtual bool data_get(class id, int64_t & i);

      virtual bool data_set(class id, class id idIndex, bool b, update_hint * phint = NULL);
      virtual bool data_set(class id, class id idIndex, var & var, update_hint * puh = NULL);
      virtual bool data_set(class id, class id idIndex, int i, update_hint * puh = NULL);
      virtual bool data_set(class id, class id idIndex, int64_t i, update_hint * puh = NULL);
      virtual bool data_set(class id, class id idIndex, const char * lpsz, update_hint * puh = NULL);
      virtual bool data_set(class id, class id idIndex, const wchar_t * lpsz, update_hint * puh = NULL);
      virtual bool data_set(class id, class id idIndex, ex1::readable & obj, update_hint * puh = NULL);
      virtual bool data_set(class id, class id idIndex, ex1::byte_serializable & obj, update_hint * puh = NULL);
      virtual bool data_set(class id, class id idIndex, ex1::byte_input_stream & obj, update_hint * puh = NULL);
      virtual bool data_set(class id, class id idIndex, ex1::plain_text_serializable & obj, update_hint * puh = NULL);
      virtual bool data_set(class id, class id idIndex, ex1::plain_text_input_stream & obj, update_hint * puh = NULL);
      virtual bool data_set(selection & selection, const char * lpsz, update_hint * puh = NULL);
      virtual bool data_set(selection & selection, var & var, update_hint * puh = NULL);

      virtual bool data_set(class id dataid, class id, class id idIndex, const char * lpsz, update_hint * puh = NULL);

      virtual bool data_get(class id, class id idIndex, bool & b);
      virtual bool data_get(class id, class id idIndex, var & var);
      virtual bool data_get(class id, class id idIndex, int & i);
      virtual bool data_get(class id, class id idIndex, int64_t & i);
      virtual bool data_get(class id, class id idIndex, string & str);
      virtual bool data_get(class id, class id idIndex, ex1::writable & obj);
      virtual bool data_get(class id, class id idIndex, ex1::byte_serializable & obj);
      virtual bool data_get(class id, class id idIndex, ex1::byte_output_stream & obj);
      virtual bool data_get(class id, class id idIndex, ex1::plain_text_serializable & obj);
      virtual bool data_get(class id, class id idIndex, ex1::plain_text_output_stream & obj);
      virtual bool data_get(class id dataid, class id, class id idIndex, string & str);

      virtual bool data_pulse_change(class id, class id idIndex, update_hint * puh);



      virtual string calc_key(::database::id & idSection, ::database::id & id, ::database::id & idIndex);


      server *  m_pdataserver;
      id        m_dataid;


   };

   class CLASS_DECL_ca client_array :
      public comparable_array < client * , client * >
   {
   public:


      client_array();
      virtual ~client_array();


      virtual void add_client(client * pclient);
      virtual void RemoveClient(client * pclient);
      client * ClientAt(index iClient);
      ::count GetClientCount();


   };


} // namespace database


