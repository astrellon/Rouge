#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace lua {
namespace game {

	int MapRegion_ctor(lua_State *lua);
	int MapRegion_dtor(lua_State *lua);
	int MapRegion_eq(lua_State *lua);

	int MapRegion_register(lua_State *lua);

	int MapRegion_size(lua_State *lua);
	int MapRegion_data(lua_State *lua);
	int MapRegion_location(lua_State *lua);
	int MapRegion_intersects_with(lua_State *lua);

	int MapRegion_add_event_listener(lua_State *lua);
	int MapRegion_remove_event_listener(lua_State *lua);
	int MapRegion_has_event_listener(lua_State *lua);
}
}
}
