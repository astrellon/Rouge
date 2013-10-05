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
#include "game/lua_door.h"
#include <game/door.h>
#include "game/lua_map_region.h"
#include <game/map_region.h>
using namespace am::lua::game;

#include "gfx/lua_sprite.h"
#include <gfx/gfx_sprite.h>
#include "gfx/lua_asset.h"
#include <gfx/gfx_asset.h>
#include "gfx/lua_texture.h"
#include <gfx/gfx_texture.h>
#include "gfx/lua_gfx_engine.h"
#include <gfx/gfx_engine.h>
#include "gfx/lua_font.h"
#include <gfx/gfx_font.h>
using namespace am::lua::gfx;

#include "sfx/lua_sound.h"
#include <sfx/sfx_isound.h>
using namespace am::lua::sfx;

#include "util/lua_data_table.h"
#include <util/data_table.h>

#include "lua_return_codes.h"

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace lua {
namespace wrapper {

	void addWrapper(lua_State *lua, const char *name, lua_CFunction call, int id)
	{
		lua_pushstring(lua, name);
		call(lua);
		lua_settable(lua, -3);
		LuaState::registerWrapper(name, id);
	}

	void AssignWrappers(lua_State *lua, int tableRef)
	{
		lua_rawgeti(lua, LUA_REGISTRYINDEX, tableRef);
		// am.game
		addWrapper(lua, "character", Character_register, Character::LUA_ID);
		addWrapper(lua, "stats", Stats_register, Stats::LUA_ID);
		addWrapper(lua, "body_part", BodyPart_register, BodyPart::LUA_ID);
		addWrapper(lua, "stat_modifiers", StatModifiers_register, StatModifiers::LUA_ID);
		addWrapper(lua, "stat_modifier", StatModifier_register, StatModifier::LUA_ID);
		addWrapper(lua, "item", Item_register, Item::LUA_ID);
		addWrapper(lua, "inventory", Inventory_register, Inventory::LUA_ID);
		addWrapper(lua, "map", Map_register, Map::LUA_ID);
		addWrapper(lua, "tile", Tile_register, Tile::LUA_ID);
		addWrapper(lua, "tile_set", TileSet_register, TileSet::LUA_ID);
		addWrapper(lua, "tile_type", TileType_register, TileType::LUA_ID);
		addWrapper(lua, "tile_instance", TileInstance_register, TileInstance::LUA_ID);
		addWrapper(lua, "engine", Engine_register, Engine::LUA_ID);
		addWrapper(lua, "game", Game_register, Game::LUA_ID);
		addWrapper(lua, "coin_purse", CoinPurse_register, CoinPurse::LUA_ID);
		addWrapper(lua, "quest", Quest_register, Quest::LUA_ID);
		addWrapper(lua, "dialogue", Dialogue_register, Dialogue::LUA_ID);
		addWrapper(lua, "race", Race_register, Race::LUA_ID);
		addWrapper(lua, "dialogue_component", DialogueComponent_register, DialogueComponent::LUA_ID);
		addWrapper(lua, "door", Door_register, Door::LUA_ID);
		addWrapper(lua, "map_region", MapRegion_register, MapRegion::LUA_ID);
		// am.gfx
		addWrapper(lua, "sprite", Sprite_register, Sprite::LUA_ID);
		addWrapper(lua, "asset", Asset_register, Asset::LUA_ID);
		addWrapper(lua, "texture", Texture_register, Texture::LUA_ID);
		addWrapper(lua, "gfx_engine", GfxEngine_register, GfxEngine::LUA_ID);
		addWrapper(lua, "font", Font_register, Font::LUA_ID);
		// am.util.data
		addWrapper(lua, "data_table", am::lua::util::data::DataTable_register, data::Table::LUA_ID);
		//am.sfx
		addWrapper(lua, "sound", Sound_register, am::sfx::ISound::LUA_ID);

		ReturnCodes_register(lua);

		lua_pop(lua, 1);

	}
	
}
}
}
