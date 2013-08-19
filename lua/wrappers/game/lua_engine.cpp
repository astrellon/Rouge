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
#include <game/game.h>
#include <game/race.h>
using namespace am::game;

#include <sfx/sfx_engine.h>
#include <sfx/sfx_isound.h>
using namespace am::sfx;

#include "lua_tile.h"
#include "lua_tile_set.h"
#include "lua_game.h"
#include "lua_character.h"
#include "lua_item.h"
#include "lua_race.h"
#include "lua_tile_type.h"

#include <sstream>
using std::stringstream;

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * The engine class provides core functionality that has affects
	 * across multiple game instances.
	 */
	int Engine_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "game", Engine_game },
			{ "grid_size", Engine_grid_size },
			{ "using_tile_set", Engine_using_tile_set },
			{ "clear_using_tile_set", Engine_clear_using_tile_set },
			{ "tile", Engine_tile },
			{ "tile_set", Engine_tile_set },
			{ "add_tile_set", Engine_add_tile_set },
			{ "top_level_tile_set", Engine_top_level_tile_set },
			{ "game_object", Engine_get_by_game_id },
			{ "register_game_object", Engine_register_game_object },
			{ "deregister_game_object", Engine_deregister_game_object },
			{ "add_race", Engine_add_race },
			{ "remove_race", Engine_remove_race },
			{ "race", Engine_race },
			{ "unknown_race", Engine_unknown_race },
			{ "add_tile_type", Engine_add_tile_type },
			{ "tile_type", Engine_tile_type },
			{ "bgm", Engine_bgm },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, Engine::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	/**
	 * @static
	 * Returns the current game set on the engine, can be nil if
	 * there is no game running.
	 * @returns am.game The current game.
	 */
	/**
	 * @static
	 * Sets the current game.
	 * @param am.game game The new current game
	 */
	int Engine_game(lua_State *lua)
	{
		if (lua_gettop(lua) == 0)
		{
			Game *game = Engine::getEngine()->getCurrentGame();
			if (game)
			{
				wrapRefObject<Game>(lua, game);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		else if (lua_isnil(lua, 1))
		{
			Engine::getEngine()->setCurrentGame(nullptr);
			return 0;
		}
		else
		{
			Game *game = castUData<Game>(lua, 1);
			if (game)
			{
				Engine::getEngine()->setCurrentGame(game);
				return 0;
			}
		}
		return LuaState::expectedArgs(lua, "@game", 2, "Game game", "nil game");
	}
	/**
	 * @static
	 * Returns the grid size in pixels for the tiles in the game.
	 * @returns number Grid size
	 */
	/**
	 * @static
	 * Sets the grid size in pixels for the tiles in the game.
	 * @param number size The grid size
	 */
	int Engine_grid_size(lua_State *lua)
	{
		if (lua_gettop(lua) == 0)
		{
			lua_pushnumber(lua, Engine::getEngine()->getGridSize());
			return 1;
		}
		if (lua_isnum(lua, 1))
		{
			Engine::getEngine()->setGridSize(static_cast<float>(lua_tonumber(lua, 1)));
			return 0;
		}
		return LuaState::expectedArgs(lua, "@grid_size", "number size");
	}
	/**
	 * @static
	 * Adds a tile set name to look up when loading tiles.
	 * This allows maps to defines the list of tiles without having
	 * to specify the tile set each time.
	 * @param string tile_set_name The tile set name to use
	 */
	int Engine_using_tile_set(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			Engine::getEngine()->usingTileSet(lua_tostring(lua, 1));
			return 0;
		}
		return LuaState::expectedArgs(lua, "@using_tile_set", "string tile_set_name");
	}
	/**
	 * @static
	 * Clears the current list of tile sets that are in use.
	 */
	int Engine_clear_using_tile_set(lua_State *lua)
	{
		Engine::getEngine()->clearUsingTileSet();
		return 0;
	}
	/**
	 * @static
	 * Looks up a tile based on its name and the current list of tile sets in use.
	 * @param string tile_name The tile name to look up.
	 * @returns am.tile The found tile, or nil.
	 */
	int Engine_tile(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			Tile *tile = Engine::getEngine()->getTile(lua_tostring(lua, 1));
			if (tile)
			{
				wrapRefObject<Tile>(lua, tile);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@tile", "string tile_name");
	}
	/**
	 * @static
	 * Looks up a tile set based on its name.
	 * @param string tile_set_name Tile set name to look up
	 * @returns am.tile_set The found tile set or nil.
	 */
	int Engine_tile_set(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			TileSet *set = Engine::getEngine()->getTileSet(lua_tostring(lua, 1));
			if (set)
			{
				wrapRefObject<TileSet>(lua, set);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@tile_set", "string tile_set_name");
	}
	/**
	 * @static
	 * Adds a tile set to the game engines list of tile sets.
	 * @param am.tile_set tile_set The tile set to add.
	 */
	int Engine_add_tile_set(lua_State *lua)
	{
		TileSet *set = castUData<TileSet>(lua, 1);
		if (set)
		{
			Engine::getEngine()->addTileSet(set);
			return 0;
		}
		return LuaState::expectedArgs(lua, "@add_tile_set", "am.tile_set tile_set");
	}
	/**
	 * @static
	 * Returns the top level tile set.
	 * The top level tile set is the default tile set
	 * which all tile look ups will look up if all others fail.
	 * @returns am.tile_set The top level tile set.
	 */
	int Engine_top_level_tile_set(lua_State *lua)
	{
		wrapRefObject<TileSet>(lua, Engine::getEngine()->getTopLevelTileSet());
		return 1;
	}
	/**
	 * @static
	 * Looks up a game object by gameId.
	 * Returns a game object if found, nil otherwise.
	 * @param string game_id The game id to look up.
	 * @returns am.game_object The found game object, nil if not found.
	 */
	int Engine_get_by_game_id(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			wrapGameObject(lua, Engine::getEngine()->getGameObject(lua_tostring(lua, 1)));
			return 1;
		}
		return LuaState::expectedArgs(lua, "@game_object", "string gameId");
	}
	/**
	 * @static
	 * Registers a game object with the current game so that it can be
	 * looked up from anywhere. This is just shorthand for getting
	 * the current game and registering the game object with that.
	 * @param am.game_object game_object The game object to register with the game engine.
	 */
	int Engine_register_game_object(lua_State *lua)
	{
		GameObject *obj = castUData<Character>(lua, 1);
		if (!obj)
		{
			obj = castUData<Item>(lua, 1);
		}
		if (obj)
		{
			if (!Engine::getEngine()->registerGameObject(obj))
			{
				stringstream ss;
				ss << "Unable to register game object: ";
				LuaState::printTypeValue(lua, 2, ss, true);
				ss << " (" << obj->getName() << " does not have a game id";
				LuaState::warning(lua, ss.str().c_str());
			}
			return 0;
		}
		return LuaState::expectedArgs(lua, "@register_game_object", "am.game_object object");
	}
	/**
	 * @static
	 * Removes a game object from the current game. This is just 
	 * shorthand for removing the game object from the current game.
	 * @param string game_id The game id of the game object to remove.
	 */
	/**
	 * @static
	 * Removes a game object from the game engine. This is just 
	 * shorthand for removing the game object from the current game.
	 * @param am.game_object game_object The game object to remove.
	 */
	int Engine_deregister_game_object(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			Engine::getEngine()->deregisterGameObject(lua_tostring(lua, 1));
			return 0;
		}
		GameObject *obj = getGameObject(lua, 1);
		if (obj)
		{
			Engine::getEngine()->deregisterGameObject(obj);
			return 0;
		}
		return LuaState::expectedArgs(lua, "@deregister_game_object", 2, "string gameId", "am.game_object game_object");
	}
	/**
	 * @static
	 * Adds a race to the game engine.
	 * @param am.race race The race to add.
	 * @returns boolean True if the race was successfully added.
	 */
	int Engine_add_race(lua_State *lua)
	{
		Race *race = castUData<Race>(lua, 1);
		if (race)
		{
			lua_pushboolean(lua, Engine::getEngine()->addRace(race));
			return 1;
		}
		return LuaState::expectedArgs(lua, "@add_race", "am.race race");
	}
	/**
	 * @static
	 * Removes a race from the game engine.
	 * @param string race_name The name of the race to remove.
	 * @returns boolean True if the race was sucessfully removed.
	 */
	/**
	 * @static
	 * Removes a race from the game engine.
	 * @param am.race race The race to remove.
	 * @returns boolean True if the race was sucessfully removed.
	 */
	int Engine_remove_race(lua_State *lua)
	{
		Race *race = nullptr;
		if (lua_isstr(lua, 1))
		{
			race = Engine::getEngine()->getRace(lua_tostring(lua, 1));
		}
		else
		{
			race = castUData<Race>(lua, 1);
		}

		if (race)
		{
			lua_pushboolean(lua, Engine::getEngine()->removeRace(race));
			return 1;
		}
		return LuaState::expectedArgs(lua, "@remove_race", 2, "string race_name", "Race race");
	}
	/**
	 * @static
	 * Looks up a race in the game engine by name.
	 * @param string race_name The race name to look up.
	 * @returns am.race The found race or nil.
	 */
	int Engine_race(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			Race *race = Engine::getEngine()->getRace(lua_tostring(lua, 1));
			if (race)
			{
				wrapRefObject<Race>(lua, race);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@race", "string race_name");
	}
	/**
	 * @static
	 * Returns the 'unknown' race.
	 * @returns am.race The unknown race
	 */
	int Engine_unknown_race(lua_State *lua)
	{
		Race *race = Engine::getEngine()->getUnknownRace();
		if (race)
		{
			wrapRefObject<Race>(lua, race);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * @static
	 * Adds a tile type to the game engine.
	 * @param am.tile_type tile_type The type to add.
	 */
	int Engine_add_tile_type(lua_State *lua)
	{
		Handle<TileType> type(castUData<TileType>(lua, 1));
		if (type)
		{
			Engine::getEngine()->addTileType(type);
			return 0;
		}
		return LuaState::expectedArgs(lua, "@add_tile_type", "am.tile_type tile_type");
	}
	/**
	 * @static
	 * Looks up a tile tpye by name.
	 * @param string tile_type_name The name of the tile type to lookup.
	 * @returns am.tile_type The found tile type or nil.
	 */
	int Engine_tile_type(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			Handle<TileType> type(Engine::getEngine()->getTileType(lua_tostring(lua, 1)));
			if (type)
			{
				wrapObject<TileType>(lua, type);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@tile_type", "string tileTypeName");
	}

	/**
	 * @static
	 * Returns the sound object of the currently playing background music.
	 *
	 * @returns am.sound The current playing background music sound object.
	 */
	/**
	 * @static
	 * Sets the background music to stream from the given filename.
	 * Unsupported files or unfound files will not change the music.
	 *
	 * @param string filename The filename of the file to stream as music.
	 * @returns boolean True if the music was successfully changed.
	 */
	/**
	 * @static
	 * Sets the background music to the given sound object. The background
	 * music source is always set to be looping, so this works best with
	 * music designed for looping or with cue points.
	 *
	 * @param am.sound bgm The new sound object to use as background music.
	 */
	/**
	 * @static
	 * Turns the background music off.
	 *
	 * @param nil turn_off Turns music off.
	 */
	int Engine_bgm(lua_State *lua)
	{
		if (lua_gettop(lua) == 0)
		{
			ISound *bgm = SfxEngine::getEngine()->getBackgroundMusic();
			if (bgm)
			{
				wrapRefObject<ISound>(lua, bgm);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		else if (lua_isstr(lua, 1))
		{
			ISound *bgm = SfxEngine::getEngine()->loadStream(lua_tostring(lua, 1));
			if (bgm)
			{
				SfxEngine::getEngine()->setBackgroundMusic(bgm);
			}
			return 0;
		}
		else if (lua_isuserdata(lua, 1))
		{
			ISound *bgm = castUData<ISound>(lua, 1);
			if (bgm)
			{
				SfxEngine::getEngine()->setBackgroundMusic(bgm);
				return 0;
			}
		}
		else if (lua_isnil(lua, 1))
		{
			SfxEngine::getEngine()->setBackgroundMusic((ISound *)nullptr);
		}
		return LuaState::expectedArgs(lua, "@bgm", 2, "string filename", "nil set_no_sound", "am.sound bgm");
	}
	
}
}
}
