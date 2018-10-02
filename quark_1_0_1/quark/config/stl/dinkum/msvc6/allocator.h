// stl_alloc.h
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
#ifndef _FARQ   /* specify standard memory model */
#define _FARQ
#define _PDFT  ptrdiff_t
#define _SIZT  size_t
#endif /* _FARQ */

template <class _T, class _Pool>
struct allocator 
{
public:
	// type definitions
	typedef _SIZT size_type;
	typedef std::ptrdiff_t difference_type;
	typedef _Ty _FARQ *pointer;
	typedef const _Ty _FARQ *const_pointer;
	typedef _Ty _FARQ& reference;
	typedef const _Ty _FARQ& const_reference;
	typedef _Ty value_type;
	
	// constructors and destructor
	//- nothing to do because the allocator has no state
	explicit allocator() throw() {}
	
	explicit allocator(const allocator& ) throw() {}
	
	~allocator() throw() {}
	
	// return address of values
	pointer address(reference value) const 
	{
		return &value;
	}
	
	inline const_pointer address(const_reference value) const 
	{
		return &value;
	}
	
	// return maximum number of elements that can be allocated
	size_type max_size() const throw()
	{
		return std::numeric_limits<size_type>::max() / sizeof(_T);
	}
	
	pointer allocate(size_type num, const void* = 0) 
	{
		return (pointer)_Pool::instance().allocate(num * sizeof(_T));
	}
	
	// deallocate storage p of deleted elements
	void deallocate(pointer p, size_type num) 
	{
		_Pool::instance().free(p, num * sizeof(_T));
	}
	
	// initialize elements of allocated storage p with value value
	void construct(pointer p, const _T& value) 
	{
		// initialize memory with placement new
		::new((void*)p)_T(value);
	}
	
	// destroy elements of initialized storage p
	void destroy(pointer p) 
	{
		// destroy objects by calling their destructor
		p->~_T();
	}
	
	// VC6: <list>, <map>, <deque> require this func
	char* _Charalloc(size_type num)
	{
		return (char*)_Pool::instance().allocate(num);
	}		 

	// VC6: <list>, <map>, <deque> require this func
	void deallocate(void* p, size_type num) 
	{
		_Pool::instance().free(p, num);
	}

	bool operator==(const allocator& ) 
	{ 
		return true; 
	}

	bool operator!=(const allocator& rhs) 
	{ 
		return !operator==(rhs);
	}
};
} // namespace quark

#endif //_STL_ALLOC_H_
