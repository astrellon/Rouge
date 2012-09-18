#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
} 

namespace am {
namespace game {
	class GameObject;
}

namespace lua {
namespace game {

	int l_GameObject_ctor(lua_State *lua);
	int l_GameObject_dtor(lua_State *lua);
	int l_GameObject_register(lua_State *lua);
	
	int l_GameObject_get_name(lua_State *lua);
	int l_GameObject_set_name(lua_State *lua);
	
	am::game::GameObject *l_Check_GameObject(lua_State *lua, int n);

}
}
}
