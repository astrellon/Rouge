#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace lua {
namespace gfx {

	int Font_ctor(lua_State *lua);
	int Font_dtor(lua_State *lua);
	int Font_eq(lua_State *lua);
	int Font_register(lua_State *lua);

	int Font_asset(lua_State *lua);
	int Font_name(lua_State *lua);
	int Font_fixed_width(lua_State *lua);
	int Font_char_width(lua_State *lua);
	int Font_char_height(lua_State *lua);

	int Font_kerning(lua_State *lua);
	int Font_leading(lua_State *lua);
	int Font_space_width(lua_State *lua);
	int Font_tab_width(lua_State *lua);

	int Font_measure_text(lua_State *lua);
	int Font_measure_line(lua_State *lua);
	int Font_measure_word(lua_State *lua);

	int Font_process(lua_State *lua);
}
}
}
