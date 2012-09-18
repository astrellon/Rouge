#include "lua_wrappers.h"

#include "lua_game_object.h"

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace lua {
namespace wrapper {

	void l_Assign_Wrappers(lua_State *lua)
	{
		LuaState::registerWrapper("GameObject", am::lua::game::l_GameObject_register);
	}
	
}
}
}
