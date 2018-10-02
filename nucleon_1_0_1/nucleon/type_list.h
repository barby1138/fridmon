// typelist.h
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
// 
// Last update: Sept 29, 2002
//
// Rani Sharoni's VC 7 port is heavily based on explicit template specialization 
// inside class templates.
// This approach has two problems:
// First: It does not seem to work correctly with VC 6
// Second: The C++ Standard allows explicit specialization only in namespace-scope.
// 
// In general my solutions need more template-classes. But I hope that they
// are all conforming to the C++ Standard.

#include "type_manip.h"

#ifndef _NUCLEON_TYPELIST_INC_
#define _NUCLEON_TYPELIST_INC_

#define ASSERT_TYPELIST(_List)	\
	typedef quark::static_assert_test<sizeof(nucleon::tl::prvt::List_is_not_legal_typelist	\
	< (bool) ( nucleon::tl::prvt::is_typelist<_List>::value ) >)> static_assert_typedef_

////////////////////////////////////////////////////////////////////////////////
// macros TYPELIST_1, TYPELIST_2, ... TYPELIST_50
// Each takes a number of arguments equal to its numeric suffix
// The arguments are type names. TYPELIST_NN generates a typelist containing 
//     all types passed as arguments, in that order.
// Example: TYPELIST_2(char, int) generates a type containing char and int.
////////////////////////////////////////////////////////////////////////////////

#define TYPELIST_1(T1) ::nucleon::typelist<T1, ::nucleon::null_type>

#define TYPELIST_2(T1, T2) ::nucleon::typelist<T1, TYPELIST_1(T2) >

#define TYPELIST_3(T1, T2, T3) ::nucleon::typelist<T1, TYPELIST_2(T2, T3) >

#define TYPELIST_4(T1, T2, T3, T4) \
    ::nucleon::typelist<T1, TYPELIST_3(T2, T3, T4) >

#define TYPELIST_5(T1, T2, T3, T4, T5) \
    ::nucleon::typelist<T1, TYPELIST_4(T2, T3, T4, T5) >

#define TYPELIST_6(T1, T2, T3, T4, T5, T6) \
    ::nucleon::typelist<T1, TYPELIST_5(T2, T3, T4, T5, T6) >

#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) \
    ::nucleon::typelist<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7) >

#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) \
    ::nucleon::typelist<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8) >

#define TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) \
    ::nucleon::typelist<T1, TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9) >

#define TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
    ::nucleon::typelist<T1, TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >

#define TYPELIST_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) \
    ::nucleon::typelist<T1, TYPELIST_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) >

#define TYPELIST_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) \
    ::nucleon::typelist<T1, TYPELIST_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12) >

#define TYPELIST_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) \
    ::nucleon::typelist<T1, TYPELIST_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13) >

#define TYPELIST_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14) \
    ::nucleon::typelist<T1, TYPELIST_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14) >

#define TYPELIST_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15) \
    ::nucleon::typelist<T1, TYPELIST_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15) >

#define TYPELIST_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16) \
    ::nucleon::typelist<T1, TYPELIST_15(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16) >

#define TYPELIST_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17) \
    ::nucleon::typelist<T1, TYPELIST_16(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17) >

#define TYPELIST_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18) \
    ::nucleon::typelist<T1, TYPELIST_17(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18) >

#define TYPELIST_19(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19) \
    ::nucleon::typelist<T1, TYPELIST_18(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19) >

#define TYPELIST_20(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) \
    ::nucleon::typelist<T1, TYPELIST_19(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) >

#define TYPELIST_21(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21) \
    ::nucleon::typelist<T1, TYPELIST_20(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21) >

#define TYPELIST_22(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22) \
    ::nucleon::typelist<T1, TYPELIST_21(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22) >

#define TYPELIST_23(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23) \
    ::nucleon::typelist<T1, TYPELIST_22(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23) >

#define TYPELIST_24(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24) \
    ::nucleon::typelist<T1, TYPELIST_23(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24) >

