#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class StatModifier;
}

namespace lua {
namespace game {

	int StatModifier_ctor(lua_State *lua);
	int StatModifier_dtor(lua_State *lua);
	int StatModifier_eq(lua_State *lua);
	int StatModifier_register(lua_State *lua);

	int StatModifier_value(lua_State *lua);
	int StatModifier_type(lua_State *lua);
	int StatModifier_magical(lua_State *lua);

}
}
}
