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

	int Map_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 1 && lua_isstring(lua, -1))
		{
			Map *map = new Map(lua_tostring(lua, -1));
			Map_wrap(lua, map);
			return 1;
		}
		else if (args == 3 && lua_isstring(lua, -3) && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			Map *map = new Map(lua_tostring(lua, -3), lua_tointeger(lua, -2), lua_tointeger(lua, -1));
			Map_wrap(lua, map);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	void Map_wrap(lua_State *lua, Map *map)
	{
		Map ** udata = (Map **)lua_newuserdata(lua, sizeof(Map *));
		*udata = map;

		map->retain();

		luaL_getmetatable(lua, Map_tableName);
		lua_setmetatable(lua, -2);
	}

	int Map_dtor(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
		if (map)
		{
			map->release();
		}
		return 0;
	}

	int Map_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Map_ctor },
			{ "__gc",  Map_dtor },
			{ "set_name", Map_set_name },
			{ "get_name", Map_get_name },
			{ "set_full_name", Map_set_full_name },
			{ "get_full_name", Map_get_full_name },
			{ "get_tile", Map_get_tile },
			{ "get_tile_instance", Map_get_tile_instance },
			{ "set_tiles", Map_set_tiles },
			//{ "get_tiles", NULL },
			{ "set_map_size", Map_set_map_size },
			{ "get_map_size", Map_get_map_size },
			{ "get_objects", NULL },
			{ "add_game_object", Map_add_game_object },
			{ "remove_game_object", Map_remove_game_object },
			{ "has_game_object", Map_has_game_object },
			{ "is_valid_location", Map_is_valid_location },
			{ "is_valid_grid_location", Map_is_valid_grid_location },
			{ "load_def", Map_load_def },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Map_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	Map *Check_Map(lua_State *lua, int n)
	{
		return *(Map **)luaL_checkudata(lua, n, Map_tableName);
	}

	int Map_set_name(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
		if (map && lua_isstring(lua, -1))
		{
			map->setName(lua_tostring(lua, -1));
		}
		return 0;
	}
	int Map_get_name(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
		if (map)
		{
			lua_pushstring(lua, map->getName().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Map_set_full_name(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
		if (map && lua_isstring(lua, -1))
		{
			map->setFullName(lua_tostring(lua, -1));
		}
		return 0;
	}
	int Map_get_full_name(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
		if (map)
		{
			lua_pushstring(lua, map->getFullName().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Map_get_tile(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
		if (map && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			Tile *tile = map->getTile(lua_tointeger(lua, -2), lua_tointeger(lua, -1));
			if (tile)
			{
				Tile_wrap(lua, tile);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int Map_get_tile_instance(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
		if (map && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			TileInstance *inst = map->getTileInstance(lua_tointeger(lua, -2), lua_tointeger(lua, -1));
			if (inst)
			{
				TileInstance_wrap(lua, inst);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	int Map_set_tiles(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
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
					Tile *tile = Check_Tile(lua, -1);
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
		}
		return 0;
	}

	int Map_set_map_size(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
		if (map && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			map->setMapSize(lua_tointeger(lua, -2), lua_tointeger(lua, -1));
		}
		return 0;
	}
	int Map_get_map_size(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
		if (map)
		{
			lua_pushinteger(lua, map->getMapWidth());
			lua_pushinteger(lua, map->getMapHeight());
			return 2;
		}
		lua_pushnil(lua);
		lua_pushnil(lua);
		return 2;
	}

	int Map_add_game_object(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
		if (map)
		{
			GameObject *obj = getGameObject(lua, -1);
			lua_pushboolean(lua, map->addGameObject(obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Map_remove_game_object(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
		if (map)
		{
			GameObject *obj = getGameObject(lua, -1);
			lua_pushboolean(lua, map->removeGameObject(obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Map_has_game_object(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
		if (map)
		{
			GameObject *obj = getGameObject(lua, -1);
			lua_pushboolean(lua, map->hasGameObject(obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Map_is_valid_location(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
		if (map && lua_isnumber(lua, -3) && lua_isnumber(lua, -2))
		{
			GameObject *obj = getGameObject(lua, -1);
			if (obj)
			{
				map->isValidLocation(lua_tofloat(lua, -3), lua_tofloat(lua, -2), obj);
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int Map_is_valid_grid_location(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
		if (map && lua_isnumber(lua, -3) && lua_isnumber(lua, -2))
		{
			GameObject *obj = getGameObject(lua, -1);
			if (obj)
			{
				map->isValidGridLocation(lua_tointeger(lua, -3), lua_tointeger(lua, -2), obj);
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	int Map_load_def(lua_State *lua)
	{
		Map *map = Check_Map(lua, 1);
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
