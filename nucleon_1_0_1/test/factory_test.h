// factory_test.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _NUCLEON_FACTORY_TEST_H_
#define _NUCLEON_FACTORY_TEST_H_

#include <meson/test.h>

//////////////////////////////////////////////////
// class factory_test
//
class factory_test : public meson::test
{
public:
	virtual const char* getName() const;
	virtual void execute();

private:
	void testStaticFactory();
};

#endif // _NUCLEON_FACTORY_TEST_H_
