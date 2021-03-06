#include "framework.h"


namespace filemanager
{


   impact::impact()
   {

      m_pmanager = NULL;

      m_bEditConnectInit = false;

   }

   impact::~impact()
   {

   }


   void impact::install_message_routing(::message::sender * psender)
   {

      ::user::impact::install_message_routing(psender);

      connect_command_probe("edit_paste",&impact::_001OnUpdateEditPaste);

      connect_command("edit_paste",&impact::_001OnEditPaste);

      IGUI_MSG_LINK(WM_APP + 1024,psender,this,&impact::_001OnOperationDocMessage);

   }



   ::user::impact * impact::get_this_view()
   {

      return dynamic_cast <::user::impact *> (this);

   }


   ::fs::item * impact::get_filemanager_item()
   {

      return get_filemanager_manager()->get_filemanager_item();

   }

   ::file::path impact::get_filemanager_path()
   {

      return get_filemanager_item()->m_filepath;
   }

   manager * impact::get_filemanager_manager()
   {

      return m_pmanager;

   }


   data * impact::get_filemanager_data()
   {

      return  get_filemanager_manager()->get_filemanager_data();

   }


   manager_template * impact::get_filemanager_template()
   {

      return  get_filemanager_data()->get_filemanager_template();

   }


   void impact::on_update(::user::impact * psender, LPARAM lhint, object * phint)
   {

      UNREFERENCED_PARAMETER(psender);
      UNREFERENCED_PARAMETER(lhint);

      if (phint != NULL)
      {
         if (base_class < update_hint > ::bases(phint))
         {
            update_hint * puh = (update_hint *)phint;
            if (puh->is_type_of(update_hint::TypeInitialize))
            {
               if (m_pmanager == NULL
                     && (puh->m_pview == NULL ||
                         puh->m_pview == dynamic_cast < ::user::impact * > (this)))
               {

                  m_pmanager = puh->m_pmanager;

               }

            }
            else if(puh->is_type_of(update_hint::TypeSynchronizePath))
            {
               if(Application.is_equal_file_path(puh->m_filepath, get_filemanager_item()->m_filepath))
               {
#define DBG_LOOP  1
                  for (index i = 0; i < DBG_LOOP; i++)
                  {
                     browse_sync(puh->m_actioncontext + ::action::source_sync);

                  }
               }
               else
               {
                  knowledge(puh->m_filepath, puh->m_actioncontext + ::action::source_sync);
               }
               set_need_redraw();
            }

         }
         else if(base_class < ::user::view_update_hint > ::bases(phint))
         {
            sp(::user::view_update_hint) puh = phint;
            if(puh->is_type_of(::user::view_update_hint::hint_create_views))
            {
               if(m_pmanager == NULL
                     && (puh->oprop("data").cast < impact >() == NULL ||
                         puh->oprop("data").cast < impact >() == (this)))
               {
                  m_pmanager = puh->oprop("manager").cast <manager>();
               }
            }
         }
      }
   }


   void impact::_001Refresh()
   {

      get_filemanager_manager()->FileManagerBrowse(get_filemanager_item(),::action::source_sync);

   }


   void impact::browse_sync(::action::context actioncontext)
   {

      UNREFERENCED_PARAMETER(actioncontext);

   }


   ::database::key impact::calc_parent_data_key()
   {

      ::database::key key = ::user::impact::calc_parent_data_key();

      get_filemanager_data()->defer_update_data_key();

      key += get_filemanager_data()->m_datakey;

      return key;

   }


   void impact::knowledge(const string & strPath, ::action::context actioncontext)
   {

      UNREFERENCED_PARAMETER(strPath);
      UNREFERENCED_PARAMETER(actioncontext);

   }


   void impact::_001OnUpdateEditPaste(::message::message * pobj)
   {

      SCAST_PTR(::user::command,pcommand,pobj);

      pcommand->Enable(Session.copydesk().has_filea());

      pobj->m_bRet = true;

   }


   void impact::_001OnEditPaste(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      ::file::listing listing;

      ::user::copydesk::e_op eop;

      if(!Session.copydesk().get_filea(listing, eop) || listing.is_empty())
      {

         pobj->m_bRet = true;

         return;

      }

      string strDir;

      strDir = get_filemanager_item()->m_filepath;

      ::user::impact * pview  = get_this_view();

      if(pview == NULL)
      {

         return;

      }

      tab_view * ptabview = get_this_view()->GetTypedParent < tab_view >();

      if(ptabview != NULL)
      {

         bool bDeleteOriginOnSuccessfulCopy = eop == ::user::copydesk::op_cut;

         ptabview->filemanager_manager().get_operation_doc(true)->m_thread.queue_copy(listing, strDir, cnull, true, false, bDeleteOriginOnSuccessfulCopy, this, WM_APP + 1024, 4096);

         ptabview->filemanager_manager().get_operation_doc(true)->m_thread.kick();

      }

      pobj->m_bRet =true;

   }


   void impact::_001OnOperationDocMessage(::message::message * pobj)
   {

      SCAST_PTR(::message::base,pbase,pobj);

      if(pbase->m_wparam == 4096)
      {

         if(pbase->m_lparam == 0)
         {

            _001Refresh();

         }

      }

   }




} // namespace filemanager




