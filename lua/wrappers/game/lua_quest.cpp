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
	/**
	 * @class
	 * Holds information relating to a single quest. This information
	 * is very high level as it only contains information about if the quest
	 * has been completed, the name of the quest and a description. As such
	 * this means that it's up to each individual quest to store it's own information about
	 * steps required, rewards and events to itself.
	 */
	/**
	 * Creates a new quest with the given quest id.
	 *
	 * @param string questId The quest id for this quest.
	 */
	int Quest_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 1 && lua_isstring(lua, -1))
		{
			Quest *quest = new Quest(lua_tostring(lua, -1));
			wrapObject<Quest>(lua, quest);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Deletes the quest, TODO check if this actually works, should
	 * probably extend Quest from IManaged.
	 */
	int Quest_dtor(lua_State *lua)
	{
		Quest *quest = castUData<Quest>(lua, 1);
		if (quest)
		{
			delete quest;
		}
		return 0;
	}
	/**
	 * Compares if this quest is the same as the given quest object.
	 *
	 * @param Quest rhs The other quest to compare with.
	 * @returns boolean True if they are the same quest object.
	 */
	int Quest_eq(lua_State *lua)
	{
		Quest *lhs = castUData<Quest>(lua, 1);
		Quest *rhs = castUData<Quest>(lua, 2);
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
			{ "start_quest", Quest_start_quest },
			{ "finish_quest", Quest_finish_quest },
			{ "complete", Quest_complete },
			{ "title", Quest_title },
			{ "description", Quest_description },
			{ "active_text", Quest_active_text },
			{ "on", Quest_add_event_listener },
			{ "off", Quest_remove_event_listener },
			{ "has_event_listener", Quest_has_event_listener },
			{ "add_quest", Quest_add_quest },
			{ "remove_quest", Quest_remove_quest },
			{ "find", Quest_find },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Quest::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	/**
	 * Sets the flag that this quest has been started, if the quest has already been
	 * started this will return false.
	 * If the quest is started then the "startQuest" event will be fired.
	 *
	 * @returns boolean True if the quest was started, false indicates the quest has already been started.
	 */
	int Quest_start_quest(lua_State *lua)
	{
		Quest *quest = castUData<Quest>(lua, 1);
		if (quest)
		{
			lua_pushboolean(lua, quest->startQuest());
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Sets that the quest has been finished, this will return false is the quest has already
	 * been finished. If the quest is finished then the "finishQuest" event will be fired.
	 *
	 * @returns boolean True if the quest was finished, false indicates that the quest has alrady been finished.
	 */
	int Quest_finish_quest(lua_State *lua)
	{
		Quest *quest = castUData<Quest>(lua, 1);
		if (quest)
		{
			lua_pushboolean(lua, quest->finishQuest());
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Returns true if the quest has been completed/finished.
	 *
	 * @returns boolean True if the quest has been completed/finished.
	 */
	/**
	 * Silently sets the quest completed/finished flag, this does not fire any events.
	 *
	 * @param boolean completed Sets if the quest has been completed/finished.
	 * @returns Quest This
	 */
	int Quest_complete(lua_State *lua)
	{
		Quest *quest = castUData<Quest>(lua, 1);
		if (quest)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushboolean(lua, quest->isCompleted());
				return 1;
			}
			else if (lua_isboolean(lua, -1))
			{
				quest->setCompleted(lua_toboolean(lua, -1) > 0);
				lua_first(lua);
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the title of this quest.
	 *
	 * @returns string The quest title.
	 */
	/**
	 * Sets the title of this quest.
	 *
	 * @param string title The new quest title.
	 * @returns Quest This
	 */
	int Quest_title(lua_State *lua)
	{
		Quest *quest = castUData<Quest>(lua, 1);
		if (quest)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, quest->getTitle());
				return 1;
			}
			else if (lua_isstring(lua, -1))
			{
				quest->setTitle(lua_tostring(lua, -1));
				lua_first(lua);
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the quest description.
	 *
	 * @returns string The quest description.
	 */
	/**
	 * Sets the quest description.
	 *
	 * @param string description The new quest description.
	 * @returns Quest This
	 */
	int Quest_description(lua_State *lua)
	{
		Quest *quest = castUData<Quest>(lua, 1);
		if (quest)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, quest->getDescription());
				return 1;
			}
			else if (lua_isstring(lua, -1))
			{
				quest->setDescription(lua_tostring(lua, -1));
				lua_first(lua);
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the current active text for the quest. This can change over time as
	 * the player progresses through the quest.
	 *
	 * @returns string The current active text.
	 */
	/**
	 * Sets the active text, this should change to reflect how the player is progressing through the quest.
	 *
	 * @param string text The new active text.
	 * @returns Quest This
	 */
	int Quest_active_text(lua_State *lua)
	{
		Quest *quest = castUData<Quest>(lua, 1);
		if (quest)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, quest->getActiveText());
				return 1;
			}
			else if (lua_isstring(lua, -1))
			{
				quest->setActiveText(lua_tostring(lua, -1));
				lua_first(lua);
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Adds an event listener to this quest.
	 * <pre>
	 * Quest = import("Quest")
	 * quest = Quest.new("testQuest")
	 * quest:on("startQuest", function(event)
	 *     am_log("Quest started")
	 *     quest:finish_quest()
	 * end)
	 * quest:on("finishQuest", function(event)
	 *     am_log("Quest finished")
	 * end)
	 * quest:start_quest()
	 * </pre>
	 *
	 * @param string eventType The event type or name to trigger on
	 * @param function listener The function to call when the event is fired.
	 * @param table [nil] content An option context for the listener to be
	 * called with.
	 * @returns boolean True if the event was added successfully.
	 */
	int Quest_add_event_listener(lua_State *lua)
	{
		Quest *quest = castUData<Quest>(lua, 1);
		if (quest && lua_isstring(lua, 2) && lua_isfunction(lua, 3))
		{
			lua_pushboolean(lua, am::lua::ui::addEventListener(lua, quest));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Removes an event listener from the quest.
	 * Currently the quest only has events which should only fire once, 
	 * but if an event listener needs to changed before the event is fire it can be done.
	 *
	 * @param string eventType The event type the listener was listening for.
	 * @param function listener The listener function to remove.
	 * @param table [nil] context The context which the listener was going to 
	 * be called with, this is only optional if the listener was added with no context.
	 * @returns boolean True if the event listener was successfully removed.
	 */
	int Quest_remove_event_listener(lua_State *lua)
	{
		Quest *quest = castUData<Quest>(lua, 1);
		if (quest && lua_isstring(lua, 2) && lua_isfunction(lua, 3))
		{
			lua_pushboolean(lua, am::lua::ui::removeEventListener(lua, quest));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns true when there is an event listener for the given eventType.
	 * @param string eventType The event type to look up.
	 * @returns boolean True if there is any event listener 
	 * that will be trigged by this event type.
	 */
	int Quest_has_event_listener(lua_State *lua)
	{
		Quest *quest = castUData<Quest>(lua, 1);
		if (quest && lua_isstring(lua, -1))
		{
			lua_pushboolean(lua, quest->hasEventListener(lua_tostring(lua, -1)));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * @static
	 * Adds a quest to the game engine pool of quests. This allows for them to be
	 * looked up from anywhere using their quest id.
	 *
	 * @param Quest quest The quest to add to the pool.
	 * @returns boolean True if the quest was successfully added.
	 */
	int Quest_add_quest(lua_State *lua)
	{
		Quest *quest = castUData<Quest>(lua, 1);
		if (quest)
		{
			lua_pushboolean(lua, Engine::getGame()->addQuest(quest));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * @static
	 * Removes a quest from the quest pool using its quest id.
	 *
	 * @param string questId The quest id for the quest to remove.
	 * @returns boolean True if the quest was successfully removed.
	 */
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
	/**
	 * @static
	 * Returns the found quest from the quest pool.
	 * 
	 * @param string questId The quest id of the quest to look up.
	 * @returns Quest The found quest or nil.
	 */
	int Quest_find(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			Quest *quest = dynamic_cast<Quest *>(Engine::getGame()->getQuest(lua_tostring(lua, -1)));
			if (quest)
			{
				wrapObject<Quest>(lua, quest);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
