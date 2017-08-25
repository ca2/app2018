#include "framework.h"


namespace filemanager
{


   path_view::path_view(::aura::application * papp) :
      object(papp),
      ::user::interaction(papp),
      ::user::plain_edit(papp)
   {

      m_bVoidSync = false;

   }

   
   void path_view::install_message_routing(::message::sender * psender)
   {
      
      ::filemanager::impact::install_message_routing(psender);
      ::user::plain_edit::install_message_routing(psender);
      
   }
   
   
   void path_view::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
   {

      ::filemanager::impact::on_update(pSender, lHint, phint);

      if (phint != NULL)
      {
         if (base_class < update_hint >::bases(phint))
         {
            update_hint * puh = (update_hint *) phint;
            if (puh->m_pview == this &&
               puh->is_type_of(update_hint::TypeInitialize))
            {
               //            get_filemanager_manager() = puh->get_filemanager_manager();
               /*            m_pserverNext = simpledb::AppGet()->GetDataServer();
                           AddClient(this);
                           SetDataInterface(&m_datainterface);
                           AddClient(&m_datainterface);
                           string str;
                           str.Format("::filemanager::file_list(%d,%d)", get_filemanager_data()->m_iTemplate, get_filemanager_data()->m_iDocument);
                           if(get_filemanager_data()->m_bTransparentBackground)
                           {
                           ::user::list::m_etranslucency = ::user::list::translucency_present;
                           }
                           DISetSection(str);
                           _001UpdateColumns();*/
            }
            else if (puh->is_type_of(update_hint::TypeFilter))
            {
               /*if(puh->m_wstrFilter.is_empty())
               {
               FilterClose();
               }
               else
               {
               FilterBegin();
               Filter1(puh->m_wstrFilter);
               FilterApply();
               }*/
            }
         }
         else if (base_class < ::user::form_update_hint >::bases(phint))
         {
         }

      }
   }


   void path_view::browse_sync(::action::context actioncontext)
   {

      if (m_bVoidSync)
         return;

      if (get_filemanager_manager() == NULL)
         return;

      string strOld;

      _001GetText(strOld);

      string strPath = get_filemanager_item()->m_filepath;

      if (strOld == strPath)
         return;

      _001SetText(get_filemanager_item()->m_filepath, actioncontext);

   }


   void path_view::_001OnAfterChangeText(::action::context actioncontext)
   {

      if (!actioncontext.is_user_source())
         return;

      string str;
      _001GetText(str);

      sp(manager) pmanager = get_filemanager_manager();

      if (pmanager == NULL)
         return;

      ::fs::data * pfsdata = pmanager->get_fs_data();

      if (pfsdata->is_dir(str))
      {

         ::file::path strPreviousPath = get_filemanager_manager()->m_item->m_filepath;
         ::file::path strPath = str;
         if (strPreviousPath != strPath)
         {

            get_filemanager_manager()->FileManagerBrowse(str, ::action::source::sync(actioncontext));

         }

      }
      else
      {

         ::file::path strName = str;

         while (true)
         {

            strName = strName.folder();

            if (get_filemanager_manager()->get_fs_data()->is_dir(strName))
            {
               if (!get_filemanager_item()->m_filepath == strName)
               {
                  keep < bool > keepVoidSync(&m_bVoidSync, true, false, true);
                  get_filemanager_manager()->FileManagerBrowse(strName, ::action::source::sync(actioncontext));
               }
               break;
            }
            if (strName.is_empty())
               break;
         }
      }

      get_filemanager_data()->m_pmanager->m_strTopic = str;
      get_filemanager_data()->m_pmanagerMain->m_strTopic = str;


   }


   void path_view::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      //COLORREF crBackground = 0;

      //get_color(crBackground, ::user::color_background);

      //if ((crBackground & ARGB(255, 0, 0, 0)) != 0)
      //{

      //   rect rectClient;

      //   GetClientRect(rectClient);

      //   pgraphics->FillSolidRect(rectClient, ARGB(255, 255, 255, 255));

         ::user::plain_edit::_001OnDraw(pgraphics);

      //}

   }


} // namespace filemanager



