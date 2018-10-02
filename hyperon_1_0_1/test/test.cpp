// test.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "test.h"
#include <quark/strings.h>
#include <quark/mempool.h>

using namespace quark;

//////////////////////////////////////////////////////////////////////
// test_exception
//////////////////////////////////////////////////////////////////////

test_exception::test_exception(const char* expr, const char* file, size_t line)
	:	pruntime_error(strings::format("%s; file: %s; line: %d", expr, file, line))
{
}

//////////////////////////////////////////////////////////////////////
// test_suite
//////////////////////////////////////////////////////////////////////

test_suite::~test_suite()
{
	std::for_each(_tests.begin(), _tests.end(), cleaner_ptr());
}

void test_suite::registerTest(test* tst)
{
	_tests.push_back(tst);
}

void test_suite::execute()
{
	for (tests_t::iterator itor = _tests.begin(); itor != _tests.end(); ++itor)
	{
		test* tst = (*itor);
		try
		{
			::printf("%s started\n", tst->getName());
			tst->execute();
			::printf("%s passed\n", tst->getName());
		}
		catch (std::exception& ex)
		{
			::printf("test_suite::execute(): exception: %s\n", ex.what());
			::printf("%s failed\n", tst->getName());
		}
	}

	printf("%s\n", default_memory_pool::instance().dump().c_str());
}
