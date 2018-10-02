// user_mempool.h
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

#ifndef _QUARK_USER_MEMPOOL_H_
#define _QUARK_USER_MEMPOOL_H_

// #define _NO_MEMPOOL // to disable memory pool uncomment and rebuild application

namespace quark
{

//////////////////////////////////////////////////
// class user_mempool
//
class user_mempool
{
private:
	typedef void* (*alloc_fn) (size_t);
	typedef void (*free_fn) (void*, size_t);

public:

#ifndef _NO_MEMPOOL
	void* allocate(size_t size)
	{
		if (_fnAlloc == 0)
			defaultBind();
		assert(_fnAlloc);
		return _fnAlloc(size);
	}

	void free(void* p, size_t size)
	{
		assert(_fnFree);
		_fnFree(p, size);
	}
#else
	void* allocate(size_t size)
	{
		return ::malloc(size); //return ::operator new(size);
	}

	void free(void* p, size_t size)
	{
		::free(p); //::operator delete(p);
	}
#endif // _NO_MEMPOOL

	// TODO: protect bind with lock
	bool bind(alloc_fn fnAlloc, free_fn fnFree);

	static user_mempool& instance();

private:
	user_mempool();

	void defaultBind();

private:
	volatile alloc_fn	_fnAlloc;
	free_fn				_fnFree;
};

//////////////////////////////////////////////////
// class template pooled_objectT
//
template<class _Pool>
class pooled_objectT
{
public:
	virtual ~pooled_objectT() {}

	void* operator new(size_t size)
	{
		return _Pool::instance().allocate(size);
	}

	void operator delete(void* p, size_t size)
	{
		_Pool::instance().free(p, size);
	}

protected:
	pooled_objectT() {}
};

typedef pooled_objectT<user_mempool> pooled_object;

//////////////////////////////////////////////////
// class template binder
//
template<class _Pool>
class binder
{
public:
	static bool bind()
	{
		return user_mempool::instance().bind(onAllocate, onFree);
	}

	static void* onAllocate(size_t size)
	{
		return _Pool::instance().allocate(size);
	}

	static void onFree(void* p, size_t size)
	{
		_Pool::instance().free(p, size);
	}

private:
	binder();
	~binder();
};

template<class T>
bool bindPool()
{
	return binder<T>::bind();
}

} // namespace quark

#endif // _QUARK_USER_MEMPOOL_H_
