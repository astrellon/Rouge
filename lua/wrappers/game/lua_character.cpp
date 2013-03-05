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
	 * @param String gameId The game id to create the character with or the game id
	 * of an existing character.
	 */
	int Character_ctor(lua_State *lua)
	{
		const char *id = lua_tostring(lua, -1);
		if (id == NULL)
		{
			lua_pushnil(lua);
			return 1;
		}

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
		}
		return 0;
	}
	/**
	 * Compares this character with another character reference.
	 * Will not return true for equivalent characters.
	 * @param Character rhs The other character to compare with.
	 * @returns Boolean True if the characters are the same entity.
	 */
	int Character_eq(lua_State *lua)
	{
		Character *lhs = castUData<Character>(lua, 1);
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
	 * @returns String The name of the character.
	 */
	/**
	 * Sets the name of the character, a non string or nil argument
	 * will set the name to an empty string.
	 * @param String name The name to set the character.
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
			else
			{
				const char *name = luaL_checkstring(lua, 2);
				if (name)
				{
					obj->setName(name);
				}
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	/**
	 * Returns the pickup reach of the character.
	 * The pickup reach is the radius around the character in grid units
	 * in which the character is able to pickup items and interact with objects.
	 * @returns Number The pickup reach radius.
	 */
	/**
	 * Sets the pickup reach radius for the character.
	 * The pickup reach is the radius around the character in grid units
	 * in which the character is able to pickup items and interact with objects.
	 * @param Number radius The pickup reach radius.
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
			else if (lua_isnumber(lua, -1))
			{
				obj->setPickupReach(lua_tofloat(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the Stats container linked with this character.
	 * @see Stats
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
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Attempts to add a body part to the character.
	 * Returns true if this was successful, false if the
	 * 'part' parameter was not a BodyPart, 'part' was nil or if there is
	 * already another BodyPart with the same name attached.
	 * @param BodyPart part The BodyPart to add
	 * @returns Boolean If 'part' was added successfully.
	 */
	int Character_add_body_part(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		BodyPart *part = castUData<BodyPart>(lua, 2);
		if (obj && part)
		{
			lua_pushboolean(lua, obj->addBodyPart(part));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Removes a BodyPart with the same name from the Character.
	 * @param BodyPart part The BodyPart to remove.
	 * @returns Boolean True if the BodyPart was removed.
	 */
	/**
	 * Removes a BodyPart with the same name from the Character.
	 * @param String partName The body part to remove.
	 * @returns Boolean True if the BodyPart was removed.
	 */
	int Character_remove_body_part(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isstring(lua, -1))
			{
				lua_pushboolean(lua, obj->removeBodyPart(lua_tostring(lua, -1)));
				return 1;
			}
			BodyPart *part = castUData<BodyPart>(lua, 2);
			if (part)
			{
				lua_pushboolean(lua, obj->removeBodyPart(part));
				return 1;
			}
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Tells if a BodyPart with the same name is attached to the Character.
	 * @param BodyPart part The BodyPart to check for.
	 * @returns Boolean True if the BodyPart is attached.
	 */
	/**
	 * Tells if a BodyPart with the same name is attached to the Character.
	 * @param String partName The body part to check for.
	 * @returns Boolean True if the BodyPart is attached.
	 */
	int Character_has_body_part(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isstring(lua, -1))
			{
				lua_pushboolean(lua, obj->hasBodyPart(lua_tostring(lua, -1)));
				return 1;
			}
			BodyPart *part = castUData<BodyPart>(lua, -1);
			if (part)
			{
				lua_pushboolean(lua, obj->hasBodyPart(part));
				return 1;
			}
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Returns a table of all the BodyPart's attached to this Character.
	 * Returns nil if there was an error.
	 * Each table entry is a key/value pair with the key being the BodyPart's name.
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
		lua_pushnil(lua);
		return 1;
	}

	/**
	 * Equips an item to the character on the given body part.
	 * @param Item item The item to equip, it cannot be nil.
	 * @param BodyPart part The body part to equip the item to.
	 * @returns Boolean True if the item was successfully equipped,
	 * false can mean that there was already an item equipped, 'part'
	 * was nil, or 'part' was not an attached body part.
	 */
	/**
	 * Equips an item to the character on the given body part.
	 * @param Item item The item to equip, it cannot be nil.
	 * @param String partName The body part to equip the item to.
	 * @returns Boolean True if the item was successfully equipped,
	 * false can mean that there was already an item equipped, 
	 * or 'partName' was not an attached body part.
	 */
	int Character_equip_item(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		Item *item = castUData<Item>(lua, 2);
		if (obj && item)
		{
			if (lua_isstring(lua, -1))
			{
				lua_pushboolean(lua, obj->equipItem(item, lua_tostring(lua, -1)));
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
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Unequips the item on the given body part for this character.
	 * @param BodyPart part The body part to unequip the item from.
	 * @returns Boolean True if the item was unequipped or if the body part
	 * was already empty. False can mean 'part' was nil or that 
	 * 'part' was not an attached body part.
	 */
	/**
	 * Unequips the item on the given body part for this character.
	 * @param String partName The body part to unequip the item from.
	 * @returns Boolean True if the item was unequipped or if the body part
	 * was already empty. False can mean that 'partName' was not an attached body part.
	 */
	int Character_unequip_item(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj)
		{
			if (lua_isstring(lua, -1))
			{
				lua_pushboolean(lua, obj->unequipItem(lua_tostring(lua, -1)));
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
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Returns the item eqipped at the given body part.
	 * @param String partName The body part to get the item from.
	 * @returns Item The item equipped, nil if there was no item equipped
	 * or if there was not body part with 'partName' found.
	 */
	int Character_equipped(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj && lua_isstring(lua, -1))
		{
			Handle<Item> item(obj->getEquipped(lua_tostring(lua, -1)));
			if (item.get())
			{
				wrapRefObject<Item>(lua, item);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the inventory object attached to this character.
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
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Attempts to pickup an item.
	 * Returns an integer return code:
	 *  1: Success
	 *  0: Given item was nil
	 * -1: The item was too far away.
	 * -2: There was not enough space in the characters inventory.
	 * -3: Error
	 * @param Item item The item to attempt to pickup.
	 * @returns Integer Return code for the pickup.
	 */
	int Character_pickup_item(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		Item *item = castUData<Item>(lua, 2);
		if (obj && item)
		{
			lua_pushinteger(lua, obj->pickupItem(item));
			return 1;
		}
		lua_pushinteger(lua, -3);
		return 1;
	}
	/**
	 * Adds an item to the characters inventory.
	 * Returns true if it was successful, false if the item was nil or 
	 * there was not enough space.
	 * @param Item item The item to add.
	 * @returns Boolean True for successful add.
	 */
	int Character_add_item(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		Item *item = castUData<Item>(lua, 2);
		if (obj && item)
		{
			lua_pushboolean(lua, obj->addItem(item));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Removes an item from the characters inventory.
	 * Returns true if the item was removed, false if the item was nil
	 * or if the item could not be found.
	 * @param Item item The item to remove.
	 * @returns Boolean True if removal was successful.
	 */
	int Character_remove_item(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		Item *item = castUData<Item>(lua, 2);
		if (obj && item)
		{
			lua_pushboolean(lua, obj->removeItem(item));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Returns true if an item is in the characters inventory.
	 * Returns false if the item was nil or not found.
	 * @param Item item The item to search for.
	 * @returns Boolean True if found successfully.
	 */
	int Character_has_item(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		Item *item = castUData<Item>(lua, 2);
		if (obj && item)
		{
			lua_pushboolean(lua, obj->hasItem(item));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Drops an item at the characters feet.
	 * Return codes:
	 *  1: Success
	 *  0: Item was nil
	 * -1: Drop location was too far away (should not happen)
	 * -2: Drop location was invalid for item.
	 * -3: Error
	 * @param Item item The item to drop
	 * @returns Integer Drop item return code.
	 */
	/**
	 * Drops an item at the given map coordinates.
	 * Return codes:
	 *  1: Success
	 *  0: Item was nil
	 * -1: Drop location was too far away
	 * -2: Drop location was invalid for item.
	 * -3: Error
	 * @param Item item The item to drop
	 * @param Number x X map location to drop at
	 * @param Number y Y map location to drop at
	 * @returns Integer Drop item return code.
	 */
	int Character_drop_item(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		Item *item = castUData<Item>(lua, 2);
		if (obj)
		{
			if (lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
			{
				lua_pushinteger(lua, obj->dropItem(item, lua_tofloat(lua, -2), lua_tofloat(lua, -1)));
			}
			else
			{
				lua_pushinteger(lua, obj->dropItem(item));
			}
			return 1;
		}
		lua_pushinteger(lua, -3);
		return 1;
	}
	/**
	 * Returns the age of the character.
	 * @returns Number The age of the character
	 */
	/**
	 * Sets the age of the character.
	 * @param Number age The new characters age
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
			else if (lua_isnumber(lua, -1))
			{
				obj->setAge(lua_tofloat(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the characters Race.
	 * @returns Race The characters Race
	 */
	/**
	 * Sets the race of the character.
	 * If the race could not be found in the game engine, 
	 * the characters race is unchanged.
	 * @param String raceName The name of the race.
	 * @returns Character This
	 */
	/**
	 * Sets the race of the character.
	 * If the race is nil the characters race is unchanged.
	 * @param Race race The race to change to.
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
			else
			{
				Race *race = NULL;
				if (lua_isstring(lua, 2))
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
				}
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the gender of the character.
	 * @returns String The characters gender
	 */
	/**
	 * Sets the characters gender.
	 * Currently only supports "male" and "female", if it is not one of these
	 * the characters gender is not changed.
	 * @param String gender The gender to change to.
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
				if (lua_isnumber(lua, -1)) 
				{
					gender = Gender::getGenderInt(lua_tointeger(lua, -1));
				
				}
				else if (lua_isstring(lua, -1))
				{
					gender = Gender::getGenderType(lua_tostring(lua, -1));
				}
				if (gender != Gender::MAX_GENDER_LENGTH) 
				{
					obj->setGender(gender);
				}
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the characters coin purse.
	 * This stores all the coin that the character currently holds.
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
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the graphic attached to this character, can be nil.
	 * @returns am::lua::gfx::Sprite The characters sprite.
	 */
	/**
	 * Sets the characters asset graphic.
	 * Sets the assetName of the graphic for this character.
	 * @param String assetName The asset name to the graphic.
	 * @returns Character This
	 */
	/**
	 * Sets the characters graphic, can be nil.
	 * @param am::lua::gfx::Sprite sprite The graphic to set.
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
				if (lua_isstring(lua, -1))
				{
					obj->setGraphic(new Sprite(lua_tostring(lua, -1)));
				}
				else
				{
					// Can be nil
					obj->setGraphic(castUData<Sprite>(lua, -1));
				}
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the characters current map location.
	 * @returns Number The characters x map value
	 * @returns Number The characters y map value
	 */
	/**
	 * Sets the characters map location.
	 * @param Number x The x map value
	 * @param Number y The y map value
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
			else if (lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
			{
				obj->setLocation(lua_tofloat(lua, -2), lua_tofloat(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the characters currnet map grid location.
	 * @returns Integer x The x grid value
	 * @returns Integer y The y grid value
	 */
	/**
	 * Sets the characters current map grid location.
	 * @param Integer x The x grid value
	 * @param Integer y The y grid value
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
			else if (lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
			{
				obj->setGridLocation(lua_tointeger(lua, -2), lua_tointeger(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Moves a character by {x, y} amount.
	 * If the new location isn't valid, the character isn't moved.
	 * @param Number x The x amount to move by
	 * @param Number y The y amount to move by
	 * @returns Character This
	 */
	int Character_move(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			obj->move(lua_tofloat(lua, -2), lua_tofloat(lua, -1));
			lua_pushvalue(lua, 1);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Moves a character by {x, y} grid amounts.
	 * If the new location isn't valid, the character isn't moved.
	 * @param Integer x The x grid amount to by move by
	 * @param Integer y The y grid amount to by move by
	 * @returns Character This
	 */
	int Character_move_grid(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			obj->moveGrid(lua_tointeger(lua, -2), lua_tointeger(lua, -1));
			lua_pushvalue(lua, 1);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Initiates a conversation between this character
	 * and the game object id passed in.
	 * Returns true if the conversation was initiated.
	 * False can mean that the game object id was invalid or that
	 * either this character or the game object did not have a
	 * DialogComponent attached.
	 * @see DialogComponent
	 * @param String id The game object id.
	 * @returns Boolean True if the conversation was initiated.
	 */
	/**
	 * Initiates a conversation between this character
	 * and the game object passed in.
	 * Returns true if the conversation was initiated.
	 * False can mean that the game object was nil or that
	 * either this character or the game object did not have a
	 * DialogComponent attached.
	 * @see DialogComponent
	 * @param GameObject gameObject The game object to start a converstation with.
	 * @returns Boolean True if the conversation was initiated.
	 */
	int Character_talk_to(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj && obj->getDialogueComp())
		{
			//GameObject *other = (GameObject *)(lua_touserdata(lua, -1));
			GameObject *other = getGameObject(lua, -1);
			if (other->getDialogueComp())
			{
				obj->getDialogueComp()->talkTo(other);
				lua_pushboolean(lua, true);
				return 1;
			}
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	/**
	 * Returns true if this character is fixed to the game grid
	 * or false if the character is able to move freely about the map.
	 * @returns Boolean True if the character is fixed to the grid.
	 */
	/**
	 * Sets if the character is fixed to the grid or not.
	 * If a character is fixed to the grid, they cannot move to positions
	 * other than the center of each grid space.
	 * @param Boolean fixed Sets the fixed to grid value.
	 * @returns Character This
	 */
	int Character_fixed_to_grid(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj && lua_isboolean(lua, -1))
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushboolean(lua, obj->isFixedToGrid());
				return 1;
			}
			else if (lua_isboolean(lua, -1))
			{
				obj->setFixedToGrid(lua_tobool(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	/**
	 * Returns the current map that this character is on, can be nil.
	 * @returns Map The map the character is on.
	 */
	/**
	 * Sets the map that the character is on, can be nil.
	 * @param Map map The map to put the character on.
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
			else
			{
				Map *map = castUData<Map>(lua, 2);
				// Can be set to nil
				obj->setMap(map);
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
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
		TileType *type = castUData<TileType>(lua, 2);
		if (obj && type)
		{
			obj->addPassibleType(type);
		}
		lua_pushvalue(lua, 1);
		return 1;
	}
	/**
	 * Removes a tile type from the list of passible tiles.
	 * @param TileType tileType The tile type to remove.
	 * @returns Character This
	 */
	int Character_remove_passible_type(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		TileType *type = castUData<TileType>(lua, 2);
		if (obj && type)
		{
			obj->removePassibleType(type);
		}
		lua_pushvalue(lua, 1);
		return 1;
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
		}
		lua_pushvalue(lua, 1);
		return 1;
	}
	/**
	 * Returns true if the given tile type is found on this characters
	 * passible list.
	 * @param TileType tileType The tile type to search for
	 * @returns Boolean True if the tile type is found
	 */
	int Character_has_passible_type(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		TileType *type = castUData<TileType>(lua, 2);
		if (obj && type)
		{
			lua_pushboolean(lua, obj->hasPassibleType(type));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
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
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * @static
	 * Looks for a character with the given game id.
	 * Nil is returned if it is not found.
	 * @param String gameId The game id to look up
	 * @returns Character The found character or nil
	 */
	int Character_find(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			Character *obj = dynamic_cast<Character *>(Engine::getEngine()->getGameObject(lua_tostring(lua, -1)));
			if (obj)
			{
				wrapRefObject<Character>(lua, obj);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * @see DialogueComponent
	 * Returns the dialogue component attached to this character, can be nil.
	 * @returns DialogueComponent The attached dialogue component
	 */
	/**
	 * Sets a dialogue component onto this character, can be nil.
	 * @param DialogueComponent comp The dialogue component to attach to this character, can be nil.
	 * @param Boolean [true] setAttached When true it also sets that this character
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
		lua_pushnil(lua);
		return 1;
	}

	/**
	 * Returns the game id for this character.
	 * @returns String The characters game id.
	 */
	/**
	 * Sets the characters game id, this is used to refer to this
	 * character from the game engine. Should be unique.
	 * @param String gameId The characters unique game id.
	 * @returns Boolean True if the game id change was successful or if
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
		lua_pushnil(lua);
		return 1;
	}
	
	/**
	 * Adds an event listener for an event fired on this character.
	 * eg: <pre>
	 * character:on("talkTo", function(event)
	 *     am_log("Character talked to")
	 * end)
	 * </pre>
	 * @param String eventType The event type or name to trigger on
	 * @param Function listener The function to call when the event is fired.
	 * @param Table [nil] content An option context for the listener to be
	 * called with.
	 * @returns Boolean True if the event was added successfully.
	 */
	int Character_add_event_listener(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj && lua_isstring(lua, 2) && lua_isfunction(lua, 3))
		{
			lua_pushboolean(lua, am::lua::ui::addEventListener(lua, obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
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
	 * @param String eventType The event type the listener was listening for.
	 * @param Function listener The listener function to remove.
	 * @param Table [nil] context The context which the listener was going to 
	 * be called with, this is only optional if the listener was added with no context.
	 * @returns Boolean True if the event listener was successfully removed.
	 */
	int Character_remove_event_listener(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj && lua_isstring(lua, 2) && lua_isfunction(lua, 3))
		{
			lua_pushboolean(lua, am::lua::ui::removeEventListener(lua, obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns true when there is an event listener for the given eventType.
	 * @param String eventType The event type to look up.
	 * @returns Boolean True if there is any event listener 
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
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Returns the amount of experience this character has.
	 * @returns Integer The amount of experience this character has.
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
	 * @param Integer experience The amount of experience to set on this character.
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
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Adds the given amount of experience to the total amount of 
	 * experience this character has. The total will always be capped
	 * at the characters max level if they have one.
	 * @param Integer experience The amount of experience to add.
	 * @returns Character This
	 */
	int Character_add_experience(lua_State *lua)
	{
		Character *obj = castUData<Character>(lua, 1);
		if (obj && lua_isnumber(lua, -1))
		{
			obj->addExperience(lua_tointeger(lua, -1));
		}
		return 0;
	}
	/**
	 * Returns the characters current level.
	 * @returns Integer The characters level
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
	 * @param Integer level The characters new level.
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
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Adds the given amount of levels to this character.
	 * This new level will not go over a characters max level if they have one set.
	 * The characters experience will also be set the appropriate amount
	 * for them to be this level.
	 * @param Integer levels The number of levels to add to this character
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
		lua_pushnil(lua);
		return 1;
	}

	/**
	 * Returns this characters maximum level that they can achieve.
	 * If max level is set to 0, their effective max level will be 2^15 (32768).
	 * @returns Integer The characters current max level.
	 */
	/**
	 * Sets the characters max level. If the max level is set to 0, then their
	 * effective max level will be 2^15 (32768).
	 * @param Integer maxLevel The characters max level.
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
		lua_pushnil(lua);
		return 1;
	}
	
}
}
}
