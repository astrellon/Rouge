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

	const char BodyPart_tableName[] = "am_game_BodyPart";

	int BodyPart_ctor(lua_State *lua);
	void BodyPart_wrap(lua_State *lua, am::game::BodyPart *part);
	int BodyPart_dtor(lua_State *lua);
	int BodyPart_eq(lua_State *lua);

	int BodyPart_register(lua_State *lua);

	int BodyPart_get_name(lua_State *lua);

	int BodyPart_set_equipped_item(lua_State *lua);
	int BodyPart_get_equipped_item(lua_State *lua);

	am::game::BodyPart *Check_BodyPart(lua_State *lua, int n);

}
}
}
