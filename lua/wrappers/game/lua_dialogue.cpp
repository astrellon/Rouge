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
	/**
	 * @class
	 * The Dialogue class represents a single dialogue segment.
	 * Each segment has a wall of text, a title, a subject, an unlock flag
	 * and an action.
	 * <br>
	 * If a dialogue is locked, it can only be unlocked by subject.
	 * Once an unlocked dialogue with a given subject is activated, all
	 * locked dialogues with the same subject are unlocked.
	 */
	/**
	 * Creates a new dialogue.
	 * @param String dialogueId The unique id for this dialogue.
	 * @param String text The dialogue text.
	 * @param String [""] title The title for this dialogue.
	 * @param String [""] subject The subject of this dialogue.
	 * @param String ["none"] unlock_flag Either "none" or "locked".
	 * @param String ["none"] action Either "none", "shop" or "close".
	 */
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
	/**
	 * The Dialogue won't be destroyed until it is removed from the game engine.
	 * @see remove_dialogue()
	 */
	int Dialogue_dtor(lua_State *lua)
	{
		return 0;
	}
	/**
	 * Compares this Dialogue with another Dialogue object.
	 *
	 * @param Dialogue rhs The other Dialogue to compare with.
	 * @returns Boolean True if they are the same Dialogue object.
	 */
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
			{ "text", Dialogue_text },
			{ "title", Dialogue_title },
			{ "id", Dialogue_id },
			{ "subject", Dialogue_subject },
			{ "unlock_flag", Dialogue_unlock_flag },
			{ "dialogue_action", Dialogue_dialogue_action },
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

	/**
	 * Returns the raw dialogue text.
	 * @returns String The raw dialogue text.
	 */
	/**
	 * Sets the raw dialogue text.
	 * @param String text The new dialogue text.
	 * @returns Dialogue This
	 */
	int Dialogue_text(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, diag->getText());
				return 1;
			}
			else if (lua_isstring(lua, -1))
			{
				diag->setText(lua_tostring(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the title for this dialogue.
	 * @returns String The title for this dialogue.
	 */
	/**
	 * Sets the dialogue title.
	 * @param String title The new dialogue title.
	 * @returns Dialogue This
	 */
	int Dialogue_title(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, diag->getTitle());
				return 1;
			}
			else if (lua_isstring(lua, -1))
			{
				diag->setTitle(lua_tostring(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the unique id for this dialogue.
	 * @returns String This dialogues unique id.
	 */
	/**
	 * Sets the dialogues id.
	 * @param String id The dialogue's new id.
	 * @returns Dialogue This
	 */
	int Dialogue_id(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, diag->getId());
				return 1;
			}
			else if (lua_isstring(lua, -1))
			{
				diag->setId(lua_tostring(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the subject title for this dialogue, this can be an empty string.
	 * @returns String The subject title.
	 */
	/**
	 * Sets the subject title for this dialogue. The subject can determine
	 * if a dialogue option is unlocked, as once any dialogue with a given subject
	 * is unlocked, all dialogue's with the same subject are unlocked.
	 * @param String subject The subject title for this dialogue.
	 * @returns Dialogue This
	 */
	int Dialogue_subject(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, diag->getSubject());
				return 1;
			}
			else if (lua_isstring(lua, -1))
			{
				diag->setSubject(lua_tostring(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	
	/**
	 * Returns the dialogue unlock flag. This determines if the dialogue
	 * if locked by subject from the start or if it is always unlocked.
	 *
	 * @returns String Either "none" or "locked"
	 */
	/**
	 * Sets how the dialogue is unlocked, it is either locked by subject or
	 * always unlocked.
	 * 
	 * @param String flag Either "none" or "locked"
	 * @returns Dialogue This
	 */
	int Dialogue_unlock_flag(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, Dialogue::getUnlockFlagName(diag->getUnlockFlag()));
				return 1;
			}
			else
			{
				Dialogue::UnlockFlag flag = getUnlockFlag(lua, -1);
				if (flag != Dialogue::UNLOCK_UNKNOWN)
				{
					diag->setUnlockFlag(flag);
				}
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	/**
	 * Returns the specific action associated with this dialogue.
	 * @returns String Either "none", "shop" or "close"
	 */
	/**
	 * Sets the dialogue action, these trigger special events when the
	 * dialogue is used.
	 * @param String Either "none", "shop" or "close"
	 * @returns Dialogue This
	 */
	int Dialogue_dialogue_action(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, Dialogue::getDialogueActionName(diag->getDialogueAction()));
				return 1;
			}
			else
			{
				Dialogue::DialogueAction action = getDialogueAction(lua, -1);
				if (action != Dialogue::ACTION_UNKNOWN)
				{
					diag->setDialogueAction(action);
				}
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * @static
	 * Adds a dialogue to the game engines dialogue pool.
	 * Dialogues in the dialogue pool can be automatically accessed
	 * by the dialogue system.
	 * @param Dialogue dialogue The dialogue to add.
	 * @returns Boolean True if the dialogue was successfully added.
	 */
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
	/**
	 * @static
	 * Removes a dialogue from the game engines dialogue pool.
	 * @param String dialogueId The dialogue id to remove.
	 * @returns Boolean True if the dialogue was successfully removed.
	 */
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
	/**
	 * @static
	 * Removes all dialogue from the game engine.
	 */
	int Dialogue_remove_all_dialogue(lua_State *lua)
	{
		Engine::getGame()->removeAllDialogue();
		return 0;
	}
	/**
	 * @static
	 * Finds a dialogue in the dialogue pool with the given dialogue id.
	 * @param String dialogueId The dialogue id to search for.
	 * @returns Dialogue The found dialogue or nil.
	 */
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
