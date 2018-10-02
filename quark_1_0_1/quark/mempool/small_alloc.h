// small_alloc.h
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

////////////////////////////////////////////////////////////////////////////////
// Refactored from the Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Wesley Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _QUARK_SMALL_ALLOC_H_
#define _QUARK_SMALL_ALLOC_H_

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 2048
#endif

#ifndef MAX_SMALL_OBJECT_SIZE
#define MAX_SMALL_OBJECT_SIZE 64
#endif

#include "service/service_mempool.h"

namespace quark
{

namespace prvt
{

//////////////////////////////////////////////////
// class template service_list
//
template<typename _T>
class service_list
{
private:
    typedef std::list<_T, SERVICE_STL_ALLOC(_T)> list_t;

public:
	typedef typename list_t::iterator iterator;
	typedef typename list_t::const_iterator const_iterator;
	typedef typename list_t::size_type size_type;

public:
	service_list()
		:	_end(_list.end()), _size(0) {}

	iterator begin()
		{ return _list.begin(); }
	iterator end()
		{ return _end; }

	void push_back(const _T& val)
	{
		if (_end == _list.end())
		{
			_list.push_back(val);
			_end = _list.end();
		}
		else
		{
			*_end++ = val;
		}
		++_size;
	}
	void pop_back()
	{ 
		--_end;
		--_size;
	}

	size_type size() const
		{ return _size; }
	bool empty() const
		{ return _list.empty(); }

private:
	list_t _list;
	typename list_t::iterator _end;
	size_type _size;
};

}; // namespace prvt

//////////////////////////////////////////////////
// class small_alloc_chunk
//
template<class _Pool>
struct small_alloc_chunk
{
	small_alloc_chunk(_Pool& pool) : _pool(pool) {}
	small_alloc_chunk& operator=(const small_alloc_chunk& rhs);

	void init(size_t cellSize, u8 cells);
	void* allocate(size_t cellSize);
	void free(void* p, size_t cellSize);
	void reset(size_t cellSize, u8 cells);
	void release(size_t cellSize);
		
