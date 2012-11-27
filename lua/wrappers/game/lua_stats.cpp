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

		Stats_wrap(lua, stats);
		return 1;
	}
	void Stats_wrap(lua_State *lua, Stats *stats)
	{
		Stats ** udata = (Stats **)lua_newuserdata(lua, sizeof(Stats *));
		*udata = stats;

		luaL_getmetatable(lua, Stats_tableName);
		lua_setmetatable(lua, -2);
	}

	int Stats_dtor(lua_State *lua)
	{
		Stats *stats = Check_Stats(lua, 1);
		if (stats)
		{
			delete stats;
		}
		return 0;
	}

	int Stats_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Stats_ctor },
			{ "__gc",  Stats_dtor },
			{ "get_base_stat", Stats_get_base_stat },
			{ "set_base_stat", Stats_set_base_stat },
			{ "get_stat", Stats_get_stat },
			{ "add_modifier", Stats_add_modifier },
			{ "remove_modifier", Stats_remove_modifier },
			{ "add_modifiers", Stats_add_modifiers },
			{ "remove_modifiers", Stats_remove_modifiers },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Stats_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	Stats *Check_Stats(lua_State *lua, int n)
	{
		return *(Stats **)luaL_checkudata(lua, n, Stats_tableName);
	}

	Stat::StatType getStat(lua_State *lua, int n)
	{
		Stat::StatType stat = Stat::MAX_STAT_LENGTH;
		if (lua_isnumber(lua, n))
		{
			stat = Stat::getStatType(lua_tointeger(lua, n));
		}
		else if (lua_isstring(lua, -1))
		{
			stat = Stat::getStatType(lua_tostring(lua, n));
		}
		return stat;
	}
	StatModifierType getStatModifier(lua_State *lua, int n)
	{
		StatModifierType type = MOD_MAX_LENGTH;
		if (lua_isnumber(lua, n))
		{
			type = StatModifier::getModifierType(lua_tointeger(lua, n));
		}
		else if (lua_isstring(lua, -1))
		{
			type = StatModifier::getModifierType(lua_tostring(lua, n));
		}
		return type;
	}

	int Stats_get_base_stat(lua_State *lua)
	{
		Stats *stats = Check_Stats(lua, 1);
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
		Stats *stats = Check_Stats(lua, 1);
		if (stats)
		{
			Stat::StatType stat = getStat(lua, -2);
			if (stat != Stat::MAX_STAT_LENGTH)
			{
				if (lua_isnumber(lua, -1))
				{
					float value = lua_tonumber(lua, -1);
					stats->setBaseStat(stat, value);
				}
			}
		}
		return 0;
	}

	int Stats_get_stat(lua_State *lua)
	{
		Stats *stats = Check_Stats(lua, 1);
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
		Stats *stats = Check_Stats(lua, 1);
		if (stats)
		{
			int args = lua_gettop(lua);
			if (args == 5)
			{
				bool magical = lua_toboolean(lua, -1);
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
		Stats *stats = Check_Stats(lua, 1);
		if (stats)
		{
			int args = lua_gettop(lua);
			if (args == 5)
			{
				bool magical = lua_toboolean(lua, -1);
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
		Stats *stats = Check_Stats(lua, 1);
		StatModifiers *modifiers = Check_StatModifiers(lua, 2);
		if (stats && modifiers)
		{
			stats->addModifiers(*modifiers);
		}
		return 0;
	}
	int Stats_remove_modifiers(lua_State *lua)
	{
		Stats *stats = Check_Stats(lua, 1);
		StatModifiers *modifiers = Check_StatModifiers(lua, 2);
		if (stats && modifiers)
		{
			stats->removeModifiers(*modifiers);
		}
		return 0;
	}

}
}
}
