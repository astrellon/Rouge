#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace gfx {
	class Sprite;
}

namespace lua {
namespace gfx {

	const char Sprite_tableName[] = "am_gfx_Sprite";

	int Sprite_ctor(lua_State *lua);
	void Sprite_wrap(lua_State *lua, am::gfx::Sprite *sprite);
	int Sprite_dtor(lua_State *lua);
	int Sprite_eq(lua_State *lua);
	int Sprite_register(lua_State *lua);

	int Sprite_set_asset(lua_State *lua);

	am::gfx::Sprite *Check_Sprite(lua_State *lua, int n);

}
}
}
