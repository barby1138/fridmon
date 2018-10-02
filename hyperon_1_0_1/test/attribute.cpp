// attribute.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "attribute.h"
#include <hyperon/gen_attributes.h>
#include <hyperon/element.h>
#include <hyperon/properties.h>
#include <hyperon/storage/dummy_storage.h>

using namespace hyperon;
using namespace quark;

namespace
{

//////////////////////////////////////////////////
// class my_element
//
class my_element : public element
{
public:
DECLARE_ELEMENT("my_element");

protected:
BEGIN_ATTRIBUTE_MAP()
	MANDATORY_ATTRIBUTE_ENTRY(u32_attribute, "prop1")
	MANDATORY_ATTRIBUTE_ENTRY(str_attribute, "prop2")
END_ATTRIBUTE_MAP()
};

typedef properties_sngl<my_element, dummy_storage> props;

} // namespace

//////////////////////////////////////////////////////////////////////
// attribute_test
//////////////////////////////////////////////////////////////////////

attribute_test::attribute_test()
	:	test("attribute_test")
{
}

void attribute_test::execute()
{
	element& elem = props::instance();

	testIntAttribute(elem.getAttribute("prop1"));
	testStrAttribute(elem.getAttribute("prop2"));
	
	// fail cases
	try { 
		elem.getAttribute("prop10"); _ASSERT(false);
	}
	catch (quark::pruntime_error& ) {}
	try { 
		testIntAttribute(elem.getAttribute("prop2")); _ASSERT(false);
	}
	catch (quark::pruntime_error& ) {}
}

void attribute_test::testIntAttribute(attribute& attr)
{
	attr.setValue(u32(1));
	_ASSERT(attr.getValue() == u32(1));
	
	try { 
		attr.setValue(quark::pstring("Hello")); _ASSERT(false);
	}
	catch (quark::pruntime_error& ) {}
}

void attribute_test::testStrAttribute(attribute& attr)
{
	attr.setValue(quark::pstring("Hello"));
	_ASSERT(attr.getValue() == quark::pstring("Hello"));
	
	try { 
		attr.setValue(1); _ASSERT(false);
	}
	catch (quark::pruntime_error& ) {}
}
