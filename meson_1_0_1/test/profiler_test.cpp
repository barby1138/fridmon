// profiler_test.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "profiler_test.h"
#include <meson/profiler.h>
#include <quark/thread.h>

using namespace meson;

//////////////////////////////////////////////////////////////////////
// profiler_test
//////////////////////////////////////////////////////////////////////

const char* profiler_test::getName() const
{
	return "profiler_test";
}

void profiler_test::execute()
{
	bench_scope_low scope("one second");
	quark::thread::sleep(1000);
}
