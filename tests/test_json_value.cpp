#include "test_json_value.h"

#include <tests/asserts.h>

#include <util/json_value.h>
using namespace am::util;

namespace am {
namespace tests {

	bool TestJsonValue::testSimple()
	{
		JsonValue value(5);
		JsonValue value2(5);
		JsonValue value3("hello");

		assert(value == value2);
		assert(value2 == value);
		assert(value != value3);
		assert(value2 != value3);
		assert(value3 != value);
		assert(value3 != value2);

		return true;
	}

	bool TestJsonValue::testArrays()
	{
		JsonArray arr1;
		arr1.push_back(5);
		arr1.push_back("hello");
		arr1.push_back(true);
		JsonValue value1(arr1);

		JsonArray arr2;
		arr2.push_back(5);
		arr2.push_back("hello");
		arr2.push_back(true);
		JsonValue value2(arr2);

		assert(value1 == value2);
		assert(value2 == value1);

		JsonArray arr3;
		arr3.push_back("o hai");
		arr3.push_back(arr1);
		JsonValue value3(arr3);

		JsonArray arr4;
		arr4.push_back("o hai");
		arr4.push_back(arr2);
		JsonValue value4(arr4);

		assert(value3 == value4);
		assert(value4 == value3);
		return true;
	}

	bool TestJsonValue::testCastFloat()
	{
		JsonValue val1(5.0f);
		float actual = 0.0f;
		assert(val1.castFloat(actual));
		equalsDelta(5.0f, actual, 0.0001f);

		val1 = 6;
		assert(val1.castFloat(actual));
		equalsDelta(6.0f, actual, 0.0001f);

		val1 = "7.5";
		assert(val1.castFloat(actual));
		equalsDelta(7.5f, actual, 0.0001f);

		val1 = "hello";
		assert(!val1.castFloat(actual));
		equalsDelta(7.5f, actual, 0.0001f);

		return true;
	}
	
}
}
