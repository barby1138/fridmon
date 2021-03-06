// dinkum.h
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

#ifndef _QUARK_STL_H_
#define _QUARK_STL_H_

#if defined (__DCC__) // DIAB
#include "dinkum/diab/_memory_"
#endif

#if _MSC_VER==1200 // MSVC++ 6.0
#include "dinkum/msvc6/_list_"
#include "dinkum/msvc6/_xtree_"
#include "dinkum/msvc6/_deque_"
#else
#include <list>
#include <xtree>
#include <deque>
#endif // _MSC_VER

#include <strstream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <queue>
#include <numeric>
#include <algorithm>
#include <exception>
#include <limits>
#include <stdexcept>
#include <cassert>
#include <cstdlib>

#if defined (__DCC__) // DIAB
#include "dinkum/diab/allocator.h"
#elif defined (_MSC_VER) && (_MSC_VER == 1200) // MSVC++ 6.0
#include "dinkum/msvc6/allocator.h"
#elif defined (_MSC_VER) && (_MSC_VER > 1200) // MSVC++ 7.x
#include "dinkum/msvc7/allocator.h"
#endif

#include "../../mempool/user_mempool.h"

namespace quark
{

#define USER_STL_ALLOC(_T) quark::allocator<_T, user_mempool> 

typedef std::basic_string<char, std::char_traits<char>, USER_STL_ALLOC(char)> pstring;
typedef std::basic_string<wchar_t, std::char_traits<char>, USER_STL_ALLOC(char)> pwstring;

//////////////////////////////////////////////////
// class pruntime_error
//
class pruntime_error : public std::exception
{
public:
	explicit pruntime_error(const pstring& str)
		:	_str(str)
	{
	}

	virtual const char* what() const throw ()
	{
		return _str.c_str();
	}

protected:
	virtual void _Doraise() const
	{
		_RAISE(*this);
	}

private:
	pstring _str;
};

//////////////////////////////////////////////////
// class pvector
//
template<typename _Ty>
class pvector : public std::vector<_Ty, USER_STL_ALLOC(_Ty)>
{
private:
	typedef typename USER_STL_ALLOC(_Ty) _A;
	typedef std::vector<_Ty, _A> base;
	typedef typename base::const_iterator _It;
	
public:
	explicit pvector(const _A& _Al = _A()) 
		:	base(_Al) {}

	explicit pvector(size_type _N, const _Ty& _V = _Ty(), const _A& _Al = _A())
		:	base(_N, _V, _Al) {}

	pvector(const base& _X)
		:	base(_X) {}

	pvector(_It _F, _It _L, const _A& _Al = _A())
		:	base(_F, _L, _Al) {}
};

//////////////////////////////////////////////////
// class pmap
//
template<class _K, class _Ty, class _Pr = std::less<_K> >
class pmap : public std::map<_K, _Ty, _Pr, USER_STL_ALLOC(_Ty)>
{
private:
	typedef USER_STL_ALLOC(_Ty) _A;
	typedef std::map<_K, _Ty, _Pr, _A> base;
	typedef typename base::const_iterator _It;

public:
	explicit pmap(const _Pr& _Pred = _Pr(), const _A& _Al = _A())
		:	base(_Pred, _Al) {}

	pmap(_It _F, _It _L, const _Pr& _Pred = _Pr(), const _A& _Al = _A())
		:	base (_F, _L, _Pred, _Al) {}
};

//////////////////////////////////////////////////
// class pmultimap
//
template<class _K, class _Ty, class _Pr = std::less<_K> >
class pmultimap : public std::multimap<_K, _Ty, _Pr, USER_STL_ALLOC(_Ty)>
{
private:
	typedef USER_STL_ALLOC(_Ty) _A;
	typedef std::multimap<_K, _Ty, _Pr, _A> base;
	typedef typename base::const_iterator _It;
	
public:
	explicit pmultimap(const _Pr& _Pred = _Pr(), const _A& _Al = _A())
		:	base(_Pred, _Al) {}
	