#define TYPELIST_25(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25) \
    ::nucleon::typelist<T1, TYPELIST_24(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25) >

#define TYPELIST_26(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26) \
    ::nucleon::typelist<T1, TYPELIST_25(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26) >

#define TYPELIST_27(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27) \
    ::nucleon::typelist<T1, TYPELIST_26(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27) >

#define TYPELIST_28(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28) \
    ::nucleon::typelist<T1, TYPELIST_27(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28) >

#define TYPELIST_29(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29) \
    ::nucleon::typelist<T1, TYPELIST_28(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29) >

#define TYPELIST_30(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30) \
    ::nucleon::typelist<T1, TYPELIST_29(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30) >

#define TYPELIST_31(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31) \
    ::nucleon::typelist<T1, TYPELIST_30(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31) >

#define TYPELIST_32(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32) \
    ::nucleon::typelist<T1, TYPELIST_31(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32) >

#define TYPELIST_33(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33) \
    ::nucleon::typelist<T1, TYPELIST_32(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33) >

#define TYPELIST_34(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34) \
    ::nucleon::typelist<T1, TYPELIST_33(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34) >

#define TYPELIST_35(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35) \
    ::nucleon::typelist<T1, TYPELIST_34(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35) >

#define TYPELIST_36(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36) \
    ::nucleon::typelist<T1, TYPELIST_35(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36) >

#define TYPELIST_37(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37) \
    ::nucleon::typelist<T1, TYPELIST_36(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37) >

#define TYPELIST_38(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38) \
    ::nucleon::typelist<T1, TYPELIST_37(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38) >

#define TYPELIST_39(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39) \
    ::nucleon::typelist<T1, TYPELIST_38(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39) >

#define TYPELIST_40(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40) \
    ::nucleon::typelist<T1, TYPELIST_39(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40) >

#define TYPELIST_41(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41) \
    ::nucleon::typelist<T1, TYPELIST_40(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41) >

#define TYPELIST_42(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42) \
    ::nucleon::typelist<T1, TYPELIST_41(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42) >

#define TYPELIST_43(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43) \
    ::nucleon::typelist<T1, TYPELIST_42(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43) >

#define TYPELIST_44(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44) \
    ::nucleon::typelist<T1, TYPELIST_43(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44) >

#define TYPELIST_45(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45) \
    ::nucleon::typelist<T1, TYPELIST_44(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45) >

#define TYPELIST_46(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46) \
    ::nucleon::typelist<T1, TYPELIST_45(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46) >

#define TYPELIST_47(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47) \
    ::nucleon::typelist<T1, TYPELIST_46(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47) >

#define TYPELIST_48(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47, T48) \
    ::nucleon::typelist<T1, TYPELIST_47(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47, T48) >

#define TYPELIST_49(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47, T48, T49) \
    ::nucleon::typelist<T1, TYPELIST_48(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47, T48, T49) >

#define TYPELIST_50(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50) \
    ::nucleon::typelist<T1, TYPELIST_49(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50) >

namespace nucleon
{

////////////////////////////////////////////////////////////////////////////////
// class template typelist
// The building block of typelists of any length
// Use it through the TYPELIST_NN macros
// Defines nested types:
//     head (first element, a non-typelist type by convention)
//     tail (second element, can be another typelist)
////////////////////////////////////////////////////////////////////////////////

template <class _T, class _U>
struct typelist
{
	typedef _T head;
	typedef _U tail;
};

////////////////////////////////////////////////////////////////////////////////
// class null_type
// Used as a placeholder for "no type here"
// Useful as an end marker in typelists 
////////////////////////////////////////////////////////////////////////////////

class null_type {};

namespace tl
{

namespace prvt
{
	template <bool x> struct List_is_not_legal_typelist;

	template <> struct List_is_not_legal_typelist<true>{};
	
	struct typelist_tag {};
	struct null_type_tag {};
	struct nonelist_tag {};

