// singleton.h
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

#ifndef _QUARK_SINGLETON_H_
#define _QUARK_SINGLETON_H_

#include "config.h"
#include "threads.h"

namespace quark
{

namespace prvt
{

//////////////////////////////////////////////////
// class lifetime_tracker
//
/*! Helper class for setLongevity
*/
class lifetime_tracker : public pooled_object
{
public:
    lifetime_tracker(u32 x)
		:	_longevity(x) {}
    virtual ~lifetime_tracker() = 0;
    
    static bool compare(const lifetime_tracker* lhs, 
						const lifetime_tracker* rhs)
		{ return rhs->_longevity > lhs->_longevity; }
    
private:
    u32 _longevity;
};

inline lifetime_tracker::~lifetime_tracker() {}

// Helper data
typedef lifetime_tracker** tracker_array;
extern tracker_array pTrackerArray;
extern u32 elements;

// Helper destroyer function
template <typename _T>
struct destroyer
{
    static void destroy(_T* pObj)
		{ delete pObj; }
};

// Concrete lifetime tracker for objects of type T
template <typename _T, typename _Destroyer>
class concrete_lifetime_tracker : public lifetime_tracker
{
public:
    concrete_lifetime_tracker(_T* p, u32 longevity, _Destroyer d)
        :	lifetime_tracker(longevity),
			_pTracked(p),
			_destroyer(d)
    {}
    
    ~concrete_lifetime_tracker()
		{ _destroyer(_pTracked); }
    
private:
    _T* _pTracked;
    _Destroyer _destroyer;
};

void atExitFn(); // declaration needed below

} // namespace prvt

//////////////////////////////////////////////////
// function template setLongevity
//
/*! Assigns an object a longevity
	Ensures ordered destructions of objects registered thusly
	during the exit sequence of the application
*/
template <typename _T, typename _Destroyer>
void setLongevity(_T* pDynObject, u32 longevity,
    _Destroyer d)
{
    using namespace prvt;
    
    tracker_array pNewArray = static_cast<tracker_array>(
            VC_BROKEN_STD::realloc(pTrackerArray, sizeof(*pTrackerArray) * (elements + 1)));
    if (!pNewArray) throw std::bad_alloc();
    
    // Delayed assignment for exception safety
    pTrackerArray = pNewArray;

    lifetime_tracker* p = new concrete_lifetime_tracker<_T, _Destroyer>(
        pDynObject, longevity, d);
            
    // Insert a pointer to the object into the queue
    tracker_array pos = std::upper_bound(
        pTrackerArray, 
        pTrackerArray + elements, 
        p, 
        lifetime_tracker::compare);
    std::copy_backward(
        pos, 
        pTrackerArray + elements,
        pTrackerArray + elements + 1);
    *pos = p;
    ++elements;
    
    // Register a call to AtExitFn
    VC_BROKEN_STD::atexit(prvt::atExitFn);
}

//////////////////////////////////////////////////
// class create_using_new
//
/*! Implementation of the _CreationPolicy used by singleton_holder
	Creates objects using a straight call to the new operator 
*/
struct create_using_new
{
	template <class _T>
	static _T* create(type2type<_T>)
		{ return new _T; }

	template <class _T>
	static void destroy(_T* p)
		{ delete p; }
};

//////////////////////////////////////////////////
// class create_using_malloc
//
/*! Implementation of the _CreationPolicy used by singleton_holder
	Creates objects using a call to std::malloc, followed by a call
	to the placement new operator
*/
struct create_using_malloc
{
	template <class _T>
	static _T* create(type2type<_T>)
    {
        void* p = VC_BROKEN_STD::malloc(sizeof(_T));
        if (!p) return 0;
        return new(p)_T;
    }
    
    template <class _Type> 
	static void destroy(_Type* p)
    {
        p->~_Type();
        VC_BROKEN_STD::free((void*)p);
    }
};

//////////////////////////////////////////////////
// class create_static
//
/*! Implementation of the _CreationPolicy used by singleton_holder
	Creates an object in static memory
	Implementation is slightly nonportable because it uses the MaxAlign trick 
	(an union of all types to ensure proper memory alignment). This trick is 
	nonportable in theory but highly portable in practice.
*/
struct create_static
{
#ifdef _MSC_VER
#pragma warning( push )
// alignment of a member was sensitive to packing
#pragma warning( disable : 4121 )
#endif // _MSC_VER   
	template <class _T>
	union max_align
    {
        char _t[sizeof(_T)];
        short int _shortInt;
        int _int;
        long int _longInt;
        float _float;
        double _double;
        long double _longDouble;
        struct Test;
        int Test::*_pMember;
        int (Test::*_pMemberFn)(int);
    };
#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER
    
