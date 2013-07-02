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
	 * Stores a representation of a BodyPart.
	 */
	/**
	 * Creates a new BodyPart instance with the given name.
	 * This name is used to identify the part when it is attached to a character.
	 * @param string name The body part name.
	 * @param string [UNKNOWN_PART] type The body part type.
	 */
	int BodyPart_ctor(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			const char *partName = lua_tostring(lua, 1);
			BodyPart::BodyPartType type = BodyPart::UNKNOWN_PART;
			if (lua_isstr(lua, 2))
			{
				type = BodyPart::getBodyPartType(lua_tostring(lua, 2));
			}
			BodyPart *part = new BodyPart(partName, type);
		
			wrapObject<BodyPart>(lua, part);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@new", "string partName, string [UNKNOWN_PART] type");
	}
	/**
	 * Deletes the body part.
	 */
	int BodyPart_dtor(lua_State *lua)
	{
		BodyPart *part = castUData<BodyPart>(lua, 1);
		if (part)
		{
			delete part;
			return 0;
		}
		return LuaState::expectedContext(lua, "__gc", "BodyPart");
	}
	/**
	 * Compares if two body parts are the same object.
	 * @param BodyPart rhs The BodyPart to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int BodyPart_eq(lua_State *lua)
	{
		BodyPart *lhs = castUData<BodyPart>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "BodyPart");
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
			{ "item", BodyPart_equipped_item },
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
		lua_pushnil(lua);
		return 1;
	}

	/**
	 * Sets the given item as the equipped item on this body part.
	 * @param Item item The item to equip, can be nil.
	 * @returns BodyPart This.
	 */
	/**
	 * Returns the equipped item on this body part.
	 * @returns Item The equipped item, can be nil.
	 */
	int BodyPart_equipped_item(lua_State *lua)
	{
		BodyPart *part = castUData<BodyPart>(lua, 1);
		if (part)
		{
			// No args
			if (lua_gettop(lua) == 1) 
			{
				Item *item = part->getEqippedItem();
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
		return LuaState::expectedContext(lua, "item", "BodyPart");
	}
}
}
}
