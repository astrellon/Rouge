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

#include <lua/wrappers/lua_tile.h>

namespace am {
namespace lua {
namespace game {

	int TileInstance_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 0)
		{
			TileInstance *inst = new TileInstance();
			TileInstance_wrap(lua, inst);
			return 1;
		}
		else if (args == 1)
		{
			Tile *tile = Check_Tile(lua, 1);
			if (tile)
			{
				TileInstance *inst = new TileInstance(tile);
				TileInstance_wrap(lua, inst);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	void TileInstance_wrap(lua_State *lua, TileInstance *inst)
	{
		TileInstance ** udata = (TileInstance **)lua_newuserdata(lua, sizeof(TileInstance *));
		*udata = inst;

		luaL_getmetatable(lua, TileInstance_tableName);
		lua_setmetatable(lua, -2);
	}

	int TileInstance_dtor(lua_State *lua)
	{
		return 0;
	}

	int TileInstance_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", TileInstance_ctor },
			{ "__gc",  TileInstance_dtor },
			{ "get_tile", TileInstance_get_tile },
			{ "set_tile", TileInstance_set_tile },
			{ "set_tile_name", TileInstance_set_tile_name },
			{ "get_tile_frame", TileInstance_get_tile_frame },
			{ "set_tile_frame", TileInstance_set_tile_frame },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, TileInstance_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	TileInstance *Check_TileInstance(lua_State *lua, int n)
	{
		return *(TileInstance **)luaL_checkudata(lua, n, TileInstance_tableName);
	}

	int TileInstance_get_tile(lua_State *lua)
	{
		TileInstance *inst = Check_TileInstance(lua, 1);
		if (inst)
		{
			Tile *tile = inst->getTile();
			if (tile)
			{
				Tile_wrap(lua, tile);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int TileInstance_set_tile(lua_State *lua)
	{
		TileInstance *inst = Check_TileInstance(lua, 1);
		Tile *tile = Check_Tile(lua, 2);
		if (inst && tile)
		{
			inst->setTile(tile);
		}
		return 0;
	}

	int TileInstance_set_tile_name(lua_State *lua)
	{
		TileInstance *inst = Check_TileInstance(lua, 1);
		if (inst && lua_isboolean(lua, -1))
		{
			lua_pushboolean(lua, inst->setTileName(lua_tostring(lua, -1)));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int TileInstance_get_tile_frame(lua_State *lua)
	{
		TileInstance *inst = Check_TileInstance(lua, 1);
		if (inst)
		{
			lua_pushinteger(lua, inst->getTileFrame());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int TileInstance_set_tile_frame(lua_State *lua)
	{
		TileInstance *inst = Check_TileInstance(lua, 1);
		if (inst && lua_isnumber(lua, -1))
		{
			inst->setTileFrame(lua_tointeger(lua, -1));
		}
		return 0;
	}

}
}
}
