#include "framework.h"


file_operation::file_operation(sp(base_application) papp) :
   element(papp),
   m_fileSrc(allocer()),
   m_fileDst(allocer())
{

   m_oswindowCallback = NULL;

   m_bReplaceAll = false;

   m_eoperation = filemanager::operation_none;

   m_iBufferSize = 1024 * 1024;

}


filemanager::eoperation file_operation::operation()
{
   return m_eoperation;
}

void file_operation::set_operation(filemanager::eoperation eoperation)
{
   m_eoperation = eoperation;
}

bool file_operation::set_copy(stringa & stra, const char * pszDestBase, const char * pszSrcBase, bool bExpand)
{
   set_operation(filemanager::operation_copy);
   if(bExpand)
   {
      expand(m_stra, stra);
   }
   else
   {
      m_stra = stra;
   }
   m_str = System.dir().path(pszDestBase, "");
   string strBase;
   if(pszSrcBase != NULL)
   {
      strBase = System.dir().path(pszSrcBase, "");
   }
   else
   {
      strBase = System.dir().name(stra[0]);
      string strCompare;
      for(int32_t i = 1; i < stra.get_size(); i++)
      {
         strCompare = System.dir().name(stra[i]);
         for(int32_t j = 0; j < min(strCompare.get_length(), strBase.get_length()); j++)
         {
            if(strCompare[j] != strBase[j])
            {
               strBase = System.dir().name(strCompare.Left(j));
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

bool file_operation::set_move(stringa & stra, const char * psz)
{
   set_operation(filemanager::operation_move);
   m_stra = stra;
   m_str = psz;
   m_dRead = 0.0;
   m_iFile = 0;
   if(!initialize())
      return false;
   return true;
}

bool file_operation::set_delete(stringa & stra)
{
   set_operation(filemanager::operation_delete);
   m_stra = stra;
   m_dRead = 0.0;
   m_iFile = 0;
   if(!initialize())
      return false;
   return true;
}

bool file_operation::open_src_dst(const char * pszSrc, const char * pszDst)
{
   if(Application.dir().is(pszSrc) && !::str::ends_ci(pszSrc, ".zip"))
   {
      Application.dir().mk(System.dir().name(pszDst));
      return false;
   }
   if(!m_fileSrc->open(pszSrc, ::file::mode_read | ::file::type_binary | ::file::binary_buffer::shareDenyWrite))
   {
      TRACE("\n Could not open source file(%d)=%s", m_iFile, pszSrc);
      return false;
   }
   if(!m_bReplaceAll)
   {
/*      if(System.file().exists(pszDst))
      {
         property_set propertyset;
         propertyset["srcfile"].get_value().set_string(pszSrc);
         propertyset["dstfile"].get_value().set_string(pszDst);
         System.message_box("filemanager\\do_you_want_to_replace_the_file.xml", propertyset);
         return false;
      }*/
   }
   Application.dir().mk(System.dir().name(pszDst));
   if(!m_fileDst->open(pszDst, ::file::binary_buffer::mode_write | ::file::type_binary | ::file::binary_buffer::mode_create))
   {
      TRACE("\n Could not open dest file(%d)=%s", m_iFile, pszDst);
      property_set propertyset;
      propertyset["filepath"] = pszDst;
      System.message_box("filemanager\\not_accessible_destination_file.xhtml", propertyset);
      return false;
   }

   TRACE("\n%d Opened %s %s",  m_iFile, pszSrc, pszDst);

   return true;
}


bool file_operation::start()
{
   switch(m_eoperation)
   {
   case filemanager::operation_copy:
      {
         m_iFile = 0;
         m_pchBuffer = (char *) malloc(m_iBufferSize);
         string strName = System.dir().path(
            m_str, m_stra[m_iFile].Mid(m_strBase.get_length()));
         if(m_str == m_strBase)
         {
            make_duplicate_name(strName, m_str);
         }
         if(!open_src_dst(m_stra[m_iFile], strName))
            return false;
      }
      break;
   case filemanager::operation_delete:
      {
      }
      break;
   case filemanager::operation_move:
      {
         m_iFile = 0;
         m_pchBuffer = (char *) malloc(m_iBufferSize);
         if(!open_src_dst(m_stra[m_iFile],
            System.dir().path(
            m_str, System.file().name_(m_stra[m_iFile]))))
            return false;
      }
      break;
      default:
         
         break;
   }
   return true;
}

bool file_operation::step()
{
   switch(m_eoperation)
   {
   case filemanager::operation_copy:
      {
         if(m_iFile >= m_stra.get_size())
            return false;
         primitive::memory_size uiRead = m_fileSrc->read(m_pchBuffer, m_iBufferSize);
         if(uiRead > 0)
         {
            m_fileDst->write(m_pchBuffer, uiRead);
            m_daRead[m_iFile] += uiRead;
            m_dRead += uiRead;
         }
         else
         {
            {
               string strDestPath = m_fileDst->GetFilePath();
               m_fileDst->close();
               ::core::file_status st;
               m_fileSrc->GetStatus(st);
               System.os().set_file_status(strDestPath, st);
               m_fileSrc->close();
            }
            m_iFile++;
            while(m_iFile < m_stra.get_size() && Application.dir().is(m_stra[m_iFile]) && !::str::ends_ci(m_stra[m_iFile], ".zip"))
            {
               m_iFile++;
            }
            if(m_iFile >= m_stra.get_size())
               return false;
            string strName = System.dir().path(
               m_str, m_stra[m_iFile].Mid(m_strBase.get_length()));
            if(m_str == m_strBase)
            {
               make_duplicate_name(strName, m_str);
            }
            if(!open_src_dst(m_stra[m_iFile], strName))
            {

               return false;
            }
         }
         return true;
      }
   case filemanager::operation_delete:
      {
         if(m_iFile >= m_stra.get_size())
            return false;
        System.file().del(m_stra[m_iFile]);
         m_iFile++;
      }
      break;
   case filemanager::operation_move:
      {
         if(m_iFile >= m_stra.get_size())
            return false;
         primitive::memory_size uiRead = m_fileSrc->read(m_pchBuffer, m_iBufferSize);
         m_fileDst->write(m_pchBuffer, uiRead);
         m_daRead[m_iFile] += uiRead;
         m_dRead += uiRead;
         if(uiRead == 0)
         {
            m_fileSrc->close();
            m_fileDst->close();
            System.file().del(m_stra[m_iFile]);
            m_iFile++;
            if(m_iFile >= m_stra.get_size())
               return false;
            if(!open_src_dst(m_stra[m_iFile],
               System.dir().path(
               m_str, System.file().name_(m_stra[m_iFile]))))
               return false;
         }
      }
      break;
      default:
         
         break;
   }
   return true;
}

bool file_operation::finish()
{
   switch(m_eoperation)
   {
   case filemanager::operation_copy:
      {
         free(m_pchBuffer);
      }
      break;
   case filemanager::operation_delete:
      {
      }
      break;
   case filemanager::operation_move:
      {
         free(m_pchBuffer);
      }
      break;
      default:
         
         break;
   }
   if(m_oswindowCallback != NULL)
   {
      m_oswindowCallback->send_message(m_uiCallbackMessage, m_wparamCallback);
   }
   return true;
}


bool file_operation::initialize()
{
   m_dSize = 0.0;
   var varLen;
   for(int32_t i = 0; i < m_stra.get_size(); i++)
   {
      if(Application.dir().is(m_stra[i]) && !::str::ends_ci(m_stra[i], ".zip"))
      {
         m_daSize.add(0.0);
         m_daRead.add(0.0);
      }
      else
      {
         varLen = System.file().length(m_stra[i]);
         if(varLen.is_null())
            return false;
         m_dSize += (uint32_t) varLen;
         m_daSize.add((double) (uint32_t) varLen);
         m_daRead.add(0.0);
      }
   }
   return true;
}

int32_t file_operation::get_item_count()
{
   return (int32_t) m_stra.get_size();
}

string file_operation::get_item_message(int32_t iItem)
{
   string str;
   str.Format("Copying %s (%s) to %s", System.file().name_(m_stra[iItem]),
      System.dir().name(m_stra[iItem]), m_str);
   return str;
}

double file_operation::get_item_progress(int32_t iItem)
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

double file_operation::get_item_read(int32_t iItem)
{
   return m_daRead[iItem];
}

double file_operation::get_item_size(int32_t iItem)
{
   return m_daSize[iItem];
}

void file_operation::make_duplicate_name(string & str, const char * psz)
{
   string strDir = System.dir().path(psz, "");
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
      strName = System.file().title_(str);
      strExtension = "." + System.file().extension(str);
   }
   string strFormat;
   for(int32_t i = 1; i < 1000; i++)
   {
      strFormat.Format("-Copy-%03d", i);
      str = System.dir().path(strDir, strName + strFormat + strExtension);
      if(!Application.file().exists(str))
         return;
   }
}

void file_operation::expand(stringa & straExpanded, stringa & straExpand)
{
   for(int32_t i = 0; i < straExpand.get_size(); i++)
   {
      if(Application.dir().is(straExpand[i]) && !::str::ends_ci(m_stra[i], ".zip"))
      {
         Application.dir().rls(straExpand[i], &straExpanded);
      }
      else
      {
         straExpanded.add(straExpand[i]);
      }
   }
}
