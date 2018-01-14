﻿#include "framework.h"


namespace database
{


   server::server(::aura::application * papp) :
      object(papp)
   {

      defer_create_mutex();

   }


   server::~server()
   {
   }


   bool server::data_server_load(client * pclient, class id id, ::file::file & writable, update_hint * puh)
   {

      file::ostream ostream(&writable);
      return var_load(pclient, id, ostream, puh);
   }

   bool server::data_server_load(client * pclient, class id id, var & var, update_hint * puh)
   {

      memory_file file(get_app());

      ::file::byte_stream stream(&file);

#if MEMDLEAK

      file.get_memory()->m_strTag = "memory://member=database::server::data_server_load";

#endif

      if (!data_server_load(pclient, id, stream, puh) || file.get_length() <= 0)
      {

         return false;

      }

      try
      {

         file.seek_to_begin();

         stream >> var;

      }
      catch(...)
      {

         return false;

      }

      return true;

   }


   bool server::data_server_load(client * pclient, class id id, ::file::ostream & ostream, update_hint * puh)
   {

      return var_load(pclient, id, ostream, puh);

   }

   bool server::data_server_load(client * pclient, class id id, ::file::serializable & obj, update_hint * puh)
   {

      memory_file file(get_app());

      ::file::byte_stream stream(&file);

      if (!data_server_load(pclient, id, stream, puh) || file.get_length() <= 0)
      {

         return false;

      }

      try
      {

         file.seek_to_begin();

         obj.read(stream);

      }
      catch(...)
      {

         return false;

      }

      return file.get_position() == file.get_length();

   }


   bool server::data_server_save(client * pclient, class id id, ::file::file & readable, update_hint * puh)
   {
      ::file::istream istream(&readable);
      return var_save(pclient, id, istream, puh);
   }

   bool server::data_server_save(client * pclient, class id id, var & var, update_hint * puh)
   {

      memory_file file(get_app());

      ::file::byte_stream stream(&file);

#if MEMDLEAK

      file.get_memory()->m_strTag = "memory://member=database::server::data_server_save(1)";

#endif

      stream << var;

      file.seek_to_begin();

      if (!data_server_save(pclient, id, stream, puh))
      {

         return false;

      }

      return true;

   }


   bool server::data_server_save(client * pclient, class id id, ::file::istream & istream, update_hint * puh)
   {
      return var_save(pclient, id,  istream, puh);
   }

   bool server::data_server_save(client * pclient, class id id, ::file::serializable & obj, update_hint * puh)
   {

      memory_file file(get_app());

      ::file::byte_stream stream(&file);

#if MEMDLEAK

      file.get_memory()->m_strTag = "memory://member=database::server::data_server_save(2)";

#endif

      obj.write(stream);

      file.seek_to_begin();

      if (!data_server_save(pclient, id, stream, puh))
      {

         return false;

      }

      return true;

   }


   bool server::data_pulse_change(client * pclient, class id id, update_hint * puh)
   {
      return on_after_data_change(pclient, id, puh);
   }


   bool server::on_before_data_change(client * pclient, class id id, var & var, update_hint * puh)
   {
      ::database::change_event signal(var);
      signal.m_pserver = this;
      signal.m_key.m_pclient = pclient;
      signal.m_key.m_id = id;
      signal.m_puh = puh;
      for(int32_t i = 0; i < client_array::get_count(); i++)
      {
         client_array::element_at(i)->data_on_before_change(&signal);
         if(!signal.m_bOk)
            return false;

      }
      return true;
   }

   bool server::on_after_data_change(client * pclient, class id id, update_hint * puh)
   {
      ::database::change_event signal;
      signal.m_pserver       = this;
      signal.m_key.m_pclient     = pclient;
      signal.m_key.m_id          = id;
      signal.m_puh = puh;
      for(int32_t i = 0; i < client_array::get_count(); i++)
      {
         client_array::element_at(i)->data_on_after_change(&signal);
      }
      return true;
   }

   var server::data_load(client * pclient, class id id, update_hint * phint)
   {
      var var;
      if(data_server_load(pclient, id, var, phint))
         return var;
      return ::var(::var::type_new);
   }

   bool server::data_save(client * pclient, class id id, var var, update_hint * phint)
   {
      return data_server_save(pclient, id, var, phint);
   }

   bool server::var_load(client * pclient, class id id, ::file::ostream & ostream, update_hint * puh)
   {
      var var;
      if(!data_server_load(pclient, id, var, puh))
         return false;
      var.write(ostream);
      return true;
   }

   bool server::var_save(client * pclient, class id id, ::file::istream & istream, update_hint * puh)
   {
      var var;
      var.read(istream);
      if(!data_server_save(pclient, id, var, puh))
         return false;
      return true;
   }



} // namespace database