	enum 
	{
		NoneList_ID = 0,
		Typelist_ID = 1,
		AtomList_ID	= 2,
		null_type_ID = 4
	};
			
////////////////////////////////////////////////////////////////////////////////
// class template is_typelist
// detects if type is typelist (including null_type)
// Invocation :
// is_typelist<T>::value
////////////////////////////////////////////////////////////////////////////////

	template<typename _T>
	struct is_typelist
	{
	private:
		typedef type_tag<1>::X list;
		typedef type_tag<2>::X atom_list;
		typedef type_tag<3>::X null_list;
		typedef type_tag<4>::X no_list;
		
		// VC 6.0 does not allow overloads
		// for check(quark::type2type< typelist<head, tail> >)
		// and check(quark::type2type<null_type>);
		// so we must use to different functions
		template<class _Head, class _Tail>
		static type_tag<1>::X	check(quark::type2type< typelist<_Head, _Tail> >);
		static type_tag<4>::X	check(...);

		template <class _U>
		static type_tag<2>::X	check2(quark::type2type< typelist<_U, null_type> >);
		static type_tag<4>::X	check2(...);
		
		static type_tag<3>::X	check3(quark::type2type<null_type>);
		static type_tag<4>::X	check3(...);
	
	public:
		enum 
		{	
			temp1	= sizeof(check(quark::type2type<_T>())) == sizeof(type_tag<1>::X) ? Typelist_ID : NoneList_ID,
			temp2	= sizeof(check2(quark::type2type<_T>())) == sizeof(type_tag<2>::X) ? AtomList_ID : NoneList_ID,
			temp4	= temp2 ? Typelist_ID : NoneList_ID,
			temp3	= sizeof(check3(quark::type2type<_T>())) == sizeof(type_tag<3>::X) ? null_type_ID : NoneList_ID,
			value	= temp1 || temp2 || temp3,
			type_id	= (temp1 ^ temp4) | temp2 | temp3 
		};
		typedef typename select_type
		<
			type_id == Typelist_ID || type_id == AtomList_ID, 
			typelist_tag,
			typename select_type<type_id == null_type_ID, null_type_tag, nonelist_tag>::result
		> 
		::result type_tag;
	};

} // end of namespace prvt
	
////////////////////////////////////////////////////////////////////////////////
// class template MakeTypelist
// Takes a number of arguments equal to its numeric suffix
// The arguments are type names.
// Maketypelist<T1, T2, ...>::result
// returns a typelist that is of T1, T2, ...
////////////////////////////////////////////////////////////////////////////////

// MakeTypeList-Template from Rani Sharoni's VC 7 port.
template 
<	typename T1  = null_type, typename T2  = null_type, typename T3  = null_type,
	typename T4  = null_type, typename T5  = null_type, typename T6  = null_type,
	typename T7  = null_type, typename T8  = null_type, typename T9  = null_type,
	typename T10 = null_type, typename T11 = null_type, typename T12 = null_type,
	typename T13 = null_type, typename T14 = null_type, typename T15 = null_type,
	typename T16 = null_type, typename T17 = null_type, typename T18 = null_type,
    typename T19 = null_type, typename T20 = null_type, typename T21 = null_type, 
    typename T22 = null_type, typename T23 = null_type, typename T24 = null_type, 
    typename T25 = null_type, typename T26 = null_type, typename T27 = null_type, 
    typename T28 = null_type/*, typename T29 = null_type, typename T30 = null_type, 
    typename T31 = null_type, typename T32 = null_type, typename T33 = null_type,
	typename T34 = null_type, typename T35 = null_type, typename T36 = null_type,
	typename T37 = null_type, typename T38 = null_type, typename T39 = null_type,
	typename T40 = null_type*/
> 
struct make_type_list
{
private:
	typedef typename make_type_list
	<
	T2 , T3 , T4 , 
	T5 , T6 , T7 , 
	T8 , T9 , T10, 
	T11, T12, T13,
	T14, T15, T16, 
	T17, T18, T19,
    T20, T21, T22, 
    T23, T24, T25, 
    T26, T27, T28/*, 
    T29, T30, T31, 
    T32, T33, T34, 
    T35, T36, T37, 
    T38, T39, T40*/
	>
	::result tail_result;

public:
	typedef typelist<T1, tail_result> result;
};

template<>
struct make_type_list
<
	null_type, null_type, null_type, 
	null_type, null_type, null_type, 
	null_type, null_type, null_type, 
	null_type, null_type, null_type,
	null_type, null_type, null_type,
	null_type, null_type, null_type,
    null_type, null_type, null_type, 
    null_type, null_type, null_type, 
    null_type, null_type, null_type, 
    null_type/*, null_type, null_type,
    null_type, null_type, null_type,
    null_type, null_type, null_type,
    null_type, null_type, null_type,
    null_type*/
>
{
	typedef null_type result;
};
	
////////////////////////////////////////////////////////////////////////////////
// class template Length
// Computes the length of a typelist
// Invocation (_List is a typelist):
// Length<_List>::value
// returns a compile-time constant containing the length of _List, not counting
// the end terminator (which by convention is null_type)
////////////////////////////////////////////////////////////////////////////////

template <class _List>
struct length
{
private:
	ASSERT_TYPELIST(_List);
	typedef typename _List::head head;
	typedef typename _List::tail tail;

public:
	enum { value = 1 + length<tail>::value };
};

// explicit specialization for an empty list.
// this is the border case for the recursive length-calculation
template <>
struct length<null_type>
{
	enum { value = 0 };
};
	
////////////////////////////////////////////////////////////////////////////////
// class template TypeAt
// Finds the type at a given index in a typelist
// Invocation (_List is a typelist and index is a compile-time integral 
//     constant):
// TypeAt<_List, index>::result
// returns the type in position 'index' in _List
// If you pass an out-of-bounds index, the result is a compile-time error
////////////////////////////////////////////////////////////////////////////////

namespace prvt
{
	// The type at Index i is the type at i-1 of the List's tail
	template <unsigned int _Index>
	struct type_at_impl
	{
		template <class _List>
		struct in
		{
			ASSERT_TYPELIST(_List);
			typedef typename _List::head head;
			typedef typename _List::tail tail;
			typedef typename type_at_impl<_Index-1>::template in<tail>::result result;
		};
	};
	// the border case is represented by an explicit specialization
	// The type at Index 0 is the type of the head.
	template <>
	struct type_at_impl<0>
	{
		template <class _List>
		struct in
		{
			ASSERT_TYPELIST(_List);
			typedef typename _List::head head;
			typedef head result;
		};
	};
}	// end of namespace prvt
	
template <class _List, unsigned int _Index>
struct type_at
{
	typedef typename prvt::type_at_impl<_Index>::template in<_List>::result result ;
};

////////////////////////////////////////////////////////////////////////////////
// class template TypeAtNonStrict
// Finds the type at a given index in a typelist
// Invocations (_List is a typelist and index is a compile-time integral 
//     constant):
// a) TypeAt<_List, index>::result
// returns the type in position 'index' in _List, or null_type if index is 
//     out-of-bounds
// b) TypeAt<_List, index, D>::result
// returns the type in position 'index' in _List, or D if index is out-of-bounds
////////////////////////////////////////////////////////////////////////////////

template <class _List, unsigned int i, class _DefType = null_type>
struct type_at_non_strict;

namespace prvt
{	
	// if _List is not null_type, check if Index is 0.
	// if Index is 0, the result is _List::head
	// if Index is > 0, the result is the result of appliying TypeAtNonStrict
	// to the list's and Index-1
	template <class _List>
	struct type_at_non_strict_impl
	{
		template <class _DefType, unsigned int _Index>
		struct in
		{
			ASSERT_TYPELIST(_List);
			typedef typename select_type
			<
				_Index == 0,				// The condition
				typename _List::head,	// true-case
				typename type_at_non_strict<typename _List::tail, _Index-1, _DefType>::result
			>::result result;
		};
	};

