﻿#include "framework.h"
//#include "db_str_set.h"


int32_t g_idbchange;

#define new AURA_NEW

db_server::db_server(::aura::application * papp) :
   ::object(papp),
   server(papp)
{

   m_pdb                = NULL;
   m_plongset           = NULL;
   m_pstrset            = NULL;
   m_bWorking           = false;
   m_pfilesystemsizeset = NULL;
   m_bRemote            = true;
   m_psimpledbUser       = NULL;

}


db_server::~db_server()
{

   close();

   finalize();

}


string db_server::calc_data_key(::database::client * pclient,  ::database::id & id)
{

   if(pclient != NULL)
   {

      return pclient->calc_data_key(id);

   }
   else
   {

      return id.m_id;

   }

}


bool db_server::initialize_user(::simpledb::database * pmysqldbUser, const char * pszUser)
{

   if(pmysqldbUser == NULL)
      return false;

   m_bRemote         = false;

   m_psimpledbUser    = pmysqldbUser;
   m_strUser         = pszUser;

   m_plongset        = canew(db_long_set(this));
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

   string strAppName = System.handler()->m_spcommandline->m_strApp;

   ::file::path str;

   //str = ::dir::system() / "database.sqlite";

   if (Application.is_system())
   {

      str = Application.dir().userappdata() / strAppName / "system.sqlite";

   }
   else if (Application.is_session())
   {

      str = Application.dir().userappdata() / strAppName / "session.sqlite";

   }
   else
   {

      str = Application.dir().userappdata() / strAppName / "app.sqlite";

   }

   if(!Application.dir().mk(str.folder()))
   {

      return false;

   }

   m_pdb->setDatabase(str);

   m_pdb->connect();

   m_pdb->create_long_set("integertable");

   m_pdb->create_string_set("stringtable");

   m_plongset     = canew(db_long_set(this));

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

   m_plongset.release();

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

   m_plongset.release();

   m_bWorking = false;

}


bool db_server::data_server_load(::database::client * pclient, ::database::id id, ::file::ostream & writable, ::database::update_hint * phint)
{

   UNREFERENCED_PARAMETER(phint);

   if (!load(calc_data_key(pclient, id), writable))
   {

      return false;

   }

   return true;

}


bool db_server::data_server_save(::database::client * pclient, ::database::id id, ::file::istream & readable, ::database::update_hint * phint)
{

   synch_lock sl(m_pmutex);

   UNREFERENCED_PARAMETER(phint);

   if (!save(calc_data_key(pclient, id), readable))
   {

      return false;

   }

   return true;

}


bool db_server::load(const char * lpcszKey, string & str)
{

   if(get_db_str_set() == NULL)
      return false;

   return get_db_str_set()->load(lpcszKey, str);

}



bool db_server::load(const char * lpKey, ::file::ostream & ostream)
{

   string str;

   if(!load(lpKey, str))
   {

      return false;

   }

   ::file::byte_ostream os(ostream);

   os.write_from_hex(str);

   return true;

}


bool db_server::save(const char * lpcszKey, const char * lpcsz)
{

   synch_lock sl(m_pmutex);

   if(get_db_str_set() == NULL)
      return false;

   return get_db_str_set()->save(lpcszKey, lpcsz);

}


bool db_server::save(const char * lpKey, ::file::istream & istream)
{

   synch_lock sl(m_pmutex);

   string str;

   file::byte_istream is(istream);

   is.seek_to_begin();

   is.read_to_hex(str);

   if(!save(lpKey, str))
      return false;

   return true;

}


sp(::sqlite::base) db_server::get_database()
{

   return m_pdb;

}


bool db_server::data_pulse_change(::database::client * pclient, ::database::id id, ::database::update_hint * puh)
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




