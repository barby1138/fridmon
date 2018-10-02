// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <quark/config.h>
#include <quark/thread.h>
#include <quark/singleton.h>
#include <quark/time.h>
#include <quark/msg_queue.h>

#include <hyperon/gen_attributes.h>
#include <hyperon/element.h>
#include <hyperon/properties.h>
#include <hyperon/storage/expat_storage.h>

#include <nucleon/dispatcher.h>
#include <nucleon/type_list.h>

#include <meson/tracer.h>

using namespace quark;
using namespace hyperon;
using namespace nucleon;
using namespace meson;

// TODO: reference additional headers your program requires here
