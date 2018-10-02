// profiler_test.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MESON_PROFILER_TEST_H_
#define _MESON_PROFILER_TEST_H_

#include <meson/test.h>

//////////////////////////////////////////////////
// class profiler_test
//
class profiler_test : public meson::test
{
public:
	virtual const char* getName() const;
	virtual void execute();
};

#endif // _MESON_PROFILER_TEST_H_
