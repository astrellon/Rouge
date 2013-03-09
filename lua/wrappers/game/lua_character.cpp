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
using namespace am::game;

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
	 * Character, Game, Engine = import("Character", "Game", "Engine")
	 * npc1 = Character.new("npc1")
	 * npc1:name("Fredrick Smith")
	 * 
	 * game = Engine.get_game()
	 * main = game:main()
	 * </pre>
	 */

	/**
	 * Creates or wraps a character instance.
	 *
	 * @param string gameId The game id to create the character with or the game id
	 * of an existing character.
	 */
	int Character_ctor(lua_State *lua)
	{
		if (!lua_isstr(lua, 1))
		{
			return LuaState::expectedArgs(lua, "Character.new", "string id");
		}

		const char *id = lua_tostring(lua, 1);
		Character *obj = dynamic_cast<Character *>(Engine::getEngine()->getGameObject(id));
		if (obj == NULL)
		{
			obj = new Character();
			obj->setGameId(id);
		}
		wrapRefObject<Character>(lua, obj);
		return 1;
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
	 * @param Character rhs The other character to compare with.
	 * @returns boolean True if the characters are the same entity.
	 */
	int Character_eq(lua_State *lua)
	{
		Character *lhs = castUData<Character>(lua, 1);
		if (!lhs)
		{
			LuaState::expectedContext(lua, "__eq", "Character");
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
			{ "name", Character_name },
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
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Character::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");
		
		return 1;
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
	 * @returns Character This.
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
				lua_pushvalue(lua, 1);
				return 1;
			}
			return LuaState::expectedArgs(lua, "name", "string name");
		}
		return LuaState::expectedContext(lua, "name", "Character");
	}

	/**
	 * Returns the pickup reach of the character.
	 * The pickup reach is the radius around the character in grid units
	 * in which the character is able to pickup items and interact with objects.
	 *
	 * @returns Number The pickup reach radius.
	 */
	/**
	 * Sets the pickup reach radius for the character.
	 * The pickup reach is the radius around the character in grid units
	 * in which the character is able to pickup items and interact with objects.
	 *
	 * @param number radius The pickup reach radius.
	 * @returns Character This.
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
				lua_pushvalue(lua, 1);
				return 1;
			}
			return LuaState::expectedArgs(lua, "pickup_reach", "number radius");
		}
		return LuaState::expectedContext(lua, "pickup_reach", "Character");
	}
	/**
	 * Returns the Stats container linked with this character.
	 * @see Stats
	 *
	 * @returns Stats The Stats container.
	 */
	int Character_stats(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			Stats *stats = &obj->getStats();
			wrapObject<Stats>(lua, stats);
			return 1;
		}
		return LuaState::expectedContext(lua, "stats", "Character");
	}
	/**
	 * Attempts to add a body part to the character.
	 * Returns true if this was successful, false if there was
	 * another BodyPart with the same name attached.
	 *
	 * @param BodyPart part The BodyPart to add
	 * @returns boolean If part was added successfully.
	 */
	int Character_add_body_part(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			BodyPart *part = castUData<BodyPart>(lua, 2);
			if (part)
			{
				lua_pushboolean(lua, obj->addBodyPart(part));
				return 1;
			}
			return LuaState::expectedArgs(lua, "add_body_part", "BodyPart part");
		}
		return LuaState::expectedContext(lua, "add_body_part", "Character");
	}
	/**
	 * Removes a BodyPart with the same name from the Character.
	 *
	 * @param BodyPart part The BodyPart to remove.
	 * @returns boolean True if the BodyPart was removed.
	 */
	/**
	 * Removes a BodyPart with the same name from the Character.
	 *
	 * @param string partName The body part to remove.
	 * @returns boolean True if the BodyPart was removed.
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
			return LuaState::expectedArgs(lua, "remove_body_part", 2, "string partName", "BodyPart part");
		}
		return LuaState::expectedContext(lua, "remove_body_part", "Character");
	}
	/**
	 * Tells if a BodyPart with the same name is attached to the Character.
	 *
	 * @param BodyPart part The BodyPart to check for.
	 * @returns boolean True if the BodyPart is attached.
	 */
	/**
	 * Tells if a BodyPart with the same name is attached to the Character.
	 *
	 * @param string partName The body part to check for.
	 * @returns boolean True if the BodyPart is attached.
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
			return LuaState::expectedArgs(lua, "has_body_part", 2, "BodyPart part", "string partName");
		}
		return LuaState::expectedContext(lua, "has_body_part", "Character");
	}
	/**
	 * Returns a table of all the BodyPart's attached to this Character.
	 * Returns nil if there was an error.
	 * Each table entry is a key/value pair with the key being the BodyPart's name.
	 *
	 * @returns Table A table of all the BodyPart's.
	 */
	int Character_body_parts(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			LuaState L(lua);
			L.newTable();
			const BodyPart::BodyPartMap &map = obj->getBodyParts();
			BodyPart::BodyPartMap::const_iterator iter;
			for (iter = map.begin(); iter != map.end(); ++iter)
			{
				lua_pushstring(lua, iter->first.c_str());
				wrapObject<BodyPart>(lua, iter->second);
				lua_settable(lua, -3);
			}
			return 1;
		}
		return LuaState::expectedContext(lua, "body_parts", "Character");
	}

	/**
	 * Equips an item to the character on the given body part.
	 *
	 * @param Item item The item to equip, it cannot be nil.
	 * @param BodyPart part The body part to equip the item to.
	 * @returns boolean True if the item was successfully equipped,
	 * false can mean that there was already an item equipped, 'part'
	 * was nil, or 'part' was not an attached body part.
	 */
	/**
	 * Equips an item to the character on the given body part.
	 *
	 * @param Item item The item to equip, it cannot be nil.
	 * @param string partName The body part to equip the item to.
	 * @returns boolean True if the item was successfully equipped,
	 * false can mean that there was already an item equipped, 
	 * or 'partName' was not an attached body part.
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
			return LuaState::expectedArgs(lua, "equip_item", 2, "Item item, string partName", "Item item, BodyPart part");
		}
		return LuaState::expectedContext(lua, "equip_item", "Character");
	}
	/**
	 * Unequips the item on the given body part for this character.
	 *
	 * @param BodyPart part The body part to unequip the item from.
	 * @returns boolean True if the item was unequipped or if the body part
	 * was already empty. False can mean 'part' was nil or that 
	 * 'part' was not an attached body part.
	 */
	/**
	 * Unequips the item on the given body part for this character.
	 *
	 * @param string partName The body part to unequip the item from.
	 * @returns boolean True if the item was unequipped or if the body part
	 * was already empty. False can mean that 'partName' was not an attached body part.
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
			return LuaState::expectedArgs(lua, "unequip_item", 2, "BodyPart fromPart", "string fromPartName");
		}
		return LuaState::expectedContext(lua, "unequip_item", "Character");
	}
	/**
	 * Returns the item eqipped at the given body part.
	 *
	 * @param string partName The body part to get the item from.
	 * @returns Item The item equipped, nil if there was no item equipped
	 * or if there was not body part with 'partName' found.
	 */
	int Character_equipped(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			const char *partName = NULL;
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
			return LuaState::expectedArgs(lua, "equipped", 2, "BodyPart part", "string partName");
		}
		return LuaState::expectedContext(lua, "equipped", "Character");
	}
	/**
	 * Returns the inventory object attached to this character.
	 *
	 * @returns Inventory The attached inventory, nil if there was an error.
	 */
	int Character_inventory(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			wrapRefObject<Inventory>(lua, obj->getInventory());
			return 1;
		}
		return LuaState::expectedContext(lua, "inventory", "Character");
	}
	/**
	 * Attempts to pickup an item.
	 * Returns an integer return code:
	 * <ul>
	 * <li>1: Success.</li>
	 * <li>0: Given item was nil.</li>
	 * <li>-1: The item was too far away.</li>
	 * <li>-2: There was not enough space in the characters inventory.</li>
	 * <li>-3: Error.</li>
	 * </ul>
	 *
	 * @param Item item The item to attempt to pickup.
	 * @returns integer Return code for the pickup.
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
			return LuaState::expectedArgs(lua, "pickup_item", "Item item");
		}
		return LuaState::expectedContext(lua, "pickup_item", "Character");
	}
	/**
	 * Adds an item to the characters inventory.
	 * Returns true if it was successful, false there was not enough space.
	 *
	 * @param Item item The item to add.
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
			return LuaState::expectedArgs(lua, "add_item", "Item item");
		}
		return LuaState::expectedContext(lua, "add_item", "Character");
	}
	/**
	 * Removes an item from the characters inventory.
	 * Returns true if the item was removed, false if the item was nil
	 * or if the item could not be found.
	 *
	 * @param Item item The item to remove.
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
			return LuaState::expectedArgs(lua, "remove_item", "Item item");
		}
		return LuaState::expectedContext(lua, "remove_item", "Character");
	}
	/**
	 * Returns true if an item is in the characters inventory.
	 * Returns false if the item was nil or not found.
	 *
	 * @param Item item The item to search for.
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
			return LuaState::expectedArgs(lua, "has_item", "Item item");
		}
		return LuaState::expectedContext(lua, "has_item", "Character");
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
	 * @param Item item The item to drop
	 * @returns integer Drop item return code.
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
	 * @param Item item The item to drop
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
			return LuaState::expectedArgs(lua, "drop_item", 2, "Item item", "Item item, number x, number y");
		}
		return LuaState::expectedContext(lua, "drop_item", "Character");
	}
	/**
	 * Returns the age of the character.
	 *
	 * @returns Number The age of the character
	 */
	/**
	 * Sets the age of the character.
	 *
	 * @param number age The new characters age
	 * @returns Character This.
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
				lua_pushvalue(lua, 1);
				return 1;
			}
			return LuaState::expectedArgs(lua, "age", "number age");
		}
		return LuaState::expectedContext(lua, "age", "Character");
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
	 * @param string raceName The name of the race.
	 * @returns Character This
	 */
	/**
	 * Sets the race of the character.
	 *
	 * @param Race race The race to change to.
	 * @returns Character This
	 */
	/**
	 * Sets the race to the unknown race.
	 *
	 * @param nil race Set the race to the unknown race.
	 * @returns Character This
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
					wrapObject<Race>(lua, obj->getRace());
				}
				else
				{
					wrapObject<Race>(lua, Engine::getEngine()->getUnknownRace());
				}
				return 1;
			}
			else if(lua_isnil(lua, 2))
			{
				obj->setRace(NULL);
				lua_pushvalue(lua, 1);
				return 1;
			}
			else
			{
				Race *race = NULL;
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
					lua_pushvalue(lua, 1);
					return 1;
				}
			}
			return LuaState::expectedArgs(lua, "race", 3, "string raceName", "Race race", "nil");
		}
		return LuaState::expectedContext(lua, "race", "Character");
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
	 * @returns Character This
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
					lua_pushvalue(lua, 1);
					return 1;
				}
				return LuaState::expectedArgs(lua, "gender", "String genderName");
			}
		}
		return LuaState::expectedContext(lua, "gender", "Character");
	}
	/**
	 * Returns the characters coin purse.
	 * This stores all the coin that the character currently holds.
	 *
	 * @returns CoinPurse The characters coin purse.
	 */
	int Character_coin_purse(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			wrapRefObject<CoinPurse>(lua, obj->getCoinPurse());
			return 1;
		}
		return LuaState::expectedContext(lua, "coin_purse", "Character");
	}
	/**
	 * Returns the graphic attached to this character, can be nil.
	 *
	 * @returns am::lua::gfx::Sprite The characters sprite.
	 */
	/**
	 * Sets the characters asset graphic.
	 * Sets the assetName of the graphic for this character.
	 *
	 * @param string assetName The asset name to the graphic.
	 * @returns Character This
	 */
	/**
	 * Sets the characters graphic.
	 *
	 * @param am::lua::gfx::Sprite sprite The graphic to set.
	 * @returns Character This
	 */
	/**
	 * Sets the character graphic to nil.
	 *
	 * @param nil graphic Set the graphic to nil.
	 * @returns Character This
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
					return 1;
				}
			}
			else
			{
				if (lua_isstr(lua, 2))
				{
					obj->setGraphic(new Sprite(lua_tostring(lua, 2)));
				}
				else if (lua_isnil(lua, 2))
				{
					obj->setGraphic(NULL);
				}
				else
				{
					// Can be nil
					obj->setGraphic(castUData<Sprite>(lua, 2));
				}
				if (obj)
				{
					lua_pushvalue(lua, 1);
					return 1;
				}
				return LuaState::expectedArgs(lua, "graphic", 3, "string assetName", "Sprite sprite", "nil");
			}
		}
		return LuaState::expectedContext(lua, "graphic", "Character");
	}
	/**
	 * Returns the characters current map location.
	 * @returns Number The characters x map value
	 * @returns Number The characters y map value
	 */
	/**
	 * Sets the characters map location.
	 * @param number x The x map value
	 * @param number y The y map value
	 * @returns Character This
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
				lua_pushvalue(lua, 1);
				return 1;
			}
			return LuaState::expectedArgs(lua, "location", "number x, number y");
		}
		return LuaState::expectedContext(lua, "location", "Character");
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
	 * @returns Character This
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
				lua_pushvalue(lua, 1);
				return 1;
			}
			return LuaState::expectedArgs(lua, "grid_location", "integer x, integer y");
		}
		return LuaState::expectedContext(lua, "grid_location", "Character");
	}
	/**
	 * Moves a character by {x, y} amount.
	 * If the new location isn't valid, the character isn't moved.
	 * @param number x The x amount to move by
	 * @param number y The y amount to move by
	 * @returns Character This
	 */
	int Character_move(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj && lua_isnum(lua, 2) && lua_isnum(lua, 3))
		{
			obj->move(lua_tofloat(lua, 2), lua_tofloat(lua, 3));
			lua_pushvalue(lua, 1);
			return 1;
		}
		return LuaState::expectedContext(lua, "move", "Character");
	}
	/**
	 * Moves a character by {x, y} grid amounts.
	 * If the new location isn't valid, the character isn't moved.
	 * @param integer x The x grid amount to by move by
	 * @param integer y The y grid amount to by move by
	 * @returns Character This
	 */
	int Character_move_grid(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
			{
				obj->moveGrid(lua_tointeger(lua, 2), lua_tointeger(lua, 3));
				lua_pushvalue(lua, 1);
				return 1;
			}
			return LuaState::expectedArgs(lua, "move_grid", "integer x, integer y");
		}
		return LuaState::expectedContext(lua, "move_grid", "Character");
	}
	/**
	 * Initiates a conversation between this character
	 * and the game object id passed in.
	 * Returns true if the conversation was initiated.
	 * False can mean that the game object id was invalid or that
	 * either this character or the game object did not have a
	 * DialogComponent attached.
	 * @see DialogComponent
	 * @param string id The game object id.
	 * @returns boolean True if the conversation was initiated.
	 */
	/**
	 * Initiates a conversation between this character
	 * and the game object passed in.
	 * Returns true if the conversation was initiated.
	 * False can mean that the game object was nil or that
	 * either this character or the game object did not have a
	 * DialogComponent attached.
	 * @see DialogComponent
	 * @param GameObject talkee The game object to start a converstation with.
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
				return LuaState::expectedArgs(lua, "talk_to", "GameObject talkee");
			}
			LuaState::warning(lua, "Character needs a dialogue component to talk to another game object");
			lua_pushboolean(lua, false);
			return 1;
		}
		return LuaState::expectedContext(lua, "talk_to", "Character");
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
	 * @returns Character This
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
				lua_pushvalue(lua, 1);
				return 1;
			}
			return LuaState::expectedArgs(lua, "fixed_to_grid", "boolean fixed");
		}
		return LuaState::expectedContext(lua, "fixed_to_grid", "Character");
	}

	/**
	 * Returns the current map that this character is on, can be nil.
	 *
	 * @returns Map The map the character is on.
	 */
	/**
	 * Sets the map that the character is on.
	 *
	 * @param Map map The map to put the character on.
	 * @returns Character This
	 */
	/**
	 * Sets the map that the character is on to nil.
	 *
	 * @param nil map Sets the map to nil.
	 * @returns Character This
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
			}
			else if (lua_isnil(lua, 2))
			{
				obj->setMap(NULL);
				lua_pushvalue(lua, 1);
				return 1;
			}
			else
			{
				Map *map = castUData<Map>(lua, 2);
				if (map)
				{
					// Can be set to nil
					obj->setMap(map);
					lua_pushvalue(lua, 1);
					return 1;
				}
			}
			return LuaState::expectedArgs(lua, "map", 2, "nil map", "Map map");
		}
		return LuaState::expectedContext(lua, "map", "Character");
	}
	/**
	 * Adds a tile type to the list of tile types that this character
	 * can move freely on. TileTypes can be added multiple times.
	 * @param TileType tileType The type to add to the list.
	 * @returns Character This
	 */
	int Character_add_passible_type(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			TileType *type = castUData<TileType>(lua, 2);
			if (type)
			{
				obj->addPassibleType(type);
				lua_pushvalue(lua, 1);
				return 1;
			}
			return LuaState::expectedArgs(lua, "add_passible_type", "TileType tileType");
		}
		return LuaState::expectedContext(lua, "add_passible_type", "Character");
	}
	/**
	 * Removes a tile type from the list of passible tiles.
	 * @param TileType tileType The tile type to remove.
	 * @returns Character This
	 */
	int Character_remove_passible_type(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			TileType *type = castUData<TileType>(lua, 2);
			if (type)
			{
				obj->removePassibleType(type);
				lua_pushvalue(lua, 1);
				return 1;
			}
			return LuaState::expectedArgs(lua, "remove_passible_type", "TileType tileType");
		}
		return LuaState::expectedContext(lua, "remove_passible_type", "Character");
	}
	/**
	 * Removes all tile types from this characters passible list.
	 * @returns Character This
	 */
	int Character_remove_all_passible_types(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			obj->removeAllPassibleTypes();
			lua_pushvalue(lua, 1);
			return 1;
		}
		return LuaState::expectedContext(lua, "remove_all_passible_types", "Character");
	}
	/**
	 * Returns true if the given tile type is found on this characters
	 * passible list.
	 * @param TileType tileType The tile type to search for
	 * @returns boolean True if the tile type is found
	 */
	int Character_has_passible_type(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			TileType *type = castUData<TileType>(lua, 2);
			if (type)
			{
				lua_pushboolean(lua, obj->hasPassibleType(type));
				return 1;
			}
			return LuaState::expectedArgs(lua, "has_passible_type", "TileType tileType");
		}
		return LuaState::expectedContext(lua, "has_passible_type", "Character");
	}
	/**
	 * Returns a table of all the passible tile types for this character.
	 * @returns Table Table of all the tile types.
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
		return LuaState::expectedContext(lua, "get_passible_types", "Character");
	}
	/**
	 * @static
	 * Looks for a character with the given game id.
	 * Nil is returned if it is not found.
	 * @param string gameId The game id to look up
	 * @returns Character The found character or nil
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
		return LuaState::expectedArgs(lua, "@find", "string gameId");
	}
	/**
	 * @see DialogueComponent
	 * Returns the dialogue component attached to this character, can be nil.
	 * @returns DialogueComponent The attached dialogue component
	 */
	/**
	 * Sets a dialogue component onto this character, can be nil.
	 * @param DialogueComponent comp The dialogue component to attach to this character, can be nil.
	 * @param boolean [true] setAttached When true it also sets that this character
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
			else
			{
				DialogueComponent *comp = castUData<DialogueComponent>(lua, 2);
				if (lua_isboolean(lua, 3))
				{
					obj->setDialogueComp(comp, lua_tobool(lua, 3));
				}
				else
				{
					obj->setDialogueComp(comp);
				}
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		return LuaState::expectedContext(lua, "dialogue_component", "Character");
	}

	/**
	 * Returns the game id for this character.
	 * @returns string The characters game id.
	 */
	/**
	 * Sets the characters game id, this is used to refer to this
	 * character from the game engine. Should be unique.
	 * @param string gameId The characters unique game id.
	 * @returns boolean True if the game id change was successful or if
	 * game id was the same as the given gameId. False indicates that
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
			else if (lua_isstring(lua, -1))
			{
				lua_pushboolean(lua, obj->setGameId(lua_tostring(lua, -1)));
				return 1;
			}
		}
		return LuaState::expectedContext(lua, "id", "Character");
	}
	
	/**
	 * Adds an event listener for an event fired on this character.
	 * eg: <pre>
	 * character:on("talkTo", function(event)
	 *     am_log("Character talked to")
	 * end)
	 * </pre>
	 * @param string eventType The event type or name to trigger on
	 * @param Function listener The function to call when the event is fired.
	 * @param Table [nil] content An option context for the listener to be
	 * called with.
	 * @returns boolean True if the event was added successfully.
	 */
	int Character_add_event_listener(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj && lua_isstring(lua, 2) && lua_isfunction(lua, 3))
		{
			lua_pushboolean(lua, am::lua::ui::addEventListener(lua, obj));
			return 1;
		}
		return LuaState::expectedContext(lua, "on", "Character");
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
	 * @param string eventType The event type the listener was listening for.
	 * @param Function listener The listener function to remove.
	 * @param Table [nil] context The context which the listener was going to 
	 * be called with, this is only optional if the listener was added with no context.
	 * @returns boolean True if the event listener was successfully removed.
	 */
	int Character_remove_event_listener(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj && lua_isstring(lua, 2) && lua_isfunction(lua, 3))
		{
			lua_pushboolean(lua, am::lua::ui::removeEventListener(lua, obj));
			return 1;
		}
		return LuaState::expectedContext(lua, "off", "Character");
	}
	/**
	 * Returns true when there is an event listener for the given eventType.
	 * @param string eventType The event type to look up.
	 * @returns boolean True if there is any event listener 
	 * that will be trigged by this event type.
	 */
	int Character_has_event_listener(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj && lua_isstring(lua, -1))
		{
			lua_pushboolean(lua, obj->hasEventListener(lua_tostring(lua, -1)));
			return 1;
		}
		return LuaState::expectedContext(lua, "has_event_listener", "Character");
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
	 * @returns Character This
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
			else if (lua_isnumber(lua, -1))
			{
				obj->setExperience(lua_tointeger(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		return LuaState::expectedContext(lua, "experience", "Character");
	}
	/**
	 * Adds the given amount of experience to the total amount of 
	 * experience this character has. The total will always be capped
	 * at the characters max level if they have one.
	 * @param integer experience The amount of experience to add.
	 * @returns Character This
	 */
	int Character_add_experience(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isnum(lua, 2))
			{
				obj->addExperience(lua_tointeger(lua, 2));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		return LuaState::expectedContext(lua, "add_experience", "Character");
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
	 * @returns Character This
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
			else if (lua_isnumber(lua, -1))
			{
				obj->setLevel(static_cast<short>(lua_tointeger(lua, -1)));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		return LuaState::expectedContext(lua, "level", "Character");
	}
	/**
	 * Adds the given amount of levels to this character.
	 * This new level will not go over a characters max level if they have one set.
	 * The characters experience will also be set the appropriate amount
	 * for them to be this level.
	 * @param integer levels The number of levels to add to this character
	 * @returns Character This
	 */
	int Character_add_level(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj && lua_isnumber(lua, -1))
		{
			obj->addLevel(static_cast<short>(lua_tointeger(lua, -1)));
			lua_pushvalue(lua, 1);
			return 1;
		}
		return LuaState::expectedContext(lua, "add_level", "Character");
	}

	/**
	 * Returns this characters maximum level that they can achieve.
	 * If max level is set to 0, their effective max level will be 2^15 (32768).
	 * @returns integer The characters current max level.
	 */
	/**
	 * Sets the characters max level. If the max level is set to 0, then their
	 * effective max level will be 2^15 (32768).
	 * @param integer maxLevel The characters max level.
	 * @returns Character This
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
			else if (lua_isnumber(lua, -1))
			{
				obj->setMaxLevel(static_cast<short>(lua_tointeger(lua, -1)));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		return LuaState::expectedContext(lua, "max_level", "Character");
	}
	
}
}
}
