#include "lua_stat_modifier.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/stat_modifier.h>
using namespace am::game;

#include <sstream>
using namespace std;

#include "lua_stats.h"

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * The stat modifier object represents a single single stat modifier. Such as + 5 or + 10% or = 4.5.
	 * The modifier is not linked to a stat until it is placed into a stat modifiers collection.
	 * @see am.stat_modifiers
	 */
	/**
	 * Creates a new empty stat modifier object.
	 */
	/**
	 * Creates a new stat modifier object with the given parameters.
	 * The value of this modifier is used in the context of the type of modification.
	 * <br>This is the list of each modifier type:
	 * <ul>
	 * <li>= : Sets the stat to the given value. This mod is applied first.</li>
	 * <li>* : Multiplies the stat by the given value. This mod is applied second.</li>
	 * <li>+ : Adds the given value directly onto the stat, this value can be negative. This mod is applied third.</li>
	 * </ul>
	 *
	 * @param number value The value of this modifier.
	 * @param string type The stat modifier type.
	 * @param boolean [true] magical This currently does not change the behaviour of the modifier, however it does
	 *  allow the UI to show the origin of this mod better.
	 */
	int StatModifier_ctor(lua_State *lua)
	{
		if (lua_gettop(lua) == 0)
		{
			StatModifier *stat = new StatModifier();
		
			wrapObject<StatModifier>(lua, stat);
			return 1;
		}
		else if (lua_isnum(lua, 1) && lua_isstr(lua, 2))
		{
			StatModifierType type = getStatModifier(lua, 2);
			if (type == MOD_MAX_LENGTH)
			{
				lua_pushnil(lua);
				return 1;
			}
			bool magical = true;
			if (lua_isboolean(lua, 3))
			{
				magical = lua_tobool(lua, 3);
			}
			StatModifier *stat = new StatModifier(lua_tofloat(lua, 1), type, magical);
		
			wrapObject<StatModifier>(lua, stat);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@new", 2, "", "number value, string type, boolean [true] magical");
	}
	/**
	 * Deletes the stat modifier object.
	 */
	int StatModifier_dtor(lua_State *lua)
	{
		StatModifier *stat = castUData<StatModifier>(lua, 1);
		if (stat)
		{
			delete stat;
		}
		return 0;
	}
	/**
	 * Compares this stat modifier with the given one.
	 * Unlike many of the other Lua objects, this comparison will check
	 * if the objects are equivalent.
	 * <pre>
	 * mod1 = am.stat_modifier.new(5, "+")
	 * mod2 = am.stat_modifier.new(5, "+")
	 * mod3 = am.stat_modifier.new(5, "*")
	 * am_log("Mod 1 == Mod 2? " .. (mod1 == mod2)) -- Output "Mod 1 == Mod 2? true"
	 * am_log("Mod 1 == Mod 3? " .. (mod1 == mod3)) -- Output "Mod 1 == Mod 3? false"
	 * am_log("Mod 2 == Mod 3? " .. (mod2 == mod2)) -- Output "Mod 2 == Mod 3? false"
	 * </pre>
	 * @param am.stat_modifier modifiers The other stat modifiers object to compare with.
	 * @returns boolean True if they are the same stat modifiers object.
	 */
	int StatModifier_eq(lua_State *lua)
	{
		StatModifier *lhs = castUData<StatModifier>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.stat_modifier");
		}
		StatModifier *rhs = castUData<StatModifier>(lua, 2);
		lua_pushboolean(lua, lhs && rhs && (*lhs == *rhs));
		return 1;
	}

	int StatModifier_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", StatModifier_ctor },
			{ "__gc",  StatModifier_dtor },
			{ "__eq", StatModifier_eq },
			{ "value", StatModifier_value },
			{ "type", StatModifier_type },
			{ "magical", StatModifier_magical },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, StatModifier::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	/**
	 * Returns the value of this stat modifier.
	 *
	 * @returns number The stat modifier value.
	 */
	/**
	 * Sets the stat modifier value.
	 *
	 * @param number value The new value for this modifier.
	 * @returns am.stat_modifier This
	 */
	int StatModifier_value(lua_State *lua)
	{
		StatModifier *mod = castUData<StatModifier>(lua, 1);
		if (mod)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushnumber(lua, mod->getValue());
				return 1;
			}
			else if (lua_isnum(lua, 2))
			{
				mod->setValue(lua_tofloat(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "value", "number value");
		}
		return LuaState::expectedContext(lua, "value", "am.stat_modifier");
	}

	/**
	 * Returns the type of this stat modifier.
	 *
	 * @returns string The stat modifier type.
	 */
	/**
	 * Sets the stat modifier type.
	 * <br>This is the list of each modifier type:
	 * <ul>
	 * <li>= : Sets the stat to the given value. This mod is applied first.</li>
	 * <li>* : Multiplies the stat by the given value. This mod is applied second.</li>
	 * <li>+ : Adds the given value directly onto the stat, this value can be negative. This mod is applied third.</li>
	 * </ul>
	 *
	 * @param string type The new type for this modifier.
	 * @returns am.stat_modifier This
	 */
	int StatModifier_type(lua_State *lua)
	{
		StatModifier *mod = castUData<StatModifier>(lua, 1);
		if (mod)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, StatModifier::getModifierTypeString(mod->getType()));
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				StatModifierType type = getStatModifier(lua, 2);
				if (type != MOD_MAX_LENGTH)
				{
					mod->setType(type);
				}
				else
				{
					stringstream ss;
					ss << "Invalid stat modifier type (";
					LuaState::printTypeValue(lua, 2, ss);
					ss << ')';
					LuaState::warning(lua, ss.str().c_str());
				}
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "type", "string type");
		}
		return LuaState::expectedContext(lua, "type", "am.stat_modifier");
	}

	/**
	 * Returns the magical nature of this stat modifier.
	 *
	 * @returns boolean True if this is a magical mod.
	 */
	/**
	 * Sets if modifier is magical in nature.
	 *
	 * @param boolean magical The new magical flag for this modifier.
	 * @returns am.stat_modifier This
	 */
	int StatModifier_magical(lua_State *lua)
	{
		StatModifier *mod = castUData<StatModifier>(lua, 1);
		if (mod)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushboolean(lua, mod->isMagical());
				return 1;
			}
			else if (lua_isbool(lua, 2))
			{
				mod->setMagical(lua_tobool(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "magical", "boolean magical");
		}
		return LuaState::expectedContext(lua, "magical", "am.stat_modifier");
	}

}
}
}
