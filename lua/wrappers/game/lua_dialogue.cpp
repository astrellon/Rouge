#include "lua_dialogue.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>

#include <game/game.h>
#include <game/engine.h>

#include <lua/wrappers/lua_event_manager.h>

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * The dialogue class represents a single dialogue segment.
	 * Each segment has a wall of text, a title, a subject, an unlock flag
	 * and an action.
	 * <br>
	 * If a dialogue is locked, it can only be unlocked by subject.
	 * Once an unlocked dialogue with a given subject is activated, all
	 * locked dialogues with the same subject are unlocked.
	 */
	/**
	 * Creates a new dialogue.
	 * @param string dialogue_id The unique id for this dialogue.
	 * @param string text The dialogue text.
	 * @param string [""] title The title for this dialogue.
	 * @param string [""] subject The subject of this dialogue.
	 * @param string ["none"] unlock_flag Either "none" or "locked".
	 * @param string ["none"] action Either "none", "shop" or "close".
	 */
	int Dialogue_ctor(lua_State *lua)
	{
		if (lua_isstr(lua, 1) && lua_isstr(lua, 2))
		{
            std::string title;
            std::string subject;
            Dialogue::UnlockFlag flag = Dialogue::UNLOCK_NONE;
            Dialogue::DialogueAction action = Dialogue::ACTION_NONE;
            
            LuaState L(lua);
            if (lua_istable(lua, 3))
            {
                L.getTableString("title", title, 3);
                L.getTableString("subject", subject, 3);
                std::string temp;
                if (L.getTableString("unlock", temp, 3))
                {
			        flag = Dialogue::toUnlockFlag(temp.c_str());
                }
                if (L.getTableString("action", temp, 3))
                {
                    action = Dialogue::toDialogueAction(temp.c_str());
                }
            }
			Dialogue *dialogue = new Dialogue(lua_tostring(lua, 1), lua_tostring(lua, 2),
				title.c_str(), subject.c_str(), flag, action);
            if (lua_istable(lua, 3))
            {
                int funcRef = LUA_REFNIL;
                if (L.getTableFunc("dialogue", funcRef))
                {
                    lua::ui::addEventListener<Dialogue>(lua, dialogue, "dialogue", funcRef);
                }
            }
			wrapRefObject<Dialogue>(lua, dialogue);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@new", "string dialogue_id, string text, string title [\"\"], string subject [\"\"], string unlock_flag [\"none\"], string action [\"none\"]");
	}
	/**
	 * The dialogue won't be destroyed until it is removed from the game engine.
	 * @see remove_dialogue()
	 */
	int Dialogue_dtor(lua_State *lua)
	{
		return 0;
	}
	/**
	 * Compares this dialogue with another dialogue object.
	 *
	 * @param am.dialogue rhs The other dialogue to compare with.
	 * @returns boolean True if they are the same dialogue object.
	 */
	int Dialogue_eq(lua_State *lua)
	{
		Dialogue *lhs = castUData<Dialogue>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.dialogue");
		}
		Dialogue *rhs = castUData<Dialogue>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

    int Dialogue_test(lua_State *lua)
    {
        Dialogue *diag = castUData<Dialogue>(lua, 1);
        if (diag)
        {
            if (lua_istable(lua, 2))
            {
                lua::LuaState L(lua);
                const char *str = L.getTableString("subject");
                if (str)
                {
                    am_log("Subject", str);
                }
                else
                {
                    am_log("Subject", "nil");
                }

                str = L.getTableString("action");
                if (str)
                {
                    am_log("Action", str);
                }
                else
                {
                    am_log("Action", "nil");
                }
            }
        }
        return 0;
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
			{ "unlock", Dialogue_unlock_flag },
			{ "action", Dialogue_dialogue_action },
			// Static
			{ "add_dialogue", Dialogue_add_dialogue },
			{ "remove_dialogue", Dialogue_remove_dialogue },
			{ "remove_all_dialogue", Dialogue_remove_all_dialogue },
			{ "dialogue", Dialogue_get_dialogue },
            // EventInterface
            { "on", Dialogue_add_event_listener },
            { "off", Dialogue_remove_event_listener },
            { "has_event_listener", Dialogue_has_event_listener },

            { "test", Dialogue_test },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, Dialogue::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");
		
		return 1;
	}

	/**
	 * Returns the raw dialogue text.
	 * @returns string The raw dialogue text.
	 */
	/**
	 * Sets the raw dialogue text.
	 * @param string text The new dialogue text.
	 * @returns am.dialogue This
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
			else if (lua_isstr(lua, 2))
			{
				diag->setText(lua_tostring(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "text", "string text");
		}
		return LuaState::expectedContext(lua, "text", "am.dialogue");
	}
	/**
	 * Returns the title for this dialogue.
	 * @returns string The title for this dialogue.
	 */
	/**
	 * Sets the dialogue title.
	 * @param string title The new dialogue title.
	 * @returns am.dialogue This
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
			else if (lua_isstr(lua, 2))
			{
				diag->setTitle(lua_tostring(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "title", "string title");
		}
		return LuaState::expectedContext(lua, "title", "am.dialogue");
	}
	/**
	 * Returns the unique id for this dialogue.
	 * @returns string This dialogues unique id.
	 */
	/**
	 * Sets the dialogues id.
	 * @param string id The dialogue's new id.
	 * @returns am.dialogue This
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
			else if (lua_isstr(lua, 2))
			{
				diag->setId(lua_tostring(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "id", "string id");
		}
		return LuaState::expectedContext(lua, "id", "am.dialogue");
	}
	/**
	 * Returns the subject title for this dialogue, this can be an empty string.
	 * @returns string The subject title.
	 */
	/**
	 * Sets the subject title for this dialogue. The subject can determine
	 * if a dialogue option is unlocked, as once any dialogue with a given subject
	 * is unlocked, all dialogue's with the same subject are unlocked.
	 * @param string subject The subject title for this dialogue.
	 * @returns am.dialogue This
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
			else if (lua_isstr(lua, 2))
			{
				diag->setSubject(lua_tostring(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "subject", "string subject");
		}
		return LuaState::expectedContext(lua, "subject", "am.dialogue");
	}
	
	/**
	 * Returns the dialogue unlock flag. This determines if the dialogue
	 * if locked by subject from the start or if it is always unlocked.
	 *
	 * @returns string Either "none" or "locked"
	 */
	/**
	 * Sets how the dialogue is unlocked, it is either locked by subject or
	 * always unlocked.
	 * 
	 * @param string flag Either "none" or "locked"
	 * @returns am.dialogue This
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
			else if (lua_isstr(lua, 2))
			{
				Dialogue::UnlockFlag flag = getUnlockFlag(lua, 2);
				if (flag != Dialogue::UNLOCK_UNKNOWN)
				{
					diag->setUnlockFlag(flag);
				}
				else
				{
					LuaState::warning(lua, "Valid dialogue unlock flag values are \"none\" or \"locked\"");
				}
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "unlock", "string flag");
		}
		return LuaState::expectedContext(lua, "unlock", "am.dialogue");
	}

	/**
	 * Returns the specific action associated with this dialogue.
	 * @returns string Either "none", "shop" or "close"
	 */
	/**
	 * Sets the dialogue action, these trigger special events when the
	 * dialogue is used.
	 * @param string action Either "none", "shop" or "close"
	 * @returns am.dialogue This
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
			else if (lua_isstr(lua, 2))
			{
				Dialogue::DialogueAction action = getDialogueAction(lua, 2);
				if (action != Dialogue::ACTION_UNKNOWN)
				{
					diag->setDialogueAction(action);
				}
				else
				{
					LuaState::warning(lua, "Valid dialogue action values are \"none\", \"shop\" or \"close\"");
				}
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "action", "string action");
		}
		return LuaState::expectedContext(lua, "action", "am.dialogue");
	}
	/**
	 * @static
	 * Adds a dialogue to the game engines dialogue pool.
	 * Dialogues in the dialogue pool can be automatically accessed
	 * by the dialogue system.
	 * @param am.dialogue dialogue The dialogue to add.
	 * @returns boolean True if the dialogue was successfully added.
	 */
	int Dialogue_add_dialogue(lua_State *lua)
	{
		Dialogue *diag = castUData<Dialogue>(lua, 1);
		if (diag)
		{
			lua_pushboolean(lua, Engine::getGame()->addDialogue(diag));
			return 1;
		}
		return LuaState::expectedArgs(lua, "@add_dialogue", "am.dialogue dialogue");
	}
	/**
	 * @static
	 * Removes a dialogue from the game engines dialogue pool.
	 * @param string dialogue_id The dialogue id to remove.
	 * @returns boolean True if the dialogue was successfully removed.
	 */
	int Dialogue_remove_dialogue(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			lua_pushboolean(lua, Engine::getGame()->removeDialogue(lua_tostring(lua, 1)));
			return 1;
		}
		return LuaState::expectedArgs(lua, "@remove_dialogue", "string dialogue_id");
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
	 * @param string dialogue_id The dialogue id to search for.
	 * @returns am.dialogue The found dialogue or nil.
	 */
	int Dialogue_get_dialogue(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			Dialogue *diag = Engine::getGame()->getDialogue(lua_tostring(lua, 1));
			if (diag)
			{
				wrapRefObject<Dialogue>(lua, diag);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@dialogue", "string dialogue_id");
	}
    /**
	 * Adds an event listener for an event fired on this character.
	 * eg: <pre>
	 * character:on("talkTo", function(event)
	 *     am_log("Character talked to")
	 * end)
	 * </pre>
	 * @param string event_type The event type or name to trigger on
	 * @param function listener The function to call when the event is fired.
	 * @param table [nil] content An option context for the listener to be
	 * called with.
	 * @returns boolean True if the event was added successfully.
	 */
	int Dialogue_add_event_listener(lua_State *lua)
	{
		Dialogue *obj = castUData<Dialogue>(lua, 1);
		if (obj)
		{
			return ui::EventInterface_add_event_listener(lua, obj);
		}
		return LuaState::expectedContext(lua, "on", "am.dialogue");
	}
	/**
	 * Removes an event listener from this character.
	 * eg: 
	 * <pre>
	 * function talkToOnce(event)
	 *     am_log("Character talked to once")
	 *     character:off("talkTo", talkToOnce)
	 * end
	 * character:on("talkTo", talkToOnce)
	 * </pre>
	 * @param string event_type The event type the listener was listening for.
	 * @param function listener The listener function to remove.
	 * @param table [nil] context The context which the listener was going to 
	 * be called with, this is only optional if the listener was added with no context.
	 * @returns boolean True if the event listener was successfully removed.
	 */
	int Dialogue_remove_event_listener(lua_State *lua)
	{
		Dialogue *obj = castUData<Dialogue>(lua, 1);
		if (obj)
		{
			return ui::EventInterface_remove_event_listener(lua, obj);
		}
		return LuaState::expectedContext(lua, "off", "am.dialogue");
	}
	/**
	 * Returns true when there is an event listener for the given event_type.
	 * @param string event_type The event type to look up.
	 * @returns boolean True if there is any event listener 
	 * that will be trigged by this event type.
	 */
	int Dialogue_has_event_listener(lua_State *lua)
	{
		Dialogue *obj = castUData<Dialogue>(lua, 1);
		if (obj)
		{
			return ui::EventInterface_has_event_listener(lua, obj);
		}
		return LuaState::expectedContext(lua, "has_event_listener", "am.dialogue");
	}


	Dialogue::UnlockFlag getUnlockFlag(lua_State *lua, int n)
	{
		if (lua_isstr(lua, n))
		{
			return Dialogue::toUnlockFlag(lua_tostring(lua, n));
		}
		if (lua_isnum(lua, n))
		{
			return Dialogue::toUnlockFlag(lua_tointeger(lua, n));
		}
		return Dialogue::UNLOCK_UNKNOWN;
	}
	Dialogue::DialogueAction getDialogueAction(lua_State *lua, int n)
	{
		if (lua_isstr(lua, n))
		{
			return Dialogue::toDialogueAction(lua_tostring(lua, n));
		}
		if (lua_isnum(lua, n))
		{
			return Dialogue::toDialogueAction(lua_tointeger(lua, n));
		}
		return Dialogue::ACTION_UNKNOWN;
	}

}
}
}
