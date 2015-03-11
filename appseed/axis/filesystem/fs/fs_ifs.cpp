//#include "framework.h" // from "axis/net/net_sockets.h"
//#include "axis/net/net_sockets.h"


ifs::ifs(::aura::application * papp, const char * pszRoot) :
   ::object(papp),
   ::data::data(papp),
   ::fs::data(papp)
{
   m_strRoot = pszRoot;
   m_bInitialized = false;
}

bool ifs::fast_has_subdir(const ::file::path & pszPath)
{

   synch_lock sl(data_mutex());

   uint32_t dwTimeout;

   string strDir(pszPath);

   ::str::ends_eat(strDir, "/");
   ::str::ends_eat(strDir, "\\");

   dir & dir = m_map[strDir];

   if(get_tick_count() < dir.m_uiLsTimeout)
   {

      return dir.m_pathaFolder.get_count() > 0;

   }

   return true;

}

bool ifs::has_subdir(const ::file::path & path)
{

   synch_lock sl(data_mutex());

   uint32_t dwTimeout;

   dir & dir = m_map[path];

   if(get_tick_count() < dir.m_uiLsTimeout)
   {

      return dir.m_pathaFolder.get_count() > 0;

   }

   sl.unlock();

   ls(path, NULL, NULL, NULL, NULL);

   sl.lock();

   if(get_tick_count() < dir.m_uiLsTimeout)
   {

      return dir.m_pathaFolder.get_count() > 0;

   }

   return false;

}


void ifs::root_ones(::file::patha & patha,stringa & straTitle)
{

   patha.add("uifs://");

   straTitle.add("User Intelligent File System");

}


