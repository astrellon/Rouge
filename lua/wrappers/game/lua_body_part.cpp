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
#include <game/item.h>
using namespace am::game;

#include <lua/wrappers/game/lua_item.h>

namespace am {
namespace lua {
namespace game {

	int BodyPart_ctor(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			const char *partName = lua_tostring(lua, -1);
			BodyPart *part = new BodyPart(partName);
		
			wrapObject<BodyPart>(lua, part);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int BodyPart_dtor(lua_State *lua)
	{
		BodyPart *part = castUData<BodyPart>(lua, 1);
		if (part)
		{
			delete part;
		}
		return 0;
	}

	int BodyPart_eq(lua_State *lua)
	{
		BodyPart *lhs = castUData<BodyPart>(lua, 1);
		BodyPart *rhs = castUData<BodyPart>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int BodyPart_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", BodyPart_ctor },
			{ "__gc",  BodyPart_dtor },
			{ "__eq", BodyPart_eq },
			{ "get_name", BodyPart_get_name },
			{ "set_equipped_item", BodyPart_set_equipped_item },
			{ "get_equipped_item", BodyPart_get_equipped_item },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, BodyPart::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	int BodyPart_get_name(lua_State *lua)
	{
		BodyPart *part = castUData<BodyPart>(lua, 1);
		if (part)
		{
			lua_pushstring(lua, part->getName());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int BodyPart_set_equipped_item(lua_State *lua)
	{
		BodyPart *part = castUData<BodyPart>(lua, 1);
		Item *item = castUData<Item>(lua, 2);
		if (part)
		{
			part->setEquippedItem(item);
		}
		return 0;
	}
	int BodyPart_get_equipped_item(lua_State *lua)
	{
		BodyPart *part = castUData<BodyPart>(lua, 1);
		if (part)
		{
			Item *item = part->getEqippedItem();
			if (item)
			{
				wrapRefObject<Item>(lua, item);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
}
}
}
