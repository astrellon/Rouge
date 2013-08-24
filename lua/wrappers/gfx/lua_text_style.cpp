#include "lua_text_style.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <gfx/gfx_text_style.h>
using namespace am::gfx;

namespace am {
namespace lua {
namespace gfx {
	/**
	 * @class
	 * Simple wrapper for the font class. Before a font can be used it must have process() called.
	 */
	/**
	 * @static
	 * Creates a new font using the given name.
	 *
	 * @param string font_name Name of the font.
	 */
	int Font_ctor(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			Font *font = new Font(lua_tostring(lua, 1));
			wrapRefObject<Font>(lua, font);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@new", "string font_name");
	}
	/**
	 * Releases the reference counter on the font.
	 */
	int Font_dtor(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			font->release();
		}
		return 0;
	}
	/**
	 * Compares this font with another font object.
	 *
	 * @param am.font rhs The other font to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int Font_eq(lua_State *lua)
	{
		Font *lhs = castUData<Font>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.font");
		}
		Font *rhs = castUData<Font>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Font_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", TextStyle_ctor },
			{ "__gc", TextStyle_dtor },
			{ "__eq", TextStyle_eq },
			{ "colour", nullptr },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, TextStyle::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	
}
}
}
