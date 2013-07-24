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

	int BodyPart_name(lua_State *lua);
	int BodyPart_type(lua_State *lua);

	int BodyPart_can_hold_onto(lua_State *lua);
	int BodyPart_weapon_part(lua_State *lua);
	int BodyPart_is_holding_onto(lua_State *lua);

	int BodyPart_can_equip_item(lua_State *lua);
	int BodyPart_item(lua_State *lua);

}
}
}
