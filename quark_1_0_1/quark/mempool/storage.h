// storage.h
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

#ifndef _QUARK_MEM_STORAGE_H_
#define _QUARK_MEM_STORAGE_H_

#ifndef DEFAULT_STORAGE_CHUNK_SIZE
#define DEFAULT_STORAGE_CHUNK_SIZE 64*1024
#endif

namespace quark
{

//////////////////////////////////////////////////
// struct storage_statistics
//
struct storage_statistics
{
	storage_statistics()
		:	_totalAllocated(0), _totalUsed(0) {}

	storage_statistics(size_t totalAllocated, size_t totalUsed)
		:	_totalAllocated(totalAllocated), _totalUsed(totalUsed) {}

	size_t _totalAllocated;
	size_t _totalUsed;
};

//////////////////////////////////////////////////
// class fixed_storage
//
template<const size_t _Size, class _Mutex/* = light_mutex*/>
class fixed_storage : public noncopyable
{
public:
	typedef _Mutex mutex_type;

public:
	fixed_storage();
	~fixed_storage();

	void* allocate(size_t size);
	const storage_statistics getStatistics() const;
	
private:
	u8*		_pStore;
	u8*		_pCur;
	size_t	_remainSize;
};	

template<const size_t _Size, class _Mutex>
fixed_storage<_Size, _Mutex>::fixed_storage()
	:	_pStore((u8*)malloc(_Size)),
		_pCur(_pStore),
		_remainSize(_Size)
{
}

template<const size_t _Size, class _Mutex>
fixed_storage<_Size, _Mutex>::~fixed_storage()
{
	::free(_pStore);
}

template<const size_t _Size, class _Mutex>
void* fixed_storage<_Size, _Mutex>::allocate(size_t size)
{
	assert(size <= _remainSize);

	void* p = _pCur;
	_pCur += size;
	_remainSize -= size;
	return p;
}

template<const size_t _Size, class _Mutex>
const storage_statistics fixed_storage<_Size, _Mutex>::getStatistics() const
{
	return storage_statistics(_Size, _Size - _remainSize);
}

//////////////////////////////////////////////////
// class expandable_storage
//
template
<
	const size_t _ChunkSize = DEFAULT_STORAGE_CHUNK_SIZE,
	class _Mutex = critical_section
>
class expandable_storage
{
private:
	typedef std::vector<void*, SERVICE_STL_ALLOC(void*)> chunks_t;

public:
	typedef _Mutex mutex_type;

public:
	expandable_storage();
	~expandable_storage();

	void* allocate(size_t size);
	const storage_statistics getStatistics() const;

private:
	void newChunk();
	void* extraAllocate(size_t size);

private:
	u8*		_pCur;
	size_t	_remainSize;
	size_t	_totalAllocSize;
	size_t	_totalUsedSize;
	chunks_t _chunks;
};

template<const size_t _ChunkSize, class _Mutex>
expandable_storage<_ChunkSize, _Mutex>::expandable_storage()
	:	_pCur(0),
		_remainSize(0),
		_totalAllocSize(0),
		_totalUsedSize(0)
{
	_chunks.reserve(10);
	newChunk();
}

template<const size_t _ChunkSize, class _Mutex>
expandable_storage<_ChunkSize, _Mutex>::~expandable_storage()
{
	for (chunks_t::const_iterator itor = _chunks.begin();
		 itor != _chunks.end(); ++itor)
		::free(*itor);
}

template<const size_t _ChunkSize, class _Mutex>
void* expandable_storage<_ChunkSize, _Mutex>::allocate(size_t size)
{
	if (size > _ChunkSize)
		return extraAllocate(size);

	if (size > _remainSize)
		newChunk();

	void* p = _pCur;
	_pCur += size;
	_remainSize -= size;
	_totalUsedSize += size;
	
	return p;
}

template<const size_t _ChunkSize, class _Mutex>
void expandable_storage<_ChunkSize, _Mutex>::newChunk()
{
	_pCur = (u8*)malloc(_ChunkSize);
	assert(_pCur);
	_chunks.push_back(_pCur);
	_remainSize = _ChunkSize;
	_totalAllocSize += _ChunkSize;
}

template<const size_t _ChunkSize, class _Mutex>
void* expandable_storage<_ChunkSize, _Mutex>::extraAllocate(size_t size)
{
	void* p = (u8*)malloc(size);
	assert(p);
	_chunks.push_back(p);
	_totalUsedSize += size;
	_totalAllocSize += size;
	return p;
}

template<const size_t _ChunkSize, class _Mutex>
const storage_statistics expandable_storage<_ChunkSize, _Mutex>::getStatistics() const
{
	return storage_statistics(_totalAllocSize, _totalUsedSize);
}

} // namespace quark

#endif // _QUARK_MEM_STORAGE_H_
