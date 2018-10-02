// element.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "element.h"
#include <hyperon/element.h>
#include <hyperon/properties.h>
#include <hyperon/storage/dummy_storage.h>

using namespace hyperon;
using namespace quark;

namespace
{

//////////////////////////////////////////////////
// class my_sub_element
//
struct my_sub_element : public element
{
DECLARE_ELEMENT("elem1")
};

//////////////////////////////////////////////////
// class my_element2
//
class my_element : public element
{
public:
DECLARE_ELEMENT("my_element");

protected:
BEGIN_ELEMENT_MAP()
	MANDATORY_ELEMENT_ENTRY(my_sub_element)
END_ELEMENT_MAP()
};

typedef properties_sngl<my_element, dummy_storage> props;

} // namespace

//////////////////////////////////////////////////////////////////////
// element_test
//////////////////////////////////////////////////////////////////////

element_test::element_test()
	:	test("element_test")
{
}

void element_test::execute()
{
	element& elem = props::instance();

	elem.getElement("elem1");
	elem.getElement(index_item("elem1"));
	
	// fail cases
	try { 
		elem.getElement(index_item("elem1", 1)); _ASSERT(false);
	}
	catch (quark::pruntime_error& ) {}
}
