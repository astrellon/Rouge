#include "test_lua_map.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/tile.h>
#include <game/map.h>
#include <game/tile_instance.h>
using namespace am::game;

#include <lua/wrappers/lua_map.h>
using namespace am::lua::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaMap::testSimple() {
		LuaState lua;
		
		int loadResult = lua.loadString("Map = import(\"Map\")\n"
			"Engine = import(\"Engine\")\n"
			"local result = loadfile(\"data\\\\tileTypes2.lua\")\n"
			"if (result == nil) then\n"
			"	am_log(\"Error loading\")\n"
			"else\n"
			"	am_log(\"No error\")\n"
			"end\n"
			"result()\n"

			"map = Map.new(\"testMap\", 6, 7)\n"
			"map:set_full_name(\"The Region of Test\")\n"

			"Engine.clear_using_tile_set()\n"
			"Engine.using_tile_set(\"nature\")\n"

			"map:set_tiles({\n"
			"	\"brick:0\", \"brick:2\", \"grass:0\", \"grass:2\", \"grass:3\", \"dirt:3\",\n"
			"	\"brick:1\", \"brick:0\", \"grass:1\", \"dirt:0\", \"dirt:2\", \"grass:1\",\n"
			"	\"brick:0\", \"brick:2\", \"grass:2\", \"dirt:3\", \"dirt:0\", \"grass:3\",\n"
			"	\"brick:2\", \"brick:1\", \"grass:3\", \"dirt:2\", \"dirt:1\", \"swamp\",\n"
			"	\"brick:1\", \"brick:0\", \"grass:1\", \"grass:0\", \"swamp\", \"water\",\n"
			"	\"brick:2\", \"brick:2\", \"grass:2\", \"grass:3\", \"water\", \"water\",\n"
			"	\"brick:2\", \"brick:2\", \"grass:0\", \"swamp\", \"water\", \"water\",\n"
			"	\"brick:2\", \"brick:2\", \"grass:3\", \"swamp\", \"water\", \"water\"\n"
			"})\n"
			"function getMap()\n"
			"	return map\n"
			"end\n"
			);

		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		assert(lua.hasGlobalFunction("getMap"));
		lua.call(0, 1);
		Map *map = Check_Map(lua, -1);
		assert(map != NULL);

		equals(6, map->getMapWidth());
		equals(7, map->getMapHeight());

		equalsStr("The Region of Test", map->getFullName().c_str());

		Tile *tile = map->getTile(0, 0);
		assert(tile);
		equalsStr("brick", tile->getName().c_str());
		equals(0, map->getTileInstance(0, 0)->getTileFrame());

		tile = map->getTile(3, 0);
		assert(tile);
		equalsStr("grass", tile->getName().c_str());
		equals(2, map->getTileInstance(3, 0)->getTileFrame());

		tile = map->getTile(4, 4);
		assert(tile);
		equalsStr("swamp", tile->getName().c_str());
		equals(0, map->getTileInstance(4, 4)->getTileFrame());

		tile = map->getTile(5, 6);
		assert(tile);
		equalsStr("water", tile->getName().c_str());
		equals(0, map->getTileInstance(5, 6)->getTileFrame());

		return true;
	}

}
}