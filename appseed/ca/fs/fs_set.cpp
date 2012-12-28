#include "framework.h"


namespace fs
{


   set::set(::ca::application * papp) :
      ca(papp),
      ::ca::data(papp),
      ::fs::data(papp)
   {

   }

   set::~set()
   {
   }


   ::collection::string_map < ::fs::data * > & set::fsmap()
   {
      return m_fsdatamap;
   }

   spa(::fs::data) & set::fsa()
   {
      return m_spafsdata;
   }


   void set::root_ones(stringa & stra)
   {
      
      m_fsdatamap.remove_all();

      stringa straFs;

      for(int32_t i = 0; i < m_spafsdata.get_count(); i++)
      {
         straFs.remove_all();
         m_spafsdata[i]->root_ones(straFs);
         stra.add(straFs);
         for(int32_t j = 0; j < straFs.get_size(); j++)
         {
            m_fsdatamap[straFs[j]] = m_spafsdata[i];
         }
      }

   }

   data * set::path_data(const char * psz)
   {
      POSITION pos = m_fsdatamap.get_start_position();

      string strRoot;
      ::fs::data * pdata;

      while(pos != NULL)
      {
         m_fsdatamap.get_next_assoc(pos, strRoot, pdata);
         if(gen::str::begins_ci(psz, strRoot))
         {
            return pdata;
         }
      }
      
      return NULL;

   }

   bool set::ls(const char * psz, stringa * pstraPath, stringa * pstraTitle)
   {

      if(psz == NULL || strlen(psz) == 0)
      {
         stringa straPath;
         root_ones(straPath);
         for(int32_t i = 0; i < straPath.get_size(); i++)
         {
            if(pstraPath != NULL)
            {
               pstraPath->add(straPath[i]);
            }
            if(pstraTitle != NULL)
            {
               pstraTitle->add(file_name(straPath[i]));
            }
         }
         return true;
      }
      
      ::fs::data * pdata = path_data(psz);

      if(pdata != NULL)
      {
         return pdata->ls(psz, pstraPath, pstraTitle);
      }
      
      return false;

   }

   bool set::is_dir(const char * psz)
   {
      
      ::fs::data * pdata = path_data(psz);

      if(pdata != NULL)
      {
         return pdata->is_dir(psz);
      }

      return false;

   }


   string set::file_name(const char * psz)
   {

      ::fs::data * pdata = path_data(psz);

      if(pdata != NULL)
      {
         return pdata->file_name(psz);
      }

      return "";

   }


   bool set::file_move(const char * pszDst, const char * pszSrc)
   {
      
      ::fs::data * pdataDst = path_data(pszDst);
      ::fs::data * pdataSrc = path_data(pszSrc);

      if(pdataDst != NULL && pdataSrc == pdataDst)
      {
         return pdataDst->file_move(pszDst, pszSrc);
      }
      else
      {
         try
         {
            Application.file().copy(pszDst, pszSrc);
         }
         catch(...)
         {
            return false;
         }
         return true;
      }

   }


   bool set::has_subdir(const char * psz)
   {
      
      ::fs::data * pdata = path_data(psz);

      if(pdata != NULL)
      {
         return pdata->has_subdir(psz);
      }

      return false;

   }


   void set::get_ascendants_path(const char * psz, stringa & stra)
   {

      ::fs::data * pdata = path_data(psz);

      if(pdata != NULL)
      {
         pdata->get_ascendants_path(psz, stra);
      }

   }


   string set::eat_end_level(const char * psz, int32_t iLevel)
   {
      
      ::fs::data * pdata = path_data(psz);

      if(pdata != NULL)
      {
         return pdata->eat_end_level(psz, iLevel);
      }

      return "";

   }


   string set::dir_path(const char * pszPath1, const char * pszPath2)
   {

      ::fs::data * pdata = path_data(pszPath1);

      if(pdata != NULL)
      {
         return pdata->dir_path(pszPath1, pszPath2);
      }

      return System.dir().path(pszPath1, pszPath2);

   }


} // namespace fs

