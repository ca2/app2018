#include "framework.h"
#include "mysql/mysql.h"


namespace mysql
{


   result::result(database * pdatabase, bool bAutoDelete, void * pres) :
      m_pdatabase(pdatabase)
   {
      m_bAutoDelete = bAutoDelete;
      m_pdatabase->m_resultptra.add(this);
      m_pres = pres;
      m_iFieldCount = -1;
   }

   void * result::fetch_row()
   {
      MYSQL_ROW row;
      if(m_pres == NULL)
         return NULL;
      row = mysql_fetch_row((MYSQL_RES *) m_pres);
      if(mysql_errno ((MYSQL *) m_pdatabase->m_pmysql) != 0)
         m_pdatabase->trace_error1( "mysql_fetch_row() failed");
      return row;
   }

   unsigned long * result::fetch_lengths()
   {
      unsigned long * lengths;
      if(m_pres == NULL)
         return NULL;
      lengths = mysql_fetch_lengths((MYSQL_RES *) m_pres);
      if(mysql_errno ((MYSQL *) m_pdatabase->m_pmysql) != 0)
         m_pdatabase->trace_error1( "mysql_fetch_row() failed");
      return lengths;
   }

   int result::num_fields()
   {
      return mysql_num_fields((MYSQL_RES *) m_pres);
   }

   __int64 result::num_rows()
   {
      return mysql_num_rows ((MYSQL_RES *) m_pres);
   }

   result::~result()
   {
      free_result();
   }

   /* process rows and then free the result set */
   //process_result_set (conn, res_set);


   bool result::free_result()
   {
      if(m_pres != NULL)
      {
         mysql_free_result((MYSQL_RES *) m_pres);
      }
      return true;
   }


} // namespace mysql


