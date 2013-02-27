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
using namespace std;

#include <log/logger.h>

#include "lua_stat_modifiers.h"

namespace am {
namespace lua {
namespace game {

	int Stats_ctor(lua_State *lua)
	{
		Stats *stats = new Stats();

		wrapObject<Stats>(lua, stats);
		return 1;
	}
	int Stats_dtor(lua_State *lua)
	{
		Stats *stats = castUData<Stats>(lua, 1);
		if (stats)
		{
			delete stats;
		}
		return 0;
	}

	int Stats_eq(lua_State *lua)
	{
		Stats *lhs = castUData<Stats>(lua, 1);
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
			{ "get_base_stat", Stats_get_base_stat },
			{ "set_base_stat", Stats_set_base_stat },
			{ "get_stat", Stats_get_stat },
			{ "add_modifier", Stats_add_modifier },
			{ "remove_modifier", Stats_remove_modifier },
			{ "add_modifiers", Stats_add_modifiers },
			{ "remove_modifiers", Stats_remove_modifiers },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Stats::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

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
		return stat;
	}
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

	int Stats_get_base_stat(lua_State *lua)
	{
		Stats *stats = castUData<Stats>(lua, 1);
		if (stats)
		{
			Stat::StatType stat = getStat(lua, -1);
			if (stat != Stat::MAX_STAT_LENGTH)
			{
				lua_pushnumber(lua, stats->getBaseStat(stat));
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	int Stats_set_base_stat(lua_State *lua)
	{
		Stats *stats = castUData<Stats>(lua, 1);
		if (stats)
		{
			Stat::StatType stat = getStat(lua, -2);
			if (stat != Stat::MAX_STAT_LENGTH)
			{
				if (lua_isnumber(lua, -1))
				{
					float value = lua_tofloat(lua, -1);
					stats->setBaseStat(stat, value);
				}
			}
		}
		return 0;
	}

	int Stats_get_stat(lua_State *lua)
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
		}
		lua_pushnil(lua);
		return 1;
	}

	int Stats_add_modifier(lua_State *lua)
	{
		Stats *stats = castUData<Stats>(lua, 1);
		if (stats)
		{
			int args = lua_gettop(lua);
			if (args == 5)
			{
				bool magical = lua_tobool(lua, -1);
				StatModifierType type = getStatModifier(lua, -2);
				if (!lua_isnumber(lua, -3)) 
				{
					return 0;
				}
				float value = static_cast<float>(lua_tonumber(lua, -3));
				Stat::StatType stat = getStat(lua, -4);
				if (stat != Stat::MAX_STAT_LENGTH)
				{
					stats->addStatModifier(stat, StatModifier(value, type, magical));
				}
			}
			else if (args == 4)
			{
				StatModifierType type = getStatModifier(lua, -1);
				if (!lua_isnumber(lua, -2))
				{
					return 0;
				}
				float value = static_cast<float>(lua_tonumber(lua, -2));
				Stat::StatType stat = getStat(lua, -3);
				if (stat != Stat::MAX_STAT_LENGTH)
				{
					stats->addStatModifier(stat, StatModifier(value, type));
				}
			}
		}
		return 0;
	}
	int Stats_remove_modifier(lua_State *lua)
	{
		Stats *stats = castUData<Stats>(lua, 1);
		if (stats)
		{
			int args = lua_gettop(lua);
			if (args == 5)
			{
				bool magical = lua_tobool(lua, -1);
				StatModifierType type = getStatModifier(lua, -2);
				if (!lua_isnumber(lua, -3)) 
				{
					return 0;
				}
				float value = static_cast<float>(lua_tonumber(lua, -3));
				Stat::StatType stat = getStat(lua, -4);
				if (stat != Stat::MAX_STAT_LENGTH)
				{
					stats->removeStatModifier(stat, StatModifier(value, type, magical));
				}
			}
			else if (args == 4)
			{
				StatModifierType type = getStatModifier(lua, -1);
				if (!lua_isnumber(lua, -2))
				{
					return 0;
				}
				float value = static_cast<float>(lua_tonumber(lua, -2));
				Stat::StatType stat = getStat(lua, -3);
				if (stat != Stat::MAX_STAT_LENGTH)
				{
					stats->removeStatModifier(stat, StatModifier(value, type));
				}
			}
		}
		return 0;
	}

	int Stats_add_modifiers(lua_State *lua)
	{
		Stats *stats = castUData<Stats>(lua, 1);
		StatModifiers *modifiers = castUData<StatModifiers>(lua, 2);
		if (stats && modifiers)
		{
			stats->addModifiers(*modifiers);
		}
		return 0;
	}
	int Stats_remove_modifiers(lua_State *lua)
	{
		Stats *stats = castUData<Stats>(lua, 1);
		StatModifiers *modifiers = castUData<StatModifiers>(lua, 2);
		if (stats && modifiers)
		{
			stats->removeModifiers(*modifiers);
		}
		return 0;
	}

}
}
}
