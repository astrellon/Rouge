#include "lua_quest.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/lua_based_quest.h>
using namespace am::game;

#include <lua/wrappers/lua_event_manager.h>

namespace am {
namespace lua {
namespace game {

	int Quest_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 1 && lua_isstring(lua, -1))
		{
			LuaQuest *quest = new LuaQuest(lua_tostring(lua, -1));
			Quest_wrap(lua, quest);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	void Quest_wrap(lua_State *lua, LuaQuest *quest)
	{
		LuaQuest ** udata = (LuaQuest **)lua_newuserdata(lua, sizeof(LuaQuest *));
		*udata = quest;

		luaL_getmetatable(lua, Quest_tableName);
		lua_setmetatable(lua, -2);
	}

	int Quest_dtor(lua_State *lua)
	{
		Quest *quest = Check_Quest(lua, 1);
		if (quest)
		{
			delete quest;
		}
		return 0;
	}

	int Quest_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Quest_ctor },
			{ "__gc",  Quest_dtor },
			{ "set_complete", Quest_set_complete },
			{ "is_completed", Quest_is_completed },
			{ "get_title", Quest_get_title },
			{ "get_description", Quest_get_description },
			{ "get_active_text", Quest_get_active_text },
			{ "add_event_listener", Quest_add_event_listener },
			{ "remove_event_listener", Quest_remove_event_listener },
			{ "has_event_listener", Quest_has_event_listener },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Quest_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	LuaQuest *Check_Quest(lua_State *lua, int n)
	{
		return *(LuaQuest **)luaL_checkudata(lua, n, Quest_tableName);
	}


	int Quest_set_complete(lua_State *lua)
	{
		LuaQuest *quest = Check_Quest(lua, 1);
		if (quest && lua_isboolean(lua, -1))
		{
			quest->setCompleted(lua_toboolean(lua, -1) > 0);
		}
		return 0;
	}
	int Quest_is_completed(lua_State *lua)
	{
		LuaQuest *quest = Check_Quest(lua, 1);
		if (quest)
		{
			lua_pushboolean(lua, quest->isCompleted());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Quest_get_title(lua_State *lua)
	{
		LuaQuest *quest = Check_Quest(lua, 1);
		if (quest)
		{
			lua_pushstring(lua, quest->getTitle());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Quest_get_description(lua_State *lua)
	{
		LuaQuest *quest = Check_Quest(lua, 1);
		if (quest)
		{
			lua_pushstring(lua, quest->getDescription());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Quest_get_active_text(lua_State *lua)
	{
		LuaQuest *quest = Check_Quest(lua, 1);
		if (quest)
		{
			lua_pushstring(lua, quest->getActiveText());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Quest_add_event_listener(lua_State *lua)
	{
		LuaQuest *quest = Check_Quest(lua, 1);
		if (quest && lua_isstring(lua, 2) && lua_isfunction(lua, 3))
		{
			lua_pushboolean(lua, am::lua::ui::addEventListener(lua, quest));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Quest_remove_event_listener(lua_State *lua)
	{
		LuaQuest *quest = Check_Quest(lua, 1);
		if (quest && lua_isstring(lua, 2) && lua_isfunction(lua, 3))
		{
			lua_pushboolean(lua, am::lua::ui::removeEventListener(lua, quest));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Quest_has_event_listener(lua_State *lua)
	{
		LuaQuest *quest = Check_Quest(lua, 1);
		if (quest && lua_isstring(lua, -1))
		{
			lua_pushboolean(lua, quest->hasEventListener(lua_tostring(lua, -1)));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
