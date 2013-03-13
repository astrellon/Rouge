#include "lua_map.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/map.h>
#include <game/tile_instance.h>
#include <game/character.h>
#include <game/game.h>
#include <game/engine.h>
using namespace am::game;

#include "lua_tile.h"
#include "lua_tile_instance.h"
#include "lua_character.h"
#include "lua_game.h"

#include <log/logger.h>

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * The Map class stores all the information relating to a single map.
	 * This includes all game objects on the map and all the tile data for the map.
	 */
	/**
	 * Creates a new map instance.
	 * The map size will have to be set before any tile data can be set.
	 *
	 * @param string mapName The name of the map.
	 */
	/**
	 * Creates a new map instance.
	 *
	 * @param string mapName The name of the map.
	 * @param integer width The width of the map in tiles.
	 * @param integer height The height of the map in tiles.
	 */
	int Map_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 1 && lua_isstr(lua, 1))
		{
			Map *map = new Map(lua_tostring(lua, 1));
			wrapRefObject<Map>(lua, map);
			return 1;
		}
		else if (args == 3 && lua_isstr(lua, 1) && lua_isnum(lua, 2) && lua_isnum(lua, 3))
		{
			Map *map = new Map(lua_tostring(lua, 1), lua_tointeger(lua, 2), lua_tointeger(lua, 3));
			wrapRefObject<Map>(lua, map);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@new", 2, "string mapName", "string mapName, integer width, integer height");
	}
	/**
	 * Releases the reference counter on this map.
	 */
	int Map_dtor(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			map->release();
			return 0;
		}
		return LuaState::expectedContext(lua, "__gc", "Map");
	}
	/**
	 * Compares this map against another map object.
	 *
	 * @param Map rhs The other map to compare with.
	 * @returns boolean True if they are the same map object.
	 */
	int Map_eq(lua_State *lua)
	{
		Map *lhs = castUData<Map>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "Map");
		}
		Map *rhs = castUData<Map>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Map_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Map_ctor },
			{ "__gc",  Map_dtor },
			{ "__eq", Map_eq },
			{ "name", Map_name },
			{ "full_name", Map_full_name },
			{ "tile", Map_tile },
			{ "tile_instance", Map_tile_instance },
			{ "tiles", Map_tiles },
			{ "map_size", Map_map_size },
			{ "get_objects", NULL },
			{ "add_game_object", Map_add_game_object },
			{ "remove_game_object", Map_remove_game_object },
			{ "has_game_object", Map_has_game_object },
			{ "is_valid_location", Map_is_valid_location },
			{ "is_valid_grid_location", Map_is_valid_grid_location },
			{ "load_def", Map_load_def },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Map::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	/**
	 * Returns the map name which is also used as the maps id.
	 *
	 * @returns string The map name.
	 */
	/**
	 * Sets the map name, which is used as the map id.
	 * TODO Currently this doesn't update the map of Maps, so it'll still be
	 * referred to by it's previous name if it's been registered with a Game instance.
	 *
	 * @param string mapName The new map name.
	 * @returns Map This
	 */
	int Map_name(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, map->getName().c_str());
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				map->setName(lua_tostring(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "name", "string mapName");
		}
		return LuaState::expectedContext(lua, "name", "Map");
	}
	/**
	 * Returns the full name of the map. This is used when displaying the map to the user.
	 *
	 * @returns string The map's full name.
	 */
	/**
	 * Sets the map's full name. This is used when displaying the map to the user.
	 *
	 * @param string fullName The map's new full name.
	 * @returns Map This
	 */
	int Map_full_name(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, map->getFullName().c_str());
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				map->setFullName(lua_tostring(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "full_name", "string fullName");
		}
		return LuaState::expectedContext(lua, "full_name", "Map");
	}
	/**
	 * Returns the tile object on the tile instance at the given grid location.
	 *
	 * @param integer x The x position of the tile.
	 * @param integer y The y position of the tile.
	 * @returns Tile The found file, nil if out of bounds.
	 */
	int Map_tile(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			Tile *tile = map->getTile(lua_tointeger(lua, -2), lua_tointeger(lua, -1));
			if (tile)
			{
				wrapRefObject<Tile>(lua, tile);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the tile instance at the given grid location.
	 *
	 * @param integer x The x position of the tile instance.
	 * @param integer y The y position of the tile instance.
	 * @returns TileInstance The found tile instance, nil if out of bounds.
	 */
	int Map_tile_instance(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
			{
				TileInstance *inst = map->getTileInstance(lua_tointeger(lua, -2), lua_tointeger(lua, -1));
				if (inst)
				{
					wrapObject<TileInstance>(lua, inst);
					return 1;
				}
				lua_pushnil(lua);
				return 1;
			}
			return LuaState::expectedArgs(lua, "tile_instance", "integer x, integer y");
		}
		return LuaState::expectedContext(lua, "tile_instance", "Map");
	}
	/**
	 * Sets the tiles for this map from an Array of tile names and frames.
	 * The length of the array must equal the total number of tiles, which is <code>map_width * map_height</code>.
	 * <pre>
	 * Map, Engine = import("Map", "Engine")
	 * Engine.using_tile_set("nature")
	 * map = Map.new("testMap", 3, 2)
	 * map:tiles({
	 *     "dirt", "dirt:1", "castle/dirt:1",
	 *     "grass:2", "water", "castle/water"
	 * })
	 * </pre>
	 * <span>Each tile name is in the following form.</span>
	 * <pre>
	 * [tileSetName/]tileName[:frameNumber]
	 * </pre>
	 * <span>The tileSetName is optional and allows you to specify the tile set
	 * which the tile is from. If none is provided then the top level tile set
	 * or any tile sets specified with the <code>Engine.using_tile_set</code> function.
	 * The frameNumber is also optional and allows for specifying which
	 * frame of the tile sprite.</span>
	 *
	 * @param Array tiles An array of tiles to set onto the map.
	 * @returns Map This
	 */
	/**
	 * Sets the tiles for this map from an Array of tile names and frames.
	 * This function changes the size of the map to fit the parameters given.
	 * <pre>
	 * Map, Engine = import("Map", "Engine")
	 * Engine.using_tile_set("nature")
	 * map = Map.new("testMap")
	 * map:tiles({
	 *     "dirt", "dirt:1", "castle/dirt:1",
	 *     "grass:2", "water", "castle/water"
	 * }, 3, 2)
	 * </pre>
	 * <span>Each tile name is in the following form.</span>
	 * <pre>
	 * [tileSetName/]tileName[:frameNumber]
	 * </pre>
	 * <span>The tileSetName is optional and allows you to specify the tile set
	 * which the tile is from. If none is provided then the top level tile set
	 * or any tile sets specified with the <code>Engine.using_tile_set</code> function.
	 * The frameNumber is also optional and allows for specifying which
	 * frame of the tile sprite.</span>
	 *
	 * @param array tiles An array of tiles to set onto the map.
	 * @param integer width The width of the array given.
	 * @param integer height The height of the array given.
	 * @returns Map This
	 */
	int Map_tiles(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			int args = lua_gettop(lua);
			if (!lua_istable(lua, 2) || (args == 3 && !lua_isnum(lua, 3) && !lua_isnum(lua, 4)))
			{
				return LuaState::expectedArgs(lua, "tiles", 2, "array tiles", "array tiles, integer width, integer height");
			}
			if (args == 3)
			{
				map->setMapSize(lua_tointeger(lua, 3), lua_tointeger(lua, 4));
			}
			int total = map->getMapWidth() * map->getMapHeight();
			lua_pushnil(lua);
			int i = 0;
			TileInstance *insts = map->getTiles();
			while (lua_next(lua, -2) != 0)
			{
				if (lua_isstring(lua, -1))
				{
					insts[i].setTileName(lua_tostring(lua, -1));
				}
				else
				{
					Tile *tile = castUData<Tile>(lua, -1);
					insts[i].setTile(tile);
				}
				lua_pop(lua, 1);
				i++;
				if (i >= total)
				{
					break;
				}
			}
			map->updateAssetSprites();
			lua_first(lua);
		}
		return LuaState::expectedContext(lua, "tiles", "Map");
	}
	/**
	 * Returns the size of the map.
	 *
	 * @returns integer The width of the map.
	 * @returns integer The height of the map.
	 */
	/**
	 * Sets the size of the map. This destorys the old map data.
	 *
	 * @param integer width The new width.
	 * @param integer height The new height.
	 * @returns Map This
	 */
	int Map_map_size(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushinteger(lua, map->getMapWidth());
				lua_pushinteger(lua, map->getMapHeight());
				return 2;
			}
			else if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
			{
				map->setMapSize(lua_tointeger(lua, 2), lua_tointeger(lua, 3));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "map_size", "integer width, integer height");
		}
		return LuaState::expectedContext(lua, "map_size", "Map");
	}
	/**
	 * Adds a game object to this map.
	 *
	 * @param GameObject gameObject The game object to add to the map.
	 * @returns boolean True if the game object was successfully added to this map.
	 */
	int Map_add_game_object(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			GameObject *obj = getGameObject(lua, 2);
			if (obj)
			{
				lua_pushboolean(lua, map->addGameObject(obj));
				return 1;
			}
			return LuaState::expectedArgs(lua, "add_game_object", "GameObject gameObject");
		}
		return LuaState::expectedContext(lua, "add_game_object", "Map");
	}
	/**
	 * Removes a game object from the map.
	 *
	 * @param GameObject The game object to remove from the map.
	 * @returns boolean True if the game object was successfully removed from the map.
	 */
	int Map_remove_game_object(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			GameObject *obj = getGameObject(lua, 2);
			if (obj)
			{
				lua_pushboolean(lua, map->removeGameObject(obj));
				return 1;
			}
			return LuaState::expectedArgs(lua, "remove_game_object", "GameObject gameObject");
		}
		return LuaState::expectedContext(lua, "remove_game_object", "Map");
	}
	/**
	 * Returns true if the given game object is on this map.
	 *
	 * @param GameObject gameObject The game object to look up.
	 * @returns boolean True if the given game object was found.
	 */
	int Map_has_game_object(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			GameObject *obj = getGameObject(lua, 2);
			if (obj)
			{
				lua_pushboolean(lua, map->hasGameObject(obj));
				return 1;
			}
			return LuaState::expectedArgs(lua, "has_game_object", "GameObject gameObject");
		}
		return LuaState::expectedContext(lua, "has_game_object", "Map");
	}
	/**
	 * Used to tell if the given game object can be placed at the given location.
	 * This uses the game objects passibility array to tell if it can be at the given location.
	 *
	 * @param GameObject gameObject The game object to test with.
	 * @param number positionX The x position to test at.
	 * @param number positionY The y position to test at.
	 * @returns boolean True if the given position is valid for the given game object.
	 */
	int Map_is_valid_location(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			GameObject *obj = getGameObject(lua, 2);
			if (obj && lua_isnum(lua, 3) && lua_isnum(lua, 4))
			{
				lua_pushboolean(lua, map->isValidLocation(lua_tofloat(lua, 3), lua_tofloat(lua, 4), obj));
				return 1;
			}
			return LuaState::expectedArgs(lua, "is_valid_location", "GameObject gameObject, number x, number y");
		}
		return LuaState::expectedContext(lua, "is_valid_location", "Map");
	}
	/**
	 * Used to tell if the given game object can be placed at the given grid location.
	 * This uses the game objects passibility array to tell if it can be at the given grid location.
	 *
	 * @param GameObject gameObject The game object to test with.
	 * @param integer gridX The x grid position to test at.
	 * @param integer gridY The y grid position to test at.
	 * @returns boolean True if the given position is valid for the given game object.
	 */
	int Map_is_valid_grid_location(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			GameObject *obj = getGameObject(lua, 2);
			if (obj && lua_isnum(lua, 3) && lua_isnum(lua, 4))
			{
				lua_pushboolean(lua, map->isValidGridLocation(lua_tointeger(lua, 3), lua_tointeger(lua, 4), obj));
				return 1;
			}
			return LuaState::expectedArgs(lua, "is_valid_grid_location", "GameObject gameObject, integer x, integer y");
		}
		return LuaState::expectedContext(lua, "is_valid_grid_location", "Map");
	}
	/**
	 * @private
	 */
	int Map_load_def(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map && lua_istable(lua, -1))
		{
			LuaState wrap(lua);
			map->loadDef(wrap);
		}
		return 0;
	}

}
}
}
