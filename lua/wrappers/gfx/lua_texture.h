#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace gfx {
	class Texture;
}

namespace lua {
namespace gfx {

	int Texture_ctor(lua_State *lua);
	int Texture_dtor(lua_State *lua);
	int Texture_eq(lua_State *lua);
	int Texture_register(lua_State *lua);

	int Texture_filename(lua_State *lua);
	int Texture_is_loaded(lua_State *lua);
	int Texture_reload(lua_State *lua);

	int Texture_width(lua_State *lua);
	int Texture_height(lua_State *lua);
	int Texture_bytes_per_pixel(lua_State *lua);
	int Texture_gl_format(lua_State *lua);

}
}
}
