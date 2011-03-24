// PaneView1.cpp : implementation file
//

#include "stdafx.h"
#include "PaneView1.h"

#include "MediaLibraryChildFrame.h"
#include "MediaLibraryDoc.h"
#include "MediaLibraryView.h"
#include "MediaLibraryViewUpdateHint.h"

#include "MPLightView.h"
#include "PaneViewUpdateHint.h"
#include "PlaylistListView.h"
#include "VmpLightDoc.h"

// vmpLight - Options
#include "OptionsDoc.h"
#include "OptionsChildFrame.h"
#include "OptionsView.h"

#include "guibase/BaseWndMenuListView.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PaneView

IMPLEMENT_DYNCREATE(PaneView, BaseView)

PaneView::PaneView()
{
   _001InstallMessageHandling(this);

   m_basecontrol.Create(this);

   m_pviewdataOld = NULL;

   // BaseWndInterface
   m_etranslucency      = TranslucencyPresent;

   m_pviewdata              = NULL;
   m_pviewdataOld              = NULL;

   SingleDocTemplate* pdoctemplate;
	pdoctemplate = new SingleDocTemplate(
		IDR_ALBUM,
		RUNTIME_CLASS(MediaLibraryDoc),
		RUNTIME_CLASS(MediaLibraryChildFrame),
		RUNTIME_CLASS(MediaLibraryView));
   m_pdoctemplateAlbum = pdoctemplate;

	pdoctemplate = new SingleDocTemplate(
		IDR_ALBUM,
		RUNTIME_CLASS(OptionsDoc),
		RUNTIME_CLASS(OptionsChildFrame),
		RUNTIME_CLASS(OptionsView));

   
   m_pdoctemplateOptions = pdoctemplate;

	pdoctemplate = new SingleDocTemplate(
		IDR_ALBUM,
		RUNTIME_CLASS(GoodMixerDoc),
		RUNTIME_CLASS(BaseChildFrame),
		RUNTIME_CLASS(MixerMainView));

   m_pdoctemplateAudioControl = pdoctemplate;
   
	IGUI_WIN_MSG_LINK(WM_CREATE       , this, this, &PaneView::_001OnCreate);
	IGUI_WIN_MSG_LINK(WM_SIZE         , this, this, &PaneView::_001OnSize);
	IGUI_WIN_MSG_LINK(WM_ERASEBKGND   , this, this, &PaneView::_001OnEraseBkgnd);
   IGUI_WIN_MSG_LINK(WM_USER + 1122  , this, this, &PaneView::_001OnMenuMessage);

}

PaneView::~PaneView()
{
}


BEGIN_MESSAGE_MAP(PaneView, BaseView)
	//{{AFX_MSG_MAP(PaneView)
   /*
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
   ON_MESSAGE(WM_USER + 1122, OnMenuMessage)
   */
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PaneView drawing

