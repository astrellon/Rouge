#include "lua_game_object.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/game_object.h>
using namespace am::game;

namespace am {
namespace lua {
namespace game {

	int l_GameObject_ctor(lua_State *lua)
	{
		const char *id = lua_tostring(lua, -1);
		if (id == NULL)
		{
			lua_pushnil(lua);
			return 1;
		}

		GameObject *obj = GameObject::getByGameId(id);
		if (obj == NULL)
		{
			obj = new GameObject();
			obj->setGameId(id);
		}

		GameObject ** udata = (GameObject **)lua_newuserdata(lua, sizeof(GameObject *));
		*udata = obj;

		luaL_getmetatable(lua, "luaL_am_GameObject");
		lua_setmetatable(lua, -2);
		return 1;
	}

	int l_GameObject_dtor(lua_State *lua)
	{
		return 0;
	}

	int l_GameObject_register(lua_State *lua)
	{
		luaL_Reg GameObjectRegs[] = 
		{
			{ "new", l_GameObject_ctor },
			{ "get_name", l_GameObject_get_name },
			{ "set_name", l_GameObject_set_name },
			{ "__gc", l_GameObject_dtor },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, "luaL_am_GameObject");
		luaL_setfuncs(lua, GameObjectRegs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		LuaState ll(lua);

		return 1;
	}

	GameObject *l_Check_GameObject(lua_State *lua, int n)
	{
		return *(GameObject **)luaL_checkudata(lua, n, "luaL_am_GameObject");
	}

	int l_GameObject_get_name(lua_State *lua)
	{
		GameObject *obj = l_Check_GameObject(lua, 1);
		if (obj)
		{
			lua_pushstring(lua, obj->getName().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int l_GameObject_set_name(lua_State *lua)
	{
		LuaState ll(lua);
		GameObject *obj = l_Check_GameObject(lua, 1);
		if (obj)
		{
			const char *name = luaL_checkstring(lua, 2);
			if (name) 
			{
				obj->setName(name);
			}
		}
		return 0;
	}
	
}
}
}
