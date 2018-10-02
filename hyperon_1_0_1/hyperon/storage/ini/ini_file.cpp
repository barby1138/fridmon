// ini_file.cpp: implementation of the ini_file class.
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

////////////////////////////////////////////////////////////////////////////////
// Redesigned from Todd Davis's original source code located on
//     http://www.codeproject.com/file/Cini_file.asp.
////////////////////////////////////////////////////////////////////////////////

#include "ini_file.h"
#include <quark/strings.h>
#include <fstream>

using namespace hyperon;
using namespace quark;

//////////////////////////////////////////////////////////////////////
// ini_section_exception
//////////////////////////////////////////////////////////////////////

ini_section_exception::ini_section_exception(const char* name, const char* descr)
	:	pruntime_error(strings::format("ini section: %s; %s", name, descr))
{
}

//////////////////////////////////////////////////////////////////////
// ini_section
//////////////////////////////////////////////////////////////////////

ini_section::ini_section()
{
}

ini_section::ini_section(const pstring& header, const pstring& comments)
{
	pstring name = header;
	name.erase(name.begin());		// erase the leading bracket
	name.erase(name.find(']'));		// erase the trailing bracket
	_comments = comments;
	_name = name;
}

void ini_section::addValue(const pstring& record, const pstring& comments)
{
	// set the Key value to everything before the = sign
	pstring key = record.substr(0,record.find('='));
	pstring value = record.substr(record.find('=') + 1);
	_records[key] = ini_record(value, comments);
}

const pstring ini_section::getValue(const pstring& key) const
{
	records_t::const_iterator itor = _records.find(key);
	if (itor == _records.end())
		throw ini_section_exception(getName(),
			strings::format("specified key '%s' does not exist", key.c_str()).c_str());
	return itor->second.value;
}

const ini_section::records_t& ini_section::getRecords() const
{
	return _records;
}

//////////////////////////////////////////////////////////////////////
// ini_file
//////////////////////////////////////////////////////////////////////

ini_file::ini_file()
{
}

ini_file::~ini_file()
{
}

bool ini_file::load(const pstring& file)
{
	pstring str;

	std::ifstream inFile(file.c_str());
	if (!inFile.is_open()) 
		return false;

	_fileName = file;

	_sections.clear();
	sections_t::iterator currentSection = _sections.end();

	pstring comments = "";

	while (!std::getline(inFile, str).eof())
	{
		strings::trim(str); // trim whitespace from the ends

		if (!str.empty())
		{
			if ((str[0] == '#') || (str[0] == ';'))
			{
				comments += str + '\n';	// add the comment to the current comments string
				continue;
			}

			if (str.find('[') != pstring::npos)
			{
				ini_section section(str, comments);
				pstring name = section.getName();
				_sections[name] = section;
				currentSection = _sections.find(name);
				comments = "";
				continue;
			}

			if (str.find('=') != pstring::npos)
			{
				if (currentSection != _sections.end())
				{
					currentSection->second.addValue(str, comments);
					comments = "";
				}
			}
		}
	}
	
	inFile.close();
	return true;
}

const ini_file::sections_t& ini_file::getSections() const
{
	return _sections;
}
