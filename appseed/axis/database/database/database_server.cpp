#include "framework.h"


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


   bool server::data_server_load(client * pclient, key id, ::file::file & file, update_hint * puh)
   {

      memory mem;

      {

    //     synch_lock sl(m_pmutex);

         if (!data_server_load(pclient, id, mem, puh))
         {

            return false;

         }

      }

      file.write(mem.get_data(), mem.get_size());

      return true;

   }


   bool server::data_server_load(client * pclient, key id, memory & mem, update_hint * puh)
   {

#if MEMDLEAK

      mem.m_strTag = "memory://member=database::server::data_server_load(1)";

#endif

      return true;

   }


   bool server::data_server_load(client * pclient, key id, serialize & serialize, update_hint * puh)
   {

      memory mem(get_app());

      {

  //       synch_lock sl(m_pmutex);

         if (!data_server_load(pclient, id, mem, puh))
         {

            return false;

         }

      }

      serialize.write(mem.get_data(), mem.get_size());

      return true;

   }


   bool server::data_server_load(client * pclient, key id, ::object & obj, update_hint * puh)
   {

      memory_reader reader(get_app());

      {

//         synch_lock sl(m_pmutex);

         if (!data_server_load(pclient, id, reader.get_memory(), puh))
         {

            return false;

         }

      }

      try
      {

         reader(obj);

      }
      catch(...)
      {

         return false;

      }

      return true;

   }


   bool server::data_server_save(client * pclient, key id, ::file::file & file, update_hint * puh)
   {

      memory mem;

      mem.allocate(file.get_length() - file.get_position());

      file.read(mem.get_data(), mem.get_size());

      {

        // synch_lock sl(m_pmutex);

         if (!data_server_save(pclient, id, mem, puh))
         {

            return false;

         }

      }

      return true;

   }


   bool server::data_server_save(client * pclient, key id, memory & mem, update_hint * puh)
   {

#if MEMDLEAK

      mem.m_strTag = "memory://member=database::server::data_server_save(1)";

#endif

      return true;

   }


   bool server::data_server_save(client * pclient, key id, serialize & serialize, update_hint * puh)
   {

      memory mem;

      mem.allocate(serialize.get_left());

      serialize.read(mem.get_data(), mem.get_size());

      {

      //   synch_lock sl(m_pmutex);

         if (!data_server_save(pclient, id, mem, puh))
         {

            return false;

         }

      }

      return true;

   }


   bool server::data_server_save(client * pclient, key id, ::object & obj, update_hint * puh)
   {

      memory_writer writer(get_app());

      try
      {

         writer(obj);

      }
      catch (...)
      {

         return false;

      }

      {

    //     synch_lock sl(m_pmutex);

         if (!data_server_save(pclient, id, writer.get_memory(), puh))
         {

            return false;

         }

      }

      return true;

   }


   bool server::data_pulse_change(client * pclient, key id, update_hint * puh)
   {

      return on_after_data_change(pclient, id, puh);

   }


   bool server::on_before_data_change(client * pclient, key id, var & var, update_hint * puh)
   {

      ::database::change_event signal(var);

      signal.m_pserver = this;
      signal.m_pclient = pclient;
      signal.m_datakey = id;
      signal.m_puh = puh;

      for(int32_t i = 0; i < client_array::get_count(); i++)
      {

         client_array::element_at(i)->data_on_before_change(&signal);

         if(!signal.m_bOk)
         {

            return false;

         }

      }
      return true;
   }

   bool server::on_after_data_change(client * pclient, key id, update_hint * puh)
   {
      ::database::change_event signal;
      signal.m_pserver       = this;
      signal.m_pclient     = pclient;
      signal.m_datakey          = id;
      signal.m_puh = puh;
      for(int32_t i = 0; i < client_array::get_count(); i++)
      {
         client_array::element_at(i)->data_on_after_change(&signal);
      }
      return true;
   }


   var server::data_load(client * pclient, key id, update_hint * phint)
   {

      memory_reader reader(get_app());

      {

  //       synch_lock sl(m_pmutex);

         if (!data_server_load(pclient, id, reader.get_memory(), phint))
         {

            return var::type_null;

         }

      }

      var var;

      reader(var);

      return var;

   }


   bool server::data_save(client * pclient, key id, var & var, update_hint * phint)
   {

      memory_reader writer(get_app());

      writer(var);

      {

//         synch_lock sl(m_pmutex);

         if (!data_server_load(pclient, id, writer.get_memory(), phint))
         {

            return false;

         }

      }

      return true;

   }


} // namespace database



