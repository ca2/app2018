﻿#pragma once


#include "primitive_memory_base.h"


namespace primitive
{


   class shared_memory;
   class virtual_memory;


   class CLASS_DECL_AURA memory_container :
      virtual public ::object
   {
   protected:


      sp(memory_base)                        m_spmemory;
      pointer_array < void ** > *            m_pvppa;

   public:

      memory_container(::aura::application * papp);
      memory_container(::aura::application * papp, void * pMemory, memory_size_t dwSize);
      memory_container(::aura::application * papp, memory_base * pmemory);
      memory_container(const memory_container & container);
      virtual ~memory_container();




      void allocate_add_up(memory_size_t dwAddUp);
      void allocate(memory_size_t dwNewLength);
      void allocate_internal(memory_size_t dwNewLength);

      virtual memory_base * create_memory();
      memory_base * get_memory();
      void set_memory(memory_base * pmemory);

      memory_size_t get_size() const;

      memory_size_t get_size_raw() const;

      void from_string(const unichar * pwsz);
      void from_string(const char * psz);
      void from_string(const string & str);
      void from_string(const var & str);
      void to_string(string & str);
      string to_string();

      void read(memory_base *pmemorystorage);
      void read(::file::istream & is);

      void keep_pointer(void **ppvoid);
      void offset_kept_pointers(memory_offset_t iOffset);

      LPBYTE get_data();
      const LPBYTE get_data() const;

      LPBYTE get_data_raw();
      const LPBYTE get_data_raw() const;

      bool IsValid() const;

      void copy_this(const memory_container & container);

      memory_container & operator =(const memory_container &container);

      bool attach(memory_base *pstorage);

      memory_base * detach();

      virtual memory *          get_primitive_memory();
#if !defined(METROWIN)
      virtual shared_memory *   get_shared_memory();
#endif
      virtual virtual_memory *  get_virtual_memory();

//      virtual memory *          detach_primitive_memory();
//#if !defined(METROWIN)
//      virtual shared_memory *   detach_shared_memory();
//#endif
//      virtual virtual_memory *  detach_virtual_memory();
//
//      virtual LPBYTE            detach_primitive_storage();
//#if !defined(METROWIN)
//      virtual HGLOBAL           detach_shared_storage();
//#endif
//      virtual LPBYTE            detach_virtual_storage();

      string str() const;
      void str(const string & str);

   protected:

      pointer_array < void ** > & vppa();

   };

   inline LPBYTE memory_container::get_data_raw()
   {

      return m_spmemory->get_data();

   }

   inline const LPBYTE memory_container::get_data_raw() const
   {

      return m_spmemory->get_data();

   }

   inline memory_size_t memory_container::get_size_raw() const
   {
      return m_spmemory->get_size();
   }


   inline LPBYTE memory_container ::get_data()
   {

      return m_spmemory.is_null() ? 0 : get_data_raw();

   }

   inline const LPBYTE memory_container ::get_data() const
   {

      return m_spmemory.is_null() ? 0 : get_data_raw();

   }

   inline memory_size_t memory_container ::get_size() const
   {
      return m_spmemory.is_null() ? 0 : get_size_raw();
   }


   inline void memory_container ::from_string(const unichar * pwsz)
   {

      if(m_spmemory.is_null())
         m_spmemory = create_memory();

      m_spmemory->from_string(pwsz);

   }


   inline void memory_container ::from_string(const char * psz)
   {

      if(m_spmemory.is_null())
         m_spmemory = create_memory();

      m_spmemory->from_string(psz);

   }

   inline void memory_container ::from_string(const string & str)
   {

      if(m_spmemory.is_null())
         m_spmemory = create_memory();

      m_spmemory->from_string(str);

   }

   inline void memory_container ::from_string(const var & var)
   {

      if(m_spmemory.is_null())
         m_spmemory = create_memory();

      m_spmemory->from_string(var);

   }

   inline void memory_container ::to_string(string & str)
   {

      if (m_spmemory.is_null())
      {

         str.Empty();

      }
      else
      {

         m_spmemory->to_string(str);

      }

   }

   inline string memory_container::to_string()
   {

      if (m_spmemory.is_null())
      {

         return "";

      }
      else
      {

         return m_spmemory->to_string();

      }

   }

   inline string memory_container::str() const
   {

      return string((const char *) get_data(), get_size());

   }


} // namespace primitive



