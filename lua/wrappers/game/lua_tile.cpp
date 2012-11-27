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
using namespace am::game;

#include <util/json_value.h>
using namespace am::util;

#include "lua_tile_type.h"
#include "lua_tile_set.h"

namespace am {
namespace lua {
namespace game {

	int Tile_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 1 && lua_isstring(lua, -1))
		{
			Tile *tile = new Tile(lua_tostring(lua, -1));
			Tile_wrap(lua, tile);
			return 1;
		}
		else if (args == 2 && lua_isstring(lua, -2) && lua_isstring(lua, -1))
		{
			Tile *tile = new Tile(lua_tostring(lua, -2), lua_tostring(lua, -1));
			Tile_wrap(lua, tile);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	void Tile_wrap(lua_State *lua, Tile *tile)
	{
		Tile ** udata = (Tile **)lua_newuserdata(lua, sizeof(Tile *));
		*udata = tile;

		tile->retain();

		luaL_getmetatable(lua, Tile_tableName);
		lua_setmetatable(lua, -2);
	}

	int Tile_dtor(lua_State *lua)
	{
		Tile *tile = Check_Tile(lua, 1);
		if (tile)
		{
			tile->release();
		}
		return 0;
	}

	int Tile_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Tile_ctor },
			{ "__gc",  Tile_dtor },
			{ "get_name", Tile_get_name },
			{ "set_name", Tile_set_name },
			{ "get_full_name", Tile_get_full_name },
			{ "set_full_name", Tile_set_full_name },
			{ "get_description", Tile_get_description },
			{ "set_description", Tile_set_description },
			{ "get_tile_set", Tile_get_tile_set },
			{ "set_tile_set", Tile_set_tile_set },
			{ "add_tile_type", Tile_add_tile_type },
			{ "remove_tile_type", Tile_remove_tile_type },
			{ "remove_all_tile_types", Tile_remove_all_tile_types },
			{ "has_tile_type", Tile_has_tile_type },
			{ "get_tile_types", NULL },
			{ "load_def", Tile_load_def },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Tile_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	Tile *Check_Tile(lua_State *lua, int n)
	{
		return *(Tile **)luaL_checkudata(lua, n, Tile_tableName);
	}

	int Tile_get_name(lua_State *lua)
	{
		Tile *tile = Check_Tile(lua, 1);
		if (tile)
		{
			lua_pushstring(lua, tile->getName().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Tile_set_name(lua_State *lua)
	{
		Tile *tile = Check_Tile(lua, 1);
		if (tile && lua_isstring(lua, -1))
		{
			tile->setName(lua_tostring(lua, -1));
		}
		return 0;
	}

	int Tile_get_full_name(lua_State *lua)
	{
		Tile *tile = Check_Tile(lua, 1);
		if (tile)
		{
			lua_pushstring(lua, tile->getFullName().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Tile_set_full_name(lua_State *lua)
	{
		Tile *tile = Check_Tile(lua, 1);
		if (tile && lua_isstring(lua, -1))
		{
			tile->setFullName(lua_tostring(lua, -1));
		}
		return 0;
	}

	int Tile_get_description(lua_State *lua)
	{
		Tile *tile = Check_Tile(lua, 1);
		if (tile)
		{
			lua_pushstring(lua, tile->getDescription().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Tile_set_description(lua_State *lua)
	{
		Tile *tile = Check_Tile(lua, 1);
		if (tile && lua_isstring(lua, -1))
		{
			tile->setDescription(lua_tostring(lua, -1));
		}
		return 0;
	}

	int Tile_get_tile_set(lua_State *lua)
	{
		Tile *tile = Check_Tile(lua, 1);
		if (tile && tile->getTileSet())
		{
			TileSet_wrap(lua, tile->getTileSet());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Tile_set_tile_set(lua_State *lua)
	{
		Tile *tile = Check_Tile(lua, 1);
		TileSet *set = Check_TileSet(lua, 2);
		if (tile && set)
		{
			tile->setTileSet(set);
		}
		return 0;
	}

	int Tile_add_tile_type(lua_State *lua)
	{
		Tile *tile = Check_Tile(lua, 1);
		if (tile)
		{
			TileType *tileType = getTileType(lua, -1);
			if (tileType)
			{
				tile->addTileType(tileType);
			}
		}
		return 0;
	}
	int Tile_remove_tile_type(lua_State *lua)
	{
		Tile *tile = Check_Tile(lua, 1);
		if (tile)
		{
			TileType *tileType = getTileType(lua, -1);
			if (tileType)
			{
				tile->removeTileType(tileType);
			}
		}
		return 0;
	}
	int Tile_remove_all_tile_types(lua_State *lua)
	{
		Tile *tile = Check_Tile(lua, 1);
		if (tile)
		{
			tile->removeAllTileTypes();
		}
		return 0;
	}
	int Tile_has_tile_type(lua_State *lua)
	{
		Tile *tile = Check_Tile(lua, 1);
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

	int Tile_load_def(lua_State *lua)
	{
		Tile *tile = Check_Tile(lua, 1);
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
			tileType = TileType::getTileType(lua_tostring(lua, n));
		}
		else
		{
			tileType = Check_TileType(lua, n);
		}
		return tileType;
	}

}
}
}
