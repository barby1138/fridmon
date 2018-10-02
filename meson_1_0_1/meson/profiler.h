// profiler.h
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

#ifndef _MESON_PROFILER_PUBLIC_H_
#define _MESON_PROFILER_PUBLIC_H_

#include <quark/singleton.h>
#include <meson/profiler/profiler.h>

namespace meson
{

typedef quark::singleton_holder
<
	profiler<simple_meter>,
	quark::create_static,
	quark::default_lifetime,
	quark::class_level_lockable<profiler<simple_meter> >
> profiler_low;

typedef quark::singleton_holder
<
	profiler<hires_meter>,
	quark::create_static,
	quark::default_lifetime,
	quark::class_level_lockable<profiler<hires_meter> >
> profiler_hi;

typedef bench_scope<profiler_low> bench_scope_low;
typedef bench_scope<profiler_hi> bench_scope_hi;

} // namespace meson

#endif // _MESON_PROFILER_PUBLIC_H_
