#include "test_selector_value.h"

#include <tests/asserts.h>

#include <util/selector_value.h>
using namespace am::util;

namespace am {
namespace tests {

	bool TestSelectorValue::testSimple() {
		SelectorValue styleValue;
		styleValue.setType(SelectorValue::EQUAL);
		styleValue.setValue(5.0f);

		assert(styleValue.match(5.0f));

		styleValue.setValue(6.0f);
		assert(!styleValue.match(5.0f));

		styleValue.setType(SelectorValue::LESS_THAN);
		assert(styleValue.match(5.0f));

		styleValue.setType(SelectorValue::GREATER_THAN);
		assert(!styleValue.match(5.0f));

		styleValue.setValue(4.0f);
		assert(styleValue.match(5.0f));

		styleValue.setType(SelectorValue::GREATER_THAN_EQUAL);
		styleValue.setValue(5.0f);
		assert(styleValue.match(5.0f));

		styleValue.setType(SelectorValue::GREATER_THAN);
		assert(!styleValue.match(5.0f));

		styleValue.setType(SelectorValue::LESS_THAN);
		assert(!styleValue.match(5.0f));

		styleValue.setType(SelectorValue::LESS_THAN_EQUAL);
		assert(styleValue.match(5.0f));

		styleValue.setValue("hello");
		styleValue.setType(SelectorValue::EQUAL);
		assert(styleValue.match("hello"));

		styleValue.setType(SelectorValue::LESS_THAN);
		assert(!styleValue.match("aaa"));
		assert(styleValue.match("zzz"));

		styleValue.setType(SelectorValue::GREATER_THAN);
		assert(styleValue.match("aaa"));
		assert(!styleValue.match("zzz"));

		JsonArray arr;
		arr.push_back(4.0f);
		arr.push_back(5.0f);
		arr.push_back(6.0f);
		styleValue.setType(SelectorValue::OR);
		styleValue.setValue(arr);

		assert(!styleValue.match(3.5f));
		assert(styleValue.match(4.0f));
		assert(!styleValue.match(4.5f));
		assert(styleValue.match(5.0f));
		assert(!styleValue.match(5.5f));
		assert(styleValue.match(6.0f));
		assert(!styleValue.match(6.5f));
		assert(!styleValue.match("hello"));

		arr.push_back("hello");
		styleValue.setValue(arr);

		assert(!styleValue.match(3.5f));
		assert(styleValue.match(4.0f));
		assert(!styleValue.match(4.5f));
		assert(styleValue.match(5.0f));
		assert(!styleValue.match(5.5f));
		assert(styleValue.match(6.0f));
		assert(!styleValue.match(6.5f));
		assert(styleValue.match("hello"));

		return true;
	}

}
}
