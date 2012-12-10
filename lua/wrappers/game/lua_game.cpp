#include "lua_game.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/game.h>
#include <game/engine.h>
#include <game/map.h>
using namespace am::game;

#include "lua_map.h"
#include "lua_character.h"

namespace am {
namespace lua {
namespace game {

	int Game_ctor(lua_State *lua)
	{
		Game *game = new Game(Engine::getEngine());
		
		Game_wrap(lua, game);
		return 1;
	}
	void Game_wrap(lua_State *lua, Game *game)
	{
		Game ** udata = (Game **)lua_newuserdata(lua, sizeof(Game *));
		*udata = game;

		game->retain();

		luaL_getmetatable(lua, Game_tableName);
		lua_setmetatable(lua, -2);
	}

	int Game_dtor(lua_State *lua)
	{
		Game *game = Check_Game(lua, 1);
		if (game)
		{
			game->release();
		}
		return 0;
	}

	int Game_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Game_ctor },
			{ "__gc", Game_dtor },
			{ "get_map", Game_get_map },
			{ "get_current_map", Game_get_current_map },
			{ "set_current_map", Game_set_current_map },
			{ "add_game_object", Game_add_game_object },
			{ "remove_game_object", Game_remove_game_object },
			{ "has_game_object", Game_has_game_object },
			{ "move_object_to_map", Game_move_object_to_map },
			{ "move_object_to_map_grid", Game_move_object_to_map_grid },
			{ "get_main_character", Game_get_main_character },
			{ "set_main_character", Game_set_main_character },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Game_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	Game *Check_Game(lua_State *lua, int n)
	{
		return *(Game **)luaL_checkudata(lua, n, Game_tableName);
	}

	int Game_get_map(lua_State *lua)
	{
		Game *game = Check_Game(lua, 1);
		if (game && lua_tostring(lua, -1))
		{
			Map *map = game->getMapLua(lua_tostring(lua, -1));
			if (map)
			{
				Map_wrap(lua, map);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int Game_get_current_map(lua_State *lua)
	{
		Game *game = Check_Game(lua, 1);
		if (game)
		{
			Map *map = game->getCurrentMap();
			if (map)
			{
				Map_wrap(lua, map);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int Game_set_current_map(lua_State *lua)
	{
		Game *game = Check_Game(lua, 1);
		if (game)
		{
			if (lua_isstring(lua, -1))
			{
				game->setCurrentMap(lua_tostring(lua, -1));
			}
			else
			{
				Map *map = Check_Map(lua, 2);
				game->setCurrentMap(map);
			}
		}
		return 0;
	}

	int Game_add_game_object(lua_State *lua)
	{
		Game *game = Check_Game(lua, 1);
		if (game && lua_isuserdata(lua, -1))
		{
			GameObject *obj = reinterpret_cast<GameObject *>(lua_touserdata(lua, -1));
			lua_pushboolean(lua, game->addGameObject(obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Game_remove_game_object(lua_State *lua)
	{
		Game *game = Check_Game(lua, 1);
		if (game && lua_isuserdata(lua, -1))
		{
			GameObject *obj = reinterpret_cast<GameObject *>(lua_touserdata(lua, -1));
			lua_pushboolean(lua, game->removeGameObject(obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Game_has_game_object(lua_State *lua)
	{
		Game *game = Check_Game(lua, 1);
		if (game && lua_isuserdata(lua, -1))
		{
			GameObject *obj = reinterpret_cast<GameObject *>(lua_touserdata(lua, -1));
			lua_pushboolean(lua, game->hasGameObject(obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Game_move_object_to_map(lua_State *lua)
	{
		Game *game = Check_Game(lua, 1);
		if (game)
		{
			GameObject *obj = reinterpret_cast<GameObject *>(lua_touserdata(lua, 2));
			if (obj)
			{
				Map *map = NULL;
				if (lua_isstring(lua, 3))
				{
					map = game->getMapLua(lua_tostring(lua, 3));
				}
				else if (lua_isuserdata(lua ,3))
				{
					map = Check_Map(lua, 3);
				}
				if (map && lua_isnumber(lua, 4) && lua_isnumber(lua, 5))
				{
					int args = lua_gettop(lua);
					if (args == 6 && lua_isboolean(lua, 6))
					{
						game->moveObjectToMap(obj, map, lua_tofloat(lua, 4), lua_tofloat(lua, 5), lua_tobool(lua, 6));
					}
					else
					{
						game->moveObjectToMap(obj, map, lua_tofloat(lua, 4), lua_tofloat(lua, 5));
					}
				}
			}
		}
		return 0;
	}
	int Game_move_object_to_map_grid(lua_State *lua)
	{
		Game *game = Check_Game(lua, 1);
		if (game)
		{
			GameObject *obj = reinterpret_cast<GameObject *>(lua_touserdata(lua, 2));
			if (obj)
			{
				Map *map = NULL;
				if (lua_isstring(lua, 3))
				{
					map = game->getMapLua(lua_tostring(lua, 3));
				}
				else if (lua_isuserdata(lua ,3))
				{
					map = Check_Map(lua, 3);
				}
				if (map && lua_isnumber(lua, 4) && lua_isnumber(lua, 5))
				{
					int args = lua_gettop(lua);
					if (args == 6 && lua_isboolean(lua, 6))
					{
						game->moveObjectToMapGrid(obj, map, lua_tointeger(lua, 4), lua_tointeger(lua, 5), lua_toboolean(lua, 6) > 0);
					}
					else
					{
						game->moveObjectToMapGrid(obj, map, lua_tointeger(lua, 4), lua_tointeger(lua, 5));
					}
				}
			}
		}
		return 0;
	}

	int Game_get_main_character(lua_State *lua)
	{
		Game *game = Check_Game(lua, 1);
		if (game)
		{
			Character *character = game->getMainCharacter();
			if (character)
			{
				Character_wrap(lua, character);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int Game_set_main_character(lua_State *lua)
	{
		Game *game = Check_Game(lua, 1);
		Character *character = Check_Character(lua, 2);
		if (game && character)
		{
			game->setMainCharacter(character);
		}
		return 0;
	}

}
}
}
