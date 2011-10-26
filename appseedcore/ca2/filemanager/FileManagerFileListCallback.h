#pragma once

class ::userbase::button;
class ::fs::item;
class ::fs::item_array;

class CLASS_DECL_ca FileManagerFileListCallback :
   virtual public ::radix::object
{
public:
   
   image_list *           m_pimagelistSubItemHover;
   image_list *           m_pimagelistItemHover;
   image_list *           m_pimagelistNormal;

   FileManagerFileListCallback();
   virtual ~FileManagerFileListCallback();

   virtual bool initialize();

   virtual int GetActionButtonCount();
   virtual image_list * GetActionButtonImageList(id i);
   virtual void InitializeActionButton(id i, BaseButtonControl * pbutton);
   virtual void OnButtonAction(id i, const ::fs::item & item);
   
   virtual bool GetMenuItemCallback(id iId);
   virtual void OnMenuItemAction(id iId, const ::fs::item_array & itema);
   virtual void GetMenuItemUpdate(id iId, const ::fs::item_array & itema, cmd_ui * pcmdui);

};
