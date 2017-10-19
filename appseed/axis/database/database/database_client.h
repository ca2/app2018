﻿#pragma once


namespace database
{


   class client;


} // namespace database


namespace file
{


   class CLASS_DECL_AXIS data_trigger_ostream :
      virtual public byte_stream
   {
      public:


         memory_file             m_file;
         ::database::client *    m_pclient;
         class ::database::id    m_id;


         data_trigger_ostream(data_trigger_ostream && d);
         data_trigger_ostream(::database::client * pclient,class ::database::id);
         virtual ~data_trigger_ostream();


   };


   class CLASS_DECL_AXIS data_trigger_istream :
      virtual public byte_stream
   {
      public:


         memory_file    m_file;


         data_trigger_istream(data_trigger_istream && d);
         data_trigger_istream(::database::client * pclient,class ::database::id);
         virtual ~data_trigger_istream();

   };


} // namespace file


namespace database
{


   class client_array;




   class CLASS_DECL_AXIS client :
      virtual public ::object
   {
      public:


         friend class client_array;
         friend class server;


         server *  m_pdataserver;
         id        m_dataid2;



         client();
         virtual ~client();


         virtual bool set_data_server(server * pserver);


         void initialize_data_client(server * pserver);

         DECL_GEN_SIGNAL(data_on_before_change);
         DECL_GEN_SIGNAL(data_on_after_change);

         //virtual bool data_set(class id, bool b, update_hint * phint = NULL);
         //virtual bool data_set(class id, const char * lpsz, update_hint * puh = NULL);
         //virtual bool data_set(class id, int32_t i, update_hint * puh = NULL);
         //virtual bool data_set(class id, int64_t i, update_hint * puh = NULL);

         //virtual bool data_get(class id, bool & b);
         //virtual bool data_get(class id, string & str);
         //virtual bool data_get(class id, int32_t & i);
         //virtual bool data_get(class id, int64_t & i);

         virtual bool data_set(class id, bool b, update_hint * phint = NULL);
         virtual bool data_set(class id, var & var, update_hint * puh = NULL);
         virtual bool data_set(class id, int32_t i, update_hint * puh = NULL);
         virtual bool data_set(class id, int64_t i, update_hint * puh = NULL);
#ifdef APPLEOS
         virtual bool data_set(class id, long l, update_hint * puh = NULL);
#endif
         virtual bool data_set(class id, const char * lpsz, update_hint * puh = NULL);
         virtual bool data_set(class id, const unichar * lpsz, update_hint * puh = NULL);
         virtual bool data_set(class id, ::file::file & obj, update_hint * puh = NULL);
         virtual bool data_set(class id, ::file::serializable & obj, update_hint * puh = NULL);
         virtual bool data_set(class id, ::file::istream & obj, update_hint * puh = NULL);
//      virtual bool data_set(class id, ::file::serializable & obj, update_hint * puh = NULL);
         //virtual bool data_set(class id, ::core::plain_text_istream & obj, update_hint * puh = NULL);
         virtual bool data_set(selection & selection, const char * lpsz, update_hint * puh = NULL);
         virtual bool data_set(selection & selection, var & var, update_hint * puh = NULL);

         //virtual bool data_set(class id dataid, class id,  const char * lpsz, update_hint * puh = NULL);

         template < typename T >
         inline bool data_save(class id id, const T & t)
         {
            try
            {

               ::file::data_trigger_ostream os(this,id);

               os << t;

            }
            catch(...)
            {
               return false;
            }
            return true;
         }


         virtual bool data_get(class id,  bool & b);
         virtual bool data_get(class id, var & var);
         virtual bool data_get(class id, int32_t & i);
         virtual bool data_get(class id, int64_t & i);
#ifdef APPLEOS
         virtual bool data_get(class id, long & l);
#endif
         virtual bool data_get(class id, string & str);
         virtual bool data_get(class id, ::file::file & obj);
         virtual bool data_get(class id, ::file::serializable & obj);
         virtual bool data_get(class id, ::file::ostream & obj);
//      virtual bool data_get(class id, ::file::serializable & obj);
         //virtual bool data_get(class id, ::core::plain_text_ostream & obj);
//      virtual bool data_get(class id dataid, class id, string & str);


         template < typename T >
         inline bool data_load(class id id,T & t)
         {
            try
            {

               ::file::data_trigger_istream is(this,id);

               is >> t;

               if(is.fail())
                  return false;

            }
            catch(...)
            {
               return false;
            }
            return true;
         }

         virtual bool data_pulse_change(class id, update_hint * puh);



         virtual string calc_data_key(::database::id & id);

         virtual id get_data_id();
         virtual string calc_data_id();
         virtual void update_data_id();



   };

   class CLASS_DECL_AXIS client_array :
      public ref_array < client >
   {
      public:


         client_array();
         virtual ~client_array();


         virtual void add_client(client * pclient);
         virtual void remove_client(client * pclient);
         client * client_at(index iClient);
         ::count get_client_count();


   };


} // namespace database