	// if _List is null_type the result is *always* the specified DefaultType.
	template <>
	struct type_at_non_strict_impl<null_type>
	{
		template <class _DefType, unsigned int _Index>
		struct in
		{
			typedef _DefType result;
		};
	};
		
}	// end of namespace prvt

template <class _List, unsigned int i, class _DefType>
struct type_at_non_strict
{
	typedef typename 
	prvt::type_at_non_strict_impl<_List>::template in<_DefType, i>::result result;
};	
	
////////////////////////////////////////////////////////////////////////////////
// class template IndexOf
// Finds the index of a type in a typelist
// Invocation (_List is a typelist and T is a type):
// IndexOf<_List, T>::value
// returns the position of T in _List, or -1 if T is not found in _List
////////////////////////////////////////////////////////////////////////////////

template <class _List, class _T> 
struct index_of;

namespace prvt
{
	// If _List is a typelist and _List::head is T, then the Index is 0
	// If _List::head is not T, compute the result of IndexOf applied to
	// _List's tail and T into a temporary value temp.
	// If temp is -1, then value is -1
	// Else value is 1 + temp
	template <class _List>
	struct index_of_impl
	{
		template <class _T>
		struct in
		{
			ASSERT_TYPELIST(_List);
			typedef typename _List::head head;
			typedef typename _List::tail tail;
			
