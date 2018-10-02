// smart_ptr__test.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "smart_ptr_test.h"
#include <nucleon/smart_ptr.h>

using namespace quark;
using namespace nucleon;

//////////////////////////////////////////////////////////////////////
// smart_ptr__test
//////////////////////////////////////////////////////////////////////

const char* smart_ptr_test::getName() const
{
	return "smart_ptr_test";
}

void smart_ptr_test::execute()
{
	testSmartPtr();
}

class testObj : public pooled_object
{
public:
	testObj()
		:	_foo(0)
	{
		printf("foo::ctor()\n");
		destroyed = false;
	}

	~testObj() 
	{
		printf("foo::dtor()\n");
		destroyed = true;
	}

	void setFoo(u32 foo) { _foo = foo; }
	u32 getFoo() const { return _foo; }

	static bool destroyed;

private:
	u32 _foo;
};

bool testObj::destroyed = true;

void smart_ptr_test::testSmartPtr()
{
	testObj* p = new testObj;

	{
		smart_ptr<testObj> p2;
	
		{	
			printf("p1 scope begin\n");
			smart_ptr<testObj> p1 = p;
			p2 = p1;
			printf("p1 scope end\n");
		}

		_TEST_ASSERT(!testObj::destroyed);
		(*p2).setFoo(1);
		p2->setFoo(2);
	}

	_TEST_ASSERT(testObj::destroyed);
}
