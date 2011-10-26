#include "StdAfx.h"

namespace userbase
{

   tool_bar_control::~tool_bar_control()
   {
      DestroyWindow();
   }

   BOOL tool_bar_control::create(DWORD dwStyle, const RECT& rect, ::user::interaction* pParentWnd,
      UINT nID)
   {
      // initialize common controls
      VERIFY(System.DeferRegisterClass(AFX_WNDCOMMCTL_BAR_REG, NULL));

      ::user::interaction* pWnd = this;
      return pWnd->create(TOOLBARCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID);
   }

   int tool_bar_control::AddBitmap(int nNumButtons, ::ca::bitmap* pBitmap)
   {
      ASSERT(IsWindow());
      TBADDBITMAP tbab;
      tbab.hInst = NULL;
      tbab.nID = (UINT)pBitmap->get_os_data();
      return (int) SendMessage( TB_ADDBITMAP, (WPARAM)nNumButtons,
         (LPARAM)&tbab);
   }

   int tool_bar_control::AddBitmap(int nNumButtons, UINT nBitmapID)
   {
      ASSERT(IsWindow());
      TBADDBITMAP tbab;
      tbab.hInst = AfxFindResourceHandle((const char *)nBitmapID, RT_BITMAP);
      ASSERT(tbab.hInst != NULL);
      tbab.nID = nBitmapID;
      return (int) SendMessage( TB_ADDBITMAP, (WPARAM)nNumButtons,
         (LPARAM)&tbab);
   }

   void tool_bar_control::SaveState(HKEY hKeyRoot, const char * lpszSubKey,
      const char * lpszValueName)
   {
      ASSERT(IsWindow());
      TBSAVEPARAMS tbs;
      tbs.hkr = hKeyRoot;
      tbs.pszSubKey = lpszSubKey;
      tbs.pszValueName = lpszValueName;
      SendMessage( TB_SAVERESTORE, (WPARAM)TRUE, (LPARAM)&tbs);
   }

   void tool_bar_control::RestoreState(HKEY hKeyRoot, const char * lpszSubKey,
      const char * lpszValueName)
   {
      ASSERT(IsWindow());
      TBSAVEPARAMS tbs;
      tbs.hkr = hKeyRoot;
      tbs.pszSubKey = lpszSubKey;
      tbs.pszValueName = lpszValueName;
      SendMessage( TB_SAVERESTORE, (WPARAM)FALSE, (LPARAM)&tbs);
   }