void PaneView::OnDraw(CDC* pDC)
{
	Document* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// PaneView diagnostics

#ifdef _DEBUG
void PaneView::AssertValid() const
{
	BaseView::AssertValid();
}

void PaneView::Dump(CDumpContext& dc) const
{
	BaseView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// PaneView message handlers

void PaneView::_001OnCreate(gen::signal_object * pobj) 
{
   SCAST_PTR(igui::win::message::create, pcreate, pobj)
   if(pobj->previous())
      return;
	
   CreateContext cc;
   cc.m_pCurrentDoc = GetDocument();
   cc.m_pNewViewClass =  RUNTIME_CLASS(MusicalPlayerLightView);


   ViewData * pviewdata = CreateView(PaneViewKaraoke);
   if(pviewdata == NULL)
   {
      pcreate->set_lresult(-1);
      pcreate->m_bRet = true;
      return;
   }
   m_viewmap.SetAt(PaneViewKaraoke, pviewdata);

   m_pviewdata = pviewdata;

   m_pviewdata->m_pwnd->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
   ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

//   m_plightview = (MusicalPlayerLightView *) m_pviewdata->m_pwnd;

   Layout();
}

CWnd* PaneView::CreateView(CreateContext* pContext, UINT nID, CWnd * pwndParent)
{
	ASSERT(pwndParent->m_hWnd != NULL);
	ASSERT(::IsWindow(pwndParent->m_hWnd));
	ASSERT(pContext != NULL);
	ASSERT(pContext->m_pNewViewClass != NULL);

	// Note: can be a CWnd with PostNcDestroy self cleanup
	CWnd* pView = (CWnd*)pContext->m_pNewViewClass->CreateObject();
	if (pView == NULL)
	{
		TRACE1("Warning: Dynamic create of view type %hs failed.\n",
			pContext->m_pNewViewClass->m_lpszClassName);
		return NULL;
	}
	ASSERT_KINDOF(CWnd, pView);

	// views are always created with a border!
	if (!pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0), pwndParent, nID, (CCreateContext *) pContext))
	{
		TRACE0("Warning: could not create view for frame.\n");
		return NULL;        // can't continue without a view
	}

   (dynamic_cast < BaseView * > (pView))->_001OnInitialUpdate(NULL);
/*	if (afxData.bWin4 && (pView->GetExStyle() & WS_EX_CLIENTEDGE))
	{
		// remove the 3d style from the frame, since the view is
		//  providing it.
		// make sure to recalc the non-client area
		ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
	}*/
	return pView;
}

void PaneView::OnUpdate(BaseView* pSender, LPARAM lHint, CObject* pHint) 
{

   if(pHint != NULL)
   {
      if(pHint->IsKindOf(RUNTIME_CLASS(PaneViewUpdateHint)))
      {
         PaneViewUpdateHint * puh = (PaneViewUpdateHint *) pHint;
         if(puh->IsTypeOf(PaneViewUpdateHint::TypeGetView))
         {
            puh->m_eview = GetViewId();
         }
         else if(puh->IsTypeOf(PaneViewUpdateHint::TypeSetView))
         {
            ShowView(puh->m_eview);
         }
      }
      else if(pHint->IsKindOf(RUNTIME_CLASS(MediaLibraryViewUpdateHint)))
      {
         Document * pdoc = GetMediaLibraryDoc();
         if(pdoc != NULL)
         {
            pdoc->UpdateAllViews(pSender, lHint, pHint);
         }
      }
   }
	
}

/*void PaneView::ShowPlaylist()
{
   ViewData * pview;
   if(!m_viewmap.Lookup(PaneViewPlaylist, pview))
   {
      CreatePlaylist();
   }

   if(m_pplaylistview != NULL)
   {
      Show(m_pplaylistview);
   }

}*/

/*void PaneView::ShowFileBrowser()
{
   if(m_pfilemanagerview == NULL)
   {
      FileManagerDoc * pdoc = ((MusicalPlayerLightApp *) AfxGetApp())->GetStdFileManagerTemplate()->OpenChild(((MusicalPlayerLightApp *) AfxGetApp()), false, true);

      if(pdoc != NULL)
      {
         POSITION pos = pdoc->GetFirstViewPosition();
         BaseView * pview = pdoc->GetNextView(pos);
         if(pview != NULL)
         {
            CFrameWnd * pframe = pview->GetParentFrame();
            if(pframe != NULL)
            {
               pframe->SetParent(this);
               pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
               m_pfilemanagerview = pframe;
            }
         }
      }
   }

   if(m_pfilemanagerview != NULL)
   {
      Show(m_pfilemanagerview);
   }
}*/

void PaneView::Show(ViewData * pviewdata)
{
   EPaneView eviewOld = m_pviewdata->m_eview;
   CWnd * pwndOld = m_pviewdata->m_pwnd;
   if(eviewOld != PaneViewKaraoke)
   {
      pwndOld->ShowWindow(SW_HIDE);
   }

   m_pviewdataOld = m_pviewdata;
   m_pviewdata = pviewdata;


   Layout();

   m_pviewdata->m_pwnd->UpdateWindow();
   m_pviewdata->m_pwnd->SetFocus();
   
}

