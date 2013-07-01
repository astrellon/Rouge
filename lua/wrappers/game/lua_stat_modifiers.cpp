#include "lua_stat_modifiers.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/stat_modifier.h>
#include <game/stat_modifiers.h>
using namespace am::game;

#include <log/logger.h>

#include <sstream>
using namespace std;

#include "lua_stats.h"

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * The stat modifiers object represents a collection of modifiers that can be applied to stats.
	 */
	/**
	 * Creates a new stat modifier object.
	 */
	int StatModifiers_ctor(lua_State *lua)
	{
		StatModifiers *stats = new StatModifiers();
		
		wrapRefObject<StatModifiers>(lua, stats);
		return 1;
	}
	/**
	 * Deletes the stat modifiers object. TODO Check if it should do this.
	 */
	int StatModifiers_dtor(lua_State *lua)
	{
		StatModifiers *stats = castUData<StatModifiers>(lua, 1);
		if (stats)
		{
			return 0;
		}
		return LuaState::expectedContext(lua, "__gc", "StatModifiers");
	}
	/**
	 * Compares this stat modifiers object with the given one.
	 *
	 * @param StatModifiers modifiers The other stat modifiers object to compare with.
	 * @returns boolean True if they are the same stat modifiers object.
	 */
	int StatModifiers_eq(lua_State *lua)
	{
		StatModifiers *lhs = castUData<StatModifiers>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "StatModifiers");
		}
		StatModifiers *rhs = castUData<StatModifiers>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int StatModifiers_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", StatModifiers_ctor },
			{ "__gc",  StatModifiers_dtor },
			{ "__eq", StatModifiers_eq },
			{ "add", StatModifiers_add},
			{ "remove", StatModifiers_remove},
			{ "mods", StatModifiers_mods },
			{ "calculate_stat", StatModifiers_calculate_stat },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, StatModifiers::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	/**
	 * Adds a stat modifier to this collection.
	 * A modifier is made up the stat it modifiers, the type of modification and
	 * a value.<br>
	 * This is a list of valid stat names:
	 * <ul>
	 * <li>health</li><li>maxHealth</li><li>strength</li><li>dexterity</li><li>constitution</li>
	 * <li>arcane</li><li>divine</li><li>minDamage</li><li>maxDamage</li><li>armour</li>
	 * </ul>
	 * <span>Each modifier type has a different affect and is applied in a specific order.
	 * This ensures that a set (=) mod doesn't override other mods, and that a multiply (*) mod doesn't
	 * get an unexpected boost because of an add (+) mod.</span>
	 * <ul>
	 * <li>= : Sets the stat to the given value. This mod is applied first.</li>
	 * <li>* : Multiplies the stat by the given value. This mod is applied second.</li>
	 * <li>+ : Adds the given value directly onto the stat, this value can be negative. This mod is applied third.</li>
	 * </ul>
	 * <span>An example</span>
	 * <pre>
	 * StatModifiers = import("StatModifiers")
	 * mods = StatModifiers.new()
	 * 
	 * base_health = 5
	 * mods:add("health", 4, "+")
	 * am_log("Calculated Health: " .. mods:calculate_stat("health", base_health)) -- Outputs 9 (5 + 4)
	 * 
	 * mods:add("health", 3, "*")
	 * am_log("Calculated Health: " .. mods:calculate_stat("health", base_health)) -- Outputs 19 (5 * 3 + 4)
	 * 
	 * mods:add("health", 7, "=")
	 * am_log("Calculated Health: " .. mods:calculate_stat("health", base_health)) -- Outputs 25 (7 * 3 + 4)
	 * </pre>
	 * @param string statName The name of the stat this modifier will affect.
	 * @param number value The stat modifier value.
	 * @param string modifierType The modifier type name.
	 * @param boolean [true] magical Defines if the modification is magical in-nature or not.
	 *  This does not affect how the final stat value is calculated, but it does allow for the UI
	 *  to categorise how the modification is displayed. This may change in future.
	 * @returns integer Return codes
	 * <ul>
	 * <li>1: The stat modifier was added successfully.</li>
	 * <li>0: The context object was not a stat modifier collection.</li>
	 * <li>-1: The given stat name was invalid.</li>
	 * <li>-2: The given value was not a number.</li>
	 * <li>-3: The given stat modifier was invalid.</li>
	 * </ul>
	 */
	/**
	 * Merges another stat modifiers collection into this one.
	 * This behaves as if all the modifiers were taken from the given collection
	 * and added through the other add modifier function.
	 *
	 * @param StatModifiers mods The collection of stat modifiers to combine with this one.
	 * @returns integer Return codes
	 * <ul>
	 * <li>1: The stat modifiers were successfully merged.</li>
	 * <li>0: The context object was not a stat modifier collection.</li>
	 * <li>-1: The given stat modifiers collection was not a StatModifier instance.</li>
	 * </ul>
	 */
	/**
	 * Adds a StatModifier instance to this collection of modifiers.
	 *
	 * @param string statName The name of the stat this modifier will affect.
	 * @param StatModifier mod The stat modifier to add.
	 * @returns integer Return codes
	 * <ul>
	 * <li>1: The stat modifier was added successfully.</li>
	 * <li>0: The context object was not a stat modifier collection.</li>
	 * <li>-1: The given stat name was invalid.</li>
	 * <li>-4: The given stat modifier was not a valid stat modifier instance.</li>
	 * </ul>
	 */
	int StatModifiers_add(lua_State *lua)
	{
		StatModifiers *stats = castUData<StatModifiers>(lua, 1);
		if (stats)
		{
			lua_pushinteger(lua, addToStatModifier(lua, stats));
			return 1;
		}
		return LuaState::expectedContext(lua, "add", "StatModifiers");
	}
	/**
	 * Removes a stat modifier from the collection.
	 * See the add function for a list of valid stat names and modifier types.
	 * Each modifier does not contain any identification information about it,
	 * as such to remove a modifier you need to specify the same stats which you want to remove.
	 * So if you previous added a + 5 to Health, to remove it you remove a + 5 to Health.
	 * <p><b>Note:</b><span> Internally the values are stored as 32-bit floats, as such two different
	 * modifier values are considered the same if their different is less than 0.00001.</span></p>
	 * @see StatModifiers.add
	 *
	 * <pre>
	 * StatModifiers = import("StatModifiers")
	 * mods = StatModifiers.new()
	 * 
	 * base_health = 5
	 * mods:add("health", 4, "+")
	 * am_log("Calculated Health: " .. mods:calculate_stat("health", base_health)) -- Outputs 9 (5 + 4)
	 * 
	 * mods:add("health", 3, "*")
	 * am_log("Calculated Health: " .. mods:calculate_stat("health", base_health)) -- Outputs 19 (5 * 3 + 4)
	 * 
	 * mods:add("health", 7, "=")
	 * am_log("Calculated Health: " .. mods:calculate_stat("health", base_health)) -- Outputs 25 (7 * 3 + 4)
	 * 
	 * -- Remove them all again but in a different order.
	 * mods:remove("health", 3, "*")
	 * am_log("Calculated Health: " .. mods:calculate_stat("health", base_health)) -- Outputs 11 (7 + 4)
	 * 
	 * mods:remove("health", 4, "+")
	 * am_log("Calculated Health: " .. mods:calculate_stat("health", base_health)) -- Outputs 7 (7)
	 * 
	 * mods:remove("health", 7, "=")
	 * am_log("Calculated Health: " .. mods:calculate_stat("health", base_health)) -- Outputs 5 (5)
	 * </pre>
	 *
	 * @param string statName The name of the stat to remove this modifier from.
	 * @param number value The stat modifier value.
	 * @param string modifierType The modifier type name.
	 * @param boolean [true] magical If the added modifier was magical, it has be removed as magical.
	 * @returns integer Return codes
	 * <ul>
	 * <li>1: The stat modifier was removed.</li>
	 * <li>0: The context object was not a stat modifier collection.</li>
	 * <li>-1: The given stat name was invalid.</li>
	 * <li>-2: The given value was not a number.</li>
	 * <li>-3: The given stat modifier was invalid.</li>
	 * </ul>
	 */
	/**
	 * Removes all the stat modifiers from the given collection from this collection.
	 * This behaves as if all the modifiers were taken from the given collection
	 * and removed through the other remove modifier function.
	 *
	 * @param StatModifiers mods The collection of stat modifiers to remove.
	 * @returns integer Return codes
	 * <ul>
	 * <li>1: The stat modifiers were successfully removed.</li>
	 * <li>0: The context object was not a stat modifier collection.</li>
	 * <li>-1: The given stat modifiers collection was not a StatModifier instance.</li>
	 * </ul>
	 */
	/**
	 * Removes a stat modifier from this collection.
	 *
	 * @param string statName The name of the stat to remove this modifier from.
	 * @param StatModifier mod The modifier to remove.
	 * @returns integer Return codes
	 * <ul>
	 * <li>1: The stat modifier was removed.</li>
	 * <li>0: The context object was not a stat modifier collection.</li>
	 * <li>-1: The given stat name was invalid.</li>
	 * <li>-4: The given stat modifier was not a valid StatModifier instance.</li>
	 * </ul>
	 */
	int StatModifiers_remove(lua_State *lua)
	{
		StatModifiers *stats = castUData<StatModifiers>(lua, 1);
		if (stats)
		{
			lua_pushinteger(lua, removeFromStatModifier(lua, stats));
			return 1;
		}
		return LuaState::expectedContext(lua, "remove", "StatModifiers");
	}

	/**
	 * Returns a map of stat name keys and arrays of all the modifcations values.
	 * This is a copy of the internal mods map, changing values on this table will have no 
	 * affect on the internal values.
	 *
	 * <pre>
	 * StatModifiers, StatModifier = import("StatModifiers", "StatModifier")
	 * statMods = StatModifiers.new()
	 * statMods:add("health", 5, "+", false)
	 * statMods:add("health", 8, "=")
	 * statMods:add("health", 2, "*")
	 * 
	 * mods = statMods:mods()
	 * am_log("Magical: " .. tostring(mods["health"][1].magical)) -- Outputs "Magical: false"
	 * am_log("Type: " .. mods["health"][2].type)                 -- Outputs "Type: ="
	 * am_log("Value: " .. mods["health"][1].value)               -- Outputs "Value: 2"
	 * </pre>
	 *
	 * @returns table All the modifications as a Lua table.
	 */
	int StatModifiers_mods(lua_State *lua)
	{
		StatModifiers *stats = castUData<StatModifiers>(lua, 1);
		if (stats)
		{
			LuaState L(lua);
			// Table to hold a map of stat name keys and arrays of stat mod values.
			L.newTable();
			const StatModifiers::StatModifierMap &mods = stats->getModifiers();
			for (auto mapIter = mods.begin(); mapIter != mods.end(); ++mapIter)
			{
				const char *statName = Stat::getStatName(mapIter->first);
				lua_pushstring(lua, statName);
				L.newTable();
				int i = 1;
				for (auto iter = mapIter->second.begin(); iter != mapIter->second.end(); ++iter)
				{
					L.newTable();
					L.setTableValue("value", iter->getValue());
					L.setTableValue("type", StatModifier::getModifierTypeString(iter->getType()));
					L.setTableValue("magical", iter->isMagical());
					lua_rawseti(lua, -2, i++);
				}
				lua_settable(lua, -3);
			}
			return 1;
		}
		return LuaState::expectedContext(lua, "mods", "StatModifiers");
	}
	/**
	 * Calculates the final value for a given stat with the given base value.
	 * Stats with no modifications will simply return the base value.
	 *
	 * @param string statName The stat name, nil will be returned if this is invalid.
	 * @param number baseValue The base value for the stat.
	 * @returns Number The calculated stat value.
	 */
	int StatModifiers_calculate_stat(lua_State *lua)
	{
		StatModifiers *stats = castUData<StatModifiers>(lua, 1);
		if (stats)
		{
			if (lua_isstr(lua, 2))
			{
				Stat::StatType type = getStat(lua, 2);
				if (type != Stat::MAX_STAT_LENGTH)
				{
					if (lua_isnum(lua, 3))
					{
						float value = static_cast<float>(lua_tonumber(lua, 3));
						lua_pushnumber(lua, stats->calculateStat(type, value));
						return 1;
					}
				}
			}
			return LuaState::expectedArgs(lua, "calculate_stat", "string statName, number baseValue");
		}
		return LuaState::expectedContext(lua, "calculate_stat", "StatModifiers");
	}

	int addToStatModifier(lua_State *lua, am::game::IStatModifiers *stats)
	{
		if (!lua)
		{
			return 0;
		}
		if (!stats)
		{
			return -1;
		}
		int args = lua_gettop(lua);
		if (args == 2)
		{
			StatModifiers *other = castUData<StatModifiers>(lua, 2);
			if (other)
			{
				stats->addModifiers(*other);
				return 1;
			}
			return -1;
		}

		Stat::StatType stat = getStat(lua, 2);
		if (stat == Stat::MAX_STAT_LENGTH)
		{
			return -1;
		}

		if (args == 3)
		{
			StatModifier *mod = castUData<StatModifier>(lua, 3);
			if (mod)
			{
				stats->addStatModifier(stat, *mod);
				return 1;
			}
			return -4;
		}

		if (!lua_isnum(lua, 3)) 
		{
			return -2;
		}
		float value = static_cast<float>(lua_tonumber(lua, 3));
		StatModifierType type = getStatModifier(lua, 4);
		if (type == MOD_MAX_LENGTH)
		{
			return -3;
		}
			
		bool magical = args >= 5 ? lua_tobool(lua, 5) : true;
			
		stats->addStatModifier(stat, StatModifier(value, type, magical));
		return 1;
	}
	int removeFromStatModifier(lua_State *lua, am::game::IStatModifiers *stats)
	{
		if (!lua)
		{
			return 0;
		}
		if (!stats)
		{
			return -1;
		}
		int args = lua_gettop(lua);
		if (args == 2)
		{
			StatModifiers *other = castUData<StatModifiers>(lua, 2);
			if (other)
			{
				stats->removeModifiers(*other);
				return 1;
			}
			return -1;
		}

		Stat::StatType stat = getStat(lua, 2);
		if (stat == Stat::MAX_STAT_LENGTH)
		{
			return -1;
		}

		if (args == 3)
		{
			StatModifier *mod = castUData<StatModifier>(lua, 3);
			if (mod)
			{
				stats->removeStatModifier(stat, *mod);
				return 1;
			}
			return -4;
		}

		if (!lua_isnum(lua, 3)) 
		{
			return -2;
		}
		float value = static_cast<float>(lua_tonumber(lua, 3));
		StatModifierType type = getStatModifier(lua, 4);
			
		bool magical = args >= 5 ? lua_tobool(lua, 5) : true;
			
		stats->removeStatModifier(stat, StatModifier(value, type, magical));
		return 1;
	}

}
}
}
