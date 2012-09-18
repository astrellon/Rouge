#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
} 

namespace am {
namespace lua {
namespace wrapper {

	void l_Assign_Wrappers(lua_State *lua);
	
}
}
}
