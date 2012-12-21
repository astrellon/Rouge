#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

#include <game/dialogue_component.h>
using namespace am::game;

namespace am {
namespace lua {
namespace game {

	const char DialogueComponent_tableName[] = "am_game_DialogueComponent";

	int DialogueComponent_ctor(lua_State *lua);
	void DialogueComponent_wrap(lua_State *lua, am::game::DialogueComponent *DialogueComponent);
	int DialogueComponent_dtor(lua_State *lua);
	int DialogueComponent_eq(lua_State *lua);
	int DialogueComponent_register(lua_State *lua);

	int DialogueComponent_talk_to(lua_State *lua);
	int DialogueComponent_get_talking_to(lua_State *lua);

	int DialogueComponent_set_start_dialogue(lua_State *lua);
	int DialogueComponent_get_start_dialogue(lua_State *lua);

	int DialogueComponent_set_subject_lock(lua_State *lua);
	int DialogueComponent_is_subject_locked(lua_State *lua);

	int DialogueComponent_set_dialogue_available(lua_State *lua);
	int DialogueComponent_is_dialogue_available(lua_State *lua);

	int DialogueComponent_set_attached_to(lua_State *lua);
	int DialogueComponent_get_attached_to(lua_State *lua);

	am::game::DialogueComponent *Check_DialogueComponent(lua_State *lua, int n);

}
}
}
