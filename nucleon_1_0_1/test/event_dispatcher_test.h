// event_dispatcher_test.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _NUCLEON_EVENT_DISPATCHER_TEST_H_
#define _NUCLEON_EVENT_DISPATCHER_TEST_H_

#include <meson/test.h>

//////////////////////////////////////////////////
// class event_dispatcher_test
//
class event_dispatcher_test : public meson::test
{
public:
	virtual const char* getName() const;
	virtual void execute();
};

#endif // _NUCLEON_EVENT_DISPATCHER_TEST_H_
