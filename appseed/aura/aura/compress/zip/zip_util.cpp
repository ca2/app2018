#include "framework.h"
//#include"in_file.h"
#include "zip_unzip.h"


namespace str
{

   ::index begins_eat_ci(string & str,stringa & stra)
   {

      for(index i = 0; i < stra.get_count(); i++)
      {

         if(::str::begins_eat_ci(str,stra[i]))
            return i;

      }

      return -1;

   }


   ::index begins_eat(string & str,stringa & stra)
   {

      for(index i = 0; i < stra.get_count(); i++)
      {

         if(::str::begins_eat(str,stra[i]))
            return i;

      }

      return -1;

   }



}

namespace zip
{


   util::util()
   {
   }

   util::~util()
   {

   }


   bool util::ls(::aura::application * papp, ::file::listing & listing)
   {

      string strZip;

      string strRemain;

      string strLastZip;

      if(::str::ends_ci(listing.m_path, ".zip"))
      {

         strZip = listing.m_path;

         strLastZip = strZip;

         strZip += ":";

      }
      else if(::str::find_file_extension("zip:", listing.m_path) >= 0)
      {

         strZip = listing.m_path;

         strRemain = strZip.Mid(strZip.reverse_find(".zip:") + strlen(".zip:"));

         strLastZip = strZip.Left(strZip.reverse_find(".zip:") + strlen(".zip"));

      }
      else
      {

         return false;


      }

      in_file infile(papp);

      if(!infile.unzip_open(strZip, 0))
      {
         return false;
      }

      void * pf = infile.get_zip_file()->m_pfUnzip;

      string str;

      string wstrFolder;

      stringa wstraFolder;

      strRemain.replace("\\", "/");

      ::str::begins_eat(strRemain, "/");

      if(strRemain.has_char())
      {

         if(!::str::ends(strRemain,"/"))
         {

            strRemain += "/";

         }

      }

      unz_file_info fi;

      if(pf != NULL)
      {

         int iLastZip;

         iLastZip = int( ::file::path(strLastZip + ":").length());

         while(true)
         {

            CHAR szTitle[_MAX_PATH];

            unzGetCurrentFileInfo(
            pf,
            &fi,
            szTitle,
            _MAX_PATH,
            NULL, // extra Field
            0,
            NULL, // comment
            0);

            string strTitle(szTitle);

            if(strRemain != strTitle && ((strRemain.is_empty() &&
                                          (strTitle.find("/") < 0  || strTitle.find("/") == (strTitle.get_length() - 1)))
                                         || (strRemain.has_char() && ::str::begins_eat_ci(strTitle, strRemain))))
            {

               if(listing.m_bRecursive || strTitle.find("/") < 0 || strTitle.find("/") == (strTitle.get_length() - 1))
               {

                  listing.add(::file::path(strLastZip + ":" + strRemain + strTitle));
                  listing.last().m_iRelative = iLastZip;
                  listing.last().m_iDir = ::str::ends(szTitle, "/") || ::str::ends(szTitle, "\\") || ::str::ends(szTitle, ".zip");
                  listing.last().m_iSize = fi.uncompressed_size;

               }

            }

            if(unzGoToNextFile(pf) != UNZ_OK)
            {

               break;

            }

         }

      }

      return true;

   }


