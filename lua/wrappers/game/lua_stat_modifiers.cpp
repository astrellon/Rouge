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

#include <sstream>
using namespace std;

#include "lua_stats.h"

namespace am {
namespace lua {
namespace game {

	int StatModifiers_ctor(lua_State *lua)
	{
		StatModifiers *stats = new StatModifiers();
		
		StatModifiers_wrap(lua, stats);
		return 1;
	}
	void StatModifiers_wrap(lua_State *lua, StatModifiers *stats)
	{
		StatModifiers ** udata = (StatModifiers **)lua_newuserdata(lua, sizeof(StatModifiers *));
		*udata = stats;

		luaL_getmetatable(lua, StatModifiers_tableName);
		lua_setmetatable(lua, -2);
	}

	int StatModifiers_dtor(lua_State *lua)
	{
		StatModifiers *stats = Check_StatModifiers(lua, 1);
		if (stats)
		{
			delete stats;
		}
		return 0;
	}

	int StatModifiers_eq(lua_State *lua)
	{
		StatModifiers *lhs = Check_StatModifiers(lua, 1);
		StatModifiers *rhs = Check_StatModifiers(lua, 2);
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
			{ "add_modifier", StatModifiers_add_modifier },
			{ "remove_modifier", StatModifiers_remove_modifier },
			{ "add_modifiers", StatModifiers_add_modifiers },
			{ "remove_modifiers", StatModifiers_remove_modifiers },
			{ "calculate_stat", StatModifiers_calculate_stat },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, StatModifiers_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	StatModifiers *Check_StatModifiers(lua_State *lua, int n)
	{
		return *(StatModifiers **)luaL_checkudata(lua, n, StatModifiers_tableName);
	}

	int StatModifiers_add_modifier(lua_State *lua)
	{
		StatModifiers *stats = Check_StatModifiers(lua, 1);
		if (stats)
		{
			int args = lua_gettop(lua);
			if (args == 5)
			{
				bool magical = lua_toboolean(lua, -1) > 0;
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
	int StatModifiers_remove_modifier(lua_State *lua)
	{
		StatModifiers *stats = Check_StatModifiers(lua, 1);
		if (stats)
		{
			int args = lua_gettop(lua);
			if (args == 5)
			{
				bool magical = lua_toboolean(lua, -1) > 0;
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

	int StatModifiers_add_modifiers(lua_State *lua)
	{
		StatModifiers *stats = Check_StatModifiers(lua, 1);
		StatModifiers *other = Check_StatModifiers(lua, 2);
		if (stats && other)
		{
			stats->addModifiers(*other);
		}
		return 0;
	}
	int StatModifiers_remove_modifiers(lua_State *lua)
	{
		StatModifiers *stats = Check_StatModifiers(lua, 1);
		StatModifiers *other = Check_StatModifiers(lua, 2);
		if (stats && other)
		{
			stats->removeModifiers(*other);
		}
		return 0;
	}

	int StatModifiers_calculate_stat(lua_State *lua)
	{
		StatModifiers *stats = Check_StatModifiers(lua, 1);
		if (stats)
		{
			Stat::StatType type = getStat(lua, -2);
			if (type != Stat::MAX_STAT_LENGTH)
			{
				if (lua_isnumber(lua, -1))
				{
					float value = static_cast<float>(lua_tonumber(lua, -1));
					lua_pushnumber(lua, stats->calculateStat(type, value));
					return 1;
				}
			}
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
