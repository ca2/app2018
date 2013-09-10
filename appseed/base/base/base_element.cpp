#include "framework.h"


allocator::~allocator()
{

}


element::element()
{

   m_ulFlags            = (uint32_t) flag_auto_clean;
   m_pfactoryitembase   = NULL;

}


element::element(const element & o) :
   m_pbaseapp(o.m_pbaseapp)
{

   m_ulFlags            = o.m_ulFlags;
   m_pfactoryitembase   = NULL;

}


element::element(sp(base_application) papp) :
   m_pbaseapp(papp)
{

   m_ulFlags            = (uint32_t) flag_auto_clean;
   m_pfactoryitembase   = NULL;

}


element::~element()
{

}


/*sp(base_application) element::get_app() const
{

   return m_pbaseapp;

}*/


void element::set_app(sp(base_application) papp)
{

   m_pbaseapp = papp;

}


void element::system(const char * pszProjectName)
{

   UNREFERENCED_PARAMETER(pszProjectName);

}


element & element::operator = (const element & o)
{

   if(this != &o)
   {

      m_ulFlags   = o.m_ulFlags;
      m_pbaseapp      = o.m_pbaseapp;

   }

   return *this;

}


void element::delete_this()
{

   if(m_pfactoryitembase != NULL && m_pfactoryitembase->m_pallocator)
   {

      m_pfactoryitembase->m_pallocator->discard(this);

   }
   else if(m_ulFlags & flag_discard_to_factory)
   {

      m_pbaseapp->m_pbasesystem->discard_to_factory(this);

   }
   else if(is_heap())
   {

      delete this;

   }

}


sp(base_element) element::clone()
{

   if(m_pfactoryitembase != NULL)
      return m_pfactoryitembase->clone(this);
   else
      return m_pbaseapp->m_pbasesystem->clone(this);

}


allocatorsp::allocatorsp(sp(base_application) papp)
{

   sp(allocator) spallocator = canew(allocator());

   spallocator->m_pbaseapp = papp;

   smart_pointer < allocator >::operator = (spallocator);

}


