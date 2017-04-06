//#include "framework.h"


namespace filemanager
{


   tree::tree(::aura::application * papp) :
      object(papp),
      ::data::data(papp),
      ::data::tree(papp),
      ::user::tree_data(papp),
      ::userfs::tree(papp)
   {

      m_iAnimate = 0;

      m_pimagelist = Session.userex()->shellimageset().GetImageList16();


   }


   tree::~tree()
   {

   }


   void tree::_017EnsureVisible(const char * lpcsz, ::action::context actioncontext)
   {
      stringa stra;
      sp(::data::tree_item) pitem = find_item(lpcsz);


      pitem = find_item(lpcsz);

      if(pitem == NULL)
         return;

      if(!(pitem->m_dwState & ::data::tree_item_state_expanded))
      {
         _001ExpandItem(pitem,actioncontext,true,false,false);
      }

      m_straUpdatePtrFilter.remove_all();

      _001EnsureVisible(pitem);

   }


   ::data::tree_item * tree::find_item(const char * lpcsz, ::data::tree_item * pitemStart)
   {

      return find_absolute(lpcsz, pitemStart);

   }


   void tree::knowledge(const ::file::path & strPath,::action::context actioncontext,bool bOnlyParent)
   {

      if(bOnlyParent && strPath.has_char() && find_item(strPath))
         return;

      ::file::listing listing(get_document()->get_fs_data());

      ::file::path strDir;

      if(bOnlyParent && strPath.has_char())
      {

         strDir = strPath.folder();

         ::file::path & path = listing.add(strPath);

         path.m_iDir = 1;

      }
      else
      {

         strDir = strPath;

         listing.ls(strPath);

      }

      filemanager_tree_insert(strDir,listing,actioncontext, bOnlyParent);

      ::data::tree_item * pitem = find_item(strDir);

      if(pitem != NULL)
      {
         if(listing.get_count() > 0)
         {

            pitem->m_dwState |= ::data::tree_item_state_expanded;
            pitem->m_dwState |= ::data::tree_item_state_expandable;

         }
      }


      if(!bOnlyParent)
      {

         arrange(::fs::arrange_by_name);

         if(m_treeptra.has_elements())
         {

            _polishing_start(m_treeptra[0]);

         }

      }

   }


