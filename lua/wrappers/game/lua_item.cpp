#include "lua_item.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/item.h>
#include <game/engine.h>
using namespace am::game;

#include "lua_stat_modifiers.h"

#include <log/logger.h>

namespace am {
namespace lua {
namespace game {

	int Item_ctor(lua_State *lua)
	{
		Item *item = new Item();
		
		wrapRefObject<Item>(lua, item);
		return 1;
	}
	
	int Item_dtor(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			item->release();
		}
		return 0;
	}

	int Item_eq(lua_State *lua)
	{
		Item *lhs = castUData<Item>(lua, 1);
		Item *rhs = castUData<Item>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Item_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Item_ctor },
			{ "__gc",  Item_dtor },
			{ "__eq", Item_eq },
			// Item methods
			{ "set_graphic", NULL },
			{ "get_graphic", NULL },
			{ "set_ground_graphic", NULL },
			{ "get_ground_graphic", NULL },
			{ "set_item_type", Item_set_item_type },
			{ "get_item_type", Item_get_item_type },
			{ "set_inventory_size", Item_set_inventory_size },
			{ "get_inventory_size", Item_get_inventory_size },
			{ "set_item_location", Item_set_item_location },
			{ "get_item_location", Item_get_item_location },
			{ "set_quest_item_id", Item_set_quest_item_id },
			{ "get_quest_item_id", Item_get_quest_item_id },
			{ "is_quest_item", Item_is_quest_item },
			{ "set_item_name", Item_set_item_name },
			{ "get_item_name", Item_get_item_name },
			{ "set_prefix", Item_set_prefix },
			{ "get_prefix", Item_get_prefix },
			{ "set_postfix", Item_set_postfix },
			{ "get_postfix", Item_get_postfix },
			{ "set_item_fullname", Item_set_item_fullname },
			{ "get_item_fullname", Item_get_item_fullname },
			{ "set_item_from", Item_set_item_from },
			{ "get_stat_modifiers", Item_get_stat_modifiers },
			{ "load_def", Item_load_def },
			{ "get_width", Item_get_width },
			{ "get_height", Item_get_height },
			{ "get_name", Item_get_name },
			// GameObject methods
			{ "set_location", Item_set_location },
			{ "get_location", Item_get_location },
			{ "set_grid_location", Item_set_grid_location },
			{ "get_grid_location", Item_get_grid_location },
			{ "talk_to", NULL },
			{ "move", NULL },
			{ "move_grid", NULL },
			{ "set_fixed_to_grid", NULL },
			{ "is_fixed_to_grid", NULL },
			{ "set_only_on_passable", NULL },
			{ "is_only_on_passable", NULL },
			{ "set_camera_offset", NULL },
			{ "get_camera_offset", NULL },
			{ "set_map", NULL },
			{ "get_map", NULL },
			{ "add_passible_type", NULL },
			{ "remove_passible_type", NULL },
			{ "remove_all_passible_type", NULL },
			{ "has_passible_type", NULL },
			{ "get_passible_types", NULL },
			{ "set_game_id", Item_set_game_id },
			{ "get_game_id", Item_get_game_id },
			// Static GameObject methods
			{ "get_by_game_id", Item_get_by_game_id },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Item::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	int Item_set_item_type(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			ItemCommon::ItemType type = ItemCommon::UNKNOWN;
			if (lua_isnumber(lua, -1))
			{
				type = ItemCommon::getItemType(lua_tointeger(lua, -1));
			}
			else if (lua_isstring(lua, -1))
			{
				type = ItemCommon::getItemType(lua_tostring(lua, -1));
			}
			if (type != ItemCommon::UNKNOWN)
			{
				item->setItemType(type);
			}
		}
		return 0;
	}
	int Item_get_item_type(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			const char *name = item->getItemTypeName();
			if (name != NULL)
			{
				lua_pushstring(lua, name);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	int Item_set_inventory_size(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			short width = static_cast<short>(lua_tointeger(lua, -2));
			short height = static_cast<short>(lua_tointeger(lua, -1));
			item->setInventorySize(width, height);
		}
		return 0;
	}
	int Item_get_inventory_size(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushinteger(lua, item->getInventorySizeX());
			lua_pushinteger(lua, item->getInventorySizeY());
			return 2;
		}
		lua_pushnil(lua);
		lua_pushnil(lua);
		return 2;
	}