		private:
			enum { temp = is_equal_type<_T, head>::value != 0 ? 0 
							: index_of<tail, _T>::temp };
		public:
			enum { value = temp == -1 ? -1 : 1 + temp };
		};
	};

	// T cannot be in an empty list.
	// Therefore return -1 to indicate Not-In-List
	template <>
	struct index_of_impl<null_type>
	{
		template <class _T>
		struct in
		{
			enum { value = -1 };
		};
	};

}	// end of namespace Private
	
// The primary IndexOfImpl-Template is always one step ahead.
// Therefore if T is in list, we need to subtract one from the result.
template <class _List, class _T> 
struct index_of
{
	enum { temp = prvt::index_of_impl<_List>::template in<_T>::value };
	enum { value = temp == -1 ? -1 : temp - 1 };
};

////////////////////////////////////////////////////////////////////////////////
// class template Append
// Appends a type or a typelist to another
// Invocation (_List is a typelist and T is either a type or a typelist):
// Append<_List, T>::result
// returns a typelist that is _List followed by T and null_type-terminated
////////////////////////////////////////////////////////////////////////////////

template <class _List, class _T>
struct append;

namespace prvt
{
	template <class _List>
	struct append_impl
	{	// if _List is not null_type the result
		// is a typelist having _List::head as its head and
		// and the result of appending T to _List::tail as its tail.
		ASSERT_TYPELIST(_List);
		template <class _T>
		struct in
		{
			typedef typelist<typename _List::head, 
				typename append<typename _List::tail, _T>::result> result;
		};
			
	};

	template <>
	struct append_impl<null_type>
	{	
		// if _List is null_type, check if T is null_type, a single type
		// or a typelist
		// If _List is null_type and T is null_type
		// the result is null_type, too
		//
		// If _List is null_type and T is not null_type.
		// Check if T is a Typelist
		//
		// if _List is null_type and T is a typelist the result is T
		// if _List is null_type and T is not a typelist
		// the result is a typelist containing only T
		template <class _T>
		struct in
		{
			typedef typename select_type
			<
				is_equal_type<_T, null_type>::value,	// is T == null_type?	
				null_type,							// yes
				typename select_type						// no. check if T is a Typelist
				<
					is_typelist<_T>::value,			// is T a typelist?
					_T,								// yes
					typelist<_T, null_type>			// no
				>::result
			>::result result;
		};
	};
}	// end of namespace Private

template <class _List, class _T>
struct append
{
	typedef typename prvt::append_impl<_List>::template in<_T>::result result;
};

////////////////////////////////////////////////////////////////////////////////
// class template Erase
// Erases the first occurence, if any, of a type in a typelist
// Invocation (_List is a typelist and T is a type):
// Erase<_List, T>::result
// returns a typelist that is _List without the first occurence of T
////////////////////////////////////////////////////////////////////////////////

template <class _List, class _T>
struct erase;

namespace prvt
{
	template <class _List>
	struct erase_impl
	{	// _List is not null_type. 
		// Check if _List::head is equal to T
		// if T is the same as _List::head, then the result is _List::tail
		//
		// if _List is not null_type and _List::head is not equal to T,
		// then the result is a Typelist having _List::head as its head
		// and the result of applying Erase to the tail of list as its
		// tail.
		template <class _T>
		struct in
		{
			ASSERT_TYPELIST(_List);
			typedef typename _List::head head;
			typedef typename _List::tail tail;
			typedef typename select_type
			<
				is_equal_type<head, _T>::value,	// is T equal to head?
				tail,							// Yes. result is tail
				typelist<typename _List::head,	// No. recurse
				typename erase<typename _List::tail, _T>::result>
			>::result result;
		};
	};