void PaneView::Layout()
{
   if(m_pviewdata != NULL)
   {
      CRect rect;
      GetClientRect(rect);
      GetKaraokeWindow()->SetWindowPos(
         NULL,
         rect.left, rect.top,
         rect.Width(), rect.Height(),
         SWP_NOREDRAW | SWP_NOZORDER);
      CRect rectChild;
      GetChildRect(rectChild);
      if(m_pviewdata->m_eview != PaneViewKaraoke)
      {
         m_pviewdata->m_pwnd->SetWindowPos(
            &CWnd::wndTop,
            rectChild.left, rectChild.top,
            rectChild.Width(), rectChild.Height(),
            SWP_SHOWWINDOW);
      }
      RedrawWindow(NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE);
   }
}

void PaneView::_001OnSize(gen::signal_object * pobj) 
{
	pobj->previous();
	
   Layout();	

}

void PaneView::ShowView(EPaneView eview)
{
   CWaitCursor wc;

   ViewData * pview;
   if(m_viewmap.Lookup(eview, pview))
   {
      Show(pview);
   }
   else
   {
      pview = CreateView(eview);
      if(pview != NULL)
      {
         m_viewmap.SetAt(eview, pview);
         Show(pview);
      }
      else
      {
         TRACE("failed to create view\n");
//         AfxMessageBox("failed to create view\n");
      }
   }
   if(eview == PaneViewKaraoke)
   {
      PaneViewUpdateHint uh;
      uh.SetType(PaneViewUpdateHint::TypeOnShowKaraoke);
      GetDocument()->UpdateAllViews(this, NULL, &uh);
   }
   if(eview == PaneViewContextMenu)
   {
/*      if(m_pviewdataOld != NULL)
      {
         GetParentFrame()->SetActiveView((CView *) m_pviewdataOld->m_pwnd);
      }*/
      BaseWndMenuListView * pmenuview = dynamic_cast < BaseWndMenuListView * > (pview->m_pwnd);
      pmenuview->m_pview = m_pviewdataOld == NULL ? NULL : dynamic_cast < CView * > (m_pviewdataOld->m_pwnd);
      pmenuview->m_wnd.m_uiMessage = WM_USER + 1122;
      pmenuview->m_wnd.TrackPopupMenu(pmenuview, GetParentFrame());
   }
   {
      PaneViewUpdateHint uh;
      uh.SetType(PaneViewUpdateHint::TypeOnShowView);
      uh.m_eview = eview;
      GetDocument()->UpdateAllViews(this, NULL, &uh);
   }
}


EPaneView PaneView::GetViewId()
{
   if(m_pviewdata == NULL)
      return PaneViewNone;
   else
      return m_pviewdata->m_eview;
}


void PaneView::_001OnEraseBkgnd(gen::signal_object * pobj) 
{
   SCAST_PTR(igui::win::message::erase_bkgnd, perasebkgnd, pobj)
   perasebkgnd->set_result(TRUE);
   perasebkgnd->m_bRet = true;
}

BOOL PaneView::PreCreateWindow(CREATESTRUCT& cs) 
{
   cs.dwExStyle &= ~WS_EX_CLIENTEDGE;	

	return BaseView::PreCreateWindow(cs);
}

Document * PaneView::GetMediaLibraryDoc()
{
   ViewData * pview;
   if(m_viewmap.Lookup(PaneViewMediaLibrary, pview))
   {
      return pview->m_pdoc;
   }
   else
   {
      return NULL;
   }
}

