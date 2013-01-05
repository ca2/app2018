#include "framework.h"
#ifndef METROWIN
#include <openssl/ssl.h>
#include <openssl/md5.h>
#endif

CLASS_DECL_c void NESSIEinit(struct NESSIEstruct * const structpointer);
CLASS_DECL_c void NESSIEadd(const uchar * const source, uint_ptr sourceBits, struct NESSIEstruct * const structpointer);
CLASS_DECL_c void NESSIEfinalize(struct NESSIEstruct * const structpointer, uchar * const result);


namespace ca4
{

   file::file()
   {
      m_path.m_pfile = this;
   }

   file::path & file::path36()
   {
      return m_path;
   }

   bool file::path::rename(const char *pszNew, const char *psz, ::ca::application * papp)
   {
      string strDir = System.dir().name(psz);
      string strDirNew = System.dir().name(pszNew);
      if(strDir == strDirNew)
      {
         string strOld = System.file().name_(psz);
      }
      throw not_implemented(get_app());
      //if(!System.file_as_string().move(psz, pszNew))
      {
         gen::property_set propertyset;
         System.message_box("err\\::fontopus::user\\file_system\\could_not_rename_file.xml", propertyset);
         return false;
      }
      return true;
   }

   string file::md5(const char * psz)
   {

      ex1::filesp spfile(get_app());

      try
      {
         if(!spfile->open(psz, ::ex1::file::type_binary | ::ex1::file::mode_read))
            return "";
      }
      catch(ex1::file_exception * pe)
      {
         gen::del(pe);
         return "";
      }


      int32_t iBufSize = 1024 * 256;

      primitive::memory buf;

      buf.allocate(1024 * 256);

      ::crypto::md5::context ctx(get_app());

      int32_t iRead;

      while((iRead = (int32_t) spfile->read(buf, iBufSize)) > 0)
      {

         ctx.update(buf.get_data(), iRead);

      }

      return ctx.to_hex();

   }


   void file::dtf(const char * pszFile, const char * pszDir, ::ca::application * papp)
   {
      stringa stra;
      stringa straRelative;
      System.dir().rls(papp, pszDir, &stra, NULL, &straRelative);
      dtf(pszFile, stra, straRelative, papp);
   }

   void file::dtf(const char * pszFile, stringa & stra, stringa & straRelative, ::ca::application * papp)
   {

      ex1::filesp spfile = App(papp).get_file(pszFile, ::ex1::file::mode_create | ::ex1::file::mode_write  | ::ex1::file::type_binary);

      if(spfile.is_null())
         throw "failed";

      string strVersion;

      strVersion = "fileset v1";

      ::crypto::md5::context ctx(get_app());

      write_ex1_string(spfile, NULL, strVersion);

      ex1::filesp file2(get_app());

      ::primitive::memory_size iBufSize = 1024 * 1024;

      ::primitive::memory_size uiRead;

      primitive::memory buf;

      buf.allocate(iBufSize);

      string strMd5 = "01234567012345670123456701234567";

      uint64_t iPos;

      for(int32_t i = 0; i < stra.get_size(); i++)
      {
         if(gen::str::ends_ci(stra[i], ".zip"))
         {
         }
         else if(System.dir().is(stra[i], get_app()))
            continue;
         write_n_number(spfile, NULL, 1);
         iPos = spfile->get_position();
         write_ex1_string(spfile, NULL, strMd5);
         ctx.reset();
         write_ex1_string(spfile, &ctx, straRelative[i]);
         if(!file2->open(stra[i], ::ex1::file::mode_read | ::ex1::file::type_binary))
            throw "failed";
         write_n_number(spfile, &ctx, (int32_t) file2->get_length());
         while((uiRead = file2->read(buf, iBufSize)) > 0)
         {
            spfile->write(buf, uiRead);
            ctx.update(buf, uiRead);
         }
         spfile->seek(iPos, ::ex1::seek_begin);
         strMd5 = ctx.to_hex();
         write_ex1_string(spfile, NULL, strMd5);
         spfile->seek_to_end();

      }
      write_n_number(spfile, NULL, 2);
   }

