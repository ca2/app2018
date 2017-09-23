﻿#include "framework.h"


namespace userfs
{


   tree::tree(::aura::application * papp) :
      object(papp),
      ::data::data(papp),
      ::data::tree(papp),
      ::user::tree_data(papp)
   {

      m_iAnimate = 0;

      //m_iDefaultImage = -1;
      //m_iDefaultImageSelected = -1;

   }

   tree::~tree()
   {
   }


#ifdef DEBUG
   void tree::assert_valid() const
   {
   }

   void tree::dump(dump_context &) const
   {
   }
#endif //DEBUG


   void tree::install_message_routing(::message::sender * pinterface)
   {

      IGUI_MSG_LINK(WM_CREATE, pinterface, this, &tree::_001OnCreate);
      IGUI_MSG_LINK(WM_LBUTTONDBLCLK, pinterface, this, &tree::_001OnLButtonDblClk);
      IGUI_MSG_LINK(WM_CONTEXTMENU, pinterface, this, &tree::_001OnContextMenu);

   }

   void tree::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
   {
      UNREFERENCED_PARAMETER(pSender);
      UNREFERENCED_PARAMETER(lHint);

      //::filemanager::data_interface::on_update(pSender, lHint, phint);
      if (phint != NULL)
      {
         if (base_class < filemanager::update_hint > ::bases(phint))
         {
            filemanager::update_hint * puh = (filemanager::update_hint *)phint;
            if (puh->is_type_of(filemanager::update_hint::TypeInitialize))
            {
               //if (get_filemanager_template()->get_filemanager_data()->m_bTransparentBackground)
               //{
               //   ::user::tree::m_etranslucency = ::user::tree::translucency_present;
               //}
               /* xxx _001SetExpandImage(
                  System.LoadIcon(
                  get_filemanager_template()->get_filemanager_template()->m_uiExpandBox));
                  _001SetCollapseImage(
                  System.LoadIcon(
                  get_filemanager_template()->get_filemanager_template()->m_uiCollapseBox));*/


               //            VmsDataInitialize(this);
               //          SetDataInterface(&m_datainterface);
               //        AddClient(&m_datainterface);
               /*               string str;
                              str.Format("tree(%s)", get_filemanager_template()->get_filemanager_data()->m_strDISection);
                              m_dataid = str;*/
               //            _001UpdateColumns();
            }
            if (puh->is_type_of(filemanager::update_hint::TypeFilter))
            {
               if (puh->m_wstrFilter.is_empty())
               {
                  //               FilterClose();
               }
               else
               {
                  //             FilterBegin();
                  //           Filter1(puh->m_wstrFilter);
                  //         FilterApply();
               }
            }
         }
      }
   }


   void tree::_001OnLButtonDblClk(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //   int32_t iItem;

      /*   if(_001HitTest_(point, iItem))
         {
         if(m_itema.get_item(iItem).IsFolder())
         {
         ::fs::item item;
         item.m_strPath         = m_itema.get_item(iItem).m_strPath;
         item.m_lpiidlAbsolute   = m_itema.get_item(iItem).m_lpiidlAbsolute;
         item.m_lpiidlRelative   = m_itema.get_item(iItem).m_lpiidlAbsolute;
         get_document()->OpenFolder(item);
         }
         else
         {
         ::fs::item item;
         item.m_strPath         = m_itema.get_item(iItem).m_strPath;
         item.m_lpiidlAbsolute   = m_itema.get_item(iItem).m_lpiidlAbsolute;
         item.m_lpiidlRelative   = m_itema.get_item(iItem).m_lpiidlAbsolute;

         ::fs::item_array itema;
         itema.add(item);

         get_filemanager_template()->get_filemanager_data()->OnFileManagerOpenFile(itema);
         }
         }*/
   }

   /*
   bool tree::OnSetData(const ::database::id &key, int32_t iLine, int32_t iColumn, var & var, ::database::update_hint * puh)
   {
   if(key.get_value() == FILE_MANAGER_ID_FILE_NAME)
   {
   ASSERT(var.get_type() == var::type_string
   || var.is_empty());
   string str;
   str = var.m_str;
   RenameFile(iLine, str);
   }
   return true;
   }
   */

