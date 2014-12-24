#include "framework.h"
#include "axis/net/net_sockets.h"
#include "db_str_set.h"


class CLASS_DECL_AXIS db_str_set_item
{
public:


   uint32_t          m_dwTimeout;
   string            m_str;


};


class CLASS_DECL_AXIS db_str_set_queue_item:
   virtual public element
{
public:

   string            m_strKey;
   uint32_t          m_dwTimeout;
   string            m_str;


   db_str_set_queue_item() {}
   db_str_set_queue_item(const db_str_set_queue_item & item){ operator =(item); }
   virtual ~db_str_set_queue_item() {  }


   db_str_set_queue_item & operator = (const db_str_set_queue_item & item)
   {
      if(this != &item)
      {
         m_strKey = item.m_strKey;
         m_dwTimeout = item.m_dwTimeout;
         m_str = item.m_str;
      }
      return *this;
   }



};


class CLASS_DECL_AXIS db_str_set_core:
   virtual public ::db_set
{
public:


   mutex                                        m_mutex;
   sockets::socket_handler                      m_handler;
   sockets::http_session *                      m_phttpsession;
   string_map < db_str_set_item >               m_map;
   bool                                         m_bIndexed;
   ::simpledb::database *                          m_psimpledbUser;
   string                                       m_strUser;

   class db_str_sync_queue *                    m_pqueue;




   db_str_set_core(db_server * pserver):
      element(pserver->get_app()),
      db_set(pserver,"stringtable"),
      m_handler(get_app()),
      m_mutex(get_app()),
      m_phttpsession(NULL),
      m_pqueue(NULL),
      m_psimpledbUser(pserver->m_psimpledbUser),
      m_strUser(pserver->m_strUser)
   {


   }

   ~db_str_set_core()
   {

   }


};


class CLASS_DECL_AXIS db_str_sync_queue:
   public simple_thread
{
public:

   mutex                                              m_mutex;
   db_str_set *                                       m_pset;
   sockets::socket_handler                            m_handler;
   sockets::http_session *                            m_phttpsession;

   smart_pointer_array < db_str_set_queue_item >      m_itema;


   db_str_sync_queue(::aura::application * papp):
      element(papp),
      thread(papp),
      simple_thread(papp),
      m_handler(papp),
      m_mutex(papp),
      m_phttpsession(NULL)
   { }

   virtual ~db_str_sync_queue() {}


   virtual int32_t run();

   void queue(const char * pszKey,const char * psz);

};






int32_t db_str_sync_queue::run()
{

   single_lock sl(&m_mutex, false);

   m_bRun = true;

   try
   {

      while(get_run())
      {

repeat:;

      if(!get_run())
         break;
       {

          sl.lock();

          if(m_itema.get_size() <= 0)
          {
             sl.unlock();
             Sleep(1984);
             goto repeat;
          }

          if(Session.fontopus()->m_puser == NULL)
          {
             sl.unlock();
             Sleep(5000);
             goto repeat;
          }

          for(int32_t i = 1; i < m_itema.get_size(); i++)
          {
             if(m_itema[i]->m_strKey == m_itema[0]->m_strKey)
             {
                m_itema.remove_at(0);
                sl.unlock();
                goto repeat;
             }
          }

          try
          {


             property_set set(get_app());

             string strUrl;

             set["interactive_user"] = true;




             strUrl = "https://" + System.dir().get_api_cc() + "/account/str_set_save?key=";
             strUrl += System.url().url_encode(m_itema[0]->m_strKey);
             strUrl += "&value=";
             strUrl += System.url().url_encode(m_itema[0]->m_str);

             m_itema.remove_at(0);

             sl.unlock();

             set["user"] = &ApplicationUser;

             m_phttpsession = System.http().request(m_phttpsession, strUrl, set);

             if(m_phttpsession == NULL || ::http::status_failed(set["get_status"]))
             {
                Sleep(1984);
                System.dir().m_strApiCc = "";
                goto repeat;
             }

          }
          catch(...)
          {
          }

          sl.unlock();

       }


      }
   }
   catch(...)
   {
   }
   m_pset->m_pcore->m_pqueue = NULL;
   return 0;
}


void db_str_sync_queue::queue(const char * pszKey,const char * psz)
{

   single_lock sl(&m_mutex, true);

   sp(db_str_set_queue_item) item(new db_str_set_queue_item);

   item->m_strKey = pszKey;
   item->m_str = psz;

   m_itema.add(item);

}


db_str_set::db_str_set(db_server * pserver):
element(pserver->get_app()),
m_mutex(pserver->get_app())
{

   m_pcore = new db_str_set_core(pserver);

}

db_str_set::~db_str_set()
{

   ::aura::del(m_pcore);

}




// true if deleted
bool db_str_set::remove(const char * lpKey)
{
   UNREFERENCED_PARAMETER(lpKey);
   return false;
}

