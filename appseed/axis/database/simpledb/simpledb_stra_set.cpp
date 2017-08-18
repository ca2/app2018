#include "framework.h"


db_stra_set::db_stra_set(db_server * pserver, ::database::id & id) :
   m_id(id)
{

   UNREFERENCED_PARAMETER(pserver);

   m_iMaximumCount = 32;

}


void db_stra_set::get_data(stringa & stra)
{

   data_load(m_id, stra);

}


// return the number of set directories
::count db_stra_set::set_data(stringa & stra)
{

   data_save(m_id, stra);

   return stra.get_size();

}






