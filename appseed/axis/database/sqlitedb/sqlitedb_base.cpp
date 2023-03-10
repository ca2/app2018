#include "framework.h"
#include "fiasco_finder.h"
#include <stdio.h>

//#include "database/sqlite/sqlite_base.h"
//#include "database/sqlite/sqlite_set.h"
#include "sqlite3.h"


//#ifdef LINUX
//#include <unistd.h>
//#endif

extern "C" int32_t axis_sqlite_callback(void * res_ptr,int32_t ncol, char** reslt,char** cols);

namespace sqlite
{


   base::base(::aura::application * papp) :
      ::object(papp)
   {

      defer_create_mutex();

      active = false;
      _in_transaction = false;      // for transaction

      error = "Unknown database error";//S_NO_CONNECTION;
      host = "localhost";
      port = "";
      db = "sqlite.db";
      login = "root";
      passwd = "";
      conn = NULL;
      
   }

   
   base::~base()
   {

      disconnect();

   }


   database::set * base::CreateDataset()
   {
      
      return canew(class set((base*)this));
      
   }

   
   int32_t base::status()
   {
      
      if (active == false)
      {
         
         return DB_CONNECTION_NONE;
         
      }
      
      return DB_CONNECTION_OK;
      
   }
   

   int32_t base::setErr(int32_t err_code)
   {
      
      switch (err_code)
      {
      case SQLITE_OK: error ="Successful result";
         break;
      case SQLITE_ERROR: error = "SQL error or missing database";
         break;
      case SQLITE_INTERNAL: error = "An internal logic error in SQLite";
         break;
      case SQLITE_PERM: error ="Access permission denied";
         break;
      case SQLITE_ABORT: error = "Callback routine requested an abort";
         break;
      case SQLITE_BUSY: error = "The database file is locked";
         break;
      case SQLITE_LOCKED: error = "A table in the database is locked";
         break;
      case SQLITE_NOMEM: error = "A malloc() failed";
         break;
      case SQLITE_READONLY: error = "Attempt to write a readonly database";
         break;
      case SQLITE_INTERRUPT: error = "Operation terminated by sqlite_interrupt()";
         break;
      case  SQLITE_IOERR: error = "Some kind of disk I/O error occurred";
         break;
      case  SQLITE_CORRUPT: error = "The database disk image is malformed";
         break;
      case SQLITE_NOTFOUND: error = "(Internal Only) Table or record not found";
         break;
      case SQLITE_FULL: error = "Insertion failed because database is full";
         break;
      case SQLITE_CANTOPEN: error = "Unable to open the database file";
         break;
      case SQLITE_PROTOCOL: error = "base lock protocol error";
         break;
      case SQLITE_EMPTY:  error = "(Internal Only) base table is is_empty";
         break;
      case SQLITE_SCHEMA: error = "The database schema changed";
         break;
      case SQLITE_TOOBIG: error = "Too much data for one row of a table";
         break;
      case SQLITE_CONSTRAINT: error = "Abort due to contraint violation";
         break;
      case SQLITE_MISMATCH:  error = "Data type mismatch";
         break;
      default : error = "Undefined SQLite error";
      }
      return err_code;
   }

   
   const char *base::getErrorMsg()
   {
      
      return error;
      
   }
   

   int32_t base::connect()
   {
      
      synch_lock sl(m_pmutex);
      
      disconnect();
      
      if(sqlite3_open(db, (sqlite3 * *) &conn) == SQLITE_OK)
      {
         //cout << "Connected!\n";
         char* err = NULL;
         if(setErr(sqlite3_exec((sqlite3 *) getHandle(),"PRAGMA empty_result_callbacks=ON",NULL,NULL,&err)) != SQLITE_OK)
         {
            TRACE("Error: %s",err);
            _throw(database::DbErrors(::get_app(),getErrorMsg()));
         }
//         if (setErr(sqlite3_exec((sqlite3 *)getHandle(), "PRAGMA cache_size=-20000", NULL, NULL, &err)) != SQLITE_OK)
//         {
//            TRACE("Error: %s", err);
//            const char * pszErrorMessage = getErrorMsg();
//            if(pszErrorMessage != NULL)
//            {
//
//               output_debug_string(pszErrorMessage);
//            }
//            //_throw(database::DbErrors(::get_app(),getErrorMsg()));
//         }
////         if (setErr(sqlite3_exec((sqlite3 *)getHandle(), "PRAGMA synchronous=OFF", NULL, NULL, &err)) != SQLITE_OK)
////         {
////            fprintf(stderr, "Error: %s", err);
////            _throw(database::DbErrors(::get_app(),getErrorMsg()));
////         }
         //if (setErr(sqlite3_exec((sqlite3 *)getHandle(), "PRAGMA temp_store=MEMORY", NULL, NULL, &err)) != SQLITE_OK)
         //{
         //   TRACE("Error: %s", err);
         //   //_throw(database::DbErrors(::get_app(),getErrorMsg()));
         //}
         active = true;
         return DB_CONNECTION_OK;
      }
      
      return DB_CONNECTION_NONE;
      
   }
   