   bool util::has_sub_folder(::aura::application * papp, const char * lpszFileName)
   {
      string strZip;
      if(::str::ends_ci(lpszFileName, ".zip"))
      {
         strZip = lpszFileName;
         strZip += ":";
      }
      else if(::str::find_ci(".zip:", lpszFileName) > 0)
      {
         strZip = lpszFileName;
      }
      else
         return false;

      in_file infile(papp);

      strZip.replace("\\", "/");
      if(!::str::ends_ci(strZip, "/"))
      {
         strZip += "/";
      }

      return infile.unzip_open(strZip, 0) != FALSE;


      //string wstrFileName(lpszFileName);
//      smart_pointer_array < File, File & > filea;

      /*int32_t iStart = 0;
      int32_t iFind;
      iFind  = wstrFileName.find(L".zip", iStart);*/


      /*    stringa wstraPath;
          cregexp_util::add_tokens(wstraPath, lpszFileName, "((([A-Z]:)|([^:]))[^:]+\\.zip)",1);

          if(wstraPath.get_size() == 0)
             return false;


          filea.add(new File(get_app()));
          if(!filea.last().open(wstraPath[0]))
             return false;


          stringa wstraPrefix;
          string str;
          int32_t i;
          smart_pointer_array < in_file, in_file & > izfilea;
       //   smart_pointer_array < buffered_file, buffered_file & > bzfilea;
          for(i = 1; i < wstraPath.get_size(); i++)
          {
             izfilea.add(new in_file(get_app()));
             str =  wstraPath[i];
             izfilea.last().open(&filea.last(), str);
         //    bzfilea.add(new buffered_file(&izfilea.last(), 1024 * 1024, 1024 * 1024));
             filea.add(new File(get_app()));
             //filea.last().open(&bzfilea.last());
             filea.last().open(&izfilea.last());
             wstraPrefix.add(wstraPath[i]);
          }

          string wstrPrefix;
          wstraPrefix.implode(wstrPrefix, ":");

          if(wstrPrefix.get_length() > 0)
          {
             wstrPrefix += ":";
          }

          unzFile pf = filea.last().m_pf;
          string wstrFolder;
          stringa wstraFolder;
          string wstrZip;
          if(pf != NULL)
          {
             while(true)
             {
       //         if(unzOpenCurrentFile(pf) == UNZ_OK)
                {
                   const int32_t BUFSIZE = 4096;
                   WCHAR lpPathBuffer[BUFSIZE];
                   GetTempPathW(BUFSIZE,   // length of the buffer
                      lpPathBuffer);      // buffer for path


                    CHAR szTitle[_MAX_PATH];

                   unzGetCurrentFileInfo(
                      pf,
                      NULL,
                      szTitle,
                      _MAX_PATH,
                      NULL, // extra Field
                      0,
                      NULL, // comment
                      0);

                   str = szTitle;
                   if(str.Right(1) == "/")
                   {
                      return true;
                   }
                   wstrZip = lpszFileName;
                   wstrZip += ":";
                   wstrZip + str;
                   if(IsUnzipable(wstrZip))
                   {
                      return true;
                   }

         //          unzCloseCurrentFile(pf);
                }
                if(unzGoToNextFile(pf) != UNZ_OK)
                {
                   break;
                }
             }
          }*/
      return false;
   }


   bool util::is_file_or_dir(::aura::application * papp, const char * pszPath, ::file::e_type * petype)
   {

      return extract(papp, pszPath, NULL, petype);

   }


   bool util::extract(::aura::application * papp, const char * lpszFileName, const char * lpszExtractFileName, ::file::e_type * petype)
   {

      in_file infile(papp);

      if(!infile.unzip_open(lpszFileName, petype))
      {

         if (is_set(petype))
         {

            *petype = ::file::type_none;

         }

         return false;

      }

      if (lpszExtractFileName == NULL)
      {

         return true;

      }

      ::file::file_sp spfile = Sess(papp).file().get_file(lpszExtractFileName, ::file::mode_create | ::file::mode_write | ::file::defer_create_directory);

      if(spfile.is_set())
      {
         return infile.dump(spfile);
      }
      else
      {
         return false;
      }

   }


   bool util::extract(::aura::application * papp, memory & m, const char * lpszFileName)
   {

      in_file infile(papp);

      if(!infile.unzip_open(lpszFileName, 0))
      {

         return false;

      }

      return infile.dump(m);

   }


   bool util::extract_all(::aura::application * papp,const char * pszDir,var varFile, stringa * pstraFilter, bool_array * pbaBeginsFilterEat)
   {

      ::file::file_sp pfile(App(papp).file().get_file(varFile,::file::type_binary | ::file::mode_read));

      if(pfile.is_null())
         return false;

      in_file infile(papp);

      if(!infile.unzip_open(pfile))
      {
         return false;
      }

      unzFile pf = infile.get_zip_file()->m_pfUnzip;
      string str;
      string wstrFolder;
      stringa wstraFolder;


      unz_file_info fi;
      if(pf != NULL)
      {
         while(true)
         {

            CHAR szTitle[_MAX_PATH];

            unzGetCurrentFileInfo(
            pf,
            &fi,
            szTitle,
            _MAX_PATH,
            NULL, // extra Field
            0,
            NULL, // comment
            0);
            string strTitle(szTitle);
            if(::str::ends(szTitle, "/") || ::str::ends(szTitle, "\\"))
            {
            }
            else if(infile.locate(strTitle) && (pstraFilter == NULL || ::str::begins_eat_ci(strTitle, *pstraFilter) >= 0))
            {

               ::file::file_sp spfile = Sess(pfile->get_app()).file().get_file(
                                        ::file::path(pszDir) / strTitle,
                                        ::file::mode_create | ::file::mode_write | ::file::defer_create_directory);


               if(spfile.is_set())
               {
                  infile.dump(spfile);
               }
               else
               {
                  //   return false;
               }
            }
            if(unzGoToNextFile(pf) != UNZ_OK)
            {
               break;
            }
         }
      }

      return true;

   }


   bool util::is_unzippable(::aura::application * papp, const char * lpszFileName)
   {

      string str(lpszFileName);
      if(str.get_length() < 4)
         return false;
      if(str.Right(4) != ".zip")
         return false;

      ::zip::file file(papp);

      return file.unzip_open(lpszFileName);
   }

} // namespace zip

