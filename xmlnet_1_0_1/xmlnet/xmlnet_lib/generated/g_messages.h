// g_messages.h
//
//////////////////////////////////////////////////////////////////////

#include "..\messages.h"
#include "g_xml_schemes.h"

class xml_getModuleVersionRequest_message : public xml_request_message
{
public:
DECLARE_SCHEME(getModuleVersionRequest_element)
};

class xml_getModuleVersionResponse_message : public xml_response_message
{
public:
DECLARE_SCHEME(getModuleVersionResponse_element)

	pstring _version;
};

class xml_getModuleversionFailure_message : public xml_failure_message
{
public:
DECLARE_SCHEME(getModuleVersionFailure_element)
};

#define XML_MESSAGE_TYPE_ENTRY(type) \
	TYPE_ENTRY(type::getClassName(), type)

class xml_message_factory : public static_factory<xml_message, pstring>
{
protected:
	BEGIN_TYPE_MAP()
		XML_MESSAGE_TYPE_ENTRY(xml_parser_error_response_message)
		XML_MESSAGE_TYPE_ENTRY(xml_unknown_request_response_message)
		XML_MESSAGE_TYPE_ENTRY(xml_keepalive_request_message)
		XML_MESSAGE_TYPE_ENTRY(xml_keepalive_response_message)
		XML_MESSAGE_TYPE_ENTRY(xml_keepalive_failure_message)
		XML_MESSAGE_TYPE_ENTRY(xml_getModuleVersionRequest_message)
		XML_MESSAGE_TYPE_ENTRY(xml_getModuleVersionResponse_message)
		XML_MESSAGE_TYPE_ENTRY(xml_getModuleversionFailure_message)
	END_TYPE_MAP()
};

typedef singleton_holder<xml_message_factory, create_using_new> xml_message_factory_sngl;

template <class T>
	class xml_message_dispatcher_t : public static_dispatcher<T, pstring, xml_message, void>
{};

#define XML_DISP_ENTRY(id, type, fn) \
	disp::handler_type handler##id = disp::handler_maker<type>()(&fn); \
	map[type::scheme_t::getClassName()] = handler##id;

class xml_message_dispatcher : public xml_message_dispatcher_t<xml_message_dispatcher>
{
public:
	void process(const xml_parser_error_response_message& msg) {}
	void process(const xml_unknown_request_response_message& msg) {}
	void process(const xml_keepalive_request_message& msg) {}
	void process(const xml_keepalive_response_message& msg) {}
	void process(const xml_keepalive_failure_message& msg) {}
	void process(const xml_getModuleVersionRequest_message& msg) {}
	void process(const xml_getModuleVersionResponse_message& msg) {}
	void process(const xml_getModuleversionFailure_message& msg) {}

public:
	typedef xml_message_dispatcher_t<xml_message_dispatcher> disp;
	BEGIN_DISP_MAP(disp::dispatcher_type)
		XML_DISP_ENTRY(0, xml_parser_error_response_message, xml_message_dispatcher::process)
		XML_DISP_ENTRY(1, xml_unknown_request_response_message, xml_message_dispatcher::process)
		XML_DISP_ENTRY(2, xml_keepalive_request_message, xml_message_dispatcher::process)
		XML_DISP_ENTRY(3, xml_keepalive_response_message, xml_message_dispatcher::process)
		XML_DISP_ENTRY(4, xml_keepalive_failure_message, xml_message_dispatcher::process)
		XML_DISP_ENTRY(5, xml_getModuleVersionRequest_message, xml_message_dispatcher::process)
		XML_DISP_ENTRY(6, xml_getModuleVersionResponse_message, xml_message_dispatcher::process)
		XML_DISP_ENTRY(7, xml_getModuleversionFailure_message, xml_message_dispatcher::process)
	END_DISP_MAP()
};

template <>
	class xml_api_scheme<null_type> : public element
{
public:
	DECLARE_ELEMENT("XML_API")

protected:
	BEGIN_ELEMENT_MAP()
		MANDATORY_ELEMENT_ENTRY(version_element)
		OPTIONAL_ELEMENT_ENTRY(xml_parser_error_response_message::scheme_t)
		OPTIONAL_ELEMENT_ENTRY(xml_unknown_request_response_message::scheme_t)
		OPTIONAL_ELEMENT_ENTRY(xml_keepalive_request_message::scheme_t)
		OPTIONAL_ELEMENT_ENTRY(xml_keepalive_response_message::scheme_t)
		OPTIONAL_ELEMENT_ENTRY(xml_keepalive_failure_message::scheme_t)
		OPTIONAL_ELEMENT_ENTRY(xml_getModuleVersionRequest_message::scheme_t)
		OPTIONAL_ELEMENT_ENTRY(xml_getModuleVersionResponse_message::scheme_t)
		OPTIONAL_ELEMENT_ENTRY(xml_getModuleversionFailure_message::scheme_t)
	END_ELEMENT_MAP()
};
