#include "lua_tile_type.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/tile_type.h>
#include <game/engine.h>
using namespace am::game;

#include <util/json_value.h>
using namespace am::util;

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * The tile type class represents the type of tile, such as land, water, lava, etc.
	 * These types can be added to tiles so that a tile can have multiple types.
	 * Each game object also contains a list of tile types which it can
	 * pass through.
	 */
	/**
	 * Creates a new tile type with the given name, this name should be unique
	 * to all tile types.
	 *
	 * @param string tile_type_name The name of the tile type.
	 */
	/**
	 * Creates a new tile type with the given name and full name. This name
	 * should be unique to all tile types. The full name is used to display
	 * the name of this tile type to the user.
	 *
	 * @param string tile_type_name The name of the tile type.
	 * @param string full_name The full name of this tile type.
	 */
	int TileType_ctor(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			TileType *tile = NULL;
			if (lua_isstr(lua, 2))
			{
				tile = new TileType(lua_tostring(lua, 1), lua_tostring(lua, 2));
			}
			else
			{
				tile = new TileType(lua_tostring(lua, 1));
			}
			if (tile)
			{
				wrapRefObject<TileType>(lua, tile);
				return 1;
			}
		}
		return LuaState::expectedArgs(lua, "@new", 2, "string tile_type_name", "string tile_type_name, string full_name");
	}
	/**
	 * Releases the reference counter on the tile type.
	 */
	int TileType_dtor(lua_State *lua)
	{
		TileType *type = castUData<TileType>(lua, 1);
		if (type)
		{
			type->release();
			return 0;
		}
		return LuaState::expectedContext(lua, "__gc", "am.tile_type");
	}
	/**
	 * Compares this tile type against another tile type.
	 *
	 * @param am.tile_type rhs The other tile type to compare with.
	 * @returns boolean Returns true if they are the same object.
	 */
	int TileType_eq(lua_State *lua)
	{
		TileType *lhs = castUData<TileType>(lua, 1);
		TileType *rhs = castUData<TileType>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int TileType_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", TileType_ctor },
			{ "__gc", TileType_dtor },
			{ "__eq", TileType_eq },
			{ "name", TileType_name },
			{ "full_name", TileType_full_name },
			{ "load_def", NULL },
			// Static TileType methods
			{ "add_tile_type", TileType_add_tile_type },
			{ "tile_type", TileType_tile_type },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, TileType::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	/**
	 * Returns the name of this tile type.
	 *
	 * @returns string This tile type's name.
	 */
	int TileType_name(lua_State *lua)
	{
		TileType *tile = castUData<TileType>(lua, 1);
		if (tile)
		{
			lua_pushstring(lua, tile->getName());
			return 1;
		}
		return LuaState::expectedContext(lua, "name", "am.tile_type");
	}
	/**
	 * Returns the full name of this tile type.
	 *
	 * @returns string This tile type's full name.
	 */
	/**
	 * Sets the full name of the tile type.
	 * This is used to display the name of the tile type to the user.
	 *
	 * @param string full_name The full name of the tile type.
	 * @returns am.tile_type This
	 */
	int TileType_full_name(lua_State *lua)
	{
		TileType *tile = castUData<TileType>(lua, 1);
		if (tile)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, tile->getFullName());
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				tile->setFullName(lua_tostring(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "full_name", "string full_name");
		}
		return LuaState::expectedContext(lua, "full_name", "am.tile_type");
	}
	/**
	 * @static
	 * Register a tile type with the game engine.
	 *
	 * @param am.tile_type tile_type The tile type to register.
	 */
	int TileType_add_tile_type(lua_State *lua)
	{
		TileType *tile = castUData<TileType>(lua, 1);
		if (tile)
		{
			Engine::getEngine()->addTileType(tile);
			return 0;
		}
		return LuaState::expectedArgs(lua, "@add_tile_type", "am.tile_type tile_type");
	}
	/**
	 * @static
	 * Looks up a tile type with the given name.
	 *
	 * @param string tile_type_name The name of the tile type to look up.
	 * @returns am.tile_type The found tile type or nil.
	 */
	int TileType_tile_type(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			TileType *tile = Engine::getEngine()->getTileType(lua_tostring(lua, 1));
			if (tile)
			{
				wrapRefObject<TileType>(lua, tile);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@tile_type", "string tile_type_name");
	}

}
}
}
