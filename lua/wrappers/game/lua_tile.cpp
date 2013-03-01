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
	 * The Tile class represents a single tile and all the properties of that tile.
	 * Tiles are added to maps via the TileInstance class. Each tile stores their graphic
	 * tile type list, name, description and the tile set which this tile is apart of.
	 */
	/**
	 * Creates a new tile object with the given name, the name need only be unique within its tile set.
	 * Until a full name is specified, the tile full name is "No full name"
	 *
	 * @param Stringname The name of the tile.
	 */
	/**
	 * Creates a new tile object with the given name and full name, 
	 * the name need only be unique within its tile set.
	 * Full name differs in that it is the name that is presented to the user.
	 *
	 * @param String name The name of the tile.
	 * @param String description The description of the tile.
	 */
	int Tile_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 1 && lua_isstring(lua, -1))
		{
			Tile *tile = new Tile(lua_tostring(lua, -1));
			wrapRefObject<Tile>(lua, tile);
			return 1;
		}
		else if (args == 2 && lua_isstring(lua, -2) && lua_isstring(lua, -1))
		{
			Tile *tile = new Tile(lua_tostring(lua, -2), lua_tostring(lua, -1));
			wrapRefObject<Tile>(lua, tile);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
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
		}
		return 0;
	}

	/**
	 * Compares this tile against another tile object.
	 *
	 * @param Tile rhs The other tile to compare with.
	 * @returns Boolean True if they are the same tile object.
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
	 * @returns String The name of the tile.
	 */
	/**
	 * Sets the name of the tile.
	 * If there already exists a tile with the same name, it will be overridden
	 * by this tile if the 'checkForExisting' argument is not true.
	 *
	 * @param String name The new name for this tile.
	 * @param Boolean [false] checkForExisting When true the name will be not changed if there
	 *  is already a tile with the same name in the tile's tile set.
	 * @returns Boolean True if the tile's name was changed.
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
			else if (lua_isstring(lua, 2))
			{
				bool checkForExisting = false;
				if (args >= 3)
				{
					checkForExisting = lua_tobool(lua, 3);
				}
				if (checkForExisting)
				{
					lua_pushboolean(lua, tile->setNameWithCheck(lua_tostring(lua, -1)));
					return 1;
				}
				tile->setName(lua_tostring(lua, -1));
				lua_pushboolean(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	/**
	 * Returns the full name of this tile, this is used to display the tile's name to the user.
	 *
	 * @returns String The tiles full name.
	 */
	/**
	 * Sets the tile's full name, this is used to display the tile's name to the user.
	 *
	 * @param String fullName The tile's new full name.
	 * @returns Tile This
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
			else if (lua_isstring(lua, -1))
			{
				tile->setFullName(lua_tostring(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the description for this tile.
	 *
	 * @returns String The tiles description.
	 */
	/**
	 * Sets the tile's description.
	 *
	 * @param String description The tile's description.
	 * @returns Tile This
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
			else if (lua_isstring(lua, -1))
			{
				tile->setDescription(lua_tostring(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the tile set that this tile is apart of, nil will be return if it is not part of a tile set.
	 *
	 * @returns TileSet The tile's tile set, or nil if it does not have one.
	 */
	/**
	 * Sets and adds the tile to the given tile set.
	 * Any tile with the same name in the tile set will be overridden by this tile.
	 * Unless the 'checkForExisting' argument is set to true, then if the tileset already has
	 * a tile with the same name then tile set is not changed.
	 *
	 * @param TileSet set The tile set to add to, can be nil.
	 * @param Boolean [false] checkForExisting When true the tile set will not be changed if it 
	 *  already has a tile with the same name.
	 * @returns Boolean True if the tile set was changed.
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
		}
		lua_pushnil(lua);
		return 1;
	}

	/**
	 * Adds a tile type to the list of tile types.
	 * If a type is already in the list, it will be ignored.
	 *
	 * @param TileType tileType The tile type to add to the list.
	 * @returns Tile This
	 */
	/**
	 * Adds a tile type to the list of tile types.
	 * If a type is already in the list, it will be ignored.
	 * 
	 * @param String tileTypeName The name of the tile type to add to the list.
	 * @returns Tile This
	 */
	int Tile_add_tile_type(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile)
		{
			TileType *tileType = getTileType(lua, -1);
			if (tileType)
			{
				tile->addTileType(tileType);
			}
			lua_pushvalue(lua, 1);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Removes a tile type from the list of tile types.
	 * If a type is not in the list, it will be ignored.
	 *
	 * @param TileType tileType The tile type to remove from the list.
	 * @returns Tile This
	 */
	/**
	 * Removes a tile type from the list of tile types.
	 * If a type is not in the list, it will be ignored.
	 * 
	 * @param String tileTypeName The name of the tile type to remove from the list.
	 * @returns Tile This
	 */
	int Tile_remove_tile_type(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile)
		{
			TileType *tileType = getTileType(lua, -1);
			if (tileType)
			{
				tile->removeTileType(tileType);
			}
			lua_pushvalue(lua, 1);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Removes all tile types from this tiles list of tile types.
	 *
	 * @returns Tile This
	 */
	int Tile_remove_all_tile_types(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile)
		{
			tile->removeAllTileTypes();
			lua_pushvalue(lua, 1);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns true if the given tile type is in the list of tile types.
	 *
	 * @param TileType tileType The tile type to check for.
	 * @returns Boolean True if the given tile type is in the list.
	 */
	/**
	 * Returns true if the given tile type is in the list of tile types.
	 *
	 * @param String tileTypeName The name of tile type to check for.
	 * @returns Boolean True if the given tile type is in the list.
	 */
	int Tile_has_tile_type(lua_State *lua)
	{
		Tile *tile = castUData<Tile>(lua, 1);
		if (tile)
		{
			TileType *tileType = getTileType(lua, -1);
			if (tileType)
			{
				lua_pushboolean(lua, tile->hasTileType(tileType));
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns an array of all the tile types in this tiles list of tile tpyes.
	 * 
	 * <pre>
	 * Tile, TileType = import("Tile", "TileType")
	 * tile = Tile.new("swamp")
	 * land = TileType.new("land")
	 * water = TileType.new("water")
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
				//L.setTableValue("name", (*iter)->getName());
				wrapObject<TileType>(lua, *iter);
				lua_rawseti(lua, -2, i++);
			}
			return 1;
		}
		lua_pushnil(lua);
		return 1;
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
