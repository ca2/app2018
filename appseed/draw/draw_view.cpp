#include "stdafx.h"




IMPLEMENT_DYNCREATE(html_view, BaseView)

  BEGIN_MESSAGE_MAP(html_view, BaseView)
	//{{AFX_MSG_MAP(html_view)
/*
   ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
   ON_MESSAGE(WM_USER + 177, OnTabClick)
   ON_MESSAGE(WM_APP + 119, OnWavePlayerEvent)
	ON_COMMAND(ID_FILE_PRINT, BaseView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, BaseView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, BaseView::OnFilePrintPreview)
   */
END_MESSAGE_MAP()


html_view::html_view()
{
}

html_view::~html_view()
{
}

void html_view::_001InstallMessageHandling(igui::win::message::Dispatch * pinterface)
{
   BaseView::_001InstallMessageHandling(this);

	IGUI_WIN_MSG_LINK(WM_DESTROY        , this, this, &html_view::_001OnDestroy);
	IGUI_WIN_MSG_LINK(WM_SIZE           , this, this, &html_view::_001OnSize);
	IGUI_WIN_MSG_LINK(WM_PAINT          , this, this, &html_view::_001OnPaint);
	IGUI_WIN_MSG_LINK(WM_CREATE         , this, this, &html_view::_001OnCreate);
	IGUI_WIN_MSG_LINK(WM_CONTEXTMENU    , this, this, &html_view::_001OnContextMenu);
	IGUI_WIN_MSG_LINK(WM_SETCURSOR      , this, this, &html_view::_001OnSetCursor);
	IGUI_WIN_MSG_LINK(WM_ERASEBKGND     , this, this, &html_view::_001OnEraseBkgnd);

//	IGUI_WIN_MSG_LINK(WM_USER + 177     , this, this, &html_view::_001OnTabClick);
	IGUI_WIN_MSG_LINK(WM_APP + 119      , this, this, &html_view::_001OnWavePlayerEvent);
	//IGUI_WIN_ON_COMMAND(ID_FILE_PRINT, BaseView::OnFilePrint)
	//IGUI_WIN_ON_COMMAND(ID_FILE_PRINT_DIRECT, BaseView::OnFilePrint)
	//IGUI_WIN_ON_COMMAND(ID_FILE_PRINT_PREVIEW, BaseView::OnFilePrintPreview)
   IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN    , this, this, &Ex1VirtualGuieInterface::_001OnLButtonDown);
   IGUI_WIN_MSG_LINK(WM_LBUTTONUP    , this, this, &Ex1VirtualGuieInterface::_001OnLButtonUp);
   IGUI_WIN_MSG_LINK(WM_KEYDOWN    , this, this, &Ex1VirtualGuieInterface::_001OnKeyDown);
   IGUI_WIN_MSG_LINK(WM_KEYUP    , this, this, &Ex1VirtualGuieInterface::_001OnKeyUp);
   IGUI_WIN_MSG_LINK(WM_TIMER    , this, this, &Ex1VirtualGuieInterface::_001OnTimer);


}
/////////////////////////////////////////////////////////////////////////////
// html_view drawing

void html_view::OnDraw(CDC* pdcScreen)
{
}

/////////////////////////////////////////////////////////////////////////////
// html_view diagnostics

#ifdef _DEBUG
void html_view::AssertValid() const
{
	BaseView::AssertValid();
}

void html_view::Dump(CDumpContext& dc) const
{
	BaseView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// html_view message handlers

// vmpLightView.cpp : implementation of the html_view class
//


/////////////////////////////////////////////////////////////////////////////
// html_view


BOOL html_view::PreCreateWindow(CREATESTRUCT& cs)
{
   cs.lpszClass = AfxRegisterWndClass(
		CS_DBLCLKS |
		CS_OWNDC,
		0, 0, 0);
   cs.style &= ~WS_EX_CLIENTEDGE;
	return BaseView::PreCreateWindow(cs);
}
/////////////////////////////////////////////////////////////////////////////
// html_view printing

BOOL html_view::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void html_view::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
}

void html_view::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.
//void html_view::OnCancelEditSrvr()
//{
//	GetscreencaptureViewData()->OnDeactivateUI(FALSE);
//}

/////////////////////////////////////////////////////////////////////////////
// html_view message handlers
void html_view::_001OnInitialUpdate(gen::signal_object * pobj) 
{
   BaseView::_001OnInitialUpdate(pobj);

	
}
base_wnd * html_view::KaraokeGetWnd()
{
   return this;
}

base_wnd * html_view::_001GetWnd()
{
   return this;
}

void html_view::OnUpdate(BaseView* pSender, LPARAM lHint, base_object* phint) 
{
   if(lHint == 123)
   {
      base_wnd::GetClientRect(&GetDocument()->m_document.m_rect);
      if(GetDocument()->m_document.m_rect.Width() > 0 &&
         GetDocument()->m_document.m_rect.Height() > 0)
      {
         CDC * pdc = base_wnd::GetDC();
         GetDocument()->m_document.implement(pdc);
         base_wnd::ReleaseDC(pdc);
         _001RedrawWindow();
      }
   }
	
	
}
base_wnd * html_view::BackViewGetWnd()
{
   return this;
}
void html_view::_001OnDestroy(gen::signal_object * pobj) 
{
	BaseView::OnDestroy();

}



