// VmpPlaylistCallback.cpp: implementation of the MusicalPlayerLightPlaylistCallback class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "VmpLightPlaylistCallback.h"
#include "PaneViewUpdateHint.h"
#include "VmpLightDoc.h"
#include "VmpLightViewUpdateHint.h"



#include "PaneView1.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MusicalPlayerLightPlaylistCallback::MusicalPlayerLightPlaylistCallback(
   musctrl::MusicalPlayerCentral * pplayercentral,
   MusicalPlayerLightDoc * pdoc)
{
   ASSERT(m_pplayercentral != NULL);
   m_pplayercentral = pplayercentral;
   m_pdoc = pdoc;
}

MusicalPlayerLightPlaylistCallback::~MusicalPlayerLightPlaylistCallback()
{

}
/*bool MusicalPlayerLightPlaylistCallback::OnBeforeSetCurrentSong(
	LPCWSTR lpcwsz, 
	_vmspl::ESetCurrentSongHint ehint, 
	bool bMakeVisible)
{
   return true;
}*/

EPlaylistPlay MusicalPlayerLightPlaylistCallback::PlaylistPlay(
	PlaylistDoc * pdoc,
	LPCSTR lpcwsz, 
	_vmspl::ESetCurrentSongHint ehint, 
	bool bMakeVisible)
{
	
   ASSERT(m_pplayercentral != NULL);
	//musctrl::CSingleDocTemplate * pdoctemplate =
	//	m_pplayercentral->GetDocTemplate();

	string wstr(lpcwsz);
	string str;
	str = wstr;
	//MusicalPlayerLightDoc * pplayerdoc = (MusicalPlayerLightDoc *) pdoctemplate->OpenDocumentFileNoReserve(str);
   
   MusicalPlayerLightDoc * pplayerdoc = m_pdoc;
   
   musctrl::SingleDocumentTemplate * pdoctemplate =
      dynamic_cast < musctrl::SingleDocumentTemplate * > (pplayerdoc->GetDocTemplate());

	if(pplayerdoc != NULL)
	{
      
      mediaplay::MediaViewUpdateHint uh;

      if(ehint == _vmspl::SetCurrentSongHintDoPlay)
      {
         try
         {
            if(pdoctemplate->OpenDocumentFileNoReserve(wstr) == NULL)
            {
               string wstr;
               wstr.LoadString(IDS_FILE_OPEN_ERROR);
               WaitMessageDialog dialog(pplayerdoc->get_app(), wstr, 5000);
               dialog.open_document(ca2::app(pplayerdoc->get_app()).dir().matter("system\\wait_dialog.html"));
               HWND hwndDesktop = ::GetDesktopWindow();
               ::GetWindowRect(hwndDesktop, dialog.m_rectOpen);
               int iWidth = dialog.m_rectOpen.Width();
               int iHeight = dialog.m_rectOpen.Width();
               dialog.m_rectOpen.DeflateRect(iWidth / 4, iHeight / 4);
               int iModalResult = dialog.DoModal();
               if(iModalResult == WaitMessageDialog::ResultContinue
               || iModalResult == WaitMessageDialog::ResultTimeOut)
               {
                  return PlaylistPlayErrorContinue;
               }
               else
               {
                  return PlaylistPlayErrorCancel;
               }
            }
         }
         catch(Ex1FileException * pe)
         {
            string str;
            LPTSTR lpsz = str.GetBuffer(2048);
            pe->GetErrorMessage(lpsz, 2048);
            WaitMessageDialog dialog(pplayerdoc->get_app(), str, 5000);
            dialog.open_document(ca2::app(pplayerdoc->get_app()).dir().matter("system\\wait_dialog.html"));
            HWND hwndDesktop = ::GetDesktopWindow();
            ::GetWindowRect(hwndDesktop, dialog.m_rectOpen);
            int iWidth = dialog.m_rectOpen.Width();
            int iHeight = dialog.m_rectOpen.Width();
            dialog.m_rectOpen.DeflateRect(iWidth / 4, iHeight / 4);
            int iModalResult = dialog.DoModal();
            pe->Delete();
            if(iModalResult == WaitMessageDialog::ResultContinue
            || iModalResult == WaitMessageDialog::ResultTimeOut)
            {
               return PlaylistPlayErrorContinue;
            }
            else
            {
               return PlaylistPlayErrorCancel;
            }
         }
         catch(CFileException * pe)
         {
            string str;
            LPTSTR lpsz = str.GetBuffer(2048);
            pe->GetErrorMessage(lpsz, 2048);
            WaitMessageDialog dialog(pplayerdoc->get_app(), str, 5000);
            dialog.open_document(ca2::app(pplayerdoc->get_app()).dir().matter("system\\wait_dialog.html"));
            HWND hwndDesktop = ::GetDesktopWindow();
            ::GetWindowRect(hwndDesktop, dialog.m_rectOpen);
            int iWidth = dialog.m_rectOpen.Width();
            int iHeight = dialog.m_rectOpen.Width();
            dialog.m_rectOpen.DeflateRect(iWidth / 4, iHeight / 4);
            int iModalResult = dialog.DoModal();
            pe->Delete();
            if(iModalResult == ID_CONTINUE)
            {
               return PlaylistPlayErrorContinue;
            }
            else
            {
               return PlaylistPlayErrorCancel;
            }
         }

         uh.SetType(mediaplay::MediaViewUpdateHint::TypeDoPlay);
         pplayerdoc->UpdateAllViews(NULL, 0, &uh);
      }
      else if(ehint == _vmspl::SetCurrentSongHintPlayIfNotPlaying)
      {
         uh.SetType(mediaplay::MediaViewUpdateHint::TypeGetMidiPlayerInterface);
         pplayerdoc->UpdateAllViews(NULL, 0, &uh);
         if(uh.m_pmidiplayer != NULL)
         {
            if(uh.m_pmidiplayer->GetMidiSequence().GetState() 
               == MidiSequence::StatusOpened
               ||
               uh.m_pmidiplayer->GetMidiSequence().GetState() 
               == MidiSequence::StatusNoFile)
            {
               if(pdoctemplate->OpenDocumentFileNoReserve(wstr) == NULL)
                  return PlaylistPlayErrorCancel;
               uh.SetType(mediaplay::MediaViewUpdateHint::TypeDoPlay);
               pplayerdoc->UpdateAllViews(NULL, 0, &uh);
            }
            else
            {
               return PlaylistPlayErrorBusy;
            }
         }
      }
	}
   return PlaylistPlaySuccess;
}


void MusicalPlayerLightPlaylistCallback::PlaylistOnBeforeOpen()
{
   if(m_pdoc != NULL)
   {
      PaneView * pview = m_pdoc->GetPaneView();
      if(pview != NULL)
      {
         pview->CreateView(PaneViewPlaylist);
      }
   }

}
