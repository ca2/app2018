#include "framework.h"


namespace file
{




   set::set(::aura::application * papp) :
      ::object(papp)
   {

      defer_create_mutex();

   }


   set::~set()
   {

   }

   void set::add_search(const ::file::path & pathSearchFolder, bool bRecursive)
   {

      m_pathaSearch.add(pathSearchFolder);

      m_baRecursive.add(bRecursive);

   }


   void set::add_search(const ::file::patha & pathaSearchFolder, const bool_array & baRecursive)
   {

      // derived classes may perform specific actions this add_search...

      for (index i = 0; i < pathaSearchFolder.get_count(); i++)
      {

         // ... or at the add_search overload below.

         add_search(pathaSearchFolder[i], baRecursive[i]);

      }

   }


   void set::add_filter(stringa & stra)
   {

      m_straFilter.add(stra);

   }



   ::count set::get_file_count()
   {

      return m_straFile.get_size();

   }


   void set::file_at(::index i, string & str)
   {

      str = m_straFile[i];

   }


   ::index set::find_first_file(const char * lpcsz, ::index iStart)
   {

      return m_straFile.find_first_ci(lpcsz, iStart);

   }




   void set::refresh()
   {

      clear_file();

      string strFilter;

      int32_t i;

      string strFile;

      string str;

      for(i = 0; i < m_pathaSearch.get_size(); i++)
      {

         bool bRecursive = true;

         if(i < m_baRecursive.get_size())
         {

            bRecursive = m_baRecursive[i];

         }

         m_straFile.m_pprovider = get_app();

         ::file::path & pathFolder = m_pathaSearch.element_at(i);

         if(bRecursive)
         {

            m_straFile.rls_pattern(pathFolder, m_straFilter);

         }
         else
         {

            m_straFile.ls_pattern(pathFolder, m_straFilter);

         }

      }

      m_straFile.add(m_straFileAddUp);

   }


   void set::clear_file()
   {

      m_straFile.clear();

   }


   void set::clear_search()
   {

      m_pathaSearch.remove_all();

      m_baRecursive.remove_all();

   }


   void set::clear_filter()
   {

      m_straFilter.remove_all();

   }


   bool set::get_next_file(const char * pszFile, string & strNext)
   {

      index iFind = find_first_file(pszFile);

      if(iFind < 0)
      {

         iFind = -1;

      }

      index iNext = iFind + 1;

      if(iNext >= m_straFile.get_count())
      {

         iNext = 0;

      }

      if(iNext >= m_straFile.get_count())
      {

         return false;

      }

      strNext =  m_straFile[iNext];

      return true;

   }


   string set::get_next_file(const char * pszFile)
   {

      string strNext;

      if(!get_next_file(pszFile, strNext))
      {

         return "";

      }

      return strNext;

   }


   bool set::initialize_os_wallpaper(bool bAddSearch)
   {

      if (!System.os().initialize_wallpaper_fileset(this, bAddSearch))
      {

         return false;

      }

      refresh();
      
      if(m_straFile.is_empty())
      {
         
         if (!System.os().::aura::os::initialize_wallpaper_fileset(this, bAddSearch))
         {
            
            return false;
            
         }
         
         refresh();

      }

      return true;

   }


} // namespace file





