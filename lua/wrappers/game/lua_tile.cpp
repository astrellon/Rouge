#include "lua_tile.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/tile.h>
#include <game/tile_type.h>
#include <game/engine.h>
using namespace am::game;

#include <util/json_value.h>
using namespace am::util;

#include "lua_tile_type.h"
#include "lua_tile_set.h"

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * The tile class represents a single tile and all the properties of that tile.
	 * Tiles are added to maps via the TileInstance class. Each tile stores their graphic
	 * tile type list, name, description and the tile set which this tile is apart of.
	 */
	/**
	 * Creates a new tile object with the given name, the name need only be unique within its tile set.
	 * Until a full name is specified, the tile full name is "No full name"
	 *
	 * @param string name The name of the tile.
	 */
	/**
	 * Creates a new tile object with the given name and full name, 
	 * the name need only be unique within its tile set.
	 * Full name differs in that it is the name that is presented to the user.
	 *
	 * @param string name The name of the tile.
	 * @param string full_name The full name of the tile.
	 */
	int Tile_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 1 && lua_isstr(lua, 1))
		{
			Tile *tile = new Tile(lua_tostring(lua, 1));
			wrapRefObject<Tile>(lua, tile);
			return 1;
		}
		else if (args == 2 && lua_isstr(lua, 1) && lua_isstr(lua, 2))
		{
			Tile *tile = new Tile(lua_tostring(lua, 1), lua_tostring(lua, 2));
			wrapRefObject<Tile>(lua, tile);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@new", 2, "string name", "string name, string full_name");
	}
	/**
	 * Release the reference count on this tile.
	 */
	int Tile_dtor(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile)
		{
			tile->release();
			return 0;
		}
		return LuaState::expectedContext(lua, "__gc", "am.tile");
	}

	/**
	 * Compares this tile against another tile object.
	 *
	 * @param am.tile rhs The other tile to compare with.
	 * @returns boolean True if they are the same tile object.
	 */
	int Tile_eq(lua_State *lua)
	{
		Tile *lhs = castUData<Tile>(lua, 1);
		Tile *rhs = castUData<Tile>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Tile_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Tile_ctor },
			{ "__gc", Tile_dtor },
			{ "__eq", Tile_eq },
			{ "name", Tile_name },
			{ "full_name", Tile_full_name },
			{ "description", Tile_description },
			{ "tile_set", Tile_tile_set },
			{ "add_tile_type", Tile_add_tile_type },
			{ "remove_tile_type", Tile_remove_tile_type },
			{ "remove_all_tile_types", Tile_remove_all_tile_types },
			{ "has_tile_type", Tile_has_tile_type },
			{ "tile_types", Tile_tile_types },
			{ "load_def", Tile_load_def },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Tile::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	/**
	 * Returns the tile name.
	 *
	 * @returns string The name of the tile.
	 */
	/**
	 * Sets the name of the tile.
	 * If there already exists a tile with the same name, it will be overridden
	 * by this tile if the 'check_for_existing' argument is not true.
	 *
	 * @param string name The new name for this tile.
	 * @param boolean [false] check_for_existing When true the name will be not changed if there
	 *  is already a tile with the same name in the tile's tile set.
	 * @returns boolean True if the tile's name was changed.
	 */
	int Tile_name(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile)
		{
			int args = lua_gettop(lua);
			if (args == 1)
			{
				lua_pushstring(lua, tile->getName().c_str());
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				bool checkForExisting = false;
				bool valid = true;
				if (args >= 3)
				{
					if (lua_isbool(lua, 3))
					{
						checkForExisting = lua_tobool(lua, 3);
					}
					else
					{
						valid = false;
					}
				}
				if (checkForExisting)
				{
					lua_pushboolean(lua, tile->setNameWithCheck(lua_tostring(lua, 2)));
					return 1;
				}
				if (valid)
				{
					tile->setName(lua_tostring(lua, 2));
					lua_pushboolean(lua, 1);
					return 1;
				}
			}
			return LuaState::expectedArgs(lua, "name", "string name, boolean [false] check_for_existing");
		}
		return LuaState::expectedContext(lua, "name", "am.tile");
	}

	/**
	 * Returns the full name of this tile, this is used to display the tile's name to the user.
	 *
	 * @returns string The tiles full name.
	 */
	/**
	 * Sets the tile's full name, this is used to display the tile's name to the user.
	 *
	 * @param string full_name The tile's new full name.
	 * @returns am.tile This
	 */
	int Tile_full_name(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, tile->getFullName().c_str());
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				tile->setFullName(lua_tostring(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "full_name", "string full_name");
		}
		return LuaState::expectedContext(lua, "full_name", "am.tile");
	}
	/**
	 * Returns the description for this tile.
	 *
	 * @returns string The tiles description.
	 */
	/**
	 * Sets the tile's description.
	 *
	 * @param string description The tile's description.
	 * @returns am.tile This
	 */
	int Tile_description(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, tile->getDescription().c_str());
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				tile->setDescription(lua_tostring(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "description", "string description");
		}
		return LuaState::expectedContext(lua, "description", "am.tile");
	}
	/**
	 * Returns the tile set that this tile is apart of, nil will be return if it is not part of a tile set.
	 *
	 * @returns TileSet The tile's tile set, or nil if it does not have one.
	 */
	/**
	 * Sets and adds the tile to the given tile set.
	 * Any tile with the same name in the tile set will be overridden by this tile.
	 * Unless the 'check_for_existing' argument is set to true, then if the tileset already has
	 * a tile with the same name then tile set is not changed.
	 *
	 * @param TileSet set The tile set to add to, can be nil.
	 * @param boolean [false] check_for_existing When true the tile set will not be changed if it 
	 *  already has a tile with the same name.
	 * @returns boolean True if the tile set was changed.
	 */
	int Tile_tile_set(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile)
		{
			int args = lua_gettop(lua);
			if (args == 1)
			{
				if (tile->getTileSet())
				{
					wrapRefObject<TileSet>(lua, tile->getTileSet());
					return 1;
				}
				lua_pushnil(lua);
				return 1;
			}
			else
			{
				TileSet *set = castUData<TileSet>(lua, 2);
				// If a non-nil tile set has been passed in.
				if (set)
				{
					bool withCheck = false;
					if (args == 3)
					{
						withCheck = lua_tobool(lua, 3);
					}
					// Checking for existing tiles with the same name.
					if (withCheck)
					{
						if (set->addTileWithCheck(tile))
						{
							tile->setTileSet(set);
							lua_pushboolean(lua, 1);
							return 1;
						}
						lua_pushboolean(lua, 0);
						return 1;
					}
					// Just replace any tile with the same name.
					else
					{
						tile->setTileSet(set);
						set->addTile(tile);

						lua_pushboolean(lua, 1);
						return 1;
					}
				}
				// The given tile set was nil, so there's nothing to check.
				else
				{
					tile->setTileSet(set);
					lua_pushboolean(lua, 1);
					return 1;
				}
			}
			return LuaState::expectedArgs(lua, "tile_set", "TileSet tileSet, boolean [false] check_for_existing");
		}
		return LuaState::expectedContext(lua, "tile_set", "am.tile");
	}

	/**
	 * Adds a tile type to the list of tile types.
	 * If a type is already in the list, it will be ignored.
	 *
	 * @param am.tile_type tile_type The tile type to add to the list.
	 * @returns am.tile This
	 */
	/**
	 * Adds a tile type to the list of tile types.
	 * If a type is already in the list, it will be ignored.
	 * 
	 * @param string tile_type_name The name of the tile type to add to the list.
	 * @returns am.tile This
	 */
	int Tile_add_tile_type(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile)
		{
			Handle<TileType> tileType(getTileType(lua, 2));
			if (tileType)
			{
				tile->addTileType(tileType);
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "add_tile_type", 2, "am.tile_type tile_type", "string tile_type_name");
		}
		return LuaState::expectedContext(lua, "add_tile_type", "am.tile");
	}
	/**
	 * Removes a tile type from the list of tile types.
	 * If a type is not in the list, it will be ignored.
	 *
	 * @param am.tile_type tile_type The tile type to remove from the list.
	 * @returns am.tile This
	 */
	/**
	 * Removes a tile type from the list of tile types.
	 * If a type is not in the list, it will be ignored.
	 * 
	 * @param string tile_type_name The name of the tile type to remove from the list.
	 * @returns am.tile This
	 */
	int Tile_remove_tile_type(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile)
		{
			Handle<TileType> tileType(getTileType(lua, 2));
			if (tileType)
			{
				tile->removeTileType(tileType);
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "remove_tile_type", 2, "am.tile_type tile_type", "string tile_type_name");
		}
		return LuaState::expectedContext(lua, "remove_tile_type", "am.tile");
	}
	/**
	 * Removes all tile types from this tiles list of tile types.
	 *
	 * @returns am.tile This
	 */
	int Tile_remove_all_tile_types(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile)
		{
			tile->removeAllTileTypes();
			lua_first(lua);
		}
		return LuaState::expectedContext(lua, "remove_all_tile_types", "am.tile");
	}
	/**
	 * Returns true if the given tile type is in the list of tile types.
	 *
	 * @param am.tile_type tile_type The tile type to check for.
	 * @returns boolean True if the given tile type is in the list.
	 */
	/**
	 * Returns true if the given tile type is in the list of tile types.
	 *
	 * @param string tile_type_name The name of tile type to check for.
	 * @returns boolean True if the given tile type is in the list.
	 */
	int Tile_has_tile_type(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile)
		{
			Handle<TileType> tileType(getTileType(lua, 2));
			if (tileType)
			{
				lua_pushboolean(lua, tile->hasTileType(tileType));
				return 1;
			}
			return LuaState::expectedArgs(lua, "has_tile_type", 2, "am.tile_type tile_type", "string tile_type_name");
		}
		return LuaState::expectedContext(lua, "has_tile_type", "am.tile");
	}
	/**
	 * Returns an array of all the tile types in this tiles list of tile tpyes.
	 * 
	 * <pre>
	 * tile = am.tile.new("swamp")
	 * land = am.tile_type.new("land")
	 * water = am.tile_type.new("water")
	 * tile:add_tile_type(land)
	 * tile:add_tile_type(water)
	 * 
	 * types = tile:tile_types()
	 * am_log("Type 1: " .. types[1]:name()") -- Outputs "Type 1: land"
	 * am_log("Type 2: " .. types[2]:name()") -- Outputs "Type 2: wayer"
	 * </pre>
	 *
	 * @returns Array The array of all the tile types on this tile.
	 */
	int Tile_tile_types(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile)
		{
			LuaState L(lua);
			L.newTable();
			auto types = tile->getTileTypes();
			int i = 1;
			for (auto iter = types.begin(); iter != types.end(); ++iter)
			{
				wrapRefObject<TileType>(lua, *iter);
				lua_rawseti(lua, -2, i++);
			}
			return 1;
		}
		return LuaState::expectedContext(lua, "tile_types", "am.tile");
	}

	/**
	 * @private
	 * TODO
	 */
	int Tile_load_def(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile && lua_istable(lua, -1))
		{
			LuaState wrap(lua);
			tile->loadDef(wrap);
		}
		return 0;
	}

	TileType *getTileType(lua_State *lua, int n)
	{
		TileType *tileType = NULL;
		if (lua_isstring(lua, n))
		{
			tileType = Engine::getEngine()->getTileType(lua_tostring(lua, n));
		}
		else
		{
			tileType = castUData<TileType>(lua, n);
		}
		return tileType;
	}

}
}
}
