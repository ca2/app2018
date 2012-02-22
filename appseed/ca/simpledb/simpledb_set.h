#pragma once


namespace simpledb
{


   class base;


   class CLASS_DECL_ca set : 
      public ::database::set
   {
   public:


      ::database::result_set  m_resultset;
      database::result_set    m_resultsetExec;


      set();
      set(base *newDb);
      ~set();


      virtual void query_items(stringa & stra, const char * pszSql);

      virtual bool exec(const char * pszSql);


      virtual bool sql_do_select(const char * & pszSql);
      virtual bool sql_do_insert(const char * & pszSql);

      virtual string sql_consume_keyword(const char * & pszSql);
      virtual string sql_consume_field(const char * & pszSql);
      virtual string sql_consume_table(const char * & pszSql);
      virtual string sql_consume_join_on(const char * & pszSql);
      virtual string sql_consume_value(const char * & pszSql);

      static string consume_quoted_value(const char * & pszXml);

      /* Makes direct queries to database */
      virtual void make_query(stringa &_sql);
      /* Makes direct inserts into database */
      virtual void make_insert();
      /* Edit SQL */
      virtual void make_edit();
      /* Delete SQL */
      virtual void make_deletion();

      /* This function works only with MySQL database
      Filling the fields information from select statement */
      virtual void fill_fields();
      /* Changing field values during set navigation */
      bool remove_row();

      /* opens a query  & then sets a query results */
      virtual void open();
      virtual void open(const char * sql);
      /* func. executes a query without results to return */
      virtual bool exec();
      //     virtual bool exec(const char * sql);
      virtual const void * getExecRes();
      /* as open, but with our query exept Sql */
      virtual bool query(const char *query);
      /* func. closes a query */
      virtual void close(void);
      /* Cancel changes, made in insert or edit states of set */
      virtual void cancel();
      /* sequence numbers */
      virtual long nextid(const char *seq_name);
      /* sequence numbers */
      virtual count num_rows();

      virtual bool bof();
      virtual bool eof();
      virtual void first();
      virtual void last();
      virtual void prev();
      virtual void next();

      virtual bool find_first(char * fieldname, var & value);
      /* Go to record No (starting with 0) */
      virtual bool seek(index pos=0);

      virtual bool SetFieldValue(const char *f_name, const var &value);
      virtual bool SetFieldValue(index iFieldIndex, const var &value);

      virtual var & FieldValueAt(index iFieldIndex);
      virtual int GetFieldIndex(const char *f_name);

      base * get_database();

   };


} // namespace simpledb


