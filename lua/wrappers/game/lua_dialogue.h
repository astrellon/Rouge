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

	const char Dialogue_tableName[] = "am_game_Dialogue";

	int Dialogue_ctor(lua_State *lua);
	void Dialogue_wrap(lua_State *lua, am::game::Dialogue *dialogue);
	int Dialogue_dtor(lua_State *lua);
	int Dialogue_eq(lua_State *lua);
	int Dialogue_register(lua_State *lua);

	int Dialogue_set_text(lua_State *lua);
	int Dialogue_get_text(lua_State *lua);

	int Dialogue_set_title(lua_State *lua);
	int Dialogue_get_title(lua_State *lua);

	int Dialogue_set_id(lua_State *lua);
	int Dialogue_get_id(lua_State *lua);

	int Dialogue_set_subject(lua_State *lua);
	int Dialogue_get_subject(lua_State *lua);

	int Dialogue_set_unlock_flag(lua_State *lua);
	int Dialogue_get_unlock_flag(lua_State *lua);

	int Dialogue_set_dialogue_action(lua_State *lua);
	int Dialogue_get_dialogue_action(lua_State *lua);

	int Dialogue_add_dialogue(lua_State *lua);
	int Dialogue_remove_dialogue(lua_State *lua);
	int Dialogue_remove_all_dialogue(lua_State *lua);
	int Dialogue_get_dialogue(lua_State *lua);

	am::game::Dialogue *Check_Dialogue(lua_State *lua, int n);

	Dialogue::UnlockFlag getUnlockFlag(lua_State *lua, int n);
	Dialogue::DialogueAction getDialogueAction(lua_State *lua, int n);

}
}
}
