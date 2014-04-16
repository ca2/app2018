// bitset standard header
#pragma once

/*namespace core
{
   class input_stream;
   class output_stream;
}*/

		// TEMPLATE CLASS _Bitset_base
template<int32_t>
	struct _Bitset_base
	{	// default element size
	typedef uint32_t _Ty;
	};

template<>
	struct _Bitset_base <8>
	{	// eight-byte bitset
	typedef uint64_t _Ty;
	};

		// TEMPLATE CLASS bitset
template<size_t _Bits>
	class bitset
		: public _Bitset_base <_Bits <= 8 ? 1
			: _Bits <= 16 ? 2
			: _Bits <= 32 ? 4
			: 8>,
virtual public ::object
{	// store fixed-length sequence of Boolean elements
public:


	enum {_EEN_BITS = _Bits};	// helper for expression evaluator


	typedef _Bitset_base <_Bits <= 8 ? 1
   : _Bits <= 16 ? 2
   : _Bits <= 32 ? 4
   : 8> _Mybase;
	typedef typename	// sic
   _Mybase::_Ty _Ty;

	typedef bool element_type;	// retained

   // CLASS reference
	class reference
   {	// proxy for an element
		friend class bitset<_Bits>;

	public:
		reference& operator=(bool _Val)
      {	// assign Boolean to element
			_Pbitset->set(_Mypos, _Val);
			return (*this);
      }

		reference& operator=(const reference& _Bitref)
      {	// assign reference to element
			_Pbitset->set(_Mypos, bool(_Bitref));
			return (*this);
      }

		reference& flip()
      {	// complement stored element
			_Pbitset->flip(_Mypos);
			return (*this);
      }

		bool operator~() const
      {	// return complemented element
			return (!_Pbitset->test(_Mypos));
      }

		operator bool() const
      {	// return element
			return (_Pbitset->test(_Mypos));
      }

	private:
		reference(bitset<_Bits>& _Bitset, size_t _Pos)
      : _Pbitset(&_Bitset), _Mypos(_Pos)
      {	// construct from bitset reference and position
      }

		bitset<_Bits> *_Pbitset;	// pointer to the bitset
		size_t _Mypos;	// position of element in bitset
   };


private:

   enum
   {	// parameters for packing bits into words
		_Bitsperword = (int32_t)(CHAR_BIT * sizeof (_Ty)),	// bits in each uint16_t
		_Words = (int32_t)(_Bits == 0
                     ? 0 : (_Bits - 1) / _Bitsperword)};	// NB: number of words - 1


	_Ty _Array[_Words + 1];	// the set of bits


public:


	bool at(size_t _Pos) const	// retained
		{	// subscript nonmutable sequence with checking
		return (test(_Pos));
		}

	reference at(size_t _Pos)	// retained
		{	// subscript mutable sequence with checking
		return (reference(*this, _Pos));
		}

	bool operator[](size_t _Pos) const
		{	// subscript nonmutable sequence
		return (test(_Pos));
		}

	reference operator[](size_t _Pos)
		{	// subscript mutable sequence
 #if _ITERATOR_DEBUG_LEVEL == 2
		if (_Bits <= _Pos)
			DEBUG_ERROR("bitset index outside range");

 #elif _ITERATOR_DEBUG_LEVEL == 1
		_SCL_SECURE_VALIDATE_RANGE(_Pos < _Bits);
 #endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		return (reference(*this, _Pos));
		}

	bitset()
		{	// construct with all false values
		_Tidy();
		}

 #if _HAS_CPP0X
	bitset(int32_t _Ival)
		{	// construct from bits in int32_t
		uint32_t _Val = (uint32_t)_Ival;
		_Tidy();
		for (size_t _Pos = 0; _Val != 0 && _Pos < _Bits; _Val >>= 1, ++_Pos)
			if (_Val & 1)
				set(_Pos);
		}

	bitset(_ULonglong _Val)

 #else /* _HAS_CPP0X */
	bitset(uint64_t _Val)
 #endif /* _HAS_CPP0X */

		{	// construct from bits in uint64_t
		_Tidy();
		for (size_t _Pos = 0; _Val != 0 && _Pos < _Bits; _Val >>= 1, ++_Pos)
			if (_Val & 1)
				set(_Pos);
		}

 #define _BITSET_SIZE_TYPE	\
	strsize

	explicit bitset(const string & _Str, _BITSET_SIZE_TYPE _Pos = 0)
	{	// construct from [_Pos, ...) elements in string
		_Construct(_Str, _Pos, -1, (char)'0', (char)'1');
	}

	explicit bitset(const string & _Str, _BITSET_SIZE_TYPE _Pos, _BITSET_SIZE_TYPE _Count, char _E0 = (char)'0', char _E1 = (char)'1')
	{	// construct from [_Pos, _Pos + _Count) elements in string
	_Construct(_Str, _Pos, _Count, _E0, _E1);
	}

	explicit bitset(const char *_Ptr)
	{	// initialize from NTBS
		string _Str(_Ptr);
		_Construct(_Str, 0, _Str.size(), '0', '1');
	}

   void _Construct(const string & _Str, _BITSET_SIZE_TYPE _Pos, _BITSET_SIZE_TYPE _Count, char _E0, char _E1)
   {	// initialize from [_Pos, _Pos + _Count) elements in string
		strsize _Num;
		if (_Str.size() < _Pos)
			_Xran();	// _Pos off end
		if (_Str.size() - _Pos < _Count)
			_Count = _Str.size() - _Pos;	// trim _Count to size
		if (_Bits < _Count)
			_Count = _Bits;	// trim _Count to length of bitset
		_Tidy();

		for (_Pos += _Count, _Num = 0; _Num < _Count; ++_Num)
			if (_Str[--_Pos] == _E1)
				set(_Num);
			else if (_Str[_Pos] != _E0)
				_Xinv();
		}

	bitset<_Bits>& operator&=(const bitset<_Bits>& _Right)
		{	// AND in _Right
		for (int32_t _Wpos = _Words; 0 <= _Wpos; --_Wpos)
			_Array[_Wpos] &= _Right._Getword(_Wpos);
		return (*this);
		}

	bitset<_Bits>& operator|=(const bitset<_Bits>& _Right)
		{	// OR in _Right
		for (int32_t _Wpos = _Words; 0 <= _Wpos; --_Wpos)
			_Array[_Wpos] |= _Right._Getword(_Wpos);
		return (*this);
		}

	bitset<_Bits>& operator^=(const bitset<_Bits>& _Right)
		{	// XOR in _Right
		for (int32_t _Wpos = _Words; 0 <= _Wpos; --_Wpos)
			_Array[_Wpos] ^= _Right._Getword(_Wpos);
		return (*this);
		}

	bitset<_Bits>& operator<<=(size_t _Pos)
		{	// shift left by _Pos
		const int32_t _Wordshift = (int32_t)(_Pos / _Bitsperword);
		if (_Wordshift != 0)
			for (int32_t _Wpos = _Words; 0 <= _Wpos; --_Wpos)	// shift by words
				_Array[_Wpos] = _Wordshift <= _Wpos
					? _Array[_Wpos - _Wordshift] : (_Ty)0;

		if ((_Pos %= _Bitsperword) != 0)
			{	// 0 < _Pos < _Bitsperword, shift by bits
			for (int32_t _Wpos = _Words; 0 < _Wpos; --_Wpos)
				_Array[_Wpos] = (_Ty)((_Array[_Wpos] << _Pos)
					| (_Array[_Wpos - 1] >> (_Bitsperword - _Pos)));
			_Array[0] <<= _Pos;
			}
		_Trim();
		return (*this);
		}

	bitset<_Bits>& operator>>=(size_t _Pos)
		{	// shift right by _Pos
		const int32_t _Wordshift = (int32_t)(_Pos / _Bitsperword);
		if (_Wordshift != 0)
			for (int32_t _Wpos = 0; _Wpos <= _Words; ++_Wpos)	// shift by words
				_Array[_Wpos] = _Wordshift <= _Words - _Wpos
						? _Array[_Wpos + _Wordshift] : (_Ty)0;

		if ((_Pos %= _Bitsperword) != 0)
			{	// 0 < _Pos < _Bitsperword, shift by bits
			for (int32_t _Wpos = 0; _Wpos < _Words; ++_Wpos)
				_Array[_Wpos] = (_Ty)((_Array[_Wpos] >> _Pos)
					| (_Array[_Wpos + 1] << (_Bitsperword - _Pos)));
			_Array[_Words] >>= _Pos;
			}
		return (*this);
		}

	bitset<_Bits>& set()
		{	// set all bits true
		_Tidy((_Ty)~0);
		return (*this);
		}

	bitset<_Bits>& set(size_t _Pos,
		bool _Val = true)
		{	// set bit at _Pos to _Val
		if (_Bits <= _Pos)
			_Xran();	// _Pos off end
		if (_Val)
			_Array[_Pos / _Bitsperword] |= (_Ty)1 << _Pos % _Bitsperword;
		else
			_Array[_Pos / _Bitsperword] &= ~((_Ty)1 << _Pos % _Bitsperword);
		return (*this);
		}

	bitset<_Bits>& reset()
		{	// set all bits false
		_Tidy();
		return (*this);
		}

	bitset<_Bits>& reset(size_t _Pos)
		{	// set bit at _Pos to false
		return (set(_Pos, false));
		}

	bitset<_Bits> operator~() const
		{	// flip all bits
		return (bitset<_Bits>(*this).flip());
		}

	bitset<_Bits>& flip()
		{	// flip all bits
		for (int32_t _Wpos = _Words; 0 <= _Wpos; --_Wpos)
			_Array[_Wpos] = (_Ty)~_Array[_Wpos];

		_Trim();
		return (*this);
		}

	bitset<_Bits>& flip(size_t _Pos)
		{	// flip bit at _Pos
		if (_Bits <= _Pos)
			_Xran();	// _Pos off end
		_Array[_Pos / _Bitsperword] ^= (_Ty)1 << _Pos % _Bitsperword;
		return (*this);
		}

	uint64_t to_ulong() const
   {	// convert bitset to uint64_t
		uint64_t _Val = to_ullong();
		uint64_t _Ans = (uint64_t)_Val;
		if (_Ans  != _Val)
			_Xoflo();
		return (_Ans);
	}

	uint64_t to_ullong() const
	{	// convert bitset to uint64_t long
		enum
			{	// cause zero divide if uint64_t long not multiple of _Ty
			_Assertion = 1
				/ (int32_t)(sizeof (uint64_t) % sizeof (_Ty) == 0)};

		int32_t _Wpos = _Words;
		for (; (int32_t)(sizeof (uint64_t) / sizeof (_Ty)) <= _Wpos; --_Wpos)
			if (_Array[_Wpos] != 0)
				_Xoflo();	// fail if any high-order words are nonzero

		uint64_t _Val = _Array[_Wpos];
		for (; 0 <= --_Wpos; )
			_Val = ((_Val << (_Bitsperword - 1)) << 1) | _Array[_Wpos];
   return (_Val);
		}

   string to_string(char _E0 = (char)'0', char _E1 = (char)'1') const
   {	// convert bitset to string
		string _Str;
		strsize _Pos;
		_Str.reserve(_Bits);

		for (_Pos = _Bits; 0 < _Pos; )
			if (test(--_Pos))
				_Str += _E1;
			else
				_Str += _E0;
		return (_Str);
   }



	size_t count() const
		{	// ::count number of set bits
		static char _Bitsperhex[] = "\0\1\1\2\1\2\2\3\1\2\2\3\2\3\3\4";
		size_t _Val = 0;
		for (int32_t _Wpos = _Words; 0 <= _Wpos; --_Wpos)
			for (_Ty _Wordval = _Array[_Wpos]; _Wordval != 0; _Wordval >>= 4)
				_Val += _Bitsperhex[_Wordval & 0xF];
		return (_Val);
		}

	size_t size() const
		{	// return size of bitset
		return (_Bits);
		}

 #if _HAS_CPP0X
	size_t hash() const
		{	// hash bits to size_t value by pseudorandomizing transform
		size_t _Val = 2166136261U;
		for (int32_t _Wpos = _Words; 0 <= _Wpos; --_Wpos)
			_Val = 16777619U * _Val ^ _Array[_Wpos];
		return (_Val);
		}
 #endif /* _HAS_CPP0X */

	bool operator==(const bitset<_Bits>& _Right) const
		{	// test for bitset equality
		for (int32_t _Wpos = _Words; 0 <= _Wpos; --_Wpos)
			if (_Array[_Wpos] != _Right._Getword(_Wpos))
				return (false);
		return (true);
		}

	bool operator!=(const bitset<_Bits>& _Right) const
		{	// test for bitset inequality
		return (!(*this == _Right));
		}

	bool test(size_t _Pos) const
		{	// test if bit at _Pos is set
		if (_Bits <= _Pos)
			_Xran();	// _Pos off end
		return ((_Array[_Pos / _Bitsperword]
			& ((_Ty)1 << _Pos % _Bitsperword)) != 0);
		}

	bool any() const
		{	// test if any bits are set
		for (int32_t _Wpos = _Words; 0 <= _Wpos; --_Wpos)
			if (_Array[_Wpos] != 0)
				return (true);
		return (false);
		}

	bool none() const
		{	// test if no bits are set
		return (!any());
		}

	bool all() const
		{	// test if all bits set
		return (count() == size());
		}

	bitset<_Bits> operator<<(size_t _Pos) const
		{	// return bitset shifted left by _Pos
		return (bitset<_Bits>(*this) <<= _Pos);
		}

	bitset<_Bits> operator>>(size_t _Pos) const
		{	// return bitset shifted right by _Pos
		return (bitset<_Bits>(*this) >>= _Pos);
		}

	_Ty _Getword(size_t _Wpos) const
		{	// get uint16_t at _Wpos
		return (_Array[_Wpos]);
		}

private:

	void _Tidy(_Ty _Wordval = 0)
		{	// set all words to _Wordval
		for (int32_t _Wpos = _Words; 0 <= _Wpos; --_Wpos)
			_Array[_Wpos] = _Wordval;
		if (_Wordval != 0)
			_Trim();
		}

	void _Trim()
		{	// clear any trailing bits in last uint16_t
		_Trim_if<_Bits % _Bitsperword != 0>();
		}

	template<bool _Has_bits>
		void _Trim_if()
		{	// bits to trim, remove them
         if(_Has_bits)
         {
            _Array[_Words] &= ((_Ty)1 << _Bits % _Bitsperword) - 1;
         }
		}

	void _Xinv() const
   {	// report invalid string element in bitset conversion
		throw invalid_argument_exception("invalid bitset<N> char");
	}

	void _Xoflo() const
	{	// report converted value too big to represent
		throw overflow_error(get_thread_app(), "bitset<N> overflow");
	}

	void _Xran() const
	{	// report bit index out of range
		throw range_error(get_thread_app(), "invalid bitset<N> position");
	}

};

		// bitset TEMPLATE FUNCTIONS
