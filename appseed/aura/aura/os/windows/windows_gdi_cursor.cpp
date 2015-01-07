//#include "framework.h"
//#include "aura/graphics/graphics.h"


// Derived from Microsoft KB318876!!
// on 2014-06-29!! During World Cup Games 2014!! Oitavas de Final, third game between Nederland and Mexico!!
// Thank you God!!
// Thank you Microsoft!!
// Thank you Pappi!! Pelo passeio no Mercado Municipal de Curitiba com a Mummi e pelo almo�o com o-bento de sushi e onigiri delicioso!!
// Obrigado Mummi por tudo!! E por hoje tamb�m e por propiciar isso tudo!! Por falar t�o profundamente do Chico Xavier mesmo com muitas d�vidas minhas!!
// Obrigado Thomas!! Voc� poderia colaborar, n�o �!!
// Meu Deus me ajude!!

HBITMAP CreateAlphaBitmapV5(::draw2d::dib * pdib)
{
   HDC hMemDC;
   DWORD dwWidth,dwHeight;
   BITMAPV5HEADER bi;
   HBITMAP hBitmap,hOldBitmap;
   void *lpBits;
   HCURSOR hAlphaCursor = NULL;

   dwWidth  = pdib->m_size.cx;  // width of the Bitmap V5 Dib bitmap
   dwHeight = pdib->m_size.cy;  // height of the Bitmap V5 Dib bitmap

   ZeroMemory(&bi,sizeof(BITMAPV5HEADER));
   bi.bV5Size           = sizeof(BITMAPV5HEADER);
   bi.bV5Width           = dwWidth;
   bi.bV5Height          = - (LONG)dwHeight;
   bi.bV5Planes = 1;
   bi.bV5BitCount = 32;
   bi.bV5Compression = BI_BITFIELDS;
   // The following mask specification specifies a supported 32 BPP
   // alpha format for Windows XP.
   bi.bV5RedMask   =  0x00FF0000;
   bi.bV5GreenMask =  0x0000FF00;
   bi.bV5BlueMask  =  0x000000FF;
   bi.bV5AlphaMask =  0xFF000000;

   HDC hdc;
   hdc = GetDC(NULL);

   // Create the DIB section with an alpha channel.
   hBitmap = CreateDIBSection(hdc,(BITMAPINFO *)&bi,DIB_RGB_COLORS,(void **)&lpBits,NULL,(DWORD)0);

   //hMemDC = CreateCompatibleDC(hdc);
   ReleaseDC(NULL,hdc);

   // Draw something on the DIB section.
   //hOldBitmap = (HBITMAP)SelectObject(hMemDC,hBitmap);
   //PatBlt(hMemDC,0,0,dwWidth,dwHeight,WHITENESS);
   //SetTextColor(hMemDC,RGB(0,0,0));
   //SetBkMode(hMemDC,TRANSPARENT);
   //TextOut(hMemDC,0,9,"rgba",4);
   //SelectObject(hMemDC,hOldBitmap);
   //DeleteDC(hMemDC);

   // Set the alpha values for each pixel in the cursor so that
   // the complete cursor is semi-transparent.

   int iStrideDst = dwWidth * sizeof(COLORREF);
   
   ::draw2d::copy_colorref(pdib->m_size.cx,pdib->m_size.cy,(COLORREF *)lpBits,iStrideDst, pdib->m_pcolorref,pdib->m_iScan);

   return hBitmap;

}


HCURSOR CreateAlphaIcon(::draw2d::dib * pdib,bool bIcon,int xHotSpot,int yHotSpot)
{

   HBITMAP hBitmap = ::CreateAlphaBitmapV5(pdib);

   // Create an empty mask bitmap.
   HBITMAP hMonoBitmap = CreateBitmap(pdib->m_size.cx,pdib->m_size.cy,1,1,NULL);

   if(bIcon)
   {
      xHotSpot = 0;
      yHotSpot = 0;
   }

   ICONINFO ii;
   ii.fIcon = bIcon ? TRUE : FALSE;  // Change fIcon to TRUE to create an alpha icon
   ii.xHotspot = xHotSpot;
   ii.yHotspot = yHotSpot;
   ii.hbmMask = hMonoBitmap;
   ii.hbmColor = hBitmap;

   // Create the alpha cursor with the alpha DIB section.
   HICON hicon = CreateIconIndirect(&ii);

   DeleteObject(hBitmap);
   DeleteObject(hMonoBitmap);

   return hicon;
}


HCURSOR CreateAlphaCursor(::draw2d::dib * pdib,int xHotSpot,int yHotSpot)
{

   return (HCURSOR) ::CreateAlphaIcon(pdib,false,xHotSpot,yHotSpot);

}

