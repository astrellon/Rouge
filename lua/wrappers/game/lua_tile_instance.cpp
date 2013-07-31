#include "lua_tile_instance.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/tile_instance.h>
#include <game/tile.h>
using namespace am::game;

#include "lua_tile.h"

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * The tile instance class represents where a single instance of a tile
	 * with an extra data associated. Currently this is only which tile frame to render.
	 */
	/**
	 * Creates a new tile instance, it will need to have a tile associated with it
	 * before it can be used to render.
	 */
	/**
	 * Creates a new tile instance with the given tile.
	 *
	 * @param am.tile tile The tile this instance.
	 */
	int TileInstance_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 0)
		{
			TileInstance *inst = new TileInstance();
			wrapObject<TileInstance>(lua, inst);
			return 1;
		}
		else if (args == 1)
		{
			Tile *tile = castUData<Tile>(lua, 1);
			if (tile)
			{
				TileInstance *inst = new TileInstance(tile);
				wrapObject<TileInstance>(lua, inst);
				return 1;
			}
		}
		return LuaState::expectedArgs(lua, "@new", 2, "", "am.tile tile");
	}
	/**
	 * TODO
	 */
	int TileInstance_dtor(lua_State *lua)
	{
		return 0;
	}

	/**
	 * Compares this tile instance with another tile instance.
	 *
	 * @param am.tile_instance rhs The other tile instance to compare with.
	 * @returns boolean True if they are the same tile instance objects.
	 */
	int TileInstance_eq(lua_State *lua)
	{
		TileInstance *lhs = castUData<TileInstance>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.tile_instance");
		}
		TileInstance *rhs = castUData<TileInstance>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int TileInstance_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", TileInstance_ctor },
			{ "__gc", TileInstance_dtor },
			{ "__eq", TileInstance_eq },
			{ "tile", TileInstance_tile },
			{ "tile_frame", TileInstance_tile_frame },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, TileInstance::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	/**
	 * Returns the tile this instance is representing, can be nil.
	 *
	 * @returns am.tile The tile this instance is instancing.
	 */
	/**
	 * Sets the tile this instance representings, can be nil.
	 *
	 * @param am.tile tile The tile to represent.
	 * @returns boolean Always returns true.
	 */
	/**
	 * Sets the tile based on the tile name, returns true if the tile
	 * with given name was found.
	 *
	 * @param string tile_name The name of the tile to instance.
	 * @returns boolean True if the a tile with the given name was found.
	 */
	int TileInstance_tile(lua_State *lua)
	{
		TileInstance *inst = castUData<TileInstance>(lua, 1);
		if (inst)
		{
			if (lua_gettop(lua) == 1)
			{
				Tile *tile = inst->getTile();
				if (tile)
				{
					wrapRefObject<Tile>(lua, tile);
					return 1;
				}
			}
			else if (lua_isstr(lua, 2))
			{
				lua_pushboolean(lua, inst->setTileName(lua_tostring(lua, 2)));
				return 1;
			}
			Tile *tile = castUData<Tile>(lua, 2);
			if (tile)
			{
				inst->setTile(tile);
				lua_pushboolean(lua, 1);
				return 1;
			}
			return LuaState::expectedArgs(lua, "tile", 2, "am.tile tile", "string tile_name");
		}
		return LuaState::expectedContext(lua, "tile", "am.tile_instance");
	}
	
	/**
	 * Returns the current tile frame.
	 *
	 * @returns integer The current tile frame.
	 */
	/**
	 * Sets the current tile frame. This frame refers to the frame in the tile graphic.
	 * This will only work for tile graphics that have multiple frames and are not animated.
	 *
	 * @param integer frame The new tile instance frame.
	 * @returns am.tile_instance This
	 */
	int TileInstance_tile_frame(lua_State *lua)
	{
		TileInstance *inst = castUData<TileInstance>(lua, 1);
		if (inst)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushinteger(lua, inst->getTileFrame());
				return 1;
			}
			else if (lua_isnum(lua, 2))
			{
				inst->setTileFrame(lua_tointeger(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "tile_frame", "integer frame");
		}
		return LuaState::expectedContext(lua, "tile_frame", "am.tile_instance");
	}

}
}
}
