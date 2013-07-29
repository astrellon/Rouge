#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace lua {

	int ReturnCodes_register(lua_State *lua);

	int ReturnCodes_message(lua_State *lua);
	
}
}
