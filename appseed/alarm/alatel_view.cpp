#include "StdAfx.h"
#include <math.h>


namespace alatel
{

   
//   BEGIN_MESSAGE_MAP(view, BaseView)
	//{{AFX_MSG_MAP(view)
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
//   END_MESSAGE_MAP()


   view::view(::ca::application * papp) :
      ca(papp),
      ::userbase::view(papp)
   {
   }

   view::~view()
   {
   }

   void view::_001InstallMessageHandling(user::win::message::dispatch * pinterface)
   {
      userbase::view::_001InstallMessageHandling(pinterface);
   }

   #ifdef _DEBUG
   void view::assert_valid() const
   {
	   ::userbase::view::assert_valid();
   }

   void view::dump(dump_context& dc) const
   {
	   ::userbase::view::dump(dc);
   }
   #endif //_DEBUG

   /////////////////////////////////////////////////////////////////////////////
   // view message handlers

   BOOL view::PreCreateWindow(CREATESTRUCT& cs)
   {
/*      cs.lpszClass = AfxRegisterWndClass(
		   CS_DBLCLKS |
		   CS_OWNDC,
		   0, 0, 0);
      cs.style &= ~WS_EX_CLIENTEDGE;*/
	   return ::userbase::view::PreCreateWindow(cs);
   }

   /////////////////////////////////////////////////////////////////////////////
   // OLE Server support

   // The following command handler provides the standard keyboard
   //  user interface to cancel an in-place editing session.  Here,
   //  the server (not the container) causes the deactivation.
   //void view::OnCancelEditSrvr()
   //{
   //	GetscreencaptureViewData()->OnDeactivateUI(FALSE);
   //}

   /////////////////////////////////////////////////////////////////////////////
   // view message handlers
   void view::on_update(::view * pSender, LPARAM lHint, ::radix::object* phint) 
   {
   }

   void view:: _001OnDraw(::ca::graphics * pdc)
   {
      pdc->SetBkMode(TRANSPARENT);
      pdc->SelectClipRgn(NULL);

      class rect rectClient;

      GetClientRect(rectClient);

      ::imaging & imaging = Application.imaging();

      DWORD dwPeriod = 1984;


      if(GetTypedParent < alatel::frame >() == NULL || GetTypedParent < alatel::frame >()->GetParent() == NULL)
      {

         byte uchAlpha = (byte) 50 + (((sin(((((double)(::GetTickCount() % dwPeriod)) * 2.0 * 3.1415) / ((double) dwPeriod))) + 1.0) / 2.0) * 100);

         imaging.color_blend(pdc, rectClient, RGB(50, 185, 205), uchAlpha); 
      }
      else
      {
         pdc->FillSolidRect(rectClient, RGB(255, 255, 255));
      }


      ::ca::font_sp font(get_app());

      font->CreatePointFont(84 + min(rectClient.width(), rectClient.height()), "Times New Roman");


      string strTime;

      class time time = ::time::get_current_time();

      strTime = time.Format("%H:%M");

      pdc->SetTextColor(RGB(0, 84, 77));

      pdc->SelectObject(font);
      class size sizeText = pdc->GetTextExtent(strTime);
      pdc->TextOutA(rectClient.center().x - sizeText.cx / 2, rectClient.center().y - sizeText.cy / 2, strTime);

//      GetDocument()->m_document._001OnDraw(pdc);

      //FIBITMAP * pdib = CImaging::LoadImageFile("C:\\screenshot.jpeg");

      //CBitmap bmp2;

      //bmp2.Attach(CImaging::FItoHBITMAP(pdib, true));
      //::ca::graphics dc2;
      //dc2.CreateCompatibleDC(pdc);
      //dc2.SelectObject(&bmp2);

      //::SendMessage(::GetDesktopWindow(), WM_PRINT, (WPARAM)(HDC)dc, PRF_CHILDREN | PRF_NONCLIENT | PRF_CLIENT);

      //pdc->BitBlt(0, 0, 1280, 1024, &dc2, 0, 0, SRCCOPY);
   //   pdc->TextOut(20, 20, "Curitiba, 24 de fevereiro de 2008.");
     // pdc->TextOut(20, 80, "Thomas Borregaard Sørensen � minha Vida Eterna, meu Cora��o Eterno, Todo meu tesouro eterno, meu Universo eterno, meu tudo eterno!!");
      //pdc->TextOut(20, 110, "Assinado Camilo Sasuke Tsumanuma.");
   }

} // namespace alarm