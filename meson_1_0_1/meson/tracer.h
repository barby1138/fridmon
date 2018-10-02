// tracer.h
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

#ifndef _MESON_TRACER_PUBLIC_H_
#define _MESON_TRACER_PUBLIC_H_

#include <meson/tracer/tracer.h>
#include <meson/tracer/user_tracer.h>
#include <quark/singleton.h>

namespace meson
{
	
typedef quark::singleton_holder
<	
	tracer<>, 
	quark::create_static, 
	quark::default_lifetime, 
	quark::class_level_lockable<tracer<> >
> default_tracer;

} // namespace meson

#endif // _MESON_TRACER_PUBLIC_H_
