#include "test_lua_tile.h"

#include <tests/asserts.h>

#include <base/handle.h>

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/tile.h>
#include <game/tile_type.h>
#include <game/engine.h>
using namespace am::game;

#include <lua/wrappers/game/lua_tile_type.h>
using namespace am::lua::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaTile::testSimple() {
		LuaState lua;
		
		assert(lua.loadString(
			"tile = am.tile.new(\"testTile\")\n"
			"function getName()\n"
			"	return tile:name()\n"
			"end\n"
			"function setName(name)\n"
			"	return tile:name(name)\n"
			"end\n"
			"function getFullName()\n"
			"	return tile:full_name()\n"
			"end\n"
			"function setFullName(name)\n"
			"	tile:full_name(name)\n"
			"end\n"
			"function getDescription()\n"
			"	return tile:description()\n"
			"end\n"
			"function setDescription(desc)\n"
			"	tile:description(desc)\n"
			"end\n"
			));

		assert(lua.hasGlobalFunction("getName"));
		lua_acall(lua, 0, 1);
		am_equalsStr("testTile", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setName"));
		lua.push("newTestTile");
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("getName"));
		lua_acall(lua, 0, 1);
		am_equalsStr("newTestTile", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setFullName"));
		lua.push("New Full Name");
		lua_acall(lua, 1, 0);
		
		assert(lua.hasGlobalFunction("getFullName"));
		lua_acall(lua, 0, 1);
		am_equalsStr("New Full Name", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setDescription"));
		lua.push("New Description");
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("getDescription"));
		lua_acall(lua, 0, 1);
		am_equalsStr("New Description", lua_tostring(lua, -1));
		lua.pop(1);

		return true;
	}

	bool TestLuaTile::testTileTypes() {
		LuaState lua;
		
		assert(lua.loadString(
			"tile = am.tile.new(\"testTile2\")\n"
			"function addTileType(type)\n"
			"	tile:add_tile_type(type)\n"
			"end\n"
			"function removeTileType(type)\n"
			"	tile:remove_tile_type(type)\n"
			"end\n"
			"function removeAllTileTypes()\n"
			"	tile:remove_all_tile_types()\n"
			"end\n"
			"function hasTileType(type)\n"
			"	return tile:has_tile_type(type)\n"
			"end\n"
			));

		assert(lua.hasGlobalFunction("hasTileType"));

		base::Handle<TileType> type(new TileType("superType", "Super Type"));
		Engine::getEngine()->addTileType(type);

		assert(lua.hasGlobalFunction("hasTileType"));
		wrapObject<TileType>(lua, type);
		lua_acall(lua, 1, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("hasTileType"));
		lua.push("superType");
		lua_acall(lua, 1, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("addTileType"));
		wrapObject<TileType>(lua, type);
		lua_acall(lua, 1, 0);
		
		assert(lua.hasGlobalFunction("hasTileType"));
		wrapObject<TileType>(lua, type);
		lua_acall(lua, 1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("hasTileType"));
		lua.push("superType");
		lua_acall(lua, 1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("removeTileType"));
		lua.push("superType");
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("hasTileType"));
		lua.push("superType");
		lua_acall(lua, 1, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("addTileType"));
		wrapObject<TileType>(lua, type);
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("hasTileType"));
		lua.push("superType");
		lua_acall(lua, 1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("removeAllTileTypes"));
		lua_acall(lua, 0, 0);

		assert(lua.hasGlobalFunction("hasTileType"));
		lua.push("superType");
		lua_acall(lua, 1, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		return true;
	}

	bool TestLuaTile::testTileTypes2() {
		LuaState lua;
		
		int loadResult = lua.loadString(
			"tile = am.tile.new(\"testTileTypes2\")\n"
			"land = am.tile_type.new(\"land-a\")\n"
			"water = am.tile_type.new(\"water-a\")\n"
			"tile:add_tile_type(land):add_tile_type(water)\n"
			
			"types = tile:tile_types()\n"
			"am.debug.equals(2, #types)\n"
			"am.debug.equals(\"land-a\", types[1]:name())\n"
			"am.debug.equals(\"water-a\", types[2]:name())\n"
			);

		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		return true;
	}

	bool TestLuaTile::testTransitional() {
		LuaState lua;
		
		int loadResult = lua.loadString(
			"tile = am.tile.new(\"testTransitional\")\n"
			"overlap_tile = am.tile.new(\"testOverlap\")\n"
			"am.debug.equals(true, am.debug.empty(tile:transitional()))\n"
			"am.debug.equals(true, am.debug.empty(tile:transitional(nil)))\n"
			"am.debug.equals(true, am.debug.empty(tile:transitional(overlap_tile)))\n"
			"am.debug.equals(true, am.debug.empty(tile:all_transitional()))\n"
			
			"cursor = am.asset.new(\"cursor\")\n"
			"am.debug.not_equals(nil, cursor)\n"
			"am.debug.equals(false, tile:has_transitional(cursor))\n"
			"am.debug.equals(false, tile:has_transitional(\"cursor\"))\n"
			
			"tile:add_transitional(cursor)\n"
			"trans = tile:transitional()\n"
			"am.debug.equals(false, am.debug.empty(trans))\n"
			"am.debug.equals(1, #trans)\n"
			"am.debug.equals(cursor, trans[1])\n"
			"all_trans = tile:all_transitional()\n"
			"am.debug.equals(false, am.debug.empty(all_trans))\n"
			"am.debug.equals(1, #all_trans[\"_default_\"])\n"
			"am.debug.equals(cursor, all_trans[\"_default_\"][1])\n"

			"am.debug.equals(true, tile:has_transitional(cursor))\n"
			"am.debug.equals(true, tile:has_transitional(\"cursor\"))\n"

			"paused = am.asset.new(\"paused\")\n"
			"am.debug.not_equals(nil, paused)\n"
			"tile:add_transitional(paused, overlap_tile)\n"
			"trans = tile:transitional()\n"
			"am.debug.equals(false, am.debug.empty(trans))\n"
			"am.debug.equals(1, #trans)\n"
			"am.debug.equals(cursor, trans[1])\n"
			"trans = tile:transitional(overlap_tile)\n"
			"am.debug.equals(false, am.debug.empty(trans))\n"
			"am.debug.equals(1, #trans)\n"
			"am.debug.equals(paused, trans[1])\n"
			"all_trans = tile:all_transitional()\n"
			"am.debug.equals(false, am.debug.empty(all_trans))\n"
			"am.debug.equals(1, #all_trans[\"_default_\"])\n"
			"am.debug.equals(cursor, all_trans[\"_default_\"][1])\n"
			"am.debug.equals(1, #all_trans[\"testOverlap\"])\n"
			"am.debug.equals(paused, all_trans[\"testOverlap\"][1])\n"

			"am.debug.equals(true, tile:has_transitional(cursor))\n"
			"am.debug.equals(true, tile:has_transitional(\"cursor\"))\n"
			"am.debug.equals(false, tile:has_transitional(cursor, overlap_tile))\n"
			"am.debug.equals(false, tile:has_transitional(\"cursor\", overlap_tile))\n"
			"am.debug.equals(true, tile:has_transitional(paused, overlap_tile))\n"
			"am.debug.equals(true, tile:has_transitional(\"paused\", overlap_tile))\n"
			
			"tile:remove_transitional(paused)\n"
			"am.debug.equals(true, tile:has_transitional(cursor))\n"
			"am.debug.equals(true, tile:has_transitional(\"cursor\"))\n"
			"am.debug.equals(false, tile:has_transitional(cursor, overlap_tile))\n"
			"am.debug.equals(false, tile:has_transitional(\"cursor\", overlap_tile))\n"
			"am.debug.equals(true, tile:has_transitional(paused, overlap_tile))\n"
			"am.debug.equals(true, tile:has_transitional(\"paused\", overlap_tile))\n"

			"tile:remove_transitional(paused, overlap_tile)\n"
			"am.debug.equals(true, tile:has_transitional(cursor))\n"
			"am.debug.equals(true, tile:has_transitional(\"cursor\"))\n"
			"am.debug.equals(false, tile:has_transitional(cursor, overlap_tile))\n"
			"am.debug.equals(false, tile:has_transitional(\"cursor\", overlap_tile))\n"
			"am.debug.equals(false, tile:has_transitional(paused, overlap_tile))\n"
			"am.debug.equals(false, tile:has_transitional(\"paused\", overlap_tile))\n"

			"tile:remove_transitional(\"cursor\")\n"
			"am.debug.equals(false, tile:has_transitional(cursor))\n"
			"am.debug.equals(false, tile:has_transitional(\"cursor\"))\n"
			"am.debug.equals(false, tile:has_transitional(cursor, overlap_tile))\n"
			"am.debug.equals(false, tile:has_transitional(\"cursor\", overlap_tile))\n"
			"am.debug.equals(false, tile:has_transitional(paused, overlap_tile))\n"
			"am.debug.equals(false, tile:has_transitional(\"paused\", overlap_tile))\n"
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