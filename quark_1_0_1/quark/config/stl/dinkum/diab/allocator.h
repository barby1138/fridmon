// allocator.h
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Quark Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Copyright (c) 2006, InterObject Ltd
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _QUARK_ALLOCATOR_H_
#define _QUARK_ALLOCATOR_H_

namespace quark
{

template<class _Ty, class _Pool>
class allocator
{
public:
	// type definitions
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef _Ty* pointer;
	typedef const _Ty* const_pointer;
	typedef _Ty& reference;
	typedef const _Ty& const_reference;
	typedef _Ty value_type;

	template<class _Other>
	struct rebind
	{	// convert an allocator<_Ty> to an allocator <_Other>
		typedef allocator<_Other, _Pool> other;
	};

	pointer address(reference _Val) const
	{	// return address of mutable _Val
		return (&_Val);
	}

	const_pointer address(const_reference _Val) const
	{	// return address of nonmutable _Val
		return (&_Val);
	}

	allocator()
	{	// construct default allocator (do nothing)
	}

	allocator(const allocator<_Ty, _Pool>&)
	{	// construct by copying (do nothing)
	}

	template<class _Other>
		allocator(const allocator<_Other, _Pool>&)
	{	// construct from a related allocator (do nothing)
	}

	template<class _Other>
		allocator<_Ty, _Pool>& operator=(const allocator<_Other, _Pool>&)
	{	// assign from a related allocator (do nothing)
		return (*this);
	}

	pointer allocate(size_type num, const void* )
	{	// allocate array of num elements, ignore hint
		return (pointer)_Pool::instance().allocate(num * sizeof(_Ty));
	}

	pointer allocate(size_type num)
	{	// allocate array of num elements
		return (pointer)_Pool::instance().allocate(num * sizeof(_Ty));
	}

	void deallocate(pointer p, size_type num)
	{	// deallocate object at p, ignore size
		_Pool::instance().free(p, num * sizeof(_Ty));
	}

	void construct(pointer p, const _Ty& val)
	{	// construct object at p with value _Val
		::new((void*)p)_Ty(val);
	}

	void destroy(pointer p)
	{	// destroy object at p
		p->~_Ty();
	}

	size_t max_size() const
	{	// estimate maximum array size
		size_t num = (size_t)(-1) / sizeof(_Ty);
		return (0 < num ? num : 1);
	}
};

template<class _Pool>
class allocator<void, _Pool>
{	// generic allocator for type void
public:
	typedef void _Ty;
	typedef _Ty* pointer;
	typedef const _Ty* const_pointer;
	typedef _Ty value_type;
	
	template<class _Other>
	struct rebind
	{	// convert an allocator<void> to an allocator <_Other>
		typedef allocator<_Other, _Pool> other;
	};
	
	allocator()
	{	// construct default allocator (do nothing)
	}

	allocator(const allocator<_Ty, _Pool>&)
	{	// construct by copying (do nothing)
	}

	template<class _Other>
		allocator(const allocator<_Other, _Pool>&)
	{	// construct from a related allocator (do nothing)
	}

	template<class _Other>
		allocator<_Ty, _Pool>& operator=(const allocator<_Other, _Pool>&)
	{	// assign from a related allocator (do nothing)
		return (*this);
	}
};

// allocator TEMPLATE OPERATORS
template<class _Ty,
class _Other, class _Pool> inline
	bool operator==(const allocator<_Ty, _Pool>&, const allocator<_Other, _Pool>&)
{	// test for allocator equality (always true)
	return (true);
}

template<class _Ty,
class _Other, class _Pool> inline
	bool operator!=(const allocator<_Ty, _Pool>&, const allocator<_Other, _Pool>&)
{	// test for allocator inequality (always false)
	return (false);
}

} // namespace quark

#endif // _QUARK_ALLOCATOR_H_
