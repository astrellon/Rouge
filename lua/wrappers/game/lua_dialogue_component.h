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

	int DialogueComponent_ctor(lua_State *lua);
	int DialogueComponent_dtor(lua_State *lua);
	int DialogueComponent_eq(lua_State *lua);
	int DialogueComponent_register(lua_State *lua);

	int DialogueComponent_talk_to(lua_State *lua);
	int DialogueComponent_talking_to(lua_State *lua);

	int DialogueComponent_start_dialogue(lua_State *lua);
	int DialogueComponent_subject_lock(lua_State *lua);
	int DialogueComponent_dialogue_available(lua_State *lua);

	int DialogueComponent_attached_to(lua_State *lua);

    int DialogueComponent_add_event_listener(lua_State *lua);
	int DialogueComponent_remove_event_listener(lua_State *lua);
	int DialogueComponent_has_event_listener(lua_State *lua);

}
}
}
