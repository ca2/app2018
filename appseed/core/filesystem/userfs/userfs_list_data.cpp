#include "framework.h"


string _001FileSizeText(i64 i)
{

   string str;

   double d = i;

   if(d < 1024.0)
   {

      return ::str::from(i)  + "B";

   }
   d /= 1024.0;

   if(d < 2)
   {

      str.Format("%0.1fKB", d);

      return str;

   }
   if(d < 1024.0)
   {

      str.Format("%0.0fKB", d);

      return str;

   }
   d /= 1024.0;

   if(d < 2)
   {

      str.Format("%0.1fMB", d);

      return str;

   }
   if(d < 1024.0)
   {

      str.Format("%0.0fMB", d);

      return str;

   }
   d /= 1024.0;

   if(d < 2)
   {

      str.Format("%0.1fGB", d);

      return str;

   }
   if(d < 1024.0)
   {

      str.Format("%0.0fGB", d);

      return str;

   }

   d /= 1024.0;

   str.Format("%0.0fTB", d);

   return str;

}


namespace userfs
{


   list_data::list_data(::aura::application * papp) :
      object(papp),
      ::user::mesh_data(papp),
      ::user::list_data(papp),
      m_itema(papp)
   {

      m_iNameSubItem = -1;

      m_iNameSubItemText = -1;

      m_iSizeSubItem = -1;

      m_iSelectionSubItem = -1;

      m_bPendingSize = false;

      defer_create_mutex();

   }


   list_data::~list_data()
   {

   }


   void list_data::_001GetItemText(::user::mesh_item * pitem)
   {

      synch_lock sl(m_pmutex);

//      if(is_locked())
//         return;

      if(pitem->m_iSubItem == m_iNameSubItemText)
      {

         if (pitem->m_iItem < 0 || pitem->m_iItem >= m_itema.get_size())
         {

            pitem->m_bOk = false;

            return;

         }

         pitem->m_bOk = true;

         try
         {

            pitem->m_strText = m_itema.get_item(pitem->m_iItem).m_strName;

         }
         catch (...)
         {

            pitem->m_bOk = false;

         }

         return;

      }
      else if(pitem->m_iSubItem == m_iSizeSubItem)
      {
         /*      bool bPendingSize = false;

               bool bGet;
               bGet = System.get_fs_size(str, m_itema.get_item(iItem).m_strPath, bPendingSize);
               if(bGet)
               {
                  m_straFileSize.set_at_grow(iItem, str);
               }
               else
               {
                  if(iItem < m_straFileSize.get_size())
                  {
                     str = m_straFileSize[iItem];
                  }
               }
               if(bPendingSize)
               {
                  schedule_file_size(m_itema.get_item(iItem).m_strPath);
                  m_bPendingSize = true;
               }*/

         try
         {

            ::file::path path = System.defer_process_path(m_itema.get_item(pitem->m_iItem).m_filepath, get_app());

            pitem->m_strText = _001FileSizeText(Application.file().length(path));

         }
         catch (...)
         {

            pitem->m_bOk = false;

         }

         pitem->m_bOk = true;
         return;
      }
      //else if(m_bStatic)
      //{
      // return ::user::list::_001GetItemText(str, iItem, iSubItem, iListItem);
      //}
      else
         return_(pitem->m_bOk, false);

   }

   ::count list_data::_001GetItemCount()
   {
//      if(is_locked())
//         return 0;
      return m_itema.get_count();
   }



   void list_data::_001GetItemImage(::user::mesh_item * pitem)
   {

      synch_lock sl(m_pmutex);
//      if(is_locked())
      //return;
      if(pitem->m_iSubItem == m_iNameSubItemText)
      {

         if (pitem->m_iItem < 0 || pitem->m_iItem >= m_itema.get_size())
         {

            pitem->m_bOk = false;

            return;

         }

         pitem->m_bOk = true;

         try
         {


            //pitem->m_iImage = m_itema.get_item(pitem->m_iItem).m_iImage;
            pitem->m_iImage = Session.userex()->shell()->get_image(
                              pitem->m_pmesh->get_handle(),
                              m_itema.get_item(pitem->m_iItem).m_filepath,
                              m_itema.get_item(pitem->m_iItem).m_filepath.m_iDir == 1 ? ::user::shell::file_attribute_directory : ::user::shell::file_attribute_normal,
                              ::user::shell::icon_normal);

         }
         catch (...)
         {

            pitem->m_bOk = false;

         }

         return;

      }
      else
      {
         return_(pitem->m_bOk, false);
      }

   }


} // namespace userfs