	_Pool&	_pool;
	u8*		_pData;
	u8		_firstAvailableCell;
	u8		_cellsAvailable;
};

template<class _Pool>
inline small_alloc_chunk<_Pool>& small_alloc_chunk<_Pool>::operator=(const small_alloc_chunk<_Pool>& rhs)
{
	_pData = rhs._pData;
	_firstAvailableCell = rhs._firstAvailableCell;
	_cellsAvailable = rhs._cellsAvailable;
	return *this;
}

template<class _Pool>
inline void small_alloc_chunk<_Pool>::init(size_t cellSize, u8 cells)
{
    assert(cellSize > 0);
    assert(cells > 0);
    // overflow check
    assert((cellSize * cells) / cellSize == cells);
    
    _pData = (u8*)_pool.allocate(cellSize * cells);
    reset(cellSize, cells);
}

template<class _Pool>
inline void small_alloc_chunk<_Pool>::reset(size_t cellSize, u8 cells)
{
    assert(cellSize > 0);
    assert(cells > 0);
    // overflow check
    assert((cellSize * cells) / cellSize == cells);

    _firstAvailableCell = 0;
    _cellsAvailable = cells;

    u8 i = 0;
    u8* p = _pData;
    for (; i != cells; p += cellSize) *p = ++i;
}

template<class _Pool>
inline void small_alloc_chunk<_Pool>::release(size_t cellSize)
{
    _pool.free(_pData, cellSize * _cellsAvailable);
}

template<class _Pool>
inline void* small_alloc_chunk<_Pool>::allocate(size_t cellSize)
{
    if (!_cellsAvailable) return 0;
    
    assert((_firstAvailableCell * cellSize) / cellSize == _firstAvailableCell);

    u8* pResult = _pData + (_firstAvailableCell * cellSize);
    _firstAvailableCell = *pResult;
    --_cellsAvailable;
    
    return pResult;
}

template<class _Pool>
inline void small_alloc_chunk<_Pool>::free(void* p, size_t cellSize)
{
    assert(p >= _pData);

    u8* toRelease = static_cast<u8*>(p);
    // alignment check
    assert((toRelease - _pData) % cellSize == 0);

    *toRelease = _firstAvailableCell;
    _firstAvailableCell = static_cast<u8>((toRelease - _pData) / cellSize);
    // truncation check
    assert(_firstAvailableCell == (toRelease - _pData) / cellSize);

    ++_cellsAvailable;
}

//////////////////////////////////////////////////
// class small_alloc_block
//
template<class _Pool>
class small_alloc_block : public noncopyable,
						  public service_object
{
private:
    typedef prvt::service_list<small_alloc_chunk<_Pool> > chunks_t;
	typedef typename chunks_t::iterator chunk_iterator;

public:
    small_alloc_block(_Pool& pool, size_t chunkSize, size_t blockSize);
    ~small_alloc_block();
    
    void* allocate();
    void free(void* p);
    size_t blockSize() const { return _blockSize; }

private:
    void deallocate(void* p);
    chunk_iterator findChunk(void* p);
    
private:
	_Pool& _pool;
    size_t _blockSize;
    u8 _numCells;
    chunks_t _chunks;
    chunk_iterator _pAllocChunk;
    chunk_iterator _pFreeChunk;
};

template<class _Pool>
inline small_alloc_block<_Pool>::small_alloc_block(_Pool& pool, size_t chunkSize, size_t blockSize)
    :	_pool(pool),
		_blockSize(blockSize),
		_pAllocChunk(_chunks.end()),
		_pFreeChunk(_chunks.end())
{
    assert(_blockSize > 0);
  
	size_t numCells = chunkSize / blockSize;
    if (numCells > UCHAR_MAX)
		numCells = UCHAR_MAX;
    else if (numCells == 0) 
		numCells = 8 * blockSize;
    
    _numCells = static_cast<u8>(numCells);
    assert(_numCells == numCells);
}

template<class _Pool>
inline small_alloc_block<_Pool>::~small_alloc_block()
{
    chunk_iterator itor = _chunks.begin();
    for (; itor != _chunks.end(); ++itor)
    {
       // assert(itor->_cellsAvailable == _numCells);
       itor->release(_blockSize);
    }
}

template<class _Pool>
void* small_alloc_block<_Pool>::allocate()
{
    if (_pAllocChunk == _chunks.end() || _pAllocChunk->_cellsAvailable == 0)
    {
        chunk_iterator itor = _chunks.begin();
        for (;; ++itor)
        {
            if (itor == _chunks.end())
            {
                // initialize
                small_alloc_chunk<_Pool> newChunk(_pool);
                newChunk.init(_blockSize, _numCells);
                _chunks.push_back(newChunk);
                _pAllocChunk = --_chunks.end();
                _pFreeChunk = _chunks.begin();
                break;
            }
            if (itor->_cellsAvailable > 0)
            {
                _pAllocChunk = itor;
                break;
            }
        }
    }
    assert(_pAllocChunk != _chunks.end());
    assert(_pAllocChunk->_cellsAvailable > 0);
    
    return _pAllocChunk->allocate(_blockSize);
}

template<class _Pool>
inline void small_alloc_block<_Pool>::free(void* p)
{
    assert(!_chunks.empty());
    
    _pFreeChunk = findChunk(p);
    assert(_pFreeChunk != _chunks.end());

    deallocate(p);
}

template<class _Pool>
typename small_alloc_block<_Pool>::chunk_iterator small_alloc_block<_Pool>::findChunk(void* p)
{
    assert(!_chunks.empty());
    assert(_pFreeChunk != _chunks.end());

    const size_t chunkLength = _numCells * _blockSize;

    chunk_iterator lo = _pFreeChunk;
    chunk_iterator hi = _pFreeChunk; ++hi;
    chunk_iterator loBound = _chunks.begin();
    chunk_iterator hiBound = _chunks.end();
	
    for (;;)
    {
        if (lo != _chunks.end())
        {
            if (p >= lo->_pData && p < lo->_pData + chunkLength)
            {
				return lo;
            }
            if (lo == loBound) lo = _chunks.end();
            else --lo;
        }
        
        if (hi != _chunks.end())
        {
            if (p >= hi->_pData && p < hi->_pData + chunkLength)
            {
				return hi;
            }	
            if (++hi == hiBound) hi = _chunks.end();
        }
    }
}

template<class _Pool>
void small_alloc_block<_Pool>::deallocate(void* p)
{
    assert(_pFreeChunk->_pData <= p);
    assert(_pFreeChunk->_pData + _numCells * _blockSize > p);

    // call into the chunk, will adjust the inner list but won't release memory
    _pFreeChunk->free(p, _blockSize);

    if (_pFreeChunk->_cellsAvailable == _numCells)
    {
        // _pFreeChunk is completely free, should we release it? 
        chunk_iterator pLastChunk = --_chunks.end();
        
        if (pLastChunk == _pFreeChunk)
        {
            // check if we have two last chunks empty
            if (_chunks.size() > 1 && 
                (--chunk_iterator(_pFreeChunk))->_cellsAvailable == _numCells)
            {
                // two free chunks, discard the last one
                pLastChunk->release(_blockSize);
                _chunks.pop_back();
                _pAllocChunk = _pFreeChunk = _chunks.begin();
            }
            return;
        }
        
        if (pLastChunk->_cellsAvailable == _numCells)
        {
            // two free cells, discard one
            pLastChunk->release(_blockSize);
            _chunks.pop_back();
            _pAllocChunk = _pFreeChunk;
        }
        else
        {
            // move the empty chunk to the end
            std::swap(*_pFreeChunk, *pLastChunk);
            _pAllocChunk = --_chunks.end();
        }
    }
}

//////////////////////////////////////////////////
// class small_allocator
//
template
<
	class _Pool, 
	size_t _ChunkSize = DEFAULT_CHUNK_SIZE, 
	size_t _MaxObjectSize = MAX_SMALL_OBJECT_SIZE
>
class small_allocator : public noncopyable
{
private:
    typedef std::vector<small_alloc_block<_Pool>*, 
						SERVICE_STL_ALLOC(small_alloc_block<_Pool>*)> blocks_t;

public:
	typedef _Pool pool_type;

public:
    explicit small_allocator(_Pool& pool);
	~small_allocator();

