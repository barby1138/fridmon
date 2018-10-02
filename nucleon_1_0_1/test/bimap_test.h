// bimap_test.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _NUCLEON_BIMAP_TEST_H_
#define _NUCLEON_BIMAP_TEST_H_

#include <meson/test.h>

//////////////////////////////////////////////////
// class bimap_test
//
class bimap_test : public meson::test
{
public:
	virtual const char* getName() const;
	virtual void execute();

private:
	void testStaticBimap();
};

#endif // _NUCLEON_BIMAP_TEST_H_
