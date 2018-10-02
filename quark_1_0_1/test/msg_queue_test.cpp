// msg_queue_test.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "msg_queue_test.h"
#include <quark/msg_queue.h>
#include <quark/thread.h>

using namespace quark;

enum { msgCount = 1000 };

namespace 
{

////////////////////////////////////
// class message
//
class message : public pooled_object
{
public:
	message(u32 data)
		:	_data(data) {}

	u32 getData() const
		{ return _data; }

private:
	u32 _data;
};

////////////////////////////////////
// class receiver
//
class receiver : public thread
{
private:
	typedef msg_queue<message*> queue_t;

public:
	receiver()
		:	_count(0) {}

	void enqueue(message* msg)
		{ _queue.push(msg); }

	virtual void terminate()
		{ thread::terminate(); _queue.push(NULL); }

	void check()
	{ 
		_TEST_ASSERT(_count == msgCount); 
	}

protected:
	virtual void run()
	{
		while (1)
		{
			pauto_ptr<message> spMsg(_queue.get()); // blocked receive
			if (!spMsg.get())
				break;
			++_count;
		}
	}

private:
	queue_t _queue;
	size_t _count;
};

} // namespace

//////////////////////////////////////////////////////////////////////
// msg_queue_test
//////////////////////////////////////////////////////////////////////

const char* msg_queue_test::getName() const
{
	return "msg_queue_test";
}

void msg_queue_test::execute()
{
	receiver recv;
	recv.start();

	// check msg_queue::get(infinity)
	for (size_t i = 0; i < msgCount; ++i)
	{
		recv.enqueue(new message(i));
	}

	thread::sleep(500);

	// check msg_queue::get(timeout)
	msg_queue<message*> queue;
	_TEST_ASSERT_EXCEPTION(queue.get(500), pruntime_error);

	recv.stop();
	recv.check();
}
