#include "framework.h"
#include "windows_simple_ui.h"


namespace os
{

   map < HWND, HWND, simple_ui *, simple_ui * > m_windowmap;

   simple_ui::simple_ui(sp(base_application) papp) :
      element(papp),
      interaction(papp)
   {

      m_w = 840;
      m_h = 284;
      m_window = NULL;
      m_bShiftKey = false;


   }

   simple_ui::~simple_ui()
   {
   }

   ATOM simple_ui::register_window_class(HINSTANCE hInstance)
   {
      WNDCLASSEX wcex;

      wcex.cbSize = sizeof(WNDCLASSEX);

      wcex.style = CS_HREDRAW | CS_VREDRAW;
      wcex.lpfnWndProc = s_window_prodecure;
      wcex.cbClsExtra = 0;
      wcex.cbWndExtra = 0;
      wcex.hInstance = hInstance;
      //   wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
      wcex.hIcon = NULL;
      wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
      wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
      //wcex.lpszMenuName = MAKEINTRESOURCE(IDC_WIN32PROJECT1);
      wcex.lpszMenuName = NULL;
      wcex.lpszClassName = m_strWindowClass;
      //wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
      wcex.hIconSm = NULL;

      return RegisterClassEx(&wcex);
   }

   bool simple_ui::prepare_window(LPCRECT lpcrect)
   {

      HWND hWnd;


      hWnd = CreateWindowEx(WS_EX_LAYERED, m_strWindowClass, m_strTitle, 0,
         lpcrect->left,
         lpcrect->top,
         width(lpcrect),
         height(lpcrect),
         NULL, NULL, m_hinstance, this);

      if (!hWnd)
      {
         return FALSE;
      }

      m_window = hWnd;

      SetTimer(hWnd, 123, 23, NULL);

      ShowWindow(hWnd, SW_SHOW);

      UpdateWindow(hWnd);

      ::SetWindowLong(hWnd, GWL_STYLE, WS_VISIBLE);


      return TRUE;

   }


