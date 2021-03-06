#include "framework.h"


namespace filemanager
{


   preview::preview(::aura::application * papp) :
      object(papp)
   {

   }

   preview::~preview()
   {
   }

   void preview::install_message_routing(::message::sender * psender)
   {
      ::user::impact::install_message_routing(psender);
   }


   void preview::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
   {
      ::filemanager::impact::on_update(pSender, lHint, phint);
      if (phint != NULL)
      {
         if (base_class < update_hint > ::bases(phint))
         {
            update_hint * puh = (update_hint *)phint;
            if (puh->is_type_of(update_hint::TypeInitialize))
            {

               /*            m_pserverNext = simpledb::AppGet()->GetDataServer();
                           AddClient(this);
                           SetDataInterface(&m_datainterface);
                           AddClient(&m_datainterface);
                           string str;
                           str.Format("file_list(%d,%d)", get_filemanager_data()->m_iTemplate, get_filemanager_data()->m_iDocument);
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
         else if (base_class < ::user::form_update_hint > ::bases(phint))
         {
         }

      }
   }



   void preview::_001OnDraw(::draw2d::graphics * pgraphics)
   {



      rect rectClient;
      GetClientRect(rectClient);
      pgraphics->fill_solid_rect(rectClient, RGB(200, 200, 190));

      if(get_filemanager_manager() == NULL)
         return;

      if (m_iCurFile < 0)
      {
      }
      else if (m_iCurFile == 0 && get_filemanager_manager()->m_listing.get_size() == 0)
      {
         pgraphics->text_out(10, 10, "No files in this directory");
      }
      else if(m_iCurFile < get_filemanager_manager()->m_listing.get_size())
      {

         pgraphics->text_out(10,10,get_filemanager_manager()->m_listing.title(m_iCurFile));

      }

   }



} // namespace filemanager




