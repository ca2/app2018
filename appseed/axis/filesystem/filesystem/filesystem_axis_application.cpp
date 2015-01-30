#include "framework.h"
#include "axis/net/net_sockets.h"


namespace file
{


   namespace axis
   {


      application::application(::aura::application * papp) :
         element(papp),
         ::file::application(papp)
      {
      }

      application::~application()
      {
      }

      void application::copy(const char * pszNew,const char * pszOld,bool bFailIfExists,e_extract eextract)
      {
         return m_pauraapp->m_paurasystem->m_spfile->copy(pszNew,pszOld,bFailIfExists,eextract,m_pauraapp);
      }

      void application::trash_that_is_not_trash(const char * psz)
      {
         return m_pauraapp->m_paurasystem->m_spfile->trash_that_is_not_trash(psz,m_pauraapp);
      }

      void application::trash_that_is_not_trash(stringa & stra)
      {
         return m_pauraapp->m_paurasystem->m_spfile->trash_that_is_not_trash(stra,m_pauraapp);
      }

      void application::replace(const char * pszContext,const char * pszFind,const char * pszReplace)
      {
         return m_pauraapp->m_paurasystem->m_spfile->replace(pszContext,pszFind,pszReplace,m_pauraapp);
      }

      bool application::exists(const char * pszPath)
      {

         if(::str::begins_ci(pszPath,"uifs://"))
         {

            return AppUser(m_pauraapp).m_pifs->file_exists(pszPath);

         }

         return m_pauraapp->m_paurasystem->m_spfile->exists(pszPath,m_pauraapp);

      }


      bool application::exists(const string & strPath)
      {

         if(::str::begins_ci_iws(strPath,"uifs://"))
         {
            ::fontopus::user & user = AppUser(m_pauraapp);
            return user.m_pifs->file_exists(strPath);
         }

         return m_pauraapp->m_paurasystem->m_spfile->exists(strPath,m_pauraapp);

      }

      bool application::exists(const var & var)
      {

         const string & strPath = var.get_string();

         if(::str::begins_ci_iws(strPath,"uifs://"))
         {
            return AppUser(m_pauraapp).m_pifs->file_exists(strPath);
         }

         return m_pauraapp->m_paurasystem->m_spfile->exists(strPath,m_pauraapp);

      }


      var application::length(const char * pszPath)
      {

         if(::str::begins_ci(pszPath,"uifs://"))
         {

            return AppUser(m_pauraapp).m_pifs->file_length(pszPath);

         }

         return m_pauraapp->m_paurasystem->m_spfile->length(pszPath,m_pauraapp);

      }


      var application::length(const string & strPath)
      {

         if(::str::begins_ci_iws(strPath,"uifs://"))
         {

            return AppUser(m_pauraapp).m_pifs->file_length(strPath);

         }

         return m_pauraapp->m_paurasystem->m_spfile->length(strPath,m_pauraapp);

      }


      var application::length(const var & var)
      {

         const string & strPath = var.get_string();

         if(::str::begins_ci_iws(strPath,"uifs://"))
         {

            return AppUser(m_pauraapp).m_pifs->file_length(strPath);

         }

         return m_pauraapp->m_paurasystem->m_spfile->length(strPath,m_pauraapp);

      }


      string application::time(const char * pszBasePath,int32_t iDepth,const char * pszPrefix,const char * pszSuffix)
      {

         return m_pauraapp->m_paurasystem->m_spfile->time(m_pauraapp,pszBasePath,iDepth,pszPrefix,pszSuffix);

      }


      string application::time_square(const char * pszPrefix,const char * pszSuffix)
      {

         return m_pauraapp->m_paurasystem->m_spfile->time_square(m_pauraapp,pszPrefix,pszSuffix);

      }


      string application::time_log(const char * pszId)
      {
         return m_pauraapp->m_paurasystem->m_spfile->time_log(m_pauraapp,pszId);
      }

      ::file::buffer_sp application::time_square_file(const char * pszPrefix,const char * pszSuffix)
      {
         return m_pauraapp->m_paurasystem->m_spfile->time_square_file(m_pauraapp,pszPrefix,pszSuffix);
      }

      ::file::buffer_sp application::get(const char * name)
      {
         return m_pauraapp->m_paurasystem->m_spfile->get(name,m_pauraapp);
      }

      string application::as_string(var varFile)
      {
         return m_pauraapp->m_paurasystem->m_spfile->as_string(varFile,m_pauraapp);
      }

      string application::as_string(var varFile,var & varQuery)
      {
         return m_pauraapp->m_paurasystem->m_spfile->as_string(varFile,varQuery,m_pauraapp);
      }

      void application::as_memory(var varFile,primitive::memory & mem)
      {
         return m_pauraapp->m_paurasystem->m_spfile->as_memory(varFile,mem,m_pauraapp);
      }