   void file::ftd(const char * pszDir, const char * pszFile, ::ca::application * papp)
   {
      string strVersion;
      ex1::filesp spfile = App(papp).get_file(pszFile, ::ex1::file::mode_read  | ::ex1::file::type_binary);
      if(spfile.is_null())
         throw "failed";
      read_ex1_string(spfile, NULL, strVersion);
      int64_t n;
      string strRelative;
      string strMd5;
      string strMd5New;
      int32_t iBufSize = 1024 * 1024;
      primitive::memory buf;
      buf.allocate(iBufSize);
      int64_t iLen;
      ::crypto::md5::context ctx(get_app());
      ex1::filesp file2(get_app());
      ::primitive::memory_size uiRead;
      if(strVersion == "fileset v1")
      {
         while(true)
         {
            read_n_number(spfile, NULL, n);
            if(n == 2)
               break;
            read_ex1_string(spfile, NULL, strMd5);
            ctx.reset();
            read_ex1_string(spfile, &ctx, strRelative);
            string strPath = System.dir().path(pszDir, strRelative);
            App(papp).dir().mk(System.dir().name(strPath));
            if(!file2->open(strPath, ::ex1::file::mode_create | ::ex1::file::type_binary | ::ex1::file::mode_write))
               throw "failed";
            read_n_number(spfile, &ctx, iLen);
            while(iLen > 0)
            {
             uiRead = spfile->read(buf, (UINT)  (min(iBufSize, iLen )));
             if(uiRead == 0)
                break;
               file2->write(buf, uiRead);
               ctx.update(buf, uiRead);
               iLen -= uiRead;
            }
            file2->close();
            strMd5New = ctx.to_hex();
            if(strMd5 != strMd5New)
               throw "failed";
         }
      }
   }

   void file::write_n_number(ex1::file * pfile, ::crypto::md5::context * pctx, int64_t iNumber)
   {

      string str;
      
      str.Format("%I64dn", iNumber);

      pfile->write((const char *) str, str.get_length());

      if(pctx != NULL)
      {

         pctx->update((const char *) str, (int32_t) str.get_length());

      }

   }

   void file::read_n_number(ex1::file * pfile, ::crypto::md5::context * pctx, int64_t & iNumber)
   {
      
      uint64_t uiRead;

      string str;

      char ch;

      while((uiRead = pfile->read(&ch, 1)) == 1)
      {

         if(ch >= '0' && ch <= '9')
            str += ch;
         else
            break;

         if(pctx != NULL)
         {
            pctx->update(&ch, 1);
         }

      }

      if(ch != 'n')
         throw "failed";

      if(pctx != NULL)
      {
         pctx->update(&ch, 1);
      }

      iNumber = gen::str::to_int64(str);

   }

   void file::write_ex1_string(ex1::file * pfile, ::crypto::md5::context * pctx, string & str)
   {
      count iLen = str.get_length();
      write_n_number(pfile, pctx, iLen);
      pfile->write((const char *) str, str.get_length());
      if(pctx != NULL)
      {
         pctx->update((const char *) str, (int32_t) str.get_length());
      }
   }

   void file::read_ex1_string(ex1::file * pfile, ::crypto::md5::context * pctx, string & str)
   {
      int64_t iLen;
      read_n_number(pfile, pctx, iLen);
      LPSTR lpsz = str.GetBufferSetLength((strsize) (iLen + 1));
      pfile->read(lpsz, (primitive::memory_size) iLen);
      if(pctx != NULL)
      {
         int64_t iProcessed = 0;
         while(iLen - iProcessed > 0)
         {
            int32_t iProcess = (int32_t) min(1024 * 1024, iLen - iProcessed);
            pctx->update(&lpsz[iProcessed], iProcess);
            iProcessed += iProcess;
         }
      }
      lpsz[iLen] = '\0';
      str.ReleaseBuffer();
   }


} // namespace ca36
