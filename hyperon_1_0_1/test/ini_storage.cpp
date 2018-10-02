// ini_storage.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "ini_storage.h"
#include <hyperon/gen_attributes.h>
#include <hyperon/element.h>
#include <hyperon/properties.h>
#include <hyperon/storage/ini_storage.h>

using namespace hyperon;
using namespace quark;

namespace
{

//////////////////////////////////////////////////
// class logger_element
//
class logger_element : public element
{
public:
DECLARE_ELEMENT("LOGGER")

protected:
BEGIN_ATTRIBUTE_MAP()
	MANDATORY_ATTRIBUTE_ENTRY(str_attribute, "level")
	MANDATORY_ATTRIBUTE_ENTRY(str_attribute, "path")
END_ATTRIBUTE_MAP()
};

//////////////////////////////////////////////////
// class sccp_nm_element
//
class sccp_nm_element : public element
{
public:
DECLARE_ELEMENT("SCCP_NM")

protected:
BEGIN_ATTRIBUTE_MAP()
	MANDATORY_ATTRIBUTE_ENTRY(u32_attribute, "spc")
	MANDATORY_ATTRIBUTE_ENTRY(u32_attribute, "ssn")
END_ATTRIBUTE_MAP()
};

//////////////////////////////////////////////////
// class root_element
//
class root_element : public element
{
public:
DECLARE_ELEMENT("root")

protected:
BEGIN_ELEMENT_MAP()
	MANDATORY_ELEMENT_ENTRY(logger_element)
	MANDATORY_ELEMENT_ENTRY(sccp_nm_element)
END_ELEMENT_MAP()
};

typedef properties_sngl<root_element, ini_storage> props;

} // namespace

//////////////////////////////////////////////////////////////////////
// ini_storage_test
//////////////////////////////////////////////////////////////////////

ini_storage_test::ini_storage_test()
	:	test("ini_storage_test")
{
}

void ini_storage_test::execute()
{
	props::instance().load("ini/test.ini");

	// test ini file parsing
	_ASSERT(getProperty<pstring>(props::instance(), "LOGGER", "level") == "debug");

	// test set/get property
	element& elem = props::instance().getElement("SCCP_NM");
	setProperty<u32>(elem, "spc", 555);
	_ASSERT(getProperty<u32>(props::instance(), "SCCP_NM", "spc") == 555);
}
