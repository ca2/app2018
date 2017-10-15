#include "framework.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(::helloaura::render * prender, HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int main_window(::helloaura::render * prender)
{

   HINSTANCE hInstance = Sys(prender->get_app()).m_hinstance;
   // TODO: Place code here.

   // Initialize global strings
   LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
   LoadStringW(hInstance, IDC_WINDOWSPROJECT2, szWindowClass, MAX_LOADSTRING);
   MyRegisterClass(hInstance);

   // Perform application initialization:
   if (!InitInstance(prender, hInstance, SW_SHOW))
   {
      return FALSE;
   }

   HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT2));

   MSG msg;

   // Main message loop:
   while (GetMessage(&msg, nullptr, 0, 0))
   {
      if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
      {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
   }

   return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
   WNDCLASSEXW wcex;

   wcex.cbSize = sizeof(WNDCLASSEX);

   wcex.style = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc = WndProc;
   wcex.cbClsExtra = 0;
   wcex.cbWndExtra = 0;
   wcex.hInstance = hInstance;
   wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT2));
   wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
   wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT2);
   wcex.lpszClassName = szWindowClass;
   wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

   return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(::helloaura::render * prender, HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, prender);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

   ::helloaura::render * prender;

   if (message == WM_CREATE)
   {

      prender = (::helloaura::render *)((LPCREATESTRUCT)lParam)->lpCreateParams;

      SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)prender);

      prender->begin();

      SetTimer(hWnd, 123, 50, NULL);

   }
   else
   {

      prender = (::helloaura::render *) GetWindowLongPtr(hWnd, GWL_USERDATA);

   }

   switch (message)
   {
   case WM_COMMAND:
   {
      int wmId = LOWORD(wParam);
      // Parse the menu selections:
      switch (wmId)
      {
      case IDM_ABOUT:
         DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
         break;
      case IDM_EXIT:
         DestroyWindow(hWnd);
         break;
      default:
         return DefWindowProc(hWnd, message, wParam, lParam);
      }
   }
   break;
   case WM_TIMER:
   {
      if (wParam == 123)
      {

         RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
      }
   }
   break
   ;
   case WM_ERASEBKGND:
      return 1;
   case WM_PAINT:
   {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);

      ::draw2d::graphics_sp g(prender->allocer());

      g->Attach(hdc);

      auto & dib = prender->m_dibWindow;

      if (dib.is_set() && dib->m_pcolorref != NULL)
      {

         dib->get_graphics()->FillSolidRect(prender->m_rectClient, ARGB(255, 255, 255, 255));

         prender->_001OnHelloDraw(dib->get_graphics());

      }

      g->BitBlt(0, 0, dib->m_size.cx, dib->m_size.cy, prender->m_dibWindow->get_graphics());

      ::GdiFlush();

      EndPaint(hWnd, &ps);

   }
   break;
   case WM_SIZE:
   {

      ::GetClientRect(hWnd, prender->m_rectClient);

      prender->m_dibWindow.alloc(prender->allocer());

      if (prender->m_dibWindow.is_set())
      {

         prender->m_dibWindow->create(prender->m_rectClient.size());

      }

      prender->m_bNewLayout = true;

      prender->m_bFast = true;

   } break;
   case WM_DESTROY:
      PostQuitMessage(0);
      break;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
   UNREFERENCED_PARAMETER(lParam);
   switch (message)
   {
   case WM_INITDIALOG:
      return (INT_PTR)TRUE;

   case WM_COMMAND:
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
      {
         EndDialog(hDlg, LOWORD(wParam));
         return (INT_PTR)TRUE;
      }
      break;
   }
   return (INT_PTR)FALSE;
}
