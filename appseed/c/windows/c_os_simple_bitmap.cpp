#include "framework.h"
#undef new
#include <GdiPlus.h>


simple_bitmap::simple_bitmap()
{
   
   m_pbitmap = NULL;

}

simple_bitmap::~simple_bitmap()
{
   
   if(m_pbitmap != NULL)
   {

      destroy();

   }

}

bool simple_bitmap::create(int cx, int cy, simple_graphics & g, COLORREF ** ppdata)
{

   if(m_pbitmap != NULL)
   {
      
      destroy();

   }

   m_size.cx = abs(cx);
   m_size.cy = abs(cy);

   m_memory.allocate(4 * m_size.cx * m_size.cy);

   if(m_memory.get_data() == NULL)
   {
      m_size.cx = 0;
      m_size.cy = 0;
      return false;
   }

   m_pbitmap = new Gdiplus::Bitmap(m_size.cx, m_size.cy, m_size.cx * 4, PixelFormat32bppARGB, (BYTE *) m_memory.get_data());

   if(m_pbitmap == NULL)
   {
      m_memory.allocate(0);
      m_size.cx = 0;
      m_size.cy = 0;
      return FALSE;
   }

   if(ppdata != NULL)
   {
      *ppdata = (COLORREF *) m_memory.get_data(); 
   }

   return true;

}


bool simple_bitmap::create_from_data(int cx, int cy, COLORREF * pdata, simple_graphics & g)
{

   if(m_pbitmap != NULL)
   {
      
      destroy();

   }

   m_size.cx = abs(cx);
   m_size.cy = abs(cy);

   m_memory.attach(pdata, m_size.cx * m_size.cy * 4);

   if(m_memory.get_data() == NULL)
   {
      m_size.cx = 0;
      m_size.cy = 0;
      return false;
   }

   m_pbitmap = new Gdiplus::Bitmap(m_size.cx, m_size.cy, m_size.cx * 4, PixelFormat32bppARGB, (BYTE *) m_memory.get_data());

   if(m_pbitmap == NULL)
   {
      m_memory.allocate(0);
      m_size.cx = 0;
      m_size.cy = 0;
      return FALSE;
   }

   return true;

}



bool simple_bitmap::destroy()
{
   
   if(m_hbitmap == NULL)
      return true;


   bool bOk = ::DeleteObject(m_hbitmap) != FALSE;

   m_hbitmap = NULL;

   if(!bOk)
      return false;

   return true;

}
