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

	int TileType_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 1 && lua_isstring(lua, -1))
		{
			TileType *tile = new TileType(lua_tostring(lua, -1));
			wrapObject<TileType>(lua, tile);
			return 1;
		}
		else if (args == 2 && lua_isstring(lua, -2) && lua_isstring(lua, -1))
		{
			TileType *tile = new TileType(lua_tostring(lua, -2), lua_tostring(lua, -1));
			wrapObject<TileType>(lua, tile);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int TileType_dtor(lua_State *lua)
	{
		TileType *type = castUData<TileType>(lua, 1);
		if (type)
		{
			delete type;
		}
		return 0;
	}

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
			{ "get_name", TileType_get_name },
			{ "get_full_name", TileType_get_full_name },
			{ "set_full_name", TileType_set_full_name },
			{ "load_def", NULL },
			// Static TileType methods
			{ "add_tile_type", TileType_add_tile_type },
			{ "get_tile_type", TileType_get_tile_type },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, TileType::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	int TileType_get_name(lua_State *lua)
	{
		TileType *tile = castUData<TileType>(lua, 1);
		if (tile)
		{
			lua_pushstring(lua, tile->getName());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int TileType_get_full_name(lua_State *lua)
	{
		TileType *tile = castUData<TileType>(lua, 1);
		if (tile)
		{
			lua_pushstring(lua, tile->getFullName());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int TileType_set_full_name(lua_State *lua)
	{
		TileType *tile = castUData<TileType>(lua, 1);
		if (tile && lua_isstring(lua, -1))
		{
			tile->setFullName(lua_tostring(lua, -1));
		}
		return 0;
	}

	int TileType_add_tile_type(lua_State *lua)
	{
		TileType *tile = castUData<TileType>(lua, 1);
		if (tile)
		{
			Engine::getEngine()->addTileType(tile);
		}
		return 0;
	}
	int TileType_get_tile_type(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			TileType *tile = Engine::getEngine()->getTileType(lua_tostring(lua, -1));
			if (tile)
			{
				wrapObject<TileType>(lua, tile);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
