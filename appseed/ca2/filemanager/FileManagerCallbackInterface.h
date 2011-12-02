#pragma once

#ifdef _WINDOWS
#include <shlobj.h>
#endif


namespace filemanager
{

   class data;

} // namespace filemanager



class CLASS_DECL_ca FileManagerCallbackInterface :
   virtual public file_manager_interface,
   virtual public ex1::request_interface
{
public:


   FileManagerCallbackInterface();
   virtual ~FileManagerCallbackInterface();


   virtual void on_request(::ax::create_context * pcreatecontext);


   virtual void OnFileManagerOpenContextMenuFolder(::filemanager::data * pdata, const ::fs::item & item, stringa & straCommand, stringa & straCommandTitle);
   virtual void OnFileManagerOpenContextMenuFile(::filemanager::data * pdata, const ::fs::item_array & itema);
   virtual void OnFileManagerOpenContextMenu(::filemanager::data * pdata);
   virtual void OnFileManagerOpenFolder(::filemanager::data * pdata, const ::fs::item & item);
   virtual bool GetFileManagerItemCallback(::filemanager::data * pdata, id id, const ::fs::item_array & itema);
   virtual void OnFileManagerItemUpdate(::filemanager::data * pdata, cmd_ui * pcmdui, const ::fs::item_array & itema);
   virtual void OnFileManagerItemCommand(::filemanager::data * pdata, id pszId, const ::fs::item_array & itema);
   virtual void OnFileManagerInitializeFormPreData(::filemanager::data * pdata, id uiId, ::user::form * pform);


};


