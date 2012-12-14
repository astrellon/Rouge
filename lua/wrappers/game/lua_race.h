#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class Race;
}

namespace lua {
namespace game {

	const char Race_tableName[] = "am_game_Race";

	int Race_ctor(lua_State *lua);
	void Race_wrap(lua_State *lua, am::game::Race *race);

	int Race_dtor(lua_State *lua);
	int Race_register(lua_State *lua);

	int Race_get_race_name(lua_State *lua);

	am::game::Race *Check_Race(lua_State *lua, int n);

}
}
}
