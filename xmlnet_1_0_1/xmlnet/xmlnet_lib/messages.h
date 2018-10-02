// messages.h
//
//////////////////////////////////////////////////////////////////////

#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include "xml_schemes.h"

//
// messages
//
#define DECLARE_SCHEME(ConcreteSchemeElement) \
	typedef xml_api_scheme<ConcreteSchemeElement> scheme_t;\
	virtual const char* getName() const { return ConcreteSchemeElement::getClassName(); } \
	static const char* getClassName() { return ConcreteSchemeElement::getClassName(); }
	
class xml_message
{
public:	
	virtual const char* getName() const = 0;

	virtual void fromString(const pstring& str) {}
	virtual const pstring toString() const { return ""; }

	void setConnectionId(u32 id) { _connId = id; }
	u32 getConnectionId() const { return _connId; }

	const pstring& getVersion() const { return _version; }
	void setVersion(const pstring& version) { _version = version; }

	virtual ~xml_message() {}

protected:
	xml_message()	
		:	_version("1.0")
	{}

private:
	u32 _connId;
	pstring _version;
};

class xml_request_message : public xml_message
{
public:	
	u32 getRequestId() const { return _requestId; }
	void setRequestId(u32 value) { _requestId = value; }

protected:
	u32	_requestId;
};

class xml_response_message : public xml_message
{
public:
	xml_response_message()
	{ setReturnValue("OK"); }

	const pstring& getReturnValue() const { return _returnValue; }
	void setReturnValue(const pstring& return_value) { _returnValue = return_value; }
	
	u32 getRequestId() const { return _requestId; }
	void setRequestId(u32 value) { _requestId = value; }

private:
	pstring _returnValue;
	u32	_requestId;
};

class xml_failure_message : public xml_response_message
{
public:
	xml_failure_message()
	{ setReturnValue("Failure"); }
};

class xml_notification_message : public xml_message
{
public:
	void setCookie(const u32& cookie) { _cookie = cookie; }
	u32 getCookie() const { return _cookie; }

	void setNotificationId(const u32& notificationId) { _notificationId = notificationId; }
	u32 getNotificationId() const { return _notificationId; }

private:
	u32 _cookie;
	u32 _notificationId;
};

class xml_parser_error_response_message : public xml_response_message
{
public:
	DECLARE_SCHEME(parser_error_response_element)

public:
	xml_parser_error_response_message()
	{ setReturnValue("InvalidXML"); }
};

class xml_unknown_request_response_message : public xml_response_message
{
public:
	DECLARE_SCHEME(unknown_request_response_element)

public:
	xml_unknown_request_response_message()
	{ setReturnValue("XMLUnknownRequest"); }
	
public:
	const pstring& getUnknownRequest() const { return _unknownRequest; }
	void setUnknownRequest(const pstring& unknownRequest) { _unknownRequest = unknownRequest; }
	
private:
	pstring _unknownRequest;
};

class xml_keepalive_request_message : public xml_request_message
{
public:
	DECLARE_SCHEME(keepalive_request_element)
};

class xml_keepalive_response_message : public xml_response_message
{
public:
	DECLARE_SCHEME(keepalive_response_element)
};

class xml_keepalive_failure_message : public xml_response_message
{
public:
	DECLARE_SCHEME(keepalive_response_element)
};

#endif // _MESSAGES_H_
