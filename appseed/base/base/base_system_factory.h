#pragma once


template < class T >
void base_factory::creatable(int32_t iCount, bool bOverwrite)
{

   if(bOverwrite || !is_set(System.type_info <  T  > ()->name()))
      set_at(System.type_info <  T  > ()->name(), new creatable_factory_item<T>(get_app(), get_allocator<T>(iCount)));

}


template < class T >
void base_factory::cloneable(int32_t iCount, bool bOverwrite)
{

   if(bOverwrite || !is_set(System.type_info <  T  > ()->name()))
      set_at(System.type_info <  T  > ()->name(), new cloneable_factory_item<T>(get_app(), get_allocator<T>(iCount)));

}

template < class T >
sp(factory_allocator) base_factory::get_allocator(int32_t iCount)
{

   sp(factory_allocator) pallocator = get_allocator(System.type_info <  T  > ()->name());

   if(pallocator != NULL)
      return pallocator;

   pallocator = canew(factory_allocator_impl < T > (get_app(), iCount));

   set_at(System.type_info <  T  > ()->name(), pallocator);

   return pallocator;

}
