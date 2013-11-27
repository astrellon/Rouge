#include "test_lua_data_table.h"

#include <tests/asserts.h>

#include <base/handle.h>

#include <lua/lua_state.h>
using namespace am::lua;

#include <util/data_table.h>
#include <util/data_number.h>
#include <util/data_string.h>
#include <util/data_boolean.h>
using namespace am::util;

#include <lua/wrappers/util/lua_data_table.h>
using namespace am::lua::util;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaDataTable::testSimple() {
		LuaState lua;
		
		bool loadResult = lua.loadString(
			"map = am.data_table.new()\n"
			"map:at(\"num\", 5)\n"
			"map:at(\"str\", \"test\")\n"
			"map:at(\"pos\", {\n"
			"	x = 5.4,\n"
			"	y = 3.2,\n"
			"	origin = false\n"
			"})\n"
			"map:at(\"niltest\", nil)\n"
			"map:at(\"names\", {\"jeb\", \"bob\", \"bill\"})\n"
			"map:at(1, \"one\")\n"
			"map:at(2, \"two\")\n"
			"map:push(\"three\")\n"
			"function getMap()\n"
			"	return map\n"
			"end\n"
			"size = 0\n"
			"check_key = {\n"
			"	\"one\", \"two\", \"three\",\n"
			"	num = 5,\n"
			"	str = \"test\",\n"
			"	pos = map:at(\"pos\"),\n"
			"	names = map:at(\"names\"),\n"
			"	niltest = nil\n"
			"}\n"
			"function loop()\n"
			"	size = 0\n"
			"	map:pairs(function(key, value)\n"
			"		size = size + 1\n"
			"		if (check_key[key] ~= value) then\n"
			"			am_log(\"Did not match: \", key, \" = \", value)\n"
			"			assert(false)\n"
			"		end\n"
			"	end)\n"
			"end\n"
			"local testTable = {\n"
			"	5, 3, 1, \"hello\", "
			"	num = 6, name = \"Melli\", cute = true\n"
			"}\n"
			);
		if (!loadResult)
		{
			lua.logStack("TESTERR");
			assert(loadResult);
		}

		assert(lua.hasGlobalFunction("getMap"));
		lua_acall(lua, 0, 1);

		base::Handle<util::data::Table> dataMap(castUData<util::data::Table>(lua, -1));
		assert(dataMap);

		base::Handle<util::data::Number> num(dataMap->at<util::data::Number>("num"));
		assert(num);
		am_equalsDelta(5.0, num->number(), 0.00001);

		base::Handle<util::data::String> str(dataMap->at<util::data::String>("str"));
		assert(str);
		am_equalsStr("test", str->string());

		base::Handle<util::data::Table> pos(dataMap->at<util::data::Table>("pos"));
		assert(pos);
		am_equals(3u, pos->mapInner().size());

		num = pos->at<util::data::Number>("x");
		assert(num);
		am_equalsDelta(5.4, num->number(), 0.00001);

		num = pos->at<util::data::Number>("y");
		assert(num);
		am_equalsDelta(3.2, num->number(), 0.00001);

		base::Handle<util::data::Boolean> boo(pos->at<util::data::Boolean>("origin"));
		assert(boo);
		assert(!boo->boolean());

		base::Handle<util::data::Table> names(dataMap->at<util::data::Table>("names"));
		assert(names);
		am_equals(3u, names->arrayInner().size());

		str = names->at<util::data::String>(0);
		assert(str);
		am_equalsStr("jeb", str->string());

		str = names->at<util::data::String>(1);
		assert(str);
		am_equalsStr("bob", str->string());

		str = names->at<util::data::String>(2);
		assert(str);
		am_equalsStr("bill", str->string());

		assert(lua.hasGlobalFunction("loop"));
		lua_acall(lua, 0, 0);

		return true;
	}

}
}
