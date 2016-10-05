//#include "framework.h"


namespace filemanager
{


   operation::operation(::aura::application * papp):
      object(papp)
   {

      m_oswindowCallback = NULL;

      m_bReplaceAll = false;

      m_eoperation = operation_none;

      m_iBufferSize = 1024 * 1024;

   }

   operation::~operation()
   {
   }


   e_operation operation::get_operation()
   {
      return m_eoperation;
   }

   void operation::set_operation(e_operation eoperation)
   {
      m_eoperation = eoperation;
   }


   bool operation::set_copy(::file::listing & stra,const ::file::path & pszDestBase,const ::file::path & pszSrcBase,bool bExpand)
   {

      set_operation(operation_copy);

      if(bExpand)
      {

         expand(m_stra,stra);

      }
      else
      {

         m_stra = stra;

      }
      
      m_str = pszDestBase;

      string strBase;

      if(pszSrcBase.has_char())
      {

         strBase = pszSrcBase;

      }
      else
      {

         strBase = stra[0].folder();

         ::file::path strCompare;

         for(int32_t i = 1; i < stra.get_size(); i++)
         {
            strCompare = stra[i].folder();
            for(int32_t j = 0; j < MIN(strCompare.get_length(),strBase.get_length()); j++)
            {
               if(strCompare[j] != strBase[j])
               {
                  strBase = ::file::path(strCompare.Left(j)).folder();
                  break;
               }
            }
         }
      }
      m_strBase = strBase;
      m_dRead = 0.0;
      m_iFile = 0;
      if(!initialize())
         return false;
      return true;
   }

   bool operation::set_move(::file::listing & stra,const ::file::path & psz)
   {
      set_operation(operation_move);
      m_stra = stra;
      m_str = psz;
      m_dRead = 0.0;
      m_iFile = 0;
      if(!initialize())
         return false;
      return true;
   }

   bool operation::set_delete(::file::listing & stra)
   {
      set_operation(operation_delete);
      m_stra = stra;
      m_dRead = 0.0;
      m_iFile = 0;
      if(!initialize())
         return false;
      return true;
   }

   bool operation::open_src_dst(const ::file::path & pszSrc,::file::path & strDst,const ::file::path & pszDir)
   {

      if(Application.dir().is(pszSrc) && !::str::ends_ci(pszSrc,".zip"))
      {
         
         Application.dir().mk(strDst.folder());

         return false;

      }

      m_fileSrc = Application.file().get_file(pszSrc,::file::mode_read | ::file::type_binary | ::file::share_deny_write);

      if(m_fileSrc.is_null())
      {

         TRACE("\n Could not open source file(%d)=%s",m_iFile,pszSrc);

         return false;

      }

      if(!m_bReplaceAll)
      {

         //if(System.file().exists(pszDst))
         //{
         //   property_set propertyset;
         //   propertyset["srcfile"].get_value().set_string(pszSrc);
         //   propertyset["dstfile"].get_value().set_string(pszDst);
         //   System.message_box("filemanager\\do_you_want_to_replace_the_file.xml", propertyset);
         //   return false;
         //}

         if(Application.file().exists(strDst) || Application.dir().is(strDst))
         {

            int iResult = Application.simple_message_box(m_oswindowCallback,"Do you want to overwrite?\n\nThere is already a existing file with the same name: " + strDst.name(),MB_ICONQUESTION | MB_YESNOCANCEL);

            if(iResult == IDYES)
            {
            }
            else if(iResult == IDNO)
            {
               if(!make_duplicate_name(strDst,pszDir))
               {
                  return false;
               }
            }
            else
            {
               return false;
            }

         }

      }

      Application.dir().mk(strDst.folder());

      m_fileDst = Application.file().get_file(strDst,::file::mode_write | ::file::type_binary | ::file::mode_create);

      if(m_fileDst.is_null())
      {

         TRACE("\n Could not open dest file(%d)=%s",m_iFile,strDst);

         property_set propertyset;

         propertyset["filepath"] = strDst;

         System.message_box("filemanager\\not_accessible_destination_file.xhtml",propertyset);

         return false;

      }

      TRACE("\n%d Opened %s %s",m_iFile,pszSrc,strDst);

      return true;

   }