bool ifs::ls(const ::file::path & pszDir,::file::patha * ppatha,::file::patha * ppathaName,bool bSize,bool_array * pbaDir)
{

   synch_lock sl(data_mutex());

   uint32_t dwTimeout;

   dir & dir = m_map[pszDir];

   if(get_tick_count() < dir.m_uiLsTimeout)
   {

      if(ppatha  != NULL)
      {
         ppatha->add(dir.m_pathaFolder);
         ppatha->add(dir.m_pathaFile);
      }
      if(ppathaName  != NULL)
      {
         ppathaName->add(dir.m_pathaFolderName);
         ppathaName->add(dir.m_pathaFileName);
      }

      return true;

   }

   //if(m_mapdirFolder[strDir].is_null())
   //   m_mapdirFolder[strDir] = canew(stringa);
   //if(m_mapdirFolderName[strDir].is_null())
   //   m_mapdirFolderName[strDir] = canew(stringa);
   //if(m_mapdirFile[strDir].is_null())
   //   m_mapdirFile[strDir] = canew(stringa);
   //if(m_mapdirFileName[strDir].is_null())
   //   m_mapdirFileName[strDir] = canew(stringa);
   if (m_mapdirFileSize[strDir].is_null())
      m_mapdirFileSize[strDir] = canew(int64_array);
   if (m_mapdirFolderSize[strDir].is_null())
      m_mapdirFolderSize[strDir] = canew(int64_array);
   if(m_mapdirFileDir[strDir].is_null())
      m_mapdirFileDir[strDir] = canew(bool_array);
   if(m_mapdirFolderDir[strDir].is_null())
      m_mapdirFolderDir[strDir] = canew(bool_array);


   sl.unlock();

   //if(!is_dir(strDir))
     // return false;


   ::file::patha  straDir;
   ::file::patha  straDirName;
   ::file::patha  straFile;
   ::file::patha  straFileName;
   int64_array    iaFileSize;
   int64_array    iaFolderSize;
   bool_array     baFileDir;
   bool_array     baFolderDir;



   try
   {

      defer_initialize();

   }
   catch(string & str)
   {

      if(str == "You have not logged in! Exiting!")
      {

         throw string("uifs:// You have not logged in!");

      }

      m_maplsTimeout.set_at(strDir, get_tick_count() + ((5000) * 4));

      return false;

   }

   xml::document doc(get_app());

   string strUrl;

   strUrl = "http://file.veriwell.net/ifs/ls?path=" + System.url().url_encode(pszDir);

   string strSource;

   property_set set(get_app());

   strSource = Application.http().get(strUrl, set);

   sl.lock();

   if(strSource.is_empty())
   {

      m_maplsTimeout.set_at(strDir, get_tick_count() + ((5000) * 4));

      return false;

   }

   if(!doc.load(strSource))
   {

      m_maplsTimeout.set_at(strDir, get_tick_count() + ((5000) * 4));

      return false;

   }

   if(doc.get_root()->get_name() != "folder")
   {

      m_maplsTimeout.set_at(strDir, get_tick_count() + ((5000) * 4));

      return false;

   }

   sp(::xml::node) pnode = doc.get_root()->get_child("folder");

   if(pnode != NULL)
   {

      for(int32_t i = 0; i < pnode->get_children_count(); i++)
      {

         ::file::path strName = pnode->child_at(i)->attr("name").get_string();

         if(pnode->child_at(i)->get_name() != "folder")
            continue;

         ::file::path strPath = dir_path(pszDir,strName);
         m_mapdirTimeout[strPath] = ::get_tick_count() + (4 * 1000);
         m_mapfileTimeout.remove_key(strPath);
         straDir.add(strPath);
         straDirName.add(strName);
         iaFolderSize.add(0);
         baFolderDir.add(true);
      }
   }

   pnode = doc.get_root()->get_child("file");

   if(pnode != NULL)
   {
      for(int32_t i = 0; i < pnode->get_children_count(); i++)
      {
         ::file::path strName = pnode->child_at(i)->attr("name").get_string();
         string strExtension = pnode->child_at(i)->attr("extension");
         if(pnode->child_at(i)->get_name() != "file")
            continue;
         string strSize = pnode->child_at(i)->attr("size");
         ::file::path strPath = dir_path(pszDir,strName);
         m_mapfileTimeout[strPath] = ::get_tick_count() + (4 * 1000);
         m_mapdirTimeout.remove_key(strPath);
         straFile.add(strPath);
         straFileName.add(strName);
         iaFileSize.add(::str::to_int64(strSize));
         baFileDir.add(false);
      }
   }
   if(ppatha != NULL)
   {
      ppatha->add(straDir);
      ppatha->add(straFile);
   }
   if(ppathaName != NULL)
   {
      ppathaName->add(straDirName);
      ppathaName->add(straFileName);
   }
   if (piaSize != NULL)
   {
      piaSize->add(iaFolderSize);
      piaSize->add(iaFileSize);
   }
   if(pbaDir != NULL)
   {
      pbaDir->add(baFolderDir);
      pbaDir->add(baFileDir);
   }

   //   if(m_mapdirFolder[strDir].is_null())
   //   m_mapdirFolder[strDir] = canew(stringa);
   //if(m_mapdirFolderName[strDir].is_null())
   //   m_mapdirFolderName[strDir] = canew(stringa);
   //if(m_mapdirFile[strDir].is_null())
   //   m_mapdirFile[strDir] = canew(stringa);
   //if(m_mapdirFileName[strDir].is_null())
   //   m_mapdirFileName[strDir] = canew(stringa);
   if (m_mapdirFileSize[strDir].is_null())
      m_mapdirFileSize[strDir] = canew(int64_array);
   if (m_mapdirFolderSize[strDir].is_null())
      m_mapdirFolderSize[strDir] = canew(int64_array);
   if(m_mapdirFileDir[strDir].is_null())
      m_mapdirFileDir[strDir] = canew(bool_array);
   if(m_mapdirFolderDir[strDir].is_null())
      m_mapdirFolderDir[strDir] = canew(bool_array);


   ::file::patha  & straThisDir         = m_mapdirFolder[strDir];
   ::file::patha  & straThisDirName     = m_mapdirFolderName[strDir];
   ::file::patha  & straThisFile        = m_mapdirFile[strDir];
   ::file::patha  & straThisFileName    = m_mapdirFileName[strDir];
   int64_array    & iaThisFileSize      = *m_mapdirFileSize[strDir];
   int64_array    & iaThisFolderSize    = *m_mapdirFolderSize[strDir];
   bool_array     & baThisFileDir       = *m_mapdirFileDir[strDir];
   bool_array     & baThisFolderDir     = *m_mapdirFolderDir[strDir];


   straThisDir          = straDir;
   straThisDirName      = straDirName;
   straThisFile         = straFile;
   straThisFileName     = straFileName;
   iaThisFileSize       = iaFileSize;
   iaThisFolderSize     = iaFolderSize;
   baThisFileDir        = baFileDir;
   baThisFolderDir      = baFolderDir;



   m_maplsTimeout.set_at(strDir, get_tick_count() + ((5000) * 4));

   return true;
}

