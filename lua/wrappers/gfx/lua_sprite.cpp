#include "lua_sprite.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <gfx/gfx_sprite.h>
using namespace am::gfx;

namespace am {
namespace lua {
namespace gfx {

	int Sprite_ctor(lua_State *lua)
	{
		if (lua_isstring(lua, 1))
		{
			Sprite *sprite = new Sprite(lua_tostring(lua, 1));
			Sprite_wrap(lua, sprite);
			return 1;
		}
		Sprite *sprite = new Sprite();
		Sprite_wrap(lua, sprite);
		return 1;
	}
	void Sprite_wrap(lua_State *lua, Sprite *sprite)
	{
		Sprite ** udata = (Sprite **)lua_newuserdata(lua, sizeof(Sprite *));
		*udata = sprite;

		luaL_getmetatable(lua, Sprite_tableName);
		lua_setmetatable(lua, -2);
	}

	int Sprite_dtor(lua_State *lua)
	{
		Sprite *sprite = Check_Sprite(lua, 1);
		if (sprite)
		{
			sprite->release();
		}
		return 0;
	}

	int Sprite_eq(lua_State *lua)
	{
		Sprite *lhs = Check_Sprite(lua, 1);
		Sprite *rhs = Check_Sprite(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Sprite_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Sprite_ctor },
			{ "__gc", Sprite_dtor },
			{ "__eq", Sprite_eq },
			{ "set_asset", Sprite_set_asset },
			{ "get_asset", NULL },
			{ "set_frame_rate", NULL },
			{ "get_frame_rate", NULL },
			{ "set_current_frame", NULL },
			{ "get_current_frame", NULL },
			{ "set_frame_time", NULL },
			{ "get_frame_time", NULL },
			{ "get_width", NULL },
			{ "get_height", NULL },
			// Renderable methods
			{ "set_size", NULL },
			{ "get_size", NULL },
			{ "set_width", NULL },
			{ "set_height", NULL },
			{ "set_parent", NULL },
			{ "get_parent", NULL },
			{ "set_interactive", NULL },
			{ "is_interactive", NULL },
			{ "set_visible", NULL },
			{ "is_visible", NULL },
			{ "set_position", NULL },
			{ "get_position", NULL },
			{ "get_screen_to_local", NULL },
			{ "get_local_to_screen", NULL },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Sprite_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	Sprite *Check_Sprite(lua_State *lua, int n)
	{
		return *(Sprite **)luaL_checkudata(lua, n, Sprite_tableName);
	}

	int Sprite_set_asset(lua_State *lua)
	{
		Sprite *sprite = Check_Sprite(lua, 1);
		if (sprite)
		{
			if (lua_isstring(lua, -1))
			{
				sprite->setAsset(lua_tostring(lua, -1));
			}
		}
		return 0;
	}

}
}
}