      void application::lines(stringa & stra,var varFile)
      {
         return m_pauraapp->m_paurasystem->m_spfile->lines(stra,varFile,m_pauraapp);
      }

      bool application::put_contents(var varFile,const void * pvoidContents,::count count)
      {
         return m_pauraapp->m_paurasystem->m_spfile->put_contents(varFile,pvoidContents,count,m_pauraapp);
      }

      bool application::put_contents(var varFile,const char * lpcszContents)
      {
         return m_pauraapp->m_paurasystem->m_spfile->put_contents(varFile,lpcszContents,m_pauraapp);
      }

      bool application::put_contents(var varFile,::file::reader & reader)
      {
         return m_pauraapp->m_paurasystem->m_spfile->put_contents(varFile,reader,m_pauraapp);
      }

      bool application::put_contents(var varFile,primitive::memory & mem)
      {
         return m_pauraapp->m_paurasystem->m_spfile->put_contents(varFile,mem,m_pauraapp);
      }

      bool application::put_contents_utf8(var varFile,const char * lpcszContents)
      {
         return m_pauraapp->m_paurasystem->m_spfile->put_contents_utf8(varFile,lpcszContents,m_pauraapp);
      }

      string application::sys_temp(const char * pszName,const char * pszExtension)
      {
         return m_pauraapp->m_paurasystem->m_spfile->sys_temp(pszName,pszExtension,m_pauraapp);
      }


      void application::dtf(const char * pszFile,const char * pszDir)
      {
         return m_pauraapp->m_paurasystem->m_spfile->dtf(pszFile,pszDir,m_pauraapp);
      }

      void application::dtf(const char * pszFile,stringa & stra,stringa & straRelative)
      {
         return m_pauraapp->m_paurasystem->m_spfile->dtf(pszFile,stra,straRelative,m_pauraapp);
      }

      void application::ftd(const char * pszDir,const char * pszFile)
      {
         return m_pauraapp->m_paurasystem->m_spfile->ftd(pszDir,pszFile,m_pauraapp);
      }


      bool application::crypto_set(var varFile,const char * pszData,const char * pszSalt)
      {
         // attention: operation on System crypto
         return m_pauraapp->m_paxissystem->m_spcrypto->file_set(varFile,pszData,pszSalt,m_pauraapp);
      }

      bool application::crypto_get(var varFile,string & str,const char * pszSalt)
      {
         // attention: operation on System crypto
         return m_pauraapp->m_paxissystem->m_spcrypto->file_get(varFile,str,pszSalt,m_pauraapp);
      }

      ::file::buffer_sp application::friendly_get_file(var varFile,UINT nOpenFlags)
      {
         try
         {
            return get_file(varFile,nOpenFlags);
         }
         catch(...)
         {
            return NULL;
         }
      }

