#pragma once

#include "database_definition.h"

namespace database
{

   class set;
   /******************* Class database definition ********************

   represents  connection with database server;

   ******************************************************************/
   class CLASS_DECL_ca base :
      virtual public ::radix::object
   {
   public:
      bool     active;
      string   error; // Error description
      string   host, port, db, login, passwd; //Login info
      string   sequence_table; //Sequence table for nextid

      /* constructor */
      base();
      /* destructor */
      virtual ~base();
      virtual class set *CreateDataset() const = 0;
      /* sets a new host name */
      void setHostName(const char *newHost) { host = newHost; }
      /* gets a host name */
      string getHostName(void) const { return host; }
      /* sets a new port */
      void setPort(const char *newPort) { port = newPort; }
      /* gets a port */
      string getPort(void) const { return port; }
      /* sets a new database name */
      void setDatabase(const char *newDb) { db = newDb; }
      /* gets a database name */
      string getDatabase(void) const { return db; }
      /* sets a new login to database */
      void setLogin(const char *newLogin) { login = newLogin; }
      /* gets a login */
      string getLogin(void) const { return login; }
      /* sets a password */
      void setPasswd(const char *newPasswd) { passwd = newPasswd; }
      /* gets a password */
      string getPasswd(void) const { return passwd; }
      /* active status is OK state */
      virtual bool isActive(void) const { return active; }
      /* Set new name of sequence table */
      void setSequenceTable(const char *new_seq_table) { sequence_table = new_seq_table; };
      /* get name of sequence table */
      const char *getSequenceTable(void) { return sequence_table; }


      /* virtual methods that must be overloaded in derived classes */

      virtual int init(void) { return DB_COMMAND_OK; }
      virtual int status(void) { return DB_CONNECTION_NONE; }
      virtual int setErr(int err_code)=0;
      virtual const char *getErrorMsg(void) { return error; }

      virtual int connect(void) { return DB_COMMAND_OK; }
      virtual int connectFull( const char *newDb, const char *newHost=NULL,
         const char *newLogin=NULL, const char *newPasswd=NULL,const char *newPort=NULL);
      virtual void disconnect(void) { active = false; }
      virtual int reset(void) { return DB_COMMAND_OK; }
      using ::ex1::request_interface::create;
      virtual int create(void) { return DB_COMMAND_OK; }
      virtual int drop(void) { return DB_COMMAND_OK; }
      virtual long nextid(const char* seq_name)=0;

      /* virtual methods for transaction */

      virtual void start_transaction() {};
      virtual void commit_transaction() {};
      virtual void rollback_transaction() {};

      virtual bool in_transaction() {return false;};

   };

} // namespace database

