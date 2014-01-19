#include "framework.h"


cairo_surface_t *  cairo_keep::g_cairosurface = NULL;
cairo_t *  cairo_keep::g_cairo = NULL;


cairo_keep::cairo_keep(cairo_t * pdc, bool bSave)
{

   m_pdc = pdc;

   m_bSave = false;

   if(bSave)
      save();


}


cairo_keep::~cairo_keep()
{

   if(m_bSave)
   {

      restore();

   }

}



void cairo_keep::save()
{

   if(m_bSave)
      return;

   cairo_save(m_pdc);

   m_bSave = true;

}


void cairo_keep::restore()
{

   if(!m_bSave)
      return;

   cairo_restore(m_pdc);

   m_bSave = false;

}


void cairo_keep::pulse()
{

   save();

   restore();

}


