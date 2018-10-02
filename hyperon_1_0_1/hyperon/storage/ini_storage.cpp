// ini_storage.cpp
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

#include "ini_storage.h"

using namespace hyperon;
using namespace quark;

//////////////////////////////////////////////////////////////////////
// ini_storage constructor/destructor
//////////////////////////////////////////////////////////////////////

ini_storage::ini_storage()
	:	_rootNode("root")
{
}

ini_storage::~ini_storage()
{
}

//////////////////////////////////////////////////////////////////////
// ini_storage
//////////////////////////////////////////////////////////////////////

void ini_storage::load(const char* file)
{
	if (!_parser.load(file))
		throw pruntime_error("failed to open INI file");

	parse();

	// fire event
	onLoad();
}

//////////////////////////////////////////////////////////////////////
// parse functions
//////////////////////////////////////////////////////////////////////

void ini_storage::parse()
{
	const ini_file::sections_t& sections = _parser.getSections();
	for (ini_file::sections_t::const_iterator itor = sections.begin(); 
		 itor != sections.end(); ++itor)
	{
		parseSection(itor->second);
	}
}

void ini_storage::parseSection(const ini_section& section)
{
	// create the element
	dom_element* elem = new dom_element(section.getName(), &_rootNode);

	// add attributes
	const ini_section::records_t& records = section.getRecords();
	for (ini_section::records_t::const_iterator itor = records.begin(); 
		 itor != records.end(); ++itor)
	{
		const ini_record& record = itor->second;
		dom_node* node = new dom_attribute(itor->first, record.value, elem);
		elem->addChild(node);
	}

	// add into the element tree
	_rootNode.addChild(elem);
}
