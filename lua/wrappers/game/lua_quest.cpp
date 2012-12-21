#include "lua_quest.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/quest.h>
#include <game/engine.h>
#include <game/game.h>
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
			Quest *quest = new Quest(lua_tostring(lua, -1));
			Quest_wrap(lua, quest);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	void Quest_wrap(lua_State *lua, Quest *quest)
	{
		Quest ** udata = (Quest **)lua_newuserdata(lua, sizeof(Quest *));
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

	int Quest_eq(lua_State *lua)
	{
		Quest *lhs = Check_Quest(lua, 1);
		Quest *rhs = Check_Quest(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Quest_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Quest_ctor },
			{ "__gc",  Quest_dtor },
			{ "__eq", Quest_eq },
			{ "set_complete", Quest_set_complete },
			{ "is_completed", Quest_is_completed },
			{ "set_title", Quest_set_title },
			{ "get_title", Quest_get_title },
			{ "set_description", Quest_set_description },
			{ "get_description", Quest_get_description },
			{ "set_active_text", Quest_set_active_text },
			{ "get_active_text", Quest_get_active_text },
			{ "add_event_listener", Quest_add_event_listener },
			{ "remove_event_listener", Quest_remove_event_listener },
			{ "has_event_listener", Quest_has_event_listener },
			{ "add_quest", Quest_add_quest },
			{ "remove_quest", Quest_remove_quest },
			{ "get_quest", Quest_get_quest },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Quest_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	Quest *Check_Quest(lua_State *lua, int n)
	{
		return *(Quest **)luaL_checkudata(lua, n, Quest_tableName);
	}

	int Quest_set_complete(lua_State *lua)
	{
		Quest *quest = Check_Quest(lua, 1);
		if (quest && lua_isboolean(lua, -1))
		{
			quest->setCompleted(lua_toboolean(lua, -1) > 0);
		}
		return 0;
	}
	int Quest_is_completed(lua_State *lua)
	{
		Quest *quest = Check_Quest(lua, 1);
		if (quest)
		{
			lua_pushboolean(lua, quest->isCompleted());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Quest_set_title(lua_State *lua)
	{
		Quest *quest = Check_Quest(lua, 1);
		if (quest && lua_isstring(lua, -1))
		{
			quest->setTitle(lua_tostring(lua, -1));
		}
		return 0;
	}
	int Quest_get_title(lua_State *lua)
	{
		Quest *quest = Check_Quest(lua, 1);
		if (quest)
		{
			lua_pushstring(lua, quest->getTitle());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Quest_set_description(lua_State *lua)
	{
		Quest *quest = Check_Quest(lua, 1);
		if (quest && lua_isstring(lua, -1))
		{
			quest->setDescription(lua_tostring(lua, -1));
		}
		return 0;
	}
	int Quest_get_description(lua_State *lua)
	{
		Quest *quest = Check_Quest(lua, 1);
		if (quest)
		{
			lua_pushstring(lua, quest->getDescription());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Quest_set_active_text(lua_State *lua)
	{
		Quest *quest = Check_Quest(lua, 1);
		if (quest && lua_isstring(lua, -1))
		{
			quest->setActiveText(lua_tostring(lua, -1));
		}
		return 0;
	}
	int Quest_get_active_text(lua_State *lua)
	{
		Quest *quest = Check_Quest(lua, 1);
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
		Quest *quest = Check_Quest(lua, 1);
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
		Quest *quest = Check_Quest(lua, 1);
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
		Quest *quest = Check_Quest(lua, 1);
		if (quest && lua_isstring(lua, -1))
		{
			lua_pushboolean(lua, quest->hasEventListener(lua_tostring(lua, -1)));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Quest_add_quest(lua_State *lua)
	{
		Quest *quest = Check_Quest(lua, 1);
		if (quest)
		{
			lua_pushboolean(lua, Engine::getGame()->addQuest(quest));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Quest_remove_quest(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			lua_pushboolean(lua, Engine::getGame()->removeQuest(lua_tostring(lua, -1)));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Quest_get_quest(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			Quest *quest = dynamic_cast<Quest *>(Engine::getGame()->getQuest(lua_tostring(lua, -1)));
			if (quest)
			{
				Quest_wrap(lua, quest);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
