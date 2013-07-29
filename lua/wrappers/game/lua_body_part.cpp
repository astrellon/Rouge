#include "lua_body_part.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/body_part.h>
#include <game/item.h>
using namespace am::game;

#include <lua/wrappers/game/lua_item.h>

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * Stores a representation of a body part.
	 */
	/**
	 * Creates a new body part instance with the given name.
	 * This name is used to identify the part when it is attached to a character.
	 * @param string name The body part name.
	 * @param string [UNKNOWN_PART] type The body part type.
	 */
	int BodyPart_ctor(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			const char *partName = lua_tostring(lua, 1);
			BodyPartType::PartType type = BodyPartType::UNKNOWN_PART;
			if (lua_isstr(lua, 2))
			{
				type = BodyPartType::getBodyPartType(lua_tostring(lua, 2));
			}
			BodyPart *part = new BodyPart(partName, type);
		
			wrapRefObject<BodyPart>(lua, part);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@new", "string partName, string [UNKNOWN_PART] type");
	}
	/**
	 * Releases body part reference counter.
	 */
	int BodyPart_dtor(lua_State *lua)
	{
		BodyPart *part = castUData<BodyPart>(lua, 1);
		if (part)
		{
			part->release();
			return 0;
		}
		return LuaState::expectedContext(lua, "__gc", "am.body_part");
	}
	/**
	 * Compares if two body parts are the same object.
	 * @param am.body_part rhs The body part to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int BodyPart_eq(lua_State *lua)
	{
		BodyPart *lhs = castUData<BodyPart>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.body_part");
		}
		BodyPart *rhs = castUData<BodyPart>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int BodyPart_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", BodyPart_ctor },
			{ "__gc",  BodyPart_dtor },
			{ "__eq", BodyPart_eq },
			{ "name", BodyPart_name },
			{ "type", BodyPart_type },
			{ "can_hold_onto", BodyPart_can_hold_onto },
			{ "weapon_part", BodyPart_weapon_part },
			{ "is_holding_onto", BodyPart_is_holding_onto },
			{ "can_equip_item", BodyPart_can_equip_item },
			{ "item", BodyPart_item },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, BodyPart::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	/**
	 * Returns the name of the body part.
	 * A nil return indicates that there was an error.
	 * @returns string Body part name.
	 */
	int BodyPart_name(lua_State *lua)
	{
		BodyPart *part = castUData<BodyPart>(lua, 1);
		if (part)
		{
			lua_pushstring(lua, part->getName());
			return 1;
		}
		return LuaState::expectedContext(lua, "name", "am.body_part");
	}

	/**
	 * Returns the type of the body part as a string.
	 *
	 * @returns string Body part type.
	 */
	/**
	 * Sets the type of the body part.
	 *
	 * @param string type The type to set the body part to.
	 * @returns am.body_part This
	 * @returns am.return_code Return code.
	 */
	int BodyPart_type(lua_State *lua)
	{
		BodyPart *part = castUData<BodyPart>(lua, 1);
		if (part)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, BodyPartType::getBodyPartName(part->getType()));
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				BodyPartType::PartType type = BodyPartType::getBodyPartType(lua_tostring(lua, 2));
				if (type == BodyPartType::MAX_BODY_TYPE_LENGTH)
				{
					stringstream ss;
					ss << "Unknown body part type '" << lua_tostring(lua, 2) << '\'';
					LuaState::warning(lua, ss.str().c_str());
				}
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "type", "string partType");
		}
		return LuaState::expectedContext(lua, "type", "am.body_part");
	}

	int BodyPart_can_hold_onto(lua_State *lua)
	{
		BodyPart *part = castUData<BodyPart>(lua, 1);
		if (part)
		{
			if (lua_gettop(lua) == 1)
			{
				BodyPart *other = part->getCanHoldOnto();
				if (other)
				{
					wrapRefObject<BodyPart>(lua, other);
					return 1;
				}
				lua_pushnil(lua);
				return 1;
			}
			else
			{
				if (lua_isnil(lua, 2))
				{
					part->setCanHoldOnto(NULL);
					lua_first(lua);
				}
				BodyPart *other = castUData<BodyPart>(lua, 2);
				if (other)
				{
					part->setCanHoldOnto(other);
					lua_first(lua);
				}
			}
			return LuaState::expectedArgs(lua, "can_hold_onto", 2, "BodyPart part, nil part");
		}
		return LuaState::expectedContext(lua, "can_hold_onto", "am.body_part");
	}

	int BodyPart_weapon_part(lua_State *lua)
	{
		BodyPart *part = castUData<BodyPart>(lua, 1);
		if (part)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushboolean(lua, part->isWeaponPart());
				return 1;
			}
			else if (lua_isbool(lua, 2))
			{
				part->setWeaponPart(lua_tobool(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "weapon_part", "bool isWeapon");
		}
		return LuaState::expectedContext(lua, "weapon_part", "am.body_part");
	}

	int BodyPart_is_holding_onto(lua_State *lua)
	{
		BodyPart *part = castUData<BodyPart>(lua, 1);
		if (part)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushboolean(lua, part->isHoldingOnto());
				return 1;
			}
			return LuaState::expectedArgs(lua, "is_holding_onto", "");
		}
		return LuaState::expectedContext(lua, "is_holding_onto", "am.body_part");
	}

	int BodyPart_can_equip_item(lua_State *lua)
	{
		BodyPart *part = castUData<BodyPart>(lua, 1);
		if (part)
		{
			if (lua_gettop(lua) == 1)
			{
				Handle<Item> item(castUData<Item>(lua, 2));
				if (item)
				{
					lua_pushboolean(lua, part->canEquipItem(item));
					return 1;
				}
			}
			return LuaState::expectedArgs(lua, "can_equip_item", "Item item");
		}
		return LuaState::expectedContext(lua, "can_equip_item", "am.body_part");
	}

	/**
	 * Sets the given item as the equipped item on this body part.
	 * @param Item item The item to equip, can be nil.
	 * @returns BodyPart This.
	 */
	/**
	 * Returns the equipped item on this body part.
	 * @returns am.item The equipped item, can be nil.
	 */
	int BodyPart_item(lua_State *lua)
	{
		BodyPart *part = castUData<BodyPart>(lua, 1);
		if (part)
		{
			// No args
			if (lua_gettop(lua) == 1) 
			{
				Item *item = part->getEquippedItem();
				if (item)
				{
					wrapRefObject<Item>(lua, item);
					return 1;
				}
				lua_pushnil(lua);
				return 1;
			}
			// Two or more args
			else
			{
				if (lua_isnil(lua, 2))
				{
					part->setEquippedItem(NULL);
					lua_first(lua);
				}
				Item *item = castUData<Item>(lua, 2);
				if (item)
				{
					part->setEquippedItem(item);
					lua_first(lua);
				}
			}
			return LuaState::expectedArgs(lua, "item", 2, "Item item, nil item");
		}
		return LuaState::expectedContext(lua, "item", "am.body_part");
	}
}
}
}
