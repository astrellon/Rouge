#include "lua_door.h"

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
#include <game/door.h>
using namespace am::game;

#include <util/data_table.h>
using namespace am::util;

#include "lua_game_object.h"
#include "lua_tile_type.h"
#include "lua_dialogue_component.h"
#include "lua_game.h"

#include <lua/wrappers/gfx/lua_sprite.h>
using namespace am::lua::gfx;

#include <sstream>
using namespace std;

namespace am {
namespace lua {
namespace game {

	/**
	 * @class
	 * Class for representing a Door.
	 * <pre>
	 * wooden_door = am.door.new("map1_door")
	 * wooden_door:name("Fantastic Wooden Door")
	 * </pre>
	 */

	/**
	 * @static
	 * Creates or wraps a door instance.
	 *
	 * @param string [""] game_id The game id to create the door with or the game id
	 *  of an existing door.
	 * @returns am.character The newly created door or returns an existing door
	 *  with the same game_id.
	 * @returns boolean True if the door was newly created or is just wrapped an existing one.
	 */
	int Door_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args >= 1 && !lua_isstr(lua, 1))
		{
			return LuaState::expectedArgs(lua, "@new", 2, "", "string id");
		}

		if (args == 0)
		{
			Door *door = new Door();
			wrapRefObject<Door>(lua, door);
			lua_pushboolean(lua, true);
			return 2;
		}

		bool newDoor = false;
		const char *id = lua_tostring(lua, 1);
		Handle<Door> door(dynamic_cast<Door *>(Engine::getEngine()->getGameObject(id)));
		if (!door)
		{
			newDoor = true;
			door = new Door();
			door->setGameId(id);
		}
		wrapRefObject<Door>(lua, door);
		lua_pushboolean(lua, newDoor);
		return 2;
	}
	/**
	 * Releases the reference of this door.
	 */
	int Door_dtor(lua_State *lua)
	{
		Door *door = castUData<Door>(lua, 1);
		if (door)
		{
			door->release();
			return 0;
		}
		return LuaState::expectedContext(lua, "__gc", "am.door");
	}
	/**
	 * Compares this character with another character reference.
	 * Will not return true for equivalent characters.
	 * 
	 * @param am.character rhs The other character to compare with.
	 * @returns boolean True if the characters are the same entity.
	 */
	int Door_eq(lua_State *lua)
	{
		Door *lhs = castUData<Door>(lua, 1);
		if (!lhs)
		{
			LuaState::expectedContext(lua, "__eq", "am.door");
			return 0;
		}
		Door *rhs = castUData<Door>(lua, 2);
		lua_pushboolean(lua, lhs && rhs && lhs == rhs);
		return 1;
	}

