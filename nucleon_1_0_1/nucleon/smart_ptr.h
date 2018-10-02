// smart_ptr.h
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Nucleon Library
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
// The author or Addison-Wesley Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

// Last update: Nov 19, 2002

// replaced all template template parameters with 'normal' parameters
// For each Policy there is now a wrapper-class (non template class) 
// containing a nested template class called In which
// provides a typedef (type) to the real Policy-class
// 
// VC special: The MSVC 6.0 introduces an order-dependency for template ctor 
// resp. template assignemt operators. 
// If you need both a copy-ctor and a template copy ctor (same for copy-assignment), then
// you *must* write the templated version first.
// So instead of
//	template <class T>
//	struct Foo
//	{
//		Foo(const Foo&)
//		{}
//		template <class U>
//		Foo(const Foo<U>& r)
//		{}
//	};
// you *need* to write:
//	template <class T>
//	struct Foo
//	{
//		template <class U>
//		Foo(const Foo<U>& r)
//		{}
//		
//		Foo(const Foo& r)
//		{}
//	};
//
// Many thanks to Nelson Elói for pointing that out and for providing me
// with this solution

#ifndef _NUCLEON_SMARTPTR_H_
#define _NUCLEON_SMARTPTR_H_

////////////////////////////////////////////////////////////////////////////////
// IMPORTANT NOTE
// Due to threading issues, the OwnershipPolicy has been changed as follows:
//     Release() returns a boolean saying if that was the last release
//        so the pointer can be deleted by the StoragePolicy
//     IsUnique() was removed
////////////////////////////////////////////////////////////////////////////////

#include <quark/config.h>
#include "type_manip.h"
#include "helpers.h"
#include <quark/threads.h>

namespace nucleon
{

////////////////////////////////////////////////////////////////////////////////
// class template DefaultSPStorage
// Implementation of the StoragePolicy used by SmartPtr
////////////////////////////////////////////////////////////////////////////////
	
template <class T>
class default_sp_storage
{
public:
    typedef T* stored_type;    // the type of the pointee_ object
    typedef T* pointer_type;   // type returned by operator->
    typedef T& reference_type; // type returned by operator*

public:
    default_sp_storage() : pointee_(default_value()) 
    {}

    // The storage policy doesn't initialize the stored pointer 
    //     which will be initialized by the OwnershipPolicy's Clone fn
	
	// do not alter the order of the following two constructors
	// otherwise the MSVC 6.0 will not compile the class.
	template <class U>
	default_sp_storage(const default_sp_storage<U>&)  {}

	default_sp_storage(const default_sp_storage&)
    {}

	default_sp_storage(const stored_type& p) : pointee_(p) {}
    
    pointer_type operator->() const { return pointee_; }
    
    reference_type operator*() const { return *pointee_; }
    
    void swap(default_sp_storage& rhs)
    { std::swap(pointee_, rhs.pointee_); }

    // Accessors
    friend pointer_type get_impl(const default_sp_storage& sp)
		{ return sp.get_impl_mem(); }

    friend inline const stored_type& get_impl_ref(const default_sp_storage& sp)
		{ return sp.get_impl_ref_mem(); }

    friend inline stored_type& get_impl_ref(default_sp_storage& sp)
		{ return sp.get_impl_ref_mem(); }

protected:
    // Destroys the data stored
    // (Destruction might be taken over by the OwnershipPolicy)
    void destroy()
    { delete pointee_; }
    
    // Default value to initialize the pointer
    static stored_type default_value()
    { return 0; }

	pointer_type get_impl_mem() const
		{ return pointee_; }

	stored_type& get_impl_ref_mem()
		{ return pointee_; }

	const stored_type& get_impl_ref_mem() const
		{ return pointee_; }

private:
    // Data
    stored_type pointee_;
};

struct default_sp_storage_wrapper
{
	template <class _T>
	struct in
	{
		typedef default_sp_storage<_T> type;
	};
};
    
////////////////////////////////////////////////////////////////////////////////
// class template RefCounted
// Implementation of the OwnershipPolicy used by SmartPtr
// Provides a classic external reference counting implementation
////////////////////////////////////////////////////////////////////////////////
	
template <class _P>
class ref_counted
{
public:
    ref_counted() 
    {
        pCount_ = (unsigned int*)quark::user_mempool::instance().allocate(sizeof(unsigned int));
        assert(pCount_);
        *pCount_ = 1;
    }
    
