// path.h
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

#ifndef _MESON_PATH_H_
#define _MESON_PATH_H_

#include <quark/config.h>

// TODO: move to a new file library

namespace meson
{

///////////////////////////////////////////////////////////
// class path
//
class path
{
public:
	static const quark::pstring getFileName(const char* path);
	static const quark::pstring getDirectory(const char* path);
	static const quark::pstring getExtension(const char* path);
	static const quark::pstring getDrive(const char* path);
	static bool makeDir(const char* path);
	static char getDriveSeparator();
	static char getDirSeparator();

private:
	path();
	~path();
};

} // namespace meson

#endif // _MESON_PATH_H_
