#include <iostream>

#include "KeyValueMap.h"

int main(int argc, char ** argv)
{
	KeyValueMap testMap;
	testMap.set("hello", 3l);

	bool test1 = testMap.has_type<long int>("hello");
	bool test2 = testMap.has_type<int>("hello");

	int a = test1 + test2;

	auto test_value1 = testMap.get_if<long int>("hello");
	auto test_value2 = testMap.get_if<char>("hello");

	KeyValueMap testMap2;
	testMap.set("level2", testMap2);

	int b = test1 + test2;
}