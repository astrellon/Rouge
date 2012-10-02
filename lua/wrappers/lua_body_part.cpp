#include "lua_body_part.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/body_part.h>
using namespace am::game;

namespace am {
namespace lua {
namespace game {

	int BodyPart_ctor(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			const char *partName = lua_tostring(lua, -1);
			BodyPart *part = new BodyPart(partName);
		
			BodyPart_wrap(lua, part);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	void BodyPart_wrap(lua_State *lua, BodyPart *part)
	{
		BodyPart ** udata = (BodyPart **)lua_newuserdata(lua, sizeof(BodyPart *));
		*udata = part;

		luaL_getmetatable(lua, BodyPart_tableName);
		lua_setmetatable(lua, -2);
	}

	int BodyPart_dtor(lua_State *lua)
	{
		return 0;
	}

	int BodyPart_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", BodyPart_ctor },
			{ "__gc",  BodyPart_dtor },
			{ "get_name", BodyPart_get_name },
			{ "set_equipped_item", NULL },
			{ "get_equipped_item", NULL },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, BodyPart_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	BodyPart *Check_BodyPart(lua_State *lua, int n)
	{
		return *(BodyPart **)luaL_checkudata(lua, n, BodyPart_tableName);
	}

	int BodyPart_get_name(lua_State *lua)
	{
		BodyPart *part = Check_BodyPart(lua, 1);
		if (part)
		{
			lua_pushstring(lua, part->getName());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
