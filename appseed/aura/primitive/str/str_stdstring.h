#pragma once

class string;
struct for_moving
{
};


template < class T, typename C >
void std_string_assign(T & t,const C * psz);

template < class T >
void std_string_bassign(T & t,const byte * psz, strsize nsize);

//template < class BASE >
//class stdstring :
//   public BASE
//{
//public:
//
//
//   stdstring(typename BASE::manager * pstringmanager = NULL) : BASE(pstringmanager) {}
//   stdstring(const BASE & strSrc,typename BASE::manager * pstringmanager = NULL) : BASE(strSrc, pstringmanager) {}
//   stdstring(const char * pszSrc,typename BASE::manager * pstringmanager = NULL): BASE(pstringmanager) { std_string_assign(*this, pszSrc); }
//   stdstring(strsize nsize, const byte * pszSrc,typename BASE::manager * pstringmanager = NULL): BASE(pstringmanager) { std_string_bassign(*this,pszSrc, nsize); }
//   stdstring(const unichar * pszSrc,typename BASE::manager * pstringmanager = NULL): BASE(pstringmanager) { std_string_assign(*this,pszSrc); }
//   stdstring(const typename BASE::value_type * pchSrc,strsize nLength,typename BASE::manager * pstringmanager = NULL): BASE(pchSrc,nLength,pstringmanager) {}
//   stdstring(const typename BASE::data_type * pdata,typename BASE::manager * pstringmanager = NULL): BASE(pdata,pstringmanager) {}
//   stdstring(for_moving):
//      BASE(for_moving())
//   {
//   }
//   stdstring(stdstring && s) :
//      BASE(::move(s))
//   {
//   }
//   ~stdstring() NOTHROW{}
//
//
//   strsize copy(typename BASE::value_type * s,strsize len,strsize pos = 0) const;
//
//};
//
//







