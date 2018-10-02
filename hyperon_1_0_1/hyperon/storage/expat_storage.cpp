// expat_storage.cpp
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

#include <quark/strings.h>
#include <quark/singleton.h>
#include <nucleon/static_bimap.h>

#include "expat_storage.h"
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

using namespace hyperon;
using namespace quark;
using namespace nucleon;

enum { buff_size = 1024 };

const char* indentChars = "\t";

//////////////////////////////////////////////////
// expat_parse_error
//
expat_parse_error::expat_parse_error(XML_Error errCode) throw()
:	_errCode(errCode),
	pruntime_error(strings::format("xml parse error: %s", XML_ErrorString(errCode)))
{
}

expat_parse_error::expat_parse_error(const char* fileName, XML_Error errCode, XML_Parser parser) throw()
:	_errCode(errCode),
	pruntime_error(strings::format("xml parse error: %s:%d:%d %s", 
		fileName, 
		XML_GetCurrentLineNumber(parser), 
		XML_GetCurrentColumnNumber(parser) + 1, 
		XML_ErrorString(errCode)))
{	
}

expat_parse_error::~expat_parse_error() throw()
{
}

//////////////////////////////////////////////////////////////////////
// utilities
//////////////////////////////////////////////////////////////////////

class xml_encode_map
:	public static_bimap<pstring, pstring, xml_encode_map>
{
public:
	static const char* specialChars;

private:
BEGIN_BIMAP()
	BIMAP_ENTRY("&", "&amp;")
	BIMAP_ENTRY("<", "&lt;")
	BIMAP_ENTRY(">", "&gt;")
END_BIMAP()
};

const char* xml_encode_map::specialChars = "&<>"; 

typedef singleton_holder<xml_encode_map> xml_encode_map_sngl;

pstring encodeXML(const pstring& value)
{
	pstring res = value;
	
	pstring oldVal, newVal;
	size_t pos = res.find_first_of(xml_encode_map::specialChars);
	while (pos != pstring::npos)
	{
		oldVal = res.at(pos);
		newVal = xml_encode_map_sngl::instance().right(oldVal);
		
		res.replace(pos, oldVal.length(), newVal.c_str(), newVal.length());
		
		pos = res.find_first_of(xml_encode_map::specialChars, pos + oldVal.length());
	}	
	
	return res;
}

//////////////////////////////////////////////////////////////////////
// expat_storage constructor/destructor
//////////////////////////////////////////////////////////////////////

expat_storage::expat_storage()
	:	_parser(NULL),
		_rootNode("root"),
		_isLoaded(false)
{
	_currentNode = &_rootNode;

	// memSuite
	//XML_Memory_Handling_Suite memSuite;
	//memSuite.malloc_fcn = memory_suite::malloc;
	//memSuite.realloc_fcn = memory_suite::realloc;
	//memSuite.free_fcn = memory_suite::free;

	_parser = XML_ParserCreate_MM(NULL, /*&memSuite*/NULL, NULL);
	if (NULL == _parser)
		throw pruntime_error("failed to create expat XML parser");
		
	// store our pointer
	XML_SetUserData(_parser, this);

	// register handlers
	XML_SetElementHandler(_parser, expat_storage::startElementEx, expat_storage::endElementEx);
	XML_SetCharacterDataHandler(_parser, expat_storage::charData);
}

expat_storage::~expat_storage()
{
	XML_ParserFree(_parser);
}

//////////////////////////////////////////////////////////////////////
// expat_storage
//////////////////////////////////////////////////////////////////////

void expat_storage::load(const char* file)
{
	_fileName = file;

	FILE* stream = fopen(file, "r");

	if (NULL == stream)
		throw pruntime_error(strings::format("failed to open XML file %s", file));

	while (!feof(stream))
	{
		// allocate buffer for reading from the given as function argument file
		char* buff = (char*)XML_GetBuffer(_parser, buff_size);
		if (NULL == buff)
			break;

		// read bytes to buffer
		size_t len = fread(buff, sizeof(char), buff_size, stream);
		
		// parse buffer
		if (!XML_ParseBuffer(_parser, (int) len, feof(stream)))
			break;
	}

	fclose(stream);

	// if some error occured let`s display it
	XML_Error err = XML_GetErrorCode(_parser);
	if (err != 0)
		throw expat_parse_error(getFileName(), err, _parser);

	// fire event
	onLoad();

	_isLoaded = true;
}

