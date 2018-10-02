// g_xml_schemes.h
//
//////////////////////////////////////////////////////////////////////

#include "..\xml_schemes.h"

class getModuleVersionRequest_element : public element
{
public:
	DECLARE_ELEMENT("getModuleVersionRequest")

protected:
	BEGIN_ELEMENT_MAP()
		MANDATORY_ELEMENT_ENTRY(request_id_element)
	END_ELEMENT_MAP()
};

class getModuleVersionResponse_version_element : public element
{
public:
	DECLARE_ELEMENT("version")
};

class getModuleVersionResponse_element : public element
{
public:
	DECLARE_ELEMENT("getModuleVersionResponse")

protected:
	BEGIN_ELEMENT_MAP()
		MANDATORY_ELEMENT_ENTRY(request_id_element)
		MANDATORY_ELEMENT_ENTRY(return_value_element)
		MANDATORY_ELEMENT_ENTRY(getModuleVersionResponse_version_element)
	END_ELEMENT_MAP()
};

class getModuleVersionFailure_element : public element
{
public:
	DECLARE_ELEMENT("getModuleVersionFailure_element")

protected:
	BEGIN_ELEMENT_MAP()
		MANDATORY_ELEMENT_ENTRY(request_id_element)
		MANDATORY_ELEMENT_ENTRY(return_value_element)
	END_ELEMENT_MAP()
};
