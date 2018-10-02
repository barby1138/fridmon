// hgelpers.h
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

// Last update: Dec 03, 2002
// Thanks to Adi Shavit

#ifndef _NUCLEON_HELPERS_H_
#define _NUCLEON_HELPERS_H_

namespace nucleon
{
	namespace prvt
	{
		// workaround for the "Error C2516. : is not a legal base class"
		// Use VC_Base_Workaround's LeftBase instead of the
		// alleged illegal base class.
		template <class _T, class _U>
		struct vc_base_workaround : public _T, public _U
		{
			typedef _T left_base;
		};

////////////////////////////////////////////////////////////////////////////////
// class template AlwaysFalse
// Invocation: AlwaysFalse<T>::value
// value will always by 0 (false)
////////////////////////////////////////////////////////////////////////////////

		template< typename _T >
		struct always_false
		{
			enum { value = false };
		};

	}	// end of namespace prvt

////////////////////////////////////////////////////////////////////////////////
// class template ApplyInnerType
// Invocation: ApplyInnerType<Wrapper, U>::type
// Applies the type U to the the inner template 'In' of type Wrapper and typedefs
// the resulting type to 'type'
// The class template ApplyInnerType helps to emulate template template parameters
// i first saw this technique in boost's mpl library.
////////////////////////////////////////////////////////////////////////////////

#if defined(__GNUC__) || (__DCC__)

	template <class _Wrapper, class _T>
	struct apply_inner_type
	{
		template<bool, class _D> struct wrapper_vc : _Wrapper {};

		template<class _D> struct wrapper_vc<true, _D>
			{ template<class _X> struct in; };

		typedef typename
		wrapper_vc< ::nucleon::prvt::always_false<_Wrapper>::value, _T>::template in<_T>::type type;
	};

#else

	template <class _Wrapper, class _T>
	struct apply_inner_type
	{
		template<bool> struct wrapper_vc : _Wrapper {};

		template<> struct wrapper_vc<true>
			{ template<class _X> struct in; };

		typedef typename
		wrapper_vc< ::nucleon::prvt::always_false<_Wrapper>::value>::template in<_T>::type type;
	};

#endif

}

#endif // _NUCLEON_HELPERS_H_
