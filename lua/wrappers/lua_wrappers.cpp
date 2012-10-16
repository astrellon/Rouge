#include "lua_wrappers.h"

#include "lua_character.h"
#include "lua_event_manager.h"
#include "lua_stats.h"
#include "lua_body_part.h"
#include "lua_stat_modifiers.h"
#include "lua_item.h"
#include "lua_inventory.h"
#include "lua_map.h"
#include "lua_tile.h"
#include "lua_tile_type.h"
#include "lua_tile_set.h"
#include "lua_engine.h"
#include "lua_game.h"

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace lua {
namespace wrapper {

	void AssignWrappers(lua_State *lua)
	{
		LuaState::registerWrapper("Character", am::lua::game::Character_register);
		LuaState::registerWrapper("Stats", am::lua::game::Stats_register);
		LuaState::registerWrapper("BodyPart", am::lua::game::BodyPart_register);
		LuaState::registerWrapper("StatModifiers", am::lua::game::StatModifiers_register);
		LuaState::registerWrapper("Item", am::lua::game::Item_register);
		LuaState::registerWrapper("Inventory", am::lua::game::Inventory_register);
		LuaState::registerWrapper("Map", am::lua::game::Map_register);
		LuaState::registerWrapper("Tile", am::lua::game::Tile_register);
		LuaState::registerWrapper("TileSet", am::lua::game::TileSet_register);
		LuaState::registerWrapper("TileType", am::lua::game::TileType_register);
		LuaState::registerWrapper("Engine", am::lua::game::Engine_register);
		LuaState::registerWrapper("Game", am::lua::game::Game_register);

		LuaState::registerWrapper("EventManager", am::lua::ui::EventManager_register);
	}
	
}
}
}
