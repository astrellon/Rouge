#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

#include <game/stats_common.h>
#include <game/stat_modifier.h>
using namespace am::game;

namespace am {
namespace game {
	class Stats;
}

namespace lua {
namespace game {

	int Stats_ctor(lua_State *lua);
	int Stats_dtor(lua_State *lua);
	int Stats_eq(lua_State *lua);
	int Stats_register(lua_State *lua);

	int Stats_base_stat(lua_State *lua);
	int Stats_stat(lua_State *lua);

	int Stats_add(lua_State *lua);
	int Stats_remove(lua_State *lua);
	int Stats_mods(lua_State *lua);

	Stat::StatType getStat(lua_State *lua, int arg);
	StatModifierType getStatModifier(lua_State *lua, int n);

}
}
}