	// do not alter the order of the following two constructors
	// otherwise the MSVC 6.0 will fail to compile the class.

	// MWCW lacks template friends, hence the following kludge
    template <typename _P1>
    ref_counted(const ref_counted<_P1>& rhs) 
    : pCount_(reinterpret_cast<const ref_counted&>(rhs).pCount_)
    {}

	ref_counted(const ref_counted& rhs) 
    : pCount_(rhs.pCount_)
    {}
    
    _P clone(const _P& val)
    {
        ++*pCount_;
        return val;
    }
    
    bool release(const _P&)
    {
        if (!--*pCount_)
        {
            quark::user_mempool::instance().free(pCount_, sizeof(unsigned int));
            return true;
        }
        return false;
    }
    
    void swap(ref_counted& rhs)
	{ 
		std::swap(pCount_, rhs.pCount_); 
	}

    enum { destructiveCopy = false };
	
private:
    // data
    unsigned int* pCount_;
};

struct ref_counted_wrapper
{
	template<class _T>
	struct in
	{
		typedef ref_counted<_T> type;
	};
};

////////////////////////////////////////////////////////////////////////////////
// class template RefCountedMT
// Implementation of the OwnershipPolicy used by SmartPtr
// Implements external reference counting for multithreaded programs
////////////////////////////////////////////////////////////////////////////////

template <class _P>
class ref_counted_mt
{
public:
	ref_counted_mt() 
	{
		pCount_ = (int*)quark::user_mempool::instance().allocate(sizeof(int));
		assert(pCount_);
		*pCount_ = 1;
	}
	
	// MWCW lacks template friends, hence the following kludge
	template <typename _P1>
    ref_counted_mt(const ref_counted_mt<_P1>& rhs)
        : pCount_(reinterpret_cast<const ref_counted_mt<_P>&>(rhs).pCount_)
	{}

	ref_counted_mt(const ref_counted_mt& rhs) 
        : pCount_(rhs.pCount_)
	{}
	
	_P clone(const _P& val)
	{
		quark::interlocked::increment(*pCount_);
		return val;
	}
	
	bool release(const _P&)
	{
		if (!quark::interlocked::decrement(*pCount_))
		{
			quark::user_mempool::instance().free(pCount_, sizeof(int));
			return true;
		}
		return false;
	}
	
	void swap(ref_counted_mt& rhs)
	{ 
		std::swap(pCount_, rhs.pCount_); 
	}
    
	enum { destructiveCopy = false };
	
private:
	// data
	int* pCount_;
};

struct ref_counted_mt_wrapper
{
	template<class _T>
	struct in
	{
		typedef ref_counted_mt<_T> type;
	};
};

////////////////////////////////////////////////////////////////////////////////
// class template AllowConversion
// Implementation of the ConversionPolicy used by SmartPtr
// Allows implicit conversion from SmartPtr to the pointee type
////////////////////////////////////////////////////////////////////////////////

struct allow_conversion
{
	enum { allow = true };
	
	void swap(allow_conversion&)
	{}
};

////////////////////////////////////////////////////////////////////////////////
// class template DisallowConversion
// Implementation of the ConversionPolicy used by SmartPtr
// Does not allow implicit conversion from SmartPtr to the pointee type
// You can initialize a DisallowConversion with an AllowConversion
////////////////////////////////////////////////////////////////////////////////

struct disallow_conversion
{
	disallow_conversion()
	{}
	
	disallow_conversion(const allow_conversion&)
	{}
	
	enum { allow = false };
	
	void swap(disallow_conversion&)
	{}
};

////////////////////////////////////////////////////////////////////////////////
// class template NoCheck
// Implementation of the CheckingPolicy used by SmartPtr
// Well, it's clear what it does :o)
////////////////////////////////////////////////////////////////////////////////
	
template <class P>
struct no_check
{
	no_check()
	{}
	
