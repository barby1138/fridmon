// service_mempool.cpp
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

#include <quark/config.h>
#include "service_mempool.h"
#include "../mempool.h"
#include "../storage.h"
#include <quark/singleton.h>
#include IMPL_FILE(sync_impl.h)

using namespace quark;

enum { SERVICE_STORAGE_CHUNK_SIZE = 16*1024 };
enum { SMALL_ALLOC_CHUNK_SIZE = 256 };

//////////////////////////////////////////////////
// class service_storage::mutex
//
class service_storage::mutex : public critical_section::critical_section_impl
{
public:
	void* operator new(size_t size)
		{ return ::malloc(size); }

	void operator delete(void* p, size_t size)
		{ ::free(p); }	
};

//////////////////////////////////////////////////
// class pimple_storage
//
class pimple_storage
{
public:
	typedef service_storage::mutex mutex_type;

public:
	void* allocate(size_t size)
	{
		return service_storage::instance().allocate(size);
	}
};

//////////////////////////////////////////////////
// class pimple_mempool
//
typedef memory_pool
<
	pimple_storage, 
	combined_allocator<
		small_allocator<middle_allocator<pimple_storage>, SMALL_ALLOC_CHUNK_SIZE>, 
		middle_allocator<pimple_storage> > 
> pimple_mempool_impl;

typedef singleton_holder
<	
	pimple_mempool_impl,
	create_static,
	default_lifetime,
	single_threaded<pimple_mempool_impl>
> pimple_mempool;

//////////////////////////////////////////////////////////////////////
// pimple_object
//////////////////////////////////////////////////////////////////////

void* pimple_object::operator new(size_t size)
{
	return pimple_mempool::instance().allocate(size);
}

void pimple_object::operator delete(void* p, size_t size)
{
	pimple_mempool::instance().free(p, size);
}

//////////////////////////////////////////////////////////////////////
// service_object
//////////////////////////////////////////////////////////////////////

void* service_object::operator new(size_t size)
{
	return service_storage::instance().allocate(size);
}

void service_object::operator delete(void* p, size_t size)
{
	service_storage::instance().free(p, size);
}

//////////////////////////////////////////////////////////////////////
// service_storage
//////////////////////////////////////////////////////////////////////

service_storage::service_storage()
	:	_pCur(0),
		_remainSize(0),
		_totalUsedSize(0),
		_pLock(new mutex)
{
	_chunks.reserve(10);
	newChunk();
}

service_storage::~service_storage()
{
//	::printf("fixed service storage total used size: %d\n", _totalUsedSize);

	for (chunks_t::const_iterator itor = _chunks.begin(); itor != _chunks.end(); ++itor)
		::free(*itor);

	delete _pLock;
}

service_storage& service_storage::instance()
{
	static service_storage inst;
	return inst;
}

void* service_storage::allocate(size_t size)
{
	critical_scope<mutex> quard(*_pLock);

	if (size > _remainSize)
		newChunk();

	void* p = _pCur;
	_pCur += size;
	_remainSize -= size;
	_totalUsedSize += size;
	
	return p;
}

void service_storage::free(void* p, size_t size)
{
	// none
}

void service_storage::newChunk()
{
	_pCur = (u8*)malloc(SERVICE_STORAGE_CHUNK_SIZE);
	assert(_pCur);
	_chunks.push_back(_pCur);
	_remainSize = SERVICE_STORAGE_CHUNK_SIZE;
}
