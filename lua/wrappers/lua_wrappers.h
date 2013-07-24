#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
} 

namespace am {
namespace lua {
namespace wrapper {

	void addWrapper(lua_State *lua, const char *name, lua_CFunction call, int id);
	void AssignWrappers(lua_State *lua, int tableRef);
	
}
}
}
