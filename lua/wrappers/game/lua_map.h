#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class Map;
	class GameObject;
}

namespace lua {
namespace game {

	const char Map_tableName[] = "am_game_Map";

	int Map_ctor(lua_State *lua);
	void Map_wrap(lua_State *lua, am::game::Map *map);

	int Map_dtor(lua_State *lua);
	int Map_register(lua_State *lua);

	int Map_set_name(lua_State *lua);
	int Map_get_name(lua_State *lua);

	int Map_set_full_name(lua_State *lua);
	int Map_get_full_name(lua_State *lua);

	int Map_get_tile(lua_State *lua);
	int Map_get_tile_instance(lua_State *lua);

	int Map_set_tiles(lua_State *lua);

	int Map_set_map_size(lua_State *lua);
	int Map_get_map_size(lua_State *lua);

	int Map_add_game_object(lua_State *lua);
	int Map_remove_game_object(lua_State *lua);
	int Map_has_game_object(lua_State *lua);

	int Map_is_valid_location(lua_State *lua);
	int Map_is_valid_grid_location(lua_State *lua);

	int Map_load_def(lua_State *lua);

	am::game::Map *Check_Map(lua_State *lua, int n);

	am::game::GameObject *getGameObject(lua_State *lua, int n);

}
}
}
