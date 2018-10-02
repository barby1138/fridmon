// dynamic_fsm_test.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _NUCLEON_DYNAMIC_FSM_TEST_H_
#define _NUCLEON_DYNAMIC_FSM_TEST_H_

#include <meson/test.h>

//////////////////////////////////////////////////
// class dynamic_fsm_test
//
class dynamic_fsm_test : public meson::test
{
public:
	virtual const char* getName() const;
	virtual void execute();
};

#endif // _NUCLEON_DYNAMIC_FSM_TEST_H_
