#pragma once


#include "axis/graphics/visual/visual.h"
#include "base/graphics/graphics_window_graphics.h"


class CLASS_DECL_BASE window_buffer :
   virtual public window_graphics
{
protected:
   ::visual::dib_sp              m_spdibBuffer;
public:


   HANDLE                        m_hMapFile;
   LPCTSTR                       m_pBuf;
   oswindow                      m_hwnd;


   window_buffer(::aura::application * papp);
   virtual ~window_buffer();


   virtual void update_window();
   virtual void update_window(::draw2d::dib * pdib);


   ::draw2d::graphics * on_begin_draw();
   
   ::draw2d::dib_sp & get_buffer();
   
};




class CLASS_DECL_BASE window_double_buffer :
   virtual public window_buffer
{
public:

   mutex                         m_mutex2;
   ::visual::dib_sp              m_spdibBuffer2;


   window_double_buffer(::aura::application * papp);
   virtual ~window_double_buffer();


   virtual void update_window();




};
