// thread_impl.h
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _REDHAT_QUARK_THREAD_IMPL_H_
#define _REDHAT_QUARK_THREAD_IMPL_H_

#include <quark/mempool/service/service_mempool.h>
#include <quark/sync.h>
#include <quark/time.h>

// Platform-specific headers
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h>
#include <sched.h>

#include <sched.h>  // sched_setaffinity

#include <sys/syscall.h>
#include <unistd.h>

#include <exception>


namespace quark
{
#define INVALID_THREAD_ID (-1)

//////////////////////////////////////////////////
// class thread::thread_impl
//
class thread::thread_impl : public pimple_object
{
private:
	enum result_t { error = 1, success = 0 };
	
public:
	thread_impl(thread& owner);
	~thread_impl();

	bool start();
	void terminate() { _bTerminate = true; }
	bool wait();
	bool isStarted() const { return _id != INVALID_THREAD_ID; }
	bool isTerminating() const { return _bTerminate; }
	int getId() const { return _id; }
	
	static void sleep(unsigned int ms_seconds);
	static void yield();
	static int getCurrentThreadId();

private:
	static unsigned int STDCALL threadLoop(void* ptr);
	void cleanup();
	static pid_t gettid() { return syscall(__NR_gettid); }

private:
	thread&					_owner;
	semaphore				_startUpEvent;
	semaphore				_finishEvent;
	volatile int			_id;
	volatile bool			_bTerminate;
	result_t				_result;
	pthread_t				_handle;
};

//////////////////////////////////////////////////////////////////////
// thread::thread_impl
//////////////////////////////////////////////////////////////////////

thread::thread_impl::thread_impl(thread& owner)
	:	_owner(owner),
		_startUpEvent(),
		_finishEvent(),
		_id(INVALID_THREAD_ID),
		_bTerminate(false),
		_result(error)
{
}

thread::thread_impl::~thread_impl()
{
	cleanup();
}

bool thread::thread_impl::start()
{
	// check if active object is already started
	if (isStarted()) 
	{
		return true;
	}

	// clear thread variables (in case if this active object was started once)
	cleanup();

	pthread_attr_t attr;
	sched_param sparam;
	sparam.sched_priority = _owner._priority;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setschedparam(&attr, &sparam);

	if(pthread_create(&_handle, &attr, (void*(*)(void*))&threadLoop, this) != 0)
	{
		return false;
	}
	
	pthread_attr_destroy(&attr);

	// wait for startup
	_startUpEvent.aquire();

	if (_result == error)
	{
		wait();

		return false;
	}

	return true;
}

bool thread::thread_impl::wait()
{
	if (isStarted())
	{
		assert(!pthread_equal(pthread_self(), _handle));
		
		_finishEvent.aquire();
	}

	return true;
}

inline void thread::thread_impl::sleep(unsigned int ms_seconds)
{
	while (ms_seconds > 1000)
	{
		usleep(MILLI_TO_MICRO(1000));
		ms_seconds -= 1000;
	}

	usleep(MILLI_TO_MICRO(ms_seconds));
}

inline void thread::thread_impl::yield()
{
	pthread_yield();
}

inline int thread::thread_impl::getCurrentThreadId()
{
	// returns thread id of the calling thread, not it's object
	return gettid();
}

unsigned int STDCALL thread::thread_impl::threadLoop(void* ptr)
{
	thread_impl* self = reinterpret_cast<thread_impl*>(ptr);
	assert(NULL != self);

	// set unexpected handler
	unexpected_handler oldUnexpectedHandler;
	if (self->_owner._unexpected != NULL)
	{
		oldUnexpectedHandler = std::set_unexpected(self->_owner._unexpected);
	}

	// initialize active object members
	if (!self->_owner.onInitialize())
	{
		self->_id = INVALID_THREAD_ID;
		self->_result = error;

		self->_startUpEvent.release();
	}
	else
	{		
		self->_id = getCurrentThreadId();
		self->_result = success;

		self->_startUpEvent.release();

		// run active object
		try
		{
			self->_owner.run();
		}
#ifdef _DEBUG
		catch (std::exception& )
#else
		catch (...)
#endif // _DEBUG
		{
			std::unexpected();
		}

		// uninitialize active object memberss
		self->_owner.onUninitialize();

		// thread is stopped
		self->cleanup();

		if (self->_owner._unexpected != NULL)
		{
			std::set_unexpected(oldUnexpectedHandler);
		}
	}

	self->_finishEvent.release();

	return self->_result;
}

void thread::thread_impl::cleanup()
{
	_id = INVALID_THREAD_ID;
	_bTerminate = false;
}

class scheduler::scheduler_impl : public pimple_object
{
public:
	static void setaffinity(int* handle, size_t mask_size, int* mask);
};

inline void scheduler::scheduler_impl::setaffinity(int* handle, size_t mask_size, int* mask)
{
	if (mask_size > 0)
	{
		pid_t pid = (handle == NULL) ? 0 : static_cast<pid_t>(*handle);
		cpu_set_t cpuset;
		int status;

		CPU_ZERO(&cpuset);
		for (int j = 0; j < mask_size; j++)
			CPU_SET(j, &cpuset);

		status = sched_setaffinity(pid, sizeof(cpuset), &cpuset);
		if (status != 0)
		{
			perror("sched_setaffinity");
		}
	}
}

} // namespace quark

#endif // _REDHAT_QUARK_THREAD_IMPL_H_
