#include "lua_engine.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/engine.h>
using namespace am::game;

#include <lua/wrappers/lua_tile.h>
#include <lua/wrappers/lua_tile_set.h>
#include <lua/wrappers/lua_game.h>

namespace am {
namespace lua {
namespace game {

	int Engine_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "set_current_game", Engine_set_current_game },
			{ "get_current_game", Engine_get_current_game },
			{ "set_grid_size", Engine_set_grid_size },
			{ "get_grid_size", Engine_get_grid_size },
			{ "get_tile", Engine_get_tile },
			{ "get_tile_set", Engine_get_tile_set },
			{ "add_tile_set", Engine_add_tile_set },
			{ "get_top_level_tile_set", Engine_get_top_level_tile_set },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Engine_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	int Engine_set_current_game(lua_State *lua)
	{
		Game *game = Check_Game(lua, 1);
		if (game)
		{
			Engine::getEngine()->setCurrentGame(game);
		}
		return 0;
	}
	int Engine_get_current_game(lua_State *lua)
	{
		Game *game = Engine::getEngine()->getCurrentGame();
		if (game)
		{
			Game_wrap(lua, game);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Engine_set_grid_size(lua_State *lua)
	{
		if (lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			Engine::getEngine()->setGridXSize(static_cast<float>(lua_tonumber(lua, -2)));
			Engine::getEngine()->setGridYSize(static_cast<float>(lua_tonumber(lua, -1)));
		}
		return 0;
	}
	int Engine_get_grid_size(lua_State *lua)
	{
		lua_pushnumber(lua, Engine::getEngine()->getGridXSize());
		lua_pushnumber(lua, Engine::getEngine()->getGridYSize());
		return 2;
	}

	int Engine_get_tile(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			Tile *tile = Engine::getEngine()->getTile(lua_tostring(lua, -1));
			if (tile)
			{
				Tile_wrap(lua, tile);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int Engine_get_tile_set(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			TileSet *set = Engine::getEngine()->getTileSetLua(lua_tostring(lua, -1));
			TileSet_wrap(lua, set);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Engine_add_tile_set(lua_State *lua)
	{
		TileSet *set = Check_TileSet(lua, 1);
		if (set)
		{
			Engine::getEngine()->addTileSet(set);
		}
		return 0;
	}
	int Engine_get_top_level_tile_set(lua_State *lua)
	{
		TileSet_wrap(lua, Engine::getEngine()->getTopLevelTileSet());
		return 1;
	}

}
}
}
