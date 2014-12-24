#include "framework.h"


namespace simpledb
{


   file_set::file_set(::aura::application * papp) :
      element(papp),
      ::file::set_sp(allocer())
   {

   }


   file_set::~file_set()
   {

   }


   void file_set::data_on_after_change(signal_details * pobj)
   {

      SCAST_PTR(::database::change_event, pchange, pobj);

      if(pchange->m_key.m_id == m_dataid)
      {

         refresh();

      }

   }


   bool file_set::refresh()
   {

      m_p->clear_search();

      stringa stra;

      bool_array baRecursive;

      if(!data_get(::base::system::idEmpty, stra))
         return false;

      data_load("recursive", baRecursive);

      // add_search calls Ex2FileSet refresh internally
      m_p->add_search(stra, baRecursive);

      return true;

   }


   bool file_set::add_search(const char * pszSearchDirectory)
   {

      stringa stra;

      data_get(::base::system::idEmpty, stra);

      if(stra.add_unique(pszSearchDirectory) < 0)
         return true;

      if(!data_set(::base::system::idEmpty, stra))
         return false;

      if(!refresh())
         return false;

      return true;

   }


   bool file_set::clear_search()
   {

      stringa stra;

      if(!data_set(::base::system::idEmpty, stra))
         return false;

      if(!refresh())
         return false;

      return true;

   }


} // namespace simpledb



