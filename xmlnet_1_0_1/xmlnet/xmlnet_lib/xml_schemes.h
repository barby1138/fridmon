// xml_schemes.h
//
//////////////////////////////////////////////////////////////////////

#ifndef _XML_SCHEME_H_
#define _XML_SCHEME_H_

class version_element : public element
{
public:
	DECLARE_ELEMENT("Version")
};

class request_id_element : public element
{
public:
	DECLARE_ELEMENT("RequestID")
};

class return_value_element : public element
{
public:
	DECLARE_ELEMENT("ReturnValue")
};

class notification_id_element : public element
{
public:
	DECLARE_ELEMENT("NotificationID")
};

class failure_element_base : public element
{		
protected:
	BEGIN_ELEMENT_MAP()
		MANDATORY_ELEMENT_ENTRY(request_id_element)
		MANDATORY_ELEMENT_ENTRY(return_value_element)
	END_ELEMENT_MAP()
};

class parser_error_response_element : public element
{
public:
	DECLARE_ELEMENT("XMLParser_Error")

protected:
	BEGIN_ELEMENT_MAP()
		MANDATORY_ELEMENT_ENTRY(return_value_element)
	END_ELEMENT_MAP()
};

class unknown_request_element : public element
{
public:
	DECLARE_ELEMENT("Unknown_Request")
};

class unknown_request_response_element : public element
{
public:
	DECLARE_ELEMENT("Unknown_Request_Response")

protected:
	BEGIN_ELEMENT_MAP()
		MANDATORY_ELEMENT_ENTRY(request_id_element)
		MANDATORY_ELEMENT_ENTRY(unknown_request_element)
		MANDATORY_ELEMENT_ENTRY(return_value_element)
	END_ELEMENT_MAP()
};

class keepalive_request_element : public element
{
public:
	DECLARE_ELEMENT("KeepAlive_Request")

protected:
	BEGIN_ELEMENT_MAP()
		MANDATORY_ELEMENT_ENTRY(request_id_element)
	END_ELEMENT_MAP()
};

class keepalive_response_element : public element
{
public:
	DECLARE_ELEMENT("KeepAlive_Response")

protected:
	BEGIN_ELEMENT_MAP()
		MANDATORY_ELEMENT_ENTRY(request_id_element)
		MANDATORY_ELEMENT_ENTRY(return_value_element)
	END_ELEMENT_MAP()
};

template <class _ConcreteSchemeElement>
	class xml_api_scheme : public element
{
public:
	DECLARE_ELEMENT("XML_API")

protected:
	BEGIN_ELEMENT_MAP()
		MANDATORY_ELEMENT_ENTRY(version_element)
		MANDATORY_ELEMENT_ENTRY(_ConcreteSchemeElement)
	END_ELEMENT_MAP()
};

template <class _XmlApiSchemeElement>
class xml_api_root_element : public element
{
public:
	typedef properties<xml_api_root_element<_XmlApiSchemeElement>, expat_storage_buffer> props;

	DECLARE_ELEMENT("root")

protected:
	BEGIN_ELEMENT_MAP()
		MANDATORY_ELEMENT_ENTRY(_XmlApiSchemeElement)
	END_ELEMENT_MAP()
};

#endif // _XML_SCHEME_H_
