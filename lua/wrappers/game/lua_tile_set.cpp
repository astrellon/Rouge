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

	int TileSet_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 1 && lua_isstring(lua, -1))
		{
			TileSet *tile = new TileSet(lua_tostring(lua, -1));
			TileSet_wrap(lua, tile);
			return 1;
		}
		TileSet *tile = new TileSet();
		TileSet_wrap(lua, tile);
		return 1;
	}
	void TileSet_wrap(lua_State *lua, TileSet *tile)
	{
		TileSet ** udata = (TileSet **)lua_newuserdata(lua, sizeof(TileSet *));
		*udata = tile;

		tile->retain();

		luaL_getmetatable(lua, TileSet_tableName);
		lua_setmetatable(lua, -2);
	}

	int TileSet_dtor(lua_State *lua)
	{
		TileSet *set = Check_TileSet(lua, 1);
		if (set)
		{
			set->release();
		}
		return 0;
	}

	int TileSet_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", TileSet_ctor },
			{ "__gc",  TileSet_dtor },
			{ "set_name", TileSet_set_name },
			{ "get_name", TileSet_get_name },
			{ "set_full_name", TileSet_set_full_name },
			{ "get_full_name", TileSet_get_full_name },
			{ "add_tile", TileSet_add_tile },
			{ "remove_tile", TileSet_remove_tile },
			{ "has_tile", TileSet_has_tile },
			{ "get_tile", TileSet_get_tile },
			{ "load_def", TileSet_load_def },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, TileSet_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	TileSet *Check_TileSet(lua_State *lua, int n)
	{
		return *(TileSet **)luaL_checkudata(lua, n, TileSet_tableName);
	}

	int TileSet_set_name(lua_State *lua)
	{
		TileSet *set = Check_TileSet(lua, 1);
		if (set && lua_isstring(lua, -1))
		{
			set->setName(lua_tostring(lua, -1));
		}
		return 0;
	}
	int TileSet_get_name(lua_State *lua)
	{
		TileSet *set = Check_TileSet(lua, 1);
		if (set)
		{
			lua_pushstring(lua, set->getName().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int TileSet_set_full_name(lua_State *lua)
	{
		TileSet *set = Check_TileSet(lua, 1);
		if (set && lua_isstring(lua, -1))
		{
			set->setFullName(lua_tostring(lua, -1));
		}
		return 0;
	}
	int TileSet_get_full_name(lua_State *lua)
	{
		TileSet *set = Check_TileSet(lua, 1);
		if (set)
		{
			lua_pushstring(lua, set->getFullName().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int TileSet_add_tile(lua_State *lua)
	{
		TileSet *set = Check_TileSet(lua, 1);
		Tile *tile = Check_Tile(lua, 2);
		if (set && tile)
		{
			set->addTile(tile);
		}
		return 0;
	}
	int TileSet_remove_tile(lua_State *lua)
	{
		TileSet *set = Check_TileSet(lua, 1);
		if (set)
		{
			if (lua_isstring(lua, -1))
			{
				set->removeTile(lua_tostring(lua, -1));
				return 0;
			}
			Tile *tile = Check_Tile(lua, 2);
			if (tile)
			{
				set->removeTile(tile);
			}
		}
		return 0;
	}
	int TileSet_has_tile(lua_State *lua)
	{
		TileSet *set = Check_TileSet(lua, 1);
		if (set)
		{
			if (lua_isstring(lua, -1))
			{
				lua_pushboolean(lua, set->hasTile(lua_tostring(lua, -1)));
				return 1;
			}
			Tile *tile = Check_Tile(lua, 2);
			if (tile)
			{
				lua_pushboolean(lua, set->hasTile(tile));
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int TileSet_get_tile(lua_State *lua)
	{
		TileSet *set = Check_TileSet(lua, 1);
		if (set && lua_isstring(lua, -1))
		{
			Tile_wrap(lua, set->getTile(lua_tostring(lua, -1)));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int TileSet_load_def(lua_State *lua)
	{
		TileSet *set = Check_TileSet(lua, 1);
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