void expat_storage::save() 
{
	// to storage
	dom_element* elem = onSave();

	// skip root element and start from the first child
	if ((!elem) || elem->getElements().empty())
		throw pruntime_error("error while saving XML file");

	pstring str = toString(*elem->getElements().begin());

	if (elem)
		delete elem;

	FILE* stream = fopen(_fileName.c_str(), "w+");
	if (stream == NULL)
		throw pruntime_error(strings::format("failed to open XML file %s", _fileName.c_str()));

	size_t len = fwrite(str.c_str(), sizeof(char), str.size(), stream);

	assert(len != buff_size);

	fclose(stream);
}

const pstring expat_storage::toString(dom_element* elem, const char* indent)
{	
	pstring myIndent(indent);

	pstring elemName = elem->getName();
	pstring elemVal = elem->getValue();
	elemVal = quark::strings::trim(elemVal);
	
	quark::pstring res = myIndent + "<" + elemName;

	dom_element::attributes_t attributes = elem->getAttributes();
	dom_element::attributes_t::const_iterator attrItor = attributes.begin();
	for (;  attrItor != attributes.end(); ++attrItor)
	{
		res += " " + pstring((*attrItor)->getName()) + "=\"" + encodeXML((*attrItor)->getValue()) + "\"";
	}

	dom_element::elements_t elements = elem->getElements();
	if (elements.empty() && (elemVal == ""))
	{
		res += "/>\n";
	}
	else
	{
		res += ">";
		res += encodeXML(elemVal);
		
		if (!elements.empty())
		{
			res += "\n";
			pstring childIndent = myIndent + indentChars;
			dom_element::elements_t::const_iterator elemItor = elements.begin();
			for (; elemItor != elements.end(); ++elemItor)
			{
				res += toString((*elemItor), childIndent.c_str());
			}

			res += myIndent;
		}
		
		res += "</" + elemName + ">\n";
	}

	return res;
}

//////////////////////////////////////////////////////////////////////
// parse functions
//////////////////////////////////////////////////////////////////////

void expat_storage::startElementEx(void* userData, const XML_Char* name, const XML_Char** attrs)
{
	expat_storage* self = static_cast<expat_storage*>(userData);

	// create the element
	dom_element* elem = new dom_element(pstring(name), self->_currentNode);

	// add attributes
	while (*attrs && *(attrs + 1))
	{
		dom_node* node = new dom_attribute(pstring(*attrs), pstring(*(attrs + 1)), elem);
		elem->addChild(node);
		attrs += 2;
	}

	// add into the element tree
	self->_currentNode->addChild(elem);

	self->_currentNode = elem;
}

void expat_storage::endElementEx(void* userData, const XML_Char* name)
{
	expat_storage* self = static_cast<expat_storage*>(userData);
	self->_currentNode = static_cast<dom_element*>(self->_currentNode->getParent());
}

void expat_storage::charData(void *userData, const XML_Char *s, int len)
{
	expat_storage* self = static_cast<expat_storage*>(userData);
	
	// set element value
	pstring val(s, (s + len));
	self->_currentNode->setValue(val);
}

//////////////////////////////////////////////////////////////////////
// expat_storage_buffer constructor/destructor
//////////////////////////////////////////////////////////////////////

expat_storage_buffer::expat_storage_buffer()
	:	_parser(NULL),
		_rootNode("root")
{
	_currentNode = &_rootNode;

	// memSuite
	//XML_Memory_Handling_Suite memSuite;
	//memSuite.malloc_fcn = memory_suite::malloc;
	//memSuite.realloc_fcn = memory_suite::realloc;
	//memSuite.free_fcn = memory_suite::free;

	_parser = XML_ParserCreate_MM(NULL, NULL, NULL);
	if (NULL == _parser)
		throw pruntime_error("failed to create expat XML parser");
		
	// store our pointer
	XML_SetUserData(_parser, this);

	// register handlers
	XML_SetElementHandler(_parser, expat_storage_buffer::startElementEx, expat_storage_buffer::endElementEx);
	XML_SetCharacterDataHandler(_parser, expat_storage_buffer::charData);
}

