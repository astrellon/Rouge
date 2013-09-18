#include "lua_character.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/character.h>
#include <game/race.h>
#include <game/engine.h>
#include <game/dialogue_component.h>
#include <game/map.h>
#include <game/game.h>
using namespace am::game;

#include <util/data_table.h>
using namespace am::util;

#include "lua_stats.h"
#include "lua_body_part.h"
#include "lua_item.h"
#include "lua_inventory.h"
#include "lua_map.h"
#include "lua_tile_type.h"
#include "../lua_event_manager.h"
#include "lua_coin_purse.h"
#include "lua_race.h"
#include "lua_dialogue_component.h"
#include "lua_game.h"
#include "lua_iattribute_data.h"

#include <lua/wrappers/gfx/lua_sprite.h>
using namespace am::lua::gfx;

#include <sstream>
using namespace std;

namespace am {
namespace lua {
namespace game {

	/**
	 * @class
	 * Class for representing a Character.
	 * <pre>
	 * npc1 = am.character.new("npc1")
	 * npc1:name("Fredrick Smith")
	 * 
	 * game = am.engine.get_game()
	 * main = game:main()
	 * </pre>
	 */

	/**
	 * @static
	 * Creates or wraps a character instance.
	 *
	 * For example, the first time a character is created (as in the case of a new game)
	 * there should not be any existing character with the same game_id. In which case
	 * a new character will be created.
	 * If the character was not pre-existing then it will need all the specific values
	 * set onto it. However if it was pre-existing then you are unlikely to want to set
	 * it's current values back to what they were when the character was first created.
	 * However event listeners are not maintained between saves, so they will have to be
	 * added to an existing (loaded) character as well as a new one.
	 *
	 * @param string [""] game_id The game id to create the character with or the game id
	 *  of an existing character.
	 * @returns am.character The newly created character or returns an existing character
	 *  with the same game_id.
	 * @returns boolean True if the character was newly created or false if it 
	 *  already existed.
	 */
	/**
	 * @static
	 * Behaves the same as the base constructor however the new character will be created
	 * from the given character definition if it has to be created.
	 * 
	 * @param string [""] game_id The game id to create the character with or the game id
	 *  of an existing character.
	 * @param string def_name The character definition name to look up when creating the
	 *  character if it one did not exist.
	 * @returns am.character The newly created character or returns an existing character
	 *  with the same game_id.
	 * @returns boolean True if the character was newly created or false if it 
	 *  already existed.
	 */
	int Character_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 1 && !lua_isstr(lua, 1) || 
			args > 1 && !lua_isstr(lua, 1) && !lua_isstr(lua, 2))
		{
			return LuaState::expectedArgs(lua, "@new", 3, "", "string id", "string id, string def_name");
		}

		if (args == 0)
		{
			Character *obj = new Character();
			wrapRefObject<Character>(lua, obj);
			lua_pushboolean(lua, true);
			return 2;
		}

