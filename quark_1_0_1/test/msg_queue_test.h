// msg_queue_test.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _QUARK_MSG_QUEUE_TEST_H_
#define _QUARK_MSG_QUEUE_TEST_H_

#include <meson/test.h>

//////////////////////////////////////////////////
// class msg_queue_test
//
class msg_queue_test : public meson::test
{
public:
	virtual const char* getName() const;
	virtual void execute();
};

#endif // _QUARK_MSG_QUEUE_TEST_H_
