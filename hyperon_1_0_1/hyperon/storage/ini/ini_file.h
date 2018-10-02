// ini_file.h: interface for the ini_file class.
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

#ifndef _HYPERON_INI_FILE_H_
#define _HYPERON_INI_FILE_H_

#include <quark/config.h>

namespace hyperon
{

//////////////////////////////////////////////////
// class ini_section_exception
//
class ini_section_exception : public quark::pruntime_error
{
public:
	ini_section_exception(const char* name, const char* descr);
};

//////////////////////////////////////////////////
// class ini_record  
//
//! INI File Value
/*!
*/
class ini_record
{
public:
	ini_record(const quark::pstring& inValue = "", const quark::pstring& inComments = "")
		:	value(inValue),
			comments(inComments)
	{
	}

	quark::pstring comments;
	quark::pstring value;
};

//////////////////////////////////////////////////
// class ini_section  
//
//! INI File Section
/*!
*/
class ini_section
{
public:
	typedef quark::pmap<quark::pstring, ini_record> records_t;

public:
	ini_section();
	ini_section(const quark::pstring& header, const quark::pstring& comments);

	const char* getName() const { return _name.c_str(); }

	// TODO: redesign
	void addValue(const quark::pstring& record, const quark::pstring& comments);

	const quark::pstring getValue(const quark::pstring& key) const;

	const records_t& getRecords() const;

private:
	records_t _records;
	quark::pstring _name;
	quark::pstring _comments;
};

//////////////////////////////////////////////////
// class ini_file  
//
//! INI File Parser
/*!
	Parses INI file and provides access to sections and values
*/
class ini_file  
{
public:
	typedef quark::pmap<quark::pstring, ini_section> sections_t;

public:
	ini_file();
	~ini_file();

	bool load(const quark::pstring& file);

	const sections_t& getSections() const;

	// TODO: add needed accessors

private:
	sections_t _sections;
	quark::pstring _fileName;
};

} // namespace hyperon

#endif // _HYPERON_INI_FILE_H_
