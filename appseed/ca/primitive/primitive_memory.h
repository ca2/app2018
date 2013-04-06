#pragma once





namespace primitive
{


   class CLASS_DECL_ca memory :
      public memory_base
   {
   public:


      memory(sp(::ca::application) papp);
      memory(const void *, memory_size iCount);
      memory(const memory_base & memorybase);
      memory(const memory & memory);
      memory(const simple_memory & memory);
      memory(const char * psz);
      memory(primitive::memory_container * pmsc = ::null(), memory_size dwAllocationAddUp = 4096, UINT nAllocFlags = 0);
      memory(primitive::memory_container * pmsc, void * pMemory, memory_size dwSize);
      virtual ~memory();


      LPBYTE detach();


      virtual bool allocate_internal(memory_size dwNewLength);
      void free_data();


   };


} // namespace primitive


