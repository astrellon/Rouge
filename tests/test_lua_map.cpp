#include "test_lua_map.h"

#include <tests/asserts.h>

#include <base/handle.h>

#include <lua/lua_state.h>

#include <game/tile.h>
#include <game/map.h>
#include <game/tile_instance.h>

#include <lua/wrappers/game/lua_map.h>

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaMap::testSimple() 
	{
		lua::LuaState lua;
		
		int loadResult = lua.loadString(
			"local result, msg = loadfile(\"data/tileTypes.lua\")\n"
			"if (result == nil) then\n"
			"	am.debug.log(\"Error loading: \" .. msg)\n"
			"else\n"
			"	am.debug.log(\"No error\")\n"
			"end\n"
			"result()\n"

			"map = am.map.new(\"testMap\", 6, 7)\n"
			"map:full_name(\"The Region of Test\")\n"

			"am.engine.clear_using_tile_set()\n"
			"am.engine.using_tile_set(\"nature:nature\")\n"

			"map:tiles({\n"
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
		lua_acall(lua, 0, 1);
		game::Map *map = lua::castUData<game::Map>(lua, -1);
		assert(map != nullptr);

		am_equals(6, map->getMapWidth());
		am_equals(7, map->getMapHeight());

		am_equalsStr("The Region of Test", map->getFullName().c_str());

		Tile *tile = map->getTile(0, 0);
		assert(tile);
		am_equalsStr("brick", tile->getName().c_str());
		am_equals(0, map->getTileInstance(0, 0)->getBaseVariation());

		tile = map->getTile(3, 0);
		assert(tile);
		am_equalsStr("grass", tile->getName().c_str());
		am_equals(2, map->getTileInstance(3, 0)->getBaseVariation());

		tile = map->getTile(4, 4);
		assert(tile);
		am_equalsStr("swamp", tile->getName().c_str());
		am_equals(0, map->getTileInstance(4, 4)->getBaseVariation());

		tile = map->getTile(5, 6);
		assert(tile);
		am_equalsStr("water", tile->getName().c_str());
		am_equals(0, map->getTileInstance(5, 6)->getBaseVariation());

		return true;
	}

}
}
