// main.cpp
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

#include "profiler_test.h"
#include "tracer_test.h"

#include <meson/tracer/path.h>

using namespace meson;

int main(int argc, char* argv[])
{
	test_suite suite;
	suite.registerTest(new profiler_test);
	suite.registerTest(new tracer_test);

	suite.execute();

	return 0;
}
