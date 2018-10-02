// test.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HYPERON_TEST_TEST_H_
#define _HYPERON_TEST_TEST_H_

#include <quark/config.h>

//////////////////////////////////////////////////
// class test_exception
//
class test_exception : public quark::pruntime_error
{
public:
	test_exception(const char* expr, const char* file, size_t line);
};

//////////////////////////////////////////////////
// macro _ASSERT
//
#ifdef _ASSERT
#undef _ASSERT
#define _ASSERT(expr) \
	if (!(expr)) \
		throw test_exception(#expr, __FILE__, __LINE__);
#endif
//////////////////////////////////////////////////
// class test
//
struct test
{
	test(const char* name) : _name(name) {}
	virtual ~test() {}

	const char* getName() const
		{ return _name.c_str(); }
		
	virtual void execute() = 0; // throw(test_exception)

private:
	quark::pstring _name;
};

//////////////////////////////////////////////////
// class test_suite
//
class test_suite
{
private:
	typedef quark::pvector<test*> tests_t;

public:
	~test_suite();

	void registerTest(test* tst);
	void execute();

private:
	tests_t _tests;
};

#endif // _HYPERON_TEST_TEST_H_
