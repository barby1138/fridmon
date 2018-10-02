// test.h
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Meson Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Copyright (c) 2006, InterObject Ltd
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _MESON_TEST_H_
#define _MESON_TEST_H_

#include <quark/config.h>

namespace meson
{

//////////////////////////////////////////////////
// class test_exception
//
class test_exception : public quark::pruntime_error
{
public:
	test_exception(const char* expr, const char* file, size_t line);
};

//////////////////////////////////////////////////
// macro _TEST_ASSERT
//
#define _TEST_ASSERT(expr) \
	if (!(expr)) \
		throw meson::test_exception(#expr, __FILE__, __LINE__);

#define _TEST_ASSERT_EXCEPTION(expr, expn) \
	{ \
		bool res = false; \
		try { (expr); } \
		catch (expn&) { res = true; } \
		_TEST_ASSERT(res); \
	}

//////////////////////////////////////////////////
// class test
//
class test
{
public:
	virtual ~test() {}

	virtual const char* getName() const = 0;
	virtual void execute() = 0; // throw(test_exception)

protected:
	test() {}
};

//////////////////////////////////////////////////
// class test_suite
//
class test_suite
{
private:
	typedef quark::pvector<test*> tests_t;

public:
	test_suite();
	~test_suite();

	void registerTest(test* tst);
	void execute();

private:
	tests_t _tests;
};

} // namespace meson

#endif // _MESON_TEST_H_