   void tree::filemanager_tree_insert(const ::file::path & strPath,::file::listing & listing,::action::context actioncontext,bool bOnlyParent)
   {

      ::file::listing & straRootPath = get_document()->m_listingRoot;

      ::data::tree_item_ptr_array ptraRemove;

      sp(::data::tree_item) pitem = find_item(strPath);

      if(pitem.is_null())
         return;

      sp(::data::tree_item) pitemParent = pitem;

      //if(strPath.has_char() && !bOnlyParent)
      if(!bOnlyParent)
      {

         pitem = pitemParent->first_child();

         if(pitem.is_set())
         {

            ::file::patha straPathTrim;

            straPathTrim = listing;

            straPathTrim.trim_right("\\/");

            index iFind;

            while(pitem != NULL)
            {

               string strPathOld =  pitem->m_pitem.cast < ::userfs::item >()->m_filepath;

               strPathOld.trim_right("/\\");

               iFind = straPathTrim.find_first_ci(strPathOld);

               if(iFind < 0 || !listing[iFind].m_iDir)
               {

                  ptraRemove.add(pitem);

               }

               pitem = pitem->get_next();

            }

            remove(ptraRemove);

         }

      }


      ::file::patha straRootPathTrim;

      straRootPathTrim = straRootPath;

      straRootPathTrim.trim_right(":\\/");

      int32_t iChildCount= 0;

      int32_t i;

      ::file::path strItem;

      string strCheck;

      for(i = 0; i < listing.get_size(); i++)
      {

         strItem = listing[i];

         if(strItem.is_empty())
            continue;

         pitem = find_item(strItem);

         if(pitem != NULL)
         {

            if(pitem->m_pitem.is_set())
            {

               strCheck = pitem->m_pitem.cast < ::fs::item >()->m_filepath;

               strCheck.trim_right(":\\/");

               if(!straRootPathTrim.contains_ci(strCheck))
               {

                  // reparent

                  if(pitem->m_pparent != pitemParent)
                  {

                     pitem->SetParent(pitemParent);

                  }

                  continue;

               }

            }

         }

         sp(::userfs::item) pitemChild = canew(::userfs::item(this));

         iChildCount++;

         pitemChild->m_filepath = strItem;

         pitemChild->m_strName = listing.title(i);

         if(!listing[i].m_iDir)
         {

            /*            if(zip::Util().IsUnzipable(get_app(), pitemChild->m_strPath))
            {

               pitemChild->m_flags.signalize(::fs::FlagFolder);

               pitemChild->m_iImage = m_iDefaultImage;

               pitemChild->m_iImageSelected = m_iDefaultImageSelected;

               pitemChild->m_flags.signalize(::fs::FlagInZip);

               pitem = find_item(pitemChild->m_strPath);

               if(pitem != NULL)
               {

                  pitem = insert_item(pitemChild, ::data::RelativeReplace, pitem);

               }
               else
               {

                  pitem = insert_item(pitemChild, ::data::RelativeLastChild, pitemParent);

               }

               if(zip::Util().HasSubFolder(get_app(), pitemChild->m_strPath))
               {

                  pitem->m_dwState |= ::data::tree_item_state_expandable;

               }

            /*               if(iLevel > 1)
               {

                  _017UpdateZipList(pitemChild->m_strPath, iLevel - 1, actioncontext);

               }*/

            //}
            //else
            {

               continue;

            }

         }

         pitemChild->m_flags.signalize(::fs::FlagFolder);

         pitem = find_item(pitemChild->m_filepath, pitemParent);

         if(pitem != NULL)
         {

            pitem = insert_item(pitemChild, ::data::RelativeReplace, pitem);

            // a refresh or a file monitoring event for folder deletion or creation should
            // the most precisely possible way reset this flag
            pitemChild->m_flags.signalize(::fs::FlagHasSubFolderUnknown);

         }
         else
         {

            pitem = insert_item(pitemChild, ::data::RelativeLastChild, pitemParent);

         }

         if(pitemChild->m_flags.is_signalized(::fs::FlagHasSubFolder))
         {

            pitem->m_dwState |= ::data::tree_item_state_expandable;

         }

      }

      if(iChildCount == 0)
      {

         pitemParent->m_dwState &= ~::data::tree_item_state_expandable;

      }

   }


   void tree::browse_sync(::action::context actioncontext)
   {

      ::file::path filepath = get_filemanager_path();

      {

         ::file::listing & listing = get_document()->m_listingRoot;

         if (actioncontext &::action::source_system)
         {

            knowledge("", actioncontext, true);

            if (filepath.has_char())
            {

               ::file::patha filepatha;

               filepath.ascendants_path(filepatha);

               for (index i = 0; i < filepatha.get_size(); i++)
               {

                  ::file::path path = filepatha[i];

                  knowledge(path, actioncontext, true);

               }

            }

         }



         ::userfs::item * pitemChild;

         ::file::path pathParent = get_filemanager_path().folder();

         ::data::tree_item * pitemParent = find_item(pathParent);

         if (pitemParent == NULL)
         {

            pitemParent = get_base_item();

         }

         ::data::tree_item * pitem = find_item(filepath);

         if (filepath.has_char())
         {

            if (pitem == NULL)
            {

               pitemParent = get_base_item();

               pitemChild = canew(::userfs::item(this));

               pitemChild->m_filepath = filepath;

               pitemChild->m_strName = filepath.name();

               pitemChild->m_flags.signalize(::fs::FlagFolder);

               pitemChild->m_iImageSelected = m_iDefaultImageSelected;

               pitem = insert_item(pitemChild, ::data::RelativeLastChild, pitemParent);

               if (pitemChild->m_flags.is_signalized(::fs::FlagHasSubFolder))
               {

                  pitem->m_dwState |= ::data::tree_item_state_expandable;

               }

            }
            else
            {

               if (pitem->m_pparent != pitemParent && !listing.contains_ci(pitem->m_pitem.cast < ::fs::item >()->m_filepath))
               {

                  pitem->SetParent(pitemParent);

               }

            }

         }

         pitemParent = pitem;

         if (get_filemanager_template() != NULL && get_filemanager_data()->m_ptreeFileTreeMerge != NULL
            && !(dynamic_cast <::user::tree *> (get_filemanager_data()->m_ptreeFileTreeMerge))->m_treeptra.contains(this))
         {

            get_filemanager_data()->m_ptreeFileTreeMerge->merge(this, true);

         }

      }

      {

         stringa straChildItem;

         string str;

         ::file::listing & listing = get_document()->m_listing;

         if (!actioncontext.is(::action::source_system))
         {

            filemanager_tree_insert(filepath, listing, actioncontext);

            _017EnsureVisible(filepath, actioncontext);

            _001SelectItem(find_item(filepath));

         }

         arrange(::fs::arrange_by_name);

         if (m_treeptra.has_elements())
         {

            _polishing_start(m_treeptra[0]);

         }

      }

   }


