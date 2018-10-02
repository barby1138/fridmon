// main.cpp
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Quark Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Copyright (c) 2006, InterObject Ltd
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#include "thread_test.h"
#include "msg_queue_test.h"
#include "timer_test.h"

int main(int argc, char* argv[])
{
	meson::test_suite suite;
	suite.registerTest(new thread_test);
	suite.registerTest(new msg_queue_test);
	suite.registerTest(new timer_test);

	suite.execute();

	return 0;
}
