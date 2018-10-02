// dispatcher_test.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "event_dispatcher_test.h"
#include <nucleon/event_dispatcher.h>

using namespace quark;
using namespace nucleon;

//namespace // anonymous
//{

////////////////////////////////////
// messages
//
struct message
{
	virtual ~message() {}
	virtual u8 getId() const = 0;
};

template<const u8 _Id>
struct messageT : message
{
	virtual u8 getId() const { return _Id; }
	static u8 getClassId() { return _Id; }
};

struct message1 : messageT<1>
{
};

struct message2 : messageT<2>
{
};

struct message3 : messageT<3>
{
};

enum { maxMsgId = 4 };

////////////////////////////////////
// host
//
class host
{
public:
DECLARE_DISPATCHER(disp1)
DECLARE_DISPATCHER(disp2);

public:
	bool handle1(message& msg);
	void handle2(message& msg);

private:
	bool onMessage(message1& msg)
		{ _TEST_ASSERT(msg.getId() == 1); return true; }

	bool onMessage(const message3& msg)
		{ _TEST_ASSERT(msg.getId() == 3); return false; }

	void onMessage1Void(message1& msg)
		{ _TEST_ASSERT(msg.getId() == 1); }

	void onMessage3Void(const message3& msg)
		{ _TEST_ASSERT(msg.getId() == 3); }
};

#define MSG_ENTRY(arg) \
	DISPATCH_ENTRY2(arg::getClassId(), &host_type::onMessage, arg)

BEGIN_DISPATCHER(disp1, host, message, bool)
	MSG_ENTRY(message1)
	MSG_ENTRY(message3)
END_DISPATCHER()

BEGIN_DISPATCHER(disp2, host, message, void)
	DISPATCH_ENTRY(message1::getClassId(), &host::onMessage1Void)
	DISPATCH_ENTRY(message3::getClassId(), &host::onMessage3Void)
END_DISPATCHER()

bool host::handle1(message& msg)
{ 
	disp1_impl::result_type res = disp1::instance().dispatch(this, msg.getId(), msg);
	//_TEST_ASSERT(res.disp_result);
	return res.fn_result;
}

void host::handle2(message& msg)
{ 
	disp2::instance().dispatch(this, msg.getId(), msg).disp_result;
}

//} // namespace anonymous

//////////////////////////////////////////////////////////////////////
// event_dispatcher_test
//////////////////////////////////////////////////////////////////////

const char* event_dispatcher_test::getName() const
{
	return "event_dispatcher_test";
}

void event_dispatcher_test::execute()
{
	host hst;
	message1 msg1;
	message2 msg2;
	message3 msg3;

	// check dispatcher1
	_TEST_ASSERT(hst.handle1(msg1));
	_TEST_ASSERT(!hst.handle1(msg3));
	_TEST_ASSERT(!hst.handle1(msg2));

	// check dispatcher2
	hst.handle2(msg1);
	hst.handle2(msg3);
	hst.handle2(msg2);
}
