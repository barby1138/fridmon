// common_templates.h
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

#ifndef _QUARK_COMMON_TEMPLATES_H_
#define _QUARK_COMMON_TEMPLATES_H_

namespace quark
{

// Resolves conflict with Loki
#ifndef STATIC_CHECK_INC_
#define STATIC_CHECK_INC_

template<int> struct compile_time_error;
template<> struct compile_time_error<true> {};
template<int x> struct static_assert_test {};

#define STATIC_CHECK(expr, msg) \
    { compile_time_error<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; } 

#endif // STATIC_CHECK_INC_

template<class _T>
inline pauto_ptr<_T> make_auto_ptr(_T* ptr)
{
	return pauto_ptr<_T>(ptr);
}

//////////////////////////////////////////////////
// functor cleaner_ptr
//
/*!
	This functor usually is used to delete objects 
	from STL containers using the for_each function.
*/
struct cleaner_ptr
{
	template<class _Key, class _Val>
	void operator()(const std::pair<_Key, _Val>& pr)
		{ delete pr.second; }

	template<class _Ptr>
	void operator()(const _Ptr* p) const
		{ delete p; }

	template<class _Ptr>
	void operator()(_Ptr* p)
		{ delete p; }
};

//////////////////////////////////////////////////
// functor cleaner_ref
//
struct cleaner_ref
{
	template<class _Ref>
	void operator()(const _Ref& ) const
		{}

	template<class _Ref>
	void operator()(_Ref& )
		{}
};

//////////////////////////////////////////////////
// class noncopyable
//
class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}

private:
	noncopyable(const noncopyable& );
    const noncopyable& operator=(const noncopyable& );
};

//////////////////////////////////////////////////
// class template type2type
//
template<typename _T>
struct type2type
{
	typedef _T original_type;
};

//////////////////////////////////////////////////
// class template int2type
//
template<int v>
struct int2type
{
	enum { value = v };
};

} // namespace quark

#endif // _QUARK_COMMON_TEMPLATES_H_
