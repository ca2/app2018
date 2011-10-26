#include "StdAfx.h"


FileManagerInterface::FileManagerInterface()
{
   m_item.m_strPath.Empty();
}

FileManagerInterface::~FileManagerInterface()
{

}

bool FileManagerInterface::FileManagerBrowse(const ::fs::item & item)
{
   if(&item != &m_item)
   {
      m_item.m_strPath        = item.m_strPath;
   }
   OnFileManagerBrowse();
   return false;
}

bool FileManagerInterface::FileManagerBrowse(const char * lpcsz)
{
   ::fs::item item;
   item.m_strPath          = lpcsz;
   FileManagerBrowse(item);
   return false;
}

void FileManagerInterface::FileManagerOneLevelUp()
{

   if(get_item().m_strPath.is_empty())
      return;

   string strParent = get_fs_data()->eat_end_level(get_item().m_strPath, 1);

   FileManagerBrowse(strParent);

}

void FileManagerInterface::GetActiveViewSelection(::fs::item_array & itema)
{
   UNREFERENCED_PARAMETER(itema);
}


::fs::item & FileManagerInterface::get_item()
{
   return m_item;
}

critical_section * FileManagerInterface::GetItemIdListCriticalSection()
{
   return &m_csItemIdListAbsolute;
}

void FileManagerInterface::OnFileManagerBrowse()
{
   data_set("InitialBrowsePath", ::radix::system::idEmpty, get_item().m_strPath);
   //get_filemanager_data()->OnFileManagerOpenFolder(get_item());
}

void FileManagerInterface::OpenSelectionProperties()
{
}

void FileManagerInterface::FileManagerSaveAs(document * pdocument)
{
   get_filemanager_data()->m_pdocumentSave = pdocument;
}

void FileManagerInterface::FileManagerSaveOK()
{
}

void FileManagerInterface::FileManagerSaveCancel()
{
}

::fs::data * FileManagerInterface::get_fs_data()
{
   return NULL;
}