   void base::disconnect()
   {

      synch_lock sl(m_pmutex);
      
      if (conn != NULL)
      {

         sqlite3_close((sqlite3 *)conn);

         conn = NULL;

      }

      if (active == false)
      {
         
         return;
         
      }
      
      active = false;
      
   }
   

   int32_t base::create()
   {
      
      return connect();
      
   }
   

   int32_t base::drop()
   {
      
      synch_lock sl(m_pmutex);
      
      disconnect();
      
      if (active == false)
      {
         
         return DB_ERROR;
         
      }
      
      try
      {
         
         Application.file().del(db);

      }
      catch(...)
      {
         
         return DB_ERROR;

      }
      //if (!_unlink(db))
      // return DB_ERROR;
      return DB_COMMAND_OK;
   }


   long base::nextid(const char* sname)
   {
      
      synch_lock sl(m_pmutex);

      if(!active)
      {

         return DB_UNEXPECTED_RESULT;

      }

      int32_t id;

      database::result_set res;

      char sqlcmd[512];

      sprintf(sqlcmd,"select nextid from %s where seq_name = '%s'",sequence_table.c_str(), sname);

      if ((last_err = sqlite3_exec((sqlite3 *) getHandle(),sqlcmd,&axis_sqlite_callback,&res,NULL) != SQLITE_OK))
      {

         return DB_UNEXPECTED_RESULT;

      }

      if (res.m_records.get_size() == 0)
      {

         id = 1;

         sprintf(sqlcmd,"insert into %s (nextid,seq_name) values (%d,'%s')",sequence_table.c_str(),id,sname);

         if((last_err = sqlite3_exec((sqlite3 *)conn,sqlcmd,NULL,NULL,NULL)) != SQLITE_OK)
         {

            return DB_UNEXPECTED_RESULT;

         }

         return id;
      }
      else
      {

         id = res.m_records[0][0].i32() + 1;

         sprintf(sqlcmd,"update %s set nextid=%d where seq_name = '%s'",sequence_table.c_str(),id,sname);

         if ((last_err = sqlite3_exec((sqlite3 *) conn,sqlcmd,NULL,NULL,NULL)) != SQLITE_OK) return DB_UNEXPECTED_RESULT;

         return id;

      }

      return DB_UNEXPECTED_RESULT;

   }


   void base::start_transaction()
   {

      synch_lock sl(m_pmutex);
      
      if (active)
      {
         sqlite3_exec((sqlite3 *) conn,"begin",NULL,NULL,NULL);
         _in_transaction = true;
      }

   }

   void base::commit_transaction()
   {

      synch_lock sl(m_pmutex);
      
      if (active)
      {
         sqlite3_exec((sqlite3 *) conn,"commit",NULL,NULL,NULL);
         _in_transaction = false;
      }

   }

   void base::rollback_transaction()
   {

      synch_lock sl(m_pmutex);
      
      if (active)
      {
         sqlite3_exec((sqlite3 *) conn,"rollback",NULL,NULL,NULL);
         _in_transaction = false;
      }

   }

   string base::escape(const char * psz)
   {
      string str(psz);
      str.replace("\'", "\'\'");
      str.replace("\"", "\"\"");
      return str;
   }


   void base::create_long_set(const string & strTable)
   {

      synch_lock sl(m_pmutex);
      
      try
      {

         set set(this);

         synch_lock sl(m_pmutex);

         set.query("select * from sqlite_master where type like 'table' and name like '" + strTable + "'");

         if (set.num_rows() == 0)
         {

            set.exec("create table '" + strTable + "' (id text primary key, value integer)");
         }

      }
      catch (...)
      {

      }

   }


   void base::create_string_set(const string & strTable)
   {

      synch_lock sl(m_pmutex);
      
      try
      {

         set set(this);

         synch_lock sl(m_pmutex);

         set.query("select * from sqlite_master where type like 'table' and name like '" + strTable + "'");

         if(set.num_rows() == 0)
         {

            set.exec("create table '" + strTable + "' (id text primary key, value text)");

         }

      }
      catch(...)
      {

      }

   }


} // namespace sqlite












