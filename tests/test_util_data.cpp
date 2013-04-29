#include "test_util_data.h"

#include <sstream>
#include <string>

#include <tests/asserts.h>

#include <util/idata.h>
#include <util/data_number.h>
#include <util/data_boolean.h>
#include <util/data_string.h>
#include <util/data_table.h>
using namespace am::util::data;

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace tests {

	bool TestUtilData::testSimple() {
		
		util::data::Table arr;
		arr.push(true);
		arr.push(5.6);

		Handle<util::data::Table> arr2(new util::data::Table());
		equals(1, arr2->getReferenceCounter());
		arr2->push(2);
		arr2->push("hello");
		arr.push(arr2);
		equals(2, arr2->getReferenceCounter());

		Handle<util::data::Table> arr2ref(arr.at<util::data::Table>(2));
		equals(3, arr2->getReferenceCounter());
		equals(3, arr2ref->getReferenceCounter());

		assert(arr2ref.get() == arr2.get());
		equals(2, arr2ref->at<Number>(0)->integer());
		equalsStr("hello", arr2ref->at<String>(1)->value());

		Handle<util::data::Table> map(new util::data::Table());
		map->push("name", "Melli");
		map->push("age", 22);
		map->push("female", true);
		map->push("array", arr2);
		map->comment("Test comment");

		equalsStr("Melli", map->at("name")->string());
		equals(22, map->at("age")->integer());
		equals(true, map->at("female")->boolean());
		Handle<util::data::Table> mapArr(map->at<util::data::Table>("array"));
		assert(mapArr.get());

		std::string testOut = map->toLua();

		return true;
	}

	bool TestUtilData::testFromLua()
	{
		LuaState lua(false);
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
			Handle<IData> str(IData::fromLua(lua, -1));
			assert(str);
			equalsStr("test string", str->string());
			lua.pop(1);
		}

		{
			lua_getglobal(lua, "num");
			Handle<IData> num(IData::fromLua(lua, -1));
			assert(num);
			equalsDelta(5.8, num->number(), 0.000001);
			lua.pop(1);
		}

		{
			lua_getglobal(lua, "boo");
			Handle<IData> boo(IData::fromLua(lua, -1));
			assert(boo);
			equals(true, boo->boolean());
			lua.pop(1);
		}

		{
			lua_getglobal(lua, "arr");
			Handle<IData> arr(IData::fromLua(lua, -1));
			assert(arr);
			Handle<util::data::Table> a(dynamic_cast<util::data::Table *>(arr.get()));
			assert(a);
			equals(5u, a->arrayInner().size());

			Handle<Number> num(a->at<Number>(0));
			assert(num);
			equals(5, num->integer());

			num = a->at<Number>(1);
			assert(num);
			equalsDelta(3.3, num->number(), 0.000001); 

			Handle<Boolean> boo(a->at<Boolean>(2));
			assert(boo);
			equals(true, boo->boolean());

			Handle<String> str(a->at<String>(3));
			assert(str);
			equalsStr("arr str", str->string());

			Handle<util::data::Table> a2(a->at<util::data::Table>(4));
			assert(a2);
			equals(2u, a2->arrayInner().size());

			num = a2->at<Number>(0);
			assert(num);
			equals(8, num->integer());

			str = a2->at<String>(1);
			assert(str);
			equalsStr("x", str->string());

			lua.pop(1);
		}

		{
			lua_getglobal(lua, "map");
			Handle<IData> map(IData::fromLua(lua, -1));
			assert(map);
			Handle<util::data::Table> m(dynamic_cast<util::data::Table *>(map.get()));
			assert(m);
			equals(3u, m->mapInner().size());

			Handle<String> str(m->at<String>("name"));
			assert(str);
			equalsStr("melli", str->string());

			Handle<Number> num(m->at<Number>("age"));
			assert(num);
			equals(22, num->integer());

			Handle<util::data::Table> m2(m->at<util::data::Table>("pos"));
			assert(m2);
			equals(2u, m2->mapInner().size());

			num = m2->at<Number>("x");
			assert(num);
			equals(1, num->integer());

			num = m2->at<Number>("y");
			assert(num);
			equals(5, num->integer());

			lua.pop(1);
		}

		return true;
	}

}
}