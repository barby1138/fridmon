// middle_alloc.h
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

#ifndef _QUARK_MIDDLE_ALLOC_H_
#define _QUARK_MIDDLE_ALLOC_H_

namespace quark
{

//////////////////////////////////////////////////
// class middle_allocator
//
template<class _Pool>
class middle_allocator : public noncopyable
{
private:
	struct middle_block
	{
		explicit middle_block(size_t blockSize = 0)
			:	_blockSize(blockSize), _pTop(0) {}

		bool operator<(const middle_block& rhs) const { return _blockSize < rhs._blockSize; }

		void* _pTop;
		size_t _blockSize;
	};
	
	typedef std::vector<middle_block, SERVICE_STL_ALLOC(middle_block)> blocks_t;
	
public:
	typedef _Pool pool_type;
	typedef typename blocks_t::iterator block_t_iterator;
		
public:
	explicit middle_allocator(_Pool& pool);
	~middle_allocator();

	void* allocate(size_t size);
	void free(void* p, size_t size);
	void release();

private:
	blocks_t _blocks;
	_Pool& _pool;
	size_t _allocCounter;
	size_t _freeCounter;
};

// TODO: check allocations for objects < 4 bytes (possible memory override)
template<class _Pool>
middle_allocator<_Pool>::middle_allocator(_Pool& pool)
	:	_pool(pool),
		_blocks(31),
		_allocCounter(0),
		_freeCounter(0)
{
	size_t pow = 0;
	block_t_iterator itor = _blocks.begin();
	for (; itor != _blocks.end(); ++itor)
		itor->_blockSize = 1ULL << pow++;
}

template<class _Pool>
middle_allocator<_Pool>::~middle_allocator()
{
//	assert(_allocCounter == _freeCounter);
}

template<class _Pool>
void* middle_allocator<_Pool>::allocate(size_t size)
{
	++_allocCounter;

	block_t_iterator itor = std::lower_bound(_blocks.begin(), _blocks.end(), middle_block(size));
	assert(itor != _blocks.end());

	void* pTop = itor->_pTop;
	if (pTop != 0)
	{
		void* p = pTop;
		itor->_pTop = *((void**)(pTop));
		return p;
	}
	return _pool.allocate(itor->_blockSize);
}

template<class _Pool>
void middle_allocator<_Pool>::free(void* p, size_t size)
{
	++_freeCounter;

	block_t_iterator itor = std::lower_bound(_blocks.begin(), _blocks.end(), middle_block(size));
	assert(itor != _blocks.end());

	*((void**)p) = itor->_pTop;
	itor->_pTop = p;
}

template<class _Pool>
void middle_allocator<_Pool>::release()
{
}

} // namespace quark

#endif // _QUARK_MIDDLE_ALLOC_H_