bool ifs::is_dir(const char * pszPath)
{
   //xml::node node(get_app());

   if(pszPath == NULL || strlen(pszPath) == 0)
   {
      return true;
   }

   if(stricmp_dup(pszPath, "uifs://") == 0)
   {
      return true;
   }


   defer_initialize();


   string strPath(pszPath);

   ::str::ends_eat(strPath, "/");
   ::str::ends_eat(strPath, "\\");


   uint32_t dwTimeout;

   synch_lock sl(data_mutex());

   if(m_mapfileTimeout.Lookup(strPath, dwTimeout))
   {

      if(::get_tick_count() > dwTimeout)
      {

         sl.unlock();
         ::file::patha patha;
         ::file::patha straTitle;
         int64_array iaSize;
         bool_array baDir;
         ls(System.dir().name(strPath), &patha, &straTitle, &iaSize, &baDir);
         sl.lock();
      }
      else
      {
         return false;
      }
   }

   if(m_mapdirTimeout.Lookup(strPath, dwTimeout))
   {
      if(::get_tick_count() > dwTimeout)
      {

         sl.unlock();

         ::file::patha patha;
         ::file::patha straTitle;
         int64_array iaSize;
         bool_array baDir;
         ls(System.dir().name(strPath),&patha,&straTitle,&iaSize, &baDir);

         sl.lock();

         if(m_mapdirTimeout.Lookup(strPath, dwTimeout))
         {
            return true;
         }
      }
      else
      {
         return true;
      }
   }

   return false;

}

string ifs::file_name(const char * pszPath)
{

   string strPath(pszPath);

   if(!::str::begins_eat_ci(strPath, "ifs://") && !::str::begins_eat_ci(strPath, "uifs://"))
   {
      return "";
   }

   strsize iFind = strPath.reverse_find("/");

   if(iFind < 0)
      iFind = -1;

   return strPath.Mid(iFind + 1);

}

bool ifs::file_move(const char * pszDst, const char * pszSrc)
{
   UNREFERENCED_PARAMETER(pszDst);
   UNREFERENCED_PARAMETER(pszSrc);
   return true;
}


::file::buffer_sp ifs::get_file(var varFile, UINT nOpenFlags, fesp * pfesp)
{

   if(pfesp != NULL)
   {
      ::release(pfesp->m_p);
   }

   ::fesp fesp;

   ::file::buffer_sp spfile;

   spfile = canew(ifs_file(get_app(), varFile));

   string strUrl;

   if(varFile.get_type() == var::type_propset)
   {
      if(varFile.has_property("url"))
      {
         strUrl = varFile["url"];
      }
      else
      {
         strUrl = varFile.propset().at(0).get_value();
      }
   }
   else
   {
      strUrl = varFile;
   }

   fesp = spfile->open(strUrl,nOpenFlags);

   if(!fesp)
   {
      
      spfile.release();

      if(pfesp != NULL)
      {

         *pfesp = fesp;

      }

   }

   return spfile;

}


bool ifs::file_exists(const char * pszPath)
{

   return ::fs::data::file_exists(pszPath);

}


var ifs::file_length(const char * pszPath)
{

   return ::fs::data::file_length(pszPath);

}


void ifs::defer_initialize()
{

   if(!m_bInitialized)
   {


      m_bInitialized = true;

   }

}

bool ifs::is_zero_latency(const char * psz)
{

   return false;

}
