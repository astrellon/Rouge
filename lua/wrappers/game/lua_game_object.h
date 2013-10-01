#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
} 

namespace am {
namespace game {
	class GameObject;
}
using namespace am::game;

namespace lua {
namespace game {

	int GameObject_name(lua_State *lua, GameObject *obj);
	int GameObject_description(lua_State *lua, GameObject *obj);
	
	// GameObject methods
	int GameObject_location(lua_State *lua, GameObject *obj);
	int GameObject_grid_location(lua_State *lua, GameObject *obj);

	int GameObject_move(lua_State *lua, GameObject *obj);
	int GameObject_move_grid(lua_State *lua, GameObject *obj);

	int GameObject_talk_to(lua_State *lua, GameObject *obj);

	int GameObject_fixed_to_grid(lua_State *lua, GameObject *obj);
	int GameObject_map(lua_State *lua, GameObject *obj);
	int GameObject_original_map(lua_State *lua, GameObject *obj);

	int GameObject_add_passible_type(lua_State *lua, GameObject *obj);
	int GameObject_remove_passible_type(lua_State *lua, GameObject *obj);
	int GameObject_remove_all_passible_types(lua_State *lua, GameObject *obj);
	int GameObject_has_passible_type(lua_State *lua, GameObject *obj);
	int GameObject_get_passible_types(lua_State *lua, GameObject *obj);

	int GameObject_dialogue_component(lua_State *lua, GameObject *obj);
	int GameObject_game_id(lua_State *lua, GameObject *obj);

	int GameObject_add_event_listener(lua_State *lua, GameObject *obj);
	int GameObject_remove_event_listener(lua_State *lua, GameObject *obj);
	int GameObject_has_event_listener(lua_State *lua, GameObject *obj);

	int GameObject_attrs(lua_State *lua, GameObject *obj);

}
}
}