      ::file::buffer_sp application::get_file(var varFile,UINT nOpenFlags)
      {

         ::file::buffer_sp spfile;

         string strPath;

         if(varFile.get_type() == var::type_element)
         {

            spfile = varFile.cast < ::file::stream_buffer >();

            if(spfile.is_set())
               return spfile;

         }
         else if(varFile.get_type() == var::type_string)
         {

            strPath = varFile;

            strPath.trim("\"'");

         }
         else if(varFile.get_type() == var::type_stra)
         {

            if(varFile.stra().get_count() > 0)
            {

               strPath = varFile.stra()[0];

            }

            strPath.trim("\"'");

         }
         else if(varFile.get_type() == var::type_propset)
         {

            if(varFile.has_property("url"))
            {

               strPath = varFile["url"];

               strPath.trim("\"'");

            }

         }

         if(varFile.get_type() == var::type_propset && varFile.propset()["file"].cast < ::file::binary_buffer >() != NULL)
         {

            spfile = varFile.propset()["file"].cast < ::file::binary_buffer >();

         }
         else if(::str::find_ci(".zip:",strPath) >= 0)
         {

            zip::InFile * pinfile = new zip::InFile(get_app());

            if(pinfile != NULL)
            {

            if(!pinfile->unzip_open(strPath, 0))
            {

            delete pinfile;

            pinfile = NULL;

            }

            }

            spfile = pinfile;

         }
         else if(::str::begins(strPath,"http://") || ::str::begins(strPath,"https://"))
         {

            ::url_domain domain;

            domain.create(System.url().get_server(strPath));

            if(domain.m_strRadix == "ca2" && ::str::begins(System.url().get_object(strPath),"/matter/"))
            {

               string strFile(strPath);

               if(::str::ends(strPath,"en_us_international.xml"))
               {
                  TRACE("Debug Here");
               }

               if(::str::ends(strPath,"text_select.xml"))
               {
                  TRACE("Debug Here");
               }

               if(::str::ends(strPath,"arialuni.ttf"))
               {
                  TRACE("Debug Here : arialuni.ttf");
               }

#ifdef WINDOWS
               strFile.replace("://","_\\");
#else
               strFile.replace("://", "_/");
#endif
               strFile = System.dir().appdata("cache/" + strFile);

               strFile = strFile + ".local_copy";

               single_lock sl(&System.http().m_mutexDownload,true);

               if(Application.file().exists(strFile) && !(System.http().m_straDownloading.contains(strPath) || System.http().m_straExists.contains(strPath)))
               {

                  sl.unlock();

                  spfile = Application.alloc(System.type_info < ::file::binary_buffer >());

                  try
                  {

                     if(spfile->open(strFile,nOpenFlags))
                     {
                        TRACE("from_exist_cache:\"" + strPath + "\"");
                        return spfile;

                     }
                  }
                  catch(...)
                  {

                  }

                  try
                  {

                     spfile.release();

                  }
                  catch(...)
                  {
                  }

               }


               var varQuery;

               varQuery["raw_http"] = true;

               property_set set(get_app());

               set["raw_http"] = true;

               //            bool bOk = true;

               sl.lock();

               while(System.http().m_straDownloading.contains(strPath) || System.http().m_straExists.contains(strPath))
               {
                  sl.unlock();
                  Sleep(100);
                  sl.lock();
               }

               if(!Sys(m_pauraapp).http().m_straDownloading.contains(strPath) && Sess(m_pauraapp).m_http.exists(strPath,&varQuery,set))
               {

                  System.http().m_straDownloading.add(strPath);

                  sl.unlock();

                  spfile = new ::sockets::http_buffer(get_app());

                  if(!spfile->open(strPath,nOpenFlags))
                  {
                     sl.lock();

                     System.http().m_straDownloading.remove(strPath);

                     sl.unlock();

                     spfile.release();

                  }
                  else
                  {

                     try
                     {

                        ::file::input_stream is(spfile);

                        System.file().output(m_pauraapp,strFile,&System.compress(),&::axis::compress::null,is);

                     }
                     catch(...)
                     {
                     }

                     sl.lock();

                     System.http().m_straDownloading.remove(strPath);

                     sl.unlock();

                     spfile->seek_to_begin();

                  }

               }


            }
            else
            {

               spfile = new ::sockets::http_buffer(get_app());

               spfile->oprop("http_set") = varFile["http_set"];

               if(!spfile->open(strPath,nOpenFlags))
               {

                  spfile.release();

               }

            }

         }
         else if(::str::begins(strPath,"ifs://") || ::str::begins(strPath,"uifs://"))
         {

            if(&AppUser(m_pauraapp) == NULL)
            {

               spfile = NULL;

            }
            else
            {

               spfile = AppUser(m_pauraapp).m_pifs->get_file(varFile,nOpenFlags);

            }

         }
         /* xxx      else if(::str::begins(strPath, "fs://"))
               {

               if(&Session == NULL)
               {

               spfile = NULL;

               }
               else
               {

               spfile = Session.m_prfs->get_file(varFile, nOpenFlags);

               }

               } */
         else if(::str::begins_eat_ci(strPath,"matter://"))
         {

            sp(::aura::application) papp;

            if(System.url().get_server("matter://" + strPath) == m_pauraapp->m_strAppName)
            {

               strPath = System.url().get_object("matter://" + strPath).Mid(1);

               spfile = Application.alloc(System.type_info < ::file::binary_buffer >());

               if(!spfile->open(App(m_pauraapp).dir().matter(strPath),nOpenFlags))
               {

                  spfile.release();

               }

            }
            else if(&Session != NULL && Session.m_mapApplication.Lookup(System.url().get_server("matter://" + strPath),papp) && App(m_pauraapp).m_strAppName.has_char())
            {

               spfile = App(papp).file().get_file("matter://" + strPath,nOpenFlags);

            }
            else
            {

               spfile = get_file(App(m_pauraapp).dir().matter(strPath),nOpenFlags);

            }

         }
         else
         {

            if(strPath.is_empty())
            {
               TRACE("::application::get_file file with empty name!!");
               return spfile;
            }

            if((nOpenFlags & ::file::mode_create) == 0 && !exists(strPath))
            {
               TRACE("::application::file does not exist!! : \"%s\"",strPath);
               return spfile;
            }

            spfile = Application.alloc(System.type_info < ::file::binary_buffer >());

            if(!spfile->open(strPath,nOpenFlags))
            {

               spfile.release();

            }

         }

         if(spfile.is_null())
         {

            throw ::file::exception(m_pauraapp,::file::exception::none,-1,strPath);

         }

         return spfile;

      }

      /*   ::file::byte_stream application::get_byte_stream(var varFile, UINT nOpenFlags)
         {

         return get_file(varFile, nOpenFlags);

         }
         */

   } // namespace axis


} // namespace core



























