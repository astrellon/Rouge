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
	/**
	 * @class
	 * The Game class keeps track of all current game objects, loaded maps,
	 * dialogue and quests. There should only be one Game instance in use
	 * at a time and it should be registered with the main Engine as being
	 * the current game. There can be many Game instances over the lifetime of
	 * an Engine however.
	 */
	/**
	 * Creates a new game instance.
	 */
	int Game_ctor(lua_State *lua)
	{
		Game *game = new Game(Engine::getEngine());
		
		wrapRefObject<Game>(lua, game);
		return 1;
	}
	/**
	 * Release the reference counter on the game.
	 */
	int Game_dtor(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			game->release();
		}
		return 0;
	}
	/**
	 * Compares this game against another game object.
	 *
	 * @param Game rhs The other game object to compare.
	 * @returns boolean True if they are the same object.
	 */
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
			{ "map", Game_map },
			{ "current_map", Game_current_map },
			{ "add_game_object_to_map", Game_add_game_object_to_map },
			{ "remove_game_object_from_map", Game_remove_game_object_from_map },
			{ "has_game_object_in_map", Game_has_game_object_in_map },
			{ "move_object_to_map", Game_move_object_to_map },
			{ "move_object_to_map_grid", Game_move_object_to_map_grid },
			{ "main", Game_main_character },
			// GameObject
			{ "game_object", Game_get_game_object },
			{ "register_game_object", Game_register_game_object },
			{ "deregister_game_object", Game_deregister_game_object },
			// Dialogue
			{ "add_dialogue", Game_add_dialogue },
			{ "remove_dialogue", Game_remove_dialogue },
			{ "remove_all_dialogue", Game_remove_all_dialogue },
			{ "dialogue", Game_dialogue },
			{ "available_dialogues", Game_available_dialogues },
			// Quest
			{ "add_quest", Game_add_quest },
			{ "remove_quest", Game_remove_quest },
			{ "quest", Game_quest },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Game::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	/**
	 * Manually adds a new map to the map pool. Maps generally can be loaded from
	 * file automatically via the <code>map()</code> function. However if a map
	 * has been created in code it'll have to be added with this function.
	 * If a map with the same map name already exists it will be overridden in the map pool.
	 *
	 * @param Map map The map to add to the map pool.
	 * @returns boolean True if the map was successfully added, false indicates the map was nil or had no name.
	 */
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
	/**
	 * Removes a map from the map pool.
	 *
	 * @param string mapName The name of the map to remove.
	 * @returns boolean True if the map was successfully removed, 
	 *  false indicates that map was not in the map pool.
	 */
	/**
	 * Removes a map from the map pool.
	 *
	 * @param Map map The map to remove.
	 * @returns boolean True if the map was successfully removed, 
	 *  false indicates that map was not in the map pool.
	 */
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
	/**
	 * Removes all the maps from the map pool.
	 *
	 * @returns Game This
	 */
	int Game_remove_all_maps(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			game->removeAllMaps();
			lua_first(lua);
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns a map from the map pool. If the map is not present
	 * it will attempt to load the map from file under the data directory.
	 * <p><code>data/maps/{mapName}.lua</code></p>
	 * @param string mapName The name of the map to load.
	 * @returns Map The found/loaded map, nil if it could not be found nor loaded.
	 */
	int Game_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game && lua_type(lua, -1) == LUA_TSTRING)
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
	/**
	 * Returns true if a map with the given map name is found in the map pool.
	 * @param string mapName The map name to look up.
	 * @returns boolean True if the map was found.
	 */
	int Game_has_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game && lua_isstring(lua, -1))
		{
			lua_pushboolean(lua, game->hasMap(lua_tostring(lua, -1)));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Returns the current active map, this will usually be the one that
	 * the main character is on.
	 *
	 * @returns Map The current active map.
	 */
	/**
	 * Sets the current active map.
	 *
	 * @param string mapName The name of the map to set as the active map.
	 * @returns Game This.
	 */
	/**
	 * Sets the current active map.
	 *
	 * @param Map map The map to set as the active map.
	 * @returns Game This.
	 */
	int Game_current_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_gettop(lua) == 1)
			{
				Map *map = game->getCurrentMap();
				if (map)
				{
					wrapRefObject<Map>(lua, map);
					return 1;
				}
			}
			else
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
				lua_first(lua);
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Adds a game object to the currently active map.
	 *
	 * @param GameObject gameObject The game object to add.
	 * @returns boolean True if the game object was successfully added.
	 *  False indicates that either the current map or gameObject are nil.
	 */
	/**
	 * Adds a game object to the currently active map.
	 *
	 * @param string gameObjectId The game object id of the game object to add.
	 * @returns boolean True if the game object was successfully added.
	 *  False indicates that either the current map or gameObject are nil.
	 */
	int Game_add_game_object_to_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game && lua_isuserdata(lua, -1))
		{
			GameObject *obj = getGameObject(lua, -1);
			lua_pushboolean(lua, game->addGameObjectToMap(obj));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Removes a game object from the current map.
	 *
	 * @param GameObject gameObject The game object to remove from the current map.
	 * @returns boolean True if the game object was successfully removed.
	 *  False indicates that either the current map or gameObject are nil.
	 */
	/**
	 * Removes a game object from the current map.
	 *
	 * @param string gameObjectId The game object id of the game object to remove from the current map.
	 * @returns boolean True if the game object was successfully removed.
	 *  False indicates that either the current map or gameObject are nil.
	 */
	int Game_remove_game_object_from_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game && lua_isuserdata(lua, -1))
		{
			GameObject *obj = getGameObject(lua, -1);
			lua_pushboolean(lua, game->removeGameObjectFromMap(obj));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Returns true if the game object is on the current map.
	 *
	 * @param GameObject gameObject The game object to look for.
	 * @returns boolean True if the game object was found on the current map.
	 */
	/**
	 * Returns true if the game object is on the current map.
	 *
	 * @param string gameObjectId The game object id of the game object to look for.
	 * @returns boolean True if the game object was found on the current map.
	 */
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
	/**
	 * Moves a game object from their current map to a new map.
	 *
	 * @param GameObject gameObject The game object to move.
	 * @param Map map The map to move to.
	 * @returns Game This
	 */
	/**
	 * Moves a game object from their current map to a new map.
	 * 
	 * @param GameObject gameObject The game object to move.
	 * @param string mapName The name of the map to move to.
	 * @returns Game This
	 */
	/**
	 * Moves a game object from their current map to a new map at a given position.
	 *
	 * @param GameObject gameObject The game object to move.
	 * @param Map map The map to move to.
	 * @param number positionX The x position to move the game object to.
	 * @param number positionY The y position to move the game object to.
	 * @returns Game This
	 */
	/**
	 * Moves a game object from their current map to a new map at a given position.
	 *
	 * @param GameObject gameObject The game object to move.
	 * @param string mapName The name of the map to move to.
	 * @param number positionX The x position to move the game object to.
	 * @param number positionY The y position to move the game object to.
	 * @returns Game This
	 */
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
			lua_first(lua);
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Moves a game object from their current map to a new map.
	 * 
	 * @param GameObject gameObject The game object to move.
	 * @param Map map The map to move to.
	 * @returns Game This
	 */
	/**
	 * Moves a game object from their current map to a new map.
	 * 
	 * @param GameObject gameObject The game object to move.
	 * @param string mapName The name of the map to move to.
	 * @returns Game This
	 */
	/**
	 * Moves a game object from their current map to a new map at a given grid position.
	 * @param GameObject gameObject The game object to move.
	 * @param Map map The map to move to.
	 * @param integer gridX The x grid position to move the game object to.
	 * @param integer gridY The y grid position to move the game object to.
	 * @returns Game This
	 */
	/**
	 * Moves a game object from their current map to a new map at a given grid position.
	 * @param GameObject gameObject The game object to move.
	 * @param string mapName The name of the map to move to.
	 * @param integer gridX The x grid position to move the game object to.
	 * @param integer gridY The y grid position to move the game object to.
	 * @returns Game This
	 */
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
			lua_first(lua);
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the main character for this game.
	 *
	 * @returns Character The main character.
	 */
	/**
	 * Sets the main character to the given character.
	 * 
	 * @param Character The new main character.
	 * @returns Game This
	 */
	int Game_main_character(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_gettop(lua) == 1)
			{
				Character *character = game->getMainCharacter();
				if (character)
				{
					wrapRefObject<Character>(lua, character);
					return 1;
				}
			}
			else if (lua_isuserdata(lua, -1))
			{
				Character *character = castUData<Character>(lua, -1);
				if (character)
				{
					game->setMainCharacter(character);
				}
				lua_first(lua);
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Looks for a game object with the given game object id, returns nil if a 
	 * game object could not be found.
	 * 
	 * @param string gameObjectId The game object id to look up.
	 * @returns GameObject The found game object, or nil.
	 */
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
	/**
	 * Registers a game object with the game instance.
	 * 
	 * @param GameObject gameObject The game object to register with this game.
	 * @returns Game This
	 */
	int Game_register_game_object(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			GameObject *obj = getGameObject(lua, -1);
			if (obj)
			{
				game->registerGameObject(obj);
			}
			lua_first(lua);
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Deregisters a game object with the game instance.
	 *
	 * @param GameObject gameObject The game object to deregister.
	 * @returns Game This
	 */
	int Game_deregister_game_object(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			GameObject *obj = getGameObject(lua, -1);
			if (obj)
			{
				game->deregisterGameObject(obj);
			}
			lua_first(lua);
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Adds the given dialogue to the dialogue pool.
	 *
	 * @param Dialogue dialogue The dialogue to add to the pool.
	 * @returns boolean True if the dialogue was successfully added, false
	 *  indicates that dialogue was nil or was already in the pool.
	 */
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
	/**
	 * Removes a dialogue from the dialogue pool.
	 *
	 * @param Dialogue dialogue The dialogue to remove.
	 * @returns boolean True if the dialogue was successfully removed, false
	 *  indicates that the dialogue was nil or not found.
	 */
	/**
	 * Removes a dialogue from the dialogue pool.
	 *
	 * @param string dialogueId The dialogue id of the dialogue to remove.
	 * @returns boolean True if the dialogue was successfully removed, false
	 *  indicates that the dialogue was nil or not found.
	 */
	int Game_remove_dialogue(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstring(lua, -1))
			{
				lua_pushboolean(lua, game->removeDialogue(lua_tostring(lua, -1)));
				return 1;
			}
			Dialogue *diag = castUData<Dialogue>(lua, -1);
			if (diag)
			{
				lua_pushboolean(lua, game->removeDialogue(diag->getId()));
				return 1;
			}
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Removes all dialogue from the dialogue pool.
	 *
	 * @returns Game This
	 */
	int Game_remove_all_dialogue(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			game->removeAllDialogue();
			lua_first(lua);
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns a dialogue with the given dialogue id, nil if no dialogue was found.
	 *
	 * @param string dialogueId The id of the dialogue to look up.
	 * @returns Dialogue The found dialogue or nil.
	 */
	int Game_dialogue(lua_State *lua)
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
	/**
	 * TODO
	 */
	int Game_available_dialogues(lua_State *lua)
	{
		// TODO
		return 0;
	}
	/**
	 * Adds a quest to the quest pool.
	 *
	 * @param Quest quest The quest to add.
	 * @returns boolean True if the quest was successfully added.
	 */
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
	/**
	 * Removes a quest from the quest pool.
	 *
	 * @param Quest quest The quest to remove from the quest pool.
	 * @returns boolean True if the quest was successfully removed.
	 */
	/**
	 * Removes a quest from the quest pool.
	 *
	 * @param string questId The id of the quest to remove from the quest pool.
	 * @returns boolean True if the quest was successfully removed.
	 */
	int Game_remove_quest(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstring(lua, -1))
			{
				lua_pushboolean(lua, game->removeQuest(lua_tostring(lua, -1)));
				return 1;
			}
			Quest *quest = castUData<Quest>(lua, -1);
			if (quest)
			{
				lua_pushboolean(lua, game->removeQuest(quest->getQuestId()));
				return 1;
			}
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Returns a quest with the given quest id, nil if the quest could not be found.
	 *
	 * @param string questId The id of the quest to look up.
	 * @returns Quest The found quest, or nil.
	 */
	int Game_quest(lua_State *lua)
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