	pmultimap(_It _F, _It _L, const _Pr& _Pred = _Pr(), const _A& _Al = _A())
		:	base (_F, _L, _Pred, _Al) {}
};

//////////////////////////////////////////////////
// class plist
//
template<typename _Ty>
class plist : public std::list<_Ty, USER_STL_ALLOC(_Ty)>
{
private:
	typedef USER_STL_ALLOC(_Ty) _A;
	typedef std::list<_Ty, _A> base;
	typedef typename base::const_iterator _It;

public:
	explicit plist(const _A& _Al = _A())
		:	base(_Al) {}
	
	explicit plist(size_type _N, const _Ty& _V = _Ty(), const _A& _Al = _A())
		:	base(_N,_V,_Al) {}

	plist(const base& _X)
		:	base(_X) {}

	plist(const _Ty *_F, const _Ty *_L, const _A& _Al = _A())
		:	base(_F,_L,_Al)	{}
	
	plist(_It _F, _It _L, const _A& _Al = _A())
		:	base(_F, _L, _Al) {}
};

//////////////////////////////////////////////////
// class pdeque
//
template<class _Ty>
class pdeque : public std::deque<_Ty, USER_STL_ALLOC(_Ty)>
{
private:
	typedef USER_STL_ALLOC(_Ty) _A;
	typedef std::deque<_Ty, _A> base;
	typedef typename base::const_iterator _It;

public:
	explicit pdeque(const _A& _Al = _A())
		:	base(_Al) {}

	explicit pdeque(size_type _N, const _Ty& _V = _Ty(), const _A& _Al = _A())
		:	base(_N, _V, _Al) {}

	pdeque(const _Myt& _X)
		:	base(_X) {}

	pdeque(_It _F, _It _L, const _A& _Al = _A())
		:	base(_F, _L, _Al) {}
};

//////////////////////////////////////////////////
// class pauto_ptr
//
template<class _Ty>
class pauto_ptr
{
public:
	typedef _Ty element_type;
	
	explicit pauto_ptr(_Ty *_P = 0)
		:	_Owns(_P != 0), _Ptr(_P) {}
	
	pauto_ptr(const pauto_ptr<_Ty>& _Y)
		:	_Owns(_Y._Owns), _Ptr(_Y.release()) {}
	
	pauto_ptr<_Ty>& operator=(const pauto_ptr<_Ty>& _Y)
	{   
		if (this != &_Y)
		{
			if (_Ptr != _Y.get())
			{
				if (_Owns)
					delete _Ptr;
				_Owns = _Y._Owns;
			}
			else if (_Y._Owns)
				_Owns = true;
			_Ptr = _Y.release(); 
		}
		return (*this); 
	}
	
	~pauto_ptr()
	{
		if (_Owns)
			delete _Ptr; 
	}
	
	_Ty& operator*() const
		{ return (*get()); }
	
	_Ty *operator->() const
		{ return (get()); }

	_Ty *get() const
		{ return (_Ptr); }

	_Ty *release() const
	{
		((pauto_ptr<_Ty> *)this)->_Owns = false;
		return (_Ptr); 
	}
	
private:
	bool _Owns;
	_Ty *_Ptr;
};

//////////////////////////////////////////////////
// functor select1st
//
// Returns a reference to the first component of 
// a pair-like type
template<typename _Pair>
struct select1st : std::unary_function<_Pair, typename _Pair::first_type>
{
	typename _Pair::first_type& 
		operator() (const _Pair& x) const
	{
		return x.first;
	}
};
 
//////////////////////////////////////////////////
// functor select2nd
//
// Returns a reference to the second component
// a pair-like type.
template<typename _Pair>
struct select2nd : std::unary_function<_Pair, typename _Pair::second_type>
{
	typename _Pair::second_type& 
		operator() (_Pair& x) const
	{
		return x.second;
	}
};

} // namespace quark

// STL fixes
#undef min
#undef max

namespace std
{

// Resolves conflict with boost
#if !defined(BOOST_CONFIG_HPP) && (_MSC_VER==1200) // MSVC++ 6.0

template<class _T>
inline const _T& max(const _T& a, const _T& b)
{ 
	return (a < b ? b : a);
}

template<class _T>
inline const _T& min(const _T& a, const _T& b)
{ 
	return (b < a ? b : a);
}

#endif // BOOST_CONFIG_HPP

} // namespace std

#endif // _QUARK_STL_H_
