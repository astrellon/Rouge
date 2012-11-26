#include "lua_wrappers.h"

#include "lua_character.h"
//#include "lua_event_manager.h"
#include "lua_stats.h"
#include "lua_body_part.h"
#include "lua_stat_modifiers.h"
#include "lua_item.h"
#include "lua_inventory.h"
#include "lua_map.h"
#include "lua_tile.h"
#include "lua_tile_type.h"
#include "lua_tile_set.h"
#include "lua_tile_instance.h"
#include "lua_engine.h"
#include "lua_game.h"
#include "lua_quest.h"
#include "lua_coin_purse.h"
#include "lua_dialogue.h"

#include <lua/lua_state.h>
using namespace am::lua;

using namespace am::lua::game;

namespace am {
namespace lua {
namespace wrapper {

	void AssignWrappers(lua_State *lua)
	{
		LuaState::registerWrapper("Character", Character_register);
		LuaState::registerWrapper("Stats", Stats_register);
		LuaState::registerWrapper("BodyPart", BodyPart_register);
		LuaState::registerWrapper("StatModifiers", StatModifiers_register);
		LuaState::registerWrapper("Item", Item_register);
		LuaState::registerWrapper("Inventory", Inventory_register);
		LuaState::registerWrapper("Map", Map_register);
		LuaState::registerWrapper("Tile", Tile_register);
		LuaState::registerWrapper("TileSet", TileSet_register);
		LuaState::registerWrapper("TileType", TileType_register);
		LuaState::registerWrapper("TileInstance", TileInstance_register);
		LuaState::registerWrapper("Engine", Engine_register);
		LuaState::registerWrapper("Game", Game_register);
		LuaState::registerWrapper("CoinPurse", CoinPurse_register);
		LuaState::registerWrapper("Quest", Quest_register);
		LuaState::registerWrapper("Dialogue", Dialogue_register);

		//LuaState::registerWrapper("EventManager", am::lua::ui::EventManager_register);
	}
	
}
}
}