	// if _List is null_type the result is null_type.
	template <>
	struct erase_impl<null_type>
	{
		template <class T>
		struct in
		{
			typedef null_type result;
		};
	};

}	// end of namespace prvt
	
template <class _List, class _T>
struct erase
{
	typedef typename prvt::erase_impl<_List>::template in<_T>::result result;
};

////////////////////////////////////////////////////////////////////////////////
// class template EraseAll
// Erases all occurences, if any, of a type in a typelist
// Invocation (_List is a typelist and T is a type):
// EraseAll<_List, T>::result
// returns a typelist that is _List without any occurence of T
////////////////////////////////////////////////////////////////////////////////

template <class _List, class _T>
struct erase_all;

namespace prvt
{
	template <class _List>
	struct erase_all_impl
	{
		// _List is not null_type. 
		// Check if _List::head is equal to T
		// If T is equal to TLIst::head the result is the result of EraseAll
		// applied to _List::tail
		//
		// If T is not equal to _List::head the result is a Typelist with
		// _List::head as its head and the result of applying EraseAll to _List::tail
		// as its tail.
		template <class _T>
		struct in
		{
			ASSERT_TYPELIST(_List);
			typedef typename _List::head head;
			typedef typename _List::tail tail;
			typedef typename select_type
			<
				is_equal_type<head, _T>::value,				// is T == head?
				typename erase_all<tail, _T>::result,		// Yes
				typelist<head, typename						// No
				erase_all<tail, _T>::result>
			>::result result;
		};
	};

	// if _List is null_type the result is null_type.
	template <>
	struct erase_all_impl<null_type>
	{
		template <class T>
		struct in
		{
			typedef null_type result;
		};
	};

}	// end of namespace prvt
	
template <class _List, class _T>
struct erase_all
{
	typedef typename prvt::erase_all_impl<_List>::template in<_T>::result result;
};

////////////////////////////////////////////////////////////////////////////////
// class template NoDuplicates
// Removes all duplicate types in a typelist
// Invocation (_List is a typelist):
// NoDuplicates<_List, T>::result
////////////////////////////////////////////////////////////////////////////////

// NoDuplicates taken from Rani Sharoni's Loki VC7-Port.
template <class _List> 
struct no_duplicates
{
private:
    typedef typename _List::head head;
    typedef typename _List::tail tail;

    ASSERT_TYPELIST(_List);