		bool newChar = false;
		const char *id = lua_tostring(lua, 1);
		Handle<Character> obj(dynamic_cast<Character *>(Engine::getEngine()->getGameObject(id)));
		if (!obj)
		{
			newChar = true;
			if (lua_isstr(lua, 2))
			{
				Handle<Character> def(Engine::getGame()->getCharDefinition(lua_tostring(lua, 2)));
				if (def)
				{
					obj = new Character(*def);
				}
			}
			if (!obj)
			{
				obj = new Character();
			}
			obj->setGameId(id);
		}
		wrapRefObject<Character>(lua, obj);
		lua_pushboolean(lua, newChar);
		return 2;
	}
	/**
	 * Releases the reference of this character.
	 */
	int Character_dtor(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			obj->release();
			return 0;
		}
		return LuaState::expectedContext(lua, "__gc", "Character");
	}
	/**
	 * Compares this character with another character reference.
	 * Will not return true for equivalent characters.
	 * 
	 * @param am.character rhs The other character to compare with.
	 * @returns boolean True if the characters are the same entity.
	 */
	int Character_eq(lua_State *lua)
	{
		Character *lhs = castUData<Character>(lua, 1);
		if (!lhs)
		{
			LuaState::expectedContext(lua, "__eq", "am.character");
			return 0;
		}
		Character *rhs = castUData<Character>(lua, 2);
		lua_pushboolean(lua, lhs && rhs && lhs == rhs);
		return 1;
	}

	int Character_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Character_ctor },
			{ "__gc", Character_dtor },
			{ "__eq", Character_eq },
			// Character methods
			{ "from_def", Character_from_def },

			{ "name", Character_name },
			{ "description", Character_description },
			{ "pickup_reach", Character_pickup_reach },
			{ "stats", Character_stats },
			{ "add_body_part", Character_add_body_part },
			{ "remove_body_part", Character_remove_body_part },
			{ "has_body_part", Character_has_body_part },
			{ "body_parts", Character_body_parts },
			{ "equip_item", Character_equip_item },
			{ "unequip_item", Character_unequip_item },
			{ "equipped", Character_equipped },
			{ "inventory", Character_inventory },
			{ "pickup_item", Character_pickup_item },
			{ "add_item", Character_add_item },
			{ "remove_item", Character_remove_item },
			{ "has_item", Character_has_item },
			{ "drop_item", Character_drop_item },
			{ "age", Character_age },
			{ "race", Character_race },
			{ "gender", Character_gender },
			{ "coin_purse", Character_coin_purse },
			{ "graphic", Character_graphic },
			// GameObject methods
			{ "location", Character_location },
			{ "grid_location", Character_grid_location },
			{ "move", Character_move },
			{ "move_grid", Character_move_grid },
			{ "talk_to", Character_talk_to },
			{ "fixed_to_grid", Character_fixed_to_grid },
			{ "map", Character_map },
			{ "original_map", Character_original_map },
			{ "add_passible_type", Character_add_passible_type },
			{ "remove_passible_type", Character_remove_passible_type },
			{ "remove_all_passible_types", Character_remove_all_passible_types },
			{ "has_passible_type", Character_has_passible_type },
			{ "get_passible_types", Character_get_passible_types },
			{ "game_id", Character_game_id },
			{ "find", Character_find },
			{ "dialogue_component", Character_dialogue_component },
			// EventListener methods
			{ "on", Character_add_event_listener },
			{ "off", Character_remove_event_listener },
			{ "has_event_listener", Character_has_event_listener },
			// Levelable methods
			{ "experience", Character_experience },
			{ "add_experience", Character_add_experience },
			{ "level", Character_level },
			{ "add_level", Character_add_level },
			{ "max_level", Character_max_level },
			{ "ai_func", Character_ai_func },
			// Attributes
			{ "attrs", Character_attrs },
			{ "debug", Character_debug },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, Character::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");
		
		return 1;
	}

	/**
	 * @static
	 * Creates a new character from the character definition.
	 * Character definitions are automatically loaded if one with
	 * the given name is not registered.
	 * <p>Example (a test map Lua file):</p>
	 * <pre>
	 * npc = am.character.from_def("npcs:male1")
	 * if (npc ~= nil)
	 *     npc:name("Fred")
	 * end
	 * </pre>
	 * <p>In "data/defs/npcs.lua":</p>
	 * <pre>
	 * game = am.engine.game()
	 * 
	 * npc = am.character.new()
	 * npc:age(21):gender("male"):graphic(am.sprite.new("characters/npc/front"))
	 * 
	 * -- Here the npc is registered with the name "male1" and "npcs:" will 
	 * -- automatically be prepended because of the filename.
	 * game:char_def("male1", npc) 
	 * </pre>
	 *
	 * @param string def_name The name of the character definition.
	 * @param string [""] game_id The game id to give to the newly created character.
	 * @returns am.character A copy of the character from the given definition, or nil if no definition was found.
	 */
	int Character_from_def(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (lua_isstr(lua, 1) && (args == 1 || args > 1 && lua_isstr(lua, 2)))
		{
			Game *game = Engine::getGame();
			if (game)
			{
				Character *def = game->getCharDefinition(lua_tostring(lua, 1));
				if (def)
				{
					Character *newChar = new Character(*def);
					if (lua_isstr(lua, 2))
					{
						newChar->setGameId(lua_tostring(lua, 2));
					}
					wrapRefObject<Character>(lua, newChar);
					return 1;
				}
				stringstream ss;
				ss << "No character with the definition '" << lua_tostring(lua, 1) << "' was found.";
				LuaState::warning(lua, ss.str().c_str());
				lua_pushnil(lua);
				return 1;
			}
			stringstream ss;
			ss << "Unable to get definition (" << lua_tostring(lua, 1) << " when a game is not running";
			LuaState::warning(lua, ss.str().c_str());
			lua_pushnil(lua);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@from_def", "string def_name, string [\"\"] game_id");
	}
	
	/**
	 * Returns the name of the character.
	 * A nil return indicates that there was an error.
	 *
	 * @returns string The name of the character.
	 */
	/**
	 * Sets the name of the character.
	 *
	 * @param string name The name to set the character.
	 * @returns am.character This.
	 */
	int Character_name(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
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
		return LuaState::expectedContext(lua, "name", "am.character");
	}

	/**
	 * Returns the description of the character.
	 * A nil return indicates that there was an error.
	 *
	 * @returns string The description of the character.
	 */
	/**
	 * Sets the description of the character.
	 *
	 * @param string description The description to set the character.
	 * @returns am.character This.
	 */
	int Character_description(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
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
		return LuaState::expectedContext(lua, "description", "am.character");
	}

	/**
	 * Returns the pickup reach of the character.
	 * The pickup reach is the radius around the character in grid units
	 * in which the character is able to pickup items and interact with objects.
	 *
	 * @returns number The pickup reach radius.
	 */
	/**
	 * Sets the pickup reach radius for the character.
	 * The pickup reach is the radius around the character in grid units
	 * in which the character is able to pickup items and interact with objects.
	 *
	 * @param number radius The pickup reach radius.
	 * @returns am.character This.
	 */
	int Character_pickup_reach(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_gettop(lua) == 1) 
			{
				lua_pushnumber(lua, obj->getPickupReach());
				return 1;
			}
			else if (lua_isnum(lua, 2))
			{
				obj->setPickupReach(lua_tofloat(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "pickup_reach", "number radius");
		}
		return LuaState::expectedContext(lua, "pickup_reach", "am.character");
	}
	/**
	 * Returns the stats container linked with this character.
	 * @see am.stats
	 *
	 * @returns am.stats The stats container.
	 */
	int Character_stats(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			wrapRefObject<Stats>(lua, obj->getStats());
			return 1;
		}
		return LuaState::expectedContext(lua, "stats", "am.character");
	}
	/**
	 * Attempts to add a body part to the character.
	 * Returns true if this was successful, false if there was
	 * another body part with the same name attached.
	 *
	 * @param am.body_part part The body part to add
	 * @returns boolean If part was added successfully.
	 */
	/**
	 * Attempts to add a body part with the given name to the character.
	 * Returns true if this was successful, false if there was
	 * another body part with the same name attached.
	 *
	 * @param string part_name The name of the body part to create and add to the character.
	 * @param string [UNKNOWN_PART] part_type The type of the body part.
	 * @returns boolean If part was added successfully.
	 */
	int Character_add_body_part(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isstr(lua, 2))
			{
				BodyPartType::PartType type = BodyPartType::UNKNOWN_PART;
				if (lua_isstr(lua, 3))
				{
					type = BodyPartType::getBodyPartType(lua_tostring(lua, 3));
				}
				lua_pushboolean(lua, obj->addBodyPart(new BodyPart(lua_tostring(lua, 2), type)));
				return 1;
			}
			BodyPart *part = castUData<BodyPart>(lua, 2);
			if (part)
			{
				lua_pushboolean(lua, obj->addBodyPart(part));
				return 1;
			}
			return LuaState::expectedArgs(lua, "add_body_part", 2, "am.body_part part", "string part_name, string [UNKNOWN_PART] part_type");
		}
		return LuaState::expectedContext(lua, "add_body_part", "am.character");
	}
	/**
	 * Removes a body part with the same name from the character.
	 *
	 * @param am.body_part part The body part to remove.
	 * @returns boolean True if the body part was removed.
	 */
	/**
	 * Removes a body part with the same name from the character.
	 *
	 * @param string part_name The body part to remove.
	 * @returns boolean True if the body part was removed.
	 */
	int Character_remove_body_part(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isstr(lua, 2))
			{
				lua_pushboolean(lua, obj->removeBodyPart(lua_tostring(lua, 2)));
				return 1;
			}
			else
			{
				BodyPart *part = castUData<BodyPart>(lua, 2);
				if (part)
				{
					lua_pushboolean(lua, obj->removeBodyPart(part));
					return 1;
				}
			}
			return LuaState::expectedArgs(lua, "remove_body_part", 2, "string part_name", "am.body_part part");
		}
		return LuaState::expectedContext(lua, "remove_body_part", "am.character");
	}
	/**
	 * Tells if a body part with the same name is attached to the character.
	 *
	 * @param am.body_part part The body part to check for.
	 * @returns boolean True if the body part is attached.
	 */
	/**
	 * Tells if a body part with the same name is attached to the character.
	 *
	 * @param string part_name The body part to check for.
	 * @returns boolean True if the body part is attached.
	 */
	int Character_has_body_part(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isstr(lua, 2))
			{
				lua_pushboolean(lua, obj->hasBodyPart(lua_tostring(lua, 2)));
				return 1;
			}

			BodyPart *part = castUData<BodyPart>(lua, 2);
			if (part)
			{
				lua_pushboolean(lua, obj->hasBodyPart(part));
				return 1;
			}
			return LuaState::expectedArgs(lua, "has_body_part", 2, "am.body_part part", "string part_name");
		}
		return LuaState::expectedContext(lua, "has_body_part", "am.character");
	}
	/**
	 * Returns a table of all the body parts attached to this character.
	 * Returns nil if there was an error.
	 * Each table entry is a key/value pair with the key being the body parts name.
	 *
	 * @returns table A table of all the body parts.
	 */
	int Character_body_parts(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			LuaState L(lua);
			L.newTable();
			const BodyParts::PartList &list = obj->getBodyParts().getAllParts();
			for (size_t i = 0; i < list.size(); i++)
			{
				lua_pushstring(lua, list[i]->getName());
				wrapRefObject<BodyPart>(lua, list[i]);
				lua_settable(lua, -3);
			}
			return 1;
		}
		return LuaState::expectedContext(lua, "body_parts", "am.character");
	}

	/**
	 * Equips an item to the character on the given body part.
	 *
	 * @param am.item item The item to equip, it cannot be nil.
	 * @param am.body_part part The body part to equip the item to.
	 * @returns boolean True if the item was successfully equipped,
	 * false can mean that there was already an item equipped, 'part'
	 * was nil, or 'part' was not an attached body part.
	 */
	/**
	 * Equips an item to the character on the given body part.
	 *
	 * @param am.item item The item to equip, it cannot be nil.
	 * @param string part_name The body part to equip the item to.
	 * @returns boolean True if the item was successfully equipped,
	 * false can mean that there was already an item equipped, 
	 * or 'part_name' was not an attached body part.
	 */
	int Character_equip_item(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			Item *item = castUData<Item>(lua, 2);
			if (lua_isstr(lua, 3))
			{
				lua_pushboolean(lua, obj->equipItem(item, lua_tostring(lua, 3)));
				return 1;
			}
			else
			{
				BodyPart *part = castUData<BodyPart>(lua, 3);
				if (part)
				{
					lua_pushboolean(lua, obj->equipItem(item, part->getName()));
					return 1;
				}
			}
			return LuaState::expectedArgs(lua, "equip_item", 2, "am.item item, string part_name", "am.item item, am.body_part part");
		}
		return LuaState::expectedContext(lua, "equip_item", "am.character");
	}
	/**
	 * Unequips the item on the given body part for this character.
	 *
	 * @param am.body_part part The body part to unequip the item from.
	 * @returns boolean True if the item was unequipped or if the body part
	 * was already empty. False can mean 'part' was nil or that 
	 * 'part' was not an attached body part.
	 */
	/**
	 * Unequips the item on the given body part for this character.
	 *
	 * @param string part_name The body part to unequip the item from.
	 * @returns boolean True if the item was unequipped or if the body part
	 * was already empty. False can mean that 'part_name' was not an attached body part.
	 */
	int Character_unequip_item(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isstr(lua, 2))
			{
				lua_pushboolean(lua, obj->unequipItem(lua_tostring(lua, 2)));
				return 1;
			}
			else
			{
				BodyPart *part = castUData<BodyPart>(lua, 3);
				if (part)
				{
					lua_pushboolean(lua, obj->unequipItem(part->getName()));
					return 1;
				}
			}
			return LuaState::expectedArgs(lua, "unequip_item", 2, "am.body_part fromPart", "string fromPartName");
		}
		return LuaState::expectedContext(lua, "unequip_item", "am.character");
	}
	/**
	 * Returns the item equipped at the given body part.
	 *
	 * @param string part_name The body part to get the item from.
	 * @returns am.item The item equipped, nil if there was no item equipped
	 * or if there was not body part with 'part_name' found.
	 */
	int Character_equipped(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			const char *partName = nullptr;
			if (lua_isstr(lua, 2))
			{
				partName = lua_tostring(lua, 2);
			}
			else
			{
				BodyPart *part = castUData<BodyPart>(lua, 2);
				if (part)
				{
					partName = part->getName();
				}
			}
			if (partName)
			{
				Handle<Item> item(obj->getEquipped(lua_tostring(lua, 2)));
				if (item)
				{
					wrapRefObject<Item>(lua, item);
					return 1;
				}
			}
			return LuaState::expectedArgs(lua, "equipped", 2, "am.body_part part", "string part_name");
		}
		return LuaState::expectedContext(lua, "equipped", "am.character");
	}
	/**
	 * Returns the inventory object attached to this character.
	 *
	 * @returns am.inventory The attached inventory, nil if there was an error.
	 */
	int Character_inventory(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			wrapRefObject<Inventory>(lua, obj->getInventory());
			return 1;
		}
		return LuaState::expectedContext(lua, "inventory", "am.character");
	}
	/**
	 * Attempts to pickup an item.
	 *
	 * @param am.item item The item to attempt to pickup.
	 * @returns am.code Return code for the pickup:
	 * <table class='return_codes'>
	 *  <tr><td>success</td><td>The item was picked up.</td></tr>
	 *  <tr><td>nil_parameter</td><td>The item was nil.</td></tr>
	 *  <tr><td>out_of_range</td><td>The item was out of range of this characters pickup radius.</td></tr>
	 *  <tr><td>not_enough_inventory_space</td><td>There's not enough space for the item to be picked up.</td></tr>
	 * </table>
	 */
	int Character_pickup_item(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			Item *item = castUData<Item>(lua, 2);
			if (item)
			{
				lua_pushinteger(lua, obj->pickupItem(item));
				return 1;
			}
			return LuaState::expectedArgs(lua, "pickup_item", "am.item item");
		}
		return LuaState::expectedContext(lua, "pickup_item", "am.character");
	}
	/**
	 * Adds an item to the characters inventory.
	 * Returns true if it was successful, false there was not enough space.
	 *
	 * @param am.item item The item to add.
	 * @returns boolean True for successful add.
	 */
	int Character_add_item(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			Item *item = castUData<Item>(lua, 2);
			if (item)
			{
				lua_pushboolean(lua, obj->addItem(item));
				return 1;
			}
			return LuaState::expectedArgs(lua, "add_item", "am.item item");
		}
		return LuaState::expectedContext(lua, "add_item", "am.character");
	}
	/**
	 * Removes an item from the characters inventory.
	 * Returns true if the item was removed, false if the item was nil
	 * or if the item could not be found.
	 *
	 * @param am.item item The item to remove.
	 * @returns boolean True if removal was successful.
	 */
	int Character_remove_item(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			Item *item = castUData<Item>(lua, 2);
			if (item)
			{
				lua_pushboolean(lua, obj->removeItem(item));
				return 1;
			}
			return LuaState::expectedArgs(lua, "remove_item", "am.item item");
		}
		return LuaState::expectedContext(lua, "remove_item", "am.character");
	}
	/**
	 * Returns true if an item is in the characters inventory.
	 * Returns false if the item was nil or not found.
	 *
	 * @param am.item item The item to search for.
	 * @returns boolean True if found successfully.
	 */
	int Character_has_item(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			Item *item = castUData<Item>(lua, 2);
			if (item)
			{
				lua_pushboolean(lua, obj->hasItem(item));
				return 1;
			}
			return LuaState::expectedArgs(lua, "has_item", "am.item item");
		}
		return LuaState::expectedContext(lua, "has_item", "am.character");
	}
	/**
	 * Drops an item at the characters feet.
	 * Return codes:
	 * <ul>
	 * <li>1: Success.</li>
	 * <li>0: Item was nil.</li>
	 * <li>-1: Drop location was too far away (should not happen).</li>
	 * <li>-2: Drop location was invalid for item.</li>
	 * <li>-3: Error.</li>
	 * </ul>
	 *
	 * @param am.item item The item to drop
	 * @returns integer Drop item return code:
	 * <table class='return_codes'>
	 *  <tr><td>success</td><td>The item was dropped.</td></tr>
	 *  <tr><td>nil_parameter</td><td>The item was nil.</td></tr>
	 *  <tr><td>off_the_map</td><td>The drop location was off the map.</td></tr>
	 *  <tr><td>out_of_range</td><td>The item was out of range of this characters pickup radius to drop (should not happen).</td></tr>
	 *  <tr><td>invalid_location</td><td>The drop location was invalid for the item.</td></tr>
	 * </table>
	 */
	/**
	 * Drops an item at the given map coordinates.
	 * Return codes:
	 * <ul>
	 * <li>1: Success.</li>
	 * <li>0: Item was nil.</li>
	 * <li>-1: Drop location was too far away.</li>
	 * <li>-2: Drop location was invalid for item.</li>
	 * <li>-3: Error.</li>
	 * </ul>
	 *
	 * @param am.item item The item to drop
	 * @param number x X map location to drop at
	 * @param number y Y map location to drop at
	 * @returns integer Drop item return code.
	 */
	int Character_drop_item(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			Item *item = castUData<Item>(lua, 2);
			if (item)
			{
				if (lua_isnum(lua, 3) && lua_isnum(lua, 4))
				{
					lua_pushinteger(lua, obj->dropItem(item, lua_tofloat(lua, 3), lua_tofloat(lua, 4)));
				}
				else
				{
					lua_pushinteger(lua, obj->dropItem(item));
				}
				return 1;
			}
			return LuaState::expectedArgs(lua, "drop_item", 2, "am.item item", "am.item item, number x, number y");
		}
		return LuaState::expectedContext(lua, "drop_item", "am.character");
	}
	/**
	 * Returns the age of the character.
	 *
	 * @returns number The age of the character
	 */
	/**
	 * Sets the age of the character.
	 *
	 * @param number age The new characters age
	 * @returns am.character This.
	 */
	int Character_age(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushnumber(lua, obj->getAge());
				return 1;
			}
			else if (lua_isnum(lua, 2))
			{
				obj->setAge(lua_tofloat(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "age", "number age");
		}
		return LuaState::expectedContext(lua, "age", "am.character");
	}
	/**
	 * Returns the characters Race.
	 * @returns Race The characters Race
	 */
	/**
	 * Sets the race of the character.
	 * If the race could not be found in the game engine, 
	 * the characters race is unchanged.
	 * 
	 * @param string race_name The name of the race.
	 * @returns am.character This
	 */
	/**
	 * Sets the race of the character, nil will set to the unknown race.
	 *
	 * @param Race race The race to change to.
	 * @returns am.character This
	 */
	int Character_race(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_gettop(lua) == 1)
			{
				Race *race = obj->getRace();
				if (race)
				{
					wrapRefObject<Race>(lua, obj->getRace());
				}
				else
				{
					wrapRefObject<Race>(lua, Engine::getEngine()->getUnknownRace());
				}
				return 1;
			}
			else if(lua_isnil(lua, 2))
			{
				obj->setRace(nullptr);
				lua_first(lua);
			}
			else
			{
				Race *race = nullptr;
				if (lua_type(lua, 2) == LUA_TSTRING)
				{
					race = Engine::getEngine()->getRace(lua_tostring(lua, 2));
				}
				else
				{
					race = castUData<Race>(lua, 2);
				}
				if (race)
				{
					obj->setRace(race);
					lua_first(lua);
				}
			}
			return LuaState::expectedArgs(lua, "race", 3, "string race_name", "Race race", "nil");
		}
		return LuaState::expectedContext(lua, "race", "am.character");
	}
	/**
	 * Returns the gender of the character.
	 *
	 * @returns string The characters gender
	 */
	/**
	 * Sets the characters gender.
	 * Currently only supports "male" and "female", if it is not one of these
	 * the characters gender is not changed.
	 *
	 * @param string gender The gender to change to.
	 * @returns am.character This
	 */
	int Character_gender(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, Gender::getGenderName(obj->getGender()));
				return 1;
			}
			else
			{
				Gender::GenderType gender = Gender::MAX_GENDER_LENGTH;
				if (lua_isnum(lua, 2))
				{
					gender = Gender::getGenderInt(lua_tointeger(lua, 2));
				
				}
				else if (lua_isstr(lua, 2))
				{
					gender = Gender::getGenderType(lua_tostring(lua, 2));
				}
				if (gender != Gender::MAX_GENDER_LENGTH) 
				{
					obj->setGender(gender);
					lua_first(lua);
				}
				return LuaState::expectedArgs(lua, "gender", "String genderName");
			}
		}
		return LuaState::expectedContext(lua, "gender", "am.character");
	}
	/**
	 * Returns the characters coin purse.
	 * This stores all the coin that the character currently holds.
	 *
	 * @returns am.coin_purse The characters coin purse.
	 */
	int Character_coin_purse(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			wrapRefObject<CoinPurse>(lua, obj->getCoinPurse());
			return 1;
		}
		return LuaState::expectedContext(lua, "coin_purse", "am.character");
	}
	/**
	 * Returns the graphic attached to this character, can be nil.
	 *
	 * @returns am.sprite The characters sprite.
	 */
	/**
	 * Sets the characters asset graphic.
	 * Sets the asset_name of the graphic for this character.
	 *
	 * @param string asset_name The asset name to the graphic.
	 * @returns am.character This
	 */
	/**
	 * Sets the characters graphic, can be nil.
	 *
	 * @param am.sprite sprite The graphic to set.
	 * @returns am.character This
	 */
	int Character_graphic(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_gettop(lua) == 1)
			{
				Sprite *sprite = obj->getGraphic();
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
					obj->setGraphic(new Sprite(lua_tostring(lua, 2)));
					valid = true;
				}
				else if (lua_isnil(lua, 2))
				{
					obj->setGraphic(nullptr);
					valid = true;
				}
				else
				{
					Sprite *sprite = castUData<Sprite>(lua, 2);
					if (sprite)
					{
						obj->setGraphic(sprite);
						valid = true;
					}
				}
				if (valid)
				{
					lua_first(lua);
				}
				return LuaState::expectedArgs(lua, "graphic", 3, "string asset_name", "Sprite sprite", "nil");
			}
		}
		return LuaState::expectedContext(lua, "graphic", "am.character");
	}
	/**
	 * Returns the characters current map location.
	 * @returns number The characters x map value
	 * @returns number The characters y map value
	 */
	/**
	 * Sets the characters map location.
	 * @param number x The x map value
	 * @param number y The y map value
	 * @returns am.character This
	 */
	int Character_location(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
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
		return LuaState::expectedContext(lua, "location", "am.character");
	}
	/**
	 * Returns the characters currnet map grid location.
	 * @returns integer x The x grid value
	 * @returns integer y The y grid value
	 */
	/**
	 * Sets the characters current map grid location.
	 * @param integer x The x grid value
	 * @param integer y The y grid value
	 * @returns am.character This
	 */
	int Character_grid_location(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
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
		return LuaState::expectedContext(lua, "grid_location", "am.character");
	}
	/**
	 * Moves a character by {x, y} amount.
	 * If the new location isn't valid, the character isn't moved.
	 * @param number x The x amount to move by
	 * @param number y The y amount to move by
	 * @returns am.character This
	 */
	int Character_move(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
			{
				obj->move(lua_tofloat(lua, 2), lua_tofloat(lua, 3));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "move", "number x, number y");
		}
		return LuaState::expectedContext(lua, "move", "am.character");
	}
	/**
	 * Moves a character by {x, y} grid amounts.
	 * If the new location isn't valid, the character isn't moved.
	 * @param integer x The x grid amount to by move by
	 * @param integer y The y grid amount to by move by
	 * @returns am.character This
	 */
	int Character_move_grid(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
			{
				obj->moveGrid(lua_tointeger(lua, 2), lua_tointeger(lua, 3));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "move_grid", "integer x, integer y");
		}
		return LuaState::expectedContext(lua, "move_grid", "am.character");
	}
	/**
	 * Initiates a conversation between this character
	 * and the game object id passed in.
	 * Returns true if the conversation was initiated.
	 * False can mean that the game object id was invalid or that
	 * either this character or the game object did not have a
	 * dialog component attached.
	 * @see am.dialog_component
	 * @param string id The game object id.
	 * @returns boolean True if the conversation was initiated.
	 */
	/**
	 * Initiates a conversation between this character
	 * and the game object passed in.
	 * Returns true if the conversation was initiated.
	 * False can mean that the game object was nil or that
	 * either this character or the game object did not have a
	 * dialog component attached.
	 * @see am.dialog_component
	 * @param am.game_object talkee The game object to start a converstation with.
	 * @returns boolean True if the conversation was initiated.
	 */
	int Character_talk_to(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
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
			LuaState::warning(lua, "Character needs a dialogue component to talk to another game object");
			lua_pushboolean(lua, false);
			return 1;
		}
		return LuaState::expectedContext(lua, "talk_to", "am.character");
	}
	/**
	 * Returns true if this character is fixed to the game grid
	 * or false if the character is able to move freely about the map.
	 * @returns boolean True if the character is fixed to the grid.
	 */
	/**
	 * Sets if the character is fixed to the grid or not.
	 * If a character is fixed to the grid, they cannot move to positions
	 * other than the center of each grid space.
	 * @param boolean fixed Sets the fixed to grid value.
	 * @returns am.character This
	 */
	int Character_fixed_to_grid(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
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
		return LuaState::expectedContext(lua, "fixed_to_grid", "am.character");
	}

	/**
	 * Returns the current map that this character is on, can be nil.
	 *
	 * @returns am.map The map the character is on.
	 */
	/**
	 * Sets the map that the character is on, can be nil.
	 *
	 * @param am.map map The map to put the character on.
	 * @returns am.character This
	 */
	int Character_map(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
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
		return LuaState::expectedContext(lua, "map", "am.character");
	}

	/**
	 * Returns the original map that this character was on, can be nil.
	 *
	 * @returns am.map The map the character was originally on.
	 */
	/**
	 * Sets the map that the character was originally on, can be nil.
	 * This is usually set automatically the first time a character is
	 * added to a map. This is primarily used for reloading the character
	 * and knowing where to find likely find any event handlers and additional
	 * information relating to the character that is not stored directly on the
	 * character.
	 *
	 * @param am.map map The map the character was originally was on.
	 * @returns am.character This
	 */
	int Character_original_map(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
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
			return LuaState::expectedArgs(lua, "map", 2, "nil map", "Map map");
		}
		return LuaState::expectedContext(lua, "map", "am.character");
	}
	/**
	 * Adds a tile type to the list of tile types that this character
	 * can move freely on. TileTypes can be added multiple times.
	 * 
	 * @param am.tile_type tile_type The type to add to the list.
	 * @returns am.character This
	 */
	/**
	 * Adds a tile type to the list of tile types that this character
	 * can move freely on. Tile types can be added multiple times.
	 * 
	 * @param string tile_type_name The name of the type to add to the list.
	 * @returns am.character This
	 */
	int Character_add_passible_type(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
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
		return LuaState::expectedContext(lua, "add_passible_type", "am.character");
	}
	/**
	 * Removes a tile type from the list of passible tiles.
	 * @param am.tile_type tile_type The tile type to remove.
	 * @returns am.character This
	 */
	int Character_remove_passible_type(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			Handle<TileType> type(castUData<TileType>(lua, 2));
			if (type)
			{
				obj->removePassibleType(type);
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "remove_passible_type", "am.tile_type tile_type");
		}
		return LuaState::expectedContext(lua, "remove_passible_type", "am.character");
	}
	/**
	 * Removes all tile types from this characters passible list.
	 * @returns am.character This
	 */
	int Character_remove_all_passible_types(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			obj->removeAllPassibleTypes();
			lua_first(lua);
		}
		return LuaState::expectedContext(lua, "remove_all_passible_types", "am.character");
	}
	/**
	 * Returns true if the given tile type is found on this characters
	 * passible list.
	 * @param am.tile_type tile_type The tile type to search for
	 * @returns boolean True if the tile type is found
	 */
	int Character_has_passible_type(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			Handle<TileType> type(castUData<TileType>(lua, 2));
			if (type)
			{
				lua_pushboolean(lua, obj->hasPassibleType(type));
				return 1;
			}
			return LuaState::expectedArgs(lua, "has_passible_type", "am.tile_type tile_type");
		}
		return LuaState::expectedContext(lua, "has_passible_type", "am.character");
	}
	/**
	 * Returns a table of all the passible tile types for this character.
	 * @returns table Table of all the tile types.
	 */
	int Character_get_passible_types(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
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
		return LuaState::expectedContext(lua, "get_passible_types", "am.character");
	}
	/**
	 * @static
	 * Looks for a character with the given game id.
	 * Nil is returned if it is not found.
	 * @param string game_id The game id to look up
	 * @returns am.character The found character or nil
	 */
	int Character_find(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			Character *obj = dynamic_cast<Character *>(Engine::getEngine()->getGameObject(lua_tostring(lua, 1)));
			if (obj)
			{
				wrapRefObject<Character>(lua, obj);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@find", "string game_id");
	}
	/**
	 * @see am.dialogue_component
	 * Returns the dialogue component attached to this character, can be nil.
	 * @returns am.dialogue_component The attached dialogue component
	 */
	/**
	 * Sets a dialogue component onto this character, can be nil.
	 * @param am.dialogue_component comp The dialogue component to attach to this character, can be nil.
	 * @param boolean [true] set_attached When true it also sets that this character
	 * is the game object attached to the dialogue component.
	 */
	int Character_dialogue_component(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
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
		return LuaState::expectedContext(lua, "dialogue_component", "am.character");
	}

	/**
	 * Returns the game id for this character.
	 * @returns string The characters game id.
	 */
	/**
	 * Sets the characters game id, this is used to refer to this
	 * character from the game engine. Should be unique.
	 * @param string game_id The characters unique game id.
	 * @returns boolean True if the game id change was successful or if
	 * game id was the same as the given game_id. False indicates that
	 * either there is no current game engine or there is another
	 * game object with the same game id.
	 */
	int Character_game_id(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
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
		return LuaState::expectedContext(lua, "id", "am.character");
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
	int Character_add_event_listener(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isstr(lua, 2) && lua_isfunction(lua, 3))
			{
				lua_pushboolean(lua, am::lua::ui::addEventListener(lua, obj));
				return 1;
			}
			return LuaState::expectedArgs(lua, "on", "string event_type, function listener");
		}
		return LuaState::expectedContext(lua, "on", "am.character");
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
	int Character_remove_event_listener(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isstr(lua, 2) && lua_isfunction(lua, 3))
			{
				lua_pushboolean(lua, am::lua::ui::removeEventListener(lua, obj));
				return 1;
			}
			return LuaState::expectedArgs(lua, "off", "string event_type, function listener");
		}
		return LuaState::expectedContext(lua, "off", "am.character");
	}
	/**
	 * Returns true when there is an event listener for the given event_type.
	 * @param string event_type The event type to look up.
	 * @returns boolean True if there is any event listener 
	 * that will be trigged by this event type.
	 */
	int Character_has_event_listener(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isstr(lua, 2))
			{
				lua_pushboolean(lua, obj->hasEventListener(lua_tostring(lua, 2)));
				return 1;
			}
			return LuaState::expectedArgs(lua, "has_event_listener", "string event_type");
		}
		return LuaState::expectedContext(lua, "has_event_listener", "am.character");
	}

	/**
	 * Returns the amount of experience this character has.
	 * @returns integer The amount of experience this character has.
	 */
	/**
	 * Sets the amount of experience this character has.
	 * If the amount of experience is enough to level up the character,
	 * they will be leveled up to the appropriate level.
	 * A characters level cannot go down again, even if their experience is
	 * too low for their current level.
	 * If the character has a max level and the experience is more than 
	 * the characters max level experience, it will be capped at that amount.
	 * <br>
	 * It is generally recommended to simply add experience to a character.
	 * @param integer experience The amount of experience to set on this character.
	 * @returns am.character This
	 */
	int Character_experience(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushinteger(lua, obj->getExperience());
				return 1;
			}
			else if (lua_isnum(lua, 2))
			{
				obj->setExperience(lua_tointeger(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "experience", "integer experience");
		}
		return LuaState::expectedContext(lua, "experience", "am.character");
	}
	/**
	 * Adds the given amount of experience to the total amount of 
	 * experience this character has. The total will always be capped
	 * at the characters max level if they have one.
	 * @param integer experience The amount of experience to add.
	 * @returns am.character This
	 */
	int Character_add_experience(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isnum(lua, 2))
			{
				obj->addExperience(lua_tointeger(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "add_experience", "integer experience");
		}
		return LuaState::expectedContext(lua, "add_experience", "am.character");
	}
	/**
	 * Returns the characters current level.
	 * @returns integer The characters level
	 */
	/**
	 * Sets the characters level.
	 * If the new level is higher than the characters current level,
	 * their experience is also set at the amount required to be that level.
	 * However a characters level cannot be set lower than what it currently is.
	 * The level cannot be set higher than a characters max level 
	 * if they have a max level set.
	 *
	 * It is generally recommended to use add_level instead of setting it directly.
	 * @param integer level The characters new level.
	 * @returns am.character This
	 */
	int Character_level(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushinteger(lua, static_cast<lua_Integer>(obj->getLevel()));
				return 1;
			}
			else if (lua_isnum(lua, 2))
			{
				obj->setLevel(static_cast<short>(lua_tointeger(lua, 2)));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "level", "integer level");
		}
		return LuaState::expectedContext(lua, "level", "am.character");
	}
	/**
	 * Adds the given amount of levels to this character.
	 * This new level will not go over a characters max level if they have one set.
	 * The characters experience will also be set the appropriate amount
	 * for them to be this level.
	 * @param integer levels The number of levels to add to this character
	 * @returns am.character This
	 */
	int Character_add_level(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isnum(lua, 2))
			{
				obj->addLevel(static_cast<short>(lua_tointeger(lua, 2)));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "add_level", "integer level");
		}
		return LuaState::expectedContext(lua, "add_level", "am.character");
	}

	/**
	 * Returns this characters maximum level that they can achieve.
	 * If max level is set to 0, their effective max level will be 2^15 (32768).
	 * @returns integer The characters current max level.
	 */
	/**
	 * Sets the characters max level. If the max level is set to 0, then their
	 * effective max level will be 2^15 (32768).
	 * @param integer max_level The characters max level.
	 * @returns am.character This
	 */
	int Character_max_level(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushinteger(lua, static_cast<lua_Integer>(obj->getMaxLevel()));
				return 1;
			}
			else if (lua_isnum(lua, 2))
			{
				obj->setMaxLevel(static_cast<short>(lua_tointeger(lua, 2)));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "max_level", "integer max_level");
		}
		return LuaState::expectedContext(lua, "max_level", "am.character");
	}
	/**
	 * Returns the characters attribute data table.
	 * By default if no attribute data table is present nil is returned unless true
	 * is passed as the first argument, then a data table is created if one is not present.
	 * 
	 * @param boolean [false] create_table Create a data table if one didn't exist.
	 * @returns DataTable The data table on this character.
	 */
	/**
	 * Sets the data table on this character, can be set to nil.
	 *
	 * @param am.data_table attr_table The data table to set on the character.
	 * @returns am.character This
	 */
	int Character_attrs(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			return IAttributeData_attrs(lua, obj);
		}
		return LuaState::expectedContext(lua, "attrs", "am.character");
	}

	int Character_ai_func(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isfunction(lua, -1))
			{
				LuaState::logStack(lua, "AI");
				int funcRef = luaL_ref(lua, LUA_REGISTRYINDEX);
				obj->setAIFunc(funcRef);
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "ai_func", "function function");
		}
		return LuaState::expectedContext(lua, "ai_func", "am.character");
	}

	void charRelease(IManaged *obj)
	{
		stringstream ss;
		ss << "Released " << obj->getReferenceCounter();
		am_log("CHAR", ss);
	}
	void charRetain(IManaged *obj)
	{
		stringstream ss;
		ss << "Retain " << obj->getReferenceCounter();
		am_log("CHAR", ss);
	}

	int Character_debug(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			obj->onRelease(&charRelease);
			obj->onRetain(&charRetain);
			lua_first(lua);
		}
		return LuaState::expectedContext(lua, "debug", "am.character");
	}
}
}
}
