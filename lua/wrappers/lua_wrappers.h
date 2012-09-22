#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
} 

namespace am {
namespace lua {
namespace wrapper {

	void AssignWrappers(lua_State *lua);
	
}
}
}
