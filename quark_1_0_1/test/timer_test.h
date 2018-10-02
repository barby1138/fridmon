// timer_test.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef QUARK_TIMER_TEST_H_
#define QUARK_TIMER_TEST_H_

#include <meson/test.h>

//------------------------------------------------------------------------------

class timer_test : public meson::test
{
public:
	virtual const char* getName() const;
	virtual void execute();
};

//------------------------------------------------------------------------------

#endif // QUARK_TIMER_TEST_H_
