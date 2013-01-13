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
#include <game/character.h>
#include <game/item.h>
#include <game/dialogue.h>
#include <game/quest.h>
using namespace am::game;

#include "lua_map.h"
#include "lua_character.h"
#include "lua_item.h"
#include "lua_dialogue.h"
#include "lua_quest.h"

namespace am {
namespace lua {
namespace game {

	int Game_ctor(lua_State *lua)
	{
		Game *game = new Game(Engine::getEngine());
		
		wrapRefObject<Game>(lua, game);
		return 1;
	}

	int Game_dtor(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			game->release();
		}
		return 0;
	}

	int Game_eq(lua_State *lua)
	{
		Game *lhs = castUData<Game>(lua, 1);
		Game *rhs = castUData<Game>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Game_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Game_ctor },
			{ "__gc", Game_dtor },
			{ "__eq", Game_eq },
			{ "add_map", Game_add_map },
			{ "remove_map", Game_remove_map },
			{ "remove_all_maps", Game_remove_all_maps },
			{ "get_map", Game_get_map },
			{ "get_current_map", Game_get_current_map },
			{ "set_current_map", Game_set_current_map },
			{ "add_game_object_to_map", Game_add_game_object_to_map },
			{ "remove_game_object_from_map", Game_remove_game_object_from_map },
			{ "has_game_object_in_map", Game_has_game_object_in_map },
			{ "move_object_to_map", Game_move_object_to_map },
			{ "move_object_to_map_grid", Game_move_object_to_map_grid },
			{ "get_main_character", Game_get_main_character },
			{ "set_main_character", Game_set_main_character },
			// GameObject
			{ "get_game_object", Game_get_game_object },
			{ "register_game_object", Game_register_game_object },
			{ "deregister_game_object", Game_deregister_game_object },
			// Dialogue
			{ "add_dialogue", Game_add_dialogue },
			{ "remove_dialogue", Game_remove_dialogue },
			{ "remove_all_dialogue", Game_remove_all_dialogue },
			{ "get_dialogue", Game_get_dialogue },
			{ "get_available_dialogues", Game_get_available_dialogues },
			// Quest
			{ "add_quest", Game_add_quest },
			{ "remove_quest", Game_remove_quest },
			{ "get_quest", Game_get_quest },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Game::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	int Game_add_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		Map *map = castUData<Map>(lua, 2);
		if (game && map)
		{
			lua_pushboolean(lua, game->addMap(map));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Game_remove_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstring(lua, -1))
			{
				lua_pushboolean(lua, game->removeMap(lua_tostring(lua, -1)));
				return 1;
			}
			Map *map = castUData<Map>(lua, -1);
			if (map)
			{
				lua_pushboolean(lua, game->removeMap(map));
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int Game_remove_all_maps(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			game->removeAllMaps();
		}
		return 0;
	}

	int Game_get_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game && lua_tostring(lua, -1))
		{
			Map *map = game->getMapLua(lua_tostring(lua, -1));
			if (map)
			{
				wrapRefObject<Map>(lua, map);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int Game_get_current_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			Map *map = game->getCurrentMap();
			if (map)
			{
				wrapRefObject<Map>(lua, map);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int Game_set_current_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstring(lua, -1))
			{
				game->setCurrentMap(lua_tostring(lua, -1));
			}
			else if (lua_isuserdata(lua, -1))
			{
				Map *map = castUData<Map>(lua, -1);
				game->setCurrentMap(map);
			}
		}
		return 0;
	}

