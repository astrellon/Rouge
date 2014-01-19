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
#include <game/door.h>
using namespace am::game;

#include "lua_map.h"
#include "lua_character.h"
#include "lua_item.h"
#include "lua_dialogue.h"
#include "lua_quest.h"
#include "lua_iattribute_data.h"

#include <sstream>

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * The game class keeps track of all current game objects, loaded maps,
	 * dialogue and quests. There should only be one game instance in use
	 * at a time and it should be registered with the main engine as being
	 * the current game. There can be many game instances over the lifetime of
	 * an engine however.
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
			return 0;
		}
		return LuaState::expectedContext(lua, "__gc", "am.game");
	}
	/**
	 * Compares this game against another game object.
	 *
	 * @param am.game rhs The other game object to compare.
	 * @returns boolean True if they are the same object.
	 */
	int Game_eq(lua_State *lua)
	{
		Game *lhs = castUData<Game>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.game");
		}
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
			{ "char_def", Game_char_def },
			{ "item_def", Game_item_def },
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
			
			{ "start_game", Game_start_game },
			{ "has_started", Game_has_started },
			{ "load_game", Game_load_game },
			{ "scenario_name", Game_scenario_name },
			{ "generic_dead_graphic", Game_generic_dead_graphic },
			{ "attrs", Game_attrs },

			{ nullptr, nullptr }
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
	 * @param am.map map The map to add to the map pool.
	 * @returns boolean True if the map was successfully added, false indicates the map was nil or had no name.
	 */
	int Game_add_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			Map *map = castUData<Map>(lua, 2);
			if (map)
			{
				lua_pushboolean(lua, game->addMap(map));
				return 1;
			}
			return LuaState::expectedArgs(lua, "add_map", "am.map map");
		}
		return LuaState::expectedContext(lua, "add_map", "am.game");
	}
	/**
	 * Removes a map from the map pool.
	 *
	 * @param string map_name The name of the map to remove.
	 * @returns boolean True if the map was successfully removed, 
	 *  false indicates that map was not in the map pool.
	 */
	/**
	 * Removes a map from the map pool.
	 *
	 * @param am.map map The map to remove.
	 * @returns boolean True if the map was successfully removed, 
	 *  false indicates that map was not in the map pool.
	 */
	int Game_remove_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstr(lua, 2))
			{
				lua_pushboolean(lua, game->removeMap(lua_tostring(lua, 2)));
				return 1;
			}
			Map *map = castUData<Map>(lua, 2);
			if (map)
			{
				lua_pushboolean(lua, game->removeMap(map));
				return 1;
			}
			return LuaState::expectedArgs(lua, "remove_map", 2, "string map_name", "am.map map");
		}
		return LuaState::expectedContext(lua, "remove_map", "am.game");
	}
	/**
	 * Removes all the maps from the map pool.
	 *
	 * @returns am.game This
	 */
	int Game_remove_all_maps(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			game->removeAllMaps();
			lua_first(lua);
		}
		return LuaState::expectedContext(lua, "remove_all_maps", "am.game");
	}
	/**
	 * Returns a map from the map pool. If the map is not present
	 * it will attempt to load the map from file under the data directory.
	 * <p><code>data/maps/{map_name}.lua</code></p>
	 * @param string map_name The name of the map to load.
	 * @returns am.map The found/loaded map, nil if it could not be found nor loaded.
	 */
	int Game_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstr(lua, 2))
			{
				Map *map = game->getMapLua(lua_tostring(lua, 2));
				if (map)
				{
					wrapRefObject<Map>(lua, map);
					return 1;
				}
				lua_pushnil(lua);
				return 1;
			}
			return LuaState::expectedArgs(lua, "map", "string map_name");
		}
		return LuaState::expectedContext(lua, "map", "am.game");
	}
	/**
	 * Returns true if a map with the given map name is found in the map pool.
	 * @param string map_name The map name to look up.
	 * @returns boolean True if the map was found.
	 */
	int Game_has_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstr(lua, 2))
			{
				lua_pushboolean(lua, game->hasMap(lua_tostring(lua, 2)));
				return 1;
			}
			return LuaState::expectedArgs(lua, "has_map", "string map_name");
		}
		return LuaState::expectedContext(lua, "has_map", "am.game");
	}
	/**
	 * Returns the current active map, this will usually be the one that
	 * the main character is on.
	 *
	 * @returns am.map The current active map.
	 */
	/**
	 * Sets the current active map.
	 *
	 * @param string map_name The name of the map to set as the active map.
	 * @returns am.game This.
	 */
	/**
	 * Sets the current active map.
	 *
	 * @param am.map map The map to set as the active map.
	 * @returns am.game This.
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
				lua_pushnil(lua);
				return 1;
			}
			else
			{
				if (lua_isstr(lua, 2))
				{
					game->setCurrentMap(lua_tostring(lua, 2));
					lua_first(lua);
				}
				else
				{
					Map *map = castUData<Map>(lua, 2);
					if (map)
					{
						game->setCurrentMap(map);
						lua_first(lua);
					}
				}
				return LuaState::expectedArgs(lua, "current_map", 2, "string map_name", "am.map map");
			}
		}
		return LuaState::expectedContext(lua, "currentMap", "am.game");
	}
	/**
	 * Adds a game object to the currently active map.
	 *
	 * @param am.game_object game_object The game object to add.
	 * @returns boolean True if the game object was successfully added.
	 *  False indicates that either the current map is nil.
	 */
	/**
	 * Adds a game object to the currently active map.
	 *
	 * @param string gameObjectId The game object id of the game object to add.
	 * @returns boolean True if the game object was successfully added.
	 *  False indicates that either the current map is nil.
	 */
	int Game_add_game_object_to_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			GameObject *obj = getGameObject(lua, 2);
			if (obj)
			{
				lua_pushboolean(lua, game->addGameObjectToMap(obj));
				return 1;
			}
			return LuaState::expectedArgs(lua, "add_game_object_to_map", 2, "am.game_object game_object", "string game_id");
		}
		return LuaState::expectedContext(lua, "add_game_object_to_map", "am.game");
	}
	/**
	 * Removes a game object from the current map.
	 *
	 * @param am.game_object game_object The game object to remove from the current map.
	 * @returns boolean True if the game object was successfully removed.
	 *  False indicates that either the current map or game_object are nil.
	 */
	/**
	 * Removes a game object from the current map.
	 *
	 * @param string gameObjectId The game object id of the game object to remove from the current map.
	 * @returns boolean True if the game object was successfully removed.
	 *  False indicates that either the current map or game_object are nil.
	 */
	int Game_remove_game_object_from_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			GameObject *obj = getGameObject(lua, 2);
			if (obj)
			{
				lua_pushboolean(lua, game->removeGameObjectFromMap(obj));
				return 1;
			}
			return LuaState::expectedArgs(lua, "remove_game_object_from_map", 2, "string game_id", "am.game_object game_object");
		}
		return LuaState::expectedContext(lua, "remove_game_object_from_map", "am.game");
	}
	/**
	 * Returns true if the game object is on the current map.
	 *
	 * @param am.game_object game_object The game object to look for.
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
		if (game)
		{
			GameObject *obj = getGameObject(lua, 2);
			if (obj)
			{
				lua_pushboolean(lua, game->hasGameObjectInMap(obj));
				return 1;
			}
			return LuaState::expectedArgs(lua, "has_game_object_in_map", 2, "string game_id", "am.game_object game_object");
		}
		return LuaState::expectedContext(lua, "has_game_object_in_map", "am.game");
	}
	/**
	 * Moves a game object from their current map to a new map.
	 *
	 * @param am.game_object game_object The game object to move.
	 * @param am.map map The map to move to.
	 * @returns am.game This
	 */
	/**
	 * Moves a game object from their current map to a new map.
	 * 
	 * @param am.game_object game_object The game object to move.
	 * @param string map_name The name of the map to move to.
	 * @returns am.game This
	 */
	/**
	 * Moves a game object from their current map to a new map at a given position.
	 *
	 * @param am.game_object game_object The game object to move.
	 * @param am.map map The map to move to.
	 * @param number position_x The x position to move the game object to.
	 * @param number position_y The y position to move the game object to.
	 * @param boolean [true] set_as_current When true, the new map will become the current map and the camera will
	 *  focus on the given game object.
	 * @returns am.game This
	 */
	/**
	 * Moves a game object from their current map to a new map at a given position.
	 *
	 * @param am.game_object game_object The game object to move.
	 * @param string map_name The name of the map to move to.
	 * @param number position_x The x position to move the game object to.
	 * @param number position_y The y position to move the game object to.
	 * @param boolean [true] set_as_current When true, the new map will become the current map and the camera will
	 *  focus on the given game object.
	 * @returns am.game This
	 */
	int Game_move_object_to_map(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (moveGameObject(lua, game, false))
			{
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "move_game_object_to_map", 2, "am.game_object game_object, string map_name, number x, number y, boolean [true] set_as_current", "am.game_object game_object, map map, number x, number y, boolean set_as_current [true]");
		}
		return LuaState::expectedContext(lua, "move_game_object_to_map", "am.game");
	}
	/**
	 * Moves a game object from their current map to a new map.
	 * 
	 * @param am.game_object game_object The game object to move.
	 * @param am.map map The map to move to.
	 * @returns am.game This
	 */
	/**
	 * Moves a game object from their current map to a new map.
	 * 
	 * @param am.game_object game_object The game object to move.
	 * @param string map_name The name of the map to move to.
	 * @returns am.game This
	 */
	/**
	 * Moves a game object from their current map to a new map at a given grid position.
	 * @param am.game_object game_object The game object to move.
	 * @param am.map map The map to move to.
	 * @param integer grid_x The x grid position to move the game object to.
	 * @param integer grid_y The y grid position to move the game object to.
	 * @returns am.game This
	 */
	/**
	 * Moves a game object from their current map to a new map at a given grid position.
	 * @param am.game_object game_object The game object to move.
	 * @param string map_name The name of the map to move to.
	 * @param integer grid_x The x grid position to move the game object to.
	 * @param integer grid_y The y grid position to move the game object to.
	 * @returns am.game This
	 */
	int Game_move_object_to_map_grid(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			std::stringstream output;
			LuaState::printStack(lua, output);
			if (moveGameObject(lua, game, true))
			{
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "move_game_object_to_map_grid", 2, "am.game_object game_object, string map_name, number x, number y, boolean [true] set_as_current", "am.game_object game_object, map map, integer x, integer y, boolean [true] set_as_current");
		}
		return LuaState::expectedContext(lua, "move_game_object_to_map_grid", "am.game");
	}
	/**
	 * Returns the main character for this game.
	 *
	 * @returns am.character The main character.
	 */
	/**
	 * Sets the main character to the given character.
	 * 
	 * @param am.character main_char The new main character.
	 * @returns am.game This
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
				lua_pushnil(lua);
				return 1;
			}
			else if (lua_isnil(lua, 2))
			{
				game->setMainCharacter(nullptr);
				lua_first(lua);
			}
			else
			{
				Character *character = castUData<Character>(lua, 2);
				if (character)
				{
					game->setMainCharacter(character);
					lua_first(lua);
				}
			}
			return LuaState::expectedArgs(lua, "main", 2, "am.character main_char", "nil main_char");
		}
		return LuaState::expectedContext(lua, "main", "am.game");
	}
	/**
	 * Looks for a game object with the given game object id, returns nil if a 
	 * game object could not be found.
	 * 
	 * @param string gameObjectId The game object id to look up.
	 * @returns am.game_object The found game object, or nil.
	 */
	int Game_get_game_object(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstr(lua, 2))
			{
				GameObject *obj = game->getGameObject(lua_tostring(lua, 2));
				wrapGameObject(lua, obj);
				return 1;
			}
			return LuaState::expectedArgs(lua, "game_object", "string game_id");
		}
		return LuaState::expectedContext(lua, "game_object", "am.game");
	}
	/**
	 * Registers a game object with the game instance.
	 * 
	 * @param am.game_object game_object The game object to register with this game.
	 * @returns am.game This
	 */
	int Game_register_game_object(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			GameObject *obj = getGameObject(lua, 2);
			if (obj)
			{
				if (!game->registerGameObject(obj))
				{
					std::stringstream ss;
					ss << "Unable to register game object: ";
					LuaState::printTypeValue(lua, 2, ss, true);
					ss << " (" << obj->getName() << " does not have a game id";
					LuaState::warning(lua, ss.str().c_str());
				}
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "register_game_object", "am.game_object game_object");
		}
		return LuaState::expectedContext(lua, "register_game_object", "am.game");
	}
	/**
	 * Deregisters a game object with the game instance.
	 *
	 * @param am.game_object game_object The game object to deregister.
	 * @returns am.game This
	 */
	int Game_deregister_game_object(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			GameObject *obj = getGameObject(lua, 2);
			if (obj)
			{
				game->deregisterGameObject(obj);
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "deregister_game_object", "am.game_object game_object");
		}
		return LuaState::expectedContext(lua, "deregister_game_object", "am.game");
	}

	/**
	 * Returns the character definition with the given name.
	 * The definition name is made up of the file to load the definition from
	 * and the individual name. For example <code>npcs:male1</code><span> If this
	 * definition is not found then the </span><code>data/defs/npcs.lua</code><span> file
	 * is loaded then the definition is looked for again.</span>
	 *
	 * @param string def_name The definition name of the character to load.
	 * @returns am.character The found definition or nil.
	 */
	/**
	 * Registers a character with the given definition name.
	 * Definition names are always namespaced to the file they were loaded from.
	 * For example <code>filename:defname</code><span> This filename should be relative
	 * to the </span><code>data/defs/</code><span> folder. If no filename is given then the current
	 * file being loaded will be used.
	 * 
	 * The files under </span><code>data/defs/</code><span> will be automatically loaded
	 * when a definition with their filename is used. Definitions can also be
	 * registered from outside of those files, however they will need to always
	 * provide a filename. Definitions can be overridden.</span>
	 *
	 * @param string def_name The name to store the character definition under.
	 * @param am.character def The character definition to store.
	 * @returns am.game This
	 */
	int Game_char_def(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstr(lua, 2))
			{
				if (lua_gettop(lua) == 2)
				{
					Character *obj = game->getCharDefinition(lua_tostring(lua, 2));
					if (obj)
					{
						wrapRefObject<Character>(lua, obj);
						return 1;
					}
					lua_pushnil(lua);
					return 1;
				}
				else if (lua_isnil(lua, 3))
				{
					game->addCharDefinition(nullptr, lua_tostring(lua, 2));
					lua_first(lua);
				}
				else
				{
					Character *obj = castUData<Character>(lua, 3);
					if (obj)
					{
						game->addCharDefinition(obj, lua_tostring(lua, 2));
						lua_first(lua);
					}
				}
			}
			return LuaState::expectedArgs(lua, "char_def", 3, "string def_name", "string def_name, am.character char", "string def_name, nil char");
		}
		return LuaState::expectedContext(lua, "char_def", "am.game");
	}

	/**
	 * Returns the item definition with the given name.
	 * The definition name is made up of the file to load the definition from
	 * and the individual name. For example <code>human:sword</code><span> If this
	 * definition is not found then the </span><code>data/defs/human.lua</code><span> file
	 * is loaded then the definition is looked for again.</span>
	 *
	 * @param string def_name The definition name of the item to load.
	 * @returns am.item The found definition or nil.
	 */
	/**
	 * Registers a character with the given definition name.
	 * Definition names are always namespaced to the file they were loaded from.
	 * For example <code>filename:defname</code><span> This filename should be relative
	 * to the </span><code>data/defs/</code><span> folder. If no filename is given then the current
	 * file being loaded will be used.
	 * 
	 * The files under </span><code>data/defs/</code><span> will be automatically loaded
	 * when a definition with their filename is used. Definitions can also be
	 * registered from outside of those files, however they will need to always
	 * provide a filename. Definitions can be overridden.</span>
	 *
	 * @param string def_name The name to store the item definition under.
	 * @param am.item def The item definition to store.
	 * @returns am.game This
	 */
	int Game_item_def(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstr(lua, 2))
			{
				if (lua_gettop(lua) == 2)
				{
					Item *obj = game->getItemDefinition(lua_tostring(lua, 2));
					if (obj)
					{
						wrapRefObject<Item>(lua, obj);
						return 1;
					}
					lua_pushnil(lua);
					return 1;
				}
				else if (lua_isnil(lua, 3))
				{
					game->addItemDefinition(nullptr, lua_tostring(lua, 2));
					lua_first(lua);
				}
				else
				{
					Item *obj = castUData<Item>(lua, 3);
					if (obj)
					{
						game->addItemDefinition(obj, lua_tostring(lua, 2));
						lua_first(lua);
					}
				}
			}
			return LuaState::expectedArgs(lua, "item_def", 3, "string def_name", "string def_name, am.item item", "string def_name, nil item");
		}
		return LuaState::expectedContext(lua, "item_def", "am.game");
	}

	/**
	 * Adds the given dialogue to the dialogue pool.
	 *
	 * @param am.dialogue dialogue The dialogue to add to the pool.
	 * @returns boolean True if the dialogue was successfully added, false
	 *  indicates that dialogue was nil or was already in the pool.
	 */
	int Game_add_dialogue(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			Dialogue *diag = castUData<Dialogue>(lua, 2);
			if (diag)
			{
				lua_pushboolean(lua, game->addDialogue(diag));
				return 1;
			}
			return LuaState::expectedArgs(lua, "add_dialogue", "am.dialogue dialogue");
		}
		return LuaState::expectedContext(lua, "add_dialogue", "am.game");
	}
	/**
	 * Removes a dialogue from the dialogue pool.
	 *
	 * @param am.dialogue dialogue The dialogue to remove.
	 * @returns boolean True if the dialogue was successfully removed, false
	 *  indicates that the dialogue was nil or not found.
	 */
	/**
	 * Removes a dialogue from the dialogue pool.
	 *
	 * @param string dialogue_id The dialogue id of the dialogue to remove.
	 * @returns boolean True if the dialogue was successfully removed, false
	 *  indicates that the dialogue was nil or not found.
	 */
	int Game_remove_dialogue(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstr(lua, 2))
			{
				lua_pushboolean(lua, game->removeDialogue(lua_tostring(lua, 2)));
				return 1;
			}
			Dialogue *diag = castUData<Dialogue>(lua, 2);
			if (diag)
			{
				lua_pushboolean(lua, game->removeDialogue(diag->getId()));
				return 1;
			}
			return LuaState::expectedArgs(lua, "remove_dialogue", 2, "string dialogue_id", "am.dialogue dialogue");
		}
		return LuaState::expectedContext(lua, "remove_dialogue", "am.game");
	}
	/**
	 * Removes all dialogue from the dialogue pool.
	 *
	 * @returns am.game This
	 */
	int Game_remove_all_dialogue(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			game->removeAllDialogue();
			lua_first(lua);
		}
		return LuaState::expectedContext(lua, "remove_all_dialogue", "am.game");
	}
	/**
	 * Returns a dialogue with the given dialogue id, nil if no dialogue was found.
	 *
	 * @param string dialogue_id The id of the dialogue to look up.
	 * @returns am.dialogue The found dialogue or nil.
	 */
	int Game_dialogue(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstr(lua, 2))
			{
				Dialogue *diag = game->getDialogue(lua_tostring(lua, 2));
				if (diag)
				{
					wrapRefObject<Dialogue>(lua, diag);
					return 1;
				}
				lua_pushnil(lua);
				return 1;
			}
			return LuaState::expectedArgs(lua, "dialogue", "string dialogue_id");
		}
		return LuaState::expectedContext(lua, "dialogue", "am.game");
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
	 * @param am.quest quest The quest to add.
	 * @returns boolean True if the quest was successfully added.
	 */
	int Game_add_quest(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			Quest *quest = castUData<Quest>(lua, 2);
			if (quest)
			{
				lua_pushboolean(lua, game->addQuest(quest));
				return 1;
			}
			return LuaState::expectedArgs(lua, "add_quest", "am.quest quest");
		}
		return LuaState::expectedContext(lua, "add_quest", "am.game");
	}
	/**
	 * Removes a quest from the quest pool.
	 *
	 * @param am.quest quest The quest to remove from the quest pool.
	 * @returns boolean True if the quest was successfully removed.
	 */
	/**
	 * Removes a quest from the quest pool.
	 *
	 * @param string quest_id The id of the quest to remove from the quest pool.
	 * @returns boolean True if the quest was successfully removed.
	 */
	int Game_remove_quest(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstr(lua, 2))
			{
				lua_pushboolean(lua, game->removeQuest(lua_tostring(lua, 2)));
				return 1;
			}
			Quest *quest = castUData<Quest>(lua, 2);
			if (quest)
			{
				lua_pushboolean(lua, game->removeQuest(quest->getQuestId()));
				return 1;
			}
			return LuaState::expectedArgs(lua, "remove_quest", 2, "string quest_id", "am.quest quest");
		}
		return LuaState::expectedContext(lua, "remove_quest", "am.game");
	}
	/**
	 * Returns a quest with the given quest id, nil if the quest could not be found.
	 *
	 * @param string quest_id The id of the quest to look up.
	 * @returns am.quest The found quest, or nil.
	 */
	int Game_quest(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstr(lua, 2))
			{
				Quest *quest = game->getQuest(lua_tostring(lua, 2));
				if (quest)
				{
					wrapRefObject<Quest>(lua, quest);
					return 1;
				}
				lua_pushnil(lua);
				return 1;
			}
			return LuaState::expectedArgs(lua, "quest", "string quest_id");
		}
		return LuaState::expectedContext(lua, "quest", "am.game"); 
	}
	/**
	 * Tells the game that it can start. This should be called
	 * once it is ready to display the normal game HUD and have
	 * the appropriate game input listeners installed.
	 *
	 * @returns am.game This
	 */
	int Game_start_game(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			game->startGame();
			lua_first(lua);
		}
		return LuaState::expectedContext(lua, "start_game", "am.game");
	}
	/**
	 * Returns true if the game has started.
	 *
	 * @returns boolean True if the game has started.
	 */
	int Game_has_started(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			lua_pushboolean(lua, game->hasStarted());
			return 1;
		}
		return LuaState::expectedContext(lua, "has_started", "am.game");
	}

	/**
	 * Attemps to load the game from a save file.
	 *
	 * @param string save_name The name of the save file to load.
	 * @returns integer A 1 for success, or 0 for error.
	 */
	int Game_load_game(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_isstr(lua, 2))
			{
				lua_pushinteger(lua, game->loadGame(lua_tostring(lua, 2)));
				return 1;
			}
			return LuaState::expectedArgs(lua, "load_game", "string save_name");
		}
		return LuaState::expectedContext(lua, "load_game", "am.game");
	}

	/**
	 * Returns the scenario name which this game is based off.
	 * 
	 * The scenario name is set by the engine during a new game.
	 *
	 * @returns string The games scenario name.
	 */
	int Game_scenario_name(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			lua_pushstring(lua, game->getScenarioName());
			return 1;
		}
		return LuaState::expectedContext(lua, "scenario_name", "am.game");
	}

	/**
	 * Returns the generic dead graphic that is used when a specific dead graphic
	 * is not set onto a character.
	 *
	 * @returns am.sprite The generic dead graphic to use.
	 */
	/**
	 * Sets the generic dead graphic asset name to use when one has not been set onto the
	 * character itself.
	 *
	 * @param string asset_name The graphic asset name.
	 * @returns am.game This
	 */
	/**
	 * Sets the generic dead graphic to use when one has not been set onto the
	 * character itself.
	 *
	 * @param am.sprite sprite The graphic to set.
	 * @returns am.game This
	 */
	/**
	 * Sets the generic dead graphic asset name to nil.
	 *
	 * @param nil grapic Sets to nil.
	 * @returns am.game This
	 */
	int Game_generic_dead_graphic(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			if (lua_gettop(lua) == 1)
			{
				gfx::Sprite *dead = game->getGenericDeadGraphic();
				if (dead)
				{
					lua::wrapRefObject<gfx::Sprite>(lua, dead);
				}
				else
				{
					lua_pushnil(lua);
				}
				return 1;
			}
			else
			{
				bool valid = false;
				if (lua_isstr(lua, 2))
				{
					game->setGenericDeadGraphic(new gfx::Sprite(lua_tostring(lua, 2)));
					valid = true;
				}
				else if (lua_isnil(lua, 2))
				{
					game->setGenericDeadGraphic(nullptr);
					valid = true;
				}
				else
				{
					gfx::Sprite *sprite = lua::castUData<gfx::Sprite>(lua, 2);
					if (sprite)
					{
						game->setGenericDeadGraphic(sprite);
						valid = true;
					}
				}
				if (valid)
				{
					lua_first(lua);
				}
				return LuaState::expectedArgs(lua, "generic_dead_graphic", 3, "string asset_name", "Sprite sprite", "nil");
			}
			lua_pushstring(lua, game->getScenarioName());
			return 1;
		}
		return LuaState::expectedContext(lua, "generic_dead_graphic", "am.game");
	}

	/**
	 * Returns the game's attribute data table.
	 * By default if no attribute data table is present nil is returned unless true
	 * is passed as the first argument, then a data table is created if one is not present.
	 * 
	 * @param boolean [false] create_table Create a data table if one didn't exist.
	 * @returns am.data_table The data table on this game.
	 */
	/**
	 * Sets the data table on this game, can be set to nil.
	 *
	 * @param am.data_table attrTable The data table to set on the game.
	 * @returns am.game This
	 */
	int Game_attrs(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
		if (game)
		{
			return IAttributeData_attrs(lua, game);
		}
		return LuaState::expectedContext(lua, "attrs", "am.game");
	}

	am::game::GameObject *getGameObject(lua_State *lua, int n)
	{
		if (lua_isstr(lua, n))
		{
			return Engine::getGame()->getGameObject(lua_tostring(lua, n));
		}
		else if (lua_isuserdata(lua, n))
		{
			Character *obj = lua::castUData<Character>(lua, n);
			if (obj)
			{
				return obj;
			}
			Item *item = castUData<Item>(lua, n);
			if (item)
			{
				return item;
			}
			Door *door = castUData<Door>(lua, n);
			if (door)
			{
				return door;
			}
		}
		return nullptr;
	}
	void wrapGameObject(lua_State *lua, am::game::GameObject *obj)
	{
		if (obj)
		{
			GameObject::GameObjectType type = obj->getGameObjectType();
			if (type == GameObject::CHARACTER)
			{
				Character *isChar = dynamic_cast<Character *>(obj);
				if (isChar)
				{
					wrapRefObject<Character>(lua, isChar);
					return;
				}
			}
			else if (type == GameObject::ITEM)
			{
				Item *isItem = dynamic_cast<Item *>(obj);
				if (isItem)
				{
					wrapRefObject<Item>(lua, isItem);
					return;
				}
			}
			else if (type == GameObject::DOOR)
			{
				Door *isDoor = dynamic_cast<Door *>(obj);
				if (isDoor)
				{
					wrapRefObject<Door>(lua, isDoor);
					return;
				}
			}
		}
		lua_pushnil(lua);
	}

	bool moveGameObject(lua_State *lua, Game *game, bool grid)
	{
		GameObject *obj = getGameObject(lua, 2);
		if (obj)
		{
			Map *map = nullptr;
			int args = lua_gettop(lua);
			bool valid = args == 5 || args == 6 && lua_isbool(lua, 6);
			if (valid)
			{
				if (lua_isstring(lua, 3))
				{
					map = game->getMapLua(lua_tostring(lua, 3));
				}
				else
				{
					map = castUData<Map>(lua, 3);
					valid = map != nullptr;
				}
				valid &= lua_isnum(lua, 4) && lua_isnum(lua, 5);
				
				if (valid)
				{
					if (map)
					{
						int args = lua_gettop(lua);
						bool setCurrent = args == 6 && lua_tobool(lua, 6);
						if (args == 5)
						{
							setCurrent |= dynamic_cast<Character *>(obj) == game->getMainCharacter();
						}
						if (grid)
						{
							game->moveObjectToMapGrid(obj, map, lua_tointeger(lua, 4), lua_tointeger(lua, 5), setCurrent);
						}
						else
						{
							game->moveObjectToMap(obj, map, lua_tofloat(lua, 4), lua_tofloat(lua, 5), setCurrent);
						}
					}
					return true;
				}
			}
		}
		return false;
	}

}
}
}