    template <class _T> 
	static _T* create(type2type<_T>)
    {
        static max_align<_T> staticMemory;
        return ::new(&staticMemory)_T;
    }
    
    template <class _Type> 
	static void destroy(_Type* p)
    {
        p->~_Type();
    }
};

//////////////////////////////////////////////////
// class default_lifetime
//
/*! Implementation of the _LifetimePolicy used by singleton_holder
	Schedules an object's destruction as per C++ rules
	Forwards to std::atexit
*/
struct default_lifetime
{
	template <class _T>
	static void scheduleDestruction(_T*, void (*pFun)())
		{ VC_BROKEN_STD::atexit(pFun); }

	template <class _T>
	static void onDeadReference(const volatile _T* p = 0 )
		{ throw std::logic_error("Dead Reference Detected"); }
};

namespace prvt
{

//////////////////////////////////////////////////
// class template adapter
//
/*! Helper for singleton_with_longevity below
*/
template <class _T>
struct adapter
{
    void operator()(_T*) { _pFun(); }
    void (*_pFun)();
};

} // namespace prvt

//////////////////////////////////////////////////
// class singleton_with_longevity
//
/*! Implementation of the lifetime_policy used by singleton_holder
	Schedules an object's destruction in order of their longevities
	Assumes a visible function getLongevity(T*) that returns the 
	longevity of the object
*/
class singleton_with_longevity
{
public:
    template <class _T>
	static void scheduleDestruction(_T* pObj, void (*pFun)())
    {
        prvt::adapter<_T> adap;
		adap._pFun = pFun;
        setLongevity(pObj, getLongevity(pObj), adap);
    }
    
    template <class _T>
	static void onDeadReference(const volatile _T* p = 0 )
		{ throw std::logic_error("dead reference detected"); }
};

//////////////////////////////////////////////////
// class template singleton_holder
//
/*! Provides Singleton amenities for a type _T
	To protect that type from spurious instantiations, you have to protect it
	yourself.
*/
template
<
    typename _T,
    class _CreationPolicy = create_static,
    class _LifetimePolicy = default_lifetime,
    class _ThreadingModel = class_level_lockable<_T>
>
class singleton_holder
{
public:
	typedef _T holdee_type;

public:
    static _T& instance();
    
private:
	singleton_holder();

    static void makeInstance();
    static void destroySingleton();
    
private:
	static volatile _T* _pInstance;
    static bool _destroyed;
};
    
//////////////////////////////////////////////////
// singleton_holder implementation
//////////////////////////////////////////////////

template<class _T, class _C, class _L, class _M>
volatile _T* singleton_holder<_T, _C, _L, _M>::_pInstance;

template<class _T, class _C, class _L, class _M>
bool singleton_holder<_T, _C, _L, _M>::_destroyed;

template<class _T, class _C, class _L, class _M>
inline _T& singleton_holder<_T, _C, _L, _M>::instance()
{
	if (!_pInstance)
		makeInstance();

	return (_T&)*_pInstance;
}

template
<
	class _T,
	class _CreationPolicy,
	class _LifetimePolicy,
	class _ThreadingModel
> 
void singleton_holder<_T, _CreationPolicy, _LifetimePolicy,
					  _ThreadingModel>::makeInstance()
{
	typename _ThreadingModel::lock guard;
	(void)guard;
	
	if (!_pInstance)
	{
		if (_destroyed)
		{
			_LifetimePolicy::onDeadReference(_pInstance);
			_destroyed = false;
		}
		_pInstance = _CreationPolicy::create(type2type<_T>());
		_LifetimePolicy::scheduleDestruction(_pInstance, 
			&destroySingleton);
	}
}

template
<
	class _T,
	class _CreationPolicy,
	class _L,
	class _M
>
void singleton_holder<_T, _CreationPolicy, _L, _M>::destroySingleton()
{
	assert(!_destroyed);
	
	_CreationPolicy::destroy(_pInstance);
	_pInstance = 0;
	_destroyed = true;
}
	
} // namespace quark

#endif // _QUARK_SINGLETON_H_