	template <class P1>
    no_check(const no_check<P1>&)
	{}
	
	static void on_default(const P&)
	{}
	
	static void on_init(const P&)
	{}
	
	static void on_dereference(const P&)
	{}
	
	static void swap(no_check&)
	{}
};

struct no_check_wrapper
{
	template <class _T>
	struct in
	{
		typedef no_check<_T> type;
	};
};

////////////////////////////////////////////////////////////////////////////////
// class template AssertCheck
// Implementation of the CheckingPolicy used by SmartPtr
// Checks the pointer before dereference
////////////////////////////////////////////////////////////////////////////////
	
template <class P>
struct assert_check
{
	assert_check()
	{}
	
	template <class P1>
    assert_check(const assert_check<P1>&)
	{}
	
	template <class P1>
    assert_check(const no_check<P1>&)
	{}
	
	static void on_default(const P&)
	{}
	
	static void on_init(const P&)
	{}
	
	static void on_dereference(P val)
	{ assert(val); }
	
	static void swap(assert_check&)
	{}
};

struct assert_check_wrapper
{
	template <class _T>
	struct in { typedef assert_check<_T> type; };
};

////////////////////////////////////////////////////////////////////////////////
// class template by_ref
// Transports a reference as a value
// Serves to implement the Colvin/Gibbons trick for SmartPtr
////////////////////////////////////////////////////////////////////////////////

template <class _T>
class by_ref
{
public:
	by_ref(_T& v) : value_(v) {}
	operator _T&() { return value_; }
	// gcc doesn't like this:
	// operator const T&() const { return value_; }
private:
	_T& value_;
};

////////////////////////////////////////////////////////////////////////////////
// class template SmartPtr (declaration)
// The reason for all the fuss above
////////////////////////////////////////////////////////////////////////////////

template
<
	typename T,
	class OwnershipPolicy = ref_counted_mt_wrapper,
	class ConversionPolicy = disallow_conversion,
	class CheckingPolicy = assert_check_wrapper,
	class StoragePolicy = default_sp_storage_wrapper
>
class smart_ptr;

////////////////////////////////////////////////////////////////////////////////
// class template SmartPtr (definition)
////////////////////////////////////////////////////////////////////////////////

namespace prvt
{
	template <class T, class Ow, class Con, class Check, class Stor>
	struct smart_ptr_impl
	{
		struct Dummy{};
		typedef typename apply_inner_type<Stor, T>::type TempType;
		typedef vc_base_workaround<TempType, Dummy> sttype;
		// VC 6.0 will emit an "Error C2516. : is not a legal base class"
		// if one tries to use TempType as base class for SmartPtr.
		// Don't know why the compiler is happy with this workaround
		typedef typename sttype::left_base storage; 
		typedef typename storage::pointer_type pointer_type;
		typedef typename storage::stored_type stored_type;
		typedef typename storage::reference_type reference_type;

		typedef typename apply_inner_type<Ow, pointer_type>::type Temp2Type;
		typedef typename apply_inner_type<Check, stored_type>::type Temp3Type;

		typedef vc_base_workaround<Temp2Type, Dummy> owtype;
		typedef typename owtype::left_base Owner;

		typedef vc_base_workaround<Temp3Type, Dummy> chtype;
		typedef typename chtype::left_base Checking;

