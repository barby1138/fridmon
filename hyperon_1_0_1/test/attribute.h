// attribute.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HYPERON_TEST_ATTRIBUTE_H_
#define _HYPERON_TEST_ATTRIBUTE_H_

#include "test.h"
#include <hyperon/attribute.h>

//////////////////////////////////////////////////
// class attribute_test
//
class attribute_test : public test
{
public:
	attribute_test();

	virtual void execute();

private:
	void testIntAttribute(hyperon::attribute& attr);
	void testStrAttribute(hyperon::attribute& attr);
};

#endif // _HYPERON_TEST_ATTRIBUTE_H_


