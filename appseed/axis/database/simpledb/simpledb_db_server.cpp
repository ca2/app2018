#include "framework.h"
//#include "db_str_set.h"


int32_t g_idbchange;


#define new AURA_NEW


db_server::db_server(::aura::application * papp) :
   ::object(papp),
   server(papp)
{

   m_pdb                = NULL;
   m_pstrset            = NULL;
   m_bWorking           = false;
   m_pfilesystemsizeset = NULL;
   m_bRemote            = true;
   m_psimpledbUser      = NULL;

}


db_server::~db_server()
{

   close();

   finalize();

}


::database::key db_server::calc_data_key(::database::client * pclient,  ::database::key & key)
{

   if(pclient != NULL)
   {

      return pclient->calc_data_key(key);

   }
   else
   {

      return key;

   }

}


bool db_server::initialize_user(::simpledb::database * pmysqldbUser, const char * pszUser)
{

   if(pmysqldbUser == NULL)
      return false;

   m_bRemote         = false;

   m_psimpledbUser    = pmysqldbUser;
   m_strUser         = pszUser;

//   m_plongset        = canew(db_long_set(this));
   m_pstrset         = canew(db_str_set(this));

   if(!create_message_queue())
      return false;

   m_bWorking = true;

   return true;

}


bool db_server::initialize()
{

   if(System.handler()->m_varTopicQuery["app"] == "app-core/netnodelite"
         || System.handler()->m_varTopicQuery["app"] == "app-core/netnode_dynamic_web_server"
         || System.handler()->m_varTopicQuery["app"] == "app-core/netnode_dynamic_web_server_cfg"
         || System.handler()->m_varTopicQuery["app"] == "app-core/netnodecfg"
         || System.handler()->m_varTopicQuery["app"] == "app-core/mydns"
         || System.handler()->m_varTopicQuery["app"] == "app-gtech/sensible_netnode"
         || System.handler()->m_varTopicQuery["app"] == "app-gtech/sensible_service"
         || System.handler()->m_varTopicQuery.has_property("no_remote_simpledb"))
   {

      m_bRemote = false;

   }

   m_pdb          = canew(::sqlite::base(get_app()));

   sp(::handler) phandler = System.handler();

   sp(command_line) pcommandline = phandler->m_spcommandline;

   string strAppName;

   if(pcommandline.is_null())
   {

      strAppName = System.m_pappcore->m_strAppId;

   }
   else
   {

      strAppName = pcommandline->m_strApp;

   }

   ::file::path str;

   //str = ::dir::system() / "database.sqlite";

   if (Application.is_system())
   {

      str = System.dir().appdata() / "system.sqlite";

   }
   else if (Application.is_session())
   {

      str = System.dir().appdata() / "session.sqlite";

   }
   else
   {

      str = System.dir().appdata() / "app.sqlite";

   }

   if(!Application.dir().mk(str.folder()))
   {

      return false;

   }

   m_pdb->setDatabase(str);

   m_pdb->connect();

   m_pdb->create_long_set("integertable");

   m_pdb->create_string_set("stringtable");

//   m_plongset     = canew(db_long_set(this));

   m_pstrset      = canew(db_str_set(this));

   int32_t iBufferSize = 128 * 1024;

   sp(::handler) commandthread = System.handler();

   if(commandthread->m_varTopicQuery.has_property("filesizebuffer"))
   {

      iBufferSize = commandthread->m_varTopicQuery["filesizebuffer"] * 1024 * 1024;

   }

#if !defined(METROWIN) && !defined(APPLEOS)

   if(!create_message_queue())
      return false;

#endif

   m_bWorking = true;

   return true;

}


bool db_server::finalize()
{

   m_bWorking = false;

   destroy_message_queue();

   //if(m_pfilesystemsizeset != NULL)
   //{
   //   delete m_pfilesystemsizeset;
   //   m_pfilesystemsizeset = NULL;
   //}


   m_pstrset.release();

//   m_plongset.release();

   if(m_pdb != NULL)
   {

      m_pdb->disconnect();

      m_pdb.release();

   }

   return true;

}


bool db_server::create_message_queue()
{

   //if(!IsWindow())
   //{
   //   string strName = "::draw2d::fontopus::message_wnd::simpledb::db_server";
   //   if(!::user::interaction::create_message_queue(strName))
   //   {
   //      return false;
   //   }

   //   //SetTimer(1258477, 484, NULL);

   //   ////IGUI_MSG_LINK(WM_TIMER, m_pimpl, this, &db_server::_001OnTimer);

   //}

   return true;


}

bool db_server::destroy_message_queue()
{
   //if(IsWindow())
   //{
   //   return DestroyWindow() != FALSE;
   //}
   return true;
}

void db_server::_001OnTimer(::timer * ptimer)
{

   //super::_001OnTimer(ptimer);;
   //
   //if(ptimer->m_nIDEvent == 1258477)
   //{

   //}

}


void db_server::close()
{

   m_pstrset.release();

//   m_plongset.release();

   m_bWorking = false;

}


bool db_server::data_server_load(::database::client * pclient, ::database::key key, memory & memory, ::database::update_hint * phint)
{

   UNREFERENCED_PARAMETER(phint);

   if (!load(calc_data_key(pclient, key), memory))
   {

      return false;

   }

   return true;

}


bool db_server::data_server_save(::database::client * pclient, ::database::key key, memory & memory, ::database::update_hint * phint)
{

   synch_lock sl(m_pmutex);

   UNREFERENCED_PARAMETER(phint);

   if (!save(calc_data_key(pclient, key), memory))
   {

      return false;

   }

   return true;

}


bool db_server::load(const ::database::key & key, string & str)
{

   if(get_db_str_set() == NULL)
   {

      return false;

   }

   return get_db_str_set()->load(key, str);

}



bool db_server::load(const ::database::key & key, memory & mem)
{

   string str;

   if(!load(key, str))
   {

      return false;

   }

   try
   {

      mem.from_base64(str);

   }
   catch (...)
   {

      return false;

   }

   return true;

}


bool db_server::save(const ::database::key & key, const char * lpcsz)
{

   synch_lock sl(m_pmutex);

   if(get_db_str_set() == NULL)
   {

      return false;

   }

   return get_db_str_set()->save(key, lpcsz);

}


bool db_server::save(const ::database::key & key, memory & mem)
{

   synch_lock sl(m_pmutex);

   string str;

   mem.to_base64(str);

   if(!save(key, str))
   {

      return false;

   }

   return true;

}


sp(::sqlite::base) db_server::get_database()
{

   return m_pdb;

}


bool db_server::data_pulse_change(::database::client * pclient, ::database::key id, ::database::update_hint * puh)
{

   return ::database::server::data_pulse_change(pclient, id, puh);

}


void db_server::assert_valid() const
{

   // return void HAHAHAHAHAHA

}


void db_server::dump(dump_context &) const
{

   // return void HAHAHAHAHAHA

}


db_str_set * db_server::get_db_str_set()
{

   return m_pstrset;

}




