#include "lua_wrappers.h"

#include "game/lua_character.h"
#include <game/character.h>
#include "game/lua_stats.h"
#include <game/stats.h>
#include "game/lua_body_part.h"
#include <game/body_part.h>
#include "game/lua_stat_modifiers.h"
#include <game/stat_modifiers.h>
#include "game/lua_stat_modifier.h"
#include <game/stat_modifier.h>
#include "game/lua_item.h"
#include <game/item.h>
#include "game/lua_inventory.h"
#include <game/inventory.h>
#include "game/lua_map.h"
#include <game/map.h>
#include "game/lua_tile.h"
#include <game/tile.h>
#include "game/lua_tile_type.h"
#include <game/tile_type.h>
#include "game/lua_tile_set.h"
#include <game/tile_set.h>
#include "game/lua_tile_instance.h"
#include <game/tile_instance.h>
#include "game/lua_engine.h"
#include <game/engine.h>
#include "game/lua_game.h"
#include <game/game.h>
#include "game/lua_quest.h"
#include <game/quest.h>
#include "game/lua_coin_purse.h"
#include <game/coin_purse.h>
#include "game/lua_dialogue.h"
#include <game/dialogue.h>
#include "game/lua_race.h"
#include <game/race.h>
#include "game/lua_dialogue_component.h"
#include <game/dialogue_component.h>
using namespace am::lua::game;

#include "gfx/lua_sprite.h"
#include <gfx/gfx_sprite.h>
using namespace am::lua::gfx;

#include "util/lua_data_table.h"
#include <util/data_table.h>

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace lua {
namespace wrapper {

	void AssignWrappers(lua_State *lua)
	{
		LuaState::registerWrapper("Character", Character_register, Character::LUA_ID);
		LuaState::registerWrapper("Stats", Stats_register, Stats::LUA_ID);
		LuaState::registerWrapper("BodyPart", BodyPart_register, BodyPart::LUA_ID);
		LuaState::registerWrapper("StatModifiers", StatModifiers_register, StatModifiers::LUA_ID);
		LuaState::registerWrapper("StatModifier", StatModifier_register, StatModifier::LUA_ID);
		LuaState::registerWrapper("Item", Item_register, Item::LUA_ID);
		LuaState::registerWrapper("Inventory", Inventory_register, Inventory::LUA_ID);
		LuaState::registerWrapper("Map", Map_register, Map::LUA_ID);
		LuaState::registerWrapper("Tile", Tile_register, Tile::LUA_ID);
		LuaState::registerWrapper("TileSet", TileSet_register, TileSet::LUA_ID);
		LuaState::registerWrapper("TileType", TileType_register, TileType::LUA_ID);
		LuaState::registerWrapper("TileInstance", TileInstance_register, TileInstance::LUA_ID);
		LuaState::registerWrapper("Engine", Engine_register, Engine::LUA_ID);
		LuaState::registerWrapper("Game", Game_register, Game::LUA_ID);
		LuaState::registerWrapper("CoinPurse", CoinPurse_register, CoinPurse::LUA_ID);
		LuaState::registerWrapper("Quest", Quest_register, Quest::LUA_ID);
		LuaState::registerWrapper("Dialogue", Dialogue_register, Dialogue::LUA_ID);
		LuaState::registerWrapper("Race", Race_register, Race::LUA_ID);
		LuaState::registerWrapper("DialogueComponent", DialogueComponent_register, DialogueComponent::LUA_ID);

		LuaState::registerWrapper("Sprite", Sprite_register, Sprite::LUA_ID);

		//LuaState::registerWrapper("DataMap", am::lua::util::data::DataMap_register, data::Map::LUA_ID);
		//LuaState::registerWrapper("DataArray", am::lua::util::data::DataArray_register, data::Array::LUA_ID);
		LuaState::registerWrapper("DataTable", am::lua::util::data::DataTable_register, data::Table::LUA_ID);
	}
	
}
}
}
