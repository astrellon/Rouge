#include "lua_game_object.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/engine.h>
#include <game/dialogue_component.h>
#include <game/map.h>
#include <game/game.h>
using namespace am::game;

#include <util/data_table.h>
using namespace am::util;

#include "lua_game.h"
#include "lua_iattribute_data.h"
#include "../lua_event_manager.h"

#include <lua/wrappers/gfx/lua_sprite.h>
using namespace am::lua::gfx;

#include <sstream>
using namespace std;

namespace am {
namespace lua {
namespace game {

	/**
	 * Returns the name of the GameObject.
	 * A nil return indicates that there was an error.
	 *
	 * @returns string The name of the GameObject.
	 */
	/**
	 * Sets the name of the GameObject.
	 *
	 * @param string name The name to set the GameObject.
	 * @returns am.GameObject This.
	 */
	int GameObject_name(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}
		if (lua_gettop(lua) == 1)
		{
			lua_pushstring(lua, obj->getName().c_str());
			return 1;
		}
		else if (lua_isstr(lua, 2))
		{
			const char *name = lua_tostring(lua, 2);
			if (name)
			{
				obj->setName(name);
			}
			lua_first(lua);
		}
		return LuaState::expectedArgs(lua, "name", "string name");
	}

	/**
	 * Returns the description of the GameObject.
	 * A nil return indicates that there was an error.
	 *
	 * @returns string The description of the GameObject.
	 */
	/**
	 * Sets the description of the GameObject.
	 *
	 * @param string description The description to set the GameObject.
	 * @returns am.GameObject This.
	 */
	int GameObject_description(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (lua_gettop(lua) == 1)
		{
			lua_pushstring(lua, obj->getDescription().c_str());
			return 1;
		}
		else if (lua_isstr(lua, 2))
		{
			const char *description = lua_tostring(lua, 2);
			if (description)
			{
				obj->setDescription(description);
			}
			lua_first(lua);
		}
		return LuaState::expectedArgs(lua, "description", "string description");
	}

	
	/**
	 * Returns the GameObjects current map location.
	 * @returns number The GameObjects x map value
	 * @returns number The GameObjects y map value
	 */
	/**
	 * Sets the GameObjects map location.
	 * @param number x The x map value
	 * @param number y The y map value
	 * @returns am.GameObject This
	 */
	int GameObject_location(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (lua_gettop(lua) == 1)
		{
			lua_pushnumber(lua, obj->getLocationX());
			lua_pushnumber(lua, obj->getLocationY());
			return 2;
		}
		else if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
		{
			obj->setLocation(lua_tofloat(lua, 2), lua_tofloat(lua, 3));
			lua_first(lua);
		}
		return LuaState::expectedArgs(lua, "location", "number x, number y");
	}
	/**
	 * Returns the GameObjects currnet map grid location.
	 * @returns integer x The x grid value
	 * @returns integer y The y grid value
	 */
	/**
	 * Sets the GameObjects current map grid location.
	 * @param integer x The x grid value
	 * @param integer y The y grid value
	 * @returns am.GameObject This
	 */
	int GameObject_grid_location(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (lua_gettop(lua) == 1)
		{
			lua_pushinteger(lua, obj->getGridLocationX());
			lua_pushinteger(lua, obj->getGridLocationY());
			return 2;
		}
		else if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
		{
			obj->setGridLocation(lua_tointeger(lua, 2), lua_tointeger(lua, 3));
			lua_first(lua);
		}
		return LuaState::expectedArgs(lua, "grid_location", "integer x, integer y");
	}
	/**
	 * Moves a GameObject by {x, y} amount.
	 * If the new location isn't valid, the GameObject isn't moved.
	 * @param number x The x amount to move by
	 * @param number y The y amount to move by
	 * @returns am.GameObject This
	 */
	int GameObject_move(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
		{
			obj->move(lua_tofloat(lua, 2), lua_tofloat(lua, 3));
			lua_first(lua);
		}
		return LuaState::expectedArgs(lua, "move", "number x, number y");
	}
	/**
	 * Moves a GameObject by {x, y} grid amounts.
	 * If the new location isn't valid, the GameObject isn't moved.
	 * @param integer x The x grid amount to by move by
	 * @param integer y The y grid amount to by move by
	 * @returns am.GameObject This
	 */
	int GameObject_move_grid(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
		{
			obj->moveGrid(lua_tointeger(lua, 2), lua_tointeger(lua, 3));
			lua_first(lua);
		}
		return LuaState::expectedArgs(lua, "move_grid", "integer x, integer y");
	}
	/**
	 * Initiates a conversation between this GameObject
	 * and the game object id passed in.
	 * Returns true if the conversation was initiated.
	 * False can mean that the game object id was invalid or that
	 * either this GameObject or the game object did not have a
	 * dialog component attached.
	 * @see am.dialog_component
	 * @param string id The game object id.
	 * @returns boolean True if the conversation was initiated.
	 */
	/**
	 * Initiates a conversation between this GameObject
	 * and the game object passed in.
	 * Returns true if the conversation was initiated.
	 * False can mean that the game object was nil or that
	 * either this GameObject or the game object did not have a
	 * dialog component attached.
	 * @see am.dialog_component
	 * @param am.game_object talkee The game object to start a converstation with.
	 * @returns boolean True if the conversation was initiated.
	 */
	int GameObject_talk_to(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (obj->getDialogueComp())
		{
			GameObject *other = getGameObject(lua, -1);
			if (other)
			{
				if (other->getDialogueComp())
				{
					obj->getDialogueComp()->talkTo(other);
					lua_pushboolean(lua, true);
					return 1;
				}
				LuaState::warning(lua, "Talkee game object needs a dialogue component to talk to");
				lua_pushboolean(lua, false);
				return 1;
			}
			return LuaState::expectedArgs(lua, "talk_to", "am.game_object talkee");
		}
		LuaState::warning(lua, "GameObject needs a dialogue component to talk to another game object");
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Returns true if this GameObject is fixed to the game grid
	 * or false if the GameObject is able to move freely about the map.
	 * @returns boolean True if the GameObject is fixed to the grid.
	 */
	/**
	 * Sets if the GameObject is fixed to the grid or not.
	 * If a GameObject is fixed to the grid, they cannot move to positions
	 * other than the center of each grid space.
	 * @param boolean fixed Sets the fixed to grid value.
	 * @returns am.GameObject This
	 */
	int GameObject_fixed_to_grid(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (lua_gettop(lua) == 1)
		{
			lua_pushboolean(lua, obj->isFixedToGrid());
			return 1;
		}
		else if (lua_isbool(lua, 2))
		{
			obj->setFixedToGrid(lua_tobool(lua, 2));
			lua_first(lua);
		}
		return LuaState::expectedArgs(lua, "fixed_to_grid", "boolean fixed");
	}

	/**
	 * Returns the current map that this GameObject is on, can be nil.
	 *
	 * @returns am.map The map the GameObject is on.
	 */
	/**
	 * Sets the map that the GameObject is on, can be nil.
	 *
	 * @param am.map map The map to put the GameObject on.
	 * @returns am.GameObject This
	 */
	int GameObject_map(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (lua_gettop(lua) == 1)
		{
			Map *map = obj->getMap();
			if (map)
			{
				wrapRefObject<Map>(lua, map);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		else if (lua_isnil(lua, 2))
		{
			obj->setMap(nullptr);
			lua_first(lua);
		}
		else
		{
			Map *map = castUData<Map>(lua, 2);
			if (map)
			{
				// Can be set to nil
				obj->setMap(map);
				lua_first(lua);
			}
		}
		return LuaState::expectedArgs(lua, "map", 2, "nil map", "am.map map");
	}

	/**
	 * Returns the original map that this GameObject was on, can be nil.
	 *
	 * @returns am.map The map the GameObject was originally on.
	 */
	/**
	 * Sets the map that the GameObject was originally on, can be nil.
	 * This is usually set automatically the first time a GameObject is
	 * added to a map. This is primarily used for reloading the GameObject
	 * and knowing where to find likely find any event handlers and additional
	 * information relating to the GameObject that is not stored directly on the
	 * GameObject.
	 *
	 * @param am.map map The map the GameObject was originally was on.
	 * @returns am.GameObject This
	 */
	int GameObject_original_map(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (lua_gettop(lua) == 1)
		{
			Map *map = obj->getOriginalMap();
			if (map)
			{
				wrapRefObject<Map>(lua, map);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		else if (lua_isnil(lua, 2))
		{
			obj->setOriginalMap(nullptr);
			lua_first(lua);
		}
		else
		{
			Map *map = castUData<Map>(lua, 2);
			if (map)
			{
				// Can be set to nil
				obj->setOriginalMap(map);
				lua_first(lua);
			}
		}
		return LuaState::expectedArgs(lua, "map", 2, "nil map", "am.map map");
	}
	/**
	 * Adds a tile type to the list of tile types that this GameObject
	 * can move freely on. TileTypes can be added multiple times.
	 * 
	 * @param am.tile_type tile_type The type to add to the list.
	 * @returns am.GameObject This
	 */
	/**
	 * Adds a tile type to the list of tile types that this GameObject
	 * can move freely on. Tile types can be added multiple times.
	 * 
	 * @param string tile_type_name The name of the type to add to the list.
	 * @returns am.GameObject This
	 */
	int GameObject_add_passible_type(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (lua_isstr(lua, 2))
		{
			Handle<TileType> type(Engine::getEngine()->getTileType(lua_tostring(lua, 2)));
			if (type)
			{
				obj->addPassibleType(type);
			}
			else
			{
				stringstream ss;
				ss << "Unknown tile type '";
				LuaState::printTypeValue(lua, 2, ss);
				ss << "' unable to add";
				LuaState::warning(lua, ss.str().c_str());
			}
			lua_first(lua);
		}
		Handle<TileType> type(castUData<TileType>(lua, 2));
		if (type)
		{
			obj->addPassibleType(type);
			lua_first(lua);
		}
		return LuaState::expectedArgs(lua, "add_passible_type", 2, "am.tile_type tile_type", "string tile_type_name");
	}
	/**
	 * Removes a tile type from the list of passible tiles.
	 * @param am.tile_type tile_type The tile type to remove.
	 * @returns am.GameObject This
	 */
	int GameObject_remove_passible_type(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		Handle<TileType> type(castUData<TileType>(lua, 2));
		if (type)
		{
			obj->removePassibleType(type);
			lua_first(lua);
		}
		return LuaState::expectedArgs(lua, "remove_passible_type", "am.tile_type tile_type");
	}
	/**
	 * Removes all tile types from this GameObjects passible list.
	 * @returns am.GameObject This
	 */
	int GameObject_remove_all_passible_types(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (obj)
		{
			obj->removeAllPassibleTypes();
			lua_first(lua);
		}
		return LuaState::expectedContext(lua, "remove_all_passible_types", "am.GameObject");
	}
	/**
	 * Returns true if the given tile type is found on this GameObjects
	 * passible list.
	 * @param am.tile_type tile_type The tile type to search for
	 * @returns boolean True if the tile type is found
	 */
	int GameObject_has_passible_type(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		Handle<TileType> type(castUData<TileType>(lua, 2));
		if (type)
		{
			lua_pushboolean(lua, obj->hasPassibleType(type));
			return 1;
		}
		return LuaState::expectedArgs(lua, "has_passible_type", "am.tile_type tile_type");
	}
	/**
	 * Returns a table of all the passible tile types for this GameObject.
	 * @returns table Table of all the tile types.
	 */
	int GameObject_get_passible_types(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		LuaState L(lua);
		L.newTable();
		const GameObject::PassibleTypeList &list = obj->getPassibleTypes();
		for (int i = 0; i < static_cast<int>(list.size()); i++)
		{
			lua_pushinteger(lua, i);
			wrapObject<TileType>(lua, list[i]);
			lua_settable(lua, -3);
		}
		return 1;
	}
	
	/**
	 * @see am.dialogue_component
	 * Returns the dialogue component attached to this GameObject, can be nil.
	 * @returns am.dialogue_component The attached dialogue component
	 */
	/**
	 * Sets a dialogue component onto this GameObject, can be nil.
	 * @param am.dialogue_component comp The dialogue component to attach to this GameObject, can be nil.
	 * @param boolean [true] set_attached When true it also sets that this GameObject
	 * is the game object attached to the dialogue component.
	 */
	int GameObject_dialogue_component(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (lua_gettop(lua) == 1)
		{
			DialogueComponent *comp = obj->getDialogueComp();
			if (comp)
			{
				wrapObject<DialogueComponent>(lua, comp);
				return 1;
			}
		}
		else if (lua_isnil(lua, 2))
		{
			obj->setDialogueComp(nullptr);
			lua_first(lua);
		}
		else
		{
			DialogueComponent *comp = castUData<DialogueComponent>(lua, 2);
			if (comp)
			{
				if (lua_isbool(lua, 3))
				{
					obj->setDialogueComp(comp, lua_tobool(lua, 3));
				}
				else
				{
					obj->setDialogueComp(comp);
				}
				lua_first(lua);
			}
		}
		return LuaState::expectedArgs(lua, "dialogue_component", 2, "am.dialogue_component comp, boolean [true] set_attached", "nil comp");
	}

	/**
	 * Returns the game id for this GameObject.
	 * @returns string The GameObjects game id.
	 */
	/**
	 * Sets the GameObjects game id, this is used to refer to this
	 * GameObject from the game engine. Should be unique.
	 * @param string game_id The GameObjects unique game id.
	 * @returns boolean True if the game id change was successful or if
	 * game id was the same as the given game_id. False indicates that
	 * either there is no current game engine or there is another
	 * game object with the same game id.
	 */
	int GameObject_game_id(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (lua_gettop(lua) == 1)
		{
			lua_pushstring(lua, obj->getGameId());
			return 1;
		}
		else if (lua_isstr(lua, 2))
		{
			lua_pushboolean(lua, obj->setGameId(lua_tostring(lua, 2)));
			return 1;
		}
		return LuaState::expectedArgs(lua, "id", "string id");
	}
	
	/**
	 * Adds an event listener for an event fired on this GameObject.
	 * eg: <pre>
	 * GameObject:on("talkTo", function(event)
	 *     am_log("GameObject talked to")
	 * end)
	 * </pre>
	 * @param string event_type The event type or name to trigger on
	 * @param function listener The function to call when the event is fired.
	 * @param table [nil] content An option context for the listener to be
	 * called with.
	 * @returns boolean True if the event was added successfully.
	 */
	int GameObject_add_event_listener(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (lua_isstr(lua, 2) && lua_isfunction(lua, 3))
		{
			lua_pushboolean(lua, am::lua::ui::addEventListener(lua, obj));
			return 1;
		}
		return LuaState::expectedArgs(lua, "on", "string event_type, function listener");
	}
	/**
	 * Removes an event listener from this GameObject.
	 * eg: 
	 * <pre>
	 * function talkToOnce(event)
	 *     am_log("GameObject talked to once")
	 *     GameObject:off("talkTo", talkToOnce)
	 * end
	 * GameObject:on("talkTo", talkToOnce)
	 * </pre>
	 * @param string event_type The event type the listener was listening for.
	 * @param function listener The listener function to remove.
	 * @param table [nil] context The context which the listener was going to 
	 * be called with, this is only optional if the listener was added with no context.
	 * @returns boolean True if the event listener was successfully removed.
	 */
	int GameObject_remove_event_listener(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (lua_isstr(lua, 2) && lua_isfunction(lua, 3))
		{
			lua_pushboolean(lua, am::lua::ui::removeEventListener(lua, obj));
			return 1;
		}
		return LuaState::expectedArgs(lua, "off", "string event_type, function listener");
	}
	/**
	 * Returns true when there is an event listener for the given event_type.
	 * @param string event_type The event type to look up.
	 * @returns boolean True if there is any event listener 
	 * that will be trigged by this event type.
	 */
	int GameObject_has_event_listener(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		if (lua_isstr(lua, 2))
		{
			lua_pushboolean(lua, obj->hasEventListener(lua_tostring(lua, 2)));
			return 1;
		}
		return LuaState::expectedArgs(lua, "has_event_listener", "string event_type");
	}

	/**
	 * Returns the GameObjects attribute data table.
	 * By default if no attribute data table is present nil is returned unless true
	 * is passed as the first argument, then a data table is created if one is not present.
	 * 
	 * @param boolean [false] create_table Create a data table if one didn't exist.
	 * @returns DataTable The data table on this GameObject.
	 */
	/**
	 * Sets the data table on this GameObject, can be set to nil.
	 *
	 * @param am.data_table attr_table The data table to set on the GameObject.
	 * @returns am.GameObject This
	 */
	int GameObject_attrs(lua_State *lua, GameObject *obj)
	{
		if (!obj)
		{
			return 0;
		}

		return IAttributeData_attrs(lua, obj);
	}

}
}
}
