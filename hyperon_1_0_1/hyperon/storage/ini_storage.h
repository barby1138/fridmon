// ini_storage.h
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
#include "ini/ini_file.h"

namespace hyperon
{

//////////////////////////////////////////////////
// class ini_storage
//
class ini_storage : quark::noncopyable
{
public:
	ini_storage();
	virtual ~ini_storage();

	void load(const char* file);

	dom_document* getDocument() { return &_document; }
	dom_element* getRootNode() { return &_rootNode; }
	ini_file& getNativeInterface() { return _parser; }

protected:
	virtual void onLoad() {}

private:
	void parse();
	void parseSection(const ini_section& section);

private:
	ini_file _parser;
	dom_document _document;
	dom_element _rootNode;
};

} // namespace hyperon

#endif // _HYPERON_EXPAT_H_