		typedef Con Conversion;
	};
}

template
<
    typename T,
    class OwnershipPolicy,
    class ConversionPolicy,
    class CheckingPolicy,
    class StoragePolicy
>
class smart_ptr
	: public prvt::smart_ptr_impl<T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy>::storage
    , public prvt::smart_ptr_impl<T, OwnershipPolicy, ConversionPolicy,CheckingPolicy, StoragePolicy>::Owner
    , public prvt::smart_ptr_impl<T, OwnershipPolicy, ConversionPolicy,CheckingPolicy, StoragePolicy>::Checking
    , public prvt::smart_ptr_impl<T, OwnershipPolicy, ConversionPolicy,CheckingPolicy, StoragePolicy>::Conversion
{
public:
    typedef typename prvt::smart_ptr_impl
	<	
		T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
	>::storage SP;
	typedef typename SP::pointer_type pointer_type;
    typedef typename SP::stored_type stored_type;
    typedef typename SP::reference_type reference_type;
    typedef T class_type;
    
	typedef typename prvt::smart_ptr_impl
	<	
		T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
	>::Owner OP;
    typedef typename prvt::smart_ptr_impl
	<	
		T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
	>::Checking KP;
	
	typedef ConversionPolicy CP;
    typedef typename select_type
	<
		OP::destructiveCopy, 
		smart_ptr, const smart_ptr
	>::result CopyArg;

    // i think the following two ctors have an exception-safety problem
	// in the original version. If KP throws one can't release the resources
	// which were possibly allocated by SP and/or OP.
	// don't know if my solution works.
	smart_ptr()
    { 
		try
		{
			KP::on_default(get_impl(*this));
		}
		catch(...)
		{
			if (OP::release(get_impl(*static_cast<SP*>(this))))
    		{
    			SP::destroy();
    		}		
			throw;
		}
	}
    
    smart_ptr(const stored_type& p) : SP(p)
    { 
		try
		{
			KP::on_init(get_impl(*this)); 
		}
		catch(...)
		{
			if (OP::release(get_impl(*static_cast<SP*>(this))))
    		{
    			SP::destroy();
    		}		
			throw;
		}
	}
	
	// do not alter the order of the following three constructors
	// otherwise the MSVC 6.0 will fail to compile the class.
	template
    <
        typename T1,
        class OP1,
        class CP1,
        class KP1,
        class SP1
    >
    smart_ptr(const smart_ptr<T1, OP1, CP1, KP1, SP1>& rhs)
    : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
    { get_impl_ref_mem() = OP::clone(get_impl_ref(rhs)); }

    template
    <
        typename T1,
        class OP1,
        class CP1,
        class KP1,
        class SP1
    >
    smart_ptr(smart_ptr<T1, OP1, CP1, KP1, SP1>& rhs)
    : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
    { get_impl_ref_mem() = OP::clone(get_impl_ref(rhs)); }

	smart_ptr(CopyArg& rhs)
    : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
	{ get_impl_ref_mem() = OP::clone(get_impl_ref(rhs)); }
    operator by_ref<smart_ptr>()
    { return by_ref<smart_ptr>(*this); }

	
	// do not alter the order of the following three copy-assignment operators
	// otherwise the MSVC 6.0 will fail to compile the class.
	template
    <
        typename T1,
        class OP1,
        class CP1,
        class KP1,
        class SP1
    >
    smart_ptr& operator=(const smart_ptr<T1, OP1, CP1, KP1, SP1>& rhs)
    {
    	smart_ptr temp(rhs);
    	temp.swap(*this);
    	return *this;
    }
    
    template
    <
        typename T1,
        class OP1,
        class CP1,
        class KP1,
        class SP1
    >
    smart_ptr& operator=(smart_ptr<T1, OP1, CP1, KP1, SP1>& rhs)
    {
    	
		smart_ptr temp(rhs);
    	temp.swap(*this);
    	return *this;
    }

	smart_ptr& operator=(CopyArg& rhs)
    {
    	smart_ptr temp(rhs);
    	temp.swap(*this);
    	return *this;
    }
    void swap(smart_ptr& rhs)
    {
    	OP::swap(rhs);
    	CP::swap(rhs);
    	KP::swap(rhs);
    	SP::swap(rhs);
    }
    
    ~smart_ptr()
    {
    	if (OP::release(get_impl_mem()))
    	{
    	    SP::destroy();
    	}
    }
    
    friend inline void release(smart_ptr& sp, stored_type& p)
    {
    	p = get_impl_ref(sp);
    	get_impl_ref(sp) = SP::default_value();
    }
    
    friend inline void reset(smart_ptr& sp, stored_type p)
    { smart_ptr(p).swap(sp); }

	pointer_type operator->()
    {
        KP::on_dereference(get_impl_ref_mem());
        return SP::operator->();
    }

    pointer_type operator->() const
    {
        KP::on_dereference(get_impl_ref_mem());
        return SP::operator->();
    }

    reference_type operator*()
    {
        KP::on_dereference(get_impl_ref_mem());
        return SP::operator*();
    }
    
    reference_type operator*() const
    {
        KP::on_dereference(get_impl_ref_mem());
        return SP::operator*();
    }
    
    bool operator!() const // Enables "if (!sp) ..."
    { return get_impl(*this) == 0; }
    
    inline friend bool operator==(const smart_ptr& lhs,
        const T* rhs)
    { return get_impl(lhs) == rhs; }
    
    inline friend bool operator==(const T* lhs,
        const smart_ptr& rhs)
    { return rhs == lhs; }
    
    inline friend bool operator!=(const smart_ptr& lhs,
        const T* rhs)
    { return !(lhs == rhs); }
    
    inline friend bool operator!=(const T* lhs,
        const smart_ptr& rhs)
    { return rhs != lhs; }

    // Ambiguity buster
    template
    <
        typename T1,
        class OP1,
        class CP1,
        class KP1,
        class SP1
    >
    bool operator==(const smart_ptr<T1, OP1, CP1, KP1, SP1>& rhs) const
    { return *this == get_impl(rhs); }

    // Ambiguity buster
    template
    <
        typename T1,
        class OP1,
        class CP1,
        class KP1,
        class SP1
    >
    bool operator!=(const smart_ptr<T1, OP1, CP1, KP1, SP1>& rhs) const
    { return !(*this == rhs); }

    // Ambiguity buster
    template
    <
        typename T1,
        class OP1,
        class CP1,
        class KP1,
        class SP1
    >
    bool operator<(const smart_ptr<T1, OP1, CP1, KP1, SP1>& rhs) const
    { return *this < get_impl(rhs); }

#if defined(__DCC__)
    operator bool() const
    { return get_impl(*this) != 0; }

#else
private:
    // Helper for enabling 'if (sp)'
    struct tester
    {
        tester() {}
    private:
        void operator delete(void*);
    };
    
public:
    // enable 'if (sp)'
    operator tester*() const
    {
        if (!*this) return 0;
        static tester t;
        return &t;
    }
#endif

private:
    // Helper for disallowing automatic conversion
    struct insipid
    {
        insipid(pointer_type) {}
    };
    
    typedef typename select_type<CP::allow, pointer_type, insipid>::result
        automatic_conversion_result;

public:        
    operator automatic_conversion_result() const
    { return get_impl(*this); }
};

////////////////////////////////////////////////////////////////////////////////
// free comparison operators for class template smart_ptr
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// operator== for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

template
<
    typename T,
    class OP,
    class CP,
    class KP,
    class SP,
    typename U
>
inline bool operator==(const smart_ptr<T, OP, CP, KP, SP>& lhs,
    const U* rhs)
{ return get_impl(lhs) == rhs; }
    
////////////////////////////////////////////////////////////////////////////////
// operator== for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

template
<
    typename T,
    class OP,
    class CP,
    class KP,
    class SP,
    typename U
>
inline bool operator==(const U* lhs,
    const smart_ptr<T, OP, CP, KP, SP>& rhs)
{ return rhs == lhs; }

////////////////////////////////////////////////////////////////////////////////
// operator!= for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

template
<
    typename T,
    class OP,
    class CP,
    class KP,
    class SP,
    typename U
>
inline bool operator!=(const smart_ptr<T, OP, CP, KP, SP>& lhs,
    const U* rhs)
{ return !(lhs == rhs); }
    
////////////////////////////////////////////////////////////////////////////////
// operator!= for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

template
<
    typename T,
    class OP,
    class CP,
    class KP,
    class SP,
    typename U
>
inline bool operator!=(const U* lhs,
    const smart_ptr<T, OP, CP, KP, SP>& rhs)
{ return rhs != lhs; }

////////////////////////////////////////////////////////////////////////////////
// operator< for lhs = smart_ptr, rhs = raw pointer -- NOT DEFINED
////////////////////////////////////////////////////////////////////////////////

template
<
    typename T,
    class OP,
    class CP,
    class KP,
    class SP,
    typename U
>
inline bool operator<(const smart_ptr<T, OP, CP, KP, SP>& lhs,
    const U* rhs);
        
////////////////////////////////////////////////////////////////////////////////
// operator< for lhs = raw pointer, rhs = smart_ptr -- NOT DEFINED
////////////////////////////////////////////////////////////////////////////////

template
<
    typename T,
    class OP,
    class CP,
    class KP,
    class SP,
    typename U
>
inline bool operator<(const U* lhs,
    const smart_ptr<T, OP, CP, KP, SP>& rhs);
        
////////////////////////////////////////////////////////////////////////////////
// operator> for lhs = smart_ptr, rhs = raw pointer -- NOT DEFINED
////////////////////////////////////////////////////////////////////////////////

template
<
    typename T,
    class OP,
    class CP,
    class KP,
    class SP,
    typename U
>
inline bool operator>(const smart_ptr<T, OP, CP, KP, SP>& lhs,
    const U* rhs)
{ return rhs < lhs; }
        
////////////////////////////////////////////////////////////////////////////////
// operator> for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

template
<
    typename T,
    class OP,
    class CP,
    class KP,
    class SP,
    typename U
>
inline bool operator>(const U* lhs,
    const smart_ptr<T, OP, CP, KP, SP>& rhs)
{ return rhs < lhs; }
  
////////////////////////////////////////////////////////////////////////////////
// operator<= for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

template
<
    typename T,
    class OP,
    class CP,
    class KP,
    class SP,
    typename U
>
inline bool operator<=(const smart_ptr<T, OP, CP, KP, SP>& lhs,
    const U* rhs)
{ return !(rhs < lhs); }
        
////////////////////////////////////////////////////////////////////////////////
// operator<= for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

template
<
    typename T,
    class OP,
    class CP,
    class KP,
    class SP,
    typename U
>
inline bool operator<=(const U* lhs,
    const smart_ptr<T, OP, CP, KP, SP>& rhs)
{ return !(rhs < lhs); }

////////////////////////////////////////////////////////////////////////////////
// operator>= for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

template
<
    typename T,
    class OP,
    class CP,
    class KP,
    class SP,
    typename U
>
inline bool operator>=(const smart_ptr<T, OP, CP, KP, SP>& lhs,
    const U* rhs)
{ return !(lhs < rhs); }
        
////////////////////////////////////////////////////////////////////////////////
// operator>= for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

template
<
    typename T,
    class OP,
    class CP,
    class KP,
    class SP,
    typename U
>
inline bool operator>=(const U* lhs,
    const smart_ptr<T, OP, CP, KP, SP>& rhs)
{ return !(lhs < rhs); }

} // namespace nucleon

