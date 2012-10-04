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

	const char Stats_tableName[] = "am_game_Stats";

	int Stats_ctor(lua_State *lua);
	void Stats_wrap(lua_State *lua, am::game::Stats *stats);

	int Stats_dtor(lua_State *lua);
	int Stats_register(lua_State *lua);

	int Stats_get_base_stat(lua_State *lua);
	int Stats_set_base_stat(lua_State *lua);

	int Stats_get_stat(lua_State *lua);

	int Stats_add_modifier(lua_State *lua);
	int Stats_remove_modifier(lua_State *lua);

	int Stats_add_modifiers(lua_State *lua);
	int Stats_remove_modifiers(lua_State *lua);

	Stat::StatType getStat(lua_State *lua, int arg);
	StatModifierType getStatModifier(lua_State *lua, int n);

	am::game::Stats *Check_Stats(lua_State *lua, int n);

}
}
}
