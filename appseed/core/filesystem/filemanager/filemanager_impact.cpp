//#include "framework.h"


namespace filemanager
{


   impact::impact()
   {

      m_pmanager = NULL;

   }


   impact::~impact()
   {

   }


   ::fs::item & impact::get_filemanager_item()
   {

      return get_filemanager_manager()->get_filemanager_item();

   }


   sp(manager) impact::get_filemanager_manager()
   {

      return m_pmanager;

   }


   sp(data) impact::get_filemanager_data()
   {

      return  get_filemanager_manager()->get_filemanager_data();

   }


   sp(manager_template) impact::get_filemanager_template()
   {

      return  get_filemanager_data()->get_filemanager_template();

   }


   void impact::on_update(::aura::impact * psender, LPARAM lhint, object * phint)
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
                  puh->m_pview == (this)))
               {

                  m_pmanager = puh->m_pmanager;

               }
            }
            else if(puh->is_type_of(update_hint::TypeSynchronizePath))
            {
               if(puh->m_strPath == get_filemanager_item().m_strPath)
               {
                  browse_sync(puh->m_actioncontext + ::action::source_sync);
               }
               else
               {
                  knowledge(puh->m_strPath, puh->m_actioncontext + ::action::source_sync);
               }
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


   void impact::browse_sync(::action::context actioncontext)
   {
      
      UNREFERENCED_PARAMETER(actioncontext);

   }

   void impact::knowledge(const string & strPath, ::action::context actioncontext)
   {

      UNREFERENCED_PARAMETER(strPath);
      UNREFERENCED_PARAMETER(actioncontext);

   }


} // namespace filemanager




