// fsm_test.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "fsm_test.h"
#include <nucleon/fsm.h>

using namespace quark;
using namespace nucleon;

namespace 
{

//------------------------------------------------------------------------------

enum { connect, disconnect };

class event
{
public:
	size_t id() const { return _id; }
	static size_t classId() { return -1; }

protected:
	event(size_t id) : _id(id) {}

private:
	size_t _id;
};

template<size_t _Id>
struct eventT : event
{
	static size_t classId() { return _Id; }

protected:
	eventT() : event(_Id) {}
};

struct connect_msg : eventT<connect> {};
struct disconnect_msg : eventT<disconnect> {};

//------------------------------------------------------------------------------

enum { idle, active };

template<size_t _Id>
struct stateT : fsm_state
{
	static size_t classId() { return _Id; }

protected:
	stateT() : fsm_state(_Id) {}
};

struct idle_state : stateT<idle> 
{
	void process(connect_msg& msg) {}
};

struct active_state : stateT<active>
{
	void process(disconnect_msg& msg) {}
};

//------------------------------------------------------------------------------

class test_scheme : public fsm_scheme<>
{
public:
	test_scheme()
	{
		addEvent(connect);
		addEvent(disconnect);

		// register states
		registerState(type2type<idle_state>());
		registerState(type2type<active_state>());

		// register transitions
		addTransition(idle, connect,
			new transition_actionT<idle_state, connect_msg>(active_state::classId(), &idle_state::process));
		addTransition(active, disconnect,
			new transition_actionT<active_state, disconnect_msg>(idle_state::classId(), &active_state::process));
	}
};

//------------------------------------------------------------------------------

} // anonymous namespace

//////////////////////////////////////////////////////////////////////
// fsm_test
//////////////////////////////////////////////////////////////////////

const char* fsm_test::getName() const
{
	return "fsm_test";
}

void fsm_test::execute()
{
	fsm<test_scheme> fsm(idle);
	//fsm.setFsmName("fsm");

	_TEST_ASSERT(fsm.supported(connect_msg()));
	_TEST_ASSERT(fsm.supported(disconnect_msg()));

	_TEST_ASSERT(fsm.getCurState().id() == idle);

	fsm.process(connect_msg());
	_TEST_ASSERT(fsm.getCurState().id() == active);
	fsm.process(connect_msg()); // unexpected
	_TEST_ASSERT(fsm.getCurState().id() == active);
	fsm.process(disconnect_msg());
	_TEST_ASSERT(fsm.getCurState().id() == idle);

	// benchmark
#ifndef _DEBUG
	for (size_t i = 0; i < 500000; ++i)
	{
		fsm.process(connect_msg());
		fsm.process(disconnect_msg());
	}
#endif // _DEBUG
}
