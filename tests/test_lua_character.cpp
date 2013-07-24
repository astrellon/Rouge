#include "test_lua_character.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <lua/wrappers/game/lua_character.h>
#include <lua/wrappers/game/lua_body_part.h>
using namespace am::lua::game;

#include <game/character.h>
#include <game/body_part.h>
#include <game/engine.h>
#include <game/game.h>
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

		Engine::getEngine()->setCurrentGame(new Game());

		int loadResult = lua.loadString(
			"char = am.character.new(\"char1\")\n"
			"char:name(\"Melli\"):age(23.4)\n"
			"function changeName(name)\n"
			"	char:name(name)\n"
			"end\n"
			"function getName()\n"
			"	return char:name()\n"
			"end\n"
			"function getAge()\n"
			"	return char:age()\n"
			"end");
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		Handle<Character> char1 = dynamic_cast<Character *>(Engine::getEngine()->getGameObject("char1"));
		am_equalsStr("Melli", char1->getName().c_str());

		assert(lua.hasGlobalFunction("getName"));
		lua_acall(lua, 0, 1);
		const char *name = lua_tostring(lua, -1);
		am_equalsStr("Melli", name);
		lua.pop(1);
		assert(lua.hasGlobalFunction("getAge"));
		lua_acall(lua, 0, 1);
		am_equalsDelta(23.4f, lua_tonumber(lua, -1), 0.001f);
		lua.pop(1);

		char1->setName("Melanie");
		am_equalsStr("Melanie", char1->getName().c_str());
		assert(lua.hasGlobalFunction("getName"));
		lua_acall(lua, 0, 1);
		name = lua_tostring(lua, -1);
		am_equalsStr("Melanie", name);
		lua.pop(1);
		assert(lua.hasGlobalFunction("getAge"));
		lua_acall(lua, 0, 1);
		am_equalsDelta(23.4f, lua_tonumber(lua, -1), 0.001f);
		lua.pop(1);

		assert(lua.hasGlobalFunction("changeName"));
		lua.push("Mel");
		lua_acall(lua, 1, 0);
		char1->setAge(56.8f);

		am_equalsStr("Mel", char1->getName().c_str());
		assert(lua.hasGlobalFunction("getName"));
		lua_acall(lua, 0, 1);
		name = lua_tostring(lua, -1);
		am_equalsStr("Mel", name);
		lua.pop(1);
		assert(lua.hasGlobalFunction("getAge"));
		lua_acall(lua, 0, 1);
		am_equalsDelta(56.8f, lua_tonumber(lua, -1), 0.001f);
		lua.pop(1);

		return true;
	}

	bool TestLuaCharacter::testGender() {
		LuaState lua;

		int loadResult = lua.loadString(
			"char = am.character.new(\"charGender\")\n"
			"char:gender(\"male\")\n"
			"function changeGender(gender)\n"
			"	char:gender(gender)\n"
			"end\n"
			"function getGender()\n"
			"	return char:gender()\n"
			"end");
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		Handle<Character> charGender = dynamic_cast<Character *>(Engine::getEngine()->getGameObject("charGender"));
		am_equals(static_cast<int>(Gender::MALE), static_cast<int>(charGender->getGender()));

		assert(lua.hasGlobalFunction("getGender"));
		lua_acall(lua, 0, 1);
		const char *genderName = lua_tostring(lua, -1);
		am_equalsStr("male", genderName);
		lua.pop(1);

		assert(lua.hasGlobalFunction("changeGender"));
		lua.push("female");
		lua_acall(lua, 1, 0);
		am_equals(static_cast<int>(Gender::FEMALE), static_cast<int>(charGender->getGender()));

		assert(lua.hasGlobalFunction("getGender"));
		lua_acall(lua, 0, 1);
		genderName = lua_tostring(lua, -1);
		am_equalsStr("female", genderName);
		lua.pop(1);

		return true;
	}

	bool TestLuaCharacter::testStats()
	{
		LuaState lua;

		int loadResult = lua.loadString(
			"char = am.character.new(\"charStats\")\n"
			"function getBaseStat(stat)\n"
			"	return char:stats():base_stat(stat)\n"
			"end\n"
			"function setBaseStat(stat, value)\n"
			"	char:stats():base_stat(stat, value)\n"
			"end\n");
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		Handle<Character> charStats = dynamic_cast<Character *>(Engine::getEngine()->getGameObject("charStats"));
		am_equalsDelta(0.0f, charStats->getStats()->getBaseStat(Stat::ARCANE), 0.0001f);

		charStats->getStats()->setBaseStat(Stat::ARCANE, 5.0f);

		assert(lua.hasGlobalFunction("getBaseStat"));
		lua.push("arcane");
		lua_acall(lua, 1, 1);
		float value = static_cast<float>(lua_tonumber(lua, -1));
		lua.pop(1);
		am_equalsDelta(5.0f, value, 0.0001f);

		assert(lua.hasGlobalFunction("setBaseStat"));
		lua.push("arcane");
		lua.push(8.9f);
		lua_acall(lua, 2, 0);

		am_equalsDelta(8.9f, charStats->getStats()->getBaseStat(Stat::ARCANE), 0.0001f);

		return true;
	}

	bool TestLuaCharacter::testBodyParts()
	{
		LuaState lua;

		int loadResult = lua.loadString(
			"char = am.character.new(\"charBodyPart\")\n"
			"function addBodyPart(part)\n"
			"	return char:add_body_part(part)\n"
			"end\n"
			"function removeBodyPart(part)\n"
			"	return char:remove_body_part(part)\n"
			"end\n"
			"function hasBodyPart(part)\n"
			"	return char:has_body_part(part)\n"
			"end\n"
			);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		Handle<Character> charBodyPart = dynamic_cast<Character *>(Engine::getEngine()->getGameObject("charBodyPart"));

		assert(lua.hasGlobalFunction("hasBodyPart"));
		lua.push("arm");
		lua_acall(lua, 1, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("addBodyPart"));
		Handle<BodyPart> part(new BodyPart("arm"));
		wrapRefObject<BodyPart>(lua, part);
		lua_acall(lua, 1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(!charBodyPart->hasBodyPart(new BodyPart("arm")));
		assert(charBodyPart->hasBodyPart(part));
		assert(charBodyPart->hasBodyPart("arm"));

		assert(lua.hasGlobalFunction("hasBodyPart"));
		lua.push("arm");
		lua_acall(lua, 1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("hasBodyPart"));
		wrapRefObject<BodyPart>(lua, part);
		lua_acall(lua, 1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		return true;
	}

	bool TestLuaCharacter::testAttrs()
	{
		LuaState lua;

		int loadResult = lua.loadString(
			"char = am.character.new()\n"
			"attrs = char:attrs()\n"
			"am_equals(attrs, nil)\n"
			"attrs = char:attrs(true)\n"
			"not_equals(attrs, nil)\n"
			"attrs:push(2)\n"
			"attrs:push(5)\n"
			"attrs:push(8)\n"
			"attrs2 = char:attrs(true)\n"
			"am_equals(attrs, attrs2)\n"
			);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		return true;
	}
}
}