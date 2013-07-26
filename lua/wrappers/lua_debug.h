#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace lua {
namespace debug {

	int Debug_register(lua_State *lua);

	int Debug_log(lua_State *lua);
	int Debug_equals(lua_State *lua);
	int Debug_not_equals(lua_State *lua);
	int Debug_print_stack(lua_State *lua);
	
}
}
}
