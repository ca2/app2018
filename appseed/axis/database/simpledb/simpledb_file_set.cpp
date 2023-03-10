#include "framework.h"


namespace simpledb
{


   file_set::file_set(::aura::application * papp) :
      ::object(papp),
      ::file::set(papp)
   {

      m_straFile.m_bDir      = false;
      m_straFile.m_bFile     = true;

   }


   file_set::~file_set()
   {

   }


   void file_set::data_on_after_change(::message::message * pobj)
   {

      SCAST_PTR(::database::change_event, pchange, pobj);

      if(pchange->m_datakey == m_datakey)
      {

         refresh();

      }

   }


   void file_set::refresh()
   {

      ::file::set::clear_search();

      ::file::patha patha;

      bool_array baRecursive;

      if(!data_load("", patha))
         return;

      data_load("recursive", baRecursive);

      for (index i = baRecursive.get_size(); i < patha.get_size(); i++)
      {
         baRecursive.add(false);
      }

      // add_search calls Ex2FileSet refresh internally
      ::file::set::add_search(patha, baRecursive);

      ::file::set::refresh();

   }


   void file_set::add_search(const ::file::path & pathSearchFolder, bool bRecursive)
   {

      ::file::patha & pathaFolder = m_pathaSearch;

      bool_array baRecursive;

      data_load(::aura::system::idEmpty, pathaFolder);

      data_load("recursive",baRecursive);

      index i = -1;

      if((i = pathaFolder.find_first_ci(pathSearchFolder)) < 0)
      {

         pathaFolder.add(pathSearchFolder);

         baRecursive.add(bRecursive);

         data_save(::aura::system::idEmpty, pathaFolder);

         data_save("recursive", baRecursive);

      }
      else
      {

         baRecursive.set_at_grow(i,bRecursive);

         data_save("recursive", baRecursive);

      }

   }


   void file_set::clear_search()
   {

      ::file::set::clear_search();

      stringa stra;

      data_save(::aura::system::idEmpty, stra);

   }


} // namespace simpledb



