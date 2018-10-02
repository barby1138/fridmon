// expat_storage.h
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Hyperon Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _HYPERON_EXPAT_H_
#define _HYPERON_EXPAT_H_

#include <hyperon/dom.h>

extern "C" {
#include "xml/expat/expat/expat.h"
}

namespace hyperon
{
	
//////////////////////////////////////////////////
// class expat_parse_error
//
class expat_parse_error : public quark::pruntime_error
{
public:
	expat_parse_error(XML_Error errCode) throw();
	expat_parse_error(const char* fileName, XML_Error errCode, XML_Parser parser) throw();

	~expat_parse_error() throw();
	
	int getErrCode() const { return _errCode; }
	
private:
	XML_Error _errCode;
};

//////////////////////////////////////////////////
// class expat_storage
//
class expat_storage : quark::noncopyable
{
private:
	static void startElementEx(void*, const XML_Char*, const XML_Char**);
	static void endElementEx(void*, const XML_Char*);
	static void charData(void *userData, const XML_Char *s, int len);

	const quark::pstring toString(dom_element* elem, const char* indent = "");

public:
	expat_storage();
	virtual ~expat_storage();

	void load(const char* file);
	void save();

	dom_document* getDocument() { return &_document; }
	dom_element* getRootNode() { return &_rootNode; }
	XML_Parser& getNativeInterface() { return _parser; }
	const char* getFileName() { return _fileName.c_str(); }

	bool loaded() const { return _isLoaded; }

protected:
	virtual dom_element* onSave() = 0;
	virtual void onLoad() = 0;

private:
	XML_Parser _parser;
	dom_document _document;
	dom_element _rootNode;
	dom_element* _currentNode;
	quark::pstring _fileName;
	bool _isLoaded;
};

//////////////////////////////////////////////////
// class expat_storage_buffer
//
class expat_storage_buffer : quark::noncopyable
{
private:
	static void startElementEx(void*, const XML_Char*, const XML_Char**);
	static void endElementEx(void*, const XML_Char*);
	static void charData(void *userData, const XML_Char *s, int len);

	const quark::pstring toString(dom_element* elem, const char* indent = "");

public:
	expat_storage_buffer();
	virtual ~expat_storage_buffer();

	dom_document* getDocument() { return &_document; }
	dom_element* getRootNode() { return &_rootNode; }
	XML_Parser& getNativeInterface() { return _parser; }

	quark::pstring fromString(const quark::pstring& str);
	const quark::pstring toString();
	
protected:
	virtual dom_element* onSave() = 0;
	virtual void onLoad() = 0;

private:
	XML_Parser _parser;
	dom_document _document;
	dom_element _rootNode;
	dom_element* _currentNode;
};

} // namespace hyperon

#endif // _HYPERON_EXPAT_H_