	int Item_set_item_location(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			Item::ItemLocation location = Item::MAX_LENGTH;
			if (lua_isnumber(lua, -1))
			{
				location = Item::getItemLocationType(lua_tointeger(lua, -1));
			}
			else if (lua_isstring(lua, -1))
			{
				location = Item::getItemLocationType(lua_tostring(lua, -1));
			}
			if (location != Item::MAX_LENGTH)
			{
				item->setItemLocation(location);
			}
		}
		return 0;
	}
	int Item_get_item_location(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			const char *name = item->getItemLocationTypeName(item->getItemLocation());
			if (name != NULL)
			{
				lua_pushstring(lua, name);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	int Item_set_quest_item_id(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item && lua_isnumber(lua, -1))
		{
			item->setQuestItemId(lua_tointeger(lua, -1));
		}
		return 0;
	}
	int Item_get_quest_item_id(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushinteger(lua, item->getQuestItemId());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Item_is_quest_item(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushboolean(lua, item->isQuestItem());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Item_set_item_name(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item && lua_isstring(lua, -1))
		{
			item->setItemName(lua_tostring(lua, -1));
		}
		return 0;
	}
	int Item_get_item_name(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushstring(lua, item->getItemName().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Item_set_prefix(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item && lua_isstring(lua, -1))
		{
			item->setPrefix(lua_tostring(lua, -1));
		}
		return 0;
	}
	int Item_get_prefix(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushstring(lua, item->getPrefix().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Item_set_postfix(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item && lua_isstring(lua, -1))
		{
			item->setPostfix(lua_tostring(lua, -1));
		}
		return 0;
	}
	int Item_get_postfix(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushstring(lua, item->getPostfix().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Item_set_item_fullname(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			int args = lua_gettop(lua);
			if (args == 2 && lua_tostring(lua, -1))
			{
				item->setItemFullname(lua_tostring(lua, -1));
			}
			else if (args == 3 && lua_tostring(lua, -2) && lua_tostring(lua, -1))
			{
				item->setItemFullname(lua_tostring(lua, -2), lua_tostring(lua, -1));
			}
			else if (args == 4 && lua_tostring(lua, -3) && lua_tostring(lua, -2) && lua_tostring(lua, -1))
			{
				item->setItemFullname(lua_tostring(lua, -3), lua_tostring(lua, -2), lua_tostring(lua, -1));
			}
		}
		return 0;
	}
	int Item_get_item_fullname(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushstring(lua, item->getFullItemName().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Item_set_item_from(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		Item *other = castUData<Item>(lua, 2);
		if (item && other)
		{
			item->setItemFrom(*other);
		}
		return 0;
	}

	int Item_get_stat_modifiers(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			wrapObject<StatModifiers>(lua, &item->getStatModifiers());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Item_load_def(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item && lua_istable(lua, -1))
		{
			LuaState wrap(lua);
			item->loadDef(wrap);
			lua_pushboolean(lua, true);
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}

	int Item_get_width(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushnumber(lua, item->getWidth());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Item_get_height(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushnumber(lua, item->getHeight());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Item_get_name(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushstring(lua, item->getName().c_str());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Item_set_location(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			item->setLocation(lua_tofloat(lua, -2), lua_tofloat(lua, -1));
		}
		return 0;
	}
	int Item_get_location(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushnumber(lua, item->getLocationX());
			lua_pushnumber(lua, item->getLocationY());
			return 2;
		}
		lua_pushnil(lua);
		lua_pushnil(lua);
		return 2;
	}

	int Item_set_grid_location(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			item->setGridLocation(lua_tointeger(lua, -2), lua_tointeger(lua, -1));
		}
		return 0;
	}
	int Item_get_grid_location(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushinteger(lua, item->getGridLocationX());
			lua_pushinteger(lua, item->getGridLocationY());
			return 2;
		}
		lua_pushnil(lua);
		lua_pushnil(lua);
		return 2;
	}

	int Item_set_game_id(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item && lua_isstring(lua, -1))
		{
			lua_pushboolean(lua, item->setGameId(lua_tostring(lua, -1)));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Item_get_game_id(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushstring(lua, item->getGameId());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Item_get_by_game_id(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			am::game::Item *item = dynamic_cast<Item *>(Engine::getEngine()->getGameObject(lua_tostring(lua, -1)));
			if (item)
			{
				wrapRefObject<Item>(lua, item);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
