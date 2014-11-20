#include "framework.h"



//void itemswap(void * pswaparg, index i1, index i2);
//
//
//void itemswap(void * pswaparg, index i1, index i2)
//{
//   ptr_array < factory_item_base > * pitemptra = (ptr_array < factory_item_base > *) pswaparg;
//   pitemptra->swap(i1, i2);
//}

bool is_safe_set(void * p);

base_factory::base_factory(::aura::application * papp) :
   element(papp)
{
   m_pmutex = new mutex(papp);
   m_bSimpleFactoryRequest = false;
}

base_factory::~base_factory()
{

   single_lock sl(m_pmutex, TRUE);

   m_mapItem.remove_all();

   m_mapAllocator.remove_all();

}





void base_factory::discard(sp(element) pobject)
{
   single_lock sl(m_pmutex, TRUE);
   sp(factory_allocator) & pallocator = m_mapAllocator[typeid(*pobject).name()];
   sl.unlock();
   if(pallocator == NULL)
   {
      if(::is_debugger_attached())
         debug_break();
      return;
   }
   pallocator->discard(pobject);
}

bool is_safe_set(void * p)
{
   return ((int_ptr) p) >= 32768;

}

void base_factory::enable_simple_factory_request(bool bEnable)
{

   m_bSimpleFactoryRequest = bEnable;

   if(!bEnable)
   {

      single_lock sl(m_pmutex, TRUE);

      m_typeinfoptraSimpleFactoryRequest.remove_all();

   }

}







element * base_factory::create(::aura::application * papp, sp(type) info)
{

   if(info->m_spmutex.is_null())
   {
      info->m_spmutex = new mutex(papp);
   }

   single_lock slInfo(info->m_spmutex, TRUE);

   if(info->m_pfactoryitem != NULL)
   {

      if(m_bSimpleFactoryRequest)
         return info->m_pfactoryitem->create(papp);

      info->m_pfactoryitem.release();
   }

   single_lock sl(m_pmutex, TRUE);

   sp(factory_item_base) & pitem = m_mapItem[info->m_id];

   if(m_bSimpleFactoryRequest)
   {
      m_typeinfoptraSimpleFactoryRequest.add(info);
      info->m_pfactoryitem = pitem;
   }

   return pitem->create(papp);

}



element * base_factory::base_clone(sp(element) pobject)
{

   return typed_clone((id) typeid(*pobject).name(), pobject);

}


element * base_factory::typed_clone(id idType, sp(element) pobject)
{

   single_lock sl(m_pmutex,TRUE);

   if(pobject == NULL)
      return NULL;

   sp(factory_item_base) pitem = m_mapItem[idType];

   if(pitem.is_null())
      return NULL;

   return pitem->clone(pobject);

}


factory_item_base::~factory_item_base()
{

}

































CLASS_DECL_AURA mutex * g_pmutexFactory = NULL;





















CLASS_DECL_AURA bool safe_destroy_element(element * pelement)
{

   try
   {

      pelement->~element();

   }
   catch(...)
   {

      return false;

   }

   return true;

}


CLASS_DECL_AURA bool safe_free_memory(void * ptype)
{


   try
   {

      memory_free(ptype);
      
   }
   catch(...)
   {

      return false;

   }

   return true;

}






