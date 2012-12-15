#include "lua_wrappers.h"

#include "game/lua_character.h"
//#include "lua_event_manager.h"
#include "game/lua_stats.h"
#include "game/lua_body_part.h"
#include "game/lua_stat_modifiers.h"
#include "game/lua_item.h"
#include "game/lua_inventory.h"
#include "game/lua_map.h"
#include "game/lua_tile.h"
#include "game/lua_tile_type.h"
#include "game/lua_tile_set.h"
#include "game/lua_tile_instance.h"
#include "game/lua_engine.h"
#include "game/lua_game.h"
#include "game/lua_quest.h"
#include "game/lua_coin_purse.h"
#include "game/lua_dialogue.h"
#include "game/lua_race.h"
using namespace am::lua::game;

#include "gfx/lua_sprite.h"
using namespace am::lua::gfx;

#include <lua/lua_state.h>
using namespace am::lua;

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
		LuaState::registerWrapper("Race", Race_register);

		LuaState::registerWrapper("Sprite", Sprite_register);

		//LuaState::registerWrapper("EventManager", am::lua::ui::EventManager_register);
	}
	
}
}
}
