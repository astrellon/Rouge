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
using namespace am::game;

#include <lua/wrappers/lua_stats.h>
#include <lua/wrappers/lua_body_part.h>
#include <lua/wrappers/lua_item.h>
#include <lua/wrappers/lua_inventory.h>
#include <lua/wrappers/lua_map.h>
#include <lua/wrappers/lua_tile_type.h>
#include <lua/wrappers/lua_event_manager.h>
#include <lua/wrappers/lua_coin_purse.h>

namespace am {
namespace lua {
namespace game {

	int Character_ctor(lua_State *lua)
	{
		const char *id = lua_tostring(lua, -1);
		if (id == NULL)
		{
			lua_pushnil(lua);
			return 1;
		}

		Character *obj = dynamic_cast<Character *>(Character::getByGameId(id));
		if (obj == NULL)
		{
			obj = new Character();
			obj->setGameId(id);
		}
		Character_wrap(lua, obj);
		return 1;
	}
	void Character_wrap(lua_State *lua, Character *character)
	{
		Character ** udata = (Character **)lua_newuserdata(lua, sizeof(Character *));
		*udata = character;

		character->retain();

		luaL_getmetatable(lua, Character_tableName);
		lua_setmetatable(lua, -2);
	}

	int Character_dtor(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			obj->release();
		}
		return 0;
	}

	int Character_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Character_ctor },
			{ "__gc", Character_dtor },
			// Character methods
			{ "get_name", Character_get_name },
			{ "set_name", Character_set_name },
			{ "set_pickup_reach", Character_set_pickup_reach },
			{ "get_pickup_reach", Character_get_pickup_reach },
			{ "get_stats", Character_get_stats },
			{ "add_body_part", Character_add_body_part },
			{ "remove_body_part", Character_remove_body_part },
			{ "has_body_part", Character_has_body_part },
			{ "get_body_parts", Character_get_body_parts },
			{ "equip_item", Character_equip_item },
			{ "unequip_item", Character_unequip_item },
			{ "get_equipped", Character_get_equipped },
			{ "get_inventory", Character_get_inventory },
			{ "pickup_item", Character_pickup_item },
			{ "add_item", Character_add_item },
			{ "remove_item", Character_remove_item },
			{ "has_item", Character_has_item },
			{ "drop_item", Character_drop_item },
			{ "set_age", Character_set_age },
			{ "get_age", Character_get_age },
			{ "set_race", Character_set_race },
			{ "get_race", Character_get_race },
			{ "set_gender", Character_set_gender },
			{ "get_gender", Character_get_gender },
			{ "get_coin_purse", Character_get_coin_purse },
			// GameObject methods
			{ "set_location", Character_set_location },
			{ "get_location", Character_get_location },
			{ "set_grid_location", Character_set_grid_location },
			{ "get_grid_location", Character_get_grid_location },
			{ "move", Character_move },
			{ "move_grid", Character_move_grid },
			{ "talk_to", Character_talk_to },
			{ "set_fixed_to_grid", Character_set_fixed_to_grid },
			{ "is_fixed_to_grid", Character_is_fixed_to_grid },
			{ "set_map", Character_set_map },
			{ "get_map", Character_get_map },
			{ "add_passible_type", Character_add_passible_type },
			{ "remove_passible_type", Character_remove_passible_type },
			{ "remove_all_passible_types", Character_remove_all_passible_types },
			{ "has_passible_type", Character_has_passible_type },
			{ "get_passible_types", Character_get_passible_types },
			{ "set_game_id", Character_set_game_id },
			{ "get_game_id", Character_get_game_id },
			{ "get_by_game_id", Character_get_by_game_id },
			// EventListener methods
			{ "add_event_listener", Character_add_event_listener },
			{ "remove_event_listener", Character_remove_event_listener },
			{ "has_event_listener", Character_has_event_listener },
			// Levelable methods
			{ "set_experience", Character_set_experience },
			{ "add_experience", Character_add_experience },
			{ "get_experience", Character_get_experience },
			{ "set_level", Character_set_level },
			{ "add_level", Character_add_level },
			{ "get_level", Character_get_level },
			{ "set_max_level", Character_set_max_level },
			{ "get_max_level", Character_get_max_level },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Character_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");
		