	int Game_add_game_object_to_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game && lua_isuserdata(lua, -1))
		{
			GameObject *obj = getGameObject(lua, -1);
			lua_pushboolean(lua, game->addGameObjectToMap(obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Game_remove_game_object_from_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game && lua_isuserdata(lua, -1))
		{
			GameObject *obj = getGameObject(lua, -1);
			lua_pushboolean(lua, game->removeGameObjectFromMap(obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Game_has_game_object_in_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game && lua_isuserdata(lua, -1))
		{
			GameObject *obj = getGameObject(lua, -1);
			lua_pushboolean(lua, game->hasGameObjectInMap(obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Game_move_object_to_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			GameObject *obj = getGameObject(lua, 2);
			if (obj)
			{
				Map *map = NULL;
				if (lua_isstring(lua, 3))
				{
					map = game->getMapLua(lua_tostring(lua, 3));
				}
				else if (lua_isuserdata(lua ,3))
				{
					map = castUData<Map>(lua, 3);
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
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			GameObject *obj = getGameObject(lua, 2);
			if (obj)
			{
				Map *map = NULL;
				if (lua_isstring(lua, 3))
				{
					map = game->getMapLua(lua_tostring(lua, 3));
				}
				else if (lua_isuserdata(lua ,3))
				{
					map = castUData<Map>(lua, 3);
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
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			Character *character = game->getMainCharacter();
			if (character)
			{
				wrapRefObject<Character>(lua, character);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int Game_set_main_character(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		Character *character = castUData<Character>(lua, 2);
		if (game && character)
		{
			game->setMainCharacter(character);
		}
		return 0;
	}

	int Game_get_game_object(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game && lua_isstring(lua, -1))
		{
			GameObject *obj = game->getGameObject(lua_tostring(lua, -1));
			wrapGameObject(lua, obj);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Game_register_game_object(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		GameObject *obj = getGameObject(lua, -1);
		if (game && obj)
		{
			game->registerGameObject(obj);
		}
		return 0;
	}
	int Game_deregister_game_object(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		GameObject *obj = getGameObject(lua, -1);
		if (game && obj)
		{
			game->deregisterGameObject(obj);
		}
		return 0;
	}

	int Game_add_dialogue(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		Dialogue *diag = castUData<Dialogue>(lua, -1);
		if (game && diag)
		{
			lua_pushboolean(lua, game->addDialogue(diag));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Game_remove_dialogue(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game && lua_isstring(lua, -1))
		{
			lua_pushboolean(lua, game->removeDialogue(lua_tostring(lua, -1)));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Game_remove_all_dialogue(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			game->removeAllDialogue();
		}
		return 0;
	}
	int Game_get_dialogue(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game && lua_isstring(lua, -1))
		{
			Dialogue *diag = game->getDialogue(lua_tostring(lua, -1));
			if (diag)
			{
				wrapObject<Dialogue>(lua, diag);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int Game_get_available_dialogues(lua_State *lua)
	{
		// TODO
		return 0;
	}

	int Game_add_quest(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		Quest *quest = castUData<Quest>(lua, -1);
		if (game && quest)
		{
			lua_pushboolean(lua, game->addQuest(quest));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Game_remove_quest(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game && lua_isstring(lua, -1))
		{
			lua_pushboolean(lua, game->removeQuest(lua_tostring(lua, -1)));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Game_get_quest(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game && lua_isstring(lua, -1))
		{
			Quest *quest = game->getQuest(lua_tostring(lua, -1));
			if (quest)
			{
				wrapObject<Quest>(lua, quest);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	am::game::GameObject *getGameObject(lua_State *lua, int n)
	{
		if (lua_isstring(lua, -1))
		{
			return Engine::getGame()->getGameObject(lua_tostring(lua, -1));
		}
		else if (lua_isuserdata(lua, -1))
		{
			Character *obj = castUData<Character>(lua, -1);
			if (obj)
			{
				return obj;
			}
			Item *item = castUData<Item>(lua, -1);
			if (item)
			{
				return item;
			}
		}
		return NULL;
	}
	void wrapGameObject(lua_State *lua, am::game::GameObject *obj)
	{
		if (obj)
		{
			Character *isChar = dynamic_cast<Character *>(obj);
			if (isChar)
			{
				wrapRefObject<Character>(lua, isChar);
				return;
			}
			Item *isItem = dynamic_cast<Item *>(obj);
			if (isItem)
			{
				wrapRefObject<Item>(lua, isItem);
				return;
			}
		}
		lua_pushnil(lua);
	}

}
}
}
