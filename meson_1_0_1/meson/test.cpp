// test.cpp
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

#include "test.h"
#include "profiler.h"
#include <quark/strings.h>
#include <quark/mempool.h>

using namespace quark;
using namespace meson;

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

test_suite::test_suite()
{
	_tests.reserve(12);
}

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
	size_t fails = 0;
	
	{
		bench_scope_low scope("main");
		
		for (tests_t::iterator itor = _tests.begin(); itor != _tests.end(); ++itor)
		{
			test* tst = (*itor);
			try
			{
				const char* testName = tst->getName();
				printf("--------------------------------------------------------------------------\n");
				printf("%s started\n", testName);
				{
					bench_scope_low scope(testName);
					tst->execute();
				}
				printf("%s passed\n", testName);
			}
			catch (std::exception& ex)
			{
				printf("test_suite::execute(): exception: %s\n", ex.what());
				printf("%s failed\n", tst->getName());
				++fails;
			}
		}
	}
	
	printf("--------------------------------------------------------------------------\n");
	printf("fails: %d\n", fails);
	printf("--------------------------------------------------------------------------\n");
	printf("%s\n", default_memory_pool::instance().dump().c_str());
	printf("%s\n", profiler_low::instance().dump().c_str());
}