template<size_t _Bits> inline
	bitset<_Bits> operator&(const bitset<_Bits>& _Left,
		const bitset<_Bits>& _Right)
		{	// return bitset _Left AND _Right
		bitset<_Bits> _Ans = _Left;
		return (_Ans &= _Right);
		}

template<size_t _Bits> inline
	bitset<_Bits> operator|(const bitset<_Bits>& _Left,
		const bitset<_Bits>& _Right)
		{	// return bitset _Left OR _Right
		bitset<_Bits> _Ans = _Left;
		return (_Ans |= _Right);
		}

template<size_t _Bits> inline
	bitset<_Bits> operator^(const bitset<_Bits>& _Left,
		const bitset<_Bits>& _Right)
		{	// return bitset _Left XOR _Right
		bitset<_Bits> _Ans = _Left;
		return (_Ans ^= _Right);
		}

   /*inline ::core::output_stream & operator << (::core::output_stream2 & _Ostr, const bitset<_Bits>& _Right);

	inline ::core::input_stream & operator>>(::core::input_stream  _Istr, bitset<_Bits>& _Right);

#if _HAS_CPP0X
template<class _Kty>
	class hash;

template<size_t _Bits>
	class hash<bitset<_Bits> >
		: public unary_function<bitset<_Bits>, size_t>
	{	// hash functor
public:
	typedef bitset<_Bits> _Kty;

	size_t operator()(const _Kty& _Keyval) const
		{	// hash _Keyval to size_t value by pseudorandomizing transform
		return (_Keyval.hash());
		}
	};
 #endif // _HAS_CPP0X

 //#pragma warning(pop)
 //#pragma pack(pop)

//
// Copyright (ca) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
// Consult your license regarding permissions and restrictions.
// V5.20:0009

*/


