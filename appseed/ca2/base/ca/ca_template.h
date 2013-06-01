#pragma once

//#include "_flags.h"


// the two functions below are deprecated.  Use a constructor/destructor instead.
//#pragma deprecated( DestructElements )
//#pragma deprecated( ConstructElements )

template<class TYPE>
inline void CopyElements(TYPE* pDest, const TYPE* pSrc, ::count nCount)
{
   ENSURE(nCount == 0 || pDest != 0 && pSrc != 0);
   ASSERT(nCount == 0 ||
      __is_valid_address(pDest, (size_t)nCount * sizeof(TYPE)));
   ASSERT(nCount == 0 ||
      __is_valid_address(pSrc, (size_t)nCount * sizeof(TYPE)));

   // default is element-copy using assignment
   while (nCount--)
      *pDest++ = *pSrc++;
}
/*
template<class TYPE>
void SerializeElements(CArchive& ar, TYPE* pElements, ::count nCount)
{
   ENSURE(nCount == 0 || pElements != NULL);
   ASSERT(nCount == 0 ||
      __is_valid_address(pElements, (size_t)nCount * sizeof(TYPE)));

   // default is bit-wise read/write
   if (ar.IsStoring())
   {
     TYPE* pData;
     uint_ptr nElementsLeft;

     nElementsLeft = nCount;
     pData = pElements;
     while( nElementsLeft > 0 )
     {
       UINT nElementsToWrite;

       nElementsToWrite = UINT(__min(nElementsLeft, INT_MAX/sizeof(TYPE)));
       ar.write(pData, nElementsToWrite*sizeof(TYPE));
       nElementsLeft -= nElementsToWrite;
       pData += nElementsToWrite;
     }
   }
   else
   {
     TYPE* pData;
     uint_ptr nElementsLeft;

     nElementsLeft = nCount;
     pData = pElements;
     while( nElementsLeft > 0 )
     {
       UINT nElementsToRead;

       nElementsToRead = UINT(__min(nElementsLeft, INT_MAX/sizeof(TYPE)));
       ar.EnsureRead(pData, nElementsToRead*sizeof(TYPE));
       nElementsLeft -= nElementsToRead;
       pData += nElementsToRead;
     }
   }
}

template<class TYPE>
void SerializeElementsInsertExtract(CArchive& ar, TYPE* pElements, 
   ::count nCount)
{
   ENSURE(nCount == 0 || pElements != NULL);
   ASSERT((nCount == 0) || 
      (__is_valid_address(pElements, nCount*sizeof(TYPE))));

   if (nCount == 0 || pElements == NULL)
   {
      return;
   }

   if (ar.IsStoring())
   {
      for (; nCount--; ++pElements)
         ar << *pElements;
   }
   else
   {
      for (; nCount--; ++pElements)
         ar >> *pElements;
   }
}
*/

template<class TYPE>
void dump_elements(dump_context & dumpcontext, const TYPE* pElements, ::count nCount)
{
   ENSURE(nCount == 0 || pElements != NULL);
   ASSERT(nCount == 0 ||
      __is_valid_address(pElements, (size_t)nCount * sizeof(TYPE), FALSE));
#ifdef WINDOWS
   &dumpcontext; // not used
   pElements;  // not used
   nCount; // not used
#endif
   // default does nothing
}


// the two functions below are deprecated.  Use a constructor/destructor instead.
//#pragma deprecated( DestructElements )
//#pragma deprecated( ConstructElements )

/*
template<class TYPE>
void SerializeElements(CArchive& ar, TYPE* pElements, ::count nCount)
{
   ENSURE(nCount == 0 || pElements != NULL);
   ASSERT(nCount == 0 ||
      __is_valid_address(pElements, (size_t)nCount * sizeof(TYPE)));

   // default is bit-wise read/write
   if (ar.IsStoring())
   {
     TYPE* pData;
     uint_ptr nElementsLeft;

     nElementsLeft = nCount;
     pData = pElements;
     while( nElementsLeft > 0 )
     {
       UINT nElementsToWrite;

       nElementsToWrite = UINT(__min(nElementsLeft, INT_MAX/sizeof(TYPE)));
       ar.write(pData, nElementsToWrite*sizeof(TYPE));
       nElementsLeft -= nElementsToWrite;
       pData += nElementsToWrite;
     }
   }
   else
   {
     TYPE* pData;
     uint_ptr nElementsLeft;

     nElementsLeft = nCount;
     pData = pElements;
     while( nElementsLeft > 0 )
     {
       UINT nElementsToRead;

       nElementsToRead = UINT(__min(nElementsLeft, INT_MAX/sizeof(TYPE)));
       ar.EnsureRead(pData, nElementsToRead*sizeof(TYPE));
       nElementsLeft -= nElementsToRead;
       pData += nElementsToRead;
     }
   }
}

template<class TYPE>
void SerializeElementsInsertExtract(CArchive& ar, TYPE* pElements, 
   ::count nCount)
{
   ENSURE(nCount == 0 || pElements != NULL);
   ASSERT((nCount == 0) || 
      (__is_valid_address(pElements, nCount*sizeof(TYPE))));

   if (nCount == 0 || pElements == NULL)
   {
      return;
   }

   if (ar.IsStoring())
   {
      for (; nCount--; ++pElements)
         ar << *pElements;
   }
   else
   {
      for (; nCount--; ++pElements)
         ar >> *pElements;
   }
}
*/



template<class TYPE, class ARG_TYPE>
bool CompareElements(const TYPE* pElement1, const ARG_TYPE* pElement2)
{
	ENSURE(pElement1 != NULL && pElement2 != NULL);
	ASSERT(__is_valid_address(pElement1, sizeof(TYPE), FALSE));
	ASSERT(__is_valid_address(pElement2, sizeof(ARG_TYPE), FALSE));

	return *pElement1 == *pElement2;
}





