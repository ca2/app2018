#pragma once


class CLASS_DECL_ca2 ptr_array :
   virtual public comparable_array < void * >
{
public:

};




/////////////////////////////////////////////////////////////////////////////
// CTypedPtrArray<BASE_CLASS, TYPE>

template < class TYPE, class BASE_CLASS = ptr_array>
class typed_ptr_array : 
   public BASE_CLASS
{
public:
	// Accessing elements
	TYPE GetAt(INT_PTR nIndex) const
		{ return (TYPE)BASE_CLASS::GetAt(nIndex); }
	TYPE& ElementAt(INT_PTR nIndex)
		{ return (TYPE&)BASE_CLASS::ElementAt(nIndex); }
	void SetAt(INT_PTR nIndex, TYPE ptr)
		{ BASE_CLASS::SetAt(nIndex, ptr); }

	// Potentially growing the array
	void SetAtGrow(INT_PTR nIndex, TYPE newElement)
		{ BASE_CLASS::SetAtGrow(nIndex, newElement); }
	INT_PTR Add(TYPE newElement)
		{ return BASE_CLASS::Add(newElement); }
	INT_PTR Append(const typed_ptr_array<TYPE, BASE_CLASS>& src)
		{ return BASE_CLASS::Append(src); }
	void Copy(const typed_ptr_array<TYPE, BASE_CLASS>& src)
		{ BASE_CLASS::Copy(src); }

	// Operations that move elements around
	void InsertAt(INT_PTR nIndex, TYPE newElement, INT_PTR nCount = 1)
		{ BASE_CLASS::InsertAt(nIndex, newElement, nCount); }
	void InsertAt(INT_PTR nStartIndex, typed_ptr_array<BASE_CLASS, TYPE>* pNewArray)
		{ BASE_CLASS::InsertAt(nStartIndex, pNewArray); }

	// overloaded operator helpers
	TYPE operator[](INT_PTR nIndex) const
		{ return (TYPE)BASE_CLASS::operator[](nIndex); }
	TYPE& operator[](INT_PTR nIndex)
		{ return (TYPE&)BASE_CLASS::operator[](nIndex); }
};



