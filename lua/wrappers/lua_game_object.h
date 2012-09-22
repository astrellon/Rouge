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

	const char GameObject_tableName[] = "am_game_GameObject";

	int GameObject_ctor(lua_State *lua);
	int GameObject_dtor(lua_State *lua);
	int GameObject_register(lua_State *lua);
	
	int GameObject_get_name(lua_State *lua);
	int GameObject_set_name(lua_State *lua);
	
	am::game::GameObject *Check_GameObject(lua_State *lua, int n);

}
}
}