    typedef typename no_duplicates<tail>::result L1;
    typedef typename erase<L1, head>::result    L2;

public:
    typedef typelist<head, L2> result;
};

template <> 
struct no_duplicates<null_type>
{
    typedef null_type result;
};

////////////////////////////////////////////////////////////////////////////////
// class template Replace
// Replaces the first occurence of a type in a typelist, with another type
// Invocation (_List is a typelist, T, U are types):
// Replace<_List, T, U>::result
// returns a typelist in which the first occurence of T is replaced with U
////////////////////////////////////////////////////////////////////////////////

template <class _List, class _T, class _U>
struct replace;

namespace prvt
{
	// If _List is not null_type, check if T is equal to _List::head
	template <class _List>
	struct replace_impl
	{
		template <class _T, class _U>
		struct in
		{
			// If _List::head is equal to T, the result is a typelist
			// with U as its head an _List::tail as its tail.
			// If T is not equal to _List::head, the result is a typelist
			// with _List::head as its head and the result of applying
			// Replace to _List::tail, T, U as its tail
			//ASSERT_VALID(_List);
			typedef typename _List::head head;
			typedef typename _List::tail tail;
			typedef typename select_type
			<
				is_equal_type<_T, head>::value,	// Is T == head?
				typelist<_U, tail>,				// yes
				typelist<head, typename replace<tail, _T, _U>::result>
			>::result result;
		};
	};
	
	// If _List is null_type the result is null_type
	template <>
	struct replace_impl<null_type>
	{
		template <class _T, class _U>
		struct in
		{
			typedef null_type result;
		};
	};

}	// end of namespace prvt
	
template <class _List, class _T, class _U>
struct replace
{
	typedef typename 
	prvt::replace_impl<_List>::template in<_T, _U>::result result;
};
	
////////////////////////////////////////////////////////////////////////////////
// class template ReplaceAll
// Replaces all occurences of a type in a typelist, with another type
// Invocation (_List is a typelist, T, U are types):
// Replace<_List, T, U>::result
// returns a typelist in which all occurences of T is replaced with U
////////////////////////////////////////////////////////////////////////////////

template <class _List, class _T, class _U>
struct replace_all;

namespace prvt
{
	// If _List is not null_type, check if T is equal to _List::head
	template <class _List>
	struct replace_all_impl
	{
		template <class _T, class _U>
		struct in
		{
			//ASSERT_VALID(_List);
			typedef typename _List::head head;
			typedef typename _List::tail tail;
			typedef typename select_type
			<
				is_equal_type<_T, head>::value,								// Is T == head?
				typelist<_U, typename replace_all<tail, _T, _U>::result>,	// yes
				typelist<head, typename replace_all<tail, _T, _U>::result>
			>::result result;
		};
	};
	
	// If _List is null_type the result is null_type
	template <>
	struct replace_all_impl<null_type>
	{
		template <class _T, class _U>
		struct in
		{
			typedef null_type result;
		};
	};
}

template <class _List, class _T, class _U>
struct replace_all
{
	typedef typename 
	prvt::replace_all_impl<_List>::template in<_T, _U>::result result;
};

////////////////////////////////////////////////////////////////////////////////
// class template Reverse
// Reverses a typelist
// Invocation (_List is a typelist):
// Reverse<_List>::result
// returns a typelist that is _List reversed
////////////////////////////////////////////////////////////////////////////////

//	Reverse taken from Rani Sharoni's Loki VC7-Port.
template <class _List> struct reverse;
        
template <>
struct reverse<null_type>
{
    typedef null_type result;
};

template <class _List> 
struct reverse
{
private:
    typedef typename _List::head head;
    typedef typename _List::tail tail;

    ASSERT_TYPELIST(_List);

public:
    typedef typename append<
        typename reverse<tail>::result, head>::result result;
};

////////////////////////////////////////////////////////////////////////////////
// class template MostDerived
// Finds the type in a typelist that is the most derived from a given type
// Invocation (_List is a typelist, T is a type):
// MostDerived<_List, T>::result
// returns the type in _List that's the most derived from T
////////////////////////////////////////////////////////////////////////////////

template <class _List, class _T> struct most_derived;

namespace prvt
{
	template <class _List>
	struct most_derived_impl
	{
		template <class _T>
		struct in
		{
		private:
			//ASSERT_VALID(_List);
			typedef typename _List::head head;
			typedef typename _List::tail tail;
			typedef typename most_derived<tail, _T>::result candidate;

