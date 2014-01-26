#pragma once


#include "android_window_android.h"


namespace os
{


   class CLASS_DECL_BASE simple_ui :
      virtual public ::simple_ui::interaction
   {
   public:


      HINSTANCE                  m_hinstance;
      string                     m_strTitle;
      string                     m_strWindowClass;

      int                        m_w;
      int                        m_h;
      ::draw2d::dib_sp           m_dib;
      SIZE                       m_size;
      POINT                      m_pt;
      window_android             m_gdi;


      oswindow                   m_window;
      bool                       m_bShiftKey;
      rect                       m_rectDesktop;

      bool                       m_bNoDecorations;




      simple_ui(sp(base_application) papp);

      virtual ~simple_ui();

      //static LRESULT CALLBACK s_window_prodecure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

      virtual bool create_window(LPCRECT lprect);

      //virtual ATOM register_window_class(HINSTANCE hInstance);

      virtual bool prepare_window(LPCRECT lprect);

      virtual bool show_window(bool bShow = true);

      virtual bool destroy_window();

      virtual void client_to_screen(POINT * ppt);

      virtual void screen_to_client(POINT * ppt);

      //virtual LRESULT message_handler(UINT message, WPARAM wParam, LPARAM lParam);

      virtual void get_window_rect(RECT * prect);
      virtual void get_client_rect(RECT * prect);



      virtual bool on_key_down(uint32_t uiKey);
      virtual bool on_key_up(uint32_t uiKey);

      //virtual void on_windows_gdi_draw_framebuffer();

      virtual bool on_move(int32_t x, int32_t y);
      virtual bool on_size(int32_t cx, int32_t cy);

      virtual void set_capture();
      virtual void release_capture();


      virtual bool run_loop();

      bool move_window(int32_t x, int32_t y);
      bool set_window_pos(int32_t x, int32_t y, int32_t cx, int32_t cy, bool bShow);



   };




} // namespace os





CLASS_DECL_BASE LRESULT layered_window_nc_calc_size(WPARAM wparam, LPARAM lparam);



