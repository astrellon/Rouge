#include "lua_dialogue.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/game.h>
#include <game/engine.h>
using namespace am::game;

namespace am {
namespace lua {
namespace game {

	int Dialogue_ctor(lua_State *lua)
	{
		if (lua_isstring(lua, 1) && lua_isstring(lua, 2))
		{
			int args = lua_gettop(lua);
			const char *title = NULL;
			if (args >= 3 && lua_isstring(lua, 3))
			{
				title = lua_tostring(lua, 3);
			}
			const char *subject = NULL;
			if (args >= 4 && lua_isstring(lua, 4))
			{
				subject = lua_tostring(lua, 4);
			}
			Dialogue::UnlockFlag flag = Dialogue::UNLOCK_NONE;
			if (args >= 5)
			{
				flag = getUnlockFlag(lua, 5);
				if (flag == Dialogue::UNLOCK_UNKNOWN)
				{
					flag = Dialogue::UNLOCK_NONE;
				}
			}
			Dialogue::DialogueAction action = Dialogue::ACTION_NONE;
			if (args >= 6)
			{
				action = getDialogueAction(lua, 6);
				if (action == Dialogue::ACTION_UNKNOWN)
				{
					action = Dialogue::ACTION_NONE;
				}
			}
			Dialogue *dialogue = new Dialogue(lua_tostring(lua, 1), lua_tostring(lua, 2),
				title, subject, flag, action);
			wrapObject<Dialogue>(lua, dialogue);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	
	int Dialogue_dtor(lua_State *lua)
	{
		return 0;
	}

	int Dialogue_eq(lua_State *lua)
	{
		Dialogue *lhs = castUData<Dialogue>(lua, 1);
		Dialogue *rhs = castUData<Dialogue>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Dialogue_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Dialogue_ctor },
			{ "__gc", Dialogue_dtor },
			{ "__eq", Dialogue_eq },
			{ "set_text", Dialogue_set_text },
			{ "get_text", Dialogue_get_text },
			{ "set_title", Dialogue_set_title },
			{ "get_title", Dialogue_get_title },
			{ "set_id", Dialogue_set_id },
			{ "get_id", Dialogue_get_id },
			{ "set_subject", Dialogue_set_subject },
			{ "get_subject", Dialogue_get_subject },
			{ "set_unlock_flag", Dialogue_set_unlock_flag },
			{ "get_unlock_flag", Dialogue_get_unlock_flag },
			{ "set_dialogue_action", Dialogue_set_dialogue_action },
			{ "get_dialogue_action", Dialogue_get_dialogue_action },
			// Static
			{ "add_dialogue", Dialogue_add_dialogue },
			{ "remove_dialogue", Dialogue_remove_dialogue },
			{ "remove_all_dialogue", Dialogue_remove_all_dialogue },
			{ "get_dialogue", Dialogue_get_dialogue },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Dialogue::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");
		
		return 1;
	}

	int Dialogue_set_text(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag && lua_isstring(lua, -1))
		{
			diag->setText(lua_tostring(lua, -1));
		}
		return 0;
	}
	int Dialogue_get_text(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			lua_pushstring(lua, diag->getText());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Dialogue_set_title(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag && lua_isstring(lua, -1))
		{
			diag->setTitle(lua_tostring(lua, -1));
		}
		return 0;
	}
	int Dialogue_get_title(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			lua_pushstring(lua, diag->getTitle());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Dialogue_set_id(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag && lua_isstring(lua, -1))
		{
			diag->setId(lua_tostring(lua, -1));
		}
		return 0;
	}
	int Dialogue_get_id(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			lua_pushstring(lua, diag->getId());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Dialogue_set_subject(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag && lua_isstring(lua, -1))
		{
			diag->setSubject(lua_tostring(lua, -1));
		}
		return 0;
	}
	int Dialogue_get_subject(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			lua_pushstring(lua, diag->getSubject());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Dialogue_set_unlock_flag(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			Dialogue::UnlockFlag flag = getUnlockFlag(lua, -1);
			if (flag != Dialogue::UNLOCK_UNKNOWN)
			{
				diag->setUnlockFlag(flag);
			}
		}
		return 0;
	}
	int Dialogue_get_unlock_flag(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			lua_pushstring(lua, Dialogue::getUnlockFlagName(diag->getUnlockFlag()));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Dialogue_set_dialogue_action(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			Dialogue::DialogueAction action = getDialogueAction(lua, -1);
			if (action != Dialogue::ACTION_UNKNOWN)
			{
				diag->setDialogueAction(action);
			}
		}
		return 0;
	}
	int Dialogue_get_dialogue_action(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			lua_pushstring(lua, Dialogue::getDialogueActionName(diag->getDialogueAction()));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Dialogue_add_dialogue(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			lua_pushboolean(lua, Engine::getGame()->addDialogue(diag));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Dialogue_remove_dialogue(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			lua_pushboolean(lua, Engine::getGame()->removeDialogue(lua_tostring(lua, -1)));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Dialogue_remove_all_dialogue(lua_State *lua)
	{
		Engine::getGame()->removeAllDialogue();
		return 0;
	}
	int Dialogue_get_dialogue(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			Dialogue *diag = Engine::getGame()->getDialogue(lua_tostring(lua, -1));
			if (diag)
			{
				wrapObject<Dialogue>(lua, diag);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	Dialogue::UnlockFlag getUnlockFlag(lua_State *lua, int n)
	{
		if (lua_isstring(lua, n))
		{
			return Dialogue::toUnlockFlag(lua_tostring(lua, n));
		}
		if (lua_isnumber(lua, n))
		{
			return Dialogue::toUnlockFlag(lua_tointeger(lua, n));
		}
		return Dialogue::UNLOCK_UNKNOWN;
	}
	Dialogue::DialogueAction getDialogueAction(lua_State *lua, int n)
	{
		if (lua_isstring(lua, n))
		{
			return Dialogue::toDialogueAction(lua_tostring(lua, n));
		}
		if (lua_isnumber(lua, n))
		{
			return Dialogue::toDialogueAction(lua_tointeger(lua, n));
		}
		return Dialogue::ACTION_UNKNOWN;
	}

}
}
}
