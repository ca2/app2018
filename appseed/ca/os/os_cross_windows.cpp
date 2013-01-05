#include "framework.h"


#ifndef WINDOWS



static inline void *AllocateForBSTR(size_t cb)
{

   return ::malloc(cb);

}

static inline void FreeForBSTR(void *pv)
{

   ::free(pv);

}


static UINT MyStringLen(const wchar_t *s)
{
  UINT i;
  for (i = 0; s[i] != '\0'; i++);
  return i;
}

BSTR SysAllocStringByteLen(LPCSTR psz, UINT len)
{
  int32_t realLen = len + sizeof(UINT) + sizeof(OLECHAR) + sizeof(OLECHAR);
  void *p = AllocateForBSTR(realLen);
  if (p == 0)
    return 0;
  *(UINT *)p = len;
  BSTR bstr = (BSTR)((UINT *)p + 1);
  memmove(bstr, psz, len);
  Byte *pb = ((Byte *)bstr) + len;
  for (int32_t i = 0; i < sizeof(OLECHAR) * 2; i++)
    pb[i] = 0;
  return bstr;
}

BSTR SysAllocString(const OLECHAR *sz)
{
  if (sz == 0)
    return 0;
  UINT strLen = MyStringLen(sz);
  UINT len = (strLen + 1) * sizeof(OLECHAR);
  void *p = AllocateForBSTR(len + sizeof(UINT));
  if (p == 0)
    return 0;
  *(UINT *)p = strLen;
  BSTR bstr = (BSTR)((UINT *)p + 1);
  memmove(bstr, sz, len);
  return bstr;
}

void SysFreeString(BSTR bstr)
{
  if (bstr != 0)
    FreeForBSTR((UINT *)bstr - 1);
}

UINT SysStringByteLen(BSTR bstr)
{
  if (bstr == 0)
    return 0;
  return *((UINT *)bstr - 1);
}

UINT SysStringLen(BSTR bstr)
{
  return SysStringByteLen(bstr) / sizeof(OLECHAR);
}

HRESULT VariantClear(VARIANTARG *prop)
{
  if (prop->vt == VT_BSTR)
    SysFreeString(prop->bstrVal);
  prop->vt = VT_EMPTY;
  return S_OK;
}

HRESULT VariantCopy(VARIANTARG *dest, VARIANTARG *src)
{
  HRESULT res = ::VariantClear(dest);
  if (res != S_OK)
    return res;
  if (src->vt == VT_BSTR)
  {
    dest->bstrVal = SysAllocStringByteLen((LPCSTR)src->bstrVal,
        SysStringByteLen(src->bstrVal));
    if (dest->bstrVal == 0)
      return E_OUTOFMEMORY;
    dest->vt = VT_BSTR;
  }
  else
    *dest = *src;
  return S_OK;
}

LONG CompareFileTime(const FILETIME* ft1, const FILETIME* ft2)
{
  if (ft1->dwHighDateTime < ft2->dwHighDateTime) return -1;
  if (ft1->dwHighDateTime > ft2->dwHighDateTime) return 1;
  if (ft1->dwLowDateTime < ft2->dwLowDateTime) return -1;
  if (ft1->dwLowDateTime > ft2->dwLowDateTime) return 1;
  return 0;
}

uint32_t GetLastError()
{
  return 0;
}

#endif // !defined(WINDOWS)



#ifdef METROWIN

CLASS_DECL_ca BOOL WINAPI LockFile(HANDLE hfile, uint32_t dwLo, uint32_t dwHi, uint32_t dwCountLo, uint32_t dwCountHi);

CLASS_DECL_ca BOOL WINAPI LockFile(HANDLE hfile, uint32_t dwLo, uint32_t dwHi, uint32_t dwCountLo, uint32_t dwCountHi)
{
   OVERLAPPED ov;
   zero(&ov, sizeof(ov));
   ov.Offset = dwLo;
   ov.OffsetHigh = dwHi;
   return LockFileEx(hfile, LOCKFILE_FAIL_IMMEDIATELY, 0, dwCountLo, dwCountHi, &ov);

}

CLASS_DECL_ca BOOL WINAPI UnlockFile(HANDLE hfile, uint32_t dwLo, uint32_t dwHi, uint32_t dwCountLo, uint32_t dwCountHi)
{
   OVERLAPPED ov;
   zero(&ov, sizeof(ov));
   ov.Offset = dwLo;
   ov.OffsetHigh = dwHi;
   return UnlockFileEx(hfile, 0, dwCountLo, dwCountHi, &ov);

}


#endif