// mempool.h
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Quark Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _QUARK_MEMPOOL_H_
#define _QUARK_MEMPOOL_H_

#include <quark/sync.h>
#include <quark/strings.h>

#include "small_alloc.h"
#include "middle_alloc.h"
#include "storage.h"

namespace quark
{

//////////////////////////////////////////////////
// class combined_allocator
//
template<class _Allocator1, class _Allocator2>
class combined_allocator
{
public:
	typedef typename _Allocator2::pool_type pool_type;

public:
	combined_allocator(pool_type& pool);
	~combined_allocator();

public:
    void* allocate(size_t size);
    void free(void* p, size_t size);
	void release();

private:
	_Allocator2 _allocator2;
	_Allocator1 _allocator1;
};

template<class _Allocator1, class _Allocator2>
combined_allocator<_Allocator1, _Allocator2>::combined_allocator(pool_type& pool)
	:	_allocator2(pool),
		_allocator1(_allocator2)
{
}

template<class _Allocator1, class _Allocator2>
combined_allocator<_Allocator1, _Allocator2>::~combined_allocator()
{
	release();
}

template<class _Allocator1, class _Allocator2>
void* combined_allocator<_Allocator1, _Allocator2>::allocate(size_t size)
{
	return _allocator1.allocate(size);
}

template<class _Allocator1, class _Allocator2>
void combined_allocator<_Allocator1, _Allocator2>::free(void* p, size_t size)
{
	_allocator1.free(p, size);
}

template<class _Allocator1, class _Allocator2>
void combined_allocator<_Allocator1, _Allocator2>::release()
{
	_allocator1.release();
	_allocator2.release();
}

//////////////////////////////////////////////////
// class memory_pool
//
template
<
	class _Storage = expandable_storage<>, 
	class _Allocator = combined_allocator<
		small_allocator<middle_allocator<_Storage> >, 
		middle_allocator<_Storage> >
>
class memory_pool : public noncopyable
{
private:
	typedef typename _Storage::mutex_type mutex_type;

public:
	memory_pool();
	~memory_pool();

	void* allocate(size_t size);
	void free(void* p, size_t size);
	const pstring dump() const;

private:
	_Storage			_storage;
	_Allocator			_allocator;
	mutable mutex_type	_mutex;
	u32					_zero;
};

template<class _Storage, class _Allocator>
memory_pool<_Storage, _Allocator>::memory_pool()
	:	_zero(0),
		_allocator(_storage)
{
}

template<class _Storage, class _Allocator>
memory_pool<_Storage, _Allocator>::~memory_pool()
{
	_allocator.release();
}

template<class _Storage, class _Allocator>
inline void* memory_pool<_Storage, _Allocator>::allocate(size_t size)
{
	critical_scope<mutex_type> lock(_mutex);

	if (size == 0)
		return &_zero;
	return _allocator.allocate(size);
}

template<class _Storage, class _Allocator>
inline void memory_pool<_Storage, _Allocator>::free(void* p, size_t size)
{
	critical_scope<mutex_type> lock(_mutex);

	if (size == 0 || p == 0)
		return;
	_allocator.free(p, size);
}

template<class _Storage, class _Allocator>
const pstring memory_pool<_Storage, _Allocator>::dump() const
{
	storage_statistics statistics;
	{
		critical_scope<mutex_type> lock(_mutex);
		statistics = _storage.getStatistics();
	}
	
	return strings::format("Memory pool dump, bytes\n"
		"--------------------------------------------------------------------------\n"
		"Total allocated: %d\n"
		"Peak memory used: %d\n"
		"--------------------------------------------------------------------------",
		statistics._totalAllocated,
		statistics._totalUsed);
}

} // namespace quark

#endif // _QUARK_MEMPOOL_H_
