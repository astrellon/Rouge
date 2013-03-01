#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class IStatModifiers;
}

namespace lua {
namespace game {

	int StatModifiers_ctor(lua_State *lua);
	int StatModifiers_dtor(lua_State *lua);
	int StatModifiers_eq(lua_State *lua);
	int StatModifiers_register(lua_State *lua);

	int StatModifiers_add(lua_State *lua);
	int StatModifiers_remove(lua_State *lua);
	int StatModifiers_mods(lua_State *lua);

	int StatModifiers_calculate_stat(lua_State *lua);

	int addToStatModifier(lua_State *lua, am::game::IStatModifiers *stats);
	int removeFromStatModifier(lua_State *lua, am::game::IStatModifiers *stats);

}
}
}
