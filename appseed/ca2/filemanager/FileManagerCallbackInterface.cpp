#include "StdAfx.h"

FileManagerCallbackInterface::FileManagerCallbackInterface()
{

}

FileManagerCallbackInterface::~FileManagerCallbackInterface()
{

}

void FileManagerCallbackInterface::OnFileManagerOpenContextMenuFolder(::filemanager::data * pdata, const ::fs::item & item, stringa & straCommand, stringa & straCommandTitle)
{
   UNREFERENCED_PARAMETER(item);
   if(pdata->m_pmanager != NULL)
   {
      pdata->m_pmanager->OpenSelectionProperties();
   }
}

void FileManagerCallbackInterface::OnFileManagerOpenContextMenuFile(::filemanager::data * pdata, const ::fs::item_array & itema)
{
   UNREFERENCED_PARAMETER(itema);
   if(pdata->m_pmanager != NULL)
   {
      pdata->m_pmanager->OpenSelectionProperties();
   }
}

void FileManagerCallbackInterface::OnFileManagerOpenContextMenu(::filemanager::data * pdata)
{
   UNREFERENCED_PARAMETER(pdata);
}

void FileManagerCallbackInterface::on_request(::ax::create_context * pcreatecontext)
{
   ::ax::ax * pca = dynamic_cast < ::ax::ax * > (this);
   if(pca != NULL)
   {
      Bergedge.request(pcreatecontext);
   }
}

void FileManagerCallbackInterface::OnFileManagerOpenFolder(::filemanager::data * pdata, const ::fs::item & item)
{
   UNREFERENCED_PARAMETER(pdata);
   UNREFERENCED_PARAMETER(item);
}

void FileManagerCallbackInterface::OnFileManagerItemCommand(::filemanager::data * pdata, id id, const ::fs::item_array & itema)
{
   UNREFERENCED_PARAMETER(pdata);
   UNREFERENCED_PARAMETER(id);
   UNREFERENCED_PARAMETER(itema);
}

void FileManagerCallbackInterface::OnFileManagerItemUpdate(::filemanager::data * pdata, cmd_ui * pcmdui, const ::fs::item_array & itema)
{
   UNREFERENCED_PARAMETER(pdata);
   UNREFERENCED_PARAMETER(pcmdui);
   UNREFERENCED_PARAMETER(itema);
}


void FileManagerCallbackInterface::OnFileManagerInitializeFormPreData(::filemanager::data * pdata, id id, ::user::form * pform)
{
   UNREFERENCED_PARAMETER(pdata);
   UNREFERENCED_PARAMETER(id);
   UNREFERENCED_PARAMETER(pform);
}


bool FileManagerCallbackInterface::GetFileManagerItemCallback(::filemanager::data * pdata, id id, const ::fs::item_array & itema)
{
   UNREFERENCED_PARAMETER(pdata);
   UNREFERENCED_PARAMETER(id);
   UNREFERENCED_PARAMETER(itema);
   return false;
}


