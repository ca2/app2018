#pragma once


inline strsize safe_strlen(void * p, strsize n)
{

   if (p == NULL)
   {

      return 0;

   }

   const char * psz = (const char *)p;

   strsize i = 0;

   for (; i < n; i++)
   {

      if (psz[i] == '\0')
      {

         break;

      }

   }

   return i;

}



class CLASS_DECL_AURA memory :
   virtual public ::primitive::memory_base
{
public:


   typedef memory data_type;


   bool        m_bAligned;


   memory(::aura::application * papp);
   memory(::aura::application * papp, bool bAligned);
   memory(const byte *,memory_size_t iCount);
   memory(const void *, memory_size_t iCount);
   memory(const memory_base & memorybase);
   memory(const memory & memory);
   memory(const char * psz);
   memory(primitive::memory_container * pmsc = NULL, memory_size_t dwAllocationAddUp = 0, UINT nAllocFlags = 0);
   memory(primitive::memory_container * pmsc, void * pMemory, memory_size_t dwSize);

   memory(manager * pmanager);
   memory(const memory & strSrc, manager * pmanager);
   memory(const memory * pmemory, manager * pmanager);
   memory(const byte * pchSrc, strsize nLength, manager * pmanager);
   memory(struct for_moving) {}

   virtual ~memory();


   //virtual ::object * clone() override;

   virtual LPBYTE impl_alloc(memory_size_t dwAllocation) override;
   virtual LPBYTE impl_realloc(void * pdata, memory_size_t dwAllocation) override;
   virtual void impl_free(LPBYTE pdata) override;


};