////////////////////////////////////////////////////////////////////////////////
// specialization of std::less for smart_ptr
////////////////////////////////////////////////////////////////////////////////

// MSVC 6.0 does not support partial template specialization :-(
#if !defined(_MSC_VER)
namespace std
{
    template
    <
        typename T,
        class OP,
        class CP,
        class KP,
        class SP
    >
    struct less< nucleon::smart_ptr<T, OP, CP, KP, SP> >
        : public binary_function<nucleon::smart_ptr<T, OP, CP, KP, SP>,
            nucleon::smart_ptr<T, OP, CP, KP, SP>, bool>
    {
        bool operator()(const nucleon::smart_ptr<T, OP, CP, KP, SP>& lhs,
            const nucleon::smart_ptr<T, OP, CP, KP, SP>& rhs) const
        { return less<T*>()(get_impl(lhs), get_impl(rhs)); }
    };
}
#endif

////////////////////////////////////////////////////////////////////////////////
// Change log:
// June 20, 2001: ported by Nick Thurn to gcc 2.95.3. Kudos, Nick!!!
// December 09, 2001: Included <cassert>
// Oct  26, 2002: ported by Benjamin Kaufmann to MSVC 6.0
////////////////////////////////////////////////////////////////////////////////

#endif // _NUCLEON_SMARTPTR_H_
