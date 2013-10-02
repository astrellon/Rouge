#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace gfx {
	class Sprite;
}
using namespace am::gfx;

namespace lua {
namespace gfx {

	int Sprite_ctor(lua_State *lua);
	int Sprite_dtor(lua_State *lua);
	int Sprite_eq(lua_State *lua);
	int Sprite_register(lua_State *lua);

	int Sprite_asset(lua_State *lua);

	bool getSprite(lua_State *lua, int n, Sprite *&result);

}
}
}