	int Door_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Door_ctor },
			{ "__gc", Door_dtor },
			{ "__eq", Door_eq },
			// Character methods

			{ "name", Door_name },
			{ "description", Door_description },
			{ "graphic", Door_graphic },
			// GameObject methods
			{ "location", Door_location },
			{ "grid_location", Door_grid_location },
			{ "move", Door_move },
			{ "move_grid", Door_move_grid },
			{ "talk_to", Door_talk_to },
			{ "fixed_to_grid", Door_fixed_to_grid },
			{ "map", Door_map },
			{ "original_map", Door_original_map },
			{ "add_passible_type", Door_add_passible_type },
			{ "remove_passible_type", Door_remove_passible_type },
			{ "remove_all_passible_types", Door_remove_all_passible_types },
			{ "has_passible_type", Door_has_passible_type },
			{ "get_passible_types", Door_get_passible_types },
			{ "game_id", Door_game_id },
			{ "find", Door_find },
			{ "dialogue_component", Door_dialogue_component },
			// EventListener methods
			{ "on", Door_add_event_listener },
			{ "off", Door_remove_event_listener },
			{ "has_event_listener", Door_has_event_listener },
			{ "attrs", Door_attrs },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, Door::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");
		
		return 1;
	}

	/**
	 * Returns the name of the door.
	 * A nil return indicates that there was an error.
	 *
	 * @returns string The name of the door.
	 */
	/**
	 * Sets the name of the door.
	 *
	 * @param string name The name to set the door.
	 * @returns am.door This.
	 */
	int Door_name(lua_State *lua)
	{
		Door *door = castUData<Door>(lua, 1);
		if (door)
		{
			return GameObject_name(lua, door);
		}
		return LuaState::expectedContext(lua, "name", "am.door");
	}

	/**
	 * Returns the description of the door.
	 * A nil return indicates that there was an error.
	 *
	 * @returns string The description of the door.
	 */
	/**
	 * Sets the description of the door.
	 *
	 * @param string description The description to set the door.
	 * @returns am.door This.
	 */
	int Door_description(lua_State *lua)
	{
		Door *door = castUData<Door>(lua, 1);
		if (door)
		{
			return GameObject_description(lua, door);
		}
		return LuaState::expectedContext(lua, "description", "am.door");
	}

	/**
	 * Returns the graphic attached to this door, can be nil.
	 *
	 * @returns am.sprite The door sprite.
	 */
	/**
	 * Sets the door asset graphic.
	 * Sets the asset_name of the graphic for this door.
	 *
	 * @param string asset_name The asset name to the graphic.
	 * @returns am.door This
	 */
	/**
	 * Sets the door graphic, can be nil.
	 *
	 * @param am.sprite sprite The graphic to set.
	 * @returns am.door This
	 */
	int Door_graphic(lua_State *lua)
	{
		Door *door = castUData<Door>(lua, 1);
		if (door)
		{
			if (lua_gettop(lua) == 1)
			{
				Sprite *sprite = door->getGraphic();
				if (sprite)
				{
					wrapRefObject<Sprite>(lua, sprite);
				}
				else
				{
					lua_pushnil(lua);
				}
				return 1;
			}
			else
			{
				bool valid = false;
				if (lua_isstr(lua, 2))
				{
					door->setGraphic(new Sprite(lua_tostring(lua, 2)));
					valid = true;
				}
				else if (lua_isnil(lua, 2))
				{
					door->setGraphic(nullptr);
					valid = true;
				}
				else
				{
					Sprite *sprite = castUData<Sprite>(lua, 2);
					if (sprite)
					{
						door->setGraphic(sprite);
						valid = true;
					}
				}
				if (valid)
				{
					lua_first(lua);
				}
				return LuaState::expectedArgs(lua, "graphic", 3, "string asset_name", "am.sprite sprite", "nil");
			}
		}
		return LuaState::expectedContext(lua, "graphic", "am.door");
	}

	
	/**
	 * Returns the doors current map location.
	 * @returns number The doors x map value
	 * @returns number The doors y map value
	 */
	/**
	 * Sets the doors map location.
	 * @param number x The x map value
	 * @param number y The y map value
	 * @returns am.door This
	 */
	int Door_location(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_location(lua, obj);
		}
		return LuaState::expectedContext(lua, "location", "am.door");
	}
	/**
	 * Returns the doors currnet map grid location.
	 * @returns integer x The x grid value
	 * @returns integer y The y grid value
	 */
	/**
	 * Sets the doors current map grid location.
	 * @param integer x The x grid value
	 * @param integer y The y grid value
	 * @returns am.door This
	 */
	int Door_grid_location(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_grid_location(lua, obj);
		}
		return LuaState::expectedContext(lua, "grid_location", "am.door");
	}
	/**
	 * Moves a door by {x, y} amount.
	 * If the new location isn't valid, the door isn't moved.
	 * @param number x The x amount to move by
	 * @param number y The y amount to move by
	 * @returns am.door This
	 */
	int Door_move(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_move(lua, obj);
		}
		return LuaState::expectedContext(lua, "move", "am.door");
	}
	/**
	 * Moves a door by {x, y} grid amounts.
	 * If the new location isn't valid, the door isn't moved.
	 * @param integer x The x grid amount to by move by
	 * @param integer y The y grid amount to by move by
	 * @returns am.door This
	 */
	int Door_move_grid(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_move_grid(lua, obj);
		}
		return LuaState::expectedContext(lua, "move_grid", "am.door");
	}
	/**
	 * Initiates a conversation between this door
	 * and the game object id passed in.
	 * Returns true if the conversation was initiated.
	 * False can mean that the game object id was invalid or that
	 * either this door or the game object did not have a
	 * dialog component attached.
	 * @see am.dialog_component
	 * @param string id The game object id.
	 * @returns boolean True if the conversation was initiated.
	 */
	/**
	 * Initiates a conversation between this door
	 * and the game object passed in.
	 * Returns true if the conversation was initiated.
	 * False can mean that the game object was nil or that
	 * either this door or the game object did not have a
	 * dialog component attached.
	 * @see am.dialog_component
	 * @param am.game_object talkee The game object to start a converstation with.
	 * @returns boolean True if the conversation was initiated.
	 */
	int Door_talk_to(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_talk_to(lua, obj);
		}
		return LuaState::expectedContext(lua, "talk_to", "am.door");
	}
	/**
	 * Returns true if this door is fixed to the game grid
	 * or false if the door is able to move freely about the map.
	 * @returns boolean True if the door is fixed to the grid.
	 */
	/**
	 * Sets if the door is fixed to the grid or not.
	 * If a door is fixed to the grid, they cannot move to positions
	 * other than the center of each grid space.
	 * @param boolean fixed Sets the fixed to grid value.
	 * @returns am.door This
	 */
	int Door_fixed_to_grid(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_fixed_to_grid(lua, obj);
		}
		return LuaState::expectedContext(lua, "fixed_to_grid", "am.door");
	}

	/**
	 * Returns the current map that this door is on, can be nil.
	 *
	 * @returns am.map The map the door is on.
	 */
	/**
	 * Sets the map that the door is on, can be nil.
	 *
	 * @param am.map map The map to put the door on.
	 * @returns am.door This
	 */
	int Door_map(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_map(lua, obj);
		}
		return LuaState::expectedContext(lua, "map", "am.door");
	}

	/**
	 * Returns the original map that this door was on, can be nil.
	 *
	 * @returns am.map The map the door was originally on.
	 */
	/**
	 * Sets the map that the door was originally on, can be nil.
	 * This is usually set automatically the first time a door is
	 * added to a map. This is primarily used for reloading the door
	 * and knowing where to find likely find any event handlers and additional
	 * information relating to the door that is not stored directly on the
	 * door.
	 *
	 * @param am.map map The map the door was originally was on.
	 * @returns am.door This
	 */
	int Door_original_map(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_original_map(lua, obj);
		}
		return LuaState::expectedContext(lua, "map", "am.door");
	}
	/**
	 * Adds a tile type to the list of tile types that this door
	 * can move freely on. TileTypes can be added multiple times.
	 * 
	 * @param am.tile_type tile_type The type to add to the list.
	 * @returns am.door This
	 */
	/**
	 * Adds a tile type to the list of tile types that this door
	 * can move freely on. Tile types can be added multiple times.
	 * 
	 * @param string tile_type_name The name of the type to add to the list.
	 * @returns am.door This
	 */
	int Door_add_passible_type(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_add_passible_type(lua, obj);
		}
		return LuaState::expectedContext(lua, "add_passible_type", "am.door");
	}
	/**
	 * Removes a tile type from the list of passible tiles.
	 * @param am.tile_type tile_type The tile type to remove.
	 * @returns am.door This
	 */
	int Door_remove_passible_type(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_remove_passible_type(lua, obj);
		}
		return LuaState::expectedContext(lua, "remove_passible_type", "am.door");
	}
	/**
	 * Removes all tile types from this doors passible list.
	 * @returns am.door This
	 */
	int Door_remove_all_passible_types(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_remove_all_passible_types(lua, obj);
		}
		return LuaState::expectedContext(lua, "remove_all_passible_types", "am.door");
	}
	/**
	 * Returns true if the given tile type is found on this doors
	 * passible list.
	 * @param am.tile_type tile_type The tile type to search for
	 * @returns boolean True if the tile type is found
	 */
	int Door_has_passible_type(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_has_passible_type(lua, obj);
		}
		return LuaState::expectedContext(lua, "has_passible_type", "am.door");
	}
	/**
	 * Returns a table of all the passible tile types for this door.
	 * @returns table Table of all the tile types.
	 */
	int Door_get_passible_types(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_get_passible_types(lua, obj);
		}
		return LuaState::expectedContext(lua, "get_passible_types", "am.door");
	}
	
	/**
	 * @see am.dialogue_component
	 * Returns the dialogue component attached to this door, can be nil.
	 * @returns am.dialogue_component The attached dialogue component
	 */
	/**
	 * Sets a dialogue component onto this door, can be nil.
	 * @param am.dialogue_component comp The dialogue component to attach to this door, can be nil.
	 * @param boolean [true] set_attached When true it also sets that this door
	 * is the game object attached to the dialogue component.
	 */
	int Door_dialogue_component(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_dialogue_component(lua, obj);
		}
		return LuaState::expectedContext(lua, "dialogue_component", "am.door");
	}

	/**
	 * Returns the game id for this door.
	 * @returns string The doors game id.
	 */
	/**
	 * Sets the doors game id, this is used to refer to this
	 * door from the game engine. Should be unique.
	 * @param string game_id The doors unique game id.
	 * @returns boolean True if the game id change was successful or if
	 * game id was the same as the given game_id. False indicates that
	 * either there is no current game engine or there is another
	 * game object with the same game id.
	 */
	int Door_game_id(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_game_id(lua, obj);
		}
		return LuaState::expectedContext(lua, "id", "am.door");
	}
	
	/**
	 * Adds an event listener for an event fired on this door.
	 * eg: <pre>
	 * door:on("talkTo", function(event)
	 *     am_log("Character talked to")
	 * end)
	 * </pre>
	 * @param string event_type The event type or name to trigger on
	 * @param function listener The function to call when the event is fired.
	 * @param table [nil] content An option context for the listener to be
	 * called with.
	 * @returns boolean True if the event was added successfully.
	 */
	int Door_add_event_listener(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_add_event_listener(lua, obj);
		}
		return LuaState::expectedContext(lua, "on", "am.door");
	}
	/**
	 * Removes an event listener from this door.
	 * eg: 
	 * <pre>
	 * function talkToOnce(event)
	 *     am_log("Character talked to once")
	 *     door:off("talkTo", talkToOnce)
	 * end
	 * door:on("talkTo", talkToOnce)
	 * </pre>
	 * @param string event_type The event type the listener was listening for.
	 * @param function listener The listener function to remove.
	 * @param table [nil] context The context which the listener was going to 
	 * be called with, this is only optional if the listener was added with no context.
	 * @returns boolean True if the event listener was successfully removed.
	 */
	int Door_remove_event_listener(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_remove_event_listener(lua, obj);
		}
		return LuaState::expectedContext(lua, "off", "am.door");
	}
	/**
	 * Returns true when there is an event listener for the given event_type.
	 * @param string event_type The event type to look up.
	 * @returns boolean True if there is any event listener 
	 * that will be trigged by this event type.
	 */
	int Door_has_event_listener(lua_State *lua)
	{
		Door *obj = castUData<Door>(lua, 1);
		if (obj)
		{
			return GameObject_has_event_listener(lua, obj);
		}
		return LuaState::expectedContext(lua, "has_event_listener", "am.door");
	}

	/**
	 * @static
	 * Looks up an door in the current game engine with the given game id.
	 *
	 * @param string game_id The game id to look up.
	 * @returns am.door The found door or nil.
	 */
	int Door_find(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			Door *door = dynamic_cast<Door *>(Engine::getEngine()->getGameObject(lua_tostring(lua, 1)));
			if (door)
			{
				wrapRefObject<Door>(lua, door);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@find", "string game_id");
	}

	/**
	 * Returns the doors attribute data table.
	 * By default if no attribute data table is present nil is returned unless true
	 * is passed as the first argument, then a data table is created if one is not present.
	 * 
	 * @param boolean [false] create_table Create a data table if one didn't exist.
	 * @returns am.data_table The data table on this character.
	 */
	/**
	 * Sets the data table on this door, can be set to nil.
	 *
	 * @param am.data_table attr_table The data table to set on the door.
	 * @returns am.door This
	 */
	int Door_attrs(lua_State *lua)
	{
		Door *door = castUData<Door>(lua, 1);
		if (door)
		{
			return GameObject_attrs(lua, door);
		}
		return LuaState::expectedContext(lua, "attrs", "am.door");
	}

}
}
}
