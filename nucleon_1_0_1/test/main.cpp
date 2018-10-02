// main.cpp
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Nucleon Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Copyright (c) 2006, InterObject Ltd
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#include "factory_test.h"
#include "bstream_test.h"
#include "dispatcher_test.h"
#include "bimap_test.h"
#include "smart_ptr_test.h"
#include "fsm_test.h"
#include "dynamic_fsm_test.h"
#include "event_dispatcher_test.h"

// put breakpoint to check all global allocations
void* operator new(size_t size)
{
	return malloc(size);
}

int main()
{
	meson::test_suite suite;
	suite.registerTest(new factory_test);
	suite.registerTest(new bstream_test);
	suite.registerTest(new dispatcher_test);
	suite.registerTest(new bimap_test);
	suite.registerTest(new smart_ptr_test);
	suite.registerTest(new fsm_test);
	suite.registerTest(new dynamic_fsm_test);
	suite.registerTest(new event_dispatcher_test);

	suite.execute();

	return 0;
}
