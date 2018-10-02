// factory_test.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "factory_test.h"
#include <nucleon/factory.h>
#include <quark/strings.h>

using namespace quark;
using namespace nucleon;

namespace 
{

////////////////////////////////////
// products
//
struct product : pooled_object
{
	virtual const char* getName() const = 0;
};

struct product1 : product
{ 
	virtual const char* getName() const { return "product1"; }
};

struct product2 : product
{
	virtual const char* getName() const { return "product2"; }
};

struct product3 : product
{
	virtual const char* getName() const { return "product3"; }
};

////////////////////////////////////
// factory
//
class my_static_factory_impl : public static_factory<product, pstring, throw_factory_error>
{
protected:
BEGIN_TYPE_MAP()
	TYPE_ENTRY("product1", product1)
	TYPE_ENTRY("product2", product2)
	TYPE_ENTRY("product3", product3)
END_TYPE_MAP()
};

typedef singleton_holder
<
	my_static_factory_impl,
	create_static,
	default_lifetime,
	class_level_lockable<my_static_factory_impl>
> my_static_factory;

} // namespace

//////////////////////////////////////////////////////////////////////
// factory_test
//////////////////////////////////////////////////////////////////////

const char* factory_test::getName() const
{
	return "factory_test";
}

void factory_test::execute()
{
	testStaticFactory();
}

void factory_test::testStaticFactory()
{
	// successfull test
	for (size_t i = 0; i < 3; ++i)
	{
		const pstring name = strings::format("product%d", i + 1);
		pauto_ptr<product> spProduct( my_static_factory::instance().createObject(name) );
		_TEST_ASSERT(spProduct->getName() == name);
	}
	
	// failure test
	_TEST_ASSERT_EXCEPTION(my_static_factory::instance().createObject("product4"), pruntime_error);
}