   /*
   bool tree::get_data(const ::database::id & key, int32_t iLine, int32_t iColumn, var & var)
   {
   string str;
   if(key.get_value() == FILE_MANAGER_ID_FILE_NAME)
   {
   str = m_itema.get_item(iLine).m_wstrName;
   }
   var.set_type(var::type_string);
   var.m_str = str;
   return true;
   }

   void tree::RenameFile(int32_t iLine, string &wstrNameNew)
   {
   string str = m_itema.get_item(iLine).m_strPath;

   int32_t iFind = str.reverse_find(L'\\');


   string wstrNew = str.Left(iFind + 1) + wstrNameNew;

   if(!::win::shell::MoveFile(str, wstrNew))
   {
   System.simple_message_box("Could not rename the file");
   }

   }*/

   void tree::_001OnContextMenu(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);
//      SCAST_PTR(::message::context_menu, pcontextmenu, pobj);
         //   int32_t iItem;
         //   HRESULT hr;
//         point ptClient = pcontextmenu->GetPoint();
//      ::user::tree::ScreenToClient(&ptClient);
      /*     if(_001HitTest_(ptClient, iItem))
         {
         CSimpleMenu menu(CBaseMenuCentral::GetMenuCentral());
         if (menu.LoadMenu(get_filemanager_template()->get_filemanager_template()->m_uiFilePopup))
         {
         CSimpleMenu* pPopup = (CSimpleMenu *) menu.GetSubMenu(0);
         ASSERT(pPopup != NULL);
         sp(::user::frame_window) pframe = GetTopLevelFrame();

         pframe->SetActiveView(this);

         //IContextMenu * pcontextmenu;

         hr = m_spshellfolder->GetUIObjectOf(
         NULL,
         1,
         (LPCITEMIDLIST *) &m_itema.get_item(iItem).m_lpiidlRelative,
         IID_IContextMenu,
         NULL,
         (void **) &m_contextmenu.m_pcontextmenu);


         if(SUCCEEDED(hr))
         {
         hr = m_contextmenu.m_pcontextmenu->QueryContextMenu(
         pPopup->GetSafeHmenu(),
         0,
         SHELL_COMMAND_FIRST,
         SHELL_COMMAND_LAST,
         CMF_NORMAL);

         }


         pPopup->track_popup_menu(
         point.x, point.y,
         (::window_sp) pframe);
         }
         }
         else
         {
         ::user::menu menu;
         if (menu.LoadMenu(get_filemanager_template()->get_filemanager_template()->m_uiPopup))
         {
         ::user::menu* pPopup = menu.GetSubMenu(0);
         ASSERT(pPopup != NULL);
         sp(::user::frame_window) pframe = GetTopLevelFrame();
         pPopup->track_popup_menu(
         point.x, point.y,
         (::window_sp) pframe);
         }
         }*/
   }



   void tree::_001OnTimer(::timer * ptimer)
   {

      if (ptimer->m_nIDEvent == 1234567)
      {
         m_iAnimate += 2;
         if (m_iAnimate >= 11)
         {
            m_iAnimate = 0;
            dynamic_cast < ::user::interaction * > (ptimer->m_pcallback)->KillTimer(ptimer->m_nIDEvent);

         }
         dynamic_cast < ::user::interaction * > (ptimer->m_pcallback)->RedrawWindow();
      }
      else if (ptimer->m_nIDEvent == 123)
      {
         dynamic_cast < ::user::interaction * > (ptimer->m_pcallback)->RedrawWindow();
         m_bTimer123 = false;
         dynamic_cast < ::user::interaction * > (ptimer->m_pcallback)->KillTimer(123);
      }
   }

   void tree::StartAnimation(::user::interaction * pui)
   {
      m_iAnimate = 1;
      pui->SetTimer(1234567, 50, NULL);
   }

   void tree::_001OnShellCommand(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

   }


   void tree::_017OpenFolder(::fs::item * item, ::action::context actioncontext)
   {

      get_document()->browse(item->m_filepath, actioncontext);



   }


   void tree::_001OnCreate(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

   }




   void tree::update_list()
   {

   }


   void tree::browse_sync(::action::context actioncontext)
   {


   }


   void tree::_017EnsureVisible(const ::file::path & path, ::action::context actioncontext)
   {

   }


   ::data::tree_item * tree::find_item(const::file::path & path, bool bPathFromItemFromOwnTree, ::data::tree_item * pitemStart)
   {

      return find_absolute(path, bPathFromItemFromOwnTree, pitemStart);

   }




   void tree::_001InsertColumns()
   {

      /*::user::form_control control;

      control.set_type(::user::form_control_type_edit);
      control.m_iKey = FILE_MANAGER_ID_FILE_NAME;
      pcontrol->descriptor().m_id = FILE_MANAGER_ID_FILE_NAME;
      control.set_data_type(::user::form_control_data_type_string);

      int32_t iControl =  _001AddControl(control);

      CColumn column;

      column.m_iWidth               = 500;
      column.m_iSubItem             = 1;
      column.m_bIcon                = true;
      column.m_sizeIcon.cx          = 16;
      column.m_sizeIcon.cy          = 16;
      column.m_iControl             = iControl;
      column.m_iConfigId            = FILE_MANAGER_ID_FILE_NAME;
      column.m_bEditOnSecondClick   = true;
      _001AddColumn(column);*/

   }

   void tree::GetSelectedFilePath(stringa & stra)
   {

      ::data::tree_item_ptr_array itemptraSelected;

      get_selection(itemptraSelected);

      for (int32_t i = 0; i < itemptraSelected.get_size(); i++)
      {

         stra.add(itemptraSelected[0]->m_pitem.cast < ::userfs::item >()->m_filepath);

      }

   }


   void tree::TakeAnimationSnapshot()
   {
      m_iAnimate = 1;
      //   ::user::tree::_001OnDraw(m_gdibuffer.GetBuffer());
   }


   /*   IShellFolder * tree::_001GetFolder(EFolder efolder)
   {
   IShellFolder * psf;

   if(m_mapFolder.Lookup(efolder, psf))
   {
   return psf;
   }
   else
   {
   int32_t iCSIDL = MapToCSIDL(efolder);

   ASSERT(iCSIDL >= 0);

   if(iCSIDL < 0)
   return NULL;

   IShellFolder * psfDesktop = NULL;
   HRESULT hr = SHGetDesktopFolder(&psfDesktop);
   LPITEMIDLIST lpidl;

   hr = SHGetSpecialFolderLocation(
   NULL,
   iCSIDL,
   &lpidl);

   if(FAILED(hr))
   return NULL;

   hr = psfDesktop->BindToObject(
   lpidl,
   NULL,
   IID_IShellFolder,
   (void **) &psf);

   if(FAILED(hr))
   return NULL;

   m_mapFolder.set_at(efolder, psf);

   psf->AddRef();
   psfDesktop->Release();
   return psf;
   }
   }*/

   /*   int32_t tree::MapToCSIDL(EFolder efolder)
   {
   switch(efolder)
   {
   case FolderMyComputer:
   return CSIDL_DRIVES;
   default:
   return -1;
   }
   }
   */

   void tree::_001OnItemExpand(::data::tree_item * pitem, ::action::context actioncontext)
   {
   }

   void tree::_001OnItemCollapse(::data::tree_item * pitem, ::action::context actioncontext)
   {
      UNREFERENCED_PARAMETER(pitem);
   }

   bool tree::_001IsTranslucent()
   {
      return true;
   }

   void tree::_001OnOpenItem(::data::tree_item * pitem, ::action::context actioncontext)
   {

      _017OpenFolder(canew(::fs::item(*pitem->m_pitem.cast < ::userfs::item >())), actioncontext);

   }





   COLORREF tree::get_background_color()
   {
      if (get_document() == NULL)
      {
         return RGB(200, 255, 255);
      }
      /*      else if(get_filemanager_template()->get_filemanager_data()->is_saving())
      {
      return RGB(255, 180, 90);
      }*/
      else
      {
         return RGB(200, 255, 255);
      }
   }

   ::data::tree_item * tree::find_absolute(const ::file::path & path, bool bPointerFromPathFromItemFromOwnTree, ::data::tree_item * pitemStart)
   {

      ::data::tree_item * pitem;

      if(pitemStart == NULL)
         pitem = get_base_item();
      else
         pitem = pitemStart;

      if (path.get_length() <= 0)
         return NULL;

      while (pitem != NULL)
      {

         if (pitem->m_pitem != NULL)
         {

            ::userfs::item * p = pitem->m_pitem.cast < ::userfs::item >();

            if (p != NULL)
            {

               ::file::path & pathTreeItem = p->m_filepath;

               if (bPointerFromPathFromItemFromOwnTree)
               {

                  if (&pathTreeItem == &path)
                  {

                     return pitem;

                  }

               }
               else
               {

                  if (pathTreeItem == path)
                  {

                     return pitem;

                  }

               }

            }

         }

         pitem = pitem->get_child_next_or_parent();

         if(pitem == pitemStart)
            break;

      }

      return NULL;

   }

   void tree::arrange(::fs::e_arrange earrange)
   {

      //if (earrange == ::fs::arrange_by_name)
      {
         //sort(item::CompareArrangeByName);
      }

   }

   ::userfs::document * tree::get_document()
   {

      return  dynamic_cast < ::userfs::document * > (::data::data::get_data_container_base());

   }



} // namespace userfs