   bool operation::start()
   {
      switch(m_eoperation)
      {
      case operation_copy:
      {
                                         
         m_iFile = 0;
                                         
         m_pchBuffer = (char *)malloc(m_iBufferSize);

         ::file::path strName = m_str / m_stra[m_iFile].Mid(m_strBase.get_length());

         if(m_str == m_strBase)
         {

            make_duplicate_name(strName,m_str);

         }

         if(!open_src_dst(m_stra[m_iFile],strName, m_str))
            return false;
      }
         break;
      case operation_delete:
      {
      }
         break;
      case operation_move:
      {
         
         m_iFile = 0;
         
         m_pchBuffer = (char *)malloc(m_iBufferSize);

         ::file::path strPath = m_str / m_stra[m_iFile].name();

         if(!open_src_dst(m_stra[m_iFile],strPath,m_str))
            return false;

      }
         break;
      default:

         break;
      }
      return true;
   }

   bool operation::step()
   {
      switch(m_eoperation)
      {
      case operation_copy:
      {
                                         if(m_iFile >= m_stra.get_size())
                                            return false;
                                         memory_size_t uiRead = m_fileSrc->read(m_pchBuffer,m_iBufferSize);
                                         if(uiRead > 0)
                                         {
                                            m_fileDst->write(m_pchBuffer,uiRead);
                                            m_daRead[m_iFile] += uiRead;
                                            m_dRead += uiRead;
                                         }
                                         else
                                         {

                                            {

                                               string strDestPath = m_fileDst->GetFilePath();

                                               m_fileDst->close();

                                               ::file::file_status st;

                                               bool bOk = true;

                                               try
                                               {

                                                  m_fileSrc->GetStatus(st);

                                               }
                                               catch(...)
                                               {

                                                  bOk = false;

                                               }

                                               if(bOk)
                                               {

                                                  bool bStatusOk = true;

                                                  try
                                                  {

                                                     System.os().set_file_status(strDestPath,st);

                                                  }
                                                  catch(...)
                                                  {

                                                     bStatusOk = false;

                                                  }

                                                  if(!bStatusOk)
                                                  {

                                                     TRACE("Failed to set status of destination file '%s' using file status of '%s' file",strDestPath,m_fileSrc->GetFilePath());

                                                  }

                                               }
                                               else
                                               {

                                                  TRACE("Failed to get status of source file '%s' for setting file status of '%s' file",m_fileSrc->GetFilePath(),strDestPath);

                                               }

                                               m_fileSrc->close();

                                            }
                                            m_iFile++;
                                            while(m_iFile < m_stra.get_size() && Application.dir().is(m_stra[m_iFile]) && !::str::ends_ci(m_stra[m_iFile],".zip"))
                                            {
                                               m_iFile++;
                                            }

                                            if(m_iFile >= m_stra.get_size())
                                               return false;
                                            
                                            ::file::path strName = m_str / m_stra[m_iFile].Mid(m_strBase.get_length());

                                            if(m_str == m_strBase)
                                            {

                                               make_duplicate_name(strName,m_str);

                                            }

                                            if(!open_src_dst(m_stra[m_iFile],strName, m_str))
                                            {

                                               return false;
                                            }
                                         }
                                         return true;
      }
      case operation_delete:
      {
                                           if(m_iFile >= m_stra.get_size())
                                              return false;

                                           Application.file().del(m_stra[m_iFile]);

                                           m_iFile++;

      }
         break;
      case operation_move:
      {
                                         
         if(m_iFile >= m_stra.get_size())
            return false;
                                         
         memory_size_t uiRead = m_fileSrc->read(m_pchBuffer,m_iBufferSize);
                                         
         m_fileDst->write(m_pchBuffer,uiRead);

         m_daRead[m_iFile] += uiRead;

         m_dRead += uiRead;

         if(uiRead == 0)
         {
                                            
            m_fileSrc->close();

            m_fileDst->close();

            Application.file().del(m_stra[m_iFile]);

            m_iFile++;

            if(m_iFile >= m_stra.get_size())
               return false;

            ::file::path strPath = m_str / m_stra[m_iFile].name();

            if(!open_src_dst(m_stra[m_iFile], strPath,m_str))
               return false;

         }
      }
         break;
      default:

         break;
      }
      return true;
   }

   bool operation::finish()
   {
      switch(m_eoperation)
      {
      case operation_copy:
      {
                                         free(m_pchBuffer);
      }
         break;
      case operation_delete:
      {
      }
         break;
      case operation_move:
      {
                                         free(m_pchBuffer);
      }
         break;
      default:

         break;
      }
      if(m_oswindowCallback != NULL)
      {
         m_oswindowCallback->send_message(m_uiCallbackMessage,m_wparamCallback);
      }
      return true;
   }


