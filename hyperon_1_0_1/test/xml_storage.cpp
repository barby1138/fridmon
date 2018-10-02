// xml_storage.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "xml_storage.h"
#include <hyperon/gen_attributes.h>
#include <hyperon/element.h>
#include <hyperon/properties.h>
#include <hyperon/storage/expat_storage.h>

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
DECLARE_ELEMENT("logger")

protected:
BEGIN_ATTRIBUTE_MAP()
	MANDATORY_ATTRIBUTE_ENTRY(str_attribute, "level")
	MANDATORY_ATTRIBUTE_ENTRY(str_attribute, "path")
END_ATTRIBUTE_MAP()
};

//////////////////////////////////////////////////
// class siu_entry_element
//
class siu_entry_element : public element
{
public:
DECLARE_ELEMENT("siuEntry")

protected:
BEGIN_ATTRIBUTE_MAP()
	MANDATORY_ATTRIBUTE_ENTRY(str_attribute, "id")
	MANDATORY_ATTRIBUTE_ENTRY(str_attribute, "ftpLogin")
	MANDATORY_ATTRIBUTE_ENTRY(str_attribute, "ftpPwd")
	MANDATORY_ATTRIBUTE_ENTRY(str_attribute, "tlnPwd")
	MANDATORY_ATTRIBUTE_ENTRY(u32_attribute, "port")
END_ATTRIBUTE_MAP()
};

//////////////////////////////////////////////////
// class siu_table_element
//
class siu_table_element : public element
{
public:
DECLARE_ELEMENT("siuTable")

protected:
BEGIN_ELEMENT_MAP()
	MANDATORY_ELEMENT_ENTRY(siu_entry_element)
END_ELEMENT_MAP()
};

//////////////////////////////////////////////////
// class trap_config_entry_element
//
class trap_config_entry_element : public element
{
public:
DECLARE_ELEMENT("trapConfigEntry")

protected:
BEGIN_ATTRIBUTE_MAP()
	KEY_ATTRIBUTE_ENTRY(str_attribute, "id")
	MANDATORY_ATTRIBUTE_ENTRY(str_attribute, "sendLog")
	MANDATORY_ATTRIBUTE_ENTRY(str_attribute, "severity")
	MANDATORY_ATTRIBUTE_ENTRY(str_attribute, "status")
END_ATTRIBUTE_MAP()
};

//////////////////////////////////////////////////
// class trap_config_table_element
//
class trap_config_table_element : public element
{
public:
DECLARE_ELEMENT("trapConfigTable")

protected:
BEGIN_ELEMENT_MAP()
	MANDATORY_ELEMENT_ENTRY(trap_config_entry_element)
END_ELEMENT_MAP()
};

//////////////////////////////////////////////////
// class traps_element
//
class traps_element : public element
{
public:
DECLARE_ELEMENT("traps")

protected:
BEGIN_ELEMENT_MAP()
	MANDATORY_ELEMENT_ENTRY(trap_config_table_element)
END_ELEMENT_MAP()
};

//////////////////////////////////////////////////
// class properties_element
//
class properties_element : public element
{
public:
DECLARE_ELEMENT("properties")

protected:
BEGIN_ELEMENT_MAP()
	MANDATORY_ELEMENT_ENTRY(logger_element)
	MANDATORY_ELEMENT_ENTRY(siu_table_element)
	MANDATORY_ELEMENT_ENTRY(traps_element)
END_ELEMENT_MAP()
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
	MANDATORY_ELEMENT_ENTRY(properties_element)
END_ELEMENT_MAP()
};

typedef properties_sngl<root_element, expat_storage> props;

} // namespace

//////////////////////////////////////////////////////////////////////
// xml_storage_test
//////////////////////////////////////////////////////////////////////

xml_storage_test::xml_storage_test()
	:	test("xml_storage_test")
{
}

void xml_storage_test::execute()
{
	props::instance().load("xml/test.xml");

	// test xml parsing
	_ASSERT(getProperty<u32>(props::instance(), "properties.siuTable.siuEntry", "port") == 9000);

	// test set/get property
	element& elem = props::instance().getElement("properties.siuTable");
	setProperty<pstring>(elem, "siuEntry", "id", "test_value");
	_ASSERT(getProperty<pstring>(props::instance(), "properties.siuTable.siuEntry", "id") == "test_value");

	// test composite index
	hyperon::index idx = make_index("properties.traps.trapConfigTable.trapConfigEntry", pstring("chassisFanFailure"));
	element& elem2 = props::instance().getElement(idx);
	_ASSERT(getProperty<pstring>(elem2, "severity") == "critical");
}