   void tree::_001InsertColumns()
   {

      /*::user::form_control control;

      control.set_type(::user::form_control_type_edit);
      control.m_iKey = FILE_MANAGER_ID_FILE_NAME;
      pcontrol->descriptor().m_id = FILE_MANAGER_ID_FILE_NAME;
      control.set_data_type(::user::form_control::DataTypeString);

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

      for(int32_t i = 0; i < itemptraSelected.get_size(); i++)
      {
         
         stra.add(( (itemptraSelected[0]->m_pitem.cast < ::userfs::item > ()))->m_filepath);

      }

   }



   void tree::_001OnMainPostMessage(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      switch(pbase->m_wparam)
      {
      case MessageMainPostCreateImageListItemRedraw:
         {
            ((::user::interaction *) pbase->m_pwnd->m_pvoidUserInteraction)->RedrawWindow();
            ((::user::interaction *) pbase->m_pwnd->m_pvoidUserInteraction)->KillTimer(123);
            /*
            rect rect;
            int32_t iArrange = (int32_t) lparam;
            if(_001IsItemVisible(iArrange))
            {
            m_bCreateImageListRedraw = true;
            _001GetItemRect(iArrange, iArrange, rect);
            RedrawWindow(rect);
            m_bCreateImageListRedraw = false;
            }*/
         }
         break;
      }
      pbase->set_lresult(0);
      pbase->m_bRet = true;
   }

   void tree::install_message_handling(::message::dispatch *pinterface)
   {
      ::userfs::tree::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(MessageMainPost, pinterface,  this,  &tree::_001OnMainPostMessage);
//      //IGUI_WIN_MSG_LINK(WM_TIMER, pinterface, this, &tree::_001OnTimer);

      IGUI_WIN_MSG_LINK(WM_LBUTTONDBLCLK, pinterface, this, &tree::_001OnLButtonDblClk);
      IGUI_WIN_MSG_LINK(WM_CONTEXTMENU, pinterface, this, &tree::_001OnContextMenu);
//      //IGUI_WIN_MSG_LINK(WM_TIMER, pinterface, this, &tree::_001OnTimer);
      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &tree::_001OnCreate);

      //connect_command_range(FILEMANAGER_SHELL_COMMAND_FIRST, FILEMANAGER_SHELL_COMMAND_LAST, &tree::_001OnShellCommand);

   }

   void tree::StartAnimation()
   {
      m_iAnimate = 1;
      m_treeptra[0]->SetTimer(1234567, 50, NULL);

   }

   void tree::TakeAnimationSnapshot()
   {
      m_iAnimate = 1;
      //   ::user::tree::_001OnDraw(m_gdibuffer.GetBuffer());
   }


#ifdef WINDOWSEX

   IShellFolder * tree::_001GetFolder(EFolder efolder)
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

   }

   int32_t tree::MapToCSIDL(EFolder efolder)
   {
      switch(efolder)
      {
      case FolderMyComputer:
         return CSIDL_DRIVES;
      default:
         return -1;
      }
   }