   int tool_bar_control::AddString(UINT nStringID)
   {
      ASSERT(IsWindow());
      HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE((nStringID>>4)+1),
         RT_STRING);
      ASSERT(hInst != NULL);
      return (int)SendMessage( TB_ADDSTRING, (WPARAM)hInst, nStringID);
   }

   int tool_bar_control::OnCreate(LPCREATESTRUCT lpcs)
   {
      UNREFERENCED_PARAMETER(lpcs);
   // trans   if (::user::interaction::OnCreate(lpcs) == -1)
         // return -1;
      SetButtonStructSize(sizeof(TBBUTTON));
      return 0;
   }

   HRESULT tool_bar_control::GetDropTarget(IDropTarget** ppDropTarget)
   {
      ASSERT(IsWindow());
      ASSERT(ppDropTarget);
      return (HRESULT) SendMessage( TB_GETOBJECT, (WPARAM)&IID_IDropTarget, (LPARAM)ppDropTarget);
   }

   struct CToolBarCtrlData
   {
      WORD wVersion;
      WORD wWidth;
      WORD wHeight;
      WORD wItemCount;
      //WORD aItems[wItemCount]

      WORD* items()
         { return (WORD*)(this+1); }
   };

   /*BOOL tool_bar_control::LoadToolBar(const char * lpszResourceName)
   {
      ASSERT_VALID(this);
      ASSERT(lpszResourceName != NULL);

      // determine location of the bitmap in resource fork
      HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
      HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
      if (hRsrc == NULL)
         return FALSE;

      HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
      if (hGlobal == NULL)
         return FALSE;

      CToolBarCtrlData* pData = (CToolBarCtrlData*)LockResource(hGlobal);
      if (pData == NULL)
         return FALSE;
      ASSERT(pData->wVersion == 1);

      UINT* pItems = new UINT[pData->wItemCount];
      for (int i = 0; i < pData->wItemCount; i++)
         pItems[i] = pData->items()[i];
      BOOL bResult = SetButtons(pItems, pData->wItemCount);
      delete[] pItems;

      if (bResult)
      {
         // set new sizes of the buttons
         size sizeImage(pData->wWidth, pData->wHeight);
         size sizeButton(pData->wWidth + 7, pData->wHeight + 7);
   //      SetSizes(sizeButton, sizeImage);

         // load bitmap now that sizes are known by the toolbar control
         LoadImages((int)(WORD) (long *)lpszResourceName, AfxGetResourceHandle());
      }

      UnlockResource(hGlobal);
      FreeResource(hGlobal);

      return bResult;
   }*/

   BOOL tool_bar_control::SetButtons(const UINT* lpIDArray, int nIDCount)
   {
      ASSERT_VALID(this);
      ASSERT(nIDCount >= 1);  // must be at least one of them
      ASSERT(lpIDArray == NULL ||
         fx_is_valid_address(lpIDArray, sizeof(UINT) * nIDCount, FALSE));

      // delete all existing buttons
      /* linux int nCount = (int)DefWindowProc(TB_BUTTONCOUNT, 0, 0);
      while (nCount--)
         VERIFY(DefWindowProc(TB_DELETEBUTTON, 0, 0));

      TBBUTTON button; memset(&button, 0, sizeof(TBBUTTON));
      button.iString = -1;
      if (lpIDArray != NULL)
      {
         // add new buttons to the common control
         int iImage = 0;
         for (int i = 0; i < nIDCount; i++)
         {
            button.fsState = TBSTATE_ENABLED;
            if ((button.idCommand = *lpIDArray++) == 0)
            {
               // separator
               button.fsStyle = TBSTYLE_SEP;
               // width of separator includes 8 pixel overlap
               ASSERT(_afxComCtlVersion != -1);
               if ((GetStyle() & TBSTYLE_FLAT) || _afxComCtlVersion == VERSION_IE4)
                  button.iBitmap = 6;
               else
                  button.iBitmap = 8;
            }
            else
            {
               // a command button with image
               button.fsStyle = TBSTYLE_BUTTON;
               button.iBitmap = iImage++;
            }
            if (!DefWindowProc(TB_ADDBUTTONSW, 1, (LPARAM)&button))
               return FALSE;
         }
      }
      else
      {
         // add 'blank' buttons
         button.fsState = TBSTATE_ENABLED;
         for (int i = 0; i < nIDCount; i++)
         {
            ASSERT(button.fsStyle == TBSTYLE_BUTTON);
            if (!DefWindowProc(TB_ADDBUTTONSW, 1, (LPARAM)&button))
               return FALSE;
         }
      }
   //   m_nCount = (int)DefWindowProc(TB_BUTTONCOUNT, 0, 0);
   //   m_bDelayedButtonLayout = TRUE; */

      return TRUE;
   }

   int tool_bar_control::GetButtonText(UINT uiID, string &str)
   {
      UNREFERENCED_PARAMETER(uiID);
      UNREFERENCED_PARAMETER(str);
      TBBUTTONINFOW tbbi;
      tbbi.cbSize = sizeof(tbbi);
      tbbi.dwMask = TBIF_TEXT;
           
//      int nSize = 256;
      while(true)
      {
/*         tbbi.pszText = str.GetBuffer(nSize);
         if(!tbbi.pszText)
            return 0;
         tbbi.cchText = nSize;
         if(GetButtonInfo(uiID, &tbbi) != -1)
         {
            str.ReleaseBuffer();
            return 1;
         }
         nSize += 256;*/
      }
   }

   /////////////////////////////////////////////////////////////////////////////

   tool_bar_control::tool_bar_control()
      { }
   BOOL tool_bar_control::EnableButton(int nID, BOOL bEnable)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_ENABLEBUTTON, nID, MAKELPARAM(bEnable, 0)); }
   BOOL tool_bar_control::CheckButton(int nID, BOOL bCheck)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_CHECKBUTTON, nID, MAKELPARAM(bCheck, 0)); }
   BOOL tool_bar_control::PressButton(int nID, BOOL bPress)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_PRESSBUTTON, nID, MAKELPARAM(bPress, 0)); }
   BOOL tool_bar_control::HideButton(int nID, BOOL bHide)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_HIDEBUTTON, nID, MAKELPARAM(bHide, 0)); }
   BOOL tool_bar_control::Indeterminate(int nID, BOOL bIndeterminate)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_INDETERMINATE, nID, MAKELPARAM(bIndeterminate, 0)); }
   BOOL tool_bar_control::IsButtonEnabled(int nID)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_ISBUTTONENABLED, nID, 0); }
   BOOL tool_bar_control::IsButtonChecked(int nID)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_ISBUTTONCHECKED, nID, 0); }
   BOOL tool_bar_control::IsButtonPressed(int nID)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_ISBUTTONPRESSED, nID, 0); }
   BOOL tool_bar_control::IsButtonHidden(int nID)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_ISBUTTONHIDDEN, nID, 0); }
   BOOL tool_bar_control::IsButtonIndeterminate(int nID)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_ISBUTTONINDETERMINATE, nID, 0); }
   BOOL tool_bar_control::SetState(int nID, UINT nState)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_SETSTATE, nID, MAKELPARAM(nState, 0)); }
   int tool_bar_control::GetState(int nID)
      { ASSERT(IsWindow()); return (int) SendMessage( TB_GETSTATE, nID, 0L); }
   BOOL tool_bar_control::AddButtons(int nNumButtons, LPTBBUTTON lpButtons)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_ADDBUTTONSW, nNumButtons, (LPARAM)lpButtons); }
   BOOL tool_bar_control::InsertButton(int nIndex, LPTBBUTTON lpButton)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_INSERTBUTTON, nIndex, (LPARAM)lpButton); }
   BOOL tool_bar_control::DeleteButton(int nIndex)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_DELETEBUTTON, nIndex, 0); }
   BOOL tool_bar_control::GetButton(int nIndex, LPTBBUTTON lpButton)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_GETBUTTON, nIndex, (LPARAM)lpButton); }
   int tool_bar_control::GetButtonCount()
      { ASSERT(IsWindow()); return (int) SendMessage( TB_BUTTONCOUNT, 0, 0L); }
   UINT tool_bar_control::CommandToIndex(UINT nID)
      { ASSERT(IsWindow()); return (UINT) SendMessage( TB_COMMANDTOINDEX, nID, 0L); }
   void tool_bar_control::Customize()
      { ASSERT(IsWindow()); SendMessage( TB_CUSTOMIZE, 0, 0L); }
   // lpszStrings are separated by zeroes, last one is marked by two zeroes
   int tool_bar_control::AddStrings(const char * lpszStrings)
      { ASSERT(IsWindow()); return (int) SendMessage( TB_ADDSTRING, 0, (LPARAM)lpszStrings); }
   BOOL tool_bar_control::GetItemRect(int nIndex, LPRECT lpRect)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_GETITEMRECT, nIndex, (LPARAM)lpRect); }
   void tool_bar_control::SetButtonStructSize(int nSize)
      { ASSERT(IsWindow()); SendMessage( TB_BUTTONSTRUCTSIZE, nSize, 0L); }
   BOOL tool_bar_control::SetButtonSize(size size)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_SETBUTTONSIZE, 0, MAKELPARAM(size.cx, size.cy)); }
   BOOL tool_bar_control::SetBitmapSize(size size)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_SETBITMAPSIZE, 0, MAKELPARAM(size.cx, size.cy)); }
   void tool_bar_control::AutoSize()
      { ASSERT(IsWindow()); SendMessage( TB_AUTOSIZE, 0, 0L); }
   void tool_bar_control::SetOwner(::user::interaction* pWnd)
      { ASSERT(IsWindow()); SendMessage( TB_SETPARENT, (WPARAM)pWnd, 0L); }
   void tool_bar_control::SetRows(int nRows, BOOL bLarger, LPRECT lpRect)
      { ASSERT(IsWindow()); SendMessage( TB_SETROWS, MAKELPARAM(nRows, bLarger), (LPARAM)lpRect); }
   int tool_bar_control::GetRows()
      { ASSERT(IsWindow()); return (int) SendMessage( TB_GETROWS, 0, 0L); }
   BOOL tool_bar_control::SetCmdID(int nIndex, UINT nID)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_SETCMDID, nIndex, nID); }
   UINT tool_bar_control::GetBitmapFlags()
      { ASSERT(IsWindow()); return (UINT) SendMessage( TB_GETBITMAPFLAGS, 0, 0L); }
   int tool_bar_control::hit_test(LPPOINT ppt)
      { ASSERT(IsWindow()); return (int) SendMessage( TB_HITTEST, 0, (LPARAM)ppt); }
   DWORD tool_bar_control::GetExtendedStyle()
      { ASSERT(IsWindow()); return (DWORD) SendMessage( TB_GETEXTENDEDSTYLE, 0, 0L); }
   DWORD tool_bar_control::SetExtendedStyle(DWORD dwExStyle)
      { ASSERT(IsWindow()); return (DWORD) SendMessage( TB_SETEXTENDEDSTYLE, 0, dwExStyle); }
   COLORREF tool_bar_control::GetInsertMarkColor()
      { ASSERT(IsWindow()); return (COLORREF) SendMessage( TB_GETINSERTMARKCOLOR, 0, 0); }
   COLORREF tool_bar_control::SetInsertMarkColor(COLORREF clrNew)
      { ASSERT(IsWindow()); return (COLORREF) SendMessage( TB_SETINSERTMARKCOLOR, 0, (LPARAM) clrNew); }

   /////////////////////////////////////////////////////////////////////////////

   DWORD tool_bar_control::GetButtonSize()
      { ASSERT(IsWindow()); return (DWORD) SendMessage( TB_GETBUTTONSIZE, 0, 0L); }
   image_list* tool_bar_control::GetDisabledImageList()
      { ASSERT(IsWindow()); return NULL; }
   image_list* tool_bar_control::GetHotImageList()
      { ASSERT(IsWindow()); return NULL; }
   image_list* tool_bar_control::GetImageList()
      { ASSERT(IsWindow()); return NULL; }
   DWORD tool_bar_control::GetStyle()
      { ASSERT(IsWindow()); return (DWORD) SendMessage( TB_GETSTYLE, 0, 0L); }
   INT tool_bar_control::GetMaxTextRows()
      { ASSERT(IsWindow()); return (INT) SendMessage( TB_GETTEXTROWS, 0, 0L); }
   BOOL tool_bar_control::GetRect(int nID, LPRECT lpRect)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_GETRECT, nID, (LPARAM)lpRect); }
   BOOL tool_bar_control::IsButtonHighlighted(int nID)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_ISBUTTONHIGHLIGHTED, nID, 0); }
   void tool_bar_control::LoadImages(int iBitmapID, HINSTANCE hinst)
      { ASSERT(IsWindow()); SendMessage( TB_LOADIMAGES, iBitmapID, (LPARAM)hinst); }
   BOOL tool_bar_control::SetButtonWidth(int cxMin, int cxMax)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_SETBUTTONWIDTH, 0, MAKELPARAM(cxMin, cxMax)); }

