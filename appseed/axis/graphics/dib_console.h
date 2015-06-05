#pragma once


class CLASS_DECL_AXIS dib_console:
   virtual public ::console::window,
   virtual public ::file::stream_buffer
{
public:

   ::visual::dib_sp m_dib;
   size m_sizeTile;
   size m_sizeWindow;
   int m_x;
   int m_y;
   int m_iColor;


   dib_console(::aura::application * papp,size sizeTile);


   virtual void SetWindowSize(int iHeight,int iWidth);
   virtual void SetCursorVisibility(bool show);
   virtual void SetCursorPosition(int y,int x);
   virtual void SetTextColor(int color);
   virtual void SetScreenColor(int color, int iLineStart = 0, int iLineCount = -1);
   virtual void write(const char * psz);
   virtual void write(const void * lpBuf,::primitive::memory_size nCount);

   virtual bool defer_write(char ch,int x,int y,int cx,int cy);

   static COLORREF console_COLORREF(int iColor);

};







