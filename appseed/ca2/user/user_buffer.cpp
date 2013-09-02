#include "framework.h"

namespace user
{

   buffer::buffer(sp(::application) papp) :
      element(papp),
      m_spdib(allocer())
   {
   }

   buffer::~buffer()
   {
      clear();
   }

   bool buffer::clear()
   {
      m_spdib->Destroy();
      return true;
   }

   bool buffer::UpdateBuffer(size size)
   {
      return UpdateBuffer(size.cx, size.cy);
   }

   bool buffer::UpdateBuffer(class point point)
   {
      return UpdateBuffer(point.x, point.y);
   }

   bool buffer::UpdateBuffer(int32_t cx, int32_t cy)
   {
      m_size.cx = cy;
      m_size.cy = cy;
      if(!m_spdib->create(cx, cy))
         return false;
      m_spdib->get_graphics()->SetViewportOrg(
         0,
         0);
      //m_spdib->get_graphics()->SelectClipRgn(NULL);
      return true;
   }

   critical_section * buffer::GetSemaphore()
   {
      return &m_semaphoreBuffer;
   }

   ::draw2d::graphics * buffer::GetBuffer()
   {
      return m_spdib->get_graphics();
   }

   void buffer::InstallMessageHandling(::ca2::message::dispatch *pinterface)
   {
      UNREFERENCED_PARAMETER(pinterface);
   }

   void buffer::BitBlt(::draw2d::graphics * pdc)
   {
      pdc->BitBlt(
         0, 0, 
         m_spdib->cx,
         m_spdib->cy,
         GetBuffer(),
         0, 0, 
         SRCCOPY);
   }

   void buffer::BitBlt(LPRECT lprect, ::draw2d::graphics * pdc)
   {
      pdc->BitBlt(
         lprect->left, lprect->top, 
         lprect->right - lprect->left,
         lprect->bottom - lprect->top,
         GetBuffer(),
         lprect->left, lprect->top, 
         SRCCOPY);
   }

} // namespace user

