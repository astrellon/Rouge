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

#include "lua_tile.h"
#include "lua_tile_set.h"
#include "lua_game.h"
#include "lua_character.h"
#include "lua_item.h"
#include "lua_race.h"
#include "lua_tile_type.h"

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
			{ "using_tile_set", Engine_using_tile_set },
			{ "clear_using_tile_set", Engine_clear_using_tile_set },
			{ "get_tile", Engine_get_tile },
			{ "get_tile_set", Engine_get_tile_set },
			{ "add_tile_set", Engine_add_tile_set },
			{ "get_top_level_tile_set", Engine_get_top_level_tile_set },
			{ "get_by_game_id", Engine_get_by_game_id },
			{ "register_game_object", Engine_register_game_object },
			{ "deregister_game_object", Engine_deregister_game_object },
			{ "add_race", Engine_add_race },
			{ "remove_race", Engine_remove_race },
			{ "get_race", Engine_get_race },
			{ "get_unknown_race", Engine_get_unknown_race },
			{ "add_tile_type", Engine_add_tile_type },
			{ "get_tile_type", Engine_get_tile_type },
			{ "get_game", Engine_get_game },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Engine::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	int Engine_set_current_game(lua_State *lua)
	{
		Game *game = castUData<Game>(lua, 1);
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
			wrapRefObject<Game>(lua, game);
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

	int Engine_using_tile_set(lua_State *lua)
	{
		if (lua_isstring(lua, 1))
		{
			Engine::getEngine()->usingTileSet(lua_tostring(lua, 1));
		}
		return 0;
	}
	int Engine_clear_using_tile_set(lua_State *lua)
	{
		Engine::getEngine()->clearUsingTileSet();
		return 0;
	}

	int Engine_get_tile(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			Tile *tile = Engine::getEngine()->getTile(lua_tostring(lua, -1));
			if (tile)
			{
				wrapRefObject<Tile>(lua, tile);
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
			wrapRefObject<TileSet>(lua, set);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Engine_add_tile_set(lua_State *lua)
	{
		TileSet *set = castUData<TileSet>(lua, 1);
		if (set)
		{
			Engine::getEngine()->addTileSet(set);
		}
		return 0;
	}
	int Engine_get_top_level_tile_set(lua_State *lua)
	{
		wrapRefObject<TileSet>(lua, Engine::getEngine()->getTopLevelTileSet());
		return 1;
	}

	int Engine_get_by_game_id(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			GameObject *obj = Engine::getEngine()->getGameObject(lua_tostring(lua, -1));
			if (obj)
			{
				Character *character = dynamic_cast<Character *>(obj);
				if (character)
				{
					wrapRefObject<Character>(lua, character);
					return 1;
				}
				Item *item = dynamic_cast<Item *>(obj);
				if (item)
				{
					wrapRefObject<Item>(lua, item);
					return 1;
				}
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int Engine_register_game_object(lua_State *lua)
	{
		GameObject *obj = castUData<Character>(lua, -1);
		if (!obj)
		{
			obj = castUData<Item>(lua, -1);
		}
		if (obj)
		{
			Engine::getEngine()->registerGameObject(obj);
		}
		return 0;
	}
	int Engine_deregister_game_object(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			Engine::getEngine()->deregisterGameObject(lua_tostring(lua, -1));
			return 0;
		}
		GameObject *obj = castUData<Character>(lua, -1);
		if (!obj)
		{
			obj = castUData<Item>(lua, -1);
		}
		if (obj)
		{
			Engine::getEngine()->deregisterGameObject(obj);
		}
		return 0;
	}

	int Engine_add_race(lua_State *lua)
	{
		Race *race = castUData<Race>(lua, -1);
		if (race)
		{
			lua_pushboolean(lua, Engine::getEngine()->addRace(race));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Engine_remove_race(lua_State *lua)
	{
		Race *race = NULL;
		if (lua_isstring(lua, -1))
		{
			race = Engine::getEngine()->getRace(lua_tostring(lua, -1));
		}
		else if (lua_isuserdata(lua, -1))
		{
			race = castUData<Race>(lua, -1);
		}
		else
		{
			lua_pushnil(lua);
			return 1;
		}

		if (race)
		{
			lua_pushboolean(lua, Engine::getEngine()->removeRace(race));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Engine_get_race(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			Race *race = Engine::getEngine()->getRace(lua_tostring(lua, -1));
			if (race)
			{
				wrapObject<Race>(lua, race);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int Engine_get_unknown_race(lua_State *lua)
	{
		Race *race = Engine::getEngine()->getUnknownRace();
		if (race)
		{
			wrapObject<Race>(lua, race);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Engine_add_tile_type(lua_State *lua)
	{
		TileType *type = castUData<TileType>(lua, -1);
		if (type)
		{
			Engine::getEngine()->addTileType(type);
		}
		return 0;
	}
	int Engine_get_tile_type(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			TileType *type = Engine::getEngine()->getTileType(lua_tostring(lua, -1));
			if (type)
			{
				wrapObject<TileType>(lua, type);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	int Engine_get_game(lua_State *lua)
	{
		Game *game = Engine::getGame();
		if (game)
		{
			wrapRefObject<Game>(lua, game);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
