#include "framework.h"
//#include "axis/net/net_sockets.h"


class CLASS_DECL_AXIS db_long_set_item
{
public:

   uint32_t       m_dwTimeout;
   int64_t        m_l;

};


class CLASS_DECL_AXIS db_long_set_queue_item :
   virtual public object
{
public:

   string         m_strKey;
   uint32_t       m_dwTimeout;
   int64_t        m_l;

   db_long_set_queue_item() {}
   db_long_set_queue_item(const db_long_set_queue_item & item) { operator =(item); }
   virtual ~db_long_set_queue_item() {}


   db_long_set_queue_item & operator = (const db_long_set_queue_item & item)
   {
      if (this != &item)
      {
         m_strKey = item.m_strKey;
         m_dwTimeout = item.m_dwTimeout;
         m_l = item.m_l;
      }
      return *this;
   }

};



class CLASS_DECL_AXIS db_long_set_core :
   virtual public db_set
{
public:


   sockets::socket_handler                   m_handler;
   sp(sockets::http_session)                 m_phttpsession;


   string_map < db_long_set_item >           m_map;
   bool                                      m_bIndexed;

   ::simpledb::database *                       m_psimpledbUser;
   string                                    m_strUser;

   sp(class db_long_sync_queue)                m_pqueue;

   db_long_set_core(db_server * pserver) :
      ::object(pserver->get_app()),
      db_set(pserver, "integertable"),
      m_handler(get_app()),
      m_phttpsession(NULL),
      m_pqueue(NULL),
      m_psimpledbUser(pserver->m_psimpledbUser),
      m_strUser(pserver->m_strUser)
   {

      m_ptopthis = this;
      defer_create_mutex();

   }

   virtual ~db_long_set_core() {}

};


class CLASS_DECL_AXIS db_long_sync_queue :
   public simple_thread
{
public:


   mutex                                                    m_mutex;
   db_long_set *                                            m_pset;
   sockets::socket_handler                                  m_handler;
   sp(sockets::http_session)                                m_phttpsession;

   smart_pointer_array < db_long_set_queue_item >           m_itema;

   db_long_sync_queue(::aura::application * papp) :
      ::object(papp),
      thread(papp),
      simple_thread(papp),
      m_handler(papp),
      m_mutex(papp),
      m_phttpsession(NULL)
   {}

   virtual ~db_long_sync_queue() {}



   virtual int32_t run();


   void queue(const char * pszKey, int64_t l);

};


int32_t db_long_sync_queue::run()
{

   single_lock sl(&m_mutex, false);

   try
   {

      while (get_run_thread())
      {

         try
         {

            if (m_pauraapp == NULL)
            {

               break;

            }

            if (m_pauraapp->m_paurasession == NULL)
            {

               break;

            }

            if (m_pauraapp->m_paurasession->m_pfontopus == NULL)
            {

               break;

            }

            if (&ApplicationUser == NULL)
            {

               Sleep(5000);

               continue;

            }

         }
         catch (...)
         {

            break;

         }

         sl.lock();

         if (m_itema.is_empty())
         {

            sl.unlock();

            Sleep(100);

            continue;

         }

         for (int32_t i = 1; i < m_itema.get_size(); i++)
         {

            if (m_itema[i]->m_strKey == m_itema[0]->m_strKey)
            {

               m_itema.remove_at(0);

               sl.unlock();

               continue;

            }

         }

         sp(db_long_set_queue_item) pitem = m_itema[0];

         m_itema.remove_at(0);

         sl.unlock();

         try
         {

            property_set set(get_app());

            string strUrl;

            set["interactive_user"] = true;

            strUrl = "https://" + System.dir().get_api_cc() + "/account/str_set_save?key=";

            strUrl += System.url().url_encode(pitem->m_strKey);

            strUrl += "&value=";

            strUrl += ::str::from(m_itema[0]->m_l);

            set["user"] = &ApplicationUser;

            {

               single_lock slDatabase(m_pset->m_pcore->db()->get_database()->m_pmutex, true);

               Application.http().get(strUrl, set);

            }

            if (::http::status_failed(set["get_status"]))
            {

               Sleep(500);

               System.dir().m_strApiCc = "";

            }

         }
         catch (...)
         {

         }

      }

   }
   catch (...)
   {

   }

   ((db_long_set_core *)(m_pset->m_pcore->m_ptopthis))->m_pqueue = NULL;

   return 0;

}


