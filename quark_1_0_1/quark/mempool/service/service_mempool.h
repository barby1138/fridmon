// service_mempool.h
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

#ifndef _QUARK_SERVICE_MEMPOOL_H_
#define _QUARK_SERVICE_MEMPOOL_H_

namespace quark
{

//////////////////////////////////////////////////
// class pimple_object
//
class pimple_object
{
public:
	virtual ~pimple_object() {}

	void* operator new(size_t size);
	void operator delete(void* p, size_t size);
	
protected:
	pimple_object() {}
};

//////////////////////////////////////////////////
// class service_object
//
class service_object
{
public:
	virtual ~service_object() {}

	void* operator new(size_t size);
	void operator delete(void* p, size_t size);

protected:
	service_object() {}
};

//////////////////////////////////////////////////
// class service_storage
//
class service_storage
{
public:
	class mutex;

private:
	typedef std::vector<void*> chunks_t;

public:
	~service_storage();

	void* allocate(size_t size);
	void free(void* p, size_t size);

	static service_storage& instance();
	
private:
	service_storage();
	void newChunk();

private:
	u8*		 _pCur;
	size_t	 _remainSize;
	size_t   _totalUsedSize;
	mutex*	 _pLock;
	chunks_t _chunks;
};

#define SERVICE_STL_ALLOC(_T) allocator<_T, service_storage> 

} // namespace quark

#endif // _QUARK_SERVICE_MEMPOOL_H_