		return 1;
	}

	Character *Check_Character(lua_State *lua, int n)
	{
		return *(Character **)luaL_checkudata(lua, n, Character_tableName);
	}

	int Character_get_name(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			lua_pushstring(lua, obj->getName().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Character_set_name(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			const char *name = luaL_checkstring(lua, 2);
			if (name) 
			{
				obj->setName(name);
			}
		}
		return 0;
	}

	int Character_set_pickup_reach(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			lua_Number value = luaL_checknumber(lua, 2);
			obj->setPickupReach(static_cast<float>(value));
		}
		return 0;
	}
	int Character_get_pickup_reach(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			lua_pushnumber(lua, obj->getPickupReach());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_get_stats(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			Stats *stats = &obj->getStats();
			Stats_wrap(lua, stats);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_add_body_part(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			BodyPart *part = Check_BodyPart(lua, 2);
			if (part)
			{
				lua_pushboolean(lua, obj->addBodyPart(part));
				return 1;
			}
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Character_remove_body_part(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			BodyPart *part = Check_BodyPart(lua, 2);
			if (part)
			{
				lua_pushboolean(lua, obj->removeBodyPart(part));
				return 1;
			}
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Character_has_body_part(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			BodyPart *part = Check_BodyPart(lua, 2);
			if (part)
			{
				lua_pushboolean(lua, obj->hasBodyPart(part));
				return 1;
			}
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Character_get_body_parts(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			LuaState L(lua);
			L.newTable();
			const BodyPart::BodyPartMap &map = obj->getBodyParts();
			BodyPart::BodyPartMap::const_iterator iter;
			for (iter = map.begin(); iter != map.end(); ++iter)
			{
				lua_pushstring(lua, iter->first.c_str());
				BodyPart_wrap(lua, iter->second);
				lua_settable(lua, -3);
			}
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_equip_item(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		Item *item = Check_Item(lua, 2);
		if (obj && item)
		{
			if (lua_isstring(lua, -1))
			{
				lua_pushboolean(lua, obj->equipItem(item, lua_tostring(lua, -1)));
				return 1;
			}
			else
			{
				BodyPart *part = Check_BodyPart(lua, 3);
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
	int Character_unequip_item(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			if (lua_isstring(lua, -1))
			{
				lua_pushboolean(lua, obj->unequipItem(lua_tostring(lua, -1)));
				return 1;
			}
			else
			{
				BodyPart *part = Check_BodyPart(lua, 3);
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
	int Character_get_equipped(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isstring(lua, -1))
		{
			Item_wrap(lua, obj->getEquipped(lua_tostring(lua, -1)));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_get_inventory(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			Inventory_wrap(lua, obj->getInventory());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_pickup_item(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		Item *item = Check_Item(lua, 2);
		if (obj && item)
		{
			lua_pushboolean(lua, obj->pickupItem(item));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Character_add_item(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		Item *item = Check_Item(lua, 2);
		if (obj && item)
		{
			lua_pushboolean(lua, obj->addItem(item));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Character_remove_item(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		Item *item = Check_Item(lua, 2);
		if (obj && item)
		{
			lua_pushboolean(lua, obj->removeItem(item));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Character_has_item(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		Item *item = Check_Item(lua, 2);
		if (obj && item)
		{
			lua_pushboolean(lua, obj->hasItem(item));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Character_drop_item(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		Item *item = Check_Item(lua, 2);
		if (obj && item && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			lua_pushboolean(lua, obj->dropItem(item, lua_tonumber(lua, -2), lua_tonumber(lua, -1)));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}

	int Character_set_age(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			lua_Number value = luaL_checknumber(lua, 2);
			obj->setAge(static_cast<float>(value));
		}
		return 0;
	}
	int Character_get_age(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			lua_pushnumber(lua, obj->getAge());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_set_race(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isstring(lua, -1))
		{
			Race *race = Race::getRace(lua_tostring(lua, -1));
			if (race)
			{
				obj->setRace(race);
			}
		}
		return 0;
	}
	int Character_get_race(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			Race *race = obj->getRace();
			if (race)
			{
				lua_pushstring(lua, obj->getRace()->getRaceName());
			}
			else
			{
				lua_pushstring(lua, Race::getUnknownRace()->getRaceName());
			}
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_set_gender(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
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
		}
		return 0;
	}
	int Character_get_gender(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			lua_pushstring(lua, Gender::getGenderName(obj->getGender()));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_get_coin_purse(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			CoinPurse_wrap(lua, obj->getCoinPurse());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_set_location(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			obj->setLocation(lua_tonumber(lua, -2), lua_tonumber(lua, -1));
		}
		return 0;
	}
	int Character_get_location(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			lua_pushnumber(lua, obj->getLocationX());
			lua_pushnumber(lua, obj->getLocationY());
			return 2;
		}
		lua_pushnil(lua);
		lua_pushnil(lua);
		return 2;
	}

	int Character_set_grid_location(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			obj->setGridLocation(lua_tointeger(lua, -2), lua_tointeger(lua, -1));
		}
		return 0;
	}
	int Character_get_grid_location(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			lua_pushinteger(lua, obj->getGridLocationX());
			lua_pushinteger(lua, obj->getGridLocationY());
			return 2;
		}
		lua_pushnil(lua);
		lua_pushnil(lua);
		return 2;
	}

	int Character_move(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			obj->move(lua_tonumber(lua, -2), lua_tonumber(lua, -1));
		}
		return 0;
	}
	int Character_move_grid(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			obj->moveGrid(lua_tointeger(lua, -2), lua_tointeger(lua, -1));
		}
		return 0;
	}

	int Character_talk_to(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			GameObject *other = (GameObject *)(lua_touserdata(lua, -1));
			obj->talkTo(other);
		}
		return 0;
	}

	int Character_set_fixed_to_grid(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isboolean(lua, -1))
		{
			obj->setFixedToGrid(lua_toboolean(lua, -1));
		}
		return 1;
	}
	int Character_is_fixed_to_grid(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			lua_pushboolean(lua, obj->isFixedToGrid());
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}

	int Character_set_map(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		Map *map = Check_Map(lua, 2);
		if (obj && map)
		{
			obj->setMap(map);
		}
		return 0;
	}
	int Character_get_map(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			Map *map = obj->getMap();
			if (map)
			{
				Map_wrap(lua, map);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_add_passible_type(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		TileType *type = Check_TileType(lua, 2);
		if (obj && type)
		{
			obj->addPassibleType(type);
		}
		return 0;
	}
	int Character_remove_passible_type(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		TileType *type = Check_TileType(lua, 2);
		if (obj && type)
		{
			obj->removePassibleType(type);
		}
		return 0;
	}
	int Character_remove_all_passible_types(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			obj->removeAllPassibleTypes();
		}
		return 0;
	}
	int Character_has_passible_type(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		TileType *type = Check_TileType(lua, 2);
		if (obj && type)
		{
			lua_pushboolean(lua, obj->hasPassibleType(type));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Character_get_passible_types(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			LuaState L(lua);
			L.newTable();
			const GameObject::PassibleTypeList &list = obj->getPassibleTypes();
			for (int i = 0; i < static_cast<int>(list.size()); i++)
			{
				lua_pushinteger(lua, i);
				TileType_wrap(lua, list[i]);
				lua_settable(lua, -3);
			}
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_get_by_game_id(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			Character *obj = dynamic_cast<Character *>(Character::getByGameId(lua_tostring(lua, -1)));
			if (obj)
			{
				Character_wrap(lua, obj);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_set_game_id(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isstring(lua, -1))
		{
			obj->setGameId(lua_tostring(lua, -1));
		}
		return 0;
	}
	int Character_get_game_id(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			lua_pushstring(lua, obj->getGameId());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_add_event_listener(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isstring(lua, 2) && lua_isfunction(lua, 3))
		{
			lua_pushboolean(lua, am::lua::ui::addEventListener(lua, obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Character_remove_event_listener(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isstring(lua, 2) && lua_isfunction(lua, 3))
		{
			lua_pushboolean(lua, am::lua::ui::removeEventListener(lua, obj));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Character_has_event_listener(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isstring(lua, -1))
		{
			lua_pushboolean(lua, obj->hasEventListener(lua_tostring(lua, -1)));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_set_experience(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isnumber(lua, -1))
		{
			obj->setExperience(lua_tointeger(lua, -1));
		}
		return 0;
	}
	int Character_add_experience(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isnumber(lua, -1))
		{
			obj->addExperience(lua_tointeger(lua, -1));
		}
		return 0;
	}
	int Character_get_experience(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			lua_pushinteger(lua, obj->getExperience());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_set_level(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isnumber(lua, -1))
		{
			obj->setLevel(static_cast<short>(lua_tointeger(lua, -1)));
		}
		return 0;
	}
	int Character_add_level(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isnumber(lua, -1))
		{
			obj->addLevel(static_cast<short>(lua_tointeger(lua, -1)));
		}
		return 0;
	}
	int Character_get_level(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			lua_pushinteger(lua, static_cast<lua_Integer>(obj->getLevel()));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Character_set_max_level(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj && lua_isnumber(lua, -1))
		{
			obj->setMaxLevel(static_cast<short>(lua_tointeger(lua, -1)));
		}
		return 0;
	}
	int Character_get_max_level(lua_State *lua)
	{
		Character *obj = Check_Character(lua, 1);
		if (obj)
		{
			lua_pushinteger(lua, static_cast<lua_Integer>(obj->getMaxLevel()));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
