#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class TileSet;
	class TileSetType;
}

namespace lua {
namespace game {

	const char TileSet_tableName[] = "am_game_TileSet";

	int TileSet_ctor(lua_State *lua);
	void TileSet_wrap(lua_State *lua, am::game::TileSet *set);

	int TileSet_dtor(lua_State *lua);
	int TileSet_register(lua_State *lua);

	int TileSet_set_name(lua_State *lua);
	int TileSet_get_name(lua_State *lua);

	int TileSet_set_full_name(lua_State *lua);
	int TileSet_get_full_name(lua_State *lua);

	int TileSet_add_tile(lua_State *lua);
	int TileSet_remove_tile(lua_State *lua);
	int TileSet_has_tile(lua_State *lua);
	int TileSet_get_tile(lua_State *lua);

	int TileSet_load_def(lua_State *lua);

	am::game::TileSet *Check_TileSet(lua_State *lua, int n);

}
}
}