    void* allocate(size_t size);
    void free(void* p, size_t size);
	void release();

private:
    blocks_t _blocks;
	_Pool& _pool;
};

template<class _Pool, size_t _ChunkSize, size_t _MaxObjectSize>
inline small_allocator<_Pool, _ChunkSize, _MaxObjectSize>::small_allocator(_Pool& pool)
    :	_pool(pool),
		_blocks(_MaxObjectSize + 1)
{   
}

template<class _Pool, size_t _ChunkSize, size_t _MaxObjectSize>
inline small_allocator<_Pool, _ChunkSize, _MaxObjectSize>::~small_allocator()
{
	std::for_each(_blocks.begin(), _blocks.end(), cleaner_ptr());
}

template<class _Pool, size_t _ChunkSize, size_t _MaxObjectSize>
inline void* small_allocator<_Pool, _ChunkSize, _MaxObjectSize>::allocate(size_t size)
{
    if (size > _MaxObjectSize)
		return _pool.allocate(size);

    small_alloc_block<_Pool>*& pBlock = _blocks[size];
	if (pBlock == 0)
		pBlock = new small_alloc_block<_Pool>(_pool, _ChunkSize, size);
    return pBlock->allocate();
}

template<class _Pool, size_t _ChunkSize, size_t _MaxObjectSize>
inline void small_allocator<_Pool, _ChunkSize, _MaxObjectSize>::free(void* p, size_t size)
{
    if (size > _MaxObjectSize)
	{
		_pool.free(p, size);
		return;
	}

	small_alloc_block<_Pool>* pBlock = _blocks[size];
	assert(pBlock != 0);
	assert(pBlock->blockSize() == size);
    pBlock->free(p);
}

template<class _Pool, size_t _ChunkSize, size_t _MaxObjectSize>
inline void small_allocator<_Pool, _ChunkSize, _MaxObjectSize>::release()
{
}

} // namespace quark

#endif // _QUARK_SMALL_ALLOC_H_
