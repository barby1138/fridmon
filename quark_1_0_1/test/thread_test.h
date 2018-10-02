// thread_test.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _QUARK_THREAD_TEST_H_
#define _QUARK_THREAD_TEST_H_

#include <meson/test.h>

//////////////////////////////////////////////////
// class thread_test
//
class thread_test : public meson::test
{
public:
	virtual const char* getName() const;
	virtual void execute();

private:
	void testThread();
	void testBatch();
	void testSleep();
};

#endif // _QUARK_THREAD_TEST_H_
