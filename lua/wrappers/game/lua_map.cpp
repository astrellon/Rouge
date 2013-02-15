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
	 * @param String mapName The name of the map.
	 */
	/**
	 * Creates a new map instance.
	 *
	 * @param String mapName The name of the map.
	 * @param Integer width The width of the map in tiles.
	 * @param Integer height The height of the map in tiles.
	 */
	int Map_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 1 && lua_isstring(lua, -1))
		{
			Map *map = new Map(lua_tostring(lua, -1));
			wrapRefObject<Map>(lua, map);
			return 1;
		}
		else if (args == 3 && lua_isstring(lua, -3) && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			Map *map = new Map(lua_tostring(lua, -3), lua_tointeger(lua, -2), lua_tointeger(lua, -1));
			wrapRefObject<Map>(lua, map);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
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
		}
		return 0;
	}
	/**
	 * Compares this map against another map object.
	 *
	 * @param Map rhs The other map to compare with.
	 * @returns Boolean True if they are the same map object.
	 */
	int Map_eq(lua_State *lua)
	{
		Map *lhs = castUData<Map>(lua, 1);
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
	 * @returns String The map name.
	 */
	/**
	 * Sets the map name, which is used as the map id.
	 * TODO Currently this doesn't update the map of Maps, so it'll still be
	 * referred to by it's previous name if it's been registered with a Game instance.
	 *
	 * @param String mapName The new map name.
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
			else if (lua_isstring(lua, -1))
			{
				map->setName(lua_tostring(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the full name of the map. This is used when displaying the map to the user.
	 *
	 * @returns String The map's full name.
	 */
	/**
	 * Sets the map's full name. This is used when displaying the map to the user.
	 *
	 * @param String fullName The map's new full name.
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
			else if (lua_isstring(lua, -1))
			{
				map->setFullName(lua_tostring(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the tile object on the tile instance at the given grid location.
	 *
	 * @param Integer x The x position of the tile.
	 * @param Integer y The y position of the tile.
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
	 * @param Integer x The x position of the tile instance.
	 * @param Integer y The y position of the tile instance.
	 * @returns TileInstance The found tile instance, nil if out of bounds.
	 */
	int Map_tile_instance(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			TileInstance *inst = map->getTileInstance(lua_tointeger(lua, -2), lua_tointeger(lua, -1));
			if (inst)
			{
				wrapObject<TileInstance>(lua, inst);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
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
	 * @param Array tiles An array of tiles to set onto the map.
	 * @param Integer width The width of the array given.
	 * @param Integer height The height of the array given.
	 * @returns Map This
	 */
	int Map_tiles(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map && lua_istable(lua, -1))
		{
			if (lua_isnumber(lua, 2) && lua_isnumber(lua, 3))
			{
				map->setMapSize(lua_tointeger(lua, 2), lua_tointeger(lua, 3));
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
			lua_pushvalue(lua, 1);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the size of the map.
	 *
	 * @returns Integer The width of the map.
	 * @returns Integer The height of the map.
	 */
	/**
	 * Sets the size of the map. This destorys the old map data.
	 *
	 * @param Integer width The new width.
	 * @param Integer height The new height.
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
			else if (lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
			{
				map->setMapSize(lua_tointeger(lua, -2), lua_tointeger(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		lua_pushnil(lua);
		return 2;
	}
	/**
	 * Adds a game object to this map.
	 *
	 * @param GameObject gameObject The game object to add to the map.
	 * @returns Boolean True if the game object was successfully added to this map.
	 */
	int Map_add_game_object(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			GameObject *obj = getGameObject(lua, -1);
			lua_pushboolean(lua, map->addGameObject(obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Removes a game object from the map.
	 *
	 * @param GameObject The game object to remove from the map.
	 * @returns Boolean True if the game object was successfully removed from the map.
	 */
	int Map_remove_game_object(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			GameObject *obj = getGameObject(lua, -1);
			lua_pushboolean(lua, map->removeGameObject(obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns true if the given game object is on this map.
	 *
	 * @param GameObject gameObject The game object to look up.
	 * @returns Boolean True if the given game object was found.
	 */
	int Map_has_game_object(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			GameObject *obj = getGameObject(lua, -1);
			lua_pushboolean(lua, map->hasGameObject(obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Used to tell if the given game object can be placed at the given location.
	 * This uses the game objects passibility array to tell if it can be at the given location.
	 *
	 * @param GameObject The game object to test with.
	 * @param Number positionX The x position to test at.
	 * @param Number positionY The y position to test at.
	 * @returns Boolean True if the given position is valid for the given game object.
	 */
	int Map_is_valid_location(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map && lua_isnumber(lua, -3) && lua_isnumber(lua, -2))
		{
			GameObject *obj = getGameObject(lua, -1);
			if (obj)
			{
				lua_pushboolean(lua, map->isValidLocation(lua_tofloat(lua, -3), lua_tofloat(lua, -2), obj));
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Used to tell if the given game object can be placed at the given grid location.
	 * This uses the game objects passibility array to tell if it can be at the given grid location.
	 *
	 * @param GameObject The game object to test with.
	 * @param Integer gridX The x grid position to test at.
	 * @param Integer gridY The y grid position to test at.
	 * @returns Boolean True if the given position is valid for the given game object.
	 */
	int Map_is_valid_grid_location(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map && lua_isnumber(lua, -3) && lua_isnumber(lua, -2))
		{
			GameObject *obj = getGameObject(lua, -1);
			if (obj)
			{
				lua_pushboolean(lua, map->isValidGridLocation(lua_tointeger(lua, -3), lua_tointeger(lua, -2), obj));
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
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
