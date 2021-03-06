#pragma once


class stringa;
class db_server;
//class db_long_set;
class db_server;
class db_str_set;
class db_stra_set;
class DBFileSystemSizeSet;

class MidiInstrumentSet;

class stringa;


namespace sqlite
{
   class base;
}



class CLASS_DECL_CORE db_server :
   virtual public ::database::server,
   virtual public ::user::interaction
{
public:


   sp(::sqlite::base)         m_pdb;

   ::simpledb::base *         m_pbase;

   db_long_set *              m_plongset;
   db_str_set *               m_pstrset;
   bool                       m_bWorking;
   string                     m_strDatabase;
   
   DBFileSystemSizeSet *      m_pfilesystemsizeset;

   mysql::database *          m_pmysqldbUser;
   string                     m_strUser;


   db_server(::aura::application * papp);
   virtual ~db_server();


   sp(::sqlite::base) get_database();

   using database::server::data_server_load;
   virtual bool data_server_load(::database::client * pclient, ::database::key id, ::file::output_stream & writable, ::database::update_hint * phint = NULL);

   using database::server::data_server_save;
   virtual bool data_server_save(::database::client * pclient, ::database::key id, ::file::input_stream & readable, ::database::update_hint * phint = NULL);


   virtual bool data_pulse_change(::database::client * pclient, ::database::key id, ::database::update_hint * puh);

   void close();

   bool load(const char * lpKey, string & str);
   bool load(const char * lpKey, ::file::output_stream & writer);

   bool save(const char * lpKey, const char * psz);
   bool save(const char * lpKey, ::file::input_stream & reader);


   virtual bool initialize_user(mysql::database * pmysqldbUser, const char * pszUser);
   virtual bool initialize();
   virtual bool finalize();

   bool create_message_queue();
   bool destroy_message_queue();


   inline db_str_set * get_db_str_set();
   void _001OnTimer(::timer * ptimer);

   void assert_valid() const;
   void dump(dump_context &) const;

};

