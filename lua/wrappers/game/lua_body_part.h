#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class BodyPart;
}

namespace lua {
namespace game {

	int BodyPart_ctor(lua_State *lua);
	int BodyPart_dtor(lua_State *lua);
	int BodyPart_eq(lua_State *lua);

	int BodyPart_register(lua_State *lua);

	int BodyPart_get_name(lua_State *lua);

	int BodyPart_set_equipped_item(lua_State *lua);
	int BodyPart_get_equipped_item(lua_State *lua);

}
}
}
