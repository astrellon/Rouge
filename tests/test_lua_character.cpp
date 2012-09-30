#include "test_lua_character.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <lua/wrappers/lua_character.h>
using namespace am::lua::game;

#include <game/character.h>
using namespace am::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaCharacter::testSimple() {
		LuaState lua;

		int loadResult = lua.loadString("Character = import(\"Character\")\n"
			"char = Character.new(\"char1\")\n"
			"char:set_name(\"Melli\")\n"
			"char:set_age(23.4)\n"
			"function changeName(name)\n"
			"	char:set_name(name)\n"
			"end\n"
			"function getName()\n"
			"	return char:get_name()\n"
			"end\n"
			"function getAge()\n"
			"	return char:get_age()\n"
			"end");
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		Handle<Character> char1 = dynamic_cast<Character *>(Character::getByGameId("char1"));
		assert(char1->getName().compare("Melli") == 0);

		assert(lua.hasGlobalFunction("getName"));
		lua.call(0, 1);
		const char *name = lua_tostring(lua, -1);
		assert(strcmp(name, "Melli") == 0);
		lua.pop(1);
		assert(lua.hasGlobalFunction("getAge"));
		lua.call(0, 1);
		equalsDelta(23.4f, lua_tonumber(lua, -1), 0.001f);
		lua.pop(1);

		char1->setName("Melanie");
		assert(char1->getName().compare("Melanie") == 0);
		assert(lua.hasGlobalFunction("getName"));
		lua.call(0, 1);
		name = lua_tostring(lua, -1);
		assert(strcmp(name, "Melanie") == 0);
		lua.pop(1);
		assert(lua.hasGlobalFunction("getAge"));
		lua.call(0, 1);
		equalsDelta(23.4f, lua_tonumber(lua, -1), 0.001f);
		lua.pop(1);

		assert(lua.hasGlobalFunction("changeName"));
		lua.push("Mel");
		lua.call(1, 0);
		char1->setAge(56.8f);

		assert(char1->getName().compare("Mel") == 0);
		assert(lua.hasGlobalFunction("getName"));
		lua.call(0, 1);
		name = lua_tostring(lua, -1);
		assert(strcmp(name, "Mel") == 0);
		lua.pop(1);
		assert(lua.hasGlobalFunction("getAge"));
		lua.call(0, 1);
		equalsDelta(56.8f, lua_tonumber(lua, -1), 0.001f);
		lua.pop(1);

		return true;
	}

	bool TestLuaCharacter::testGender() {
		LuaState lua;

		int loadResult = lua.loadString("Character = import(\"Character\")\n"
			"char = Character.new(\"charGender\")\n"
			"char:set_gender(\"male\")\n"
			"function changeGender(gender)\n"
			"	char:set_gender(gender)\n"
			"end\n"
			"function getGender()\n"
			"	return char:get_gender()\n"
			"end");
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		Handle<Character> charGender = dynamic_cast<Character *>(Character::getByGameId("charGender"));
		equals(static_cast<int>(Gender::MALE), static_cast<int>(charGender->getGender()));

		assert(lua.hasGlobalFunction("getGender"));
		lua.call(0, 1);
		const char *genderName = lua_tostring(lua, -1);
		assert(strcmp("male", genderName) == 0);
		lua.pop(1);

		assert(lua.hasGlobalFunction("changeGender"));
		lua.push("female");
		lua.call(1, 0);
		equals(static_cast<int>(Gender::FEMALE), static_cast<int>(charGender->getGender()));

		assert(lua.hasGlobalFunction("getGender"));
		lua.call(0, 1);
		genderName = lua_tostring(lua, -1);
		assert(strcmp("female", genderName) == 0);
		lua.pop(1);

		return true;
	}

	bool TestLuaCharacter::testStats()
	{
		LuaState lua;

		int loadResult = lua.loadString("Character = import(\"Character\")\n"
			"Stats = import(\"Stats\")\n"
			"char = Character.new(\"charStats\")\n"
			"function getBaseStat(stat)\n"
			"	return char:get_stats():get_base_stat(stat)\n"
			"end\n"
			"function setBaseStat(stat, value)\n"
			"	char:get_stats():set_base_stat(stat, value)\n"
			"end\n");
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		Handle<Character> charStats = dynamic_cast<Character *>(Character::getByGameId("charStats"));
		equalsDelta(0.0f, charStats->getStats().getBaseStat(Stat::ARCANE), 0.0001f);

		charStats->getStats().setBaseStat(Stat::ARCANE, 5.0f);

		assert(lua.hasGlobalFunction("getBaseStat"));
		lua.push("arcane");
		lua.call(1, 1);
		float value = lua_tonumber(lua, -1);
		lua.pop(1);
		equalsDelta(5.0f, value, 0.0001f);

		assert(lua.hasGlobalFunction("setBaseStat"));
		lua.push("arcane");
		lua.push(8.9f);
		lua.call(2, 0);

		equalsDelta(8.9f, charStats->getStats().getBaseStat(Stat::ARCANE), 0.0001f);

		return true;
	}
}
}