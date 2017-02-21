#pragma once





class CLASS_DECL_AURA memory :
   public ::primitive::memory_base
{
public:

   typedef memory data_type;

   bool m_bOwn;


   bool m_bAligned;

   memory();
   memory(const memory & strSrc);
   memory(const memory * pmemory);
   memory(const byte * pchSrc,strsize nLength);

   memory(::aura::application * papp);
   memory(::aura::application * papp, bool bAligned);
   memory(const byte *,memory_size_t iCount);
   memory(const void *, memory_size_t iCount);
   memory(const memory_base & memorybase);
   memory(const char * psz);
   memory(primitive::memory_container * pmsc = NULL, memory_size_t dwAllocationAddUp = 0, UINT nAllocFlags = 0);
   memory(primitive::memory_container * pmsc, void * pMemory, memory_size_t dwSize);
   memory(const memory & memory);

   virtual ~memory();


   //virtual ::object * clone() override;

   virtual LPBYTE impl_alloc(memory_size_t dwAllocation) override;
   virtual LPBYTE impl_realloc(void * pdata, memory_size_t dwAllocation) override;
   virtual void impl_free(LPBYTE pdata) override;


};





typedef stdstring < memory > bstring; // binary string (may contain nulls)


