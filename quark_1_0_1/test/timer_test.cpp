// timer_test.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "timer_test.h"
#include <quark/timer.h>
#include <quark/singleton.h>

using namespace quark;

typedef singleton_holder<timer_pool> timer_pool_sngl;

// TODO: add more tests

//------------------------------------------------------------------------------

class Observer
{
public:
	Observer()
	{
		_timer10 = timer_pool_sngl::instance().registerTimer(this, &Observer::onTimer10, 10);
		_timer100 = timer_pool_sngl::instance().registerTimer(this, &Observer::onTimer100, 100);
		_timer1000 = timer_pool_sngl::instance().registerTimer(this, &Observer::onTimer1000, 1000);
	}

	~Observer()
	{
		timer_pool_sngl::instance().unregisterTimer(_timer10);
		timer_pool_sngl::instance().unregisterTimer(_timer100);
		timer_pool_sngl::instance().unregisterTimer(_timer1000);
	}

	void wait()
	{
		_event10.aquire();
		_event100.aquire();
		_event1000.aquire();
	}

private:
	void onTimer10(size_t timerId)
	{
		_event10.release();
	}

	void onTimer100(size_t timerId)
	{
		_event100.release();
	}

	void onTimer1000(size_t timerId)
	{
		_event1000.release();
	}

private:
	semaphore _event10;
	semaphore _event100;
	semaphore _event1000;
	size_t _timer10;
	size_t _timer100;
	size_t _timer1000;
};

//------------------------------------------------------------------------------

const char* timer_test::getName() const
{
	return "timer_test";
}

void timer_test::execute()
{
	Observer observer;
	observer.wait();
}

//------------------------------------------------------------------------------
