// type_manip.h
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
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any
//     purpose is hereby granted without fee, provided that the above copyright
//     notice appear in all copies and that both that copyright notice and this
//     permission notice appear in supporting documentation.
// The author or Addison-Welsey Longman make no representations about the
//     suitability of this software for any purpose. It is provided "as is"
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

// Last update: Oct 17, 2002

#ifndef _NUCLEON_TYPEMANIP_H_
#define _NUCLEON_TYPEMANIP_H_

namespace nucleon
{
	
namespace prvt
{
	typedef char YES;
	struct NO { char dummy[3]; };
}

////////////////////////////////////////////////////////////////////////////////
// class template select_type
// Selects one of two types based upon a boolean constant
// Invocation: select_type<flag, _T, _U>::Result
// where:
// flag is a compile-time boolean constant
// _T and _U are types
// Result evaluates to _T if flag is true, and to _U otherwise.
////////////////////////////////////////////////////////////////////////////////

// VC6 compatible version
namespace prvt
{
	template <bool>
	struct select_type_impl
	{
		template <class _T, class _U>
		struct in
		{
			typedef _T result;
		};
	};

	template <>
	struct select_type_impl<false>
	{
		template <class _T, class _U>
		struct in
		{
			typedef _U result;
		};
	};

}	// end of namespace prvt

template <bool flag, typename _T, typename _U>
struct select_type
{
	typedef typename prvt::select_type_impl<flag>::template in<_T, _U>::result result;
};

////////////////////////////////////////////////////////////////////////////////
// class template IsEqualType
// Returns true if the two given types are equal
// Invocation: IsEqualType<T, U>::value
// where:
// T and U are types
// Result evaluates to true if U == T (types equal)
////////////////////////////////////////////////////////////////////////////////

//	This template is not in the original Loki-Library
template <class _T, class _U>
struct is_equal_type
{
private:
	static prvt::YES check(quark::type2type<_T>);
	static prvt::NO check(...);
public:
	enum {value = sizeof(check(quark::type2type<_U>())) == sizeof(prvt::YES)};
};

////////////////////////////////////////////////////////////////////////////////
// Helper types Small and Big - guarantee that sizeof(Small) < sizeof(Big)
////////////////////////////////////////////////////////////////////////////////

namespace prvt
{
    typedef char small;
    class big { char dummy[2]; };

	// is_void from Rani Sharoni's VC 7 port
    template<typename _T>
    struct is_void
    {
        enum { value =
            is_equal_type<_T, void>::value          ||
            is_equal_type<_T, const void>::value    ||
            is_equal_type<_T, volatile void>::value ||
            is_equal_type<_T, const volatile void>::value
        };
    };
}

//
// is one type convertable to another?
//
// is_convertible from Rani Sharoni's VC 7 port.
template <class _T, class _U>
class is_convertible
{
    struct void_replace {};

    typedef typename select_type
    <
        prvt::is_void<_T>::value,
        void_replace, _T
    >
    ::result T1;

    typedef typename select_type
    <
        prvt::is_void<_U>::value,
        void_replace, _U
    >
    ::result U1;

    static prvt::big   test(...);
    static prvt::small test(U1);
    static T1 makeT();

public:
    enum { exists = sizeof(test(makeT())) == sizeof(prvt::small) };
};

////////////////////////////////////////////////////////////////////////////////
// class template Conversion
// Figures out the conversion relationships between two types
// Invocations (T and U are types):
// a) Conversion<T, U>::exists
// returns (at compile time) true if there is an implicit conversion from T
// to U (example: Derived to Base)
// b) Conversion<T, U>::exists2Way
// returns (at compile time) true if there are both conversions from T
// to U and from U to T (example: int to char and back)
// c) Conversion<T, U>::sameType
// returns (at compile time) true if T and U represent the same type
//
// Caveat: might not work if T and U are in a private inheritance hierarchy.
////////////////////////////////////////////////////////////////////////////////

//	Conversion-Template from Rani Sharoni's VC 7 port.
template <class _T, class _U>
struct conversion
{
    enum { exists = (is_convertible<_T, _U>::exists) };
    enum { exists2Way = (exists && is_convertible<_U, _T>::exists) };
    enum { sameType = (is_equal_type<_T, _U>::value) };
};

////////////////////////////////////////////////////////////////////////////////
// class template SuperSubclass
// Invocation: SuperSubclass<B, D>::value where B and D are types. 
// Returns true if B is a public base of D, or if B and D are aliases of the 
// same type.
//
// Caveat: might not work if T and U are in a private inheritance hierarchy.
////////////////////////////////////////////////////////////////////////////////

template <class _T, class _U>
struct super_subclass
{
enum { value = (conversion<const volatile _U*, const volatile _T*>::exists &&
              !conversion<const volatile _T*, const volatile void*>::sameType) };
};

////////////////////////////////////////////////////////////////////////////////
// class template SuperSubclassStrict
// Invocation: SuperSubclassStrict<B, D>::value where B and D are types. 
// Returns true if B is a public base of D.
//
// Caveat: might not work if T and U are in a private inheritance hierarchy.
////////////////////////////////////////////////////////////////////////////////

template<class _T,class _U>
struct super_subclass_strict
{
enum { value = (conversion<const volatile _U*, const volatile _T*>::exists &&
             !conversion<const volatile _T*, const volatile void*>::sameType &&
             !conversion<const volatile _T*, const volatile _U*>::sameType) };
};

////////////////////////////////////////////////////////////////////////////////
// macro SUPERSUBCLASS
// Invocation: SUPERSUBCLASS(B, D) where B and D are types.
// Returns true if B is a public base of D, or if B and D are aliases of the
// same type.
//
// Caveat: might not work if T and U are in a private inheritance hierarchy.
////////////////////////////////////////////////////////////////////////////////

#define SUPERSUBCLASS(_T, _U) \
	(::nucleon::conversion<const volatile _U*, const volatile _T*>::exists && \
	!::nucleon::conversion<const volatile _T*, const volatile void*>::sameType)

////////////////////////////////////////////////////////////////////////////////
// macro SUPERSUBCLASS
// Invocation: SUPERSUBCLASS(B, D) where B and D are types.
// Returns true if B is a public base of D.
//
// Caveat: might not work if T and U are in a private inheritance hierarchy.
////////////////////////////////////////////////////////////////////////////////

#define SUPERSUBCLASS_STRICT(_T, _U) \
	(SUPERSUBCLASS(_T, _U) && \
	!::nucleon::conversion<const volatile _T *, const volatile _U *>::sameType)

template <unsigned i>
struct type_tag
{
	struct inner {char c[i];};
	typedef inner X;
	//STATIC_SIZE_ASSERT(X, i);
};

} // namespace nucleon

////////////////////////////////////////////////////////////////////////////////
// Change log:
// June 20, 2001: ported by Nick Thurn to gcc 2.95.3. Kudos, Nick!!!
// May  10, 2002: ported by Rani Sharoni to VC7 (RTM - 9466)
// Sept 29, 2002: ported by Benjamin Kaufmann to MSVC 6
////////////////////////////////////////////////////////////////////////////////

#endif // _NUCLEON_TYPEMANIP_H_