void db_long_sync_queue::queue(const char * pszKey, int64_t l)
{

   single_lock sl(&m_mutex, true);

   db_long_set_queue_item item;

   item.m_strKey = pszKey;
   item.m_l = l;

   m_itema.add(canew(db_long_set_queue_item(item)));

}


db_long_set::db_long_set(db_server * pserver) :
   ::object(pserver->get_app())
{

   m_pcore = canew(db_long_set_core(pserver));

}

db_long_set::~db_long_set()
{


}

// Adiciona na matriz System nomes dos diret�rios de imagens.
bool db_long_set::load(const char * lpKey, int64_t * plValue)
{

   db_long_set_core * pcore = (db_long_set_core *)m_pcore->m_ptopthis;

   if (m_pcore->m_pdataserver->m_bRemote && string(lpKey).find_ci(".local://") < 0)
   {

      // Remote

      db_long_set_item longitem;

      if (pcore->m_map.Lookup(lpKey, longitem) && longitem.m_dwTimeout > get_tick_count())
      {

         *plValue = longitem.m_l;

         return true;

      }




      property_set set(get_app());

      string strUrl;

      set["interactive_user"] = true;

      strUrl = "https://" + System.dir().get_api_cc() + "/account/long_set_load?key=";
      strUrl += System.url().url_encode(lpKey);

      //m_phttpsession = System.http().request(m_handler, m_phttpsession, strUrl, post, headers, set, NULL, &ApplicationUser, NULL, &estatus);
      //pcore-> m_phttpsession = System.http().request(pcore->m_handler, pcore->m_phttpsession,strUrl,set);

      string strValue = Application.http().get(strUrl, set);

      if (strValue.is_empty() || ::http::status_failed(set["get_status"]))
      {
         return false;
      }

      *plValue = ::str::to_int64(strValue);

      longitem.m_dwTimeout = get_tick_count() + 23 * (5000);
      longitem.m_l = *plValue;

      pcore->m_map.set_at(lpKey, longitem);
      return true;

   }
#ifndef METROWIN
   else if (pcore->m_psimpledbUser != NULL)
   {

      try
      {

         *plValue = pcore->m_psimpledbUser->query_item("SELECT `value` FROM fun_user_str_set WHERE user = '" + pcore->m_strUser + "' AND `key` = '" + pcore->m_psimpledbUser->real_escape_string(lpKey) + "'").int32();

         return true;

      }
      catch (...)
      {
      }

      return false;

   }
#endif
   else
   {
      // LOCAL (sqlite)

      single_lock slDatabase(m_pcore->db()->get_database()->m_pmutex);

      string strKey;
      strKey = lpKey;
      strKey.replace("'", "''");

      string strSql;
      strSql.Format(
         "select value FROM integertable WHERE id = '%s';",
         strKey);


      slDatabase.lock();
      //try
      {
         m_pcore->m_pdataset->query(strSql);
      }
      //catch(...)
      {
         // return false;
      }

      if (m_pcore->m_pdataset->num_rows() <= 0)
         return false;

      *plValue = m_pcore->m_pdataset->fv("value");

      return true;

   }


}

bool db_long_set::save(const char * lpKey, int64_t lValue)
{

   db_long_set_core * pcore = (db_long_set_core *)m_pcore->m_ptopthis;

   if (m_pcore->m_pdataserver->m_bRemote)
   {

      if (pcore->m_pqueue == NULL)
      {

         pcore->m_pqueue = canew(db_long_sync_queue(get_app()));
         pcore->m_pqueue->m_pset = this;
         pcore->m_pqueue->begin();

      }

      pcore->m_pqueue->queue(lpKey, lValue);

      db_long_set_item longitem;

      longitem.m_dwTimeout = get_tick_count() + 23 * (5000);
      longitem.m_l = lValue;

      pcore->m_map.set_at(lpKey, longitem);

      return true;

   }
#ifndef METROWIN
   else if (pcore->m_psimpledbUser != NULL)
   {

      string strSql = "REPLACE INTO fun_user_long_set VALUE('" + pcore->m_strUser + "', '" + pcore->m_psimpledbUser->real_escape_string(lpKey) + "', " + ::str::from(lValue) + ")";

      TRACE(strSql);

      return pcore->m_psimpledbUser->query(strSql) != NULL;

   }
#endif
   else
   {
      single_lock slDatabase(m_pcore->db()->get_database()->m_pmutex);
      string strKey;
      strKey = lpKey;
      strKey.replace("'", "''");


      sp(::sqlite::base) pdb = m_pcore->db()->get_database();
      string strSql;
      int64_t l;
      slDatabase.lock();
      if (load(lpKey, &l))
      {
         strSql.Format(
            "UPDATE integertable SET value = '%d' WHERE id = '%s';",
            lValue,
            strKey);

         try
         {
            pdb->start_transaction();
            m_pcore->m_pdataset->exec(strSql);
            pdb->commit_transaction();
         }
         catch (...)
         {
            pdb->rollback_transaction();
            return false;
         }
      }
      else
      {

         strSql.Format(
            "INSERT INTO integertable (id, value) values ('%s', '%d');",
            strKey,
            lValue);



         try
         {
            pdb->start_transaction();
            m_pcore->m_pdataset->exec(strSql);
            pdb->commit_transaction();
         }
         catch (...)
         {
            pdb->rollback_transaction();
            return false;
         }
      }

      return true;
   }

}

