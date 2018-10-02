// smart_ptr__test.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _NUCLEON_SMARTPTR_TEST_H_
#define _NUCLEON_SMARTPTR_TEST_H_

#include <meson/test.h>

//////////////////////////////////////////////////
// class smart_ptr__test
//
class smart_ptr_test : public meson::test
{
public:
	virtual const char* getName() const;
	virtual void execute();

private:
	void testSmartPtr();
};

#endif // _NUCLEON_SMARTPTR_TEST_H_
