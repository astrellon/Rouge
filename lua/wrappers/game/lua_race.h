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

	int Race_ctor(lua_State *lua);
	int Race_dtor(lua_State *lua);
	int Race_eq(lua_State *lua);
	int Race_register(lua_State *lua);

	int Race_race_name(lua_State *lua);

}
}
}