bool db_long_set::find(const char * lpKey)
{
   UNREFERENCED_PARAMETER(lpKey);
   /*    single_lock sl(&m_CriticalSection, TRUE);
   HRESULT hr;
   if(m_bIndexed)
   {
   //         TRACE("CSongsSet::FindKey Seeking\n");
   string strSection(lpSection);
   string strKey(lpKey);
   if(FAILED(hr = MoveFirst()))
   {
   AddOLEDBException(m_spCommand, IID_IRowsetChange);
   }
   if(hr == DB_S_ENDOFROWSET)
   {
   return FALSE;
   }
   if(FAILED(get_data(0)))
   {
   AddOLEDBException(m_spCommand, IID_IRowsetChange);
   }
   if(strSection.compare_ci(m_Section) == 0 &&
   strKey.compare_ci(m_Key) == 0)
   {
   return TRUE;
   }
   CDBLongRow l;
   _tcscpy(l.m_Section , lpSection);
   _tcscpy(l.m_Key , lpKey);
   ReleaseRows();
   hr = m_spRowsetIndex->seek(
   m_pAccessor->GetHAccessor(0),
   2,
   &l,
   DBSEEK_FIRSTEQ);
   if(FAILED(hr))
   return FALSE;
   HROW* phRow = &m_hRow;
   ULONG ulRowsFetched = 0;
   hr = m_spRowset->GetNextRows(NULL, 0, 1, &ulRowsFetched, &phRow);
   if (hr != S_OK)
   return FALSE;
   hr = get_data();
   if(FAILED(hr))
   return FALSE;
   return TRUE;
   }
   else
   {
   string strSection(lpSection);
   string strKey(lpKey);
   if(FAILED(hr = MoveFirst()))
   {
   AddOLEDBException(m_spCommand, IID_IRowsetChange);
   }
   if(hr == DB_S_ENDOFROWSET)
   {
   return FALSE;
   }
   while(TRUE)
   {
   if(FAILED(get_data(0)))
   {
   AddOLEDBException(m_spCommand, IID_IRowsetChange);
   }
   if(strSection.compare_ci(m_Section) == 0 &&
   strKey.compare_ci(m_Key) == 0)
   {
   return TRUE;
   }
   if(FAILED(hr = MoveNext()))
   {
   AddOLEDBException(m_spCommand, IID_IRowsetChange);
   }
   if(hr == DB_S_ENDOFROWSET)
   {
   return FALSE;
   }
   }
   }*/
   return false;
}


bool db_long_set::load(const char * lpKey, LPRECT lpRect)
{

   rect rect;

   string strKey = lpKey;

   if (!load(strKey + ".left", rect.left))
      return false;

   if (!load(strKey + ".top", rect.top))
      return false;

   if (!load(strKey + ".right", rect.right))
      return false;

   if (!load(strKey + ".bottom", rect.bottom))
      return false;

   *lpRect = rect;

   return true;

}

//////////////////////////////
//
// Output:
// 'false' if one or more save operations has failed.
// 'true' otherwise

bool db_long_set::save(const char * lpKey, LPCRECT lpRect)
{

   string strKey = lpKey;

   if (!save(strKey + ".left", lpRect->left))
      return false;

   if (!save(strKey + ".top", lpRect->top))
      return false;

   if (!save(strKey + ".right", lpRect->right))
      return false;

   if (!save(strKey + ".bottom", lpRect->bottom))
      return false;

   return true;

}