bool db_str_set::load(const char * lpKey, string & strValue)
{

   if(m_pcore->m_pdataserver == NULL)
      return false;

   if(m_pcore->m_pdataserver->m_bRemote && string(lpKey).find(".local://") < 0)
   {

      Application.assert_user_logged_in();

      synch_lock sl(&m_mutex);

      if(m_pcore->m_phttpsession == NULL)
      {

         m_pcore->m_phttpsession = Session.fontopus()->m_mapFontopusSession[Session.fontopus()->m_strFirstFontopusServer];

      }

      db_str_set_item stritem;

      if(m_pcore->m_map.Lookup(lpKey,stritem) && stritem.m_dwTimeout > get_tick_count())
      {
         strValue = stritem.m_str;
         return true;
      }

      single_lock slDatabase(m_pcore->db()->get_database()->m_pmutex,true);

      property_set set(get_app());

      set["interactive_user"] = true;

      string strUrl;

      strUrl = "https://" + System.dir().get_api_cc() + "/account/str_set_load?key=";

      strUrl += System.url().url_encode(lpKey);

      set["user"] = &ApplicationUser;

      set["get_response"] = "";

      m_pcore->m_phttpsession = System.http().request(m_pcore->m_phttpsession,strUrl,set);

      if(m_pcore->m_phttpsession == NULL || ::http::status_failed(set["get_status"]))
      {
         return false;
      }

      strValue = set["get_response"];

      stritem.m_dwTimeout = get_tick_count() + 23 * (5000);
      stritem.m_str = strValue;

      m_pcore->m_map.set_at(lpKey,stritem);


   }
#ifndef METROWIN
   else if(m_pcore->m_psimpledbUser != NULL)
   {

      try
      {

         strValue = m_pcore->m_psimpledbUser->query_item("SELECT `value` FROM fun_user_str_set WHERE user = '" + m_pcore->m_strUser + "' AND `key` = '" + m_pcore->m_psimpledbUser->real_escape_string(lpKey) + "'");

         return true;

      }
      catch(...)
      {
      }

      return false;

   }
#endif
   else
   {
      single_lock slDatabase(m_pcore->db()->get_database()->m_pmutex);

      string strKey;
      strKey = lpKey;
      strKey.replace("'", "''");

      string strSql;
      strSql.Format(
         "select value FROM stringtable WHERE id = '%s';",
         strKey);


      slDatabase.lock();
      try
      {
         m_pcore->m_pdataset->query(strSql);
      }
      catch(...)
      {
         return false;
      }

      if(m_pcore->m_pdataset->num_rows() <= 0)
         return false;

      strValue = m_pcore->m_pdataset->fv("value");

   }

   return true;
}

bool db_str_set::save(const char * lpKey, const char * lpcsz)
{

   if(m_pcore->m_pdataserver == NULL)
      return false;

   if(!m_pcore->m_pdataserver->m_bRemote || string(lpKey).find(".local://") >= 0)
   {
      if(m_pcore->db() == NULL)
         return false;
      single_lock slDatabase(m_pcore->db()->get_database()->m_pmutex);

      string strKey;
      strKey = lpKey;
      strKey.replace("'", "''");

      string strValue(lpcsz);
      strValue.replace("'", "''");

      sp(::sqlite::base) pdb   = m_pcore->db()->get_database();
      string strSql;
      string str;
      slDatabase.lock();
      if(load(lpKey, str))
      {
         strSql.Format(
            "UPDATE stringtable SET value = '%s' WHERE id = '%s';",
            strValue,
            strKey);

         pdb->start_transaction();
         if(!m_pcore->m_pdataset->exec(strSql))
         {
            pdb->rollback_transaction();
            return false;
         }
         pdb->commit_transaction();
      }
      else
      {

         strSql.Format(
            "INSERT INTO stringtable (id, value) values ('%s', '%s');",
            strKey,
            strValue);

         pdb->start_transaction();
         if(!m_pcore->m_pdataset->exec(strSql))
         {
            pdb->rollback_transaction();
            return false;
         }
         pdb->commit_transaction();
      }
      return true;
   }
#ifdef HAVE_MYSQL
   else if(m_pcore->m_psimpledbUser != NULL)
   {

      string strSql = "REPLACE INTO fun_user_str_set VALUE('" + m_pcore->m_strUser + "', '" + m_pcore->m_psimpledbUser->real_escape_string(lpKey) + "', '" + m_pcore->m_psimpledbUser->real_escape_string(lpcsz) + "')";

      TRACE(strSql);

      return m_pcore->m_psimpledbUser->query(strSql) != NULL;

   }
#endif
   else
   {


      if(m_pcore->m_pqueue == NULL)
      {

         m_pcore->m_pqueue = new db_str_sync_queue(get_app());
         m_pcore->m_pqueue->m_pset = this;
         m_pcore->m_pqueue->begin();

      }


      m_pcore->m_pqueue->queue(lpKey,lpcsz);

      db_str_set_item stritem;

      stritem.m_dwTimeout = get_tick_count() + 23 * (5000);
      stritem.m_str = lpcsz;

      m_pcore->m_map.set_at(lpKey,stritem);


      return true;

   }

}
