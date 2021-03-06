#include "lua_stats.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/stats.h>
using namespace am::game;

#include <sstream>

#include <log/logger.h>

#include "lua_stat_modifiers.h"

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * The stats class stores all base values for each stat, as well as all the current
	 * modifiers for each stat.
	 */
	/**
	 * Creates a new stats instance.
	 */
	int Stats_ctor(lua_State *lua)
	{
		Stats *stats = new Stats();

		wrapRefObject<Stats>(lua, stats);
		return 1;
	}
	/**
	 * Deletes a stats instance. TODO Check if it should do this.
	 */
	int Stats_dtor(lua_State *lua)
	{
		Stats *stats = castUData<Stats>(lua, 1);
		if (stats)
		{
			stats->release();
		}
		return 0;
	}
	/**
	 * Compares this stats object against another stats object.
	 *
	 * @param am.stats rhs The other stats object to compare with.
	 * @returns boolean True if they are the same stats object.
	 */
	int Stats_eq(lua_State *lua)
	{
		Stats *lhs = castUData<Stats>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.stats");
		}
		Stats *rhs = castUData<Stats>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Stats_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Stats_ctor },
			{ "__gc", Stats_dtor },
			{ "__eq", Stats_eq },
			{ "base_stat", Stats_base_stat },
			{ "stat", Stats_stat },
			{ "add", Stats_add },
			{ "remove", Stats_remove },
			{ "mods", Stats_mods },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, Stats::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	/// Parses the Lua value at the given index and returns the StatType enum
	/// value. Stat::MAX_STAT_LENGTH will be returned if it is not a valid enum value.
	Stat::StatType getStat(lua_State *lua, int n)
	{
		Stat::StatType stat = Stat::MAX_STAT_LENGTH;
		int argType = lua_type(lua, n);
		if (argType == LUA_TNUMBER)
		{
			stat = Stat::getStatType(lua_tointeger(lua, n));
		}
		else if (argType == LUA_TSTRING)
		{
			stat = Stat::getStatType(lua_tostring(lua, n));
		}
		else
		{
			std::stringstream ss;
			ss << "Invalid stat name (";
			LuaState::printTypeValue(lua, n, ss);
			ss << ')';
			LuaState::warning(lua, ss.str().c_str());
		}
		return stat;
	}
	/// Parses the Lua value at the given index and returns the StatModifierType enum value.
	/// MOD_MAX_LENGTH will be returned if it is not a valid stat modifier type.
	StatModifierType getStatModifier(lua_State *lua, int n)
	{
		StatModifierType type = MOD_MAX_LENGTH;
		int argType = lua_type(lua, n);
		if (argType == LUA_TNUMBER)
		{
			type = StatModifier::getModifierType(lua_tointeger(lua, n));
		}
		else if (argType == LUA_TSTRING)
		{
			type = StatModifier::getModifierType(lua_tostring(lua, n));
		}
		return type;
	}

	/**
	 * Returns the base value for the given stat. This is the value of the stat before
	 * any stat modifiers are applied.
	 *
	 * @param string stat_name The name of the stat to get the base value of.
	 * @returns number The base value.
	 */
	/**
	 * Sets the base value of the given stat with the given value.
	 *
	 * @param string stat_name The name of the stat to set the base value of.
	 * @param number base_value The new base value for the stat.
	 * @returns am.stats This
	 */
	int Stats_base_stat(lua_State *lua)
	{
		Stats *stats = castUData<Stats>(lua, 1);
		if (stats)
		{
			if (lua_isstr(lua, 2) || lua_isnum(lua, 2)) {
				Stat::StatType stat = getStat(lua, 2);
				if (stat != Stat::MAX_STAT_LENGTH)
				{
					if (lua_gettop(lua) == 2)
					{
						lua_pushnumber(lua, stats->getBaseStat(stat));
						return 1;
					}
					else if (lua_isnum(lua, 3))
					{
						float value = lua_tofloat(lua, 3);
						stats->setBaseStat(stat, value);
						lua_first(lua);
					}
				}
				std::stringstream ss;
				ss << "Invalid stat name (";
				LuaState::printTypeValue(lua, 2, ss);
				ss << ") for am.stats.base_value";
				LuaState::warning(lua, ss.str().c_str());
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "base_stat", "string stat_name, number base_value");
		}
		return LuaState::expectedContext(lua, "base_stat", "am.stats");
	}

	/**
	 * Returns the calculated stat value for the given stat.
	 * This uses the base value of the stat and any modifiers applied to that stat
	 * to return the value from this function.
	 *
	 * @param string stat_name The name of the stat to get the calculated value of.
	 * @returns number The calculated stat value.
	 */
	int Stats_stat(lua_State *lua)
	{
		Stats *stats = castUData<Stats>(lua, 1);
		if (stats)
		{
			Stat::StatType stat = getStat(lua, -1);
			if (stat != Stat::MAX_STAT_LENGTH)
			{
				lua_pushnumber(lua, stats->getStat(stat));
				return 1;
			}
			return LuaState::expectedArgs(lua, "stat", "string stat_name");
		}
		return LuaState::expectedContext(lua, "stat", "am.stats");
	}
	/**
	 * Adds a modifier to the stat modifiers collection. This behaves the same
	 * way as the stat modifiers class add function.
	 * @see am.stat_modifiers
	 *
	 * <pre>
	 * stats = am.stats.new()
	 * stats:add("strength", 6, "+")
	 * stats:mods():add("strength", 4, "*")
	 * 
	 * mods = stats:mods():mods()
	 * am_log("Mod 1: " .. mods["strength"][1].type .. " " .. mods["strength"][1].value) -- Outputs "Mod 1: + 6"
	 * am_log("Mod 2: " .. mods["strength"][2].type .. " " .. mods["strength"][2].value) -- Outputs "Mod 2: * 4"
	 * am_log("Strength: " .. stats:stat("strength", 5)) -- Outputs "Strength: 26"
	 * </pre>
	 *
	 * @param string stat_name The name of the stat to add the modifier to.
	 * @param number value The value of the stat modifier.
	 * @param string type The type of the modifier.
	 * @param Booleam [true] magical If the modifier is magical in nature.
	 * @returns integer Return codes
	 * <ul>
	 * <li>1: The stat modifier was added successfully.</li>
	 * <li>0: The context object was not a stats object.</li>
	 * <li>-1: The given stat name was invalid.</li>
	 * <li>-2: The given value was not a number.</li>
	 * <li>-3: The given stat modifier was invalid.</li>
	 * </ul>
	 */
	/**
	 * Merges another stat modifiers collection into the stat modifiers collection of this stats object.
	 * This behaves as if all the modifiers were taken from the given collection
	 * and added through the other add modifier function.
	 * @see am.stat_modifiers
	 *
	 * @param am.stat_modifiers mods The collection of stat modifiers to combine with this one.
	 * @returns integer Return code
	 * <ul>
	 * <li>1: The stat modifiers were successfully merged.</li>
	 * <li>0: The context object was not a stats object.</li>
	 * <li>-1: The given stat modifiers collection was not a stat modifier instance.</li>
	 * </ul>
	 */
	/**
	 * Adds a stat modifier instance to this stats collection of modifiers.
	 * @see am.stat_modifiers
	 *
	 * @param string stat_name The name of the stat this modifier will affect.
	 * @param am.stat_modifier mod The stat modifier to add.
	 * @returns integer Return code for success.
	 * <ul>
	 * <li>1: The stat modifier was added successfully.</li>
	 * <li>0: The context object was not a stats object.</li>
	 * <li>-1: The given stat name was invalid.</li>
	 * <li>-4: The given stat modifier was not a valid stat modifier instance.</li>
	 * </ul>
	 */
	int Stats_add(lua_State *lua)
	{
		Stats *stats = castUData<Stats>(lua, 1);
		if (stats)
		{
			lua_pushinteger(lua, addToStatModifier(lua, stats));
			return 1;
		}
		return LuaState::expectedContext(lua, "add", "am.stats");
	}
	/**
	 * Removes a modifier from the stat modifiers collection. This behaves the same
	 * way as the stat modifiers class remove function.
	 * @see am.stat_modifiers
	 *
	 * <pre>
	 * stats = am.stats.new()
	 * stats:add("strength", 6, "+")
	 * stats:mods():add("strength", 4, "*")
	 * 
	 * mods = stats:mods():mods()
	 * am_log("Mod 1: " .. mods["strength"][1].type .. " " .. mods["strength"][1].value) -- Outputs "Mod 1: + 6"
	 * am_log("Mod 2: " .. mods["strength"][2].type .. " " .. mods["strength"][2].value) -- Outputs "Mod 2: * 4"
	 * am_log("Strength: " .. stats:stat("strength", 5)) -- Outputs "Strength: 26"
	 * 
	 * stats:remove("strength", 6, "+")
	 * mods = stats:mods()
	 * am_log("Mod 1: " .. mods["strength"][1].type .. " " .. mods["strength"][1].value) -- Outputs "Mod 1: * 4"
	 * am_log("Strength: " .. stats:stat("strength", 5)) -- Outputs "Strength: 20"
	 * </pre>
	 * 
	 * @param string stat_name The name of the stat to remove this modifier from.
	 * @param number value The stat modifier value.
	 * @param string modifier_type The modifier type name.
	 * @param boolean [true] magical If the added modifier was magical, it has be removed as magical.
	 * @returns integer Return codes
	 * <ul>
	 * <li>1: The stat modifier was removed.</li>
	 * <li>0: The context object was not a stats object.</li>
	 * <li>-1: The given stat name was invalid.</li>
	 * <li>-2: The given value was not a number.</li>
	 * <li>-3: The given stat modifier was invalid.</li>
	 * </ul>
	 */
	/**
	 * Removes all the stat modifiers from the given collection from this stats modifiers collection.
	 * This behaves as if all the modifiers were taken from the given collection
	 * and removed through the other remove modifier function.
	 * @see am.stat_modifiers
	 *
	 * @param am.stat_modifiers mods The collection of stat modifiers to remove.
	 * @returns integer Return codes
	 * <ul>
	 * <li>1: The stat modifiers were successfully removed.</li>
	 * <li>0: The context object was not a stats object.</li>
	 * <li>-1: The given stat modifiers collection was not a stat modifier instance.</li>
	 * </ul>
	 */
	/**
	 * Removes a stat modifier from this stats modifiers collection.
	 * @see am.stat_modifiers
	 *
	 * @param string stat_name The name of the stat to remove this modifier from.
	 * @param am.stat_modifier mod The modifier to remove.
	 * @returns integer Return codes
	 * <ul>
	 * <li>1: The stat modifier was removed.</li>
	 * <li>0: The context object was not a stats object.</li>
	 * <li>-1: The given stat name was invalid.</li>
	 * <li>-4: The given stat modifier was not a valid stat modifier instance.</li>
	 * </ul>
	 */
	int Stats_remove(lua_State *lua)
	{
		Stats *stats = castUData<Stats>(lua, 1);
		if (stats)
		{
			lua_pushinteger(lua, removeFromStatModifier(lua, stats));
			return 1;
		}
		return LuaState::expectedContext(lua, "remove", "am.stats");
	}

	/**
	 * Returns the internal stat modifiers object.
	 *
	 * @returns am.stat_modifiers The internal stat modifiers object.
	 */
	int Stats_mods(lua_State *lua)
	{
		Stats *stats = castUData<Stats>(lua, 1);
		if (stats)
		{
			wrapRefObject<StatModifiers>(lua, stats->getStatModifiers());
			return 1;
		}
		return LuaState::expectedContext(lua, "mods", "am.stats");
	}
}
}
}
