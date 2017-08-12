//#include "framework.h"


namespace simpledb
{


   simpledb::simpledb(::aura::application * papp) :
      ::object(papp),
      ::aura::department(papp)
   {

      m_pserver         = NULL;
      m_bInitialized    = false;

   }


   simpledb::~simpledb()
   {

      FinalizeDataCentral();

   }


   bool simpledb::InitializeDataCentral()
   {

      if (m_bInitialized)
      {

         return true;

      }

      if (m_pserver != NULL)
      {

         return true;

      }

      if(m_pauraapp->is_system())
      {
//#ifndef METROWIN
//         /* initialize client library */
//         if (mysql_library_init (0, NULL, NULL))
//         {
//            TRACE("mysql_library_init() failed\n");
//            return false;
//         }
//#endif
      }

         m_pserver = new db_server(m_pauraapp);


         m_pserver->add_client(this);

         if (!m_pserver->initialize())
         {
            Application.simple_message_box(NULL, "Could not initialize simpledb.", MB_OK);
            return false;
         }

      m_bInitialized = true;

      return true;

   }


   void simpledb::on_set_locale(const char * lpcsz, ::action::context actioncontext)
   {
      if(actioncontext.is_user_source())
      {
         data_set(".local://locale", lpcsz);
      }
      m_pauraapp->m_paxissession->on_set_locale(lpcsz, actioncontext);
   }

   void simpledb::on_set_schema(const char * lpcsz, ::action::context actioncontext)
   {
      if(actioncontext.is_user_source())
      {
         data_set(".local://schema", lpcsz);
      }
      m_pauraapp->m_paxissession->on_set_schema(lpcsz,actioncontext);
   }

   bool simpledb::FinalizeDataCentral()
   {

      if (!m_bInitialized)
      {

         return true;

      }

      if (m_pserver == NULL)
      {

         return true;

      }

      try
      {
         
         m_pserver->finalize();

      }
      catch(...)
      {

      }


      ::aura::del(m_pserver);

      m_pserver = NULL;

      m_bInitialized = false;

      return true;

   }

   bool simpledb::initialize2()
   {

      if(m_pauraapp->handler()->m_varTopicQuery["locale"].get_count() > 0)
      {
         string str = m_pauraapp->handler()->m_varTopicQuery["locale"].stra()[0];
         m_pauraapp->m_paxissession->set_locale(str,::action::source::database());
      }

      if(m_pauraapp->handler()->m_varTopicQuery["schema"].get_count() > 0)
      {
         string str = m_pauraapp->handler()->m_varTopicQuery["schema"].stra()[0];
         m_pauraapp->m_paxissession->set_schema(str,::action::source::database());
      }

//      if(&AppUser(this) == NULL)
  //       return false;

      if(!InitializeDataCentral())
      {
         Application.simple_message_box(NULL, "Could not initialize data central");
         return false;
      }

      ::database::client::initialize_data_client(m_pserver);

      

//      ::core::application_request * prequest = System.get_application_request();


      m_pauraapp->m_paxissession->fill_locale_schema(*m_pauraapp->m_paxissession->str_context()->m_plocaleschema);


      //if(!m_pauraapp->m_pcoreapp->is_installing() && !m_pauraapp->m_pcoreapp->is_uninstalling() && !m_pauraapp->m_pcoreapp->is_system())
      //{

      //   set_keyboard_layout(NULL, ::action::source::system());

      //}


      return true;

   }




   bool simpledb::initialize()
   {



      if(!::database::database::initialize())
         return false;



      return true;


   }


   bool simpledb::finalize()
   {

      try
      {
       //  ::database::database::finalize();
      }
      catch(...)
      {
      }

      try
      {
         FinalizeDataCentral();
      }
      catch(...)
      {
      }


      return true;


   }


   ::database::server * simpledb::get_data_server()
   {

      return m_pserver;

   }


   db_server & simpledb::db()
   {

      return *m_pserver;

   }




   void simpledb::on_set_keyboard_layout(const char * pszPath, ::action::context actioncontext)
   {

      if(actioncontext.is_user_source())
      {

         if(Sess(m_pauraapp).safe_get_user() != NULL)
         {

            data_set("keyboard_layout", pszPath);

         }

      }

   }


} // namespace simpledb