PaneView::ViewData * PaneView::CreateView(EPaneView eview)
{
   ViewData * pviewdata = NULL;
   if(m_viewmap.Lookup(eview, pviewdata))
   {
      return pviewdata;
   }
   switch(eview)
   {
   case PaneViewContextMenu:
      {
         CreateContext cc;
         cc.m_pCurrentDoc = GetDocument();
         cc.m_pNewViewClass =  RUNTIME_CLASS(BaseWndMenuListView);

         BaseView * pview = dynamic_cast < BaseView * > (CreateView(&cc, 101, this));
         if(pview != NULL)
         {
            BaseWndMenuListView * pmenuview = (BaseWndMenuListView *) pview;
            pmenuview->m_wnd.LoadMenu(IDR_POPUP_LYRICVIEW);
            pmenuview->m_wnd.m_bAutoClose = false;
            pviewdata = new ViewData();
            pviewdata->m_eview = eview;
            pviewdata->m_pdoc = GetDocument();
            pviewdata->m_pwnd = pview;
         }
      }
      break;
   case PaneViewKaraoke:
      {
         CreateContext cc;
         cc.m_pCurrentDoc = GetDocument();
         cc.m_pNewViewClass =  RUNTIME_CLASS(MusicalPlayerLightView);

         BaseView * pview = dynamic_cast < BaseView * > (CreateView(&cc, 101, this));
         if(pview != NULL)
         {
            pviewdata = new ViewData();
            pviewdata->m_eview = eview;
            pviewdata->m_pdoc = GetDocument();
            pviewdata->m_pwnd = pview;
         }
      }
      break;
   case PaneViewFileManager:
      {
         FileManagerDoc * pdoc = ((MusicalPlayerLightApp *) AfxGetApp())->GetStdFileManagerTemplate()->OpenChild(((MusicalPlayerLightApp *) AfxGetApp()), false, true);
         if(pdoc != NULL)
         {
            POSITION pos = pdoc->GetFirstViewPosition();
            BaseView * pview = pdoc->GetNextView(pos);
            if(pview != NULL)
            {
               BaseFrameWnd * pframe = (BaseFrameWnd *) pview->GetParentFrame();
               if(pframe != NULL)
               {
                  pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
                  pframe->SetParent(this);
                  pviewdata = new ViewData();
                  pviewdata->m_eview = eview;
                  pviewdata->m_pdoc = pdoc;
                  pviewdata->m_pwnd = pframe;


               }
            }
         }
      }
      break;
   case PaneViewPlaylist:
      {
         PlaylistDoc * pdoc = ((MusicalPlayerLightApp *) AfxGetApp())->GetPlaylistCentral().GetCurrentPlaylist(true, false);
         
         if(pdoc != NULL)
         {
            MusicalPlayerLightDoc * pplayerdoc = (MusicalPlayerLightDoc *) GetDocument();
            if(pplayerdoc != NULL)
            {
               pplayerdoc->AttachPlaylist(pdoc);
            }
            if(pdoc != NULL)
            {
               POSITION pos = pdoc->GetFirstViewPosition();
               BaseView * pview = pdoc->GetNextView(pos);
               if(pview != NULL)
               {
                  BaseFrameWnd * pframe = (BaseFrameWnd *) pview->GetParentFrame();
                  if(pframe != NULL)
                  {
                     pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
                     pframe->SetParent(this);
                     pviewdata = new ViewData();
                     pviewdata->m_eview = eview;
                     pviewdata->m_pdoc = pdoc;
                     pviewdata->m_pwnd = pframe;
                  }
               }
            }
         }
      }
      break;
   case PaneViewMediaLibrary:
      {
         MediaLibraryDoc * pdoc = (MediaLibraryDoc *) m_pdoctemplateAlbum->OpenDocumentFile(NULL, FALSE);	
         if(pdoc != NULL)
         {
            POSITION pos = pdoc->GetFirstViewPosition();
            BaseView * pview = pdoc->GetNextView(pos);
            if(pdoc != NULL)
            {
               POSITION pos = pdoc->GetFirstViewPosition();
               BaseView * pview = pdoc->GetNextView(pos);
               if(pview != NULL)
               {
                  BaseFrameWnd * pframe = (BaseFrameWnd *) pview->GetParentFrame();
                  if(pframe != NULL)
                  {
                     pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
                     pframe->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
                     pframe->SetParent(this);
                     pviewdata = new ViewData();
                     pviewdata->m_eview = eview;
                     pviewdata->m_pdoc = pdoc;
                     pviewdata->m_pwnd = pframe;
                  }
               }
            }
         }
      }
      break;
   case PaneViewAudioControl:
      {
         GoodMixerDoc * pdoc = (GoodMixerDoc *) m_pdoctemplateAudioControl->OpenDocumentFile(NULL, FALSE);	
         if(pdoc != NULL)
         {
            POSITION pos = pdoc->GetFirstViewPosition();
            BaseView * pview = pdoc->GetNextView(pos);
            if(pdoc != NULL)
            {
               POSITION pos = pdoc->GetFirstViewPosition();
               BaseView * pview = pdoc->GetNextView(pos);
               if(pview != NULL)
               {
                  BaseFrameWnd * pframe = (BaseFrameWnd *) pview->GetParentFrame();
                  if(pframe != NULL)
                  {
                     pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
                     pframe->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
                     pframe->SetParent(this);
                     pviewdata = new ViewData();
                     pviewdata->m_eview = eview;
                     pviewdata->m_pdoc = pdoc;
                     pviewdata->m_pwnd = pframe;
                  }
               }
            }
         }
      }
      break;
   case PaneViewOptions:
      {
         OptionsDoc * pdoc = (OptionsDoc *) m_pdoctemplateOptions->OpenDocumentFile(NULL, FALSE);	
         if(pdoc != NULL)
         {
            POSITION pos = pdoc->GetFirstViewPosition();
            BaseView * pview = pdoc->GetNextView(pos);
            if(pdoc != NULL)
            {
               POSITION pos = pdoc->GetFirstViewPosition();
               BaseView * pview = pdoc->GetNextView(pos);
               if(pview != NULL)
               {
                  BaseFrameWnd * pframe = (BaseFrameWnd *) pview->GetParentFrame();
                  if(pframe != NULL)
                  {
                     pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
                     pframe->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
                     pframe->SetParent(this);
                     pviewdata = new ViewData();
                     pviewdata->m_eview = eview;
                     pviewdata->m_pdoc = pdoc;
                     pviewdata->m_pwnd = pframe;
                  }
               }
            }
         }
      }
      break;
   default:
      ASSERT(FALSE);
      break;
   }

   return pviewdata;
}


