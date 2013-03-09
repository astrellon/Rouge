#include "lua_tile_set.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/tile.h>
#include <game/tile_set.h>
using namespace am::game;

#include "lua_tile.h"

namespace am {
namespace lua {
namespace game {

	/**
	 * @class
	 * A tile set holds a collection of tiles. The tiles can then be refered
	 * to by their tile set name and by their individual name. This allows for
	 * multiple tiles to have the same name, as long as they are in different
	 * tile sets.
	 */
	/**
	 * Creates a new tile set with the given name.
	 *
	 * @param string name The tile set name.
	 */
	int TileSet_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 1 && lua_type(lua, -1) == LUA_TSTRING)
		{
			TileSet *tile = new TileSet(lua_tostring(lua, -1));
			wrapRefObject<TileSet>(lua, tile);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Releases the reference counter on this tile set.
	 */
	int TileSet_dtor(lua_State *lua)
	{
		TileSet *set = castUData<TileSet>(lua, 1);
		if (set)
		{
			set->release();
		}
		return 0;
	}
	/**
	 * Compares this tile set with another tile set.
	 *
	 * @param TileSet rhs The other tile set to compare with.
	 * @returns boolean True if they are the same tile set object.
	 */
	int TileSet_eq(lua_State *lua)
	{
		TileSet *lhs = castUData<TileSet>(lua, 1);
		TileSet *rhs = castUData<TileSet>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int TileSet_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", TileSet_ctor },
			{ "__gc", TileSet_dtor },
			{ "__eq", TileSet_eq },
			{ "name", TileSet_name },
			{ "full_name", TileSet_full_name },
			{ "add_tile", TileSet_add_tile },
			{ "remove_tile", TileSet_remove_tile },
			{ "has_tile", TileSet_has_tile },
			{ "tile", TileSet_tile },
			{ "load_def", TileSet_load_def },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, TileSet::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	
	/**
	 * Returns the name of this tile set.
	 *
	 * @returns string The name of this tile set.
	 */
	int TileSet_name(lua_State *lua)
	{
		TileSet *set = castUData<TileSet>(lua, 1);
		if (set)
		{
			lua_pushstring(lua, set->getName().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the full name of the tile set. This is used to display
	 * the tile set name to the user.
	 *
	 * @returns string The full tile set name.
	 */
	/**
	 * Sets the full name o the tile set. This is used to display
	 * the tile set name to the user.
	 *
	 * @param string fullName The new full name of the tile set.
	 * @returns TileSet This
	 */
	int TileSet_full_name(lua_State *lua)
	{
		TileSet *set = castUData<TileSet>(lua, 1);
		if (set)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, set->getFullName().c_str());
				return 1;
			}
			else if (lua_type(lua, -1) == LUA_TSTRING)
			{
				set->setFullName(lua_tostring(lua, -1));
				lua_first(lua);
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Adds a tile to this tile set.
	 *
	 * @param Tile tile The tile to add.
	 * @returns TileSet This
	 */
	int TileSet_add_tile(lua_State *lua)
	{
		TileSet *set = castUData<TileSet>(lua, 1);
		Tile *tile = castUData<Tile>(lua, 2);
		if (set && tile)
		{
			set->addTile(tile);
			lua_first(lua);
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Removes a tile from this tile set.
	 *
	 * @param string tileName The name of the tile to remove.
	 * @returns TileSet This
	 */
	/**
	 * Removes a tile from this tile set.
	 *
	 * @param Tile tile The tile to remove.
	 * @returns TileSet This
	 */
	int TileSet_remove_tile(lua_State *lua)
	{
		TileSet *set = castUData<TileSet>(lua, 1);
		if (set)
		{
			if (lua_type(lua, -1) == LUA_TSTRING)
			{
				set->removeTile(lua_tostring(lua, -1));
				lua_first(lua);
			}
			Tile *tile = castUData<Tile>(lua, 2);
			if (tile)
			{
				set->removeTile(tile);
			}
			lua_first(lua);
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns true if there is a tile with the given name in the tile set.
	 *
	 * @param string tileName The name of the tile to look for.
	 * @returns boolean True if the tile was found.
	 */
	/**
	 * Retruns true if the given tile was found in this tile set.
	 *
	 * @param Tile tile The tile to look for.
	 * @returns boolean True if the tile was found.
	 */
	int TileSet_has_tile(lua_State *lua)
	{
		TileSet *set = castUData<TileSet>(lua, 1);
		if (set)
		{
			if (lua_type(lua, -1) == LUA_TSTRING)
			{
				lua_pushboolean(lua, set->hasTile(lua_tostring(lua, -1)));
				return 1;
			}
			Tile *tile = castUData<Tile>(lua, 2);
			if (tile)
			{
				lua_pushboolean(lua, set->hasTile(tile));
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Looks for a tile in this tile set with the given name.
	 *
	 * @param string tileName The name of the tile to look for.
	 * @returns Tile The found tile, or nil.
	 */
	int TileSet_tile(lua_State *lua)
	{
		TileSet *set = castUData<TileSet>(lua, 1);
		if (set && lua_type(lua, -1) == LUA_TSTRING)
		{
			wrapRefObject<Tile>(lua, set->getTile(lua_tostring(lua, -1)));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	/**
	 * @private
	 */
	int TileSet_load_def(lua_State *lua)
	{
		TileSet *set = castUData<TileSet>(lua, 1);
		if (set && lua_istable(lua, -1))
		{
			LuaState wrap(lua);
			set->loadDef(wrap);
		}
		return 0;
	}

}
}
}
