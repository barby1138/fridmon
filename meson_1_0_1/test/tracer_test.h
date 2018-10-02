// tracer_test.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MESON_TRACER_TEST_H_
#define _MESON_TRACER_TEST_H_

#include <meson/test.h>

//////////////////////////////////////////////////
// class tracer_test
//
class tracer_test : public meson::test
{
public:
	virtual const char* getName() const;
	virtual void execute();
};

#endif // _MESON_TRACER_TEST_H_
