#include "test_util_data.h"

#include <sstream>
#include <string>

#include <tests/asserts.h>

#include <util/idata.h>
#include <util/data_number.h>
#include <util/data_boolean.h>
#include <util/data_string.h>
#include <util/data_table.h>

#include <lua/lua_state.h>

namespace am {
namespace tests {

	bool TestUtilData::testSimple() {
		
		am::util::data::Table arr;
		arr.push(true);
		arr.push(5.6);

		base::Handle<am::util::data::Table> arr2(new am::util::data::Table());
		am_equals(1, arr2->getReferenceCounter());
		arr2->push(2);
		arr2->push("hello");
		arr.push(arr2);
		am_equals(2, arr2->getReferenceCounter());

		base::Handle<am::util::data::Table> arr2ref(arr.at<am::util::data::Table>(2));
		am_equals(3, arr2->getReferenceCounter());
		am_equals(3, arr2ref->getReferenceCounter());

		assert(arr2ref.get() == arr2.get());
		am_equals(2, arr2ref->at<am::util::data::Number>(0)->integer());
		am_equalsStr("hello", arr2ref->at<am::util::data::String>(1)->value());

		base::Handle<am::util::data::Table> map(new am::util::data::Table());
		map->at("name", "Melli");
		map->at("age", 22);
		map->at("female", true);
		map->at("array", arr2);
		map->comment("Test comment");

		am_equalsStr("Melli", map->at("name")->string());
		am_equals(22, map->at("age")->integer());
		am_equals(true, map->at("female")->boolean());
		base::Handle<am::util::data::Table> mapArr(map->at<am::util::data::Table>("array"));
		assert(mapArr.get());

		std::string testOut = map->toLua();

		return true;
	}

	bool TestUtilData::testFromLua()
	{
		lua::LuaState lua(false);
		bool loadResult = lua.loadString("str = \"test string\"\n"
			"num = 5.8\n"
			"boo = true\n"
			"arr = {5, 3.3, true, \"arr str\", {8, \"x\"}}\n"
			"map = {\n"
			"name = \"melli\",\n"
			"age = 22,\n"
			"pos = {\n"
			"x = 1,\n"
			"y = 5\n"
			"}}");
		if (!loadResult)
		{
			lua.logStack("ERR");
		}
		assert(loadResult);

		{
			lua_getglobal(lua, "str");
			base::Handle<am::util::data::IData> str(am::util::data::IData::fromLua(lua, -1));
			assert(str);
			am_equalsStr("test string", str->string());
			lua.pop(1);
		}

		{
			lua_getglobal(lua, "num");
			base::Handle<am::util::data::IData> num(am::util::data::IData::fromLua(lua, -1));
			assert(num);
			am_equalsDelta(5.8, num->number(), 0.000001);
			lua.pop(1);
		}

		{
			lua_getglobal(lua, "boo");
			base::Handle<am::util::data::IData> boo(am::util::data::IData::fromLua(lua, -1));
			assert(boo);
			am_equals(true, boo->boolean());
			lua.pop(1);
		}

		{
			lua_getglobal(lua, "arr");
			base::Handle<am::util::data::IData> arr(am::util::data::IData::fromLua(lua, -1));
			assert(arr);
			base::Handle<am::util::data::Table> a(dynamic_cast<am::util::data::Table *>(arr.get()));
			assert(a);
			am_equals(5u, a->arrayInner().size());

			base::Handle<am::util::data::Number> num(a->at<am::util::data::Number>(0));
			assert(num);
			am_equals(5, num->integer());

			num = a->at<am::util::data::Number>(1);
			assert(num);
			am_equalsDelta(3.3, num->number(), 0.000001); 

			base::Handle<am::util::data::Boolean> boo(a->at<am::util::data::Boolean>(2));
			assert(boo);
			am_equals(true, boo->boolean());

			base::Handle<am::util::data::String> str(a->at<am::util::data::String>(3));
			assert(str);
			am_equalsStr("arr str", str->string());

			base::Handle<am::util::data::Table> a2(a->at<am::util::data::Table>(4));
			assert(a2);
			am_equals(2u, a2->arrayInner().size());

			num = a2->at<am::util::data::Number>(0);
			assert(num);
			am_equals(8, num->integer());

			str = a2->at<am::util::data::String>(1);
			assert(str);
			am_equalsStr("x", str->string());

			lua.pop(1);
		}

		{
			lua_getglobal(lua, "map");
			base::Handle<am::util::data::IData> map(am::util::data::IData::fromLua(lua, -1));
			assert(map);
			base::Handle<am::util::data::Table> m(dynamic_cast<am::util::data::Table *>(map.get()));
			assert(m);
			am_equals(3u, m->mapInner().size());

			base::Handle<am::util::data::String> str(m->at<am::util::data::String>("name"));
			assert(str);
			am_equalsStr("melli", str->string());

			base::Handle<am::util::data::Number> num(m->at<am::util::data::Number>("age"));
			assert(num);
			am_equals(22, num->integer());

			base::Handle<am::util::data::Table> m2(m->at<am::util::data::Table>("pos"));
			assert(m2);
			am_equals(2u, m2->mapInner().size());

			num = m2->at<am::util::data::Number>("x");
			assert(num);
			am_equals(1, num->integer());

			num = m2->at<am::util::data::Number>("y");
			assert(num);
			am_equals(5, num->integer());

			lua.pop(1);
		}

		return true;
	}

}
}
