

CLASS_DECL_AURA void * processor_cache_oriented_memory_alloc(size_t s)
{

#if defined(_M_IX86)

   return x86_cache_oriented_memory_alloc(s);

#else

   return memory_alloc(s);

#endif

}


CLASS_DECL_AURA void processor_cache_oriented_memory_free(void * p)
{

#if defined(_M_IX86)

   x86_cache_oriented_memory_free(p);

#else

   memory_free(s);

#endif

}


CLASS_DECL_AURA void processor_cache_oriented_memory_reset()
{

#if defined(_M_IX86)

   x86_cache_oriented_memory_reset();

#endif

}


CLASS_DECL_AURA int_bool processor_cache_oriented_set_thread_memory_pool(int iPoolIndex)
{

#if defined(_M_IX86)

   return x86_cache_oriented_set_thread_memory_pool(iPoolIndex);

#else

   return true;

#endif

}

CLASS_DECL_AURA  void processor_cache_oriented_destroy_all_memory_pools()
{

#if defined(_M_IX86)

   x86_cache_oriented_destroy_all_memory_pools();

#endif

}



