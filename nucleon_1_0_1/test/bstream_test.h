// bstream_test.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _NUCLEON_BSTREAM_TEST_H_
#define _NUCLEON_BSTREAM_TEST_H_

#include <meson/test.h>

//////////////////////////////////////////////////
// class bstream_test
//
class bstream_test : public meson::test
{
public:
	virtual const char* getName() const;
	virtual void execute();

private:
	void testWordStream();
	void testByteStream();
};

#endif // _NUCLEON_BSTREAM_TEST_H_
