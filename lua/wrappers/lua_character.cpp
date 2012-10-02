#include "lua_character.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
#include <lua/wrappers/lua_stats.h>
#include <lua/wrappers/lua_body_part.h>
using namespace am::lua;

#include <game/character.h>
using namespace am::game;

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

		luaL_getmetatable(lua, Character_tableName);
		lua_setmetatable(lua, -2);
	}

	int Character_dtor(lua_State *lua)
	{
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
			{ "get_body_parts", NULL },
			{ "equip_item", NULL },
			{ "unequip_item", NULL },
			{ "get_equipped", NULL },
			{ "get_inventory", NULL },
			{ "pickup_item", NULL },
			{ "add_item", NULL },
			{ "remove_item", NULL },
			{ "has_item", NULL },
			{ "drop_item", NULL },
			{ "set_age", Character_set_age },
			{ "get_age", Character_get_age },
			{ "set_race", NULL },
			{ "get_race", NULL },
			{ "set_gender", Character_set_gender },
			{ "get_gender", Character_get_gender },
			// GameObject methods
			{ "set_location", NULL },
			{ "get_location_x", NULL },
			{ "get_location_y", NULL },
			{ "set_grid_location", NULL },
			{ "get_grid_location_x", NULL },
			{ "get_grid_location_y", NULL },
			{ "move", NULL },
			{ "move_grid", NULL },
			{ "talk_to", NULL },
			{ "set_fixed_to_grid", NULL },
			{ "is_fixed_to_grid", NULL },
			{ "set_map", NULL },
			{ "get_map", NULL },
			{ "add_passible_type", NULL },
			{ "remove_passible_type", NULL },
			{ "remove_all_passible_types", NULL },
			{ "has_passible_type", NULL },
			{ "get_passible_types", NULL },
			{ "set_game_id", NULL },
			{ "get_game_id", NULL },
			{ "get_by_game_id", NULL },
			// EventListener methods
			{ "add_event_listener", NULL },
			{ "remove_event_listener", NULL },
			{ "has_event_listener", NULL },
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

}
}
}
