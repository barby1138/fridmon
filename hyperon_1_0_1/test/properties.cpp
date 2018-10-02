// properties.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "properties.h"
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
END_ATTRIBUTE_MAP()
};

typedef properties_sngl<my_element, dummy_storage> props;

} // namespace

//////////////////////////////////////////////////////////////////////
// properties_test
//////////////////////////////////////////////////////////////////////

properties_test::properties_test()
	:	test("properties_test")
{
}

void properties_test::execute()
{
	setProperty<u32>(props::instance(), "prop1", 55);
	_ASSERT(55 == getProperty<u32>(props::instance(), "prop1"));
}
