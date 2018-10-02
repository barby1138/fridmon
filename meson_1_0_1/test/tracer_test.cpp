// tracer_test.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "tracer_test.h"
#include <meson/tracer.h>

using namespace meson;

#ifdef __VX_WORKS__
typedef tracer
<
	thread_formatter<>,
	console_writer 
> tracer_impl;
#else
typedef tracer
<
	thread_formatter<>,
	combined_writer<console_writer, file_writer> 
> tracer_impl;
#endif 

typedef quark::singleton_holder
<	
	tracer_impl,
	quark::create_static, 
	quark::default_lifetime, 
	quark::class_level_lockable<tracer_impl>
> custom_tracer;

static bool bound = bindTracer<custom_tracer>();

//////////////////////////////////////////////////////////////////////
// tracer_test
//////////////////////////////////////////////////////////////////////

const char* tracer_test::getName() const
{
	return "tracer_test";
}

void tracer_test::execute()
{
	_TEST_ASSERT(bound);

#ifndef __VX_WORKS__
	custom_tracer::instance().setFile("tracer_test.log");
#endif
	//custom_tracer::instance().setMask(tlNone);

	for (size_t i = 0; i < 100; ++i)
	{
		{
			TRACE_DEBUG("debug message: %d", i);
			{
				TRACE_INDENT(".");
				TRACE_INFO("info message: %d", i);
				{
					TRACE_INDENT("..");
					TRACE_WARNING("warning message: %d", i);
					{
						TRACE_INDENT("...");
						TRACE_ERROR("error message: %d", i);
					}
				}
			}
		}	
	}
}
