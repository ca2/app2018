//#include "framework.h"


namespace ace
{


   allocator::~allocator()
   {

   }


} // namespace ace

//root::root()
//{
//
//
//}
//
//
//root::root(::ace::application * papp)
//{
//
//
//}
//
//
//root::~root()
//{
//}



//
//void root::delete_this()
//{
//
//   // quite verisimple default implementation
//   delete this;
//
//}
//
//


//element::element()
//{
//
//   m_ulFlags            = (uint32_t) flag_auto_clean;
//   m_pfactoryitembase   = NULL;
//
//   // root like (Rute like, Thank you Rute and Inha and Lizir!!)
//   m_countReference  = 1;
//   m_bHeap           = false;
//   m_paceapp        = NULL;
//
//}
//
//
//element::element(const element & o)
//{
//
//   m_paceapp = o.m_paceapp;
//   m_ulFlags = o.m_ulFlags;
//   m_pfactoryitembase   = o.m_pfactoryitembase;
//
//   // root like (Rute like, Thank you Rute and Inha and Lizir!!)
//   m_bHeap = false;
//   m_countReference  = 1;
//
//}
//
//
//element::element(::ace::application * papp)
//{
//
//   m_paceapp = papp;
//   m_ulFlags = (uint32_t)flag_auto_clean;
//   m_pfactoryitembase   = NULL;
//
//   // root like (Rute like, Thank you Rute and Inha and Lizir!!)
//   m_countReference  = 1;
//   m_bHeap           = false;
//   m_paceapp        = papp;
//
//}
//
//
//element::~element()
//{
//
//}
//
//
///*sp(::ace::application) element::get_app() const
//{
//
//   return m_paceapp;
//
//}*/
//
//
//void element::set_app(::ace::application * papp)
//{
//
//   m_paceapp = papp;
//
//}
//
//
//void element::system(const char * pszProjectName)
//{
//
//   UNREFERENCED_PARAMETER(pszProjectName);
//
//}
//
//
//element & element::operator = (const element & o)
//{
//
//   if(this != &o)
//   {
//
//      m_ulFlags   = o.m_ulFlags;
//      m_paceapp      = o.m_paceapp;
//
//   }
//
//   return *this;
//
//}
//
//
//void element::delete_this()
//{
//
//   if(m_pfactoryitembase != NULL && m_pfactoryitembase->m_pallocator)
//   {
//
//      m_pfactoryitembase->m_pallocator->discard(this);
//
//   }
//   else if(m_ulFlags & flag_discard_to_factory)
//   {
//
//      m_paceapp->m_pacesystem->discard_to_factory(this);
//
//   }
//   else if(is_heap())
//   {
//
//      delete this;
//
//   }
//
//}
//
//
//element * element::clone()
//{
//
//   if(m_pfactoryitembase != NULL)
//      return m_pfactoryitembase->clone(this);
//
//   return NULL;
//
//}
//
//
//namespace ace
//{
//
//   
//   allocatorsp::allocatorsp(::ace::application * papp)
//   {
//
//      allocator * pallocator = canew(allocator());
//
//      pallocator->m_paceapp = papp;
//
//      smart_pointer < allocator >::operator = (pallocator);
//
//   }
//
//
//} // namespace ace
//
//
//
//
//
//
//
//
//
//
//