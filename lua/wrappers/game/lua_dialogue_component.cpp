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

	/**
	 * @class 
	 * The DialogueComponent class stores all the data to do with the dialogue
	 * that can be attached to a GameObject.<br>
	 * This includes the starting dialogue this component will open with, and all the currently
	 * available dialogue options, and which dialogues this component has unlocked.<br>
	 * GameObjects do not have a dialogue component by default so one must be
	 * created and attached.
	 */
	/**
	 * Creates a new DialogueComponent.
	 * @param GameObject [nil] attachTo Attaches this dialogue component to the given GameObject
	 *  if it is not null.
	 */
	int DialogueComponent_ctor(lua_State *lua)
	{
		GameObject *obj = NULL;
		if (lua_gettop(lua) == 1)
		{
			obj = getGameObject(lua, 1);
			if (!obj && !lua_isnil(lua, 1))
			{
				return LuaState::expectedArgs(lua, "new", 2, "GameObject attachTo", "nil attachTo");
			}
		}
		DialogueComponent *comp = new DialogueComponent(obj);
		wrapObject<DialogueComponent>(lua, comp);
		return 1;
	}
	/**
	 * TODO
	 */
	int DialogueComponent_dtor(lua_State *lua)
	{
		return 0;
	}
	/**
	 * Compares this DialogueComponent with another DialogueComponent.
	 *
	 * @param DialogueComponent rhs The other DialogueComponent to compare with.
	 * @returns boolean True if they are the same DialogueComponent object.
	 */
	int DialogueComponent_eq(lua_State *lua)
	{
		DialogueComponent *lhs = castUData<DialogueComponent>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "DialogueComponent");
		}
		DialogueComponent *rhs = castUData<DialogueComponent>(lua, 2);
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
			{ "talking_to", DialogueComponent_talking_to },
			{ "start_dialogue", DialogueComponent_start_dialogue },
			{ "locked", DialogueComponent_subject_lock },
			{ "unlocked_subjects", NULL },
			{ "available", DialogueComponent_dialogue_available },
			{ "dialogues_available", NULL },
			{ "attached_to", DialogueComponent_attached_to },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, DialogueComponent::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");
		
		return 1;
	}
	/**
	 * Attempts to start a dialogue with another GameObject.
	 * The other GameObject must have a DialogueComponent attached to talk to.
	 *
	 * @param GameObject talkee The game object to talk to.
	 * @param Dialogue [nil] withDialogue By default the talk to function will start a conversation
	 *  using the start_dialogue value, this allows for that to be overridden for this talk.
	 * @returns DialogueComponent This
	 */
	int DialogueComponent_talk_to(lua_State *lua)
	{
		DialogueComponent *comp = castUData<DialogueComponent>(lua, 1);
		if (comp)
		{
			GameObject *other = getGameObject(lua, 2);
			if (other)
			{
				bool valid = true;
				if (lua_gettop(lua) == 3)
				{
					Dialogue *diag = castUData<Dialogue>(lua, 3);
					if (diag)
					{
						comp->talkTo(other, diag);
					}
					else
					{
						valid = false;
					}
				}
				else
				{
					comp->talkTo(other);
				}
				if (valid)
				{
					lua_first(lua);
				}
			}
			return LuaState::expectedArgs(lua, "talk_to", "GameObject talkee, DialogueComponent withDialogue [nil]");
		}
		return LuaState::expectedContext(lua, "talk_to", "DialogueComponent");
	}
	/**
	 * Returns the GameObject that this DialogueComponent is currently talking to.
	 * Will return nil if there is no dialogue.
	 * @returns GameObject The game object being talked to or nil.
	 */
	int DialogueComponent_talking_to(lua_State *lua)
	{
		DialogueComponent *comp = castUData<DialogueComponent>(lua, 1);
		if (comp)
		{
			wrapGameObject(lua, comp->getTalkingTo());
			return 1;
		}
		return LuaState::expectedContext(lua, "talking_to", "DialogueComponent");
	}
	/**
	 * Returns the openning dialogue this component will use when talked to, or nil.
	 * @returns Dialogue The starting dialogue, or nil.
	 */
	/**
	 * Sets the starting dialogue this component will use when talked to, can be nil.
	 * @param string dialogueId The dialogue id of the dialogue to open with.
	 * @returns DialogueComponent This
	 */
	/**
	 * Sets the starting dialogue this component will use when talked to, can be nil.
	 * @param Dialogue dialogue The dialogue to open with.
	 * @returns DialogueComponent This
	 */
	int DialogueComponent_start_dialogue(lua_State *lua)
	{
		DialogueComponent *comp = castUData<DialogueComponent>(lua, 1);
		if (comp)
		{
			if (lua_gettop(lua) == 1)
			{
				Dialogue *diag = comp->getStartDialogue();
				if (diag)
				{
					wrapObject<Dialogue>(lua, diag);
					return 1;
				}
				lua_pushnil(lua);
				return 1;
			}
			else if (lua_isnil(lua, 2))
			{
				comp->setStartDialogue(NULL);
				lua_first(lua);
			}
			else
			{
				bool valid = true;
				Dialogue *diag = NULL;
				if (lua_isstr(lua, 2))
				{
					diag = Engine::getGame()->getDialogue(lua_tostring(lua, 2));
				}
				else
				{
					diag = castUData<Dialogue>(lua, 2);
					if (!diag)
					{
						valid = false;
					}
				}
				if (valid)
				{
					comp->setStartDialogue(diag);
					lua_first(lua);
				}
				return LuaState::expectedArgs(lua, "start_dialogue", 3, "string dialogueId", "Dialogue dialogue", "nil dialogue");
			}
		}
		return LuaState::expectedContext(lua, "start_dialogue", "DialogueComponent");
	}
	/**
	 * Returns if the given subject title is locked or unlocked.
	 * @param string subject The subject to look up.
	 * @returns boolean True if the subject is locked, false if the subject is unlocked.
	 */
	/**
	 * Sets if a dialogue subject is locked or unlocked for this dialogue component.
	 * Unlocked subjects will be available to use in dialogue.
	 * @param string subject The subject to lock or unlock.
	 * @param boolean locked Sets is the subject is locked or unlocked.
	 * @returns DialogueComponent This
	 */
	int DialogueComponent_subject_lock(lua_State *lua)
	{
		DialogueComponent *comp = castUData<DialogueComponent>(lua, 1);
		if (comp)
		{
			if (lua_gettop(lua) == 2)
			{
				if (lua_isstr(lua, 2))
				{
					lua_pushboolean(lua, comp->isSubjectLocked(lua_tostring(lua, 2)));
					return 1;
				}
				return LuaState::expectedArgs(lua, "locked", "string subject");
			}
			else if (lua_isbool(lua, 3))
			{
				comp->setSubjectLock(lua_tostring(lua, 2), lua_tobool(lua, 3));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "locked", "string subject, boolean locked");
		}
		return LuaState::expectedContext(lua, "locked", "DialogueComponent");
	}
	/**
	 * Returns if the given dialogue is available from this component.
	 * @param string dialogueId The dialogue id to look up.
	 * @returns boolean True if the dialogue is available.
	 */
	/**
	 * Sets if the given dialogue will be available from this component.
	 * @param string dialogueId The dialogue id to set.
	 * @param boolean available The availability of the given dialogue.
	 * @returns DialogueComponent This
	 */
	int DialogueComponent_dialogue_available(lua_State *lua)
	{
		DialogueComponent *comp = castUData<DialogueComponent>(lua, 1);
		if (comp)
		{
			if (lua_isstr(lua, 2))
			{
				if (lua_gettop(lua) == 2)
				{
					lua_pushboolean(lua, comp->isDialogueAvailable(lua_tostring(lua, 2)));
					return 1;
				}
				else
				{
					if (lua_isbool(lua, 3))
					{
						comp->setDialogueAvailable(lua_tostring(lua, 2), lua_tobool(lua, 3));
						lua_first(lua);
					}
				}
				return LuaState::expectedArgs(lua, "available", "string dialogueId, boolean available");
			}
			return LuaState::expectedArgs(lua, "available", "string dialogueId");
		}
		return LuaState::expectedContext(lua, "available", "DialogueComponent");
	}
	
	/**
	 * Returns the game object this dialogue component is attached to, can be nil.
	 * @returns GameObject The game object this is attached to.
	 */
	/**
	 * Attaches this dialogue component to a game object, can be nil.
	 * @param GameObject gameObject The game object to attach to.
	 * @returns DialogueComponent This
	 */
	int DialogueComponent_attached_to(lua_State *lua)
	{
		DialogueComponent *comp = castUData<DialogueComponent>(lua, 1);
		if (comp)
		{
			if (lua_gettop(lua) == 1)
			{
				wrapGameObject(lua, comp->getAttachedTo());
				return 1;
			}
			if (lua_isnil(lua, 2))
			{
				comp->setAttachedTo(NULL);
				lua_first(lua);
			}
			GameObject *obj = getGameObject(lua, 2);
			if (obj)
			{
				comp->setAttachedTo(obj);
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "attached_to", "GameObject gameobject");
		}
		return LuaState::expectedContext(lua, "attached_to", "DialogueComponent");
	}
	
}
}
}
