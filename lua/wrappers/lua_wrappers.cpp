#include "lua_wrappers.h"

#include "lua_game_object.h"
#include "lua_event_manager.h"

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace lua {
namespace wrapper {

	void AssignWrappers(lua_State *lua)
	{
		LuaState::registerWrapper("GameObject", am::lua::game::GameObject_register);
		LuaState::registerWrapper("EventManager", am::lua::ui::EventManager_register);
	}
	
}
}
}
