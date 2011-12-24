#include "StdAfx.h"


wstring_data wstring_data::g_nil;


verisimple_wstring verisimple_wstring::g_nil(&wstring_data::g_nil);


verisimple_wstring::verisimple_wstring()
{

   m_pwsz         = *wstring::g_nil.get_data();

}

verisimple_wstring::verisimple_wstring(const wchar_t * pwsz, int iCount)
{
   if(pwsz == NULL)
   {

      m_pwsz         = *wstring::g_nil.get_data();

   }
   else
   {

      m_pwsz = (wchar_t *) wstring_data::alloc(iCount + 1);
      get_data()->m_iLength = iCount;
      memcpy_dup(m_pwsz, pwsz, get_data()->m_iLength * sizeof(wchar_t));
      m_pwsz[get_data()->m_iLength] = L'\0';

   }
}


verisimple_wstring::verisimple_wstring(const wchar_t * pwsz)
{

   if(pwsz == NULL)
   {

      m_pwsz         = *wstring::g_nil.get_data();

   }
   else
   {

      int iLen         = wcslen_dup(pwsz);
      m_pwsz = (wchar_t *) wstring_data::alloc(iLen + 1);
      get_data()->m_iLength = iLen;
      memcpy_dup(m_pwsz, pwsz, get_data()->m_iLength * sizeof(wchar_t));
      m_pwsz[get_data()->m_iLength] = L'\0';

   }
}

verisimple_wstring::verisimple_wstring(const verisimple_wstring & wstr)
{

   m_pwsz         = *wstring::g_nil.get_data();

   operator = (wstr);

}

verisimple_wstring::~verisimple_wstring()
{

   wstring_data::free(m_pwsz);

}

wchar_t * verisimple_wstring::alloc(int iCount)
{

   if(iCount < get_data()->m_iAllocation)
      return m_pwsz;

   wstring_data::free(m_pwsz);

   m_pwsz            = (wchar_t *) wstring_data::alloc(iCount);

   return m_pwsz;

}


verisimple_wstring & verisimple_wstring::operator = (const verisimple_wstring & wstr)
{
   
   if(this != &wstr)
   {

      if(get_data()->m_iAllocation >= (wstr.length() + 1))
      {

         memcpy_dup(m_pwsz, wstr.m_pwsz, (wstr.length() + 1) * sizeof(wchar_t));

      }
      else
      {

         alloc(wstr.storage_size());
         memcpy_dup(m_pwsz, wstr.m_pwsz, storage_size() * sizeof(wchar_t));

      }

      set_length(wstr.get_length());
      

   }

   return *this;

}
   
verisimple_wstring & verisimple_wstring::operator = (const wchar_t * pwsz)
{
   
   if(m_pwsz != pwsz)
   {

      int iLen = wcslen_dup(pwsz);

      if(storage_size() >= (iLen + 1))
      {

         memcpy_dup(m_pwsz, pwsz, (iLen + 1) * sizeof(wchar_t));


      }
      else
      {

         alloc(iLen + 1);
         memcpy_dup(m_pwsz, pwsz, storage_size() * sizeof(wchar_t));
      }

      set_length(iLen);

   }

   return *this;

}


verisimple_wstring operator + (const verisimple_wstring & wstr1, const verisimple_wstring & wstr2)
{
   int iLen1 = wstr1.length();
   int iLen2 = wstr2.length();
   verisimple_wstring wstrRet;
   wstrRet.alloc(iLen1 + iLen2 + 1);
   wcscpy_dup(wstrRet, wstr1);
   wcscat_dup(wstrRet, wstr2);
   wstrRet.set_length(iLen1 + iLen2);
   return wstrRet;
}


verisimple_wstring operator + (const verisimple_wstring & wstr, const wchar_t * wpsz)
{
   return wstr + verisimple_wstring(wpsz);
}

verisimple_wstring operator + (const wchar_t * wpsz, const verisimple_wstring & wstr)
{
   return verisimple_wstring(wpsz) + wstr;
}


CLASS_DECL_c wstring gen_utf8_to_16(const char * psz)
{
   
   wstring wstr;
   int iLen = utf16_len(psz);
   if(iLen < 0)
      return wstr;
   wstr.alloc(iLen + 1);
   utf8_to_16(wstr, psz);
   wstr.set_length(iLen);
   return wstr;

}






template<> CLASS_DECL_c BOOL simple_CompareElements(const wstring * pElement1, const wstring * pElement2)
{
   return pElement1->Compare(*pElement2);
}


