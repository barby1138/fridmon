// test.cpp : Defines the entry point for the console application.
//

#include "attribute.h"
#include "element.h"
#include "properties.h"
#include "xml_storage.h"
#include "ini_storage.h"

int main(int argc, char* argv[])
{
	test_suite suite;
	suite.registerTest(new attribute_test);
	suite.registerTest(new element_test);
	suite.registerTest(new properties_test);
	suite.registerTest(new xml_storage_test);
	suite.registerTest(new ini_storage_test);

	suite.execute();

	return 0;
}
