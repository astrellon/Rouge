#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class TileInstance;
}

namespace lua {
namespace game {

	const char TileInstance_tableName[] = "am_game_TileInstance";

	int TileInstance_ctor(lua_State *lua);
	void TileInstance_wrap(lua_State *lua, am::game::TileInstance *inst);
	int TileInstance_dtor(lua_State *lua);
	int TileInstance_eq(lua_State *lua);
	int TileInstance_register(lua_State *lua);

	int TileInstance_get_tile(lua_State *lua);
	int TileInstance_set_tile(lua_State *lua);

	int TileInstance_set_tile_name(lua_State *lua);

	int TileInstance_get_tile_frame(lua_State *lua);
	int TileInstance_set_tile_frame(lua_State *lua);



	am::game::TileInstance *Check_TileInstance(lua_State *lua, int n);

}
}
}
