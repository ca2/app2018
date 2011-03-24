#include "StdAfx.h"
#include "AlbumBuild.h"
#include "AlbumBuildWnd.h"

namespace  mediamanager
{

   AlbumBuildWnd::AlbumBuildWnd(::ca::application * papp) :
      ca(papp)
   {
      m_b1123 = false;
      m_iFieldIndexTitle = -1;
      m_iFieldIndexArtist = -1;
      m_iFieldIndexFileName = -1;
      m_iPreBuildStep = -1;
   }

   AlbumBuildWnd::~AlbumBuildWnd()
   {
   }

   bool AlbumBuildWnd::create()
   {
      return create_message_window("_vmsgenAlbumBuildWnd");
   }

   void AlbumBuildWnd::KickBuild()
   {
      KillTimer(1123);
      KillTimer(1124);
      m_iPreBuildStep = -1;
      SetTimer(1122, 30, NULL);
   }

   void AlbumBuildWnd::KillBuild()
   {
      m_b1123 = false;
      KillTimer(1123);
      KillTimer(1124);
   }

   void AlbumBuildWnd::_001OnTimer(gen::signal_object * pobj) 
   {
      SCAST_PTR(::user::win::message::timer, ptimer, pobj);
      UINT nIDEvent = ptimer->m_nIDEvent;
      if(nIDEvent == 1122)
      {
         if(!m_palbumbuild->PreBuild(m_iPreBuildStep, true, true))
         {
            KillTimer(1122);
         }
         m_hwndCallback->PostMessage(WM_USER + 1217, 0, -1);
         if(m_iPreBuildStep == -1)
         {
            KillTimer(1122);
            m_b1123 = true;
            SetTimer(1123, 30, NULL);
         }
      }
      else if(nIDEvent == 1123)
      {
         AlbumBuild * palbumbuild = GetAlbumBuild();
         if(palbumbuild->m_bBuilt == true)
         {
            KillTimer(1123);
            m_b1123 = false;
         }
         else
         {
            KillTimer(1124);

            CSingleLock slRead1(&palbumbuild->m_csRead1, TRUE);
            ::sqlite::set * pds = palbumbuild->m_pdsRead1;

            if(m_iFieldIndexTitle < 0)
            {
               m_iFieldIndexTitle      = pds->GetFieldIndex("title");
               m_iFieldIndexArtist     = pds->GetFieldIndex("artist");
               //m_iFieldIndexFileName   = pds->GetFieldIndex("filename");
            }


            int_array & ia = palbumbuild->GetPriorityArray();

            if(ia.get_size() <= 0)
            {
               if(palbumbuild->m_pdb->in_transaction())
               {
                  palbumbuild->m_pdb->commit_transaction();
               }
               KillTimer(1123);
               m_b1123 = false;
               SetTimer(1124, 50, NULL);
            }

            if(!palbumbuild->m_pdb->in_transaction())
            {
               m_iTransactionTrigger = 0;
               palbumbuild->m_pdb->start_transaction();
            }

            int iUpdatedId;

            System.GetThread()->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
         

            int iRemove = max(30, ia.get_size());

            int_array iaRemove;
            var fv;
            int iId;
            int iAdded = 0;
            int iUpdated = 0;
            int iRemoved = 0;
            while(ia.get_size() > 0)
            {
               iId = ia.element_at(0);
               ia.remove_at(0);
               fv = iId;
               if(!pds->find_first("id", fv))
                  continue;

               string wstrPath;
               wstrPath = pds->fv("filepath");

               int iFind;
               if((iFind = palbumbuild->m_fileinfo.m_wstraAdd.find_first(wstrPath)) >= 0)
               {
                  iAdded++;
                  palbumbuild->add(wstrPath, palbumbuild->m_fileinfo.m_timeaAdd[iFind]);
                  palbumbuild->m_fileinfo.m_wstraAdd.remove_at(iFind);
                  palbumbuild->m_fileinfo.m_timeaAdd.remove_at(iFind);
                  break;
               }
               else if((iFind = palbumbuild->m_fileinfo.m_wstraUpdate.find_first(wstrPath)) >= 0)
               {
                  iUpdated++;
                  iUpdatedId = palbumbuild->m_fileinfo.m_iaUpdate[iFind];
                  palbumbuild->Update(
                     wstrPath, 
                     palbumbuild->m_fileinfo.m_timeaUpdate[iFind],
                     palbumbuild->m_fileinfo.m_iaUpdate[iFind]);
                  pds->FieldValueAt(m_iFieldIndexTitle)     = palbumbuild->GetAlbumRecord().m_wstrTitle;
                  pds->FieldValueAt(m_iFieldIndexArtist)    = palbumbuild->GetAlbumRecord().m_wstrArtist;
                 // pds->FieldValueAt(m_iFieldIndexFileName)  = palbumbuild->GetAlbumRecord().m_wstrFileName;
                  palbumbuild->m_fileinfo.m_iaUpdate.remove_at(iFind);
                  palbumbuild->m_fileinfo.m_wstraUpdate.remove_at(iFind);
                  palbumbuild->m_fileinfo.m_timeaUpdate.remove_at(iFind);
                  break;
               }
               else if((iFind = palbumbuild->m_fileinfo.m_wstraRemove.find_first(wstrPath)) >= 0)
               {
                  iRemoved++;
                  iaRemove.add(palbumbuild->m_fileinfo.m_iaRemove[iFind]);
                  palbumbuild->m_fileinfo.m_iaRemove.remove_at(iFind);
                  palbumbuild->m_fileinfo.m_wstraRemove.remove_at(iFind);
                  pds->remove_row();
                  iRemove--;
                  if(iRemove < 0)
                     break;
               }
            }
            palbumbuild->remove(iaRemove);
            if(ia.get_size() <= 0)
            {
               if(palbumbuild->m_pdb->in_transaction())
               {
                  palbumbuild->m_pdb->commit_transaction();
               }
               KillTimer(1123);
               m_b1123 = false;
               SetTimer(1124, 50, NULL);
            }
            if(iRemoved > 0 || iAdded > 0)
            {
               m_hwndCallback->PostMessage(WM_USER + 1217, 0, -1);
            }
            else if(iUpdated > 0)
            {
               m_hwndCallback->PostMessage(WM_USER + 1217, 1, iUpdated == 1 ? iUpdatedId : -1);
            }
         }

      }
      else if(nIDEvent == 1124)
      {
         AlbumBuild * palbumbuild = GetAlbumBuild();
         CSingleLock slRead1(&palbumbuild->m_csRead1, TRUE);
         ::sqlite::set * pds = palbumbuild->m_pdsRead1;

         if(m_iFieldIndexTitle < 0)
         {
            m_iFieldIndexTitle      = pds->GetFieldIndex("title");
            m_iFieldIndexArtist     = pds->GetFieldIndex("artist");
            //m_iFieldIndexFileName   = pds->GetFieldIndex("filename");
         }

         System.GetThread()->SetThreadPriority(THREAD_PRIORITY_BELOW_NORMAL);

         int iRemove = 30;
         if(!palbumbuild->m_pdb->in_transaction())
         {
            m_iTransactionTrigger = 0;
            palbumbuild->m_pdb->start_transaction();
         }

         int_array iaRemove;
         int iFind = 0;
         var fv;
         int iAdded = 0;
         int iUpdated = 0;
         int iRemoved = 0;
         while(true)
         {
            if(iFind < palbumbuild->m_fileinfo.m_iaUpdate.get_size())
            {
               iUpdated++;
               fv = palbumbuild->m_fileinfo.m_iaUpdate[iFind];
               if(pds->find_first("id", fv))
               {
                  palbumbuild->Update(
                     palbumbuild->m_fileinfo.m_wstraUpdate[iFind],
                     palbumbuild->m_fileinfo.m_timeaUpdate[iFind],
                     palbumbuild->m_fileinfo.m_iaUpdate[iFind]);
                  pds->FieldValueAt(m_iFieldIndexTitle)     = palbumbuild->GetAlbumRecord().m_wstrTitle;
                  pds->FieldValueAt(m_iFieldIndexArtist)    = palbumbuild->GetAlbumRecord().m_wstrArtist;
              //    pds->FieldValueAt(m_iFieldIndexFileName)  = palbumbuild->GetAlbumRecord().m_wstrFileName;
                  palbumbuild->m_fileinfo.m_iaUpdate.remove_at(iFind);
                  palbumbuild->m_fileinfo.m_wstraUpdate.remove_at(iFind);
                  palbumbuild->m_fileinfo.m_timeaUpdate.remove_at(iFind);
                  break;
               }
               else
               {
                  //ASSERT(FALSE);
               }

            }
            else if(iFind < palbumbuild->m_fileinfo.m_iaRemove.get_size())
            {
               iRemoved++;
               fv = palbumbuild->m_fileinfo.m_iaRemove[iFind];
               if(pds->find_first("id", fv))
               {
                  iaRemove.add(palbumbuild->m_fileinfo.m_iaRemove[iFind]);
                  palbumbuild->m_fileinfo.m_iaRemove.remove_at(iFind);
                  palbumbuild->m_fileinfo.m_wstraRemove.remove_at(iFind);
                  pds->remove_row();
                  iRemove--;
                  if(iRemove < 0)
                     break;
               }
               else
               {
                  iFind++;
               }
            }
            else
            {
               if(palbumbuild->m_pdb->in_transaction())
               {
                  palbumbuild->m_pdb->commit_transaction();
               }

               KillTimer(1124);
               palbumbuild->m_bBuilt = true;
               break;
            }
         }
         m_iTransactionTrigger += iUpdated;
         if(m_iTransactionTrigger > 240)
         {
            if(palbumbuild->m_pdb->in_transaction())
            {
               palbumbuild->m_pdb->commit_transaction();
            }
            palbumbuild->m_pdb->start_transaction();
            m_iTransactionTrigger = 0;
         }
         palbumbuild->remove(iaRemove);
         if(iRemoved > 0 || iAdded > 0)
         {
            m_hwndCallback->PostMessage(WM_USER + 1217, 2, -1);
         }
         else if(iUpdated > 0)
         {
            m_hwndCallback->PostMessage(WM_USER + 1217, 3, -1);
         }
      }
   }

   AlbumBuild * AlbumBuildWnd::GetAlbumBuild()
   {
      return m_palbumbuild;
   }

   void AlbumBuildWnd::Initialize(AlbumBuild *palbumbuild)
   {
      m_palbumbuild = palbumbuild;
   }

}