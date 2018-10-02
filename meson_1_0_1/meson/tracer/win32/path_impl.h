// path_impl.h
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Meson Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Copyright (c) 2006, InterObject Ltd
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _MESON_PATH_IMPL_H_
#define _MESON_PATH_IMPL_H_

#include <windows.h>

// TODO: implement platfrom independent parse functions

using namespace quark;

namespace meson
{

static const char dirSeparator = '\\';
static const char driveSeparator = ':';

const pstring path::getFileName(const char* path)
{
	assert(path);
	char file[_MAX_FNAME];
	::_splitpath(path, NULL, NULL, file, NULL);
	return file;
}

const pstring path::getDirectory(const char* path)
{
	assert(path);
	char drive[_MAX_DRIVE], dir[_MAX_DIR];
	::_splitpath(path, drive, dir, NULL, NULL);
	char res[_MAX_PATH];
	::_makepath(res, drive, dir, NULL, NULL);
	return res;
}

const pstring path::getExtension(const char* path)
{
	assert(path);
	char ext[_MAX_EXT];
	::_splitpath(path, NULL, NULL, NULL, ext);
	return ext;
}

const pstring path::getDrive(const char* path)
{
	assert(path);
	char drive[_MAX_DRIVE];
	::_splitpath(path, drive, NULL, NULL, NULL);
	return drive;
}

bool path::makeDir(const char* path)
{
	assert(path);
	
	// remove last separator
	pstring strPath = path;
	pstring::size_type pos = strPath.find_last_not_of("\\/");
	if (pos != pstring::npos)
		strPath.erase(pos + 1);
	
	pstring	strSubpath = getDirectory(strPath.c_str());
	if (strSubpath == strPath)
		return false;
	
	// recursion call, ignore result
	makeDir(strSubpath.c_str());
	
	// create directory
	BOOL res = ::CreateDirectory(strPath.c_str(), NULL);
	if (!res)
	{
		if (::GetLastError() == ERROR_ALREADY_EXISTS)
			res = TRUE;
	}
	return res ? true : false;
}

char path::getDirSeparator()
{
	return dirSeparator;
}

char path::getDriveSeparator()
{
	return driveSeparator;
}

} // namespace meson

#endif // _MESON_PATH_IMPL_H_