   LRESULT CALLBACK simple_ui::s_window_prodecure(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
   {

      simple_ui * pui = NULL;

      if (message == WM_CREATE)
      {

         LPCREATESTRUCT lpcreatestruct = (LPCREATESTRUCT)lparam;

         pui = (simple_ui *) lpcreatestruct->lpCreateParams;

         if (pui == NULL)
            return DefWindowProc(hWnd, message, wparam, lparam);

         m_windowmap[hWnd] = pui;

         pui->m_window = hWnd;

      }

      pui = m_windowmap[hWnd];

      if (pui != NULL)
         return pui->window_procedure(hWnd, message, wparam, lparam);

      return DefWindowProc(hWnd, message, wparam, lparam);

   }


   void simple_ui::client_to_screen(POINT * ppt)
   {
      ::ClientToScreen(m_window, ppt);
   }

   void simple_ui::screen_to_client(POINT * ppt)
   {
      ::ScreenToClient(m_window, ppt);
   }

   bool simple_ui::on_windows_key_down(WPARAM wparam, LPARAM lparam)
   {
      if (wparam == VK_SHIFT)
      {
         m_bShiftKey = true;
      }
      else if (wparam == VK_ESCAPE)
      {
         on_action("escape");
      }

      return false;

   }

   bool simple_ui::on_windows_key_up(WPARAM wparam, LPARAM lparam)
   {

      string str;
      wchar_t wsz[32];

      BYTE baState[256];

      ZERO(baState);
      for (int i = 0; i < 256; i++)
      {
         baState[i] = (BYTE)GetAsyncKeyState(i);
      }

      baState[wparam & 0xff] = 0x80;

      /*if((GetAsyncKeyState(::user::key_shift) & 0x80000000) != 0)
      {
      baState[::user::key_shift] |= 0x80;
      }
      */
      if (m_bShiftKey)
      {
         baState[VK_SHIFT] |= 0x80;
      }

      int32_t iRet = ToUnicodeEx((UINT)wparam, 0, baState, wsz, 32, 0, GetKeyboardLayout(GetCurrentThreadId()));
      str = wsz;
      on_char(static_cast<UINT>(wparam), str);

      if (m_bShiftKey && wparam == VK_SHIFT)
      {
         m_bShiftKey = false;
      }


      return false;

   }


   LRESULT simple_ui::window_procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
   {

      switch (message)
      {
      case WM_DESTROY:
         PostQuitMessage(0);
         break;
      case WM_LBUTTONDOWN:
         on_lbutton_down(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
         break;
      case WM_LBUTTONUP:
         on_lbutton_up(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
         break;
      case WM_MOUSEMOVE:
         on_mouse_move(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
         break;
      case WM_KEYDOWN:
         if (!on_windows_key_down(wParam, lParam))
            goto default_window_procedure;
         break;
      case WM_KEYUP:
         if (!on_windows_key_up(wParam, lParam))
            goto default_window_procedure;
         break;
      case WM_TIMER:
         on_windows_gdi_draw_framebuffer();
         goto default_window_procedure;
      case WM_MOVE:
         if (!on_windows_move(LOWORD(lParam), HIWORD(lParam)))
            goto default_window_procedure;
         break;
      case WM_SIZE:
         if (!on_windows_size(LOWORD(lParam), HIWORD(lParam)))
            goto default_window_procedure;
         break;
      default:
         goto default_window_procedure;
      }

      return 0;

   default_window_procedure:

      return DefWindowProc(hWnd, message, wParam, lParam);

   }

   void simple_ui::GetWindowRect(RECT * prect)
   {
      ::GetWindowRect(m_window, prect);
   }
   void simple_ui::get_client_rect(RECT * prect)
   {

      ::GetClientRect(m_window, prect);

   }


   void simple_ui::on_windows_gdi_draw_framebuffer()
   {
      if (m_dib->get_graphics() != NULL)
      {
         RECT rectClient;
         rectClient.left = 0;
         rectClient.top = 0;
         rectClient.right = m_size.cx;
         rectClient.bottom = m_size.cy;
         m_dib->get_graphics()->set_alpha_mode(draw2d::alpha_mode_set);
         m_dib->get_graphics()->SetViewportOrg(0, 0);
         m_dib->get_graphics()->FillSolidRect(&rectClient, ARGB(0, 0, 0, 0));
         draw(m_dib->get_graphics());
         RECT rect;
         rect.left = m_pt.x;
         rect.top = m_pt.y;

         rect.right = m_pt.x + m_size.cx;
         rect.bottom = m_pt.y + m_size.cy;

         BYTE *dst = (BYTE*)m_dib->get_data();
         int64_t size = m_size.cx * m_size.cy;


         // >> 8 instead of / 255 subsequent alpha_blend operations say thanks on true_blend because (255) * (1/254) + (255) * (254/255) > 255


         while (size >= 8)
         {
            dst[0] = LOBYTE(((int32_t)dst[0] * (int32_t)dst[3]) >> 8);
            dst[1] = LOBYTE(((int32_t)dst[1] * (int32_t)dst[3]) >> 8);
            dst[2] = LOBYTE(((int32_t)dst[2] * (int32_t)dst[3]) >> 8);

            dst[4 + 0] = LOBYTE(((int32_t)dst[4 + 0] * (int32_t)dst[4 + 3]) >> 8);
            dst[4 + 1] = LOBYTE(((int32_t)dst[4 + 1] * (int32_t)dst[4 + 3]) >> 8);
            dst[4 + 2] = LOBYTE(((int32_t)dst[4 + 2] * (int32_t)dst[4 + 3]) >> 8);

            dst[8 + 0] = LOBYTE(((int32_t)dst[8 + 0] * (int32_t)dst[8 + 3]) >> 8);
            dst[8 + 1] = LOBYTE(((int32_t)dst[8 + 1] * (int32_t)dst[8 + 3]) >> 8);
            dst[8 + 2] = LOBYTE(((int32_t)dst[8 + 2] * (int32_t)dst[8 + 3]) >> 8);

            dst[12 + 0] = LOBYTE(((int32_t)dst[12 + 0] * (int32_t)dst[12 + 3]) >> 8);
            dst[12 + 1] = LOBYTE(((int32_t)dst[12 + 1] * (int32_t)dst[12 + 3]) >> 8);
            dst[12 + 2] = LOBYTE(((int32_t)dst[12 + 2] * (int32_t)dst[12 + 3]) >> 8);

            dst[16 + 0] = LOBYTE(((int32_t)dst[16 + 0] * (int32_t)dst[16 + 3]) >> 8);
            dst[16 + 1] = LOBYTE(((int32_t)dst[16 + 1] * (int32_t)dst[16 + 3]) >> 8);
            dst[16 + 2] = LOBYTE(((int32_t)dst[16 + 2] * (int32_t)dst[16 + 3]) >> 8);

            dst[20 + 0] = LOBYTE(((int32_t)dst[20 + 0] * (int32_t)dst[20 + 3]) >> 8);
            dst[20 + 1] = LOBYTE(((int32_t)dst[20 + 1] * (int32_t)dst[20 + 3]) >> 8);
            dst[20 + 2] = LOBYTE(((int32_t)dst[20 + 2] * (int32_t)dst[20 + 3]) >> 8);

            dst[24 + 0] = LOBYTE(((int32_t)dst[24 + 0] * (int32_t)dst[24 + 3]) >> 8);
            dst[24 + 1] = LOBYTE(((int32_t)dst[24 + 1] * (int32_t)dst[24 + 3]) >> 8);
            dst[24 + 2] = LOBYTE(((int32_t)dst[24 + 2] * (int32_t)dst[24 + 3]) >> 8);

            dst[28 + 0] = LOBYTE(((int32_t)dst[28 + 0] * (int32_t)dst[28 + 3]) >> 8);
            dst[28 + 1] = LOBYTE(((int32_t)dst[28 + 1] * (int32_t)dst[28 + 3]) >> 8);
            dst[28 + 2] = LOBYTE(((int32_t)dst[28 + 2] * (int32_t)dst[28 + 3]) >> 8);

            dst += 4 * 8;
            size -= 8;
         }
         while (size--)
         {
            dst[0] = LOBYTE(((int32_t)dst[0] * (int32_t)dst[3]) >> 8);
            dst[1] = LOBYTE(((int32_t)dst[1] * (int32_t)dst[3]) >> 8);
            dst[2] = LOBYTE(((int32_t)dst[2] * (int32_t)dst[3]) >> 8);
            dst += 4;
         }

         m_gdi.update_window(m_window, (COLORREF *)m_dib->get_data(), &rect);

      }

   }

   bool simple_ui::on_windows_move(int32_t x, int32_t y)
   {

      m_pt.x = x;
      m_pt.y = y;

      return true;

   }

   bool simple_ui::on_windows_size(int32_t cx, int32_t cy)
   {

      m_size.cx = cx;
      m_size.cy = cy;

      m_dib.create(::get_thread_app()->allocer());
      m_dib->create(m_size.cx, m_size.cy);

      m_gdi.create(m_window, cx, cy);

      layout();

      return true;

   }


   void simple_ui::set_capture()
   {

      ::SetCapture(m_window);

   }


   void simple_ui::release_capture()
   {
      
      ::ReleaseCapture();

   }

} // namespace os






