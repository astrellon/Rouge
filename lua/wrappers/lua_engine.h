#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class Engine;
}

namespace lua {
namespace game {

	const char Engine_tableName[] = "am_game_Engine";

	int Engine_register(lua_State *lua);

	int Engine_set_grid_size(lua_State *lua);
	int Engine_get_grid_size(lua_State *lua);

	int Engine_get_tile(lua_State *lua);
	int Engine_get_tile_set(lua_State *lua);

}
}
}