expat_storage_buffer::~expat_storage_buffer()
{
	XML_ParserFree(_parser);
}

//////////////////////////////////////////////////////////////////////
// expat_storage_buffer
//////////////////////////////////////////////////////////////////////

pstring expat_storage_buffer::fromString(const pstring& str)
{
	pstring remainingString;

	XML_Status xmlParseStatus = XML_Parse(_parser, str.c_str(), (int) str.size(), true);
	if (xmlParseStatus != XML_STATUS_OK)
	{
		XML_Error xmlError = XML_GetErrorCode(_parser);
		if (xmlError == XML_ERROR_JUNK_AFTER_DOC_ELEMENT)
		{
			int offset, size;
			if (XML_GetInputContext(_parser, &offset, &size))
			{
				remainingString.append(str.c_str(), offset, size - offset);
			}
		}
		else
		{
			XML_ParserReset(_parser, NULL);
			throw expat_parse_error(xmlError);
		}
	}

	// fire event
	onLoad();

	return remainingString;
}

const pstring expat_storage_buffer::toString(dom_element* elem, const char* indent)
{
	pstring myIndent(indent);
	
	pstring elemName = elem->getName();
	pstring elemVal = elem->getValue();
	elemVal = quark::strings::trim(elemVal);
	
	quark::pstring res = myIndent + "<" + elemName;
	
	dom_element::attributes_t attributes = elem->getAttributes();
	dom_element::attributes_t::const_iterator attrItor = attributes.begin();
	for (;  attrItor != attributes.end(); ++attrItor)
	{
		res += " " + pstring((*attrItor)->getName()) + "=\"" + encodeXML((*attrItor)->getValue()) + "\"";
	}
	
	dom_element::elements_t elements = elem->getElements();
	if (elements.empty() && (elemVal == ""))
	{
		res += "/>\n";
	}
	else
	{
		res += ">";
		res += encodeXML(elemVal);
		
		if (!elements.empty())
		{
			res += "\n";
			pstring childIndent = myIndent + indentChars;
			dom_element::elements_t::const_iterator elemItor = elements.begin();
			for (; elemItor != elements.end(); ++elemItor)
			{
				res += toString((*elemItor), childIndent.c_str());
			}
			
			res += myIndent;
		}
		
		res += "</" + elemName + ">\n";
	}
	
	return res;
}

const pstring expat_storage_buffer::toString() 
{
	// to storage
	dom_element* elem = onSave();

	pstring str("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n");
	// skip root element and start from the first child
	if (elem && !elem->getElements().empty())
		str += toString(*elem->getElements().begin());

	if (elem)
		delete elem;

	return str;
}

//////////////////////////////////////////////////////////////////////
// parse functions
//////////////////////////////////////////////////////////////////////

void expat_storage_buffer::startElementEx(void* userData, const XML_Char* name, const XML_Char** attrs)
{
	expat_storage_buffer* self = static_cast<expat_storage_buffer*>(userData);

	// create the element
	dom_element* elem = new dom_element(pstring(name), self->_currentNode);

	// add attributes
	while (*attrs && *(attrs + 1))
	{
		dom_node* node = new dom_attribute(pstring(*attrs), pstring(*(attrs + 1)), elem);
		elem->addChild(node);
		attrs += 2;
	}

	// add into the element tree
	self->_currentNode->addChild(elem);

	self->_currentNode = elem;
}

void expat_storage_buffer::endElementEx(void* userData, const XML_Char* name)
{
	expat_storage_buffer* self = static_cast<expat_storage_buffer*>(userData);
	self->_currentNode = static_cast<dom_element*>(self->_currentNode->getParent());
}

void expat_storage_buffer::charData(void *userData, const XML_Char *s, int len)
{
	expat_storage_buffer* self = static_cast<expat_storage_buffer*>(userData);

	// set element value
	pstring val(s, (s + len));
	self->_currentNode->setValue(pstring(self->_currentNode->getValue()).append(val));
}

