#pragma once


#include "aura/graphics/graphics_window_graphics.h"







class CLASS_DECL_AURA window_buffer :
   virtual public window_graphics
{
protected:


   COLORREF *                    m_pcolorref;
   ::visual::dib_sp              m_spdibBuffer;


public:


   bool                          m_bDibIsHostingBuffer;
   oswindow                      m_hwnd;
   file_memory_map               m_memorymap;


   window_buffer(::aura::application * papp);
   virtual ~window_buffer();


   virtual void update_window() override;
   virtual void update_window(::draw2d::dib * pdib);

   virtual bool is_buffer_ok() override;

   ::draw2d::graphics * on_begin_draw() override;

   ::draw2d::dib_sp & get_buffer();

   virtual void defer_prepare_ipc_copy_();
   virtual void ipc_copy(int cx, int cy, COLORREF * pcolorref, int iScan);

   void on_create_window(::user::interaction_impl * pimpl) override;

};




class CLASS_DECL_AURA window_double_buffer :
   virtual public window_buffer
{
public:

   mutex                         m_mutex2;
   ::visual::dib_sp              m_spdibBuffer2;


   window_double_buffer(::aura::application * papp);
   virtual ~window_double_buffer();


   virtual void update_window();




};


#ifdef WINDOWSEX

#include "aura/aura/os/windows/windows_window_buffer.h"

#endif