void html_view::_001OnSize(gen::signal_object * pobj) 
{
   SCAST_PTR(igui::win::message::size, psize, pobj)

   base_wnd::GetClientRect(&GetDocument()->m_document.m_rect);
   if(GetDocument()->m_document.m_rect.Width() > 0 &&
      GetDocument()->m_document.m_rect.Height() > 0)
   {
      CDC * pdc = base_wnd::GetDC();
      GetDocument()->m_document.implement(pdc);
      base_wnd::ReleaseDC(pdc);
      _001RedrawWindow();
   }

/*   pobj->previous();

   double d = psize->m_size.cy;
   d /= GetSystemMetrics(SM_CYSCREEN);
   SetScale(d);

   GetClientRect(m_rectKaraokeView);


   rect rectClient;
   _w001GetClientRect(rectClient);
   rect rect = rectClient;

   rect.top = rect.bottom - 24;
   m_viewlineStatus.SetPlacement(rect);


   m_rectKaraokeView.bottom -= 16;

   LayoutTitleLines();
   LayoutLyricTemplateLines();
   LayoutLyricLines();
   LayoutKaraokeBouncingBall();*/
}

void html_view::_001OnPaint(gen::signal_object * pobj) 
{
	CPaintDC dc(this); // device context for painting
   

   dc.TextOut(20, 20, "Thomas Borregaard Sørensen � minha Vida Eterna, meu Cora��o Eterno, Todo meu tesouro eterno, meu Universo eterno, meu tudo eterno!!");
}


void html_view:: _001OnDraw(CDC * pdc)
{
   m_hwnd = GetSafeHwnd();
   pdc->SetBkMode(TRANSPARENT);
   GetDocument()->m_document._001OnDraw(pdc);

   //FIBITMAP * pdib = CImaging::LoadImageFile("C:\\screenshot.jpeg");

   //CBitmap bmp2;

   //bmp2.Attach(CImaging::FItoHBITMAP(pdib, true));
   //CDC dc2;
   //dc2.CreateCompatibleDC(pdc);
   //dc2.SelectObject(&bmp2);

   //::SendMessage(::GetDesktopWindow(), WM_PRINT, (WPARAM)(HDC)dc, PRF_CHILDREN | PRF_NONCLIENT | PRF_CLIENT);

   //pdc->BitBlt(0, 0, 1280, 1024, &dc2, 0, 0, SRCCOPY);
//   pdc->TextOut(20, 20, "Curitiba, 24 de fevereiro de 2008.");
  // pdc->TextOut(20, 80, "Thomas Borregaard Sørensen � minha Vida Eterna, meu Cora��o Eterno, Todo meu tesouro eterno, meu Universo eterno, meu tudo eterno!!");
   //pdc->TextOut(20, 110, "Assinado Camilo Sasuke Tsumanuma.");
}

void html_view::_001OnCreate(gen::signal_object * pobj) 
{
   if(pobj->previous())
      return;
   
   GetDocument()->m_document.m_pwnd = this;
   GetDocument()->m_document.m_pguie = this;


}
void html_view::_001OnContextMenu(gen::signal_object * pobj) 
{
   SCAST_PTR(igui::win::message::context_menu, pcontextmenu, pobj)
   point point = pcontextmenu->GetPoint();
	
}


void html_view::_001OnTabClick(int iTab)
{
   if(iTab == 1)
   {
      AfxMessageBox("Playlist");
   }
}

void html_view::_001OnWavePlayerEvent(gen::signal_object * pobj)
{
   SCAST_PTR(igui::win::message::base, pbase, pobj)
}

void html_view::_001OnUpdateViewEncoding(gen::signal_object * pobj)
{
   SCAST_PTR(igui::win::message::update_cmd_ui, pupdatecmdui, pobj)
}
void html_view::_001OnViewEncoding(gen::signal_object * pobj)
{
   SCAST_PTR(igui::win::message::command, pcommand, pobj)
}


/*xxxvoid html_view::VmsDataOnAfterChange(int iConfigurationId, int iLine, int iColumn, CVmsDataUpdateHint * puh)
{
   if(iConfigurationId == _vmsp::CConfiguration::CfgKaraokeEncoding)
   {
      PrepareLyricLines();
      RedrawWindow();
   }
}*/


void html_view::_001OnSetCursor(gen::signal_object * pobj) 
{
   ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	
	pobj->previous();
}
void html_view::_001OnEraseBkgnd(gen::signal_object * pobj) 
{
   SCAST_PTR(igui::win::message::erase_bkgnd, perasebkgnd, pobj)
   perasebkgnd->m_bRet = true;
   perasebkgnd->set_result(TRUE);
}



html_document * html_view::GetDocument()
{
   return dynamic_cast < html_document * > (BaseView::GetDocument());
}