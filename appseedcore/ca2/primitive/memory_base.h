#pragma once

namespace primitive
{

   class memory_container;


   class CLASS_DECL_ca memory_base :
      virtual public ex1::byte_serializable
   {
   public:


      memory_offset           m_iOffset;
      memory_size             m_cbStorage;
      memory_size             m_dwAllocation;
      memory_size             m_dwAllocationAddUp;

      memory_container *      m_pcontainer;

      mutex *                 m_pmutex;
      bool                    m_bLockMode;
      bool                    m_bLock;


      memory_base();
      virtual ~memory_base();

      memory_base & operator = (const memory_base & s);

      virtual void FullLoad(ex1::file &file);
      virtual void from_string(const wchar_t * pwsz);
      virtual void from_string(const char * psz);
      virtual void to_string(string & str);

      virtual void delete_begin(memory_size iSize);
      virtual void eat_begin(void * pdata, memory_size iSize);
      virtual void set_data(void * pdata, memory_size uiSize);
      virtual void copy_from(const memory_base * pstorage);
      virtual bool IsEnabled() const;
      virtual bool IsLocked() const;

      virtual void append(const void * pdata, memory_size iCount);
      virtual void assign(const void * pdata, memory_position iStart, memory_size iCount);
      virtual void append(memory_size iCount, unsigned char uch);
      virtual void assign(memory_size iCount, unsigned char uch);

      void _RemoveOffset();

      byte operator [] (int i) const;
      byte & operator [] (int i);

      operator const byte *() const;
      operator byte *();

      operator const void *() const;
      operator void *();

      virtual LPBYTE get_data() const;
      virtual LPBYTE internal_get_data() const;
      virtual memory_size get_size() const;

      virtual void To(string & str, memory_position iStart = 0, memory_position iEnd = -1);
      virtual void From(const char * psz);

      virtual void ToAsc(string & str);
      virtual void FromAsc(const char * psz);

      virtual bool operator ==(memory_base & s);

      virtual ::primitive::memory_size read(ex1::file & file);

      virtual void write(ex1::byte_output_stream & ostream);
      virtual void read(ex1::byte_input_stream & ostream);


      virtual bool allocate(memory_size dwNewLength);
      virtual void allocate_add_up(memory_size dwAddUp);
      virtual bool allocate_internal(memory_size dwNewLength);


   };


} // namespace primitive
