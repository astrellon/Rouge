#include "lua_dialogue_component.h"

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

#include "lua_dialogue.h"
#include "lua_game.h"

namespace am {
namespace lua {
namespace game {

	int DialogueComponent_ctor(lua_State *lua)
	{
		GameObject *obj = getGameObject(lua, -1);
		DialogueComponent *comp = new DialogueComponent(obj);
		DialogueComponent_wrap(lua, comp);
		return 1;
	}
	void DialogueComponent_wrap(lua_State *lua, DialogueComponent *dialogue)
	{
		DialogueComponent ** udata = (DialogueComponent **)lua_newuserdata(lua, sizeof(DialogueComponent *));
		*udata = dialogue;

		luaL_getmetatable(lua, DialogueComponent_tableName);
		lua_setmetatable(lua, -2);
	}

	int DialogueComponent_dtor(lua_State *lua)
	{
		return 0;
	}

	int DialogueComponent_eq(lua_State *lua)
	{
		DialogueComponent *lhs = Check_DialogueComponent(lua, 1);
		DialogueComponent *rhs = Check_DialogueComponent(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int DialogueComponent_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", DialogueComponent_ctor },
			{ "__gc", DialogueComponent_dtor },
			{ "__eq", DialogueComponent_eq },
			{ "talk_to", DialogueComponent_talk_to },
			{ "get_talking_to", DialogueComponent_get_talking_to },
			{ "set_start_dialogue", DialogueComponent_set_start_dialogue },
			{ "get_start_dialogue", DialogueComponent_get_start_dialogue },
			{ "set_subject_lock", DialogueComponent_set_subject_lock },
			{ "is_subject_locked", DialogueComponent_is_subject_locked },
			{ "get_unlocked_subjects", NULL },
			{ "set_dialogue_available", DialogueComponent_set_dialogue_available },
			{ "is_dialogue_available", DialogueComponent_is_dialogue_available },
			{ "get_dialogues_available", NULL },
			{ "set_attached_to", DialogueComponent_set_attached_to },
			{ "get_attached_to", DialogueComponent_get_attached_to },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, DialogueComponent_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");
		
		return 1;
	}

	DialogueComponent *Check_DialogueComponent(lua_State *lua, int n)
	{
		return *(DialogueComponent **)luaL_checkudata(lua, n, DialogueComponent_tableName);
	}

	int DialogueComponent_talk_to(lua_State *lua)
	{
		DialogueComponent *comp = Check_DialogueComponent(lua, 1);
		GameObject *other = getGameObject(lua, 2);
		if (comp && other)
		{
			Dialogue *diag = Check_Dialogue(lua, 3);
			if (diag)
			{
				comp->talkTo(other, diag);
			}
			else
			{
				comp->talkTo(other);
			}
		}
		return 0;
	}
	int DialogueComponent_get_talking_to(lua_State *lua)
	{
		DialogueComponent *comp = Check_DialogueComponent(lua, 1);
		if (comp)
		{
			wrapGameObject(lua, comp->getTalkingTo());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int DialogueComponent_set_start_dialogue(lua_State *lua)
	{
		DialogueComponent *comp = Check_DialogueComponent(lua, 1);
		if (comp)
		{
			Dialogue *diag = NULL;
			if (lua_isstring(lua, -1))
			{
				diag = Engine::getGame()->getDialogue(lua_tostring(lua, -1));
			}
			else
			{
				diag = Check_Dialogue(lua, -1);
			}
			comp->setStartDialogue(diag);
		}
		return 0;
	}
	int DialogueComponent_get_start_dialogue(lua_State *lua)
	{
		DialogueComponent *comp = Check_DialogueComponent(lua, 1);
		if (comp)
		{
			Dialogue *diag = comp->getStartDialogue();
			if (diag)
			{
				Dialogue_wrap(lua, diag);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	int DialogueComponent_set_subject_lock(lua_State *lua)
	{
		DialogueComponent *comp = Check_DialogueComponent(lua, 1);
		if (comp && lua_isstring(lua, 2))
		{
			if (lua_isboolean(lua, 3))
			{
				comp->setSubjectLock(lua_tostring(lua, 2), lua_tobool(lua, 3));
			}
			else
			{
				comp->setSubjectLock(lua_tostring(lua, 2));
			}
		}
		return 0;
	}
	int DialogueComponent_is_subject_locked(lua_State *lua)
	{
		DialogueComponent *comp = Check_DialogueComponent(lua, 1);
		if (comp && lua_isstring(lua, 2))
		{
			lua_pushboolean(lua, comp->isSubjectLocked(lua_tostring(lua, 2)));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}

	int DialogueComponent_set_dialogue_available(lua_State *lua)
	{
		DialogueComponent *comp = Check_DialogueComponent(lua, 1);
		if (comp && lua_isstring(lua, 2))
		{
			if (lua_isboolean(lua, 3))
			{
				comp->setDialogueAvailable(lua_tostring(lua, 2), lua_tobool(lua, 3));
			}
			else
			{
				comp->setDialogueAvailable(lua_tostring(lua, 2));
			}
		}
		return 0;
	}
	int DialogueComponent_is_dialogue_available(lua_State *lua)
	{
		DialogueComponent *comp = Check_DialogueComponent(lua, 1);
		if (comp && lua_isstring(lua, 2))
		{
			lua_pushboolean(lua, comp->isDialogueAvailable(lua_tostring(lua, 2)));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}

	int DialogueComponent_set_attached_to(lua_State *lua)
	{
		DialogueComponent *comp = Check_DialogueComponent(lua, 1);
		GameObject *obj = getGameObject(lua, 2);
		if (comp)
		{
			comp->setAttachedTo(obj);
		}
		return 0;
	}
	int DialogueComponent_get_attached_to(lua_State *lua)
	{
		DialogueComponent *comp = Check_DialogueComponent(lua, 1);
		if (comp)
		{
			wrapGameObject(lua, comp->getAttachedTo());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
