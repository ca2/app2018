#include "framework.h"


namespace primitive
{


   memory_container ::memory_container(::aura::application * papp) :
      object(papp)
   {

//      m_spmemory = canew(memory(papp));

      m_pvppa = NULL;

   }


   memory_container ::memory_container(::aura::application * papp, void * pmemory, memory_size_t dwSize) :
      object(papp)
   {
      m_pvppa = NULL;
      m_spmemory = canew(memory(this, pmemory, dwSize));
   }


   memory_container ::memory_container(::aura::application * papp, memory_base * pmemory) :
      object(papp)
   {
      m_pvppa = NULL;
      m_spmemory = pmemory;
   }


   memory_container ::memory_container(const memory_container & container) :
      object(container)
   {
      m_pvppa = NULL;
      operator = (container);
   }


   memory_container ::   ~memory_container()
   {
      if(m_pvppa != NULL)
      {
         delete m_pvppa;
         m_pvppa = NULL;

      }
   }


   void memory_container ::allocate_add_up(memory_size_t dwAddUp)
   {
      allocate(this->get_size() + dwAddUp);
   }


   void memory_container::allocate(memory_size_t dwNewLength)
   {

      if(dwNewLength <= 0)
      {

         if(m_spmemory.is_set())
         {

            if(!m_spmemory->allocate(dwNewLength))
            {

               _throw(memory_exception(get_app()));

            }

         }

         return;
      }

      if(m_spmemory.is_null())
      {

         m_spmemory =canew(memory(this));

         if(m_spmemory.is_null())
         {

            _throw(memory_exception(get_app()));

         }

      }

      if(!m_spmemory->allocate(dwNewLength))
      {

         _throw(memory_exception(get_app()));

      }

   }


   void memory_container ::allocate_internal(memory_size_t dwNewLength)
   {
      if(m_spmemory.is_null())
      {
         m_spmemory = canew(memory(this));
         if(m_spmemory.is_null())
         {
            _throw(memory_exception(get_app()));
         }
      }
      m_spmemory->allocate_internal(dwNewLength);
   }


   memory_base * memory_container::get_memory()
   {

      if(m_spmemory.is_null())
      {
         m_spmemory = create_memory();
      }

      return m_spmemory;

   }


   void memory_container::set_memory(memory_base * pmemory)
   {

      if (m_spmemory == pmemory)
      {

         return;

      }

      m_spmemory = pmemory;

      ::aura::del(m_pvppa);

   }


   ::primitive::memory_base * memory_container::create_memory()
   {

      return canew(memory(this));

   }




   void memory_container ::read(memory_base * pmemory)
   {
      if(m_spmemory.is_null())
      {
         m_spmemory = canew(memory(this));
      }
      m_spmemory->copy_from(pmemory);
   }


   void memory_container ::io(stream & stream)
   {

      if(m_spmemory.is_null())
      {

         m_spmemory = canew(memory(this));

      }

      m_spmemory->io(stream);

   }


   void memory_container ::keep_pointer(void **ppvoid)
   {

      vppa().add(ppvoid);

   }


   void memory_container ::offset_kept_pointers(memory_offset_t iOffset)
   {

      if (m_pvppa == NULL)
      {

         return;

      }

      for(int32_t i = 0; i < m_pvppa->get_size(); i++)
      {

         *m_pvppa->element_at(i) = ((LPBYTE)*m_pvppa->element_at(i)) + iOffset;

      }

   }


   bool memory_container ::IsValid() const
   {

      return true;

   }


   void memory_container::copy_this(const memory_container & container)
   {

      if (container.m_spmemory.is_null())
      {

         m_spmemory.release();

      }
      else
      {

         m_spmemory = canew(memory(this));

         m_spmemory->copy_from(container.m_spmemory);

      }

      if (m_pvppa != NULL)
      {

         delete m_pvppa;

         m_pvppa = NULL;

      }

   }

   memory_container & memory_container ::operator =(const memory_container &container)
   {

      if (&container == this)
      {

         return *this;

      }

      object::copy_this(container);

      memory_container::copy_this(container);

      return *this;

   }


   bool memory_container ::attach(memory_base * pstorage)
   {

      m_spmemory = pstorage;

      return true;

   }


   memory_base * memory_container::detach()
   {

      return m_spmemory.detach();

   }


   memory * memory_container::get_primitive_memory()
   {

      if (m_spmemory.is_null())
         m_spmemory = canew(memory(get_app()));

      return m_spmemory->m_pprimitivememory;

   }


#if !defined(METROWIN)
   shared_memory *   memory_container::get_shared_memory()
   {
      return m_spmemory->m_psharedmemory;
   }
#endif

   virtual_memory *  memory_container::get_virtual_memory()
   {
      return m_spmemory->m_pvirtualmemory;
   }


   //memory *          memory_container::detach_primitive_memory()
   //{
   //   ::primitive::memory_base * pmemorybase = m_spmemory.detach();
   //   if(pmemorybase != NULL)
   //      return NULL;
   //   memory * pmemory = pmemorybase->m_pprimitivememory;
   //   if(pmemory != NULL)
   //   {
   //      return pmemory;
   //   }
   //   pmemory = new memory(*pmemorybase);
   //   delete pmemorybase;
   //   return pmemory;
   //}

//#if !defined(METROWIN)
//   shared_memory *   memory_container::detach_shared_memory()
//   {
//      ::primitive::memory_base * pmemorybase = m_spmemory.detach();
//      if(pmemorybase != NULL)
//         return NULL;
//      ::primitive::shared_memory * psharedmemory = pmemorybase->m_psharedmemory;
//      if(psharedmemory != NULL)
//      {
//         return psharedmemory;
//      }
//      psharedmemory = new ::primitive::shared_memory(*pmemorybase);
//      delete pmemorybase;
//      return psharedmemory;
//   }
//#endif
//
//   virtual_memory *  memory_container::detach_virtual_memory()
//   {
//      ::primitive::memory_base * pmemorybase = m_spmemory.detach();
//      if(pmemorybase != NULL)
//         return NULL;
//      ::primitive::virtual_memory * pvirtualmemory = pmemorybase->m_pvirtualmemory;
//      if(pvirtualmemory != NULL)
//      {
//         return pvirtualmemory;
//      }
//      pvirtualmemory = new ::primitive::virtual_memory(*pmemorybase);
//      delete pmemorybase;
//      return pvirtualmemory;
//   }


//   LPBYTE memory_container::detach_primitive_storage()
//   {
//      sp(memory) pmemory = detach_primitive_memory();
//      return pmemory->detach();
//   }
//
//
//   LPBYTE memory_container::detach_virtual_storage()
//   {
//      sp(::primitive::virtual_memory) pvirtualmemory = detach_virtual_memory();
//      return pvirtualmemory->detach();
//   }
//
//
//#if !defined(METROWIN)
//   HGLOBAL memory_container::detach_shared_storage()
//   {
//      sp(::primitive::shared_memory) psharedmemory = detach_shared_memory();
//      return psharedmemory->detach_shared_memory();
//   }
//#endif


   void memory_container::str(const string & str)
   {

      allocate(str.get_length());

      memcpy(get_data(), str, get_size());

   }


   pointer_array < void ** > & memory_container::vppa()
   {

      if(m_pvppa == NULL)
      {

         m_pvppa = new pointer_array < void ** > ();

      }

      return *m_pvppa;

   }


} // namespace primitive