#endif


   void tree::_001OnItemExpand(::data::tree_item * pitem, ::action::context actioncontext)
   {

      if(typeid(*pitem->m_pitem) == System.type_info < ::userfs::item > ())
      {

         if(get_document()->get_fs_data()->is_link(pitem->m_pitem.cast < ::userfs::item >()->m_filepath))
         {

            string strTarget;

            string strFolder;

            string strParams;

            System.file().resolve_link(strTarget, strFolder, strParams, pitem->m_pitem.cast < ::userfs::item >()->m_filepath);

            pitem = find_item(strTarget);

            knowledge(strTarget,actioncontext);

         }
         else
         {

            knowledge(pitem->m_pitem.cast < ::userfs::item >()->m_filepath,actioncontext);

         }

      }
      else
      {

         knowledge("",actioncontext);

      }

   }

   void tree::_001OnItemCollapse(::data::tree_item * pitem, ::action::context actioncontext)
   {
      UNREFERENCED_PARAMETER(pitem);
   }

   bool tree::_001IsTranslucent()
   {
      return get_filemanager_data()->m_bTransparentBackground;
   }

   void tree::_001OnOpenItem(::data::tree_item * pitem, ::action::context actioncontext)
   {

      _017OpenFolder(canew(::fs::item(*pitem->m_pitem.cast < ::userfs::item > ())), actioncontext);



   }

   void tree::_017OpenFolder(sp(::fs::item)  item, ::action::context actioncontext)
   {

      if(get_document()->get_fs_data()->is_link(item->m_filepath))
      {

         string strTarget;

         string strFolder;

         string strParams;

         System.file().resolve_link(strTarget, strFolder, strParams, item->m_filepath);

         get_filemanager_manager()->FileManagerBrowse(strTarget,actioncontext);

      }
      else
      {

         get_filemanager_manager()->FileManagerBrowse(item,actioncontext);

      }

   }




   void tree::_polishing_start(::user::tree * pusertree)
   {

      ::data::tree_item * pitem = get_base_item()->first_child();

      ::fork(get_app(), [=]()
      {


         _polishing_run(pitem, pusertree, false);


      });

      ::fork(get_app(), [=]()
      {


         _polishing_run(pitem, pusertree, true);


      });

   }


   void tree::_polishing_run(::data::tree_item * pitemStart, ::user::tree * pusertree, bool bLowLatency)
   {

      e_step estep = (e_step)(((int)step_start) + 1);

      while(estep < step_end)
      {

         ::data::tree_item * pitem = pitemStart;

         while (pitem != NULL)
         {

            if (pitem->m_pparent == pitemStart->m_pparent)
            {

               break;

            }

            _polishing_step(pitem, bLowLatency, estep);

            pitem = pitem->get_item(::data::TreeNavigationExpandedForward);

         }

         estep = (e_step)(((int)estep) + 1);

      }

      pusertree->on_change_viewport_offset();

   }


   void tree::_polishing_step(::data::tree_item * pitem, bool bLowLatency, e_step estep)
   {

      if(pitem == NULL)
         return;


      sp(::userfs::item) item = pitem->m_pitem.cast < ::userfs::item >();

      if(bLowLatency)
      {
         if(get_document()->get_fs_data()->is_zero_latency(item->m_filepath))
            return;
      }
      else
      {
         if(!get_document()->get_fs_data()->is_zero_latency(item->m_filepath))
            return;
      }

      if(estep == step_has_subdir_visible)
      {
         if(pitem->m_pparent != NULL && ((estep == step_has_subdir_visible && pitem->m_pparent->is_expanded())
                                         || (estep == step_has_subdir_hidden && !pitem->m_pparent->is_expanded())))
         {
            if(!(pitem->m_dwState & ::data::tree_item_state_expandable))
            {
               string strPath = item->m_filepath;
               if(bLowLatency)
               {
                  //sl.unlock();
                  //if(get_document()->get_fs_data()->tree_show_subdir(strPath)
                  // && get_document()->get_fs_data()->has_subdir(strPath))
                  {
                     // sl.lock();
                     //item->m_flags.signalize(::fs::FlagHasSubFolder);
                     pitem->m_dwState |= ::data::tree_item_state_expandable;
                     item->m_flags.signalize(::fs::FlagHasSubFolderUnknown);
                  }
                  //else
                  //{
                  // sl.lock();
                  //}
               }
               else
               {
                  if(get_document()->get_fs_data()->tree_show_subdir(strPath)
                        && get_document()->get_fs_data()->has_subdir(strPath))
                  {
                     item->m_flags.signalize(::fs::FlagHasSubFolder);
                     pitem->m_dwState |= ::data::tree_item_state_expandable;
                  }
               }
            }
         }
      }
      else if(estep == step_image_visible || estep == step_image_hidden)
      {

         if((item->m_iImage < 0 ||
               item->m_iImageSelected < 0) && pitem->m_pparent != NULL && ((estep == step_image_visible && pitem->m_pparent->is_expanded())
                     || (estep == step_image_hidden && !pitem->m_pparent->is_expanded())))
         {

#ifdef WINDOWSEX
            m_pimagelist = Session.userex()->shellimageset().GetImageList16();
            try
            {
               item->m_iImage = Session.userex()->shellimageset().GetImage(m_treeptra[0]->get_handle(),item->m_filepath,_shell::FileAttributeDirectory,_shell::IconNormal);
               item->m_iImageSelected = Session.userex()->shellimageset().GetImage(m_treeptra[0]->get_handle(),item->m_filepath,_shell::FileAttributeDirectory,_shell::IconOpen);
            }
            catch(...)
            {
               item->m_iImage = m_iDefaultImage;
               item->m_iImageSelected = m_iDefaultImageSelected;
            }

            if(item->m_iImage < 0)
            {
               item->m_iImage = MAX(0,m_iDefaultImage);
            }
            if(item->m_iImageSelected < 0)
            {
               item->m_iImageSelected = MAX(0, m_iDefaultImageSelected);
            }
#else

            item->m_iImage = m_iDefaultImage;
            item->m_iImageSelected = m_iDefaultImageSelected;

#endif
         }

      }

   }


   void tree::_001OnTimer(::timer * ptimer)
   {

      ::userfs::tree::_001OnTimer(ptimer);;



//      ptimer->m_bRet = false;
      if (ptimer->m_nIDEvent == 1234567)
      {
         m_iAnimate += 2;
         if (m_iAnimate >= 11)
         {
            m_iAnimate = 0;
            timer_ui(ptimer)->KillTimer(ptimer->m_nIDEvent);

         }
         timer_ui(ptimer)->RedrawWindow();
      }
      else if (ptimer->m_nIDEvent == 123)
      {
         timer_ui(ptimer)->RedrawWindow();
         m_bTimer123 = false;
         timer_ui(ptimer)->KillTimer(123);
      }

   }





   void tree::assert_valid() const
   {
      ::data::tree::assert_valid();
   }

   void tree::dump(dump_context & dumpcontext) const
   {
      ::data::tree::dump(dumpcontext);
   }


   void tree::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
   {
      ::filemanager::impact::on_update(pSender, lHint, phint);
      if (phint != NULL)
      {
         if (base_class < update_hint > ::bases(phint))
         {
            update_hint * puh = (update_hint *)phint;
            if (puh->is_type_of(update_hint::TypeInitialize))
            {


            }
            if (puh->is_type_of(update_hint::TypeFilter))
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


   void tree::_001OnLButtonDblClk(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //   int32_t iItem;

   }

   void tree::_001OnContextMenu(signal_details * pobj)
   {
   }




   void tree::_001OnShellCommand(signal_details * pobj)
   {
      SCAST_PTR(::message::command, pcommand, pobj);
      m_contextmenu.OnCommand(pcommand->GetId());
   }

   void tree::_001OnCreate(signal_details * pobj)
   {

      pobj->previous();



   }


   void tree::on_merge_user_tree(::user::tree * pusertree)
   {

      m_iDefaultImage = Session.userex()->shellimageset().GetImage(
                           pusertree->get_handle(),
                           "foo",
                           _shell::FileAttributeDirectory,
                           _shell::IconNormal);

      m_iDefaultImageSelected = Session.userex()->shellimageset().GetImage(
                                   pusertree->get_handle(),
                                   "foo",
                                   _shell::FileAttributeDirectory,
                                   _shell::IconOpen);

   }


   void tree::on_bind_user_tree(::user::tree * pusertree)
   {

      UNREFERENCED_PARAMETER(pusertree);

   }




} // namespace filemanager