bool db_long_set::MoveWindow_(const char * lpKey, ::user::primitive * pwindow)
{

   rect rect;

   if (!load(lpKey, &rect))
      return false;

   pwindow->SetPlacement(rect);

   return true;

}


bool db_long_set::SaveWindowRect_(const char * lpKey, ::user::primitive * pwindow)
{

#ifdef WINDOWSEX

   WINDOWPLACEMENT wp;
   pwindow->GetWindowPlacement(&wp);
   return save(lpKey, &wp.rcNormalPosition);

#else

   throw todo(get_thread_app());

#endif

}

/*HRESULT db_long_set::OpenQuery(CDataSource *pdb, const char * lpcszSql)
{
CSession   session;
//      HRESULT      hr;
/*       if(m_spRowset != NULL)
close();
if(m_spRowsetIndex != NULL)
m_spRowsetIndex.Release();
m_bIndexed = false;

hr = session.open(*pdb);
if (FAILED(hr))
return hr;

CDBPropSet   propset(DBPROPSET_ROWSET);
propset.add_property(DBPROP_BOOKMARKS, true);
propset.add_property(DBPROP_CANFETCHBACKWARDS, true);
propset.add_property(DBPROP_IRowsetScroll, true);
propset.add_property(DBPROP_IRowsetChange, true);
propset.add_property(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE );
m_CriticalSection.lock();
hr = CCommand<CAccessor<CDBLongRow> >::open(session, lpcszSql, &propset);
m_CriticalSection.unlock();
if (FAILED(hr))
{
AddOLEDBException(session.m_spOpenRowset, IID_ICommandPrepare);
//         AddOLEDBException(session.m_spOpenRowset, IID_IRowsetChange);
}

return MoveNext(); */
/*    return S_OK;

}*/

/*HRESULT db_long_set::OpenIndexed(CDataSource *pdb)
{
//    HRESULT hr;
CSession session;
/*       if(m_spRowset != NULL)
close();
if(m_spRowsetIndex != NULL)
m_spRowsetIndex.Release();
try
{

hr = session.open(*pdb);
if (FAILED(hr))
{
AddOLEDBException(pdb->m_spInit, IID_IOpenRowset);
}
} catch(COLEDBException *e)
{
CDBErrorDialog dlg;
dlg.Init(e->m_spUnk, e->m_iid);
dlg.DoModal();
e->Delete();
return hr;
}
DBID    dbIdIndex;
DBID   dbIdTable;

dbIdTable.eKind = DBKIND_NAME;
dbIdTable.uName.pwszName = OLESTR("Long");

dbIdIndex.eKind = DBKIND_NAME;
dbIdIndex.uName.pwszName = OLESTR("PrimaryKey");

CDBPropSet   propset(DBPROPSET_ROWSET);
propset.add_property(DBPROP_BOOKMARKS, true);
propset.add_property(DBPROP_CANFETCHBACKWARDS, true);
propset.add_property(DBPROP_IRowsetScroll, true);
propset.add_property(DBPROP_IRowsetChange, true);
propset.add_property(DBPROP_IRowsetIndex, true);
propset.add_property(DBPROP_IRowset, true);
//        propset.add_property(DBPROP_OTHERUPDATEDELETE, true);
//        propset.add_property(DBPROP_OTHERINSERT, true);
propset.add_property(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE );


// open the Index with default properties and default interfaces. (IRowsetIndex,
// IAccessor, IRowset, IColumnsInfo, IRowsetInfo).
//CComPtr<IRowsetIndex> spIndex ;
try
{

hr = session.m_spOpenRowset->OpenRowset(
NULL,
&dbIdTable,
&dbIdIndex,
IID_IRowset,
1,
&propset,
(IUnknown**) &m_spRowset);
if (FAILED(hr))
{
AddOLEDBException(session.m_spOpenRowset, IID_IOpenRowset);
return hr;
}
hr = Bind();
if(FAILED(hr))
return hr;
CComPtr<IRowsetIndex> spRowsetIndex;
hr = m_spRowset->QueryInterface(&spRowsetIndex);
if(FAILED(hr))
return hr;
// Sets the range to the entire index.
hr = spRowsetIndex->SetRange(
m_pAccessor->GetHAccessor(0),
0,
NULL,
0,
NULL,
0);
if(FAILED(hr))
return hr;
SetupOptionalRowsetInterfaces();
hr = m_spRowset->QueryInterface(&m_spRowsetIndex);
if(FAILED(hr))
return hr;

m_bIndexed = true;

} catch(COLEDBException *e)
{
CDBErrorDialog dlg;
dlg.Init(e->m_spUnk, e->m_iid);
dlg.DoModal();
e->Delete();
return hr;
}*/

