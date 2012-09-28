#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
} 

namespace am {
namespace game {
	class Character;
}

namespace lua {
namespace game {

	const char Character_tableName[] = "am_game_Character";

	int Character_ctor(lua_State *lua);
	int Character_dtor(lua_State *lua);
	int Character_register(lua_State *lua);
	
	int Character_get_name(lua_State *lua);
	int Character_set_name(lua_State *lua);

	int Character_set_pickup_reach(lua_State *lua);
	int Character_get_pickup_reach(lua_State *lua);

	int Character_set_age(lua_State *lua);
	int Character_get_age(lua_State *lua);
	
	int Character_set_gender(lua_State *lua);
	int Character_get_gender(lua_State *lua);

	am::game::Character *Check_Character(lua_State *lua, int n);

}
}
}
