#pragma once


class CLASS_DECL_c wstring_data
{
protected:


   friend class verisimple_wstring;
   friend class wstringtou;


   static wstring_data g_nil;


public:

   
   int            m_iAllocation; // in chars "with null characters"
   int            m_iLength; // in chars without null character
   wchar_t        m_sz[1];


   // nil constructor
   inline wstring_data()
   {
      m_iAllocation  = 0;
      m_iLength      = 0;
      m_sz[0]        = L'\0';
   }


   inline static wchar_t * alloc(int iCount)
   {
      wstring_data * pdata = (wstring_data *) ca2_alloc((iCount * sizeof(wchar_t)) + sizeof(int) + sizeof(int));
      pdata->m_iAllocation = iCount;
      pdata->m_iLength = 0;
      pdata->m_sz[0] = L'\0';
      return *pdata;
   }

   inline static void free(wchar_t * pwsz)
   {
      wstring_data * pdata = (wstring_data *) (((byte *) pwsz) - sizeof(int) - sizeof(int));
      if(pdata->m_iAllocation <= 0)
         return;
      ca2_free(pdata);
   }

   inline operator const wchar_t * () const { return (wchar_t *) &m_sz; }
   inline operator const wchar_t * () { return (wchar_t *) &m_sz; }

   inline operator wchar_t * () const { return (wchar_t *) &m_sz; }
   inline operator wchar_t * () { return (wchar_t *) &m_sz; }

};


class CLASS_DECL_c verisimple_wstring
{
protected:


   friend class wstringtou;

   // it is and should be really a pointer to the m_pwsz of a wstring_data alloced in heap
   // better always use wstring_data::alloc and wstring_data::free
   wchar_t * m_pwsz;  

   static verisimple_wstring g_nil;

   inline wstring_data * get_data()
   {
      return (wstring_data *)(((byte *) m_pwsz) - sizeof(int) - sizeof(int));
   }

   inline const wstring_data * get_data() const
   {
      return (wstring_data *)(((byte *) m_pwsz) - sizeof(int) - sizeof(int));
   }

public:
   


   verisimple_wstring();
   verisimple_wstring(const wchar_t * pwsz);
   verisimple_wstring(const wchar_t * pwsz, int iCount);
   verisimple_wstring(const verisimple_wstring & wstr);
   inline verisimple_wstring(wstring_data * pdata)
   {
      m_pwsz = *pdata;
   }
   ~verisimple_wstring();


   inline void attach(wstring_data * pdata)
   {
      if(m_pwsz != NULL)
      {
         wstring_data::free(m_pwsz);
      }
      m_pwsz = *pdata;
   }

   inline wstring_data * detach()
   {
      wstring_data * pdata = get_data();
      m_pwsz = *verisimple_wstring::g_nil.get_data();
      return pdata;
   }


   verisimple_wstring & operator = (const verisimple_wstring & wstr);
   verisimple_wstring & operator = (const wchar_t * pwsz);


   inline operator const wchar_t * () const { return get_data()->m_iAllocation <= 0 ? NULL : m_pwsz; }
   inline operator const wchar_t * () { return get_data()->m_iAllocation <= 0 ? NULL : m_pwsz; }

   inline operator wchar_t * () const { return get_data()->m_iAllocation <= 0 ? NULL : m_pwsz; }
   inline operator wchar_t * () { return get_data()->m_iAllocation <= 0 ? NULL : m_pwsz; }

   inline wchar_t operator [] (int iIndex) const
   {
      return m_pwsz[iIndex];
   }

   inline wchar_t & operator [] (int iIndex)
   {
      return m_pwsz[iIndex];
   }

   wchar_t * alloc(int iCount);

   inline count get_length() const
   {
      return get_data()->m_iLength;
   }

   inline count length() const
   {
      return get_data()->m_iLength;
   }

   inline count size() const
   {
      return get_data()->m_iLength;
   }

   inline count storage_size() const
   {
      return get_data()->m_iAllocation;
   }

   inline count allocation_size() const
   {
      return get_data()->m_iAllocation;
   }

   inline count get_storage_size() const
   {
      return get_data()->m_iAllocation;
   }

   inline count get_allocation_size() const
   {
      return get_data()->m_iAllocation;
   }

   inline int verisimple_wstring::Compare(const wchar_t * psz) const
   {
      return wcscmp_dup(m_pwsz, psz);
   }
   
   inline int verisimple_wstring::Compare(const verisimple_wstring &str) const
   {
      return wcscmp_dup(m_pwsz, str);
   }

   inline int verisimple_wstring::CompareNoCase(const wchar_t * psz) const
   {
      return wcsicmp_dup(m_pwsz, psz);
   }
   
   inline int verisimple_wstring::CompareNoCase(const verisimple_wstring &str) const
   {
      return wcsicmp_dup(m_pwsz, str);
   }


   inline bool is_empty() const
   {
      return get_data()->m_iLength <= 0 || *m_pwsz == L'\0';
   }


   inline void set_length(int iLength)
   {

      if(iLength < get_data()->m_iAllocation)
      {
         get_data()->m_iLength = iLength;
      }

   }

   inline void release_buffer()
   {

      set_length(wcslen_dup(m_pwsz));

   }


};


typedef verisimple_wstring wstring;


CLASS_DECL_c verisimple_wstring operator + (const verisimple_wstring & wstr1, const verisimple_wstring & wstr2);
CLASS_DECL_c verisimple_wstring operator + (const verisimple_wstring & str, const wchar_t * psz);
CLASS_DECL_c verisimple_wstring operator + (const wchar_t * psz, const verisimple_wstring & str);

CLASS_DECL_c wstring gen_utf8_to_16(const char * psz);

