// dispatcher_test.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _NUCLEON_DISPATCHER_TEST_H_
#define _NUCLEON_DISPATCHER_TEST_H_

#include <meson/test.h>

//////////////////////////////////////////////////
// class dispatcher_test
//
class dispatcher_test : public meson::test
{
public:
	virtual const char* getName() const;
	virtual void execute();
};

#endif // _NUCLEON_DISPATCHER_TEST_H_