CWnd * PaneView::GetKaraokeWindow()
{
   ViewData * pview;
   if(m_viewmap.Lookup(PaneViewKaraoke, pview))
   {
      return pview->m_pwnd;
   }
   else
   {
      return NULL;
   }
}

void PaneView::GetChildRect(LPRECT lprect)
{
   //GetClientRect(lprect);
   //CRect rectTab;

   MusicalPlayerLightView * pview = dynamic_cast < MusicalPlayerLightView * > (GetKaraokeWindow());
   pview->m_tab.GetTabClientRect(lprect);
   //lprect->top = rectTab.bottom;
}


void PaneView::_001OnDraw(CDC *pdc)
{
   //_001DrawChildren(pdc);
}

void PaneView::Rotate()
{
   EPaneView eview = GetViewId();
   EPaneView eviewNew = PaneViewKaraoke;
   switch(eview)
   {
   case PaneViewKaraoke:
      eviewNew = PaneViewFileManager;
      break;
   case PaneViewFileManager:
      eviewNew = PaneViewPlaylist;
      break;
   case PaneViewPlaylist:
      eviewNew = PaneViewMediaLibrary;
      break;
   case PaneViewMediaLibrary:
      eviewNew = PaneViewAudioControl;
      break;
   case PaneViewAudioControl:
      eviewNew = PaneViewKaraoke;
      break;
   case PaneViewOptions:
      eviewNew = PaneViewKaraoke;
      break;
   default:
      ASSERT(FALSE);
      break;
   }

   ShowView(eviewNew);
}

void PaneView::_001OnMenuMessage(gen::signal_object * pobj)
{
   ShowView(m_pviewdataOld->m_eview);
}