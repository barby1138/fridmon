// thread_test.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "thread_test.h"
#include <quark/thread.h>
#include <quark/sync.h>

using namespace quark;

namespace 
{

////////////////////////////////////
// class simple_thread
//
class simple_thread : public thread
{
public:
	simple_thread()
		:	_initialized(false),
			_run(false),
			_uninitialized(false)
	{
	}

	void check()
	{
		_TEST_ASSERT(_initialized);
		_TEST_ASSERT(_run);
		_TEST_ASSERT(_uninitialized);
	}

protected:
	virtual bool onInitialize()
		{ _initialized = true; return true; }

	virtual void run()
		{ _run = true; }

	virtual void onUninitialize()
		{ _uninitialized = true; }

private:
	bool _initialized;
	bool _run;
	bool _uninitialized;
};

////////////////////////////////////
// class fail_thread
//
class fail_thread : public thread
{
public:
	fail_thread()
		:	_initialized(false),
			_run(false),
			_uninitialized(false)
	{
	}

	void check()
	{
		_TEST_ASSERT(_initialized);
		_TEST_ASSERT(!_run);
		_TEST_ASSERT(!_uninitialized);
	}

protected:
	virtual bool onInitialize()
		{ _initialized = true; return false; }

	virtual void run()
		{ _run = true; }

	virtual void onUninitialize()
		{ _uninitialized = true; }

private:
	bool _initialized;
	bool _run;
	bool _uninitialized;
};

////////////////////////////////////
// class batch_thread
//
class batch_thread : public thread
{
public:
	virtual void terminate()
		{ _stopEvent.release(); }

protected:
	virtual void run()
		{ _stopEvent.aquire(); } // wait for termination

private:
	semaphore _stopEvent;
};

} // namespace

//////////////////////////////////////////////////////////////////////
// thread_test
//////////////////////////////////////////////////////////////////////

const char* thread_test::getName() const
{
	return "thread_test";
}

void thread_test::execute()
{
	testThread();
	testBatch();
	testSleep();
}

void thread_test::testThread()
{
	// just start/stop a thread
	simple_thread thread;
	_TEST_ASSERT(thread.start());
	_TEST_ASSERT(thread.stop());
	thread.check();

	// one more time with more checks
	_TEST_ASSERT(thread.start()); 
	_TEST_ASSERT(thread.start());	// try to start twice
	_TEST_ASSERT(thread.stop());
	_TEST_ASSERT(thread.stop());		// try to stop twice

	// once more
	_TEST_ASSERT(thread.start());
	_TEST_ASSERT(thread.wait());

	// a fail case
	fail_thread thread2;
	_TEST_ASSERT(!thread2.start());
	_TEST_ASSERT(thread2.stop());

	// test states
	batch_thread thread3;
	_TEST_ASSERT(thread3.start());
	_TEST_ASSERT(thread3.isStarted());
	_TEST_ASSERT(thread3.stop());
	_TEST_ASSERT(!thread3.isStarted());
}

void thread_test::testBatch()
{
	pvector<batch_thread*> threads;
	thread_bundle bundle;

	// create threads and attach to a bundle
	for (size_t i = 0; i < 10; ++i)
	{
		batch_thread* thread = new batch_thread;
		threads.push_back(thread);
		bundle.attach(*thread);
	}

	// start all threads
	_TEST_ASSERT(bundle.start());

	// stop all threads
	_TEST_ASSERT(bundle.stop());

	// destroy
	std::for_each(threads.begin(), threads.end(), cleaner_ptr());
}

// TODO:
void thread_test::testSleep()
{
}
