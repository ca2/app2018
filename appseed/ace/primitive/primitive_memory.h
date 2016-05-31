#pragma once





class CLASS_DECL_ACE memory :
   public ::primitive::memory_base
{
public:

   typedef memory data_type;


   bool m_bAligned;

   memory(manager * pmanager);
   memory(const memory & strSrc,manager * pmanager);
   memory(const memory * pmemory,manager * pmanager);
   memory(const byte * pchSrc,strsize nLength,manager * pmanager);

   memory(::ace::application * papp);
   memory(::ace::application * papp, bool bAligned);
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