//return MoveNext();
//  return S_OK;

//}

bool db_long_set::SetWindowPlacement(const char * lpKey, ::user::primitive * pwindow)
{

#ifdef WINDOWSEX

   WINDOWPLACEMENT wp;
   if (!load(lpKey, &wp.rcNormalPosition))
      return false;

   string strKey;
   strKey = lpKey;
   strKey += ".ptMinPosition";
   if (!load(strKey, &wp.ptMinPosition))
      return false;
   strKey = lpKey;
   strKey += ".ptMaxPosition";
   if (!load(strKey, &wp.ptMaxPosition))
      return false;
   strKey = lpKey;
   strKey += ".showCmd";
   if (!load(strKey, (int32_t &)wp.showCmd))
      return false;
   strKey = lpKey;
   strKey += ".flags";
   if (!load(strKey, (int32_t &)wp.flags))
      return false;
   pwindow->SetWindowPlacement(&wp);
   return true;

#else

   throw todo(get_thread_app());

#endif

}

bool db_long_set::SaveWindowPlacement(const char * lpKey, ::user::primitive * pwindow)
{

#ifdef WINDOWSEX

   WINDOWPLACEMENT wp;
   pwindow->GetWindowPlacement(&wp);
   if (!save(lpKey, &wp.rcNormalPosition))
      return false;
   string strKey;
   strKey = lpKey;
   strKey += ".ptMinPosition";
   if (!save(strKey, &wp.ptMinPosition))
      return false;
   strKey = lpKey;
   strKey += ".ptMaxPosition";
   if (!save(strKey, &wp.ptMaxPosition))
      return false;
   strKey = lpKey;
   strKey += ".showCmd";
   if (!save(strKey, (int32_t)wp.showCmd))
      return false;
   strKey = lpKey;
   strKey += ".flags";
   if (!save(strKey, (int32_t)wp.flags))
      return false;

   return true;

#else

   throw todo(get_thread_app());

#endif

}


#ifdef WINDOWSEX


bool db_long_set::save(const char * lpKey, WINDOWPLACEMENT & wp)
{

   if (!save(lpKey, &wp.rcNormalPosition))
      return false;
   string strKey;
   strKey = lpKey;
   strKey += ".ptMinPosition";
   if (!save(strKey, &wp.ptMinPosition))
      return false;
   strKey = lpKey;
   strKey += ".ptMaxPosition";
   if (!save(strKey, &wp.ptMaxPosition))
      return false;
   strKey = lpKey;
   strKey += ".showCmd";
   if (!save(strKey, (int32_t)wp.showCmd))
      return false;
   strKey = lpKey;
   strKey += ".flags";
   if (!save(strKey, (int32_t)wp.flags))
      return false;

   return true;

}

bool db_long_set::load(const char * lpKey, WINDOWPLACEMENT & wp)
{

   load(lpKey, &wp.rcNormalPosition);

   string strKey;
   strKey = lpKey;
   strKey += ".ptMinPosition";
   load(strKey, &wp.ptMinPosition);
   strKey = lpKey;
   strKey += ".ptMaxPosition";
   load(strKey, &wp.ptMaxPosition);
   strKey = lpKey;
   strKey += ".showCmd";
   load(strKey, (int32_t &)wp.showCmd);
   strKey = lpKey;
   strKey += ".flags";
   load(strKey, (int32_t &)wp.flags);

   return true;
}


#endif



bool db_long_set::load(const char * lpKey, LPPOINT lpPoint)
{

   point point;

   string strKey = lpKey;

   if (!load(strKey + ".x", point.x))
      return false;

   if (!load(strKey + ".y", point.y))
      return false;

   *lpPoint = point;

   return true;

}

bool db_long_set::save(const char * lpKey, LPPOINT lpPoint)
{

   string strKey = lpKey;

   if (!save(strKey + ".x", lpPoint->x))
      return false;

   if (!save(strKey + ".y", lpPoint->y))
      return false;

   return true;

}


