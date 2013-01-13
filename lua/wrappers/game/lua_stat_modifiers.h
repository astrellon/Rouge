#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class StatModifiers;
}

namespace lua {
namespace game {

	int StatModifiers_ctor(lua_State *lua);
	int StatModifiers_dtor(lua_State *lua);
	int StatModifiers_eq(lua_State *lua);
	int StatModifiers_register(lua_State *lua);

	int StatModifiers_add_modifier(lua_State *lua);
	int StatModifiers_remove_modifier(lua_State *lua);

	int StatModifiers_add_modifiers(lua_State *lua);
	int StatModifiers_remove_modifiers(lua_State *lua);

	int StatModifiers_calculate_stat(lua_State *lua);

}
}
}
