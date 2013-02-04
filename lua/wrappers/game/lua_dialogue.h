#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

#include <game/dialogue.h>
using namespace am::game;

namespace am {
namespace lua {
namespace game {

	int Dialogue_ctor(lua_State *lua);
	int Dialogue_dtor(lua_State *lua);
	int Dialogue_eq(lua_State *lua);
	int Dialogue_register(lua_State *lua);

	int Dialogue_text(lua_State *lua);
	int Dialogue_title(lua_State *lua);
	int Dialogue_id(lua_State *lua);
	int Dialogue_subject(lua_State *lua);
	int Dialogue_unlock_flag(lua_State *lua);
	int Dialogue_dialogue_action(lua_State *lua);

	int Dialogue_add_dialogue(lua_State *lua);
	int Dialogue_remove_dialogue(lua_State *lua);
	int Dialogue_remove_all_dialogue(lua_State *lua);
	int Dialogue_get_dialogue(lua_State *lua);

	Dialogue::UnlockFlag getUnlockFlag(lua_State *lua, int n);
	Dialogue::DialogueAction getDialogueAction(lua_State *lua, int n);

}
}
}
