#include "lua_wrappers.h"

#include "lua_character.h"
#include "lua_event_manager.h"
#include "lua_stats.h"

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace lua {
namespace wrapper {

	void AssignWrappers(lua_State *lua)
	{
		LuaState::registerWrapper("Character", am::lua::game::Character_register);
		LuaState::registerWrapper("Stats", am::lua::game::Stats_register);
		LuaState::registerWrapper("EventManager", am::lua::ui::EventManager_register);
	}
	
}
}
}
