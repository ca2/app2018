#include "framework.h"


namespace userfs
{


   list::list(::aura::application * papp) :
      object(papp)
   {

      m_iSelectionSubItem = -1;
      m_iNameSubItem = -1;
      m_iIconSubItem = -1;

      m_rectMargin.left = 0;
      m_rectMargin.top = 0;
      m_rectMargin.bottom = 0;
      m_rectMargin.right = 0;

      create_int(::user::int_list_item_draw_text_flags, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_PATH_ELLIPSIS);

   }


   list::~list()
   {

   }


   void list::install_message_routing(::message::sender * pinterface)
   {

      ::user::form_list_view::install_message_routing(pinterface);
      IGUI_MSG_LINK(WM_HSCROLL, pinterface, this, &list::_001OnHScroll);
      IGUI_MSG_LINK(WM_VSCROLL, pinterface, this, &list::_001OnVScroll);
      IGUI_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &list::_001OnShowWindow);
      IGUI_MSG_LINK(WM_CREATE, pinterface, this, &list::_001OnCreate);
      IGUI_MSG_LINK(WM_LBUTTONDBLCLK, pinterface, this, &list::_001OnLButtonDblClk);
      IGUI_MSG_LINK(WM_CANCELMODE, pinterface, this, &list::_001OnCancelMode);

   }


   void list::_001OnCreate(::message::message * pobj)
   {

      pobj->previous();

      if(pobj->m_bRet)
         return;

      Session.userex()->shell()->on_update_sizes_interest();

   }


   void list::_001OnTimer(::timer * ptimer)
   {
//      UNREFERENCED_PARAMETER(pobj);

      ::user::form_list_view::_001OnTimer(ptimer);

   }


   bool list::_001OnClick(uint_ptr uiFlags, point point)
   {
      UNREFERENCED_PARAMETER(uiFlags);
      UNREFERENCED_PARAMETER(point);
      range range;
      _001GetSelection(range);
      if(range.get_item_count() > 0)
      {
         list_data * pdata = get_fs_mesh_data();
         item_range & itemrange = range.ItemAt(0);
         index iLItem = itemrange.get_lower_bound();
         index iUItem = itemrange.get_upper_bound();
         if(iUItem < iLItem)
         {
            iUItem = pdata->m_itema.get_upper_bound();
         }
         if(iLItem < 0)
            return false;
         if(iLItem >= pdata->m_itema.get_size())
            return false;
         if(iUItem >= pdata->m_itema.get_size())
            iUItem = pdata->m_itema.get_upper_bound();
         var varFile;
         var varQuery;
         if(iUItem == iLItem)
         {
            varFile = get_fs_mesh_data()->m_itema.get_item(iLItem).get_friendly_path();
         }
         else
         {
            stringa stra;
            for(index iItem = iLItem; iItem < iLItem; iItem++)
            {
               stra.add(pdata->m_itema.get_item(iItem).get_friendly_path());
            }
            varFile = stra;
         }
         get_document()->request_file_query(varFile, varQuery);
      }
      return true;
   }



   void list::assert_valid() const
   {
      ::user::form_list_view::assert_valid();
   }

   void list::dump(dump_context & dumpcontext) const
   {
      ::user::form_list_view::dump(dumpcontext);
   }


   void list::on_update(::user::impact * pSender, LPARAM lHint, object * phint)
   {


   }

   void list::_001OnLButtonDblClk(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
//      SCAST_PTR(::message::mouse, pmouse, pobj);
      /*         index iItem;
            list_data * pdata = get_fs_mesh_data();
            if(_001HitTest_(pmouse->m_pt, iItem))
            {
               ::fs::item item;
               item.m_strPath         = pdata->m_foldera.GetFolder(iItem).m_strPath;
               get_filemanager_template()->get_filemanager_data()->OnFileManagerOpenFolder(item);
            }*/
   }

   void list::_001OnCancelMode(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      // trans   ::user::impact::OnCancelMode();

      // TODO: add your message handler code here

   }

   bool list::pre_create_window(::user::create_struct& cs)
   {

      cs.style |= WS_CLIPCHILDREN;

      return ::user::form_list_view::pre_create_window(cs);
   }

   void list::_001InsertColumns()
   {
      ::user::list_column column;
      column.m_iWidth = 200;
      column.m_iSubItem = 0;

      _001InsertColumn(column);
      column.m_iWidth = 300;
      column.m_iSubItem = 1;

      _001InsertColumn(column);
      column.m_iWidth = 100;
      column.m_iSubItem = 2;

      _001InsertColumn(column);
   }


   bool list::_001OnUpdateItemCount(uint32_t dwFlags)
   {
      return ::user::list::_001OnUpdateItemCount(dwFlags);
   }

   void list::GetSelectedFilePath(stringa & array)
   {
      range range;

      _001GetSelection(range);

      list_data * pdata = get_fs_mesh_data();
      string str;
      //      HRESULT hr;
      for (index i = 0; i < range.get_item_count(); i++)
      {
         item_range & itemrange = range.ItemAt(i);
         for (index iItem = itemrange.get_lower_bound(); iItem <= itemrange.get_upper_bound(); iItem++)
         {
            index iStrict;
            if (m_eview == view_icon)
            {
               iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
            }
            else
            {
               iStrict = m_meshlayout.m_iaDisplayToStrict[iItem];
            }
            list_item & item = pdata->m_itema.get_item(iStrict);
            if (!item.IsFolder())
            {
               array.add(item.get_friendly_path());
            }
         }
      }

   }


   void list::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      ::user::form_list_view::_001OnDraw(pgraphics);

   }


   void list::StartAnimation()
   {
      //         m_iAnimate = 1;
   }

   void list::TakeAnimationSnapshot()
   {
      //       m_iAnimate = 1;
      //     ::user::list::_001OnDraw(m_gdibuffer.GetBuffer());
   }


   void list::_017OpenSelected(bool bOpenFile, ::action::context actioncontext)
   {
      list_data * pdata = get_fs_mesh_data();
      ::fs::item_array itema;
      index iItemRange, iItem;
      range range;
      _001GetSelection(range);
      for (iItemRange = 0;
            iItemRange < range.get_item_count();
            iItemRange++)
      {
         item_range itemrange = range.ItemAt(iItemRange);
         for (iItem = itemrange.get_lower_bound();
               iItem <= itemrange.get_upper_bound();
               iItem++)
         {
            if (iItem < 0)
               continue;
            if (iItem >= _001GetItemCount())
               continue;
            index iStrict;
            if (m_eview == view_icon)
            {
               iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
            }
            else
            {
               iStrict = m_meshlayout.m_iaDisplayToStrict[iItem];
            }
            list_item & item = pdata->m_itema.get_item(iStrict);
            sp(::fs::item) pitem(canew(::fs::item(item)));
            if (item.IsFolder())
            {
               _017OpenFolder(pitem, ::action::source::sel(actioncontext));
               break;
            }
            else
            {
               itema.add(pitem);
            }
         }
      }
      if (bOpenFile && itema.get_size() > 0)
      {
         _017OpenFile(itema, ::action::source::sel(actioncontext));
      }
      _001ClearSelection();
   }

   void list::_017OpenContextMenuSelected(::action::context actioncontext)
   {
      list_data * pdata = get_fs_mesh_data();
      ::fs::item_array itema;
      index iItemRange, iItem;
      range range;
      _001GetSelection(range);
      for (iItemRange = 0;
            iItemRange < range.get_item_count();
            iItemRange++)
      {
         item_range itemrange = range.ItemAt(iItemRange);
         for (iItem = itemrange.get_lower_bound();
               iItem <= itemrange.get_upper_bound();
               iItem++)
         {
            if (iItem < 0)
               continue;
            if (iItem >= pdata->m_itema.get_count())
               continue;
            index iStrict;
            if (m_eview == view_icon)
            {
               iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
            }
            else
            {
               iStrict = m_meshlayout.m_iaDisplayToStrict[iItem];
            }
            list_item & item = pdata->m_itema.get_item(iStrict);
            if (pdata->m_itema.get_item(iStrict).IsFolder())
            {
               _017OpenContextMenuFolder(canew(::fs::item(item)), actioncontext);
               break;
            }
            else
            {
               itema.add(canew(::fs::item(item)));
            }
         }
      }
      if (itema.get_size() > 0)
      {
         _017OpenContextMenuFile(itema, actioncontext);
      }
      else
      {
         _017OpenContextMenu(actioncontext);
      }
      _001ClearSelection();
   }

   void list::_017OpenContextMenuFolder(sp(::fs::item) item, ::action::context actioncontext)
   {
      UNREFERENCED_PARAMETER(item);
      UNREFERENCED_PARAMETER(actioncontext);
   }

   void list::_017OpenContextMenuFile(const ::fs::item_array &itema, ::action::context actioncontext)
   {
      UNREFERENCED_PARAMETER(itema);
      UNREFERENCED_PARAMETER(actioncontext);
   }

   void list::_017OpenContextMenu(::action::context actioncontext)
   {
      UNREFERENCED_PARAMETER(actioncontext);
   }

   void list::_017OpenFolder(sp(::fs::item) item, ::action::context actioncontext)
   {
      UNREFERENCED_PARAMETER(item);
      UNREFERENCED_PARAMETER(actioncontext);
      ASSERT(FALSE);
   }

   void list::_017OpenFolder(const ::userfs::list_item &item, ::action::context actioncontext)
   {
      UNREFERENCED_PARAMETER(item);
      UNREFERENCED_PARAMETER(actioncontext);
      ASSERT(FALSE);
   }

   void list::_017OpenFile(const ::fs::item_array &itema, ::action::context actioncontext)
   {
      UNREFERENCED_PARAMETER(itema);
      UNREFERENCED_PARAMETER(actioncontext);
      ASSERT(FALSE);
   }

   void list::_001OnInitializeForm(sp(::user::control) pcontrol)
   {
      ASSERT(pcontrol != NULL);
      if (pcontrol == NULL)
         return;

      /*      filemanager::file_list_callback * pcallback =
      get_filemanager_template()->get_filemanager_template()->m_pfilelistcallback;

      sp(::user::button) pbutton =  (pcontrol);
      if(pcallback != NULL && pbutton != NULL)
      {
      pcallback->InitializeActionButton(((int32_t) pcontrol->descriptor().m_id) - 1000, pbutton);
      } */
   }

   void list::_001OnButtonAction(sp(::user::control) pcontrol)
   {
      UNREFERENCED_PARAMETER(pcontrol);
      //      list_data * pdata = get_fs_mesh_data();
      /* filemanager::file_list_callback * pcallback =
      get_filemanager_template()->get_filemanager_template()->m_pfilelistcallback;

      if(pcallback != NULL)
      {
      ::fs::item item;
      int32_t iItem = pcontrol->GetEditItem();
      int32_t iStrict;
      if(m_eview == view_icon)
      {
      iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
      }
      else
      {
      iStrict = m_meshlayout.m_iaDisplayToStrict[iItem];
      }
      item.m_strPath         = pdata->m_itema.get_item(iStrict).m_strPath;
      item.m_strExtra        = pdata->m_itema.get_item(iStrict).m_strExtra;
      pcallback->OnButtonAction((int32_t) pcontrol->descriptor().m_id - 1000, item);
      } */
   }

   void list::GetSelected(::fs::item_array &itema)
   {
      list_data * pdata = get_fs_mesh_data();
      index iItemRange, iItem;
      range range;
      _001GetSelection(range);
      index_array iaItem;
      for (iItemRange = 0;
            iItemRange < range.get_item_count();
            iItemRange++)
      {
         item_range itemrange = range.ItemAt(iItemRange);
         for (iItem = MAX(0, itemrange.get_lower_bound());
               iItem <= itemrange.get_upper_bound();
               iItem++)
         {
            ::fs::item item;
            if (iItem < pdata->m_itema.get_count() && !iaItem.contains(iItem))
            {
               iaItem.add(iItem);
               index iStrict;
               if (m_eview == view_icon)
               {
                  iStrict = m_iconlayout.m_iaDisplayToStrict.get_b(iItem);
               }
               else
               {
                  if(iItem >= m_meshlayout.m_iaDisplayToStrict.get_count())
                     continue;
                  iStrict = m_meshlayout.m_iaDisplayToStrict[iItem];
               }
               itema.add(canew(::fs::item(pdata->m_itema.get_item(iStrict))));
            }
         }
      }
   }

   void list::_001OnVScroll(::message::message * pobj)
   {
      //      SCAST_PTR(::message::scroll, pscroll, pobj);
      //m_iCreateImageListStep = pscroll->m_nPos;
      //m_bRestartCreateImageList = true;
      pobj->m_bRet = false;
   }

   void list::_001OnHScroll(::message::message * pobj)
   {
      pobj->m_bRet = false;
   }

   ::count list::_001GetItemCount()
   {
      return get_fs_mesh_data()->m_itema.get_count();
   }

   void list::add_item(const char * pszPath, const char * pszTitle)
   {

      list_item item(get_app());

      item.m_filepath = pszPath;

      item.m_strName = pszTitle;

      if (get_document()->get_fs_data()->is_dir(pszPath))
      {

         item.m_flags.signalize(::fs::FlagFolder);

      }

      get_fs_mesh_data()->m_itema.add_item(item);

      _001OnUpdateItemCount();

   }


   sp(image_list) list::GetActionButtonImageList(index i)
   {
      if (i == 0)
      {
         return   Session.userex()->shell()->GetImageList(16);
      }
      return NULL;
   }


   void list::_001OnFileRename(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      range range;

      _001GetSelection(range);

      if (range.get_item_count() == 1 && range.ItemAt(0).get_lower_bound() == range.ItemAt(0).get_upper_bound())
      {

         auto iEditItem = range.ItemAt(0).get_lower_bound();

         sp(::user::control) pcontrol = _001GetControl(iEditItem, get_fs_mesh_data()->m_iNameSubItem);

         _001PlaceControl(pcontrol, iEditItem);

      }

   }


   void list::_001OnUpdateFileRename(::message::message * pobj)
   {
      SCAST_PTR(::user::command, pcommand, pobj);
      range range;
      _001GetSelection(range);
      pcommand->Enable(
      range.get_item_count() == 1
      && range.ItemAt(0).get_lower_bound() == range.ItemAt(0).get_upper_bound());
      pobj->m_bRet = true;
   }

   void list::_001OnShowWindow(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //      SCAST_PTR(::message::show_window, pshow, pobj);

      db_server * pcentral = dynamic_cast < db_server * > (&System.m_psimpledb->db());
      if (pcentral == NULL)
         return;
      //DBFileSystemSizeSet * pset = pcentral->m_pfilesystemsizeset;
      /*if(pshow->m_bShow)
      {
      for(int32_t i = 0; i < m_itema.get_item_count(); i++)
      {
      pset->m_table.add_request(m_itema.get_item(i).m_strPath);
      }
      }
      else
      {
      for(int32_t i = 0; i < m_itema.get_item_count(); i++)
      {
      pset->m_table.remove_request(m_itema.get_item(i).m_strPath);
      }
      }*/
   }


   void list::_001InitializeFormPreData()
   {
      /*      ::filemanager::data * pdata = get_filemanager_template()->get_filemanager_data();
      pdata->m_pcallback->OnFileManagerInitializeFormPreData(
      pdata,
      GetDlgCtrlId(),
      this);*/
   }


   bool list::query_drop(index iDisplayDrop, index iDisplayDrag)
   {

      list_data * pdata = get_fs_mesh_data();

      if (iDisplayDrag < 0)
      {

         return false;

      }

      if (iDisplayDrop < 0)
      {

         return false;

      }

      if (iDisplayDrag != iDisplayDrop)
      {

         index strict;

         if (m_eview == view_icon)
         {

            strict = m_iconlayout.m_iaDisplayToStrict[iDisplayDrop];

         }
         else
         {

            strict = m_meshlayout.m_iaDisplayToStrict[iDisplayDrop];

         }

         if (strict <= -1)
         {

            return true; // can drop in is_empty place

         }
         else if (strict >= _001GetItemCount())
         {

            return true; // can drop if destination is invalid

         }
         else
         {

            // can drop if destination is folder
            return pdata->m_itema.get_item(strict).IsFolder();

         }

      }

      return false;

   }


   bool list::do_drop(index iDisplayDrop, index iDisplayDrag)
   {

      list_data * pdata = get_fs_mesh_data();

      index strict;

      index strictDrag;

      if (m_eview == view_icon)
      {

         strict = m_iconlayout.m_iaDisplayToStrict[iDisplayDrop];

         strictDrag = m_iconlayout.m_iaDisplayToStrict[iDisplayDrag];

      }
      else
      {

         strict = m_meshlayout.m_iaDisplayToStrict[iDisplayDrop];

         strictDrag = m_meshlayout.m_iaDisplayToStrict[iDisplayDrag];

      }

      if (strict >= 0 && pdata->m_itema.get_item(strict).IsFolder())
      {

         ::file::path strPath = pdata->m_itema.get_item(strictDrag).m_filepath;

         string strName = strPath.name();

         get_document()->get_fs_data()->file_move(pdata->m_itema.get_item(strict).m_filepath, strPath);

//         _017Synchronize(::action::source::add(::action::source_paste, ::action::source_user));

      }
      else
      {

         ::user::list::do_drop(iDisplayDrop, iDisplayDrag);

      }

      return true;

   }


   COLORREF list::get_background_color()
   {
      //if(get_filemanager_template() != NULL && get_filemanager_template()->get_filemanager_data()->is_saving())
      {
         return RGB(255, 180, 90);
      }
      //      else
      {
         return RGB(200, 255, 255);
      }
   }


   list_data * list::get_fs_mesh_data()
   {

      return m_pmeshdata.cast < list_data > ();

   }

   ::user::list_data * list::create_mesh_data()
   {

      return canew(list_data(get_app()));

   }

   sp(::userfs::document) list::get_document()
   {
      return  (::user::form_list_view::get_document());
   }

   void list::_001GetItemText(::user::mesh_item * pitem)
   {
      return ::user::form_list_view::_001GetItemText(pitem);
   }


   void list::_001GetItemImage(::user::mesh_item * pitem)
   {


      if (pitem->m_iSubItem == m_iSelectionSubItem || pitem->m_iSubItem == m_iNameSubItem)
      {

         if (pitem->m_iSubItem == m_iIconSubItem)
         {

            list_data * pdata = get_fs_mesh_data();

            auto & item = pdata->m_itema.get_item(pitem->m_iItem);

            pitem->m_iImage = Session.userex()->shell()->get_image(
                              get_handle(),
                              item.m_filepath,
                              get_document()->get_fs_data()->is_dir(item.m_filepath) ?
                              ::user::shell::file_attribute_directory : ::user::shell::file_attribute_normal,
                              ::user::shell::icon_normal);

            if (pitem->m_iImage >= 0)
            {

               pitem->m_bOk = true;

            }

         }
         else
         {

            pitem->m_iImage = -1;

            pitem->m_bOk = false;

         }

         return;

      }


      return ::user::form_list_view::_001GetItemImage(pitem);

   }


} // namespace userfs

