#pragma once

namespace sqlite
{

   class base;

   /* class 'set' does a query to SQLite-server */

   class CLASS_DECL_AXIS set :
      public ::database::set
   {
   public:


      ::database::result_set result;
      database::result_set exec_res;


      set(::aura::application * papp);
      set(base *newDb);
      ~set();



      virtual void query_items(stringa & stra, const char * pszSql);

      void * handle(); // sqlite *


      /* Makes direct queries to database */
      virtual void make_query(stringa &_sql);
      /* Makes direct inserts into database */
      virtual void make_insert() override;
      /* Edit SQL */
      virtual void make_edit() override;
      /* Delete SQL */
      virtual void make_deletion() override;

      //static int32_t sqlite_callback(void * res_ptr,int32_t ncol, char** reslt, char** cols);

      /* This function works only with MySQL database
      Filling the fields information from select statement */
      virtual void fill_fields() override;
      /* Changing field values during set navigation */

      bool remove_row();

      /* opens a query  & then sets a query results */
      virtual void open() override;
      virtual void open(const char * sql) override;
      /* func. executes a query without results to return */
      virtual bool exec() override;
      virtual bool exec(const char * sql) override;
      virtual const void * getExecRes() override;
      /* as open, but with our query exept Sql */
      virtual bool query(const char *query) override;
      /* func. closes a query */
      virtual void close() override;
      /* Cancel changes, made in insert or edit states of set */
      virtual void cancel() override;
      /* sequence numbers */
      virtual long nextid(const char *seq_name) override;
      /* sequence numbers */
      virtual ::count num_rows() override;

      virtual bool bof() override;
      virtual bool eof() override;
      virtual void first() override;
      virtual void last() override;
      virtual void prev() override;
      virtual void next() override;

      virtual bool find_first(const char * fieldname, var & value);
      /* Go to record No (starting with 0) */
      virtual bool seek(index pos=0) override;

      virtual bool SetFieldValue(const char *f_name, const var &value) override;
      virtual bool SetFieldValue(index iFieldIndex, const var &value);

      virtual var field_value_at(index iFieldIndex) override;
      virtual var FieldValueAt(index iFieldIndex);
      virtual int32_t GetFieldIndex(const char *f_name);

   };

} // namespace vmssqlite
