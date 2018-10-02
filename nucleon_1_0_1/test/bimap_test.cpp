// bimap_test.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "bimap_test.h"
#include <nucleon/static_bimap.h>
#include <quark/singleton.h>

using namespace quark;
using namespace nucleon;

namespace 
{

enum record_t
{
	record1 = 0,
	record2,
	record3,
	record4
};

////////////////////////////////////
// host
//
class my_static_bimap_impl : public static_bimap<record_t, pstring, my_static_bimap_impl>
{
private:
BEGIN_BIMAP()
	BIMAP_ENTRY(record1, "record1")
	BIMAP_ENTRY(record2, "record2")
	BIMAP_ENTRY(record3, "record3")
END_BIMAP()
};

typedef singleton_holder<my_static_bimap_impl> my_static_bimap;

} // namespace

//////////////////////////////////////////////////////////////////////
// bimap_test
//////////////////////////////////////////////////////////////////////

const char* bimap_test::getName() const
{
	return "bimap_test";
}

void bimap_test::execute()
{
	testStaticBimap();
}

void bimap_test::testStaticBimap()
{
	const my_static_bimap_impl& bimap = my_static_bimap::instance();

	_TEST_ASSERT(bimap.size() == 3);

	// check "right"
	_TEST_ASSERT(bimap.right(record1) == "record1");
	_TEST_ASSERT(bimap.right(record2) == "record2");
	_TEST_ASSERT(bimap.right(record3) == "record3");
	_TEST_ASSERT_EXCEPTION(bimap.right(record4), std::logic_error);

	// check "left"
	_TEST_ASSERT(bimap.left("record1") == record1);
	_TEST_ASSERT(bimap.left("record2") == record2);
	_TEST_ASSERT(bimap.left("record3") == record3);
	_TEST_ASSERT_EXCEPTION(bimap.left("record4"), std::logic_error);

	// check enumerator
	pvector<record_t> records;
	records.reserve(bimap.size());
	bimap.enumerate(records);
	_TEST_ASSERT(records.size() == 3);
	_TEST_ASSERT(records[0] == record1);
	_TEST_ASSERT(records[1] == record2);
	_TEST_ASSERT(records[2] == record3);
}
