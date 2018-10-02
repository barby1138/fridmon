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

using namespace quark;

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

namespace meson
{

static const char extSeparator = '.';
static const char dirSeparator = '/';
static const char driveSeparator = ':';

const pstring path::getFileName(const char* path)
{
	assert(path);

	pstring spath(path);

	pstring::size_type nNameIndex = spath.rfind(dirSeparator);
	if (nNameIndex == pstring::npos)
	{
		nNameIndex = 0;
	}
	else
	{
		++nNameIndex;
	}

	pstring::size_type nExtIndex = spath.find(extSeparator, nNameIndex);
	if (nExtIndex == pstring::npos)
	{
		nExtIndex = spath.length();
	}

	return spath.substr(nNameIndex, nExtIndex - nNameIndex);
}

const pstring path::getDirectory(const char* path)
{
	assert(path);

	pstring spath(path);

	pstring::size_type nNameIndex = spath.rfind(dirSeparator);
	if (nNameIndex == pstring::npos)
	{
		nNameIndex = 0;
	}

	return spath.substr(0, nNameIndex);
}

const pstring path::getExtension(const char* path)
{
	assert(path);

	pstring spath(path);

	pstring::size_type nExtIndex = spath.rfind(extSeparator);
	if (nExtIndex != pstring::npos)
	{
		++nExtIndex;
	}

	return spath.substr(nExtIndex);
}

const pstring path::getDrive(const char* path)
{
	assert(path);

	pstring spath(path);

	pstring::size_type nDriveIndex = spath.find(driveSeparator);
	if (nDriveIndex == pstring::npos)
	{
		nDriveIndex = 0;
	}

	return spath.substr(0, nDriveIndex);
}

bool path::makeDir(const char* path)
{
	assert(path);
	
	// remove last separator
	pstring strPath = path;
	pstring::size_type pos = strPath.find_last_not_of(dirSeparator);
	if (pos != pstring::npos)
		strPath.erase(pos + 1);
	
	pstring	strSubpath = getDirectory(strPath.c_str());
	if (strSubpath == strPath)
		return false;
	
	// recursion call, ignore result
	makeDir(strSubpath.c_str());
	
	// create directory
	int res = mkdir(strPath.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH );
	if ((res != 0) && (errno == EEXIST))
	{
		res = 0;
	}

	return (res == 0);
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