		public:
			typedef typename select_type
			<
				SUPERSUBCLASS(candidate, head),
				head,
				candidate
			>::result result;
		};
	};

	template <>
	struct most_derived_impl<null_type>
	{
		template <class _T>
		struct in { typedef _T result; };
	};	

}	// end of namespace prvt

template <class _List, class _T> 
struct most_derived
{
	typedef typename 
	prvt::most_derived_impl<_List>::template in<_T>::result result;
};

////////////////////////////////////////////////////////////////////////////////
// class template FindDerived
// Finds the type in a typelist that is the most derived from a given type
// Invocation (_List is a typelist, T is a type):
// MostDerived<_List, T>::result
// returns the type in _List that's the most derived from T
////////////////////////////////////////////////////////////////////////////////

template <class _List, class _T> struct find_derived;

namespace prvt
{
	template <class _List>
	struct find_derived_impl
	{
		template <class _T>
		struct in
		{
			private:
				//ASSERT_VALID(_List);
				typedef typename _List::head head;
				typedef typename _List::tail tail;
				typedef typename find_derived<tail, _T>::result candidate;

			public:
				typedef typename select_type
				<
				SUPERSUBCLASS(candidate, head),
				head,
				candidate
				>::result result;
		};
	};

	template <>
	struct find_derived_impl<null_type>
	{
		template <class _T>
		struct in { typedef null_type result; };
	};	

} // end of namespace prvt

template <class _List, class _T> 
struct find_derived
{
	typedef typename 
	prvt::find_derived_impl<_List>::template in<_T>::result result;
};

////////////////////////////////////////////////////////////////////////////////
// class template DerivedToFront
// Arranges the types in a typelist so that the most derived types appear first
// Invocation (_List is a typelist):
// DerivedToFront<_List>::result
// returns the reordered _List 
////////////////////////////////////////////////////////////////////////////////

// DerivedToFront taken from Rani Sharoni's Loki VC7-Port.
template <class _List>
struct derived_to_front
{
private:
    ASSERT_TYPELIST(_List);

    typedef typename _List::head head;
    typedef typename _List::tail tail;

    typedef typename most_derived<tail, head>::result the_most_derived;
    typedef typename replace_all<tail, the_most_derived, head>::result temp;
    typedef typename derived_to_front<temp>::result L;

public:
    typedef typelist<the_most_derived, L> result;
};
    
template <>
struct derived_to_front<null_type>
{
	typedef null_type result;
};

////////////////////////////////////////////////////////////////////////////////
// class template DerivedToFrontAll
// Arranges all the types in a typelist so that the most derived types appear first
// Invocation (_List is a typelist):
// DerivedToFront<_List>::result
// returns the reordered _List 
////////////////////////////////////////////////////////////////////////////////

// DerivedToFrontAll taken from Rani Sharoni's Loki VC7-Port.
template <class _List>
struct derived_to_front_all
{
private:
	ASSERT_TYPELIST(_List);
	
	typedef typename _List::head head;
	typedef typename _List::tail tail;
    
	typedef typename most_derived<tail, head>::result the_most_derived;
	typedef typename replace<tail, the_most_derived, head>::result L;
	
	typedef typename derived_to_front_all<L>::result tail_result;
	
public:
	typedef typelist<the_most_derived, tail_result> result;
};

template <>
struct derived_to_front_all<null_type>
{
	typedef null_type result;
};
    
} // end of namespace tl

} // end of namespace nucleon

////////////////////////////////////////////////////////////////////////////////
// Change log:
// June 09, 2001: Fix bug in parameter list of macros TYPELIST_23 to TYPELIST_27
//      (credit due to Dave Taylor)
// June 20, 2001: ported by Nick Thurn to gcc 2.95.3. Kudos, Nick!!!
// May  10, 2002: ported by Rani Sharoni to VC7 (RTM - 9466)
// Sept 29, 2002: ported by Benjamin Kaufmann to MSVC 6.0
////////////////////////////////////////////////////////////////////////////////

#endif // _NUCLEON_TYPELIST_INC_