   bool operation::initialize()
   {

      m_dSize = 0.0;

      var varLen;

      for(int32_t i = 0; i < m_stra.get_size(); i++)
      {

         if(Application.dir().is(m_stra[i]) && !::str::ends_ci(m_stra[i],".zip"))
         {

            m_daSize.add(0.0);

            m_daRead.add(0.0);

         }
         else
         {

            varLen = Application.file().length(m_stra[i]);

            if(varLen.is_null())
            {

               m_daSize.add(0.0);

               m_daRead.add(0.0);

            }
            else
            {

               m_dSize += (uint32_t)varLen;

               m_daSize.add((double)(uint32_t)varLen);

               m_daRead.add(0.0);

            }

         }

      }

      return true;

   }


   int32_t operation::get_item_count()
   {

      return (int32_t)m_stra.get_size();

   }


   string operation::get_item_message(int32_t iItem)
   {

      string str;

      str.Format("Copying %s (%s) to %s", m_stra[iItem].name(),m_stra[iItem].name(),m_str);

      return str;

   }


   double operation::get_item_progress(int32_t iItem)
   {

      if(m_daSize[iItem] == 0.0)
      {
         if(m_iFile > iItem)
         {
            return 1.0;
         }
         else
         {
            return 0.0;
         }
      }
      return m_daRead[iItem] / m_daSize[iItem];
   }

   double operation::get_item_read(int32_t iItem)
   {
      return m_daRead[iItem];
   }

   double operation::get_item_size(int32_t iItem)
   {
      return m_daSize[iItem];
   }

   bool has_digit(string strName)
   {
      for(index i= 0; i < strName.get_length(); i++)
      {
         if(::isdigit_dup(strName[i]))
         {
            return true;
         }
      }
      return false;
   }

   string get_number_mask(string strName)
   {

      string strResult;
      bool bFirst = true;
      for(index i= 0; i < strName.get_length(); i++)
      {
         if(::isdigit_dup(strName[i]))
         {
            if(bFirst)
            {
               bFirst = false;
               strResult += "1";
            }
            else
            {
               strResult += "X";
            }
         }
         else
         {
            strResult += "0";
         }
      }
      return strResult;

   }

   int64_t get_number_value(string strName)
   {
      string strResult;
      string strMask = get_number_mask(strName);
      index i;
      for(i= strMask.get_length() - 1; i >= 0 ; i--)
      {
         if(strMask[i] == '1' || strMask[i] == 'X')
         {
            strResult = strName[i] + strResult;
         }
         if(strMask[i] == '1')
         {
            break;
         }
      }
      return atoi64_dup(strResult);

   }


   string set_number_value(string strName, int64_t iValue)
   {
      string strValue = ::str::from(iValue);
      string strResult = strName;
      string strMask = get_number_mask(strName);
      int j = strValue.get_length() - 1;
      index i;
      for(i= strMask.get_length()-1; i >= 0 ; i--)
      {
         if(strMask[i] == 'X' || strMask[i] == '1')
         {
            if(j >= 0)
            {
               strResult.set_at(i,strValue[j]);
               j--;
            }
            else
            {
               strResult.set_at(i, '0');
            }
            if(strMask[i] == '1')
            {
               break;
            }
         }
      }
      while(j >= 0)
      {
         strValue.Insert(i,strValue[j]);
         j--;
      }
      return strResult;

   }

   bool operation::make_duplicate_name(::file::path & str,const ::file::path & psz)
   {

      string strDir = psz;
      string strName = str.Mid(strDir.get_length());
      string strExtension;
      bool bDir;
      strsize iFind = strName.find("\\");
      bDir = iFind > 0;
      if(bDir)
      {
         strExtension = "\\" + strName.Mid(iFind + 1);
         strName = strName.Left(iFind);
      }
      else
      {
         strName = str.title();
         strExtension = "." + str.ext();
      }


      if(has_digit(strName))
      {
         int64_t iValue = get_number_value(strName);
         string strFormat;
         for(int32_t i = 1; i < 1000; i++)
         {
            strFormat = set_number_value(strName, iValue + i);
            str = strDir /strFormat + strExtension;
            if(!Application.file().exists(str))
               return true;
         }
      }
      else
      {

         string strFormat;
         for(int32_t i = 1; i < 1000; i++)
         {
            strFormat.Format("-Copy-%03d",i);
            str = strDir /strName + strFormat + strExtension;
            if(!Application.file().exists(str))
               return true;
         }
      }
      return false;
   }


   void operation::expand(::file::listing & straExpanded,::file::patha & straExpand)
   {

      straExpanded.m_pprovider = get_app();

      for(int32_t i = 0; i < straExpand.get_size(); i++)
      {

         if(Application.dir().is(straExpand[i]) && !::str::ends_ci(m_stra[i],".zip"))
         {

            straExpanded.rls(straExpand[i]);

         }
         else
         {

            straExpanded.add(straExpand[i]);

         }

      }

   }


} // namespace filemanager