image_list* tool_bar_control::SetDisabledImageList(image_list* pImageList)
{
   UNREFERENCED_PARAMETER(pImageList);
   ASSERT(IsWindow()); 
   return NULL; 
}

image_list* tool_bar_control::SetHotImageList(image_list* pImageList)
{
   UNREFERENCED_PARAMETER(pImageList);
   ASSERT(IsWindow()); 
   return NULL; 
}

image_list* tool_bar_control::SetImageList(image_list* pImageList)
{ 
   UNREFERENCED_PARAMETER(pImageList);
   ASSERT(IsWindow()); 
   return NULL; 
}

   BOOL tool_bar_control::SetIndent(int iIndent)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_SETINDENT, iIndent, 0L); }
   BOOL tool_bar_control::SetMaxTextRows(int iMaxRows)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_SETMAXTEXTROWS, iMaxRows, 0L); }
   void tool_bar_control::SetStyle(DWORD dwStyle)
      { ASSERT(IsWindow()); SendMessage( TB_SETSTYLE, 0, dwStyle); }
   BOOL tool_bar_control::GetButtonInfo(int nID, TBBUTTONINFO* ptbbi)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_GETBUTTONINFO, nID, (LPARAM)ptbbi); }
   BOOL tool_bar_control::GetButtonInfo(int nID, TBBUTTONINFOW* ptbbi)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_GETBUTTONINFOW, nID, (LPARAM)ptbbi); }
   BOOL tool_bar_control::SetButtonInfo(int nID, TBBUTTONINFO* ptbbi)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_SETBUTTONINFO, nID, (LPARAM)ptbbi); }
   BOOL tool_bar_control::SetButtonInfo(int nID, TBBUTTONINFOW* ptbbi)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_SETBUTTONINFOW, nID, (LPARAM)ptbbi); }
   DWORD tool_bar_control::SetDrawTextFlags(DWORD dwMask, DWORD dwDTFlags)
      { ASSERT(IsWindow()); return (DWORD) SendMessage( TB_SETDRAWTEXTFLAGS, dwMask, dwDTFlags); }
   BOOL tool_bar_control::GetAnchorHighlight()
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_GETANCHORHIGHLIGHT, 0, 0); }
   BOOL tool_bar_control::SetAnchorHighlight(BOOL fAnchor)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_SETANCHORHIGHLIGHT, fAnchor, 0); }
   int tool_bar_control::GetHotItem()
      { ASSERT(IsWindow()); return (int) SendMessage( TB_GETHOTITEM, 0, 0); }
   int tool_bar_control::SetHotItem(int nHot)
      { ASSERT(IsWindow()); return (int) SendMessage( TB_SETHOTITEM, nHot, 0); }
   void tool_bar_control::GetInsertMark(TBINSERTMARK* ptbim)
      { ASSERT(IsWindow()); SendMessage( TB_GETINSERTMARK, 0, (LPARAM)ptbim); }
   void tool_bar_control::SetInsertMark(TBINSERTMARK* ptbim)
      { ASSERT(IsWindow()); SendMessage( TB_SETINSERTMARK, 0, (LPARAM)ptbim); }
   BOOL tool_bar_control::GetMaxSize(LPSIZE pSize)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_GETMAXSIZE, 0, (LPARAM)pSize); }
   BOOL tool_bar_control::InsertMarkHitTest(LPPOINT ppt, LPTBINSERTMARK ptbim)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_INSERTMARKHITTEST, (WPARAM)ppt, (LPARAM)ptbim); }
   BOOL tool_bar_control::MapAccelerator(char chAccel, UINT* pIDBtn)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_MAPACCELERATOR, (WPARAM)chAccel, (LPARAM)pIDBtn); }
   BOOL tool_bar_control::MarkButton(int nID, BOOL bHighlight)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_MARKBUTTON, nID, MAKELPARAM(bHighlight, 0)); }
   BOOL tool_bar_control::MoveButton(UINT nOldPos, UINT nNewPos)
      { ASSERT(IsWindow()); return (BOOL) SendMessage( TB_MOVEBUTTON, nOldPos, nNewPos); }


} // namespace userbase