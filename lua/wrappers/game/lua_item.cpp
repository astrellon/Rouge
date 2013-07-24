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
#include <game/game.h>
using namespace am::game;

#include "lua_stat_modifiers.h"
#include "lua_iattribute_data.h"

#include <log/logger.h>

#include <sstream>
using std::stringstream;

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * TheiItem class represents a single item within the game.
	 * All items are individual and are not dependant on other
	 * item's or an item definition. Items can be cloned and have their values
	 * set from another item however if a definition like system is required.
	 */
	/**
	 * Creates a new item instance, it will have the name "Item" and will take up
	 * 1x1 inventory slots.
	 */
	int Item_ctor(lua_State *lua)
	{
		Item *item = new Item();
		
		wrapRefObject<Item>(lua, item);
		return 1;
	}
	/**
	 * Releases the reference counter on this item.
	 */
	int Item_dtor(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			item->release();
			return 0;
		}
		return LuaState::expectedContext(lua, "__gc", "am.item");
	}
	/**
	 * Returns true if the given item is the same as this item.
	 *
	 * @param am.item rhs The other item to compare with.
	 * @returns boolean True if they are the same item.
	 */
	int Item_eq(lua_State *lua)
	{
		Item *lhs = castUData<Item>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.item");
		}
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
			{ "from_def", Item_from_def },
			// Item methods
			{ "clone", Item_clone },
			{ "graphic", Item_graphic },
			{ "ground_graphic", Item_ground_graphic },
			{ "item_type", Item_item_type },
			{ "inventory_size", Item_inventory_size },
			{ "item_location", Item_item_location },
			{ "quest_item_id", Item_quest_item_id },
			{ "is_quest_item", Item_is_quest_item },
			{ "item_name", Item_item_name },
			{ "prefix", Item_prefix },
			{ "postfix", Item_postfix },
			{ "item_fullname", Item_item_fullname },
			{ "set_item_from", Item_set_item_from },
			{ "stat_modifiers", Item_stat_modifiers },
			{ "width", Item_width },
			{ "height", Item_height },
			{ "name", Item_name },
			// GameObject methods
			{ "location", Item_location },
			{ "grid_location", Item_grid_location },
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
			{ "game_id", Item_game_id },
			// Static GameObject methods
			{ "find", Item_find },
			{ "attrs", Item_attrs },
			{ "add_body_type", Item_add_body_type },
			{ "remove_body_type", Item_remove_body_type },
			{ "has_body_type", Item_has_body_type },
			{ "get_body_types", Item_get_body_types },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Item::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	/**
	 * @static
	 * Creates a new item from the item definition.
	 * Item definitions are automatically loaded if one with
	 * the given name is not registered.
	 * <p>Example (a test map Lua file):</p>
	 * <pre>
	 * item = am.item.from_def("wooden:sword")
	 * if (item ~= nil)
	 *     item:name("Fred's Sword")
	 * end
	 * </pre>
	 * <p>In "data/defs/wooden.lua":</p>
	 * <pre>
	 * game = am.engine.game()
	 * 
	 * item = am.item.new()
	 * item:item_type("sword")
	 *     :graphic(am.sprite.new("item/sword"))
	 *     :groundGraphic(am.sprite.new("item/swordGround"))
	 * 
	 * -- Here the npc is registered with the name "sword" and "wooden:" will 
	 * -- automatically be prepended because of the filename.
	 * game:item_def("sword", item) 
	 * </pre>
	 *
	 * @param string def_name The name of the item definition.
	 * @returns Item A copy of the item from the given definition, or nil if no definition was found.
	 */
	int Item_from_def(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (lua_isstr(lua, 1) && (args == 1 || args > 1 && lua_isstr(lua, 2)))
		{
			Game *game = Engine::getGame();
			if (game)
			{
				Item *def = game->getItemDefinition(lua_tostring(lua, 1));
				if (def)
				{
					Item *newItem = new Item(*def);
					wrapRefObject<Item>(lua, newItem);
					return 1;
				}
				stringstream ss;
				ss << "No item with the definition '" << lua_tostring(lua, 1) << "' was found.";
				LuaState::warning(lua, ss.str().c_str());
				lua_pushnil(lua);
				return 1;
			}
			stringstream ss;
			ss << "Unable to get definition (" << lua_tostring(lua, 1) << " when a game is not running";
			LuaState::warning(lua, ss.str().c_str());
			lua_pushnil(lua);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@from_def", "string def_name");
	}

	/**
	 * Creates a copy of this item, will create
	 * a new instance of the sprites used for the graphics.
	 *
	 * @returns am.item The newly cloned item.
	 */
	int Item_clone(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			Item *newItem = item->clone();
			wrapRefObject<Item>(lua, newItem);
			return 1;
		}
		return LuaState::expectedContext(lua, "clone", "am.item");
	}
	/**
	 * Returns the graphic used to display the item. This
	 * graphic is used to display the item on the inventory screen
	 * and will also be used as the ground graphic if one
	 * is not set.
	 *
	 * @returns am.sprite The graphic for the item, can be nil.
	 */
	/**
	 * Sets the graphic for the item, can be set to nil.
	 *
	 * @param am.sprite graphic The new graphic for the item.
	 * @param boolean [false] calc_size Calculates the inventory size
	 *  of the item based off the graphic size. This is based off the
	 *  the size of the item compared to the size of each inventory slot size.
	 * @returns am.item This
	 */
	int Item_graphic(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_gettop(lua) == 1)
			{
				Sprite *graphic = item->getGraphic();
				if (graphic)
				{
					wrapRefObject<Sprite>(lua, graphic);
					return 1;
				}
				lua_pushnil(lua);
				return 1;
			}
			else if (lua_isnil(lua, 2))
			{
				item->setGraphic(NULL);
			}
			else
			{
				Sprite *graphic = castUData<Sprite>(lua, 2);
				int args = lua_gettop(lua);
				if (graphic && (args == 2 || (args == 3 && lua_isbool(lua, 3))))
				{
					if (args == 3)
					{
						item->setGraphic(graphic, lua_tobool(lua, 3));
					}
					else
					{
						item->setGraphic(graphic);
					}
					lua_first(lua);
				}
			}
			return LuaState::expectedArgs(lua, "graphic", 2, "am.sprite graphic, bool [true] calc_size", "nil graphic");
		}
		return LuaState::expectedContext(lua, "graphic", "am.item");
	}
	/**
	 * Returns the graphic used to display the item on the ground. This
	 * graphic is used to display the item when it is on the ground
	 * and will also be used as the inventory graphic if one
	 * is not set.
	 *
	 * @returns am.sprite The ground graphic for the item, can be nil.
	 */
	/**
	 * Sets the ground graphic for the item, can be set to nil.
	 *
	 * @param am.sprite graphic The new ground graphic for the item.
	 * @returns am.item This
	 */
	int Item_ground_graphic(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_gettop(lua) == 1)
			{
				Sprite *graphic = item->getGroundGraphic();
				if (graphic)
				{
					wrapRefObject<Sprite>(lua, graphic);
					return 1;
				}
			}
			else if (lua_isnil(lua, 2))
			{
				item->setGroundGraphic(NULL);
				lua_first(lua);
			}
			else
			{
				Sprite *graphic = castUData<Sprite>(lua, 2);
				if (graphic)
				{
					item->setGroundGraphic(graphic);
					lua_first(lua);
				}
			}
			return LuaState::expectedArgs(lua, "ground_graphic", 2, "am.sprite graphic", "nil graphic");
		}
		return LuaState::expectedContext(lua, "ground_graphic", "am.item");
	}
	/**
	 * Returns the item type. The item type defines
	 * what special properties the item has, such as if it
	 * is coin or a potion.
	 *
	 * @returns string The item type.
	 */
	/**
	 * Sets the item type, can be one of the following values:
	 * <ul>
	 * <li>helm</li><li>body</li><li>sword</li><li>axe</li><li>bow</li>
	 * <li>ring</li><li>amulet</li><li>leggins</li><li>boots</li><li>shield</li>
	 * <li>potion</li><li>gold</li><li>unknown</li>
	 * </ul>
	 *
	 * @param string item_type The item type to set on this item.
	 * @returns am.item This
	 */
	int Item_item_type(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_gettop(lua) == 1)
			{
				const char *name = item->getItemTypeName();
				if (name != NULL)
				{
					lua_pushstring(lua, name);
					return 1;
				}
				lua_pushstring(lua, "");
				return 1;
			}
			else
			{
				ItemCommon::ItemType type = ItemCommon::UNKNOWN;
				if (lua_isnum(lua, 2))
				{
					type = ItemCommon::getItemType(lua_tointeger(lua, 2));
				}
				else if (lua_isstr(lua, 2))
				{
					type = ItemCommon::getItemType(lua_tostring(lua, 2));
				}
				else
				{
					return LuaState::expectedArgs(lua, "item_type", 2, "string type_name", "integer type_id");
				}
				if (type != ItemCommon::UNKNOWN)
				{
					item->setItemType(type);
				}
				else
				{
					stringstream ss;
					ss << "Unknown item type: ";
					LuaState::printTypeValue(lua, 2, ss);
					LuaState::warning(lua, ss.str().c_str());
				}
				lua_first(lua);
			}
		}
		return LuaState::expectedContext(lua, "item_type", "item");
	}
	/**
	 * Returns the inventory size of this item as a pair of
	 * integers in inventory size units.
	 *
	 * @returns integer The width in inventory units.
	 * @returns integer The height in inventory units.
	 */
	/**
	 * Sets size of the inventory size in inventory units.
	 *
	 * @param integer width The width in inventory units.
	 * @param integer height The height in inventory units.
	 * @returns am.item This
	 */
	int Item_inventory_size(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushinteger(lua, item->getInventorySizeX());
				lua_pushinteger(lua, item->getInventorySizeY());
				return 2;
			}
			else if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
			{
				short width = static_cast<short>(lua_tointeger(lua, 2));
				short height = static_cast<short>(lua_tointeger(lua, 3));
				item->setInventorySize(width, height);
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "inventory_size", "integer width, integer height");
		}
		return LuaState::expectedContext(lua, "inventory_size", "am.item");
	}
	/**
	 * Returns the item location, as being in either in an inventory, in the player's hand or on the ground.
	 *
	 * @return String The item location as either "ground", "inventory" or "hand"
	 */
	/**
	 * Sets the item location as being either on the ground, in an inventory or in the player's hand.
	 *
	 * @param string location The location as either "ground", "inventory" or "hand"
	 * @returns am.item This
	 */
	int Item_item_location(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_gettop(lua) == 1)
			{
				const char *name = item->getItemLocationTypeName(item->getItemLocation());
				if (name != NULL)
				{
					lua_pushstring(lua, name);
					return 1;
				}
				lua_pushnil(lua);
				return 1;
			}
			else
			{
				Item::ItemLocation location = Item::MAX_LENGTH;
				if (lua_isnum(lua, 2))
				{
					location = Item::getItemLocationType(lua_tointeger(lua, 2));
				}
				else if (lua_isstr(lua, 2))
				{
					location = Item::getItemLocationType(lua_tostring(lua, 2));
				}
				else
				{
					return LuaState::expectedArgs(lua, "item_location", 2, "string location_name", "integer location_id");
				}
				if (location != Item::MAX_LENGTH)
				{
					item->setItemLocation(location);
				}
				else
				{
					stringstream ss;
					ss << "Unknown item location: ";
					LuaState::printTypeValue(lua, 2, ss);
					LuaState::warning(lua, ss.str().c_str());
				}
				lua_first(lua);
			}
		}
		return LuaState::expectedContext(lua, "item_location", "am.item");
	}
	/**
	 * Returns the quest ID for this item.
	 * The quest ID is a unique identifier for this item, this allows
	 * this item to be kept track of. Any value above 0 is considered
	 * to be a valid id.
	 *
	 * @returns integer The quest ID of this time.
	 */
	/**
	 * Sets the quest ID, any value higher than 0 is considered a valid ID.
	 *
	 * @param integer quest_id The new quest ID.
	 * @returns am.item This
	 */
	int Item_quest_item_id(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushinteger(lua, item->getQuestItemId());
				return 1;
			}
			else if (lua_isnum(lua, 2))
			{
				item->setQuestItemId(lua_tointeger(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "quest_item_id", "integer quest_id");
		}
		return LuaState::expectedContext(lua, "quest_item_id", "am.item");
	}
	/**
	 * Returns true if this item has a valid quest ID.
	 * 
	 * @returns boolean True if this item is a quest item.
	 */
	int Item_is_quest_item(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushboolean(lua, item->isQuestItem());
			return 1;
		}
		return LuaState::expectedContext(lua, "is_quest_item", "am.item");
	}
	/**
	 * Returns the main item name.
	 * 
	 * @returns string The main item name.
	 */
	/**
	 * Sets the main item name, this goes between the prefix and the postfix.
	 *
	 * @param string name The main item name.
	 * @returns am.item This
	 */
	int Item_item_name(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, item->getItemName().c_str());
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				item->setItemName(lua_tostring(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "item_name", "string name");
		}
		return LuaState::expectedContext(lua, "item_name", "am.item");
	}
	/**
	 * Returns the item prefix, this goes before the item name, can be an empty string.
	 *
	 * @returns string The item prefix.
	 */
	/**
	 * Sets the item prefix, nil is considered to be an empty string.
	 *
	 * @param string prefix The item prefix.
	 * @returns am.item This
	 */
	int Item_prefix(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, item->getPrefix().c_str());
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				item->setPrefix(lua_tostring(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "prefix", "string prefix");
		}
		return LuaState::expectedContext(lua, "prefix", "am.item");
	}
	/**
	 * Returns the item postfix, this goes after the item name, can be an empty string.
	 *
	 * @returns string The item postfix.
	 */
	/**
	 * Sets the item postfix, nil is considered to be an empty string.
	 *
	 * @param string postfix The item postfix.
	 * @returns am.item This
	 */
	int Item_postfix(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, item->getPostfix().c_str());
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				item->setPostfix(lua_tostring(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "postfix", "string postfix");
		}
		return LuaState::expectedContext(lua, "postfix", "am.item");
	}
	/**
	 * Returns the whole item name as one string.
	 *
	 * @returns string The whole item name.
	 */
	/**
	 * Sets the whole item name, the main name, prefix and postfix.
	 *
	 * @param string main_name The main item name.
	 * @param string [""] prefix The item prefix.
	 * @param string [""] postfix The item potsfix.
	 * @returns am.item This
	 */
	int Item_item_fullname(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			int args = lua_gettop(lua);
			if (args == 1)
			{
				lua_pushstring(lua, item->getFullItemName().c_str());
				return 1;
			}
			else if (args == 2 && lua_isstr(lua, -1))
			{
				item->setItemFullname(lua_tostring(lua, -1));
			}
			else if (args == 3 && lua_isstr(lua, -2) && lua_isstr(lua, -1))
			{
				item->setItemFullname(lua_tostring(lua, -2), lua_tostring(lua, -1));
			}
			else if (args == 4 && lua_isstr(lua, -3) && lua_isstr(lua, -2) && lua_isstr(lua, -1))
			{
				item->setItemFullname(lua_tostring(lua, -3), lua_tostring(lua, -2), lua_tostring(lua, -1));
			}
			else
			{
				return LuaState::expectedArgs(lua, "item_fullname", "string main_name, string [""] prefix, string [""] postfix");
			}
			lua_first(lua);
		}
		return LuaState::expectedContext(lua, "item_fullname", "am.item");
	}
	/**
	 * Sets all the values from the given item onto this item.
	 *
	 * @param am.item other The other item to set the values from.
	 * @returns am.item This
	 */
	int Item_set_item_from(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			Item *other = castUData<Item>(lua, 2);
			if (other)
			{
				item->setItemFrom(*other);
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "set_item_from", "am.item other");
		}
		return LuaState::expectedContext(lua, "set_item_from", "am.item");
	}
	/**
	 * Returns the stats modifiers object for this item.
	 * @see am.stats_modifiers
	 *
	 * @returns am.stats_modifiers The stats modifiers object.
	 */
	int Item_stat_modifiers(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			wrapRefObject<StatModifiers>(lua, &item->getStatModifiers());
			return 1;
		}
		return LuaState::expectedContext(lua, "stat_modifiers", "am.item");
	}
	/**
	 * Returns the item width for the main graphic.
	 *
	 * @returns number The width of the item graphic.
	 */
	int Item_width(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushnumber(lua, item->getWidth());
			return 1;
		}
		return LuaState::expectedContext(lua, "width", "am.item");
	}
	/**
	 * Returns the item height for the main graphic.
	 *
	 * @returns number The height of the item graphic.
	 */
	int Item_height(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushnumber(lua, item->getHeight());
			return 1;
		}
		return LuaState::expectedContext(lua, "height", "am.item");
	}
	/**
	 * An alias for returning the full item name.
	 *
	 * @returns string The full item name.
	 */
	int Item_name(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_pushstring(lua, item->getName().c_str());
			return 1;
		}
		return LuaState::expectedContext(lua, "name", "am.item");
	}
	/**
	 * Returns the item's location.
	 *
	 * @returns number The x location.
	 * @returns number The y location.
	 */
	/**
	 * Sets the item's location for the map it is currently on.
	 *
	 * @param number location_x The x location value.
	 * @param number location_y The y location value.
	 * @returns am.item This
	 */
	int Item_location(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushnumber(lua, item->getLocationX());
				lua_pushnumber(lua, item->getLocationY());
				return 2;
			}
			else if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
			{
				item->setLocation(lua_tofloat(lua, 3), lua_tofloat(lua, 3));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "location", "number x, number y");
		}
		return LuaState::expectedContext(lua, "location", "am.item");
	}

	/**
	 * Returns the item's grid location.
	 *
	 * @returns integer The x grid location.
	 * @returns integer The y grid location.
	 */
	/**
	 * Sets the item's grid location for the map it is currently on.
	 *
	 * @param integer grid_x The x grid location value.
	 * @param integer grid_y The y grid location value.
	 * @returns am.item This
	 */
	int Item_grid_location(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushinteger(lua, item->getGridLocationX());
				lua_pushinteger(lua, item->getGridLocationY());
				return 2;
			}
			else if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
			{
				item->setGridLocation(lua_tointeger(lua, 2), lua_tointeger(lua, 3));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "grid_location", "integer x, integer y");
		}
		return LuaState::expectedContext(lua, "grid_location", "am.item");
	}
	/**
	 * Returns the item's unique game id.
	 *
	 * @returns string The item's game id.
	 */
	/**
	 * Set's a new game id for this item.
	 *
	 * @param string game_id The new game id.
	 * @returns boolean True if the item's game id was successfully changed.
	 */
	int Item_game_id(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, item->getGameId());
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				lua_pushboolean(lua, item->setGameId(lua_tostring(lua, 2)));
				return 1;
			}
			return LuaState::expectedArgs(lua, "game_id", "string game_id");
		}
		return LuaState::expectedContext(lua, "game_id", "am.item");
	}
	/**
	 * @static
	 * Looks up an item in the current game engine with the given game id.
	 *
	 * @param string game_id The game id to look up.
	 * @returns am.item The found item or nil.
	 */
	int Item_find(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			Item *item = dynamic_cast<Item *>(Engine::getEngine()->getGameObject(lua_tostring(lua, 1)));
			if (item)
			{
				wrapRefObject<Item>(lua, item);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@find", "string game_id");
	}

	/**
	 * Returns the items attribute data table.
	 * By default if no attribute data table is present nil is returned unless true
	 * is passed as the first argument, then a data table is created if one is not present.
	 * 
	 * @param boolean [false] create_table Create a data table if one didn't exist.
	 * @returns am.data_table The data table on this character.
	 */
	/**
	 * Sets the data table on this item, can be set to nil.
	 *
	 * @param am.data_table attr_table The data table to set on the item.
	 * @returns am.item This
	 */
	int Item_attrs(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			return IAttributeData_attrs(lua, item);
		}
		return LuaState::expectedContext(lua, "attrs", "am.item");
	}

	/**
	 * Adds a new body type which this item can be equipped to.
	 * If the new body type is already on the item it will be ignored.
	 * 
	 * @param string part_type The name of the body part.
	 * @returns am.item This
	 */
	int Item_add_body_type(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_isstr(lua, 2))
			{
				BodyPartType::PartType type = BodyPartType::getBodyPartType(lua_tostring(lua, 2));
				if (type != BodyPartType::MAX_BODY_TYPE_LENGTH)
				{
					item->addBodyPartType(type);
					lua_first(lua);
				}
				stringstream ss;
				ss << "Unknown body part type '" << lua_tostring(lua, 2) << '\'';
				LuaState::warning(lua, ss.str().c_str());
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "add_body_type", "string body_type");
		}
		return LuaState::expectedContext(lua, "add_body_type", "am.item");
	}

	/**
	 * Remove a new body type from this item that it can be equipped to.
	 * If the body type is not on the item, this function will be ignored.
	 * 
	 * @param string part_type The name of the body part.
	 * @returns am.item This
	 */
	int Item_remove_body_type(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_isstr(lua, 2))
			{
				BodyPartType::PartType type = BodyPartType::getBodyPartType(lua_tostring(lua, 2));
				if (type != BodyPartType::MAX_BODY_TYPE_LENGTH)
				{
					item->removeBodyPartType(type);
					lua_first(lua);
				}
				stringstream ss;
				ss << "Unknown body part type '" << lua_tostring(lua, 2) << '\'';
				LuaState::warning(lua, ss.str().c_str());
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "remove_body_type", "string body_type");
		}
		return LuaState::expectedContext(lua, "remove_body_type", "am.item");
	}

	/**
	 * Checks if a new body type is on this item.
	 * 
	 * @param string part_type The name of the body part.
	 * @returns boolean True if the body part type was found on this item.
	 */
	int Item_has_body_type(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			if (lua_isstr(lua, 2))
			{
				BodyPartType::PartType type = BodyPartType::getBodyPartType(lua_tostring(lua, 2));
				if (type != BodyPartType::MAX_BODY_TYPE_LENGTH)
				{
					lua_pushboolean(lua, item->hasBodyPartType(type));
					return 1;
				}
				stringstream ss;
				ss << "Unknown body part type '" << lua_tostring(lua, 2) << '\'';
				LuaState::warning(lua, ss.str().c_str());
				lua_pushboolean(lua, false);
				return 1;
			}
			return LuaState::expectedArgs(lua, "has_body_type", "string body_type");
		}
		return LuaState::expectedContext(lua, "has_body_type", "am.item");
	}

	/**
	 * Returns the set of all the body part types that this
	 * item can be equipped on to.
	 *
	 * @returns table A set of all the body part types.
	 */
	int Item_get_body_types(lua_State *lua)
	{
		Item *item = castUData<Item>(lua, 1);
		if (item)
		{
			lua_newtable(lua);
			const BodyPartType::TypeList &list = item->getBodyPartTypeList();
			for (size_t i = 0; i < list.size(); i++)
			{
				lua_pushstring(lua, BodyPartType::getBodyPartName(list[i]));
				lua_pushboolean(lua, true);
				lua_settable(lua, -3);
			}
			return 1;
		}
		return LuaState::expectedContext(lua, "get_body_types", "am.item");
	}

}
}
}
