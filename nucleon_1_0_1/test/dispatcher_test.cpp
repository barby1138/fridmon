// dispatcher_test.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "dispatcher_test.h"
#include <nucleon/dispatcher.h>

using namespace quark;
using namespace nucleon;

namespace 
{

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

////////////////////////////////////
// host
//
class host : public static_dispatcher<host, 4, message, bool>,
			 public static_dispatcher<host, 4, message, void>
{
private:
#define DISP1 static_dispatcher<host, 4, message, bool>
#define DISP2 static_dispatcher<host, 4, message, void>
	typedef DISP1 disp1;
	typedef DISP2 disp2;

public:
	bool handle1(message& msg)
		{ return disp1::dispatch(msg.getId(), msg); }

	bool handle2(message& msg)
		{ return disp2::dispatch(msg.getId(), msg); }

private:
	bool onMessage1(message1& msg)
		{ _TEST_ASSERT(msg.getId() == 1); return true; }

	bool onMessage3(const message3& msg)
		{ _TEST_ASSERT(msg.getId() == 3); return false; }

	void onMessage1Void(message1& msg)
		{ _TEST_ASSERT(msg.getId() == 1); }

	void onMessage3Void(const message3& msg)
		{ _TEST_ASSERT(msg.getId() == 3); }

BEGIN_DISP_MAP(DISP1)
	DISP_ENTRY(message1::getClassId(), message1, host::onMessage1)
	DISP_ENTRY(message3::getClassId(), message3, host::onMessage3)
END_DISP_MAP()

BEGIN_DISP_MAP(DISP2)
	DISP_ENTRY(message1::getClassId(), message1, host::onMessage1Void)
	DISP_ENTRY(message3::getClassId(), message3, host::onMessage3Void)
END_DISP_MAP()
};

} // namespace

//////////////////////////////////////////////////////////////////////
// dispatcher_test
//////////////////////////////////////////////////////////////////////

const char* dispatcher_test::getName() const
{
	return "dispatcher_test";
}

void dispatcher_test::execute()
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
	_TEST_ASSERT(hst.handle2(msg1));
	_TEST_ASSERT(hst.handle2(msg3));
	_TEST_ASSERT(!hst.handle2(msg2));
}